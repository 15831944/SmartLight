#include "gSystemDef.h"
#include "agcFuncRtdb.h"
#include "Q_frameInclude.h"
#include "CBrowseDir.h"
#ifndef UNIX_ENV
#pragma warning(disable:4996) 
#endif
agcFuncRtdb *g_pAgcFuncDb;
agcFuncRtdb::agcFuncRtdb()
{
	m_dbmg_sdb=NULL;
	m_dbmg_mdb=NULL;
	m_dbmg_agc=NULL;
	g_pAgcFuncDb=this;
	_msg_dealer=NULL;
	nLogCurIndex=0;

	m_Util=new CFuncUtil(AGC_LOG_PATH,AGC_PATH_LOGFILE);
}
agcFuncRtdb::~agcFuncRtdb()
{
	if(m_Util!=NULL)
		delete m_Util;
}

bool agcFuncRtdb::rtdb_init()
{
	if (!dbmg_init()) 
		return false;

	int ret =rtdb_login();

	if(0==ret)
		return true;
	else 
		return false;
	clearAgcLog();

	//return m_log.OpenLog();
}

bool agcFuncRtdb::dbmg_init()
{
	if (!m_dbmg_mdb) 
		m_dbmg_mdb = new CMultiDataBase();

	if(!m_dbmg_mdb->AttachShareMemory()) 
		return false;
	//��Ϊ�����������ķ���ʱ����ȷ�����ʵʱ��ʧ��,2016-11-25 
	m_dbmg_mdb->SocketInit();
	//��Ϊ�����������ķ���ʱ����ȷ�����ʵʱ��ʧ��,2016-11-25 end

	if (!m_dbmg_sdb) 
		m_dbmg_sdb = new CScadaDataBase();

	if(m_dbmg_mdb->GetDataBaseFromID(*m_dbmg_sdb, DB_ID_OF_SCADA)) 
	{
		if(!m_dbmg_sdb->AttachShareMemory())  
			return false;
		else
			m_dbmg_sdb->Open_Msg();
	}
	else 
		return false;

	if (!m_dbmg_agc) 
		m_dbmg_agc = new CAgcDataBase;

	if(m_dbmg_mdb->GetDataBaseFromID(*m_dbmg_agc, DB_ID_OF_AGC)) 
	{

		if(!m_dbmg_agc->AttachShareMemory()) 
			return false;

		m_dbmg_agc->initAgcTable();
	}

	return true;
}
int agcFuncRtdb::rtdb_login()
{
	if (m_dbmg_mdb->Register(pt_AGC, 0)) 
	{

		TRACE("register success\n");

		return(0);
	}
	else 
		return(-1);
}
void agcFuncRtdb::setMsgHandler(MsgDealFuncType pFunc)
{
	if( _msg_dealer==NULL)
		_msg_dealer = new CMsgDealer();

	_msg_dealer->Init(m_dbmg_mdb, pt_AGC,pFunc);
}


/* 
* logout from rtdb
*/
int agcFuncRtdb::rtdb_logout()
{
	//return  m_dbmg_mdb->UnRegister();
	if(_msg_dealer!=NULL)
		_msg_dealer->Exit();

	//ԭ����
	//return  m_dbmg_mdb->UnRegister();
	//end
	//���ĺ� tjs 2016-2-18 
	int ret= m_dbmg_mdb->UnRegister();

	m_dbmg_mdb->cleanProcessStatus(pt_AGC);
	m_dbmg_mdb->cleanNodeTabPara();

	TRACE("execute SocketExit");
	m_dbmg_mdb->SocketExit();//ע������ϢҪ�˳�
	return ret;
}
CMultiDataBase *agcFuncRtdb::getMainDb()
{
	return m_dbmg_mdb;	
}
CScadaDataBase *agcFuncRtdb::getScadaDb()
{
	return m_dbmg_sdb;
}
CAgcDataBase *agcFuncRtdb::getAgcDb()
{
	return m_dbmg_agc;
}
BOOL agcFuncRtdb::freshPhvUnit()
{

	INT nMaxIndex = m_dbmg_agc->getRecordCountByID(AGC_TAB_PhvUnit_ID)-1;
	if( nMaxIndex < 0 )
		return FALSE;

	AGC_PhvUnit* pRecord = (AGC_PhvUnit*)m_dbmg_agc->getDataAddress(AGC_TAB_PhvUnit_ID);
	if(pRecord!=NULL)
	{
		//��ټ�¼�������´˱��е�ң�⡢ң��
		SCADA_Analog    Analog;
		SCADA_Digit     digit;
		for(int i=0;i<=nMaxIndex;i++)
		{
			/*if(pRecord->iEnabled)
			{*/
			SYSTEMTIME systime;
			GetLocalTime(systime);
			memcpy(&pRecord->tSnapTime,&systime,sizeof(pRecord->tSnapTime));

			if(m_dbmg_sdb!=NULL)
			{
				pRecord->iQuality=Quality_Good;
				//�����й�
				if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Analog_ID, pRecord->iPowerAlgID,&Analog))
				{
					if(Analog.iQuality==Quality_Good)
					{
						
						pRecord->fValP=Analog.fVal;
						
						if(pRecord->fValP>pRecord->fcurCap*1.1||pRecord->fValP<0)
						{ 
							pRecord->iQuality=Quality_Bad;

							memset(m_LastError,0,sizeof(m_LastError));
							sprintf_s(m_LastError ,sizeof(m_LastError), "��%d�е�ǰ�й�Խ�ޣ���ǰֵ%.2f\n",i+1,pRecord->fValP);
							pushAgcLog(m_LastError);

						}
					}
				}
				
				//��������״̬
				/*if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Digit_ID, pRecord->iStatusDigitID,&digit))
				{
					if(digit.iQuality==Quality_Good)
					{
						pRecord->iStatus=digit.iVal;
					}
				}*/

				if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Analog_ID, pRecord->iStatusDigitID,&Analog))
				{
					if(Analog.iQuality==Quality_Good)
					{			  
						   	pRecord->iStatus=(int)Analog.fVal;
					}
				}

				if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Digit_ID, pRecord->iChannelDigitID,&digit))
				{
					if(digit.iQuality==Quality_Good)
					{
						pRecord->iChannelStatus=digit.iVal;
					}
				}
				
				
				
				//Ŀ��ָ������
				if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Analog_ID, pRecord->iUpValAlgID,&Analog))
				{
					if(Analog.iQuality==Quality_Good)
					{
						
							pRecord->fValUp=Analog.fVal;
						
					

					}
				}
				

			}
			/*	}*/

			pRecord ++;
		}

	}
	else
	{
		TRACE("relay table is empty!");
	}
	return TRUE;
}

BOOL agcFuncRtdb::freshAnlog()//�����ݴ�scada������agc�Ĳ������ң���
{

	INT nMaxIndex = m_dbmg_agc->getRecordCountByID(AGC_TAB_Analog_ID)-1;
	//TRACE("nMaxIndex:%d", nMaxIndex);
	if( nMaxIndex < 0 )
		return FALSE;

	AGC_Analog* pRecord = (AGC_Analog*)m_dbmg_agc->getDataAddress(AGC_TAB_Analog_ID);
	if(pRecord!=NULL)
	{
		//��ټ�¼�������´˱��е�ң�⡢ң��
		SCADA_Analog    Analog;
		for(int i=0;i<=nMaxIndex;i++)
		{
			if(pRecord->iAnalogID>0)
			{			
				if(m_dbmg_sdb!=NULL)
				{					
					if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Analog_ID, pRecord->iAnalogID,&Analog))
					{
						if(Analog.iQuality==Quality_Good)
						{
							
							pRecord->fVal=Analog.fVal;
							
							SYSTEMTIME systime;
							GetLocalTime(systime);
							memcpy(&pRecord->tSnapTime,&systime,sizeof(pRecord->tSnapTime));
						}
					}
					else
					{
						pRecord->iQuality=Quality_Bad;
				
						memset(m_LastError,0,sizeof(m_LastError));
						sprintf_s(m_LastError ,sizeof(m_LastError),"SCADAң���û���ҵ���ң���¼Index:%d,pRecord->iAnalogID:%d\n",i+1, pRecord->iAnalogID);
						pushAgcLog(m_LastError);
					}
				}
			}

			pRecord ++;
		}

	}
	else
	{
		TRACE("AGC_Analog table is empty!");
	}
	return TRUE;
}
BOOL agcFuncRtdb::freshDigit()
{

	INT nMaxIndex = m_dbmg_agc->getRecordCountByID(AGC_TAB_Digit_ID)-1;
	if( nMaxIndex < 0 )
		return FALSE;

	AGC_Digit* pRecord = (AGC_Digit*)m_dbmg_agc->getDataAddress(AGC_TAB_Digit_ID);
	if(pRecord!=NULL)
	{
		//��ټ�¼�������´˱��е�ң�⡢ң��
		SCADA_Digit    dig;
		for(int i=0;i<=nMaxIndex;i++)
		{
			if(pRecord->iDigitID>0)
			{
				SYSTEMTIME systime;
				GetLocalTime(systime);
				memcpy(&pRecord->tSnapTime,&systime,sizeof(pRecord->tSnapTime));

				if(m_dbmg_sdb!=NULL)
				{
					//�����й�
					if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Digit_ID, pRecord->iDigitID,&dig))
					{
						if(dig.iQuality==Quality_Good)
						{							
							pRecord->iVal=dig.iVal;
						}
					}
					else
					{
						memset(m_LastError,0,sizeof(m_LastError));
						sprintf_s(m_LastError ,sizeof(m_LastError),"SCADAң�ű�û���ҵ���ң�ż�¼Index:%d,pRecord->iAnalogID:%d\n",i+1, pRecord->iDigitID);
						pushAgcLog(m_LastError);
					}
					
				}
			}

			pRecord ++;
		}

	}
	else
	{
		TRACE("AGC_Digit table is empty!");
	}
	return TRUE;
}
int agcFuncRtdb::getCommAdParam(enmAgcParamKey key)
{
	int ret=0;
	INT nMaxIndex = m_dbmg_agc->getRecordCountByID(AGC_TAB_Parameter_ID)-1;
	if( nMaxIndex < 0 )
		return ret;

	AGC_Parameter* pRecord = (AGC_Parameter*)m_dbmg_agc->getDataAddress(AGC_TAB_Parameter_ID);
	if(pRecord!=NULL)
	{
		//��ټ�¼�������´˱��е�ң�⡢ң��
		for(int i=0;i<=nMaxIndex;i++)
		{
			if(strcmp(pRecord->strKey,g_ParamKey[key])==0)
			{
				ret=pRecord->iVal;
				break;
			}

			pRecord ++;
		}

	}
	else
	{
		TRACE("AGC_Parameter table is empty!");
	}
	return ret;
}
int agcFuncRtdb::getAgcDigit(enmAgcDigitKey key)
{
	int ret=-1;
	INT nMaxIndex = m_dbmg_agc->getRecordCountByID(AGC_TAB_Digit_ID)-1;
	if( nMaxIndex < 0 )
		return ret;

	AGC_Digit* pRecord = (AGC_Digit*)m_dbmg_agc->getDataAddress(AGC_TAB_Digit_ID);

	if(pRecord!=NULL)
	{
		//��ټ�¼�������´˱��е�ң�⡢ң��
		for(int i=0;i<=nMaxIndex;i++)
		{
			if(strcmp(pRecord->strKey,g_AgcDigitKey[key])==0)
			{
				ret=pRecord->iVal;
				break;
			}

			pRecord ++;
		}

	}
	else
	{
		TRACE("AGC_Digit table is empty!");
	}
	return ret;
}
float agcFuncRtdb::getAgcAnalog(enmAgcAnalogKey key)
{
	float ret=0;
	INT nMaxIndex = m_dbmg_agc->getRecordCountByID(AGC_TAB_Analog_ID)-1;
	if( nMaxIndex < 0 )
		return ret;

	AGC_Analog* pRecord = (AGC_Analog*)m_dbmg_agc->getDataAddress(AGC_TAB_Analog_ID);

	if(pRecord!=NULL)
	{
		//��ټ�¼�������´˱��е�ң�⡢ң��
		for(int i=0;i<=nMaxIndex;i++)
		{
			if(strcmp(pRecord->strKey,g_AgcAnalogKey[key])==0)
			{
				ret=pRecord->fVal;
				break;
			}

			pRecord ++;
		}

	}
	else
	{
		TRACE("AGC_Analog table is empty!");
	}
	return ret;
}
int agcFuncRtdb::setAgcWarn(enmAgcAlarmKey key, int iVal)
{
	BOOL ret=FALSE;
	INT nMaxIndex = m_dbmg_agc->getRecordCountByID(AGC_TAB_Alarm_ID)-1;	
	if( nMaxIndex < 0 )
		return 0;

	AGC_Alarm* pRecord = (AGC_Alarm*)m_dbmg_agc->getDataAddress(AGC_TAB_Alarm_ID);

	if(pRecord!=NULL)
	{
		for(int i=0;i<=nMaxIndex;i++)
		{
			if(strcmp(pRecord->strKey,g_AgcAlarmKey[key])==0)
			{
				pRecord->iVal=iVal;//2016-09-20
				//�ҵ���Ӧ�ļ�¼
				//�ҵ���¼�жϵ�λ�������Ƿ�ΪkW

				//ͨ��IDȡ��ĳ�����ĳ����¼
				SCADA_Digit dit;
				BOOL bFlg=m_dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID, pRecord->iDigitID, &dit);
				if(bFlg)
				{					
					//1����scada���еļ�¼
					//ret = m_dbmg_sdb->SetDigit(pRecord->iDigitID,iVal);
					pRecord->iVal=iVal;
					GetLocalTime(pRecord->tSnapTime);
					uptDigRecord(pRecord->iDigitID,iVal);
					//֧��ȫ��ͬ��
				//	dit.iVal=iVal;
				//	ret=m_dbmg_sdb->SetElementByID(SCADA_TAB_Digit_ID, pRecord->iDigitID,&dit);
					//2016-11-26 �������������ķ����������ʵʱ����ұ���
				//	uptDigRecord(pRecord->iDigitID,iVal);

									
				}
				else
				{
					TRACE("no record %d\n", pRecord->iDigitID);
				}

				break;
			}

			pRecord ++;
		}

	}
	else
	{
		TRACE("AGC_Warn table is empty!");
	}
	return ret;
}

int agcFuncRtdb::initAgcAlarmValue()
{
	BOOL ret=FALSE;
	INT nMaxIndex = m_dbmg_agc->getRecordCountByID(AGC_TAB_Alarm_ID)-1;	
	if( nMaxIndex < 0 )
		return 0;

	AGC_Alarm* pRecord = (AGC_Alarm*)m_dbmg_agc->getDataAddress(AGC_TAB_Alarm_ID);

	if(pRecord!=NULL)
	{
		for(int i=0;i<=nMaxIndex;i++)
		{
			
				pRecord->iVal=0;//2016-09-20

				SCADA_Digit dit;
				BOOL bFlg=m_dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID, pRecord->iDigitID, &dit);
				if(bFlg)
				{					
					pRecord->iVal=0;
					GetLocalTime(pRecord->tSnapTime);
					uptDigRecord(pRecord->iDigitID,0);
				}
				else
				{
					TRACE("no record %d\n", pRecord->iDigitID);
				}

			pRecord ++;
		}

	}
	else
	{
		TRACE("AGC_Warn table is empty!");
	}
	return ret;
}
BOOL agcFuncRtdb::findYk(RControl *rc,AGC_Digit * &rec)
{
//��ѯң�ر������õ�ң������
//Ȼ�����ң����������AGC_Digit��¼


	BOOL ret=FALSE;

    SCADA_Relay* srelay=NULL;
	if(m_dbmg_sdb!=NULL)
	{
		stdevKey vkey;
		vkey.stid=rc->iStation;
		vkey.device=rc->iDevice;
		vkey.no=rc->iYKPoint;
		vkey.iZf=rc->iZf;

		if(m_dbmg_sdb->GetTabRelayByKey(vkey,srelay))
		{
			//�ҵ���¼�˼�����ѯAGC_Digit��
			INT nMaxIndex = m_dbmg_agc->getRecordCountByID(AGC_TAB_Digit_ID)-1;
			if( nMaxIndex < 0 )
				return FALSE;

			AGC_Digit* pRecord = (AGC_Digit*)m_dbmg_agc->getDataAddress(AGC_TAB_Digit_ID);
			if(pRecord!=NULL)
			{
				//��ټ�¼�������´˱��е�ң�⡢ң��
				for(int i=0;i<=nMaxIndex;i++)
				{
					if(pRecord->iDigitID==srelay->ulYXID32)
					{
                       rec=pRecord;
					   ret=TRUE;
					   break;
					}
					pRecord ++;
				}
			}

		}

	}
	return ret;

}
BOOL agcFuncRtdb::findYt(RSetpoint *rs,AGC_Analog * &rec)
{
	//��ѯң�ر������õ�ң������
	//Ȼ�����ң����������AGC_Digit��¼

	BOOL ret=FALSE;

	SCADA_SetPoint* spoint=NULL;
	if(m_dbmg_sdb!=NULL)
	{
		stdevKey vkey;
		vkey.stid=rs->iStation;
		vkey.device=rs->iDevice;
		vkey.no=rs->iPoint;
		vkey.iZf=rs->iZf;

		if(m_dbmg_sdb->GetTabSetPointByKey(vkey,spoint))
		{
			//�ҵ���¼�˼�����ѯAGC_Digit��
			INT nMaxIndex = m_dbmg_agc->getRecordCountByID(AGC_TAB_Analog_ID)-1;
			if( nMaxIndex < 0 )
				return FALSE;

			AGC_Analog* pRecord = (AGC_Analog*)m_dbmg_agc->getDataAddress(AGC_TAB_Analog_ID);
			if(pRecord!=NULL)
			{
				//��ټ�¼�������´˱��е�ң�⡢ң��
				for(int i=0;i<=nMaxIndex;i++)
				{
					if(pRecord->iAnalogID==spoint->ulYCID32)
					{
						rec=pRecord;
						ret=TRUE;
						break;
					}
					pRecord ++;
				}
			}

		}

	}
	return ret;
}
BOOL agcFuncRtdb::checkYtExists(int iStation,int iDevice,int iPoint)
{
	//��ѯң�ر������õ�ң������
	//Ȼ�����ң����������AGC_Digit��¼

	BOOL ret=FALSE;

	SCADA_SetPoint* spoint=NULL;
	if(m_dbmg_sdb!=NULL)
	{
		stdevKey vkey;
		vkey.stid=iStation;
		vkey.device=iDevice;
		vkey.no=iPoint;
		vkey.iZf=0;

		if(m_dbmg_sdb->GetTabSetPointByKey(vkey,spoint))
		{
			ret=TRUE;
		}
	}
	return ret;
}

bool agcFuncRtdb::uptAlgRecordBykW(enmAgcAnalogKey key,float val)
{
	bool ret=false;
	INT nMaxIndex = m_dbmg_agc->getRecordCountByID(AGC_TAB_Analog_ID)-1;
	if( nMaxIndex < 0 )
		return false;

	AGC_Analog* pRecord = (AGC_Analog*)m_dbmg_agc->getDataAddress(AGC_TAB_Analog_ID);

	for(int i=0;i<=nMaxIndex;i++)
	{
		if(pRecord!=NULL)
		{
			if(strcmp(pRecord->strKey,g_AgcAnalogKey[key])==0)
			{
				//�ҵ���Ӧ�ļ�¼
				//�ҵ���¼�жϵ�λ�������Ƿ�ΪkW

				//ͨ��IDȡ��ĳ�����ĳ����¼
				SCADA_Analog alg;
				BOOL bFlg=m_dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID, pRecord->iAnalogID, &alg);
				if(bFlg)
				{
					pRecord->fVal=val;
					GetLocalTime(pRecord->tSnapTime);

					float fVal2=val;
				
					//1����scada���еļ�¼
					/*if(m_dbmg_sdb->SetAnalog(pRecord->iAnalogID,fVal2))
					{
					ret=true;
					}*/
					//����Ŀ��ʵ��ȫ��ͬ��
					//alg.fVal=fVal2;
				//	m_dbmg_sdb->SetElementByID(SCADA_TAB_Analog_ID, pRecord->iAnalogID,&alg);
					//2����AGC��ļ�¼����

					if(uptAlgRecordByID(pRecord->iAnalogID,fVal2,false))
					ret = true;
					else 
						ret=false;
				}
				break;
			}
		}
		else
		{
			TRACE("AGC_Analog table is empty!");
		}

		pRecord ++;
	}

	return ret;
}
bool agcFuncRtdb::uptDigRecord(enmAgcDigitKey key,int val)
{
	bool ret=false;
	INT nMaxIndex = m_dbmg_agc->getRecordCountByID(AGC_TAB_Digit_ID)-1;
	if( nMaxIndex < 0 )
		return false;

	AGC_Digit* pRecord = (AGC_Digit*)m_dbmg_agc->getDataAddress(AGC_TAB_Digit_ID);

	for(int i=0;i<=nMaxIndex;i++)
	{
		if(pRecord!=NULL)
		{
			if(strcmp(pRecord->strKey,g_AgcDigitKey[key])==0)
			{
				pRecord->iVal=val;//2016-09-20 add by tjs
				//�ҵ���Ӧ�ļ�¼
				//�ҵ���¼�жϵ�λ�������Ƿ�ΪkW

				//ͨ��IDȡ��ĳ�����ĳ����¼
				//SCADA_Digit dit;
				//BOOL bFlg=m_dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID, pRecord->iDigitID, &dit);
				//if(bFlg)
				//{					
				//	//1����scada���еļ�¼
				//	//ret = m_dbmg_sdb->SetDigit(pRecord->iDigitID,val)==TRUE?true:false;
				//	//2016-11-23 ֧��ȫ��ͬ��
				//	dit.iVal=val;
				//	ret= m_dbmg_sdb->SetElementByID(SCADA_TAB_Digit_ID, pRecord->iDigitID,&dit)==TRUE?true:false;
				//}
				//TRACE("uptDigRecord by key:digit id=%d,GetElementIndexByID failed\n",pRecord->iDigitID);
				 ret=uptDigRecord(pRecord->iDigitID,val);
				
				break;
			}

		}
		else
		{
			TRACE("AGC_Digt table is empty!");
		}
		pRecord ++;
	}

	return ret;
}
bool agcFuncRtdb::sendDeviceExecYt(int chid,int device,int no,float val)
{
	
		RSetpoint rs;
		rs.iStation =chid;
		rs.iPoint =no;
		rs.iCommandType =MES_TYPE_SPOINT;//ң�������룬��ʱ����
		rs.fVal = val;
		rs.iDevice=device;
		rs.iZf=0; // 2015/12/16
 		m_dbmg_mdb->SendMessage(pt_CHANNELOP, mes_SPExecute, sizeof(RSetpoint), (char *)&rs);
		return true;
	
}
bool agcFuncRtdb::sendDeviceExecYk(int chid,int device,int no,int val)
{

	RControl rs;
	
	rs.iStation =chid;
	rs.iYKPoint =no;
	rs.iCommandType =MES_TYPE_RELAY;//ң�������룬��ʱ����
	rs.iVal = val;
	rs.iDevice=device;
	rs.iZf=0; // 2015/12/16
	m_dbmg_mdb->SendMessage(pt_CHANNELOP, mes_RCExecute, sizeof(RControl), (char *)&rs);
	return true;

}
void agcFuncRtdb::clearAgcLog()
{
	INT nRecCount = m_dbmg_agc->getRecordCountByID(AGC_TAB_LogRecord_ID);
	if( nRecCount <=0 )
		return ;

	AGC_LogRecord* pRecord = (AGC_LogRecord*)m_dbmg_agc->getDataAddress(AGC_TAB_LogRecord_ID);
	//2016-09-12 tjs add
	if(pRecord!=NULL)
	{
		memset(pRecord,0,sizeof(AGC_LogRecord)*nRecCount);
	}
	for(int i=0;i<nRecCount;i++)
	{
		pRecord->ID32=i+1;
		pRecord++;
	}

	nLogCurIndex=0;
}
void agcFuncRtdb::pushAgcLog(char *msg)
{
	INT nRecCount = m_dbmg_agc->getRecordCountByID(AGC_TAB_LogRecord_ID);
	if( nRecCount <= 0 )
		return ;

	if(getCommAdParam(eAgcParamLogFlg)>0)
	{
		m_Util->WriteLogfile(-1, msg);
	}
	

	if(nLogCurIndex>=nRecCount)
	{
		clearAgcLog();
	}
	int len=strlen(msg)+1;
	
	AGC_LogRecord* pRecord = (AGC_LogRecord*)m_dbmg_agc->getDataAddress(AGC_TAB_LogRecord_ID);
	if(pRecord!=NULL)
	{
		pRecord+=nLogCurIndex;

		if(len>sizeof(pRecord->strText))
		{
			len=sizeof(pRecord->strText);
		}
		//pRecord->ID32=nLogCurIndex+1;//2016-09-09 ����
		GetLocalTime(pRecord->tSnapTime);
		memcpy(pRecord->strText,msg,len);
		
		nLogCurIndex++;
	}
	else
	{
		TRACE("AGC_Digit table is empty!");
	}
}
bool agcFuncRtdb::equalDouble(float num1,float num2)
{
	//���Ƚ�����
	if((num1-num2>-0.001)&&(num1-num2)<0.001)
		return true;
	else 
		return false;
}
bool agcFuncRtdb::uptDigRecord(int  nID,int val)
{
	bool ret=false;

	//SCADA_Digit dit;
	/*BOOL bFlg=m_dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID, nID, &dit);
	if(bFlg)
	{*/					
		//1����scada���еļ�¼
	//	ret = m_dbmg_sdb->SetDigit(nID,val)==TRUE?true:false;
	//	dit.iVal=val;
		//ret =m_dbmg_sdb->SetElementByID(SCADA_TAB_Digit_ID, nID,&dit)==TRUE?true:false;

	
	   int tmp_index=0;
		if (m_dbmg_sdb->GetElementIndexByID(SCADA_TAB_Digit_ID, nID, tmp_index))
		{
			DigitPacket    rtp;
			rtp.iDigitNum = 1; 

			rtp.iDigitIndex[0] = tmp_index;
			rtp.iDigitID[0] = nID;
			rtp.iDigitRaw[0] =val;

			m_dbmg_sdb->ProcessDigitPacket(rtp);

		}
		else
		{
				TRACE("uptDigRecord:diggit id=%d,GetElementIndexByID failed\n",nID);
		}

	
		
		/*}
		else
		{
		TRACE("uptDigRecord:diggit id=%d,GetElementByID failed\n",nID);
		}*/
	return ret;
}
//��ͬʱ����scada����AVC_Analog�ļ�¼��
BOOL agcFuncRtdb::uptAlgRecordByID(int nid,float val,bool checkUnit)
{
	if(nid<=0)
		return FALSE;
	BOOL ret=TRUE;

	SCADA_Analog alg;
	BOOL bFlg=m_dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,nid, &alg);
	if(bFlg)
	{
		float fVal2=val;
		
	//	ret = m_dbmg_sdb->SetAnalog(nid,fVal2);
		//2016-11-23 ʵ��ȫ��ͬ�� tjs 
		//alg.fVal=fVal2;
		//�����·��������ʵʱ��������֪��Ϊɶ 2016-11-26
	   // ret=m_dbmg_sdb->SetElementByID(SCADA_TAB_Analog_ID, nid,&alg);

		//ret = true;
		//2016-11-26 upt  by tjs
		int tmp_index=0;
		AnalogPacket    rtp;
		rtp.iAnalogNum = 1; 

		if (m_dbmg_sdb->GetElementIndexByID(SCADA_TAB_Analog_ID, nid, tmp_index))
		{
			rtp.iAnalogIndex[0] = tmp_index;
			rtp.iAnalogID[0] = nid;
			rtp.fAnalogRaw[0] =fVal2;
			m_dbmg_sdb->ProcessAnalogPacket(rtp);
			
		}
		else
		{
			ret=FALSE;
		}
			//2016-11-26 upt  by tjs end
	}
	return  ret;

}
ServerBkType agcFuncRtdb::getServerBkType()
{
	ServerBkType bktype=eSvrBkTypeSlave;

	int n=m_dbmg_mdb->GetMasterStatus();
	if(n>0)
	{
		bktype=eSvrBkTypeMaster;
	}
	return bktype;
}
BOOL agcFuncRtdb::freshSocUnit()
{
	INT nMaxIndex = m_dbmg_agc->getRecordCountByID(AGC_TAB_SocUnit_ID)-1;
	if( nMaxIndex < 0 )
		return FALSE;

	AGC_SocUnit* pRecord = (AGC_SocUnit*)m_dbmg_agc->getDataAddress(AGC_TAB_SocUnit_ID);
	if(pRecord!=NULL)
	{
		//��ټ�¼�������´˱��е�ң�⡢ң��
		SCADA_Analog    Analog;
		SCADA_Digit     digit;
		for(int i=0;i<=nMaxIndex;i++)
		{
			/*if(pRecord->iEnabled)
			{*/
			SYSTEMTIME systime;
			GetLocalTime(systime);
			memcpy(&pRecord->tSnapTime,&systime,sizeof(pRecord->tSnapTime));

			if(m_dbmg_sdb!=NULL)
			{
				pRecord->iQuality=Quality_Good;
				//�����й�
				if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Analog_ID, pRecord->iPowerAlgID,&Analog))
				{
					if(Analog.iQuality==Quality_Good)
					{
						pRecord->fValP=Analog.fVal;
					
						if(pRecord->fValP>pRecord->fcurCap*1.1||pRecord->fValP<0)
						{ 
							pRecord->iQuality=Quality_Bad;

							memset(m_LastError,0,sizeof(m_LastError));
							sprintf_s(m_LastError ,sizeof(m_LastError), "��%d�е�ǰ�й�Խ�ޣ���ǰֵ%.2f\n",i+1,pRecord->fValP);
							pushAgcLog(m_LastError);

						}
					}
				}
				
				//����״̬
				if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Analog_ID, pRecord->iStatusDigitID,&Analog))
				{
					if(Analog.iQuality==Quality_Good)
					{			 
						   int n=(int)Analog.fVal;
						   if(n==1||n==2)
						   {
							   	pRecord->iStatus=(int)Analog.fVal;
						   }
						   else
						   {
							   	pRecord->iStatus=SOC_PCS_RUN_STATUS_Undefined;
						   }


						   
					}
					else
					{
							pRecord->iStatus=SOC_PCS_RUN_STATUS_Undefined;
					}
				}
				//����״̬
				if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Analog_ID, pRecord->iExpStatusDigitID,&Analog))
				{
					if(Analog.iQuality==Quality_Good)
					{			  
						pRecord->iExpStatus=(int)Analog.fVal;
					}
				}
				// ͨ��״̬
				if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Digit_ID, pRecord->iChannelDigitID,&digit))
				{
					if(digit.iQuality==Quality_Good)
					{
						pRecord->iChannelStatus=digit.iVal;
					}
				}
			  //��ص���
				if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Analog_ID, pRecord->iSocAlgID,&Analog))
				{
					if(Analog.iQuality==Quality_Good)
					{
						pRecord->fBatteryPower=Analog.fVal;

					}
				}
				//������ѹ
				if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Analog_ID, pRecord->iUaAlgID,&Analog))
				{
					if(Analog.iQuality==Quality_Good)
					{
						pRecord->fValUa=Analog.fVal;

					}
				}
				//��������
				if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Analog_ID, pRecord->iIaAlgID,&Analog))
				{
					if(Analog.iQuality==Quality_Good)
					{
						pRecord->fValAC=Analog.fVal;

					}
				}
		//���״̬
				if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Analog_ID, pRecord->iBStatusAlgID,&Analog))
				{
					if(Analog.iQuality==Quality_Good)
					{
						int n=(int)Analog.fVal;
						if(n==Battery_Status_Full)
						{
								pRecord->iBStatus=Battery_Status_Full;
						}
						else if(n==Battery_Status_Empty)
						{
								pRecord->iBStatus=Battery_Status_Empty;

						}
						else if(n==Battery_Status_Alarm)
						{
								pRecord->iBStatus=Battery_Status_Alarm;
						}
						else if(n==Battery_Status_Fault)
						{
								pRecord->iBStatus=Battery_Status_Fault;
						}
						else if(n==Battery_Status_Normal)
						{
								pRecord->iBStatus=Battery_Status_Normal;
						}
						else
						{
							pRecord->iBStatus=Battery_Status_Null;
						}


					}
					else
					{
						pRecord->iBStatus=Battery_Status_Null;
					}
				}

				//Ŀ��ָ������
				if(m_dbmg_sdb->GetElementByID( SCADA_TAB_Analog_ID, pRecord->iUpValAlgID,&Analog))
				{
					if(Analog.iQuality==Quality_Good)
					{
							pRecord->fValUp=Analog.fVal;
						
					}
				}
				

			}
			/*	}*/

			pRecord ++;
		}

	}
	else
	{
		TRACE("relay table is empty!");
	}
	return TRUE;
}
int agcFuncRtdb::getAgcAlarm(enmAgcAlarmKey key)
{
	BOOL ret=FALSE;
	INT nMaxIndex = m_dbmg_agc->getRecordCountByID(AGC_TAB_Alarm_ID)-1;	
	if( nMaxIndex < 0 )
		return 0;

	AGC_Alarm* pRecord = (AGC_Alarm*)m_dbmg_agc->getDataAddress(AGC_TAB_Alarm_ID);

	if(pRecord!=NULL)
	{
		for(int i=0;i<=nMaxIndex;i++)
		{
			if(strcmp(pRecord->strKey,g_AgcAlarmKey[key])==0)
			{
				ret=pRecord->iVal;
				break;
			}

			pRecord ++;
		}

	}
	else
	{
		TRACE("AGC_Warn table is empty!");
	}
	return ret;
}