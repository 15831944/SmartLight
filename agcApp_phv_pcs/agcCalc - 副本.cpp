#include "agcCalc.h"
#include "gSystemDef.h"
#include <string>
#include <math.h>
#include <time.h>


#ifdef WINDOWS_ENV
#pragma warning(push)
#pragma warning(disable: 4996) 
#endif
agcCalc *g_agcCalc=NULL;
agcCalc::agcCalc(void)
{
	m_eCalcThreadStatus=eThreadInitialize;
	m_bThreadRunning=false;
	m_bNeedStopProcess =false;
	g_agcCalc=this;

	m_bFistStartFlg=true;
	time(&m_timFirstStart);
	m_1minrectar=-99;
	c_AdMaxfactor=0.85f;
	time(&m_t1minstmp);
	m_pAgcRtdbSaveAnalog=NULL;
	m_pDeleteLog=NULL;
	m_targetOld=0;
}

agcCalc::~agcCalc(void)
{
	if (m_pGenInfo != NULL)
	{
		for(int i = 0 ; i < m_nGenCount ; i++)
		{
			delete m_pGenInfo[i];
		}
		delete [] m_pGenInfo;
		m_pGenInfo=NULL;
	}

	if(m_pSocInfo!=NULL)
		delete m_pSocInfo;
	m_pSocInfo=NULL;

}
bool agcCalc::Start()
{

	m_bThreadRunning=true;
	bool bRet = CreateThreadEx(CalcThreadProc, (LPVOID)this);
	if( bRet == false )
	{
		m_eCalcThreadStatus = eThreadError;
		return false;
	}
	else
	{
		m_eCalcThreadStatus = eThreadRun;
	}

	bRet = CreateThreadEx(FreshThreadProcAux, (LPVOID)this);
	if( bRet == false )
	{
		m_eCalcThreadStatus = eThreadError;
		return false;
	}

	m_funcRtdb.clearAgcLog();

	bool bRet2 = CreateThreadEx(UpdataProcessPara,this);

	int iOpen = m_funcRtdb.getCommAdParam(eAgcParamAGCDataLog);
	if(iOpen == 1)
	{
		CMultiDataBase * pMultiDB = getFuncAgcRtdb()->getMainDb();
		if(pMultiDB != NULL)
		{
			m_pAgcRtdbSaveAnalog = new CRtdbTabSave();
			m_pAgcRtdbSaveAnalog->InitDb(pMultiDB,"AGC");		
			m_pAgcRtdbSaveAnalog->AddSaveCfg("Analog","fVal");		
			m_pAgcRtdbSaveAnalog->AddSaveCfg("Digit","iVal");
			const char *pWorkPath=getenv(HOME_NAME);
			char cPath[200] = {0};
			sprintf(cPath,"%s%s", pWorkPath, agcDataLogPath);
			m_pAgcRtdbSaveAnalog->SetPath(cPath);
			int savePeriod = m_funcRtdb.getCommAdParam(eAgcParamAGCDataLogPeriod);
			if(savePeriod < 1)
			{
				savePeriod = 5; //Ĭ��5S����һ��
			}
			m_pAgcRtdbSaveAnalog->SetDelay(savePeriod);
			m_pAgcRtdbSaveAnalog->Start();

			memset(m_LastError,NULL,sizeof(m_LastError));
			sprintf(m_LastError ,"���ݼ�¼���ܿ���,���%d��,����ң���ң�ű�\n",savePeriod);			
			writeBuffer(m_LastError);

		}
		else
		{
			printf("CMultiDataBase not init\r\n");
		}
	}

	{ //����ɾ��n����ǰ����־
		m_pDeleteLog = new DeleteLog();
		m_pDeleteLog->setLogLastDay(45); //ɾ��45����ǰ����־��ֻɾ���ӵ�ǰʱ�����𣬵�45�����־�ļ�
		int n_secs=60;
		m_pDeleteLog->setInterval(n_secs*1000);//60��ִ��һ��
		const char *pWorkPath=getenv(HOME_NAME);
		char cPath[200] = {0};
		sprintf(cPath,"%s%s", pWorkPath, agcDataLogPath);
		char cPathLog[200] = {0};
		sprintf(cPathLog,"%s%s", cPath, "####-@@-$$-agclogfile.log");//ʹ��####�滻�ļ�������ߵ��꣬@@�滻�ļ�������ߵ��£�$$�滻�ļ�������ߵ�����
		m_pDeleteLog->addFile(cPathLog);//���Ҫɾ������־�ļ���Ŀ¼���ļ���
		memset(cPathLog,0,sizeof(cPathLog));
		sprintf(cPathLog,"%s%s", cPath, "####-@@-$$_AGC_Analog.txt");
		m_pDeleteLog->addFile(cPathLog);
		memset(cPathLog,0,sizeof(cPathLog));
		sprintf(cPathLog,"%s%s", cPath, "####-@@-$$_AGC_Digit.txt");
		m_pDeleteLog->addFile(cPathLog);
		memset(m_LastError,NULL,sizeof(m_LastError));
		sprintf(m_LastError ,"����ɾ�����ܿ���,���%d��,ɾ��%d��ǰ��¼\n",n_secs,45);			
		writeBuffer(m_LastError);
		m_pDeleteLog->start();
	}
	m_funcRtdb.initAgcAlarmValue();
	return true;
}
// �����߳�
ThreadFuncRet agcCalc::CalcThreadProc(LPVOID lpParameter)
{
	agcCalc *agc = (agcCalc*)lpParameter;
	if(agc == NULL)
		return 0;

	//��ӡ
	int n_calInterval=0;
	agc->m_bNeedStopProcess = true;//��һ�����е�����ִ��
	while(agc->m_bThreadRunning == true)
	{		
		n_calInterval=agc->m_funcRtdb.getCommAdParam(eAgcParamCalcInterval);

		if(agc->waitForTimeout(CALC_TH_KeyID,n_calInterval) == false)
		{
			//TRACE("waitForTimeout break \n");
			break;
		}		

		agc->m_bNeedStopProcess = false; 

		agc->freshdata();

		if(agc->checkIsMaster()!=true)
		{
			continue;
		}

		if(agc->isLaunched() == false)
		{	
			continue;
		}

		//agc->getCommand();
		if(agc->isLocked()) // null function
		{

			continue;
		}

		if(agc->isSafeConstraint()) // null function
		{

			continue;
		}

		agc->distribute();
		agc->execResults();
		//��ӡһ��AGC�������;


	}
	TRACE("---calc thread exit----\n");
	TRACE("---calc thread exit----\n");
	return 0;
}
bool agcCalc::Init()
{
	bool ret=false;
	if(true!=m_funcRtdb.rtdb_init()) 
	{
		TRACE("init agc rtdb failed \n");
		return ret;
	}
	m_funcRtdb.setMsgHandler(ProcessMsg);

	m_adParameter.setRtdbPointer(&m_funcRtdb);

	SYSTEMTIME  timeNow;
	GetLocalTime(timeNow);


	m_fTotalInstCap=m_funcRtdb.getAgcAnalog(eAgcAnalogCapicity);

	//��ʹ������
	m_nGenCount=m_funcRtdb.getAgcDb()->getRecordCountByID(AGC_TAB_PhvUnit_ID);

	m_pGenInfo=new GeneratorUnit*[m_nGenCount];
	AGC_PhvUnit* pRecord = (AGC_PhvUnit*)m_funcRtdb.getAgcDb()->getDataAddress(AGC_TAB_PhvUnit_ID);
	for(int i=0;i<m_nGenCount;i++)
	{
		GeneratorUnit *p = new GeneratorUnit();
		m_pGenInfo[i]=p;       
		if(pRecord!=NULL)
		{
			p->p_Unit=pRecord;
			pRecord++;
		}
	}


	//��ʹ������
	m_nSocCount=m_funcRtdb.getAgcDb()->getRecordCountByID(AGC_TAB_PhvUnit_ID);
	//Ŀǰ��֧��һ������
	AGC_SocUnit* pSocRecord = (AGC_SocUnit*)m_funcRtdb.getAgcDb()->getDataAddress(AGC_TAB_SocUnit_ID);
	if(m_nSocCount>0)
	{
		m_pSocInfo= new GeSocUnit();       
		if(pSocRecord !=NULL)
		{
			m_pSocInfo->p_Unit=pSocRecord;	
		}
	}


	return true;
}
void agcCalc::ProcessMsg(tabMessage *message)
{
	TRACE("<==MT:�յ���Ϣ==>\n");
	//m_bNeedStopProcess = true; //??���ӵ��źź�ֱ����λ���¿�ʼ�����ʶ������ˢ�����ݿ�ʼ����
	switch(message->m_iMsgType) 
	{
	case mes_RCCancelSelect: //ң��
	case mes_RCSelect:
	case mes_RCExecute: 
		{			
			RControl *rc = (RControl *)(message->m_bMes);
			MesBody mesbody;
			mesbody.comm.type = MES_TYPE_RELAY;

			if (message->m_iMsgType == mes_RCExecute)
			{
				mesbody.relay.cmd = RELAY_EXECUTE;
				TRACE("<==MT:ң��ִ��==>\n");
			}
			else  
			{
				mesbody.relay.cmd = RELAY_CANCEL;
				TRACE("<==MT:ң�س���==>\n");
			}

			mesbody.comm.destination = rc->iStation;//վ��
			mesbody.relay.check = MES_CHECK_NO;
			mesbody.relay.point = rc->iYKPoint;//���
			mesbody.relay.value = rc->iVal;
			mesbody.relay.state = CHECK_FAIL;
			mesbody.relay.device=rc->iDevice;//�豸��

			AGC_Digit *rec=NULL;
			if(g_agcCalc->getFuncAgcRtdb()->findYk(rc,rec))
			{
				TRACE("��AGC��¼�����ҵ���ң�ؼ�¼\n");
				GetLocalTime(rec->tSnapTime);
				rec->iVal=rc->iVal;//����ֵ
				g_agcCalc->handleYk(rec);
			}
			else
			{
				TRACE("��AGC_Digit����û����ƥ��ļ�¼��������Scada���ң�ر�û����ȷ����\n");
			}


			//smap_append_message(&mesbody);
		}
		break;
	case mes_SPSelect:
	case mes_SPExecute: 
		{

			//if (m_pGlobal->GetWorkState() == FEP_PRIMARY) { //tjs del ��Ϊû��m_pGlobal����
			if(1==1)
			{
				RSetpoint *rs = (RSetpoint *)(message->m_bMes);

				MesBody mesbody;
				mesbody.comm.type = MES_TYPE_SPOINT;
				if (message->m_iMsgType == mes_RCSelect) 
				{
					mesbody.relay.cmd = RELAY_SELECT;
					TRACE("<==MT:ң��ѡ��==>\n");
				}
				else if (message->m_iMsgType == mes_RCExecute)
				{
					mesbody.relay.cmd = RELAY_EXECUTE;
					TRACE("<==MT:ң��ִ��==>\n");
				}

				mesbody.comm.destination = rs->iStation;
				mesbody.spoint.point = rs->iPoint;
				mesbody.spoint.val = rs->fVal;
				mesbody.spoint.device=rs->iDevice;

				AGC_Analog *rec=NULL;
				if(g_agcCalc->getFuncAgcRtdb()->findYt(rs,rec))
				{
					TRACE("��AGC_Aanlog��¼�����ҵ���ң����¼\n");
					GetLocalTime(rec->tSnapTime);
					rec->fVal=rs->fVal;//����ֵ
					g_agcCalc->handleYt(rec);
				}
				else
				{
					TRACE("��AGC_Analog����û����ƥ��ļ�¼��������Scada���ң����û����ȷ����\n");
				}
				//smap_append_message(&mesbody);
			}
			break;
		}
	default:
		break;
	}


}
bool agcCalc::Stop()
{
	m_bThreadRunning=false;
	m_funcRtdb.rtdb_logout();
	return true;
}
//ˢ������
void agcCalc::freshdata()
{
	//memset(m_LastError,0,sizeof(m_LastError));
	//sprintf(m_LastError ,"%s","ˢ����������");
	//writeBuffer(m_LastError);

	m_lockFreshData.Lock();
	m_funcRtdb.freshPhvUnit();
	m_funcRtdb.freshSocUnit();
	m_funcRtdb.freshDigit();
	m_funcRtdb.freshAnlog();
	m_lockFreshData.UnLock();

}
//�õ�����
int agcCalc::getCommand()
{

	float nTarget=0;
	int nRet=0;
	nRet=readCurrentDateTarget(nTarget,false,true);
	m_getTarget = nTarget;//���淢��Ŀ����

	if (nRet<0)
	{
		strcpy(m_LastError,"��ȡʵʱĿ��ֵʱ�����������ñ����й���");
		writeBuffer(m_LastError);
	}
	else
	{

	}
	memset(m_LastError,0,sizeof(m_LastError));
	sprintf(m_LastError ,"�õ���ǰ����%.2f",nTarget);
	writeBuffer(m_LastError);

	return true;
}
//�������
bool agcCalc::isLocked()
{
	bool ret=false;
	if(m_bFistStartFlg)
	{		
		time_t now;
		time(&now);
		int firstStartWait = 30;
		if(now-m_timFirstStart<firstStartWait)
		{			
			ret=true;
			memset(m_LastError,NULL,sizeof(m_LastError));
			sprintf(m_LastError ,"������������������ȴ�%d����ʽ�����㷨,���ڻ���%d��\n",firstStartWait,firstStartWait-(now-m_timFirstStart));			
			writeBuffer(m_LastError);
			TRACE(m_LastError);
		}
		else
		{
			m_bFistStartFlg=false;
		}
	}	
	if(false==checkYtExists())
	{
		ret=true;
	}

	int iChannelBroken = m_funcRtdb.getAgcDigit(eAgcDigitPhvChannel);
	if(iChannelBroken == 0)//���ͨ���ָ�����
	{ 
		sprintf(m_LastError ,"�ɼ�����ͨ���Ͽ�,������ִ��");			
		writeBuffer(m_LastError);

		ret=true;
	}


	float fUa = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUa);
	float fUb = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUb);
	float fUc = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUc);
	float fULowLimit = m_funcRtdb.getAgcAnalog(eAgcAnalogULimit);//�����ֵ


	if(fUa<fULowLimit && fUb<fULowLimit && fUc<fULowLimit)
	{
		//���������ı���
		m_funcRtdb.setAgcWarn(eAgcAlarmOffNet, 1);
		sprintf(m_LastError ,"�������ѹֵС������%.2f�ж�ϵͳ����",fULowLimit);			
		writeBuffer(m_LastError);
		ret=true;
	}
	else
	{
		m_funcRtdb.setAgcWarn(eAgcAlarmOffNet, 0);
	}

	if(ret)
	{
		m_funcRtdb.uptDigRecord(eAgcDigitTotalBlock, 1);

	}
	else
	{
		m_funcRtdb.uptDigRecord(eAgcDigitTotalBlock, 0);
	}

	memset(m_LastError,0,sizeof(m_LastError));
	sprintf(m_LastError ,"�������%s",ret==false?"ͨ��":"��ͨ��");
	writeBuffer(m_LastError);

	return ret;
}
//��ȫԼ�����
bool agcCalc::isSafeConstraint()
{

	if(m_funcRtdb.getCommAdParam(eAgcParamSafeCheckFlg) <=0) //������а�ȫԼ�����
	{
		memset(m_LastError,0,sizeof(m_LastError));		
		sprintf(m_LastError ,"%s","��ִ��ȫԼ�����");
		writeBuffer(m_LastError);
		return false;
	}
	bool ret=false;


	//�ж�һ�´��ܵ�״̬
	if(m_pSocInfo!=NULL)
	{

		if(m_pSocInfo->p_Unit->fBatteryPower>100||m_pSocInfo->p_Unit->fBatteryPower<0)
		{
			//����ʣ������쳣���ʵ
			memset(m_LastError,0,sizeof(m_LastError));		
			sprintf(m_LastError ,"����ʣ������쳣���ʵ,Ӧ����[0-100]֮��,��ǰֵ%.3f",m_pSocInfo->p_Unit->fBatteryPower);
			writeBuffer(m_LastError);
			ret=true;
		}



		if(m_pSocInfo->p_Unit->iExpStatus==SOC_PCS_EXP_STATUS_Fault)
		{
			sprintf(m_LastError ,"���ܹ���״̬Ϊ���ϣ�����");
			writeBuffer(m_LastError);
			m_funcRtdb.setAgcWarn(eAgcAlarmPcsFault, 1);
		}
		else
		{
			m_funcRtdb.setAgcWarn(eAgcAlarmPcsFault, 0);
		}


		float fDischargeMaxPower = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargeKw);//���ܷŵ������
		bool bOverLoad=false;
		if(m_pSocInfo->p_Unit->fValUp>0)
		{
			if(m_funcRtdb.equalDouble(m_pSocInfo->p_Unit->fValUp,fDischargeMaxPower)||
				m_pSocInfo->p_Unit->fValUp>fDischargeMaxPower)
			{
				if(fabs(m_pSocInfo->p_Unit->fValP-fDischargeMaxPower)<0.5)
				{
					bOverLoad=true;
				}
			}
		}
		if(!bOverLoad)
			m_funcRtdb.setAgcWarn(eAgcAlarmOverLoad, 0);

		int n=m_funcRtdb.getAgcAlarm(eAgcAlarmOverLoad);
		if(n==1)
		{
			if(	!m_funcRtdb.getAgcAlarm(eAgcAlarmPcsFaultDischargeNoExec))
			{
				m_funcRtdb.setAgcWarn(eAgcAlarmOverLoad,0);
			}
		}

		float fChargeMaxPower = m_funcRtdb.getAgcAnalog(eAgcAnalogSocChargekW);//���ܳ�������

		bOverLoad=false;
		if(m_pSocInfo->p_Unit->fValUp<0)
		{
			if(m_funcRtdb.equalDouble(m_pSocInfo->p_Unit->fValUp,fChargeMaxPower)||
				fabs(m_pSocInfo->p_Unit->fValUp)>fChargeMaxPower)
			{
				if(fabs(m_pSocInfo->p_Unit->fValP-fChargeMaxPower)<0.5)
				{
					bOverLoad=true;
				}
			}
		}
		if(!bOverLoad)
			m_funcRtdb.setAgcWarn(eAgcAlarmBusVoltHigh, 0);

		 n=m_funcRtdb.getAgcAlarm(eAgcAlarmBusVoltHigh);
		if(n==1)
		{
			if(	!m_funcRtdb.getAgcAlarm(eAgcAlarmPcsFaultChargeNoExec))
			{
				m_funcRtdb.setAgcWarn(eAgcAlarmBusVoltHigh,0);
			}
		}

	}

	cheakCommonVarible();
	memset(m_LastError,0,sizeof(m_LastError));
	sprintf(m_LastError ,"��ȫԼ�����%s",ret==false?"ͨ��":"��ͨ��");
	writeBuffer(m_LastError);

	return ret;
}
void agcCalc::distribute()
{
	float fUa = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUa);
	float fUb = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUb);
	float fUc = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUc);
	float fUMax = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUMax);
	float fUMin = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUMin);
	float fULowLimit = m_funcRtdb.getAgcAnalog(eAgcAnalogULimit);//�����ֵ

	

	//TODO:�������δ���еĻ�����������ʲô
	bool bUa=false;
	bool bUb=false;
	bool bUc=false;
	if((fUa>fUMin&& fUa<fUMax) ||m_funcRtdb.equalDouble(fUa,fUMin)||m_funcRtdb.equalDouble(fUa,fUMax))
	{
		bUa=true;
	}

	if((fUb>fUMin&& fUb<fUMax) ||m_funcRtdb.equalDouble(fUb,fUMin)||m_funcRtdb.equalDouble(fUb,fUMax))
	{
		bUb=true;
	}

	if((fUc>fUMin&& fUc<fUMax) ||m_funcRtdb.equalDouble(fUc,fUMin)||m_funcRtdb.equalDouble(fUc,fUMax))
	{
		bUc=true;
	}
	m_pSocInfo->adjustFlag=false;
	m_pSocInfo->m_FlgSendStart=false;
	if(bUa&&bUb&&bUc)
	{
		m_funcRtdb.setAgcWarn(eAgcAlarmOverLoad, 0);
		m_funcRtdb.setAgcWarn(eAgcAlarmBusVoltHigh, 0);

		//�ȶ����Ʋ���
		disNormalMethod();
	}
	else if(fUa>fUMax || fUb>fUMax || fUc>fUMax)
	{
		DownVoltage();
	}
	else
	{
		UpVoltage();
	}

}
//�ȶ����Ʋ���
int agcCalc::disNormalMethod()
{
	if(m_pSocInfo==NULL)
	{
		return AGC_FAIL;
	}

	memset(m_LastError,0,sizeof(m_LastError));
	sprintf(m_LastError ,"ִ���ȶ����Ʋ���");
	writeBuffer(m_LastError);


	int nRet=getCurrentChargetOrDis();
	if(nRet==0)
	{
		pcsControlMethod(0);
		return AGC_SUC;
	}
	else
	{
		if(1==nRet)
		{
			if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Charge)
			{
				float fChargetKw = getNormalChargePower();
				if(m_funcRtdb.equalDouble(m_pSocInfo->p_Unit->fValUp,-fChargetKw)!=0)
				{
					//ͬʱ��ֵ
					pcsControlMethod(-fChargetKw);
				}

			}
			else if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Discharge)
			{
				//����
				pcsControlMethod(0);
			}
			else
			{
				//�������Ҳ���ŵ�
				//�Թ̶����ʳ��
				float fChargetKw = getNormalChargePower();
				pcsControlMethod(-fChargetKw);
			}
		}
		else
		{
			if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Discharge)
			{
				float fDischargetKw = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargeKw);

				if(m_funcRtdb.equalDouble(m_pSocInfo->p_Unit->fValUp,fDischargetKw)!=0)
				{
					//ͬʱ��ֵ
					pcsControlMethod(fDischargetKw);
				}

			}
			else if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Charge)
			{
				//����
				pcsControlMethod(0);
			}
			else
			{
				//�������Ҳ���ŵ�
				//�Թ̶����ʳ��
				float fDischargetKw = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargeKw);
				pcsControlMethod(fDischargetKw);
			}
		}
	}
	
	return AGC_SUC;

}
//���ܿ��Ʋ���
int agcCalc::pcsControlMethod(float dTarget)
{

	if(m_pSocInfo==NULL)
	{
		return AGC_FAIL;
	}

	if(m_pSocInfo->p_Unit->iExpStatus==SOC_PCS_EXP_STATUS_Fault)
	{
		return AGC_FAIL;
	}

	pcsStartMethod();

	if(m_pSocInfo->p_Unit->iStatus==SOC_PCS_RUN_STATUS_Run)
	{
		m_pSocInfo->m_FlgSendStart=false;

		if(m_funcRtdb.equalDouble(dTarget,0))
		{
			//����
			pcsWaitMethod();
		}
		else if(dTarget>0)
		{
			//�ŵ�
			pcsDischargeMethod(dTarget);
		}
		else
		{
			//��֤����ȥ��ֵ
			pcsChargeMethod(0-dTarget);
		}
	}

	return AGC_SUC;
}
//��ѹ����
int agcCalc::UpVoltage()
{
	memset(m_LastError,0,sizeof(m_LastError));
	sprintf(m_LastError ,"ִ����ѹ���Ʋ���");
	writeBuffer(m_LastError);

	if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Charge)
	{
		//����
		pcsControlMethod(0);

	}
	else if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Discharge)
	{
		float fDischargeMin = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargeMin);//�ŵ�����

		if(m_funcRtdb.equalDouble(m_pSocInfo->p_Unit->fBatteryPower,fDischargeMin)||
			m_pSocInfo->p_Unit->fBatteryPower<fDischargeMin)
		{
			return AGC_SUC;
		}
		else
		{

			float fDischargeMaxPower = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargeKw);//���ܷŵ������
			bool bOverLoad=false;
			if(m_pSocInfo->p_Unit->fValUp>0)
			{
				if(m_funcRtdb.equalDouble(m_pSocInfo->p_Unit->fValUp,fDischargeMaxPower)||
					m_pSocInfo->p_Unit->fValUp>fDischargeMaxPower)
				{
					if(fabs(m_pSocInfo->p_Unit->fValP-fDischargeMaxPower)<0.5)
					{
						bOverLoad=true;
					}
				}
			}
			if(bOverLoad)
			{
				memset(m_LastError,0,sizeof(m_LastError));
				sprintf(m_LastError ,"��ǰ�й��趨%.2f���ڵ��ڷŵ����%.2f,�ж���ʱ���ܷŵ�����ñ������ɳ���",m_pSocInfo->p_Unit->fValP,fDischargeMaxPower);
				writeBuffer(m_LastError);

				m_funcRtdb.setAgcWarn(eAgcAlarmOverLoad, 1);
			}
			else
			{
				float fDisPower=calcDischargePower();
				pcsControlMethod(fDisPower);
			}

		}
	}
	else
	{


		float fDisPower=calcDischargePower();


		pcsControlMethod(fDisPower);
		int n=m_funcRtdb.getAgcAlarm(eAgcAlarmPcsFaultDischargeNoExec);
		if(n)
		{
			memset(m_LastError,0,sizeof(m_LastError));
			sprintf(m_LastError ,"���ܲ�ִ�зŵ�����ñ������ɳ���");
			writeBuffer(m_LastError);

				m_funcRtdb.setAgcWarn(eAgcAlarmOverLoad, 1);
		}
	}
	return AGC_SUC;
}
//��ѹ����
int agcCalc::DownVoltage()
{
	memset(m_LastError,0,sizeof(m_LastError));
	sprintf(m_LastError ,"ִ�н�ѹ���Ʋ���");
	writeBuffer(m_LastError);

	if(m_pSocInfo->p_Unit->iStatus==SOC_PCS_RUN_STATUS_Run)
	{
		if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Charge)
		{
			float fChargeMaxPower = m_funcRtdb.getAgcAnalog(eAgcAnalogSocChargekW);//���ܳ�������

			bool bOverLoad=false;
			if(m_pSocInfo->p_Unit->fValUp<0)
			{
				if(m_funcRtdb.equalDouble(m_pSocInfo->p_Unit->fValUp,fChargeMaxPower)||
					fabs(m_pSocInfo->p_Unit->fValUp)>fChargeMaxPower)
				{
					if(fabs(m_pSocInfo->p_Unit->fValP-fChargeMaxPower)<0.5)
					{
						bOverLoad=true;
					}
				}
			}
			if(bOverLoad)
			{	
				m_funcRtdb.setAgcWarn(eAgcAlarmBusVoltHigh, 1);

				memset(m_LastError,NULL,sizeof(m_LastError));
				sprintf_s(m_LastError,sizeof(m_LastError),"ĸ�ߵ�ѹƫ�ߣ����ܴﵽ����繦��%.2fkW,��ǰ����ʵʱ�й�%.2fkW",fChargeMaxPower,m_pSocInfo->p_Unit->fValP);
				writeBuffer(m_LastError);

				//TODO:���ù���޹���


			}
			else
			{
				float fDispower=calcChargePower();
				pcsControlMethod(fDispower);
			}
		}
		else
		{
			float fDispower=calcChargePower();
			pcsControlMethod(fDispower);
		}
	}
	else 
	{
		//����
		//pcsControlMethod(0);
	}
	return AGC_SUC;
}
float agcCalc::calcChargePower()
{
	float fUa = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUa);
	float fUb = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUb);
	float fUc = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUc);
	float fUMax = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUMax);

	//TODO:�������δ���еĻ�����������ʲô
	float fURef=fUa;
	if((fUa>fUMax) ||m_funcRtdb.equalDouble(fUa,fUMax))
	{
		fURef=fUa;
	}
	if((fUb>fUMax) ||m_funcRtdb.equalDouble(fUb,fUMax))
	{
		if(fUb>fURef)
			fURef=fUb;
	}
	if((fUc>fUMax) ||m_funcRtdb.equalDouble(fUc,fUMax))
	{
		if(fUc>fURef)
			fURef=fUc;
	}

	float fft = m_funcRtdb.getAgcAnalog(eAgcAnalogFtPowerU);//��ѹ���޹�ϵ��
	float fModify = m_funcRtdb.getAgcAnalog(eAgcAnalogAdVoltModify);//��ѹƫ������

	float fDisPower=-(fURef-fUMax-fModify)*fft;
	return  fDisPower;
}
//����ŵ繦��
float agcCalc::calcDischargePower()
{
	float fUa = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUa);
	float fUb = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUb);
	float fUc = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUc);
	float fUMin = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUMin);

	//TODO:�������δ���еĻ�����������ʲô
	float fURef=fUa;
	if((fUa<fUMin) ||m_funcRtdb.equalDouble(fUa,fUMin))
	{
		fURef=fUa;
	}
	if((fUb<fUMin) ||m_funcRtdb.equalDouble(fUb,fUMin))
	{
		if(fUb<fURef)
			fURef=fUb;
	}
	if((fUc<fUMin) ||m_funcRtdb.equalDouble(fUc,fUMin))
	{
		if(fUc<fURef)
			fURef=fUc;
	}

	float fft = m_funcRtdb.getAgcAnalog(eAgcAnalogFtPowerU);//��ѹ���޹�ϵ��
	float fModify = m_funcRtdb.getAgcAnalog(eAgcAnalogAdVoltModify);//��ѹƫ������

	float fDisPower=(fUMin-fURef+fModify)*fft;
	return  fDisPower;
}
//����
void agcCalc::distribute_phv()
{

	//�����������㷨
	float dTarget = m_getTarget;
	//	m_funcRtdb.uptAlgRecordBykW(eAgcAnalogRealPlan, m_getTarget);
	TRACE("dTarget:%f\n", dTarget);
	float nMaxRadio=100;
	// clear
	TRACE("m_nGenCount:%d\n", m_nGenCount);
	for(int i = 0 ; i < m_nGenCount ; i++)
	{
		m_pGenInfo[i]->m_ReSendTimes = 0; //�����ظ���������
		m_pGenInfo[i]->adjustFlag = false;
		//��ʼ��ʱΪ�˷�ֹ�쳣����������
		m_pGenInfo[i]->p_Unit->fDisPower = m_pGenInfo[i]->p_Unit->fcurCap;//��������й���Ӧ��������(KW)
		m_pGenInfo[i]->ratio = (int)(ceil(nMaxRadio/m_pGenInfo[i]->p_Unit->fFactor));//��û�б��ʣ��� pObject/pRating * 100,0~100
	}
	// read target
	float Ptarget = dTarget;
	float PMaxSum = 0;     //�ɵ��ڵ������
	int nAdjustNum = 0;     //�ɵ��ڵ����������
	float PRealSum = 0;    // ʵ���ܳ���
	float PRealRating = 0; //�ɵ����������������
	float dd=0;

	Q_RECORDDATATAG dataArray;
	dataArray.eDataType=eDataTypeFloat;		
	float m_loss = 0;
	if((Ptarget + m_loss) <= 0.0)
	{
		// invalid adjust value
		for(int i = 0 ; i < m_nGenCount ; i++)
		{
			if(m_pGenInfo[i]->enabled == false)
				continue;

			m_pGenInfo[i]->adjustFlag= false;
			m_pGenInfo[i]->ratio = 0;
			m_pGenInfo[i]->p_Unit->fDisPower = 0;
		}
		TRACE("invalid adjust value\n");
		return;
	}

	SYSTEMTIME curTime;
	GetLocalTime(curTime);

	m_fnoCanAdjust=0.0f;
	for(int i = 0 ; i < m_nGenCount ; i++)
	{
		//�����������״̬
		if(m_pGenInfo[i]->p_Unit->iStatus != 1) //?��û�г�ʼ��
		{
			//TRACE("m_pGenInfo:%dû�г�ʼ��\n", i);
			continue;
		}
		//�������֧�ֵ���
		if(m_pGenInfo[i]->canAdjust == false )
		{
			Ptarget -= m_pGenInfo[i]->p_Unit->fValP; //fValP ����ʵ�ʳ���(KW)�����й�
			TRACE("Ptarget:%f,m_pGenInfo[i]->p_Unit->fValP:%f\n", Ptarget, m_pGenInfo[i]->p_Unit->fValP);
			m_fnoCanAdjust+=m_pGenInfo[i]->p_Unit->fValP;//fValP ����ʵ�ʳ���(KW)�����й�
			TRACE("m_pGenInfo[%d]->canAdjust == false\n", i);
			continue;
		}

		//������ɵ�
		PMaxSum += m_pGenInfo[i]->p_Unit->fcurCap;//��������й���Ӧ��������(KW)
		nAdjustNum++;
		PRealSum += m_pGenInfo[i]->p_Unit->fValP; //fValP ����ʵ�ʳ���(KW)�����й�
		PRealRating += m_pGenInfo[i]->p_Unit->fcurCap;//��������й���Ӧ��������(KW)
	}
	memset(m_LastError,NULL,sizeof(m_LastError));
	sprintf_s(m_LastError,sizeof(m_LastError),"%d ̨�ɵ������",nAdjustNum);
	writeBuffer(m_LastError);
	if(nAdjustNum == 0) //  �޿ɵ������
	{
		TRACE("nAdjustCur==0\n");
		return;
	}

	float dDeadZone=0;
	//dDeadZone = m_funcRtdb.getAgcAnalog(eAgcAnalogAdDeadZone);

	float dValue = dTarget;//ȥ������?
	float dConnectP = 0;//m_funcRtdb.getAgcAnalog(eAgcAnalogNetPower);//�������й�
	if((fabs(dValue - dConnectP) < dDeadZone) && (dValue >= dConnectP))//������������������й��ľ���ֵС�����������Ҳ������й�С�ڵ�������ʱ�Ų�����
	{
		memset(m_LastError,NULL,sizeof(m_LastError));
		sprintf_s(m_LastError,sizeof(m_LastError),"=====����Ŀ���С�ڵ�������=====");
		writeBuffer(m_LastError);
		TRACE("����Ŀ���С�ڵ�������\n");
		return;	// 
	}
	float v_tTargetForDiffSrc=Ptarget;//�������ݲ�ͬԴʹ��


	//float nMaxRadio=100;
	//2015-3-8 ����m_loss�ĳ�-500��ʱ�򣬻����������ʱ�䲻�ܷ�100��ʱ�򡣣�����ǬǬ
	if(v_tTargetForDiffSrc > PMaxSum||(v_tTargetForDiffSrc-PMaxSum>-0.001&&v_tTargetForDiffSrc-PMaxSum<0.001)) 
	{
		for(int i = 0 ; i < m_nGenCount ; i++)
		{
			if(m_pGenInfo[i]->p_Unit->iStatus != 1 || m_pGenInfo[i]->canAdjust == false)
				continue;

			m_pGenInfo[i]->adjustFlag = true;
			m_pGenInfo[i]->p_Unit->fDisPower = m_pGenInfo[i]->p_Unit->fcurCap;
			m_pGenInfo[i]->ratio = (int)(ceil(nMaxRadio/m_pGenInfo[i]->p_Unit->fFactor));
		}
		TRACE("v_tTargetForDiffSrc:%f > PMaxSum:%f\n",v_tTargetForDiffSrc, PMaxSum);
		return;
	}
	memset(m_LastError,NULL,sizeof(m_LastError));
	sprintf_s(m_LastError ,sizeof(m_LastError),"�й�Ŀ�� %.2f , ��ǰ�й�%.2f , ���ɵ���%.2f \r\n",Ptarget, dConnectP, m_fnoCanAdjust);
	writeBuffer(m_LastError);



	if(Ptarget > dConnectP - m_fnoCanAdjust)  // �ϵ�, �������й�,���ϱ�Ptarget�Ѿ���ȥ���ɵ���������й�������Ptarget��ָ����Ҫ���ڵ��ܹ���
	{

		memset(m_LastError,NULL,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError),"�����й��������Ŀ�꣺ %.2f \r\n",Ptarget);
		writeBuffer(m_LastError);
		float pTargetSum = Ptarget;
		float pRealRating = PRealRating;
		float Pit = pTargetSum / nAdjustNum;		// ƽ������Ŀ��ֵ
		float Pir = pRealRating / nAdjustNum;		// ƽ���������
		int nAdjustCur = nAdjustNum;	// ����Ҫ���ڵ����������
		float PitTmp=Pit;
		for(int i = 0 ; i < m_nGenCount ; i++)
		{
			if(m_pGenInfo[i]->p_Unit->iStatus != 1 || m_pGenInfo[i]->canAdjust == false)
				continue;

			Pit=PitTmp;
			Pit *= (m_pGenInfo[i]->p_Unit->fcurCap / Pir);
			if(m_pGenInfo[i]->p_Unit->fValP >= Pit)  // ��ʵ�ʷ���������Ŀ��ֵʱ�������������  //fValP ����ʵ�ʳ���(KW)�����й�
			{
				pTargetSum -= m_pGenInfo[i]->p_Unit->fValP;
				nAdjustCur--;
				pRealRating -= m_pGenInfo[i]->p_Unit->fcurCap;
				m_pGenInfo[i]->adjustFlag = false;
			}
			else
			{
				m_pGenInfo[i]->adjustFlag = true;
			}
		}
		memset(m_LastError,NULL,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError),"��Ҫ�������������%d \r\n",nAdjustCur);
		writeBuffer(m_LastError);
		if(nAdjustCur <= 0)
		{
			TRACE("nAdjustCur<=0\n");
			return ;
		}
		float aa = pTargetSum / nAdjustCur;		// ���յ��ڵ�ƽ��ֵ
		float bb = pRealRating / nAdjustCur;	// ����ƽ���������
		float PadjustTmp=aa;
		for(int i = 0 ; i < m_nGenCount ; i++)
		{
			aa=PadjustTmp;//tjs update 20120905��Ϊ��������������Ϊ��ͬʱ����ͬһ������ʵ���������ʻ��в�ͬ��
			if(m_pGenInfo[i]->adjustFlag == true)
			{
				aa *= (m_pGenInfo[i]->p_Unit->fcurCap / bb);
				m_pGenInfo[i]->p_Unit->fDisPower = aa;
				m_pGenInfo[i]->ratio = (int)ceil((( m_pGenInfo[i]->p_Unit->fDisPower / m_pGenInfo[i]->p_Unit->fcurCap ) * nMaxRadio+m_pGenInfo[i]->p_Unit->iModifyAdd)/m_pGenInfo[i]->p_Unit->fFactor);
				memset(m_LastError,NULL,sizeof(m_LastError));
				sprintf_s(m_LastError ,sizeof(m_LastError),"���� %d �����������Ŀ��ֵΪ %.2f\r\n",m_pGenInfo[i]->p_Unit->ID32,m_pGenInfo[i]->p_Unit->fDisPower);
				writeBuffer(m_LastError);
			}
		}
		return;
	}
	else // �µ�
	{

		//if(!m_bCloseDiffSourceHandle)
		//	Ptarget=v_tTargetForDiffSrc;

		memset(m_LastError,NULL,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError),"�µ�������й�Ŀ�� %.2f \r\n",Ptarget);
		writeBuffer(m_LastError);
		float pTargetSum = Ptarget;
		float pRealRating = PRealRating;
		float Pit = pTargetSum / nAdjustNum;		// ƽ������Ŀ��ֵ
		float Pir = pRealRating / nAdjustNum;		// ƽ���������
		int nAdjustCur = nAdjustNum;				// Ҫ���ڵ����������

		float PitTmp = Pit;
		memset(m_LastError,NULL,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError),"��̨Ŀ�� %.2f \r\n",PitTmp);
		writeBuffer(m_LastError);
		for(int i = 0; i < m_nGenCount; i++)
		{
			if(m_pGenInfo[i]->p_Unit->iStatus != 1 || m_pGenInfo[i]->canAdjust == false)
			{
				memset(m_LastError,NULL,sizeof(m_LastError));
				sprintf_s(m_LastError ,sizeof(m_LastError),"%d ����������ɵ� \r\n",i);
				writeBuffer(m_LastError);
				continue;
			}

			Pit=PitTmp;
			Pit *= (m_pGenInfo[i]->p_Unit->fcurCap / Pir);
			if(m_pGenInfo[i]->p_Unit->fValP <= Pit)  // ��ʵ�ʷ�����С��Ŀ��ֵʱ�������������
			{
				pTargetSum -= m_pGenInfo[i]->p_Unit->fValP;
				nAdjustCur--;
				pRealRating -= m_pGenInfo[i]->p_Unit->fcurCap;
				m_pGenInfo[i]->adjustFlag = false;
				memset(m_LastError,NULL,sizeof(m_LastError));
				sprintf_s(m_LastError ,sizeof(m_LastError),"%d ��������й� %.2f Ŀ��%.2f ������\r\n",m_pGenInfo[i]->p_Unit->ID32,m_pGenInfo[i]->p_Unit->fValP,Pit);
				writeBuffer(m_LastError);
			}
			else
			{
				m_pGenInfo[i]->adjustFlag = true;
			}
		}
		//ASSERT(nAdjustCur > 0);
		if(nAdjustCur<=0)
		{
			TRACE("nAdjustCur<=0\n");
			return;
		}


		float aa = pTargetSum / nAdjustCur;		// ���յ��ڵ�ƽ��ֵ
		float bb = pRealRating / nAdjustCur;	// ����ƽ���������
		float PadjustTmp=aa;//tjs update 20120905��Ϊ��������������Ϊ��ͬʱ����ͬһ������ʵ���������ʻ��в�ͬ��
		for(int i = 0 ; i < m_nGenCount ; i++)
		{
			aa=PadjustTmp; 
			if(m_pGenInfo[i]->adjustFlag== true)
			{
				aa *= (m_pGenInfo[i]->p_Unit->fcurCap / bb);
				m_pGenInfo[i]->p_Unit->fDisPower = aa;
				m_pGenInfo[i]->ratio = (int)((( m_pGenInfo[i]->p_Unit->fDisPower / m_pGenInfo[i]->p_Unit->fcurCap ) * nMaxRadio - m_pGenInfo[i]->p_Unit->iModifySub)/m_pGenInfo[i]->p_Unit->fFactor);
				memset(m_LastError,NULL,sizeof(m_LastError));
				sprintf_s(m_LastError ,sizeof(m_LastError),"�½� %d �����������Ŀ��ֵΪ %.2f\r\n",m_pGenInfo[i]->p_Unit->ID32,m_pGenInfo[i]->p_Unit->fDisPower);
				writeBuffer(m_LastError);

				// 				//��С���ڿ��ƣ��Ȳ�����
				// 				if(m_inverterInfo[i]->pObject < (m_inverterInfo[i]->pRating * 0.1))
				// 				{
				//  					m_inverterInfo[i]->pObject = m_inverterInfo[i]->pRating * 0.1;
				//  					m_inverterInfo[i]->ratio = (int)((0.1 * nMaxRadio)/m_inverterInfo[i]->factor);
				// //					m_inverterInfo[i]->adjustFlag = false;//2013-09-25 YangHuaining  С����Сֵ����������������ػ��޷�����
				// //					m_inverterInfo[i]->adjustFlagCopy = false;
				// 				}
			}
		}
	}
	for(int i = 0 ; i < m_nGenCount ; i++)
	{
		if(m_pGenInfo[i]->p_Unit->fDisPower < 0)
		{
			m_pGenInfo[i]->adjustFlag = false;
			memset(m_LastError,NULL,sizeof(m_LastError));
			sprintf_s(m_LastError ,sizeof(m_LastError),"���� %d ����������为ֵ����ȷ��\r\n",m_pGenInfo[i]->p_Unit->ID32);
			writeBuffer(m_LastError);
		}
		TRACE("GenInfo ID:%d,DisPower:%f\n",m_pGenInfo[i]->p_Unit->ID32, m_pGenInfo[i]->p_Unit->fDisPower);
	}

	memset(m_LastError,0,sizeof(m_LastError));
	sprintf(m_LastError ,"%s","�����㷨����");
	writeBuffer(m_LastError);
	return;

}
void agcCalc::execPhv()
{

	int iSend = 0;
	time_t currentTime;
	time(&currentTime);
	while(iSend < m_nGenCount)
	{
		//TRACE("iSend:%d\n", iSend);
		GeneratorUnit *p = m_pGenInfo[iSend];
		if(p->adjustFlag)
		{		
			//TRACE("1\n");
			if(checkAdjustObject(currentTime,p) == true)
			{
				//TRACE("2\n");
				p->lastAdjustTime = currentTime;
				p->lastObjectValue = p->p_Unit->fDisPower;

				//do send command
				//������Ϣִ��
				p->p_Unit->iFlag=PHVUNIT_EXEC;//ִ��				

				float execval=p->p_Unit->fDisPower*p->p_Unit->fFactor;
				//д��Ŀ��ָ�����
				m_funcRtdb.uptAlgRecordByID(p->p_Unit->iDisPower,p->p_Unit->fDisPower);
				//end
				bool b_exeRes=true;
				if(isExec()==true)
				{
					memset(m_LastError,0,sizeof(m_LastError));
					sprintf(m_LastError ,"send YT channelID:%d,DeviceID:%d,YtNo:%d,YtVal=%.2f [DisPower:%.2f,Factor:%.2f] exec\n",p->p_Unit->iChannelID,p->p_Unit->iDevice,p->p_Unit->iYtNo,execval,p->p_Unit->fDisPower,p->p_Unit->fFactor);
					writeBuffer(m_LastError);

					//ң�������Ҫ��factor�������ڻ���������ӣ���ǰ����ң����е㲻����	
					b_exeRes = m_funcRtdb.sendDeviceExecYt(p->p_Unit->iChannelID,p->p_Unit->iDevice,p->p_Unit->iYtNo,execval);							

					memset(m_LastError,0,sizeof(m_LastError));
					sprintf(m_LastError ,"Yt return results:%s",b_exeRes==true?"success\n":"failed\n");
					writeBuffer(m_LastError);
				}


				//�ȴ�����ʱ��
				if(waitForTimeout(CALC_TH_KeyID,m_adParameter.getSendCmdInterval()) == false)  
				{				
					break;
				}
				else if(m_bNeedStopProcess == true)
				{
					break; //������������ѭ�� ��Ǭ
				}


				if(b_exeRes==false && m_adParameter.getReSendFlg())//?? ��Ǭע�͵���
				{
					//�ظ�����
					p->m_ReSendTimes++;

					if(p->m_ReSendTimes>=AGC_RE_SEND_MAX_TIMES)
					{
						p->adjustFlag=false; 
						writeBuffer("�������������õ�����ط��������������·���\n");
					}
					else 
					{
						p->adjustFlag=true; 
						continue;
					}
				}
				else
				{
					p->adjustFlag= false;
				}

			}
			else
			{
				TRACE("checkAdjustObject==false\n");
			}
		}

		iSend++;

	}
}
//���ܵ�ִ��
void agcCalc::execPcs()
{

	int iSend = 0;
	time_t currentTime;
	time(&currentTime);

	//TRACE("iSend:%d\n", iSend);
	GeSocUnit* p=	m_pSocInfo;
	if(p->adjustFlag)
	{	
		//TRACE("1\n");
		if(checkAdjustObjectPcs(currentTime,p) == true)
		{
			//TRACE("2\n");
			p->lastAdjustTime = currentTime;
			p->lastObjectValue = p->p_Unit->fDisPower;

			//do send command
			//������Ϣִ��
			p->p_Unit->iFlag=PHVUNIT_EXEC;//ִ��				

			float execval=p->p_Unit->fDisPower*p->p_Unit->fFactor;
			//д��Ŀ��ָ�����
			m_funcRtdb.uptAlgRecordByID(p->p_Unit->iDisPower,p->p_Unit->fDisPower);
			//end
			bool b_exeRes=true;
			if(isExec()==true)
			{
				memset(m_LastError,0,sizeof(m_LastError));
				sprintf(m_LastError ,"send YT channelID:%d,DeviceID:%d,YtNo:%d,YtVal=%.2f [DisPower:%.2f,Factor:%.2f] exec\n",p->p_Unit->iChannelID,p->p_Unit->iDevice,p->p_Unit->iYtNo,execval,p->p_Unit->fDisPower,p->p_Unit->fFactor);
				writeBuffer(m_LastError);

				//ң�������Ҫ��factor�������ڻ���������ӣ���ǰ����ң����е㲻����	
				b_exeRes = m_funcRtdb.sendDeviceExecYt(p->p_Unit->iChannelID,p->p_Unit->iDevice,p->p_Unit->iYtNo,execval);							

				memset(m_LastError,0,sizeof(m_LastError));
				sprintf(m_LastError ,"Yt return results:%s",b_exeRes==true?"success\n":"failed\n");
				writeBuffer(m_LastError);
			}
			else
			{

				memset(m_LastError,0,sizeof(m_LastError));
				sprintf(m_LastError ,"sim YT channelID:%d,DeviceID:%d,YtNo:%d,YtVal=%.2f [DisPower:%.2f,Factor:%.2f] exec\n",p->p_Unit->iChannelID,p->p_Unit->iDevice,p->p_Unit->iYtNo,execval,p->p_Unit->fDisPower,p->p_Unit->fFactor);
				writeBuffer(m_LastError);
			}


			//�ȴ�����ʱ��
			if(waitForTimeout(CALC_TH_KeyID,m_adParameter.getSendCmdInterval()) == false)  
			{				
				return ;
			}
			else if(m_bNeedStopProcess == true)
			{
				return ; //������������ѭ�� ��Ǭ
			}


			if(b_exeRes==false && m_adParameter.getReSendFlg())//?? ��Ǭע�͵���
			{
				//�ظ�����
				p->m_ReSendTimes++;

				if(p->m_ReSendTimes>=AGC_RE_SEND_MAX_TIMES)
				{
					p->adjustFlag=false; 
					writeBuffer("�������������õ�����ط��������������·���\n");
				}
				else 
				{
					p->adjustFlag=true; 
				}
			}
			else
			{
				p->adjustFlag= false;
			}

		}
		else
		{
			TRACE("checkAdjustObject==false\n");
		}
		p->adjustFlag=false;
	}

	if(p->m_FlgSendStart)
	{
		if(p->p_Unit->iStatus==SOC_PCS_RUN_STATUS_Run)
		{
			p->m_FlgSendStart=false;
			return ;
		}

		if(isExec()==true)
		{
			memset(m_LastError,0,sizeof(m_LastError));
			sprintf(m_LastError ,"send YK channelID:%d,DeviceID:%d,YkNo:%d  exec\n",p->p_Unit->iChannelID,p->p_Unit->iDevice,p->p_Unit->iYkNo);
			writeBuffer(m_LastError);

			bool	b_exeRes = m_funcRtdb.sendDeviceExecYk(p->p_Unit->iChannelID,p->p_Unit->iDevice,p->p_Unit->iYkNo,1);							

			memset(m_LastError,0,sizeof(m_LastError));
			sprintf(m_LastError ,"Yk return results:%s",b_exeRes==true?"success\n":"failed\n");
			writeBuffer(m_LastError);
		}
		else
		{
			memset(m_LastError,0,sizeof(m_LastError));
			sprintf(m_LastError ,"sim YK channelID:%d,DeviceID:%d,YkNo:%d  exec\n",p->p_Unit->iChannelID,p->p_Unit->iDevice,p->p_Unit->iYkNo);
			writeBuffer(m_LastError);
		}

	}






}
void agcCalc::execDirect()
{
	memset(m_LastError,0,sizeof(m_LastError));
	sprintf(m_LastError ,"%s","ִ�з�����������");
	writeBuffer(m_LastError);



	ServerBkType bkType=m_funcRtdb.getServerBkType();//����״̬
	if(bkType!=eSvrBkTypeMaster)
	{
		memset(m_LastError,0,sizeof(m_LastError));
		sprintf(m_LastError ,"��ǰ����Ϊ����������ֻ���䣬�����͸�ִ���豸\n");
		writeBuffer(m_LastError);
		return;
	}

	execPhv();
	execPcs();


	//TRACE("ִ�з����������\n");
}

//ִ�н��
void agcCalc::execResults()
{
	//֧�ֲ������ʣ�������һ�·�һ������
	//У��������Ч��


	execDirect();



}
//nType �̱߳�ʶ
//�ȴ�ʱ��
//����ֵtrue�ȴ���ʱ false �˳��߳�
bool agcCalc::waitForTimeout(int nType,int nTimespan)
{
	int nInterval = 0;
	if(nType == CALC_TH_KeyID)
	{
		//���м���
		//TRACE("nTimespan:%d\r\n", nTimespan);
		//TRACE("m_bNeedStopProcess:%d\r\n", m_bNeedStopProcess);
		//TRACE("m_bThreadRunning:%d\r\n", m_bThreadRunning);
		while(m_bThreadRunning == true && nInterval < nTimespan && m_bNeedStopProcess == false)
		{
#ifdef UNIX_ENV
			usleep(AGCCALC_TIMESPAN*1000);
#else 
			m_CalcEvent.Wait( AGCCALC_TIMESPAN);
#endif

			nInterval +=  AGCCALC_TIMESPAN;

		}
		//TRACE("nInterval:%d\r\n", nInterval);
		//TRACE("m_bNeedStopProcess:%d\r\n", m_bNeedStopProcess);
		if(m_bThreadRunning == false)
		{
			return false;
		}
		else
		{
			//m_bNeedStopProcess = false; //��Ǭ
			return true;
		}
	}
	else if(nType ==CALC_TH_AuxKeyID)
	{
		//���м���
		while(m_bThreadRunning == true && nInterval < nTimespan )
		{
#ifdef UNIX_ENV
			usleep(AGCCALC_TIMESPAN*1000);
#else 
			m_FreshEventAux.Wait( AGCCALC_TIMESPAN);
#endif

			nInterval +=  AGCCALC_TIMESPAN;

		}

		if(m_bThreadRunning == false)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}
void agcCalc::handleYk(AGC_Digit *rec)
{
	//AGC�����ݸ��£��� Scada�ⲻ������ΪScada������ǰ�ø��¹���
	//�ڲ���������
	//�����źű仯
	memset(m_LastError,0,sizeof(m_LastError));

	if(strcmp(rec->strKey,g_AgcDigitKey[eAgcDigitOptionOn])==0)
	{
		//��������
		sprintf(m_LastError ,"�����ܹ���Ͷ��״̬�ı䣬����Ϊ%s",rec->iVal>0?"Ͷ��":"�˳�");

	}
	else if(strcmp(rec->strKey,g_AgcDigitKey[eAgcDigitCycleOn])==0)
	{
		//�ջ�����
		sprintf(m_LastError ,"��������Ͷ��״̬�ı䣬����Ϊ%s",rec->iVal>0?"Ͷ��":"�˳�");

	}
	writeBuffer(m_LastError);
	m_bNeedStopProcess = true;
}
//����ң��
void agcCalc::handleYt(AGC_Analog *rec)
{
	if(rec==NULL)
		return ;


}
agcFuncRtdb *agcCalc::getFuncAgcRtdb()
{
	return &m_funcRtdb;
}
bool agcCalc::isLaunched()
{
	bool ret=false;

	if(m_funcRtdb.getAgcDigit(eAgcDigitOptionOn)>0)
	{
		ret=true;
	}
	memset(m_LastError,0,sizeof(m_LastError));
	sprintf_s(m_LastError ,sizeof(m_LastError), "��ǰ�����ܹ���%s",ret==true?"Ͷ��":"δͶ��");
	writeBuffer(m_LastError);
	return ret;
}

//�Ƿ�Զ������
bool agcCalc::isExec()
{
	bool ret=false;

	if(m_funcRtdb.getAgcDigit(eAgcDigitCycleOn)>0)
	{
		ret=true;
	}
	return ret;
}






bool agcCalc::getCmdSetPointValue(bool bRemote,float &fRes)
{
	//�õ�
	// enmAgcAnalogKey key;

	//	fRes=m_funcRtdb.getAgcAnalog(key);
	return true;
}
int agcCalc::readCurrentDateTarget(float& nRes,bool flg,bool remoteflg)
{
	int temp_target = eCMD_MODE_SETPOINT;//���ʽ 1Ϊ�ƻ�����2Ϊң�����Ϊ���
	int ret=0;

	bool bRet=true;
	bRet=getCmdSetPointValue(0,nRes);
	return ret;
}

/*************************************************
������������: checkAdjustObject
  ����Ҫ����: ������ָ���Ƿ�Ϸ�
   �������: 
   �������: 
   �� �� ֵ��
   �޸ļ�¼:
   *************************************************/
bool agcCalc::checkAdjustObject(time_t currentTime,GeneratorUnit *p)
{
	if(p == NULL)
	{
		memset(m_LastError,0,sizeof(m_LastError));

		sprintf_s(m_LastError ,sizeof(m_LastError), "���%s����ָ�ͨ����ԭ��ָ��Ϊ��\n",p->p_Unit->strName);
		writeBuffer(m_LastError);

		return false;
	}

	if(p->adjustFlag != true)
	{
		memset(m_LastError,0,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError), "���%s����ָ�ͨ����ԭ������ʶΪ������\n",p->p_Unit->strName);
		writeBuffer(m_LastError);
		return false;
	}

	//int val = currentTime - p->lastAdjustTime;


	if(fabs(p->p_Unit->fDisPower - p->p_Unit->fValP) < p->p_Unit->fAdZone)
	{
		memset(m_LastError,0,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError), "���%s����ָ�ͨ��������ָ��%f����ǰ�й�%f�ľ���ֵ%fС�ڷ��͵�����������%f\n",p->p_Unit->strName,  p->p_Unit->fDisPower,  p->p_Unit->fValP,  fabs(p->p_Unit->fDisPower - p->p_Unit->fValP),  p->p_Unit->fAdZone);
		writeBuffer(m_LastError);
		return false;
	}
	if(fabs(p->p_Unit->fDisPower - p->lastObjectValue) <p->p_Unit->fAdZone)//��ԭ����С��
	{
		if(m_adParameter.getReSendFlg())
		{
			if(p->m_ReSendTimes<p->p_Unit->iAdTimesMax)
			{
				return true;
			}
			else 
			{
				memset(m_LastError,0,sizeof(m_LastError));
				sprintf_s(m_LastError ,sizeof(m_LastError), "���%s����ָ�ͨ����ԭ���ظ�������������ظ����ʹ���\n",p->p_Unit->strName);
				writeBuffer(m_LastError);

				return false;
			}
		}
		else
		{
			memset(m_LastError,0,sizeof(m_LastError));
			sprintf_s(m_LastError ,sizeof(m_LastError), "���%s����ָ�ͨ����������ͬ��ǰָ��=%.3f,�ϴ�ָ��%.3f,��С����=%.3f\n",p->p_Unit->strName,p->p_Unit->fDisPower,p->lastObjectValue,p->p_Unit->fAdZone);
			writeBuffer(m_LastError);
			return false;
		}
	}
	else if(p->p_Unit->fDisPower<p->p_Unit->fAdMinLimit)
	{
		memset(m_LastError,0,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError), "���%s����ָ�ͨ������ǰָ��=%.3f���ڵ�������=%.3f\n",p->p_Unit->strName,p->p_Unit->fDisPower,p->p_Unit->fAdMinLimit);
		writeBuffer(m_LastError);
		return false;
	}
	else
	{
		if(m_adParameter.getReSendFlg()&&p->m_ReSendTimes>=p->p_Unit->iAdTimesMax)
		{
			//p->m_ReSendTimes=0;
		}
	}


	return true;

}
void agcCalc::writeBuffer(char* buffer)
{
	//TRACE("%s\n",buffer);
	m_funcRtdb.pushAgcLog(buffer);
}

bool agcCalc::checkYtExists()
{
	bool ret=true;

	for(int i = 0 ; i < m_nGenCount ; i++)
	{
		if(m_pGenInfo[i]->enabled)
		{

			GeneratorUnit* wi=m_pGenInfo[i];

			if(g_agcCalc->getFuncAgcRtdb()->checkYtExists(wi->p_Unit->iChannelID,wi->p_Unit->iDevice,wi->p_Unit->iYtNo)==FALSE)
			{
				memset(m_LastError,0,sizeof(m_LastError));
				sprintf_s(m_LastError ,sizeof(m_LastError), "���%s���õ�ң��������ң�������ڶ�Ӧ��¼\n",wi->p_Unit->strName);
				writeBuffer(m_LastError);
				ret=false;
			}
		}
	}
	return ret;

}
// �����߳�
ThreadFuncRet agcCalc::FreshThreadProcAux(LPVOID lpParameter)
{
	agcCalc *agc = (agcCalc*)lpParameter;
	if(agc == NULL)
		return 0;

	//��ӡ
	int n_calInterval=0;
	while(agc->m_bThreadRunning == true)
	{		

		if(agc->waitForTimeout(CALC_TH_AuxKeyID,3000) == false)
		{
			TRACE("waitForTimeout break \n");
			break;
		}
		//agc->RefreshQ();
		//agc->m_lockFreshData.Lock();
		agc->freshdata();
		//agc->m_lockFreshData.UnLock();
	}
	TRACE("---fresh thread exit----\n");
	return 0;
}
ThreadFuncRet agcCalc::UpdataProcessPara(LPVOID lpParameter)
{
	agcCalc *runMgr = (agcCalc*)lpParameter;lpParameter;
	if(runMgr==NULL)
		return 0;

	CMultiDataBase *mdb=NULL;
	mdb=runMgr->getFuncAgcRtdb()->getMainDb();

	if(mdb==NULL)
		return 0;

	mdb->updateProcessTabPara(pt_AGC,TRUE);

	while(runMgr->getRunStatus()==eThreadRun)
	{
		MilliSleep(3000);
		printf("UpdataNodeParaTimer\n"); 
		mdb->updateNodeTabPara();
		mdb->updateProcessTabPara(pt_AGC,FALSE);
	}
	return 0;
}
Q_ThreadStatusType agcCalc::getRunStatus()
{
	return m_eCalcThreadStatus;
}
//��������Ƿ�Ϊ���� trueΪ����,falseΪ����
bool agcCalc::checkIsMaster()
{
	bool ret=false;
	if(m_funcRtdb.getServerBkType()==eSvrBkTypeMaster)
	{
		ret=true;
	}
	if(ret!=true)
	{
		memset(m_LastError,NULL,sizeof(m_LastError));
		sprintf_s(m_LastError,sizeof(m_LastError),"��ǰ����Ϊ������ֻˢ�����ݣ��������㷨\n");
		writeBuffer(m_LastError);	
	}
	return ret;
}

float agcCalc::modifyCurrentTarget(float dTarget,float dMaxOnePower,float curPower)
{
	//��ֹһ�ε��ڶ�����������ʡ��1���ӵ�����ֵҪ��(װ�������ĵ�10%)
	float dt = dTarget;
	float dlimit=dMaxOnePower;

	//��һ�ν���1���ӱ仯�ʿ��ƣ���Ҫ��ʵ�ʹ��ʸ�ֵ���������ֵ
	if (m_1minrectar < 0)
	{
		m_1minrectar = curPower;
	}
	time_t m_timeNow = time(NULL);//ȡ�õ�ǰʱ�����ϴμ�¼����ֵ��ʱ��Ƚϣ�����1���������¼�¼

	int diff=(int)(m_timeNow - m_t1minstmp);
	if (diff >= 60)
	{
		m_t1minstmp = m_timeNow;
		m_1minrectar = curPower;

	}

	if(dt > m_1minrectar)
	{
		float delt = dt - m_1minrectar;
		if(delt > dlimit)
			delt = dlimit;
		dt = m_1minrectar+ delt;
	}
	else
	{
		float delt = m_1minrectar- dt;
		if(delt > dlimit)
			delt = dlimit;
		dt = m_1minrectar - delt;
	}

	sprintf_s(m_LastError ,sizeof(m_LastError), "1���ӱ仯������(ʱ���ֵ=%d��):Ŀ��ֵ%.2f,������Ŀ��%.2fʵʱ����=%.2f,���ڲο�ֵ=%.2f,������ֵ=%.0fkW/����",diff,dTarget,dt,curPower,m_1minrectar,dMaxOnePower);
	writeBuffer(m_LastError);

	return dt;
}

//���ܳ�����
//dTarget��һ����ֵ
int agcCalc::pcsChargeMethod(float dTarget)
{

	float fChargeMax = m_funcRtdb.getAgcAnalog(eAgcAnalogSocChargeMax);//�������

	bool bReachSocMax=false;//�����Ƿ�ﵽ�������
	if(m_pSocInfo->p_Unit->iBStatus==Battery_Status_Full)
	{
		bReachSocMax=true;
	}
	else
	{
		if(m_funcRtdb.equalDouble(m_pSocInfo->p_Unit->fBatteryPower,fChargeMax)||
			m_pSocInfo->p_Unit->fBatteryPower>fChargeMax)
		{
			bReachSocMax=true;
		}

	}
	if(bReachSocMax)
	{
		sprintf_s(m_LastError ,sizeof(m_LastError), "���ܵ�ǰ����������ܴ�������");
		writeBuffer(m_LastError);

		pcsWaitMethod();//��������
		return AGC_SUC;
	}
	if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Charge)
	{
		//�����
		//������PĿ�깦�ʳ��
		m_pSocInfo->adjustFlag=true;
		m_pSocInfo->p_Unit->fDisPower=	m_pSocInfo->p_Unit->fValUp-dTarget;
	}
	else
	{
		if(m_pSocInfo->p_Unit->iStatus==SOC_PCS_RUN_STATUS_Run)
		{
			//��������Ӧ�Ƿ�ʱ
			bool flg=false;
			if(m_pSocInfo->p_Unit->fDisPower<0)
			{
				if(m_funcRtdb.equalDouble(m_pSocInfo->lastObjectValue,m_pSocInfo->p_Unit->fDisPower))
				{
					time_t now;
					time(&now);
					int per=now-m_pSocInfo->lastAdjustTime;
					if(per>10&&per<60)
					{

						m_pSocInfo->adjustFlag=false;
						sprintf_s(m_LastError ,sizeof(m_LastError), "����ִ�г��ָ����Ӧ���������ô������ź�pcs��Ӧ������Ϊ1");
						writeBuffer(m_LastError);
						m_funcRtdb.setAgcWarn(eAgcAlarmPcsFaultChargeNoExec, 1);

						flg=true;
					}
				}
			}
			if(!flg)
			{
				m_pSocInfo->adjustFlag=true;
				m_pSocInfo->p_Unit->fDisPower=	m_pSocInfo->p_Unit->fValUp-dTarget;
			}
		}
	}

	if(m_pSocInfo->adjustFlag)
	{
		if(m_pSocInfo->p_Unit->fDisPower<0)
		{
			float fMax = m_funcRtdb.getAgcAnalog(eAgcAnalogSocChargePMax);//�ŵ�����

			if(m_pSocInfo->p_Unit->fDisPower<-fMax)
			{
				m_pSocInfo->p_Unit->fDisPower=-fMax;
			}
		}
	}
	return AGC_SUC;
}
//���ܷŵ����
int agcCalc::pcsDischargeMethod(float dTarget)
{
	float fDischargeMin = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargeMin);//�ŵ�����



	if(m_funcRtdb.equalDouble(m_pSocInfo->p_Unit->fBatteryPower,fDischargeMin)||
		m_pSocInfo->p_Unit->fBatteryPower<fDischargeMin)
	{
		sprintf_s(m_LastError ,sizeof(m_LastError), "����ִ��ص���%.2f���ڷŵ�����%.2f�����ܴ���",m_pSocInfo->p_Unit->fBatteryPower,fDischargeMin);
		writeBuffer(m_LastError);

		pcsWaitMethod();//��������
		return AGC_SUC;
	}


	if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Discharge)
	{
		//�ŵ���
		//������PĿ�깦�ʷŵ�
		m_pSocInfo->adjustFlag=true;
		m_pSocInfo->p_Unit->fDisPower=	m_pSocInfo->p_Unit->fValUp+dTarget;
	}
	else
	{
		if(m_pSocInfo->p_Unit->iStatus==SOC_PCS_RUN_STATUS_Run)
		{
			//��������Ӧ�Ƿ�ʱ
			bool flg=false;
			if(m_pSocInfo->p_Unit->fDisPower>0)
			{
				if(m_funcRtdb.equalDouble(m_pSocInfo->lastObjectValue,m_pSocInfo->p_Unit->fDisPower))
				{
					time_t now;
					time(&now);
					int per=now-m_pSocInfo->lastAdjustTime;
					if(per>10&&per<60)
					{
						m_pSocInfo->adjustFlag=false;
						sprintf_s(m_LastError ,sizeof(m_LastError), "����ִ�зŵ�ָ����Ӧ���������ô������ź�pcs��Ӧ������Ϊ1");
						writeBuffer(m_LastError);
						m_funcRtdb.setAgcWarn(eAgcAlarmPcsFaultDischargeNoExec, 1);
						flg=true;
					}
				}
			}

			if(!flg)
			{
				m_pSocInfo->adjustFlag=true;

				m_pSocInfo->p_Unit->fDisPower=	m_pSocInfo->p_Unit->fValUp+dTarget;
			}
		}
	}
	if(m_pSocInfo->adjustFlag)
	{
		if(m_pSocInfo->p_Unit->fDisPower>0)
		{
			float fMax = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargePMax);//�ŵ�����

			if(m_pSocInfo->p_Unit->fDisPower>fMax)
			{
				m_pSocInfo->p_Unit->fDisPower=fMax;
			}
		}
	}
	return AGC_SUC;
}
//���ܴ�������
int agcCalc::pcsWaitMethod()
{
	if(m_funcRtdb.equalDouble(m_pSocInfo->p_Unit->fValUp,0))
	{
		m_pSocInfo->adjustFlag=false;
		return AGC_SUC;
	}
	else
	{
		if(m_funcRtdb.equalDouble(m_pSocInfo->lastObjectValue,0)&&m_funcRtdb.equalDouble(m_pSocInfo->p_Unit->fDisPower,0) )
		{
			time_t now;
			time(&now);

			int per=now-m_pSocInfo->lastAdjustTime;
			if(per>10&&per<60)
			{
				sprintf_s(m_LastError ,sizeof(m_LastError), "����ִ�д���ָ����Ӧ��ʱ���ô������ź�pcs��Ӧ������Ϊ1");
				writeBuffer(m_LastError);
				m_funcRtdb.setAgcWarn(eAgcAlarmPcsFaultWaitNoExec, 1);
			}
			else
			{
				m_pSocInfo->p_Unit->fDisPower=0;
				m_pSocInfo->adjustFlag=true;
			}

		}
		else
		{
			m_pSocInfo->p_Unit->fDisPower=0;
			m_pSocInfo->adjustFlag=true;
		}


	}
	return AGC_SUC;
}
//���ܿ�������
int agcCalc::pcsStartMethod()
{
	if(m_pSocInfo->p_Unit->iStatus==SOC_PCS_RUN_STATUS_Run)
	{
		m_pSocInfo->m_FlgSendStart=false;
		return AGC_SUC;
	}
	else
	{
		if(m_pSocInfo->m_FlgSendStart)
		{
			if(m_pSocInfo->m_ReSendTimes>2)
			{
				m_pSocInfo->m_FlgSendStart=false;

				sprintf_s(m_LastError ,sizeof(m_LastError), "����ִ�п���ָ�����������%d�ζ�ʧ���ˣ��ô������ź�pcs��Ӧ������Ϊ1",m_pSocInfo->m_ReSendTimes);
				writeBuffer(m_LastError);
				m_funcRtdb.setAgcWarn(eAgcAlarmPcsFaultStartNoExec, 1);
				m_pSocInfo->m_ReSendTimes=0;
			}
			else
			{
				m_pSocInfo->m_ReSendTimes++;
			}
		}
		else
		{
			m_pSocInfo->m_FlgSendStart=true;
			m_pSocInfo->m_ReSendTimes=0;
		}
	}
	return AGC_SUC;
}
//ͨ�ñ������
int agcCalc::commonVaribleCheck()
{
	return AGC_SUC;
}
/*************************************************
������������: checkAdjustObject
  ����Ҫ����: ������ָ���Ƿ�Ϸ�
   �������: 
   �������: 
   �� �� ֵ��
   �޸ļ�¼:
   *************************************************/
bool agcCalc::checkAdjustObjectPcs(time_t currentTime,GeSocUnit *p)
{
	if(p == NULL)
	{
		memset(m_LastError,0,sizeof(m_LastError));

		sprintf_s(m_LastError ,sizeof(m_LastError), "���%s����ָ�ͨ����ԭ��ָ��Ϊ��\n",p->p_Unit->strName);
		writeBuffer(m_LastError);

		return false;
	}

	if(p->adjustFlag != true)
	{
		memset(m_LastError,0,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError), "���%s����ָ�ͨ����ԭ������ʶΪ������\n",p->p_Unit->strName);
		writeBuffer(m_LastError);
		return false;
	}

	//int val = currentTime - p->lastAdjustTime;


	if(fabs(p->p_Unit->fDisPower - p->p_Unit->fValP) < p->p_Unit->fAdZone)
	{
		memset(m_LastError,0,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError), "���%s����ָ�ͨ��������ָ��%f����ǰ�й�%f�ľ���ֵ%fС�ڷ��͵�����������%f\n",p->p_Unit->strName,  p->p_Unit->fDisPower,  p->p_Unit->fValP,  fabs(p->p_Unit->fDisPower - p->p_Unit->fValP),  p->p_Unit->fAdZone);
		writeBuffer(m_LastError);
		return false;
	}
	if(fabs(p->p_Unit->fDisPower - p->lastObjectValue) <p->p_Unit->fAdZone)//��ԭ����С��
	{
		if(m_adParameter.getReSendFlg())
		{
			if(p->m_ReSendTimes<3)
			{
				return true;
			}
			else 
			{
				memset(m_LastError,0,sizeof(m_LastError));
				sprintf_s(m_LastError ,sizeof(m_LastError), "���%s����ָ�ͨ����ԭ���ظ�������������ظ����ʹ���\n",p->p_Unit->strName);
				writeBuffer(m_LastError);

				return false;
			}
		}
		else
		{
			memset(m_LastError,0,sizeof(m_LastError));
			sprintf_s(m_LastError ,sizeof(m_LastError), "���%s����ָ�ͨ����������ͬ��ǰָ��=%.3f,�ϴ�ָ��%.3f,��С����=%.3f\n",p->p_Unit->strName,p->p_Unit->fDisPower,p->lastObjectValue,p->p_Unit->fAdZone);
			writeBuffer(m_LastError);
			return false;
		}
	}

	float fChargeMax = m_funcRtdb.getAgcAnalog(eAgcAnalogSocChargePMax);//������
	float fDischargeMax = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargePMax);//�ŵ����

	if(p->p_Unit->fDisPower<0)
	{
		if(p->p_Unit->fDisPower<-fChargeMax)
		{
			memset(m_LastError,0,sizeof(m_LastError));
			sprintf_s(m_LastError ,sizeof(m_LastError), "���%s����ָ�ͨ������ǰָ��=%.2f���ڳ�������%.2f\n",p->p_Unit->strName,p->p_Unit->fDisPower,fChargeMax );
			writeBuffer(m_LastError);
			return false;
		}

	}
	else if(p->p_Unit->fDisPower>0)
	{
		if(p->p_Unit->fDisPower>fDischargeMax)
		{	
			memset(m_LastError,0,sizeof(m_LastError));
			sprintf_s(m_LastError ,sizeof(m_LastError), "���%s����ָ�ͨ������ǰָ��=%.3f���ڷŵ������=%.3f\n",p->p_Unit->strName,p->p_Unit->fDisPower,fDischargeMax );
			writeBuffer(m_LastError);
			return false;

		}
	}

	if(m_adParameter.getReSendFlg()&&p->m_ReSendTimes>=3)
	{
		p->m_ReSendTimes=0;
	}



	return true;

}

int agcCalc::cheakCommonVarible()
{
	if(m_pSocInfo->p_Unit->fValUp>0)
	{
		m_funcRtdb.setAgcWarn(eAgcAlarmPcsFaultChargeNoExec, 0);
	}
	else if(m_pSocInfo->p_Unit->fValUp>0)
	{
		m_funcRtdb.setAgcWarn(eAgcAlarmPcsFaultDischargeNoExec, 0);
	}
	else if(m_funcRtdb.equalDouble(m_pSocInfo->p_Unit->fValUp,0)==0)
	{
		m_funcRtdb.setAgcWarn(eAgcAlarmPcsFaultWaitNoExec, 0);
	}
	if(m_pSocInfo->p_Unit->iStatus==SOC_PCS_RUN_STATUS_Run)
	{
		m_funcRtdb.setAgcWarn(eAgcAlarmPcsFaultStartNoExec, 0);
	}
	return 0;

}
//����ֵ0���� 1��� 2 ���
int agcCalc::getCurrentChargetOrDis()
{
	int nChargeBeginHour = (int)m_funcRtdb.getAgcAnalog(eAgcAnalogChargeBeginHour);
	int nChargeBeginMin = (int)m_funcRtdb.getAgcAnalog(eAgcAnalogChargeBeginMin);
	int nChargeEndHour =(int) m_funcRtdb.getAgcAnalog(eAgcAnalogChargeEndHour);
	int nChargeEndMin = (int)m_funcRtdb.getAgcAnalog(eAgcAnalogChargeEndMin);
	int nDischargeBeginHour = (int)m_funcRtdb.getAgcAnalog(eAgcAnalogDischargeBeginHour);
	int nDischargeBeginMin =(int) m_funcRtdb.getAgcAnalog(eAgcAnalogDischargeBeginMin);
	int nDischargeEndHour =(int) m_funcRtdb.getAgcAnalog(eAgcAnalogDischargeEndHour);
	int nDischargeEndMin = (int)m_funcRtdb.getAgcAnalog(eAgcAnalogDischargeEndMin);


	SYSTEMTIME now;
	GetLocalTime(now);

	SYSTEMTIME*	 pTime = (SYSTEMTIME *)&now;
	char cDisplay[52];
	memset(cDisplay, NULL, sizeof(cDisplay));

	sprintf(cDisplay, "%d-%d-%d %d:%d:%d",
		pTime->wYear, pTime->wMonth, pTime->wDay,
		pTime->wHour, pTime->wMinute, pTime->wSecond);

	memset(m_LastError,0,sizeof(m_LastError));
	sprintf(m_LastError ,"���ʱ��[%d:%d-%d:%d],�ŵ�ʱ��[%d:%d-%d:%d],��ǰʱ��[%s]",nChargeBeginHour,nChargeBeginMin,nChargeEndHour,nChargeEndMin,nDischargeBeginHour,nDischargeBeginMin,nDischargeEndHour,nDischargeEndMin,cDisplay);
	writeBuffer(m_LastError);

	int nNowMin=now.wHour*60+now.wMinute;

	int nChargeBegin=nChargeBeginHour*60+nChargeBeginMin;
	int nChargetEnd=nChargeEndHour*60+nChargeEndMin;
	if(nNowMin>nChargeBegin&& nNowMin<nChargetEnd)
	{
		//���
		return 1;
	}

	int nDischargeBegin=nDischargeBeginHour*60+nDischargeBeginMin;
	int nDischargetEnd=nDischargeEndHour*60+nDischargeEndMin+24*60;

	if(nNowMin>nDischargeBegin&&nNowMin<nDischargetEnd)
	{
		return 2;
	}
	

	return 0;

}
float agcCalc::getNormalChargePower()
{
	float fChargetKw =m_funcRtdb.getAgcAnalog(eAgcAnalogSocChargekW);

	float fPhvSum=m_funcRtdb.getAgcAnalog(eAgcAnalogPhvSumPower);
	float fFactor=m_funcRtdb.getAgcAnalog(eAgcAnalogSocChPhvFactor);

	float ret=fChargetKw ;
	if(fChargetKw <fPhvSum*fFactor)
	{
		float n=fPhvSum*fFactor/10;
		if(n*10>fChargetKw)
		{
			ret=n*10.0;
		}
	}

	
	if(ret>0&& ret<m_pSocInfo->p_Unit->fcurCap && ret<fPhvSum*fFactor)
	{
		return ret;
	}
	else
	{
		
		memset(m_LastError,0,sizeof(m_LastError));
		sprintf(m_LastError ,"��繦�ʼ����߼��������飬�ó�繦��Ϊ20kW,����Ԥ�ڽ��%.2f,����ڹ������繦��%.2f,���ܶ%.2f",ret,fPhvSum*fFactor,m_pSocInfo->p_Unit->fcurCap);
		writeBuffer(m_LastError);
		ret=20;
	}
	return ret;
}

#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif