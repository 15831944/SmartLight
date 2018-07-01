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
				savePeriod = 5; //默认5S保存一次
			}
			m_pAgcRtdbSaveAnalog->SetDelay(savePeriod);
			m_pAgcRtdbSaveAnalog->Start();

			memset(m_LastError,NULL,sizeof(m_LastError));
			sprintf(m_LastError ,"数据记录功能开启,间隔%d秒,保存遥测表、遥信表\n",savePeriod);			
			writeBuffer(m_LastError);

		}
		else
		{
			printf("CMultiDataBase not init\r\n");
		}
	}

	{ //定期删除n天以前的日志
		m_pDeleteLog = new DeleteLog();
		m_pDeleteLog->setLogLastDay(45); //删除45天以前的日志，只删除从当前时间算起，第45天的日志文件
		int n_secs=60;
		m_pDeleteLog->setInterval(n_secs*1000);//60秒执行一次
		const char *pWorkPath=getenv(HOME_NAME);
		char cPath[200] = {0};
		sprintf(cPath,"%s%s", pWorkPath, agcDataLogPath);
		char cPathLog[200] = {0};
		sprintf(cPathLog,"%s%s", cPath, "####-@@-$$-agclogfile.log");//使用####替换文件名字里边的年，@@替换文件名字里边的月，$$替换文件名字里边的日期
		m_pDeleteLog->addFile(cPathLog);//添加要删除的日志文件的目录及文件名
		memset(cPathLog,0,sizeof(cPathLog));
		sprintf(cPathLog,"%s%s", cPath, "####-@@-$$_AGC_Analog.txt");
		m_pDeleteLog->addFile(cPathLog);
		memset(cPathLog,0,sizeof(cPathLog));
		sprintf(cPathLog,"%s%s", cPath, "####-@@-$$_AGC_Digit.txt");
		m_pDeleteLog->addFile(cPathLog);
		memset(m_LastError,NULL,sizeof(m_LastError));
		sprintf(m_LastError ,"定期删除功能开启,间隔%d秒,删除%d天前记录\n",n_secs,45);			
		writeBuffer(m_LastError);
		m_pDeleteLog->start();
	}
	m_funcRtdb.initAgcAlarmValue();
	return true;
}
// 计算线程
ThreadFuncRet agcCalc::CalcThreadProc(LPVOID lpParameter)
{
	agcCalc *agc = (agcCalc*)lpParameter;
	if(agc == NULL)
		return 0;

	//打印
	int n_calInterval=0;
	agc->m_bNeedStopProcess = true;//第一次运行的立即执行
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
		//打印一大波AGC计算结束;


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

	//初使化机组
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


	//初使化机组
	m_nSocCount=m_funcRtdb.getAgcDb()->getRecordCountByID(AGC_TAB_PhvUnit_ID);
	//目前就支持一个得了
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
	TRACE("<==MT:收到消息==>\n");
	//m_bNeedStopProcess = true; //??，接到信号后直接置位重新开始计算标识，重新刷新数据开始计算
	switch(message->m_iMsgType) 
	{
	case mes_RCCancelSelect: //遥控
	case mes_RCSelect:
	case mes_RCExecute: 
		{			
			RControl *rc = (RControl *)(message->m_bMes);
			MesBody mesbody;
			mesbody.comm.type = MES_TYPE_RELAY;

			if (message->m_iMsgType == mes_RCExecute)
			{
				mesbody.relay.cmd = RELAY_EXECUTE;
				TRACE("<==MT:遥控执行==>\n");
			}
			else  
			{
				mesbody.relay.cmd = RELAY_CANCEL;
				TRACE("<==MT:遥控撤消==>\n");
			}

			mesbody.comm.destination = rc->iStation;//站号
			mesbody.relay.check = MES_CHECK_NO;
			mesbody.relay.point = rc->iYKPoint;//点号
			mesbody.relay.value = rc->iVal;
			mesbody.relay.state = CHECK_FAIL;
			mesbody.relay.device=rc->iDevice;//设备号

			AGC_Digit *rec=NULL;
			if(g_agcCalc->getFuncAgcRtdb()->findYk(rc,rec))
			{
				TRACE("在AGC记录表中找到了遥控记录\n");
				GetLocalTime(rec->tSnapTime);
				rec->iVal=rc->iVal;//更新值
				g_agcCalc->handleYk(rec);
			}
			else
			{
				TRACE("在AGC_Digit表中没有找匹配的记录，或者在Scada库的遥控表没有正确配置\n");
			}


			//smap_append_message(&mesbody);
		}
		break;
	case mes_SPSelect:
	case mes_SPExecute: 
		{

			//if (m_pGlobal->GetWorkState() == FEP_PRIMARY) { //tjs del 因为没有m_pGlobal变量
			if(1==1)
			{
				RSetpoint *rs = (RSetpoint *)(message->m_bMes);

				MesBody mesbody;
				mesbody.comm.type = MES_TYPE_SPOINT;
				if (message->m_iMsgType == mes_RCSelect) 
				{
					mesbody.relay.cmd = RELAY_SELECT;
					TRACE("<==MT:遥调选择==>\n");
				}
				else if (message->m_iMsgType == mes_RCExecute)
				{
					mesbody.relay.cmd = RELAY_EXECUTE;
					TRACE("<==MT:遥调执行==>\n");
				}

				mesbody.comm.destination = rs->iStation;
				mesbody.spoint.point = rs->iPoint;
				mesbody.spoint.val = rs->fVal;
				mesbody.spoint.device=rs->iDevice;

				AGC_Analog *rec=NULL;
				if(g_agcCalc->getFuncAgcRtdb()->findYt(rs,rec))
				{
					TRACE("在AGC_Aanlog记录表中找到了遥调记录\n");
					GetLocalTime(rec->tSnapTime);
					rec->fVal=rs->fVal;//更新值
					g_agcCalc->handleYt(rec);
				}
				else
				{
					TRACE("在AGC_Analog表中没有找匹配的记录，或者在Scada库的遥调表没有正确配置\n");
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
//刷新数据
void agcCalc::freshdata()
{
	//memset(m_LastError,0,sizeof(m_LastError));
	//sprintf(m_LastError ,"%s","刷新数据运行");
	//writeBuffer(m_LastError);

	m_lockFreshData.Lock();
	m_funcRtdb.freshPhvUnit();
	m_funcRtdb.freshSocUnit();
	m_funcRtdb.freshDigit();
	m_funcRtdb.freshAnlog();
	m_lockFreshData.UnLock();

}
//得到命令
int agcCalc::getCommand()
{

	float nTarget=0;
	int nRet=0;
	nRet=readCurrentDateTarget(nTarget,false,true);
	m_getTarget = nTarget;//保存发电目标量

	if (nRet<0)
	{
		strcpy(m_LastError,"读取实时目标值时发生错误，设置闭锁有功。");
		writeBuffer(m_LastError);
	}
	else
	{

	}
	memset(m_LastError,0,sizeof(m_LastError));
	sprintf(m_LastError ,"得到当前命令%.2f",nTarget);
	writeBuffer(m_LastError);

	return true;
}
//闭锁检查
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
			sprintf(m_LastError ,"闭锁，程序初启动，等待%d秒正式运行算法,现在还需%d秒\n",firstStartWait,firstStartWait-(now-m_timFirstStart));			
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
	if(iChannelBroken == 0)//如果通道恢复链接
	{ 
		sprintf(m_LastError ,"采集数据通道断开,闭锁不执行");			
		writeBuffer(m_LastError);

		ret=true;
	}


	float fUa = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUa);
	float fUb = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUb);
	float fUc = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUc);
	float fULowLimit = m_funcRtdb.getAgcAnalog(eAgcAnalogULimit);//最低限值


	if(fUa<fULowLimit && fUb<fULowLimit && fUc<fULowLimit)
	{
		//发送离网的报警
		m_funcRtdb.setAgcWarn(eAgcAlarmOffNet, 1);
		sprintf(m_LastError ,"并网点电压值小于下限%.2f判定系统离网",fULowLimit);			
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
	sprintf(m_LastError ,"闭锁检查%s",ret==false?"通过":"不通过");
	writeBuffer(m_LastError);

	return ret;
}
//安全约束检查
bool agcCalc::isSafeConstraint()
{

	if(m_funcRtdb.getCommAdParam(eAgcParamSafeCheckFlg) <=0) //如果进行安全约束检测
	{
		memset(m_LastError,0,sizeof(m_LastError));		
		sprintf(m_LastError ,"%s","不执安全约束检查");
		writeBuffer(m_LastError);
		return false;
	}
	bool ret=false;


	//判断一下储能的状态
	if(m_pSocInfo!=NULL)
	{

		if(m_pSocInfo->p_Unit->fBatteryPower>100||m_pSocInfo->p_Unit->fBatteryPower<0)
		{
			//储能剩余电量异常请核实
			memset(m_LastError,0,sizeof(m_LastError));		
			sprintf(m_LastError ,"储能剩余电量异常请核实,应该在[0-100]之间,当前值%.3f",m_pSocInfo->p_Unit->fBatteryPower);
			writeBuffer(m_LastError);
			ret=true;
		}



		if(m_pSocInfo->p_Unit->iExpStatus==SOC_PCS_EXP_STATUS_Fault)
		{
			sprintf(m_LastError ,"储能故障状态为故障，请检查");
			writeBuffer(m_LastError);
			m_funcRtdb.setAgcWarn(eAgcAlarmPcsFault, 1);
		}
		else
		{
			m_funcRtdb.setAgcWarn(eAgcAlarmPcsFault, 0);
		}


		float fDischargeMaxPower = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargeKw);//储能放电最大功率
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

		float fChargeMaxPower = m_funcRtdb.getAgcAnalog(eAgcAnalogSocChargekW);//储能充电最大功率

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
	sprintf(m_LastError ,"安全约束检查%s",ret==false?"通过":"不通过");
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
	float fULowLimit = m_funcRtdb.getAgcAnalog(eAgcAnalogULimit);//最低限值

	

	//TODO:如果储能未运行的话，我们能做什么
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

		//稳定控制策略
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
//稳定控制策略
int agcCalc::disNormalMethod()
{
	if(m_pSocInfo==NULL)
	{
		return AGC_FAIL;
	}

	memset(m_LastError,0,sizeof(m_LastError));
	sprintf(m_LastError ,"执行稳定控制策略");
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
					//同时此值
					pcsControlMethod(-fChargetKw);
				}

			}
			else if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Discharge)
			{
				//待机
				pcsControlMethod(0);
			}
			else
			{
				//即不充电也不放电
				//以固定功率充电
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
					//同时此值
					pcsControlMethod(fDischargetKw);
				}

			}
			else if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Charge)
			{
				//待机
				pcsControlMethod(0);
			}
			else
			{
				//即不充电也不放电
				//以固定功率充电
				float fDischargetKw = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargeKw);
				pcsControlMethod(fDischargetKw);
			}
		}
	}
	
	return AGC_SUC;

}
//储能控制策略
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
			//待机
			pcsWaitMethod();
		}
		else if(dTarget>0)
		{
			//放电
			pcsDischargeMethod(dTarget);
		}
		else
		{
			//保证传过去正值
			pcsChargeMethod(0-dTarget);
		}
	}

	return AGC_SUC;
}
//升压策略
int agcCalc::UpVoltage()
{
	memset(m_LastError,0,sizeof(m_LastError));
	sprintf(m_LastError ,"执行升压控制策略");
	writeBuffer(m_LastError);

	if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Charge)
	{
		//待机
		pcsControlMethod(0);

	}
	else if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Discharge)
	{
		float fDischargeMin = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargeMin);//放电下限

		if(m_funcRtdb.equalDouble(m_pSocInfo->p_Unit->fBatteryPower,fDischargeMin)||
			m_pSocInfo->p_Unit->fBatteryPower<fDischargeMin)
		{
			return AGC_SUC;
		}
		else
		{

			float fDischargeMaxPower = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargeKw);//储能放电最大功率
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
				sprintf(m_LastError ,"当前有功设定%.2f大于等于放电最大%.2f,判定此时储能放电最大，置报警负荷超载",m_pSocInfo->p_Unit->fValP,fDischargeMaxPower);
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
			sprintf(m_LastError ,"储能不执行放电命令，置报警负荷超载");
			writeBuffer(m_LastError);

				m_funcRtdb.setAgcWarn(eAgcAlarmOverLoad, 1);
		}
	}
	return AGC_SUC;
}
//降压策略
int agcCalc::DownVoltage()
{
	memset(m_LastError,0,sizeof(m_LastError));
	sprintf(m_LastError ,"执行降压控制策略");
	writeBuffer(m_LastError);

	if(m_pSocInfo->p_Unit->iStatus==SOC_PCS_RUN_STATUS_Run)
	{
		if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Charge)
		{
			float fChargeMaxPower = m_funcRtdb.getAgcAnalog(eAgcAnalogSocChargekW);//储能充电最大功率

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
				sprintf_s(m_LastError,sizeof(m_LastError),"母线电压偏高，储能达到最大充电功率%.2fkW,当前储能实时有功%.2fkW",fChargeMaxPower,m_pSocInfo->p_Unit->fValP);
				writeBuffer(m_LastError);

				//TODO:启用光伏限功率


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
		//待机
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

	//TODO:如果储能未运行的话，我们能做什么
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

	float fft = m_funcRtdb.getAgcAnalog(eAgcAnalogFtPowerU);//电压对无功系数
	float fModify = m_funcRtdb.getAgcAnalog(eAgcAnalogAdVoltModify);//电压偏差修正

	float fDisPower=-(fURef-fUMax-fModify)*fft;
	return  fDisPower;
}
//计算放电功率
float agcCalc::calcDischargePower()
{
	float fUa = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUa);
	float fUb = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUb);
	float fUc = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUc);
	float fUMin = m_funcRtdb.getAgcAnalog(eAgcAnalogNetUMin);

	//TODO:如果储能未运行的话，我们能做什么
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

	float fft = m_funcRtdb.getAgcAnalog(eAgcAnalogFtPowerU);//电压对无功系数
	float fModify = m_funcRtdb.getAgcAnalog(eAgcAnalogAdVoltModify);//电压偏差修正

	float fDisPower=(fUMin-fURef+fModify)*fft;
	return  fDisPower;
}
//分配
void agcCalc::distribute_phv()
{

	//发电量分配算法
	float dTarget = m_getTarget;
	//	m_funcRtdb.uptAlgRecordBykW(eAgcAnalogRealPlan, m_getTarget);
	TRACE("dTarget:%f\n", dTarget);
	float nMaxRadio=100;
	// clear
	TRACE("m_nGenCount:%d\n", m_nGenCount);
	for(int i = 0 ; i < m_nGenCount ; i++)
	{
		m_pGenInfo[i]->m_ReSendTimes = 0; //清零重复发生次数
		m_pGenInfo[i]->adjustFlag = false;
		//初始化时为了防止异常发生让满发
		m_pGenInfo[i]->p_Unit->fDisPower = m_pGenInfo[i]->p_Unit->fcurCap;//假设最大有功对应容量上限(KW)
		m_pGenInfo[i]->ratio = (int)(ceil(nMaxRadio/m_pGenInfo[i]->p_Unit->fFactor));//还没有比率，即 pObject/pRating * 100,0~100
	}
	// read target
	float Ptarget = dTarget;
	float PMaxSum = 0;     //可调节的最大功率
	int nAdjustNum = 0;     //可调节的逆变器数量
	float PRealSum = 0;    // 实际总出力
	float PRealRating = 0; //可调节逆变器开机容量
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
		//逆变器非运行状态
		if(m_pGenInfo[i]->p_Unit->iStatus != 1) //?还没有初始化
		{
			//TRACE("m_pGenInfo:%d没有初始化\n", i);
			continue;
		}
		//逆变器不支持调节
		if(m_pGenInfo[i]->canAdjust == false )
		{
			Ptarget -= m_pGenInfo[i]->p_Unit->fValP; //fValP 假设实际出力(KW)就是有功
			TRACE("Ptarget:%f,m_pGenInfo[i]->p_Unit->fValP:%f\n", Ptarget, m_pGenInfo[i]->p_Unit->fValP);
			m_fnoCanAdjust+=m_pGenInfo[i]->p_Unit->fValP;//fValP 假设实际出力(KW)就是有功
			TRACE("m_pGenInfo[%d]->canAdjust == false\n", i);
			continue;
		}

		//逆变器可调
		PMaxSum += m_pGenInfo[i]->p_Unit->fcurCap;//假设最大有功对应容量上限(KW)
		nAdjustNum++;
		PRealSum += m_pGenInfo[i]->p_Unit->fValP; //fValP 假设实际出力(KW)就是有功
		PRealRating += m_pGenInfo[i]->p_Unit->fcurCap;//假设最大有功对应容量上限(KW)
	}
	memset(m_LastError,NULL,sizeof(m_LastError));
	sprintf_s(m_LastError,sizeof(m_LastError),"%d 台可调逆变器",nAdjustNum);
	writeBuffer(m_LastError);
	if(nAdjustNum == 0) //  无可调逆变器
	{
		TRACE("nAdjustCur==0\n");
		return;
	}

	float dDeadZone=0;
	//dDeadZone = m_funcRtdb.getAgcAnalog(eAgcAnalogAdDeadZone);

	float dValue = dTarget;//去掉线损?
	float dConnectP = 0;//m_funcRtdb.getAgcAnalog(eAgcAnalogNetPower);//并网点有功
	if((fabs(dValue - dConnectP) < dDeadZone) && (dValue >= dConnectP))//当调度命令减并网点有功的绝对值小于死区，并且并网点有功小于调度命令时才不调节
	{
		memset(m_LastError,NULL,sizeof(m_LastError));
		sprintf_s(m_LastError,sizeof(m_LastError),"=====调节目标差小于调节死区=====");
		writeBuffer(m_LastError);
		TRACE("调节目标差小于调节死区\n");
		return;	// 
	}
	float v_tTargetForDiffSrc=Ptarget;//处理数据不同源使用


	//float nMaxRadio=100;
	//2015-3-8 当把m_loss改成-500的时候，会出现满发的时间不能发100的时候。？？李乾乾
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
	sprintf_s(m_LastError ,sizeof(m_LastError),"有功目标 %.2f , 当前有功%.2f , 不可调节%.2f \r\n",Ptarget, dConnectP, m_fnoCanAdjust);
	writeBuffer(m_LastError);



	if(Ptarget > dConnectP - m_fnoCanAdjust)  // 上调, 并网点有功,在上边Ptarget已经减去不可调逆变器总有功，这里Ptarget是指还需要调节的总功率
	{

		memset(m_LastError,NULL,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError),"上升有功。逆变器目标： %.2f \r\n",Ptarget);
		writeBuffer(m_LastError);
		float pTargetSum = Ptarget;
		float pRealRating = PRealRating;
		float Pit = pTargetSum / nAdjustNum;		// 平均调节目标值
		float Pir = pRealRating / nAdjustNum;		// 平均最大容量
		int nAdjustCur = nAdjustNum;	// 最终要调节的逆变器数量
		float PitTmp=Pit;
		for(int i = 0 ; i < m_nGenCount ; i++)
		{
			if(m_pGenInfo[i]->p_Unit->iStatus != 1 || m_pGenInfo[i]->canAdjust == false)
				continue;

			Pit=PitTmp;
			Pit *= (m_pGenInfo[i]->p_Unit->fcurCap / Pir);
			if(m_pGenInfo[i]->p_Unit->fValP >= Pit)  // 当实际发电量大于目标值时，不调此逆变器  //fValP 假设实际出力(KW)就是有功
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
		sprintf_s(m_LastError ,sizeof(m_LastError),"需要调节逆变器数量%d \r\n",nAdjustCur);
		writeBuffer(m_LastError);
		if(nAdjustCur <= 0)
		{
			TRACE("nAdjustCur<=0\n");
			return ;
		}
		float aa = pTargetSum / nAdjustCur;		// 最终调节的平均值
		float bb = pRealRating / nAdjustCur;	// 最终平均最大容量
		float PadjustTmp=aa;
		for(int i = 0 ; i < m_nGenCount ; i++)
		{
			aa=PadjustTmp;//tjs update 20120905因为当逆变器的最大功率为不同时发送同一个最大功率的逆变器功率会有不同。
			if(m_pGenInfo[i]->adjustFlag == true)
			{
				aa *= (m_pGenInfo[i]->p_Unit->fcurCap / bb);
				m_pGenInfo[i]->p_Unit->fDisPower = aa;
				m_pGenInfo[i]->ratio = (int)ceil((( m_pGenInfo[i]->p_Unit->fDisPower / m_pGenInfo[i]->p_Unit->fcurCap ) * nMaxRadio+m_pGenInfo[i]->p_Unit->iModifyAdd)/m_pGenInfo[i]->p_Unit->fFactor);
				memset(m_LastError,NULL,sizeof(m_LastError));
				sprintf_s(m_LastError ,sizeof(m_LastError),"上升 %d 号逆变器分配目标值为 %.2f\r\n",m_pGenInfo[i]->p_Unit->ID32,m_pGenInfo[i]->p_Unit->fDisPower);
				writeBuffer(m_LastError);
			}
		}
		return;
	}
	else // 下调
	{

		//if(!m_bCloseDiffSourceHandle)
		//	Ptarget=v_tTargetForDiffSrc;

		memset(m_LastError,NULL,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError),"下调逆变器有功目标 %.2f \r\n",Ptarget);
		writeBuffer(m_LastError);
		float pTargetSum = Ptarget;
		float pRealRating = PRealRating;
		float Pit = pTargetSum / nAdjustNum;		// 平均调节目标值
		float Pir = pRealRating / nAdjustNum;		// 平均最大容量
		int nAdjustCur = nAdjustNum;				// 要调节的逆变器数量

		float PitTmp = Pit;
		memset(m_LastError,NULL,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError),"单台目标 %.2f \r\n",PitTmp);
		writeBuffer(m_LastError);
		for(int i = 0; i < m_nGenCount; i++)
		{
			if(m_pGenInfo[i]->p_Unit->iStatus != 1 || m_pGenInfo[i]->canAdjust == false)
			{
				memset(m_LastError,NULL,sizeof(m_LastError));
				sprintf_s(m_LastError ,sizeof(m_LastError),"%d 号逆变器不可调 \r\n",i);
				writeBuffer(m_LastError);
				continue;
			}

			Pit=PitTmp;
			Pit *= (m_pGenInfo[i]->p_Unit->fcurCap / Pir);
			if(m_pGenInfo[i]->p_Unit->fValP <= Pit)  // 当实际发电量小于目标值时，不调此逆变器
			{
				pTargetSum -= m_pGenInfo[i]->p_Unit->fValP;
				nAdjustCur--;
				pRealRating -= m_pGenInfo[i]->p_Unit->fcurCap;
				m_pGenInfo[i]->adjustFlag = false;
				memset(m_LastError,NULL,sizeof(m_LastError));
				sprintf_s(m_LastError ,sizeof(m_LastError),"%d 号逆变器有功 %.2f 目标%.2f 不调节\r\n",m_pGenInfo[i]->p_Unit->ID32,m_pGenInfo[i]->p_Unit->fValP,Pit);
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


		float aa = pTargetSum / nAdjustCur;		// 最终调节的平均值
		float bb = pRealRating / nAdjustCur;	// 最终平均最大容量
		float PadjustTmp=aa;//tjs update 20120905因为当逆变器的最大功率为不同时发送同一个最大功率的逆变器功率会有不同。
		for(int i = 0 ; i < m_nGenCount ; i++)
		{
			aa=PadjustTmp; 
			if(m_pGenInfo[i]->adjustFlag== true)
			{
				aa *= (m_pGenInfo[i]->p_Unit->fcurCap / bb);
				m_pGenInfo[i]->p_Unit->fDisPower = aa;
				m_pGenInfo[i]->ratio = (int)((( m_pGenInfo[i]->p_Unit->fDisPower / m_pGenInfo[i]->p_Unit->fcurCap ) * nMaxRadio - m_pGenInfo[i]->p_Unit->iModifySub)/m_pGenInfo[i]->p_Unit->fFactor);
				memset(m_LastError,NULL,sizeof(m_LastError));
				sprintf_s(m_LastError ,sizeof(m_LastError),"下降 %d 号逆变器分配目标值为 %.2f\r\n",m_pGenInfo[i]->p_Unit->ID32,m_pGenInfo[i]->p_Unit->fDisPower);
				writeBuffer(m_LastError);

				// 				//最小调节控制，先不考虑
				// 				if(m_inverterInfo[i]->pObject < (m_inverterInfo[i]->pRating * 0.1))
				// 				{
				//  					m_inverterInfo[i]->pObject = m_inverterInfo[i]->pRating * 0.1;
				//  					m_inverterInfo[i]->ratio = (int)((0.1 * nMaxRadio)/m_inverterInfo[i]->factor);
				// //					m_inverterInfo[i]->adjustFlag = false;//2013-09-25 YangHuaining  小于最小值不调，避免逆变器关机无法启动
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
			sprintf_s(m_LastError ,sizeof(m_LastError),"上升 %d 号逆变器分配负值不正确，\r\n",m_pGenInfo[i]->p_Unit->ID32);
			writeBuffer(m_LastError);
		}
		TRACE("GenInfo ID:%d,DisPower:%f\n",m_pGenInfo[i]->p_Unit->ID32, m_pGenInfo[i]->p_Unit->fDisPower);
	}

	memset(m_LastError,0,sizeof(m_LastError));
	sprintf(m_LastError ,"%s","分配算法运行");
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
				//发送消息执行
				p->p_Unit->iFlag=PHVUNIT_EXEC;//执行				

				float execval=p->p_Unit->fDisPower*p->p_Unit->fFactor;
				//写入目标指令分配
				m_funcRtdb.uptAlgRecordByID(p->p_Unit->iDisPower,p->p_Unit->fDisPower);
				//end
				bool b_exeRes=true;
				if(isExec()==true)
				{
					memset(m_LastError,0,sizeof(m_LastError));
					sprintf(m_LastError ,"send YT channelID:%d,DeviceID:%d,YtNo:%d,YtVal=%.2f [DisPower:%.2f,Factor:%.2f] exec\n",p->p_Unit->iChannelID,p->p_Unit->iDevice,p->p_Unit->iYtNo,execval,p->p_Unit->fDisPower,p->p_Unit->fFactor);
					writeBuffer(m_LastError);

					//遥调表可能要加factor，或者在机组表里边添加，当前是在遥测表，有点不方便	
					b_exeRes = m_funcRtdb.sendDeviceExecYt(p->p_Unit->iChannelID,p->p_Unit->iDevice,p->p_Unit->iYtNo,execval);							

					memset(m_LastError,0,sizeof(m_LastError));
					sprintf(m_LastError ,"Yt return results:%s",b_exeRes==true?"success\n":"failed\n");
					writeBuffer(m_LastError);
				}


				//等待调节时间
				if(waitForTimeout(CALC_TH_KeyID,m_adParameter.getSendCmdInterval()) == false)  
				{				
					break;
				}
				else if(m_bNeedStopProcess == true)
				{
					break; //跳出发送命令循环 李乾
				}


				if(b_exeRes==false && m_adParameter.getReSendFlg())//?? 李乾注释掉的
				{
					//重复发送
					p->m_ReSendTimes++;

					if(p->m_ReSendTimes>=AGC_RE_SEND_MAX_TIMES)
					{
						p->adjustFlag=false; 
						writeBuffer("超过代码内配置的最大重发次数，不再重新发送\n");
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
//储能的执行
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
			//发送消息执行
			p->p_Unit->iFlag=PHVUNIT_EXEC;//执行				

			float execval=p->p_Unit->fDisPower*p->p_Unit->fFactor;
			//写入目标指令分配
			m_funcRtdb.uptAlgRecordByID(p->p_Unit->iDisPower,p->p_Unit->fDisPower);
			//end
			bool b_exeRes=true;
			if(isExec()==true)
			{
				memset(m_LastError,0,sizeof(m_LastError));
				sprintf(m_LastError ,"send YT channelID:%d,DeviceID:%d,YtNo:%d,YtVal=%.2f [DisPower:%.2f,Factor:%.2f] exec\n",p->p_Unit->iChannelID,p->p_Unit->iDevice,p->p_Unit->iYtNo,execval,p->p_Unit->fDisPower,p->p_Unit->fFactor);
				writeBuffer(m_LastError);

				//遥调表可能要加factor，或者在机组表里边添加，当前是在遥测表，有点不方便	
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


			//等待调节时间
			if(waitForTimeout(CALC_TH_KeyID,m_adParameter.getSendCmdInterval()) == false)  
			{				
				return ;
			}
			else if(m_bNeedStopProcess == true)
			{
				return ; //跳出发送命令循环 李乾
			}


			if(b_exeRes==false && m_adParameter.getReSendFlg())//?? 李乾注释掉的
			{
				//重复发送
				p->m_ReSendTimes++;

				if(p->m_ReSendTimes>=AGC_RE_SEND_MAX_TIMES)
				{
					p->adjustFlag=false; 
					writeBuffer("超过代码内配置的最大重发次数，不再重新发送\n");
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
	sprintf(m_LastError ,"%s","执行分配命令启动");
	writeBuffer(m_LastError);



	ServerBkType bkType=m_funcRtdb.getServerBkType();//主备状态
	if(bkType!=eSvrBkTypeMaster)
	{
		memset(m_LastError,0,sizeof(m_LastError));
		sprintf(m_LastError ,"当前主机为备机，命令只分配，不发送给执行设备\n");
		writeBuffer(m_LastError);
		return;
	}

	execPhv();
	execPcs();


	//TRACE("执行分配命令完成\n");
}

//执行结果
void agcCalc::execResults()
{
	//支持并发访问，即可以一下发一串命令
	//校验命令有效性


	execDirect();



}
//nType 线程标识
//等待时间
//返回值true等待超时 false 退出线程
bool agcCalc::waitForTimeout(int nType,int nTimespan)
{
	int nInterval = 0;
	if(nType == CALC_TH_KeyID)
	{
		//进行计算
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
			//m_bNeedStopProcess = false; //李乾
			return true;
		}
	}
	else if(nType ==CALC_TH_AuxKeyID)
	{
		//进行计算
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
	//AGC库内容更新，但 Scada库不更新因为Scada库是由前置更新过了
	//内部变量更新
	//触发信号变化
	memset(m_LastError,0,sizeof(m_LastError));

	if(strcmp(rec->strKey,g_AgcDigitKey[eAgcDigitOptionOn])==0)
	{
		//功能启用
		sprintf(m_LastError ,"控制总功能投入状态改变，更改为%s",rec->iVal>0?"投入":"退出");

	}
	else if(strcmp(rec->strKey,g_AgcDigitKey[eAgcDigitCycleOn])==0)
	{
		//闭环控制
		sprintf(m_LastError ,"闭锁控制投入状态改变，更改为%s",rec->iVal>0?"投入":"退出");

	}
	writeBuffer(m_LastError);
	m_bNeedStopProcess = true;
}
//处理遥调
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
	sprintf_s(m_LastError ,sizeof(m_LastError), "当前控制总功能%s",ret==true?"投入":"未投入");
	writeBuffer(m_LastError);
	return ret;
}

//是否远方运行
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
	//得到
	// enmAgcAnalogKey key;

	//	fRes=m_funcRtdb.getAgcAnalog(key);
	return true;
}
int agcCalc::readCurrentDateTarget(float& nRes,bool flg,bool remoteflg)
{
	int temp_target = eCMD_MODE_SETPOINT;//命令方式 1为计划曲线2为遥设或者为设点
	int ret=0;

	bool bRet=true;
	bRet=getCmdSetPointValue(0,nRes);
	return ret;
}

/*************************************************
　　函数名称: checkAdjustObject
  　简要描述: 检测调节指令是否合法
   输入参数: 
   输出参数: 
   返 回 值：
   修改记录:
   *************************************************/
bool agcCalc::checkAdjustObject(time_t currentTime,GeneratorUnit *p)
{
	if(p == NULL)
	{
		memset(m_LastError,0,sizeof(m_LastError));

		sprintf_s(m_LastError ,sizeof(m_LastError), "检查%s调节指令不通过，原因指针为空\n",p->p_Unit->strName);
		writeBuffer(m_LastError);

		return false;
	}

	if(p->adjustFlag != true)
	{
		memset(m_LastError,0,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError), "检查%s调节指令不通过，原因分配标识为不调节\n",p->p_Unit->strName);
		writeBuffer(m_LastError);
		return false;
	}

	//int val = currentTime - p->lastAdjustTime;


	if(fabs(p->p_Unit->fDisPower - p->p_Unit->fValP) < p->p_Unit->fAdZone)
	{
		memset(m_LastError,0,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError), "检查%s调节指令不通过，调节指令%f减当前有功%f的绝对值%f小于发送调节命令死区%f\n",p->p_Unit->strName,  p->p_Unit->fDisPower,  p->p_Unit->fValP,  fabs(p->p_Unit->fDisPower - p->p_Unit->fValP),  p->p_Unit->fAdZone);
		writeBuffer(m_LastError);
		return false;
	}
	if(fabs(p->p_Unit->fDisPower - p->lastObjectValue) <p->p_Unit->fAdZone)//由原来的小于
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
				sprintf_s(m_LastError ,sizeof(m_LastError), "检查%s调节指令不通过，原因重复发送命令，超过重复发送次数\n",p->p_Unit->strName);
				writeBuffer(m_LastError);

				return false;
			}
		}
		else
		{
			memset(m_LastError,0,sizeof(m_LastError));
			sprintf_s(m_LastError ,sizeof(m_LastError), "检查%s调节指令不通过，命令相同当前指令=%.3f,上次指令%.3f,最小调节=%.3f\n",p->p_Unit->strName,p->p_Unit->fDisPower,p->lastObjectValue,p->p_Unit->fAdZone);
			writeBuffer(m_LastError);
			return false;
		}
	}
	else if(p->p_Unit->fDisPower<p->p_Unit->fAdMinLimit)
	{
		memset(m_LastError,0,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError), "检查%s调节指令不通过，当前指令=%.3f低于调节下限=%.3f\n",p->p_Unit->strName,p->p_Unit->fDisPower,p->p_Unit->fAdMinLimit);
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
				sprintf_s(m_LastError ,sizeof(m_LastError), "检查%s配置的遥调配置在遥调表不存在对应记录\n",wi->p_Unit->strName);
				writeBuffer(m_LastError);
				ret=false;
			}
		}
	}
	return ret;

}
// 计算线程
ThreadFuncRet agcCalc::FreshThreadProcAux(LPVOID lpParameter)
{
	agcCalc *agc = (agcCalc*)lpParameter;
	if(agc == NULL)
		return 0;

	//打印
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
//如果主机是否为主机 true为主机,false为备机
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
		sprintf_s(m_LastError,sizeof(m_LastError),"当前机器为备机，只刷新数据，不启动算法\n");
		writeBuffer(m_LastError);	
	}
	return ret;
}

float agcCalc::modifyCurrentTarget(float dTarget,float dMaxOnePower,float curPower)
{
	//防止一次调节动作过大，满足省调1分钟调节限值要求(装机容量的的10%)
	float dt = dTarget;
	float dlimit=dMaxOnePower;

	//第一次进入1分钟变化率控制，需要将实际功率赋值给保存参照值
	if (m_1minrectar < 0)
	{
		m_1minrectar = curPower;
	}
	time_t m_timeNow = time(NULL);//取得当前时间与上次记录参照值的时间比较，大于1分钟则重新记录

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

	sprintf_s(m_LastError ,sizeof(m_LastError), "1分钟变化率修正(时间差值=%d秒):目标值%.2f,修正后目标%.2f实时出力=%.2f,调节参考值=%.2f,速率限值=%.0fkW/分钟",diff,dTarget,dt,curPower,m_1minrectar,dMaxOnePower);
	writeBuffer(m_LastError);

	return dt;
}

//储能充电策略
//dTarget是一个正值
int agcCalc::pcsChargeMethod(float dTarget)
{

	float fChargeMax = m_funcRtdb.getAgcAnalog(eAgcAnalogSocChargeMax);//充电上限

	bool bReachSocMax=false;//储能是否达到调节最大
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
		sprintf_s(m_LastError ,sizeof(m_LastError), "储能当前电池满，储能待机运行");
		writeBuffer(m_LastError);

		pcsWaitMethod();//待机策略
		return AGC_SUC;
	}
	if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Charge)
	{
		//充电中
		//储能以P目标功率充电
		m_pSocInfo->adjustFlag=true;
		m_pSocInfo->p_Unit->fDisPower=	m_pSocInfo->p_Unit->fValUp-dTarget;
	}
	else
	{
		if(m_pSocInfo->p_Unit->iStatus==SOC_PCS_RUN_STATUS_Run)
		{
			//检查存能响应是否超时
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
						sprintf_s(m_LastError ,sizeof(m_LastError), "储能执行充电指令响应不正常，置储报警信号pcs响应不正常为1");
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
			float fMax = m_funcRtdb.getAgcAnalog(eAgcAnalogSocChargePMax);//放电下限

			if(m_pSocInfo->p_Unit->fDisPower<-fMax)
			{
				m_pSocInfo->p_Unit->fDisPower=-fMax;
			}
		}
	}
	return AGC_SUC;
}
//储能放电策略
int agcCalc::pcsDischargeMethod(float dTarget)
{
	float fDischargeMin = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargeMin);//放电下限



	if(m_funcRtdb.equalDouble(m_pSocInfo->p_Unit->fBatteryPower,fDischargeMin)||
		m_pSocInfo->p_Unit->fBatteryPower<fDischargeMin)
	{
		sprintf_s(m_LastError ,sizeof(m_LastError), "储能执电池电量%.2f低于放电下限%.2f，储能待机",m_pSocInfo->p_Unit->fBatteryPower,fDischargeMin);
		writeBuffer(m_LastError);

		pcsWaitMethod();//待机策略
		return AGC_SUC;
	}


	if(m_pSocInfo->checkCFStatus()==SOC_POWER_STATUS_Discharge)
	{
		//放电中
		//储能以P目标功率放电
		m_pSocInfo->adjustFlag=true;
		m_pSocInfo->p_Unit->fDisPower=	m_pSocInfo->p_Unit->fValUp+dTarget;
	}
	else
	{
		if(m_pSocInfo->p_Unit->iStatus==SOC_PCS_RUN_STATUS_Run)
		{
			//检查存能响应是否超时
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
						sprintf_s(m_LastError ,sizeof(m_LastError), "储能执行放电指令响应不正常，置储报警信号pcs响应不正常为1");
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
			float fMax = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargePMax);//放电下限

			if(m_pSocInfo->p_Unit->fDisPower>fMax)
			{
				m_pSocInfo->p_Unit->fDisPower=fMax;
			}
		}
	}
	return AGC_SUC;
}
//储能待机策略
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
				sprintf_s(m_LastError ,sizeof(m_LastError), "储能执行待机指令响应超时，置储报警信号pcs响应不正常为1");
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
//储能开机策略
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

				sprintf_s(m_LastError ,sizeof(m_LastError), "储能执行开机指令不正常，尝试%d次都失败了，置储报警信号pcs响应不正常为1",m_pSocInfo->m_ReSendTimes);
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
//通用变量检查
int agcCalc::commonVaribleCheck()
{
	return AGC_SUC;
}
/*************************************************
　　函数名称: checkAdjustObject
  　简要描述: 检测调节指令是否合法
   输入参数: 
   输出参数: 
   返 回 值：
   修改记录:
   *************************************************/
bool agcCalc::checkAdjustObjectPcs(time_t currentTime,GeSocUnit *p)
{
	if(p == NULL)
	{
		memset(m_LastError,0,sizeof(m_LastError));

		sprintf_s(m_LastError ,sizeof(m_LastError), "检查%s调节指令不通过，原因指针为空\n",p->p_Unit->strName);
		writeBuffer(m_LastError);

		return false;
	}

	if(p->adjustFlag != true)
	{
		memset(m_LastError,0,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError), "检查%s调节指令不通过，原因分配标识为不调节\n",p->p_Unit->strName);
		writeBuffer(m_LastError);
		return false;
	}

	//int val = currentTime - p->lastAdjustTime;


	if(fabs(p->p_Unit->fDisPower - p->p_Unit->fValP) < p->p_Unit->fAdZone)
	{
		memset(m_LastError,0,sizeof(m_LastError));
		sprintf_s(m_LastError ,sizeof(m_LastError), "检查%s调节指令不通过，调节指令%f减当前有功%f的绝对值%f小于发送调节命令死区%f\n",p->p_Unit->strName,  p->p_Unit->fDisPower,  p->p_Unit->fValP,  fabs(p->p_Unit->fDisPower - p->p_Unit->fValP),  p->p_Unit->fAdZone);
		writeBuffer(m_LastError);
		return false;
	}
	if(fabs(p->p_Unit->fDisPower - p->lastObjectValue) <p->p_Unit->fAdZone)//由原来的小于
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
				sprintf_s(m_LastError ,sizeof(m_LastError), "检查%s调节指令不通过，原因重复发送命令，超过重复发送次数\n",p->p_Unit->strName);
				writeBuffer(m_LastError);

				return false;
			}
		}
		else
		{
			memset(m_LastError,0,sizeof(m_LastError));
			sprintf_s(m_LastError ,sizeof(m_LastError), "检查%s调节指令不通过，命令相同当前指令=%.3f,上次指令%.3f,最小调节=%.3f\n",p->p_Unit->strName,p->p_Unit->fDisPower,p->lastObjectValue,p->p_Unit->fAdZone);
			writeBuffer(m_LastError);
			return false;
		}
	}

	float fChargeMax = m_funcRtdb.getAgcAnalog(eAgcAnalogSocChargePMax);//充电最大
	float fDischargeMax = m_funcRtdb.getAgcAnalog(eAgcAnalogSocDischargePMax);//放电最大

	if(p->p_Unit->fDisPower<0)
	{
		if(p->p_Unit->fDisPower<-fChargeMax)
		{
			memset(m_LastError,0,sizeof(m_LastError));
			sprintf_s(m_LastError ,sizeof(m_LastError), "检查%s调节指令不通过，当前指令=%.2f高于充电最大功率%.2f\n",p->p_Unit->strName,p->p_Unit->fDisPower,fChargeMax );
			writeBuffer(m_LastError);
			return false;
		}

	}
	else if(p->p_Unit->fDisPower>0)
	{
		if(p->p_Unit->fDisPower>fDischargeMax)
		{	
			memset(m_LastError,0,sizeof(m_LastError));
			sprintf_s(m_LastError ,sizeof(m_LastError), "检查%s调节指令不通过，当前指令=%.3f高于放电最大功率=%.3f\n",p->p_Unit->strName,p->p_Unit->fDisPower,fDischargeMax );
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
//返回值0待机 1充电 2 充电
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
	sprintf(m_LastError ,"充电时段[%d:%d-%d:%d],放电时段[%d:%d-%d:%d],当前时间[%s]",nChargeBeginHour,nChargeBeginMin,nChargeEndHour,nChargeEndMin,nDischargeBeginHour,nDischargeBeginMin,nDischargeEndHour,nDischargeEndMin,cDisplay);
	writeBuffer(m_LastError);

	int nNowMin=now.wHour*60+now.wMinute;

	int nChargeBegin=nChargeBeginHour*60+nChargeBeginMin;
	int nChargetEnd=nChargeEndHour*60+nChargeEndMin;
	if(nNowMin>nChargeBegin&& nNowMin<nChargetEnd)
	{
		//充电
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
		sprintf(m_LastError ,"充电功率计算逻辑错误，请检查，置充电功率为20kW,计算预期结果%.2f,相对于光伏最大充电功率%.2f,储能额定%.2f",ret,fPhvSum*fFactor,m_pSocInfo->p_Unit->fcurCap);
		writeBuffer(m_LastError);
		ret=20;
	}
	return ret;
}

#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif