#include "stdafx.h"
#include "Q_104SlaveTask.h"
#include "Q_channelBase.h"
#include "Q_ProtocolIEC101Slave.h"


//----------------------------------------------------------------------------
//*
//*
//* Q_modbusTcpServerTask class
//*
//*

Q_104SlaveTask::Q_104SlaveTask(void)
{
	m_creatTime = time(0);//����ʱ��
}

Q_104SlaveTask::~Q_104SlaveTask(void)
{
}

bool Q_104SlaveTask::acceptClient()
{
	if(time(0) - m_creatTime < DATA_UPLOAD_DELAY) //IEC104��վ������ʱ����ʱ�䣬��λ�롣Ϊ��ֹɽ���ֳ�������������Ŀ���
	{
		return false;
	}
	if(m_pTcpServerLink->getLinkStatus() != eCommonStatusNormal)
	{
		/*TRACE("Q_104SlaveTask::m_pTcpServerLink status is abnormal\n");*/
		return false;
	}

	if(m_pTcpServerLink->acceptClient() == true)
	{
		/*if(getClientChannel()!=NULL && getClientChannel()->getChannelStatus()==eCommonStatusNormal) return false;*/
		/*TRACE("m_pTcpServerLink->acceptClient() == true 1\n")*/;

		/*clinet buffer*/
        Q_IEC104Setting* ppp = (Q_IEC104Setting*)m_pProtocolSetting;
		int chid=0;
		if(ppp!=NULL)
		{

			chid=ppp->getChannelID();

			char strTmp[50];
			memset(strTmp,NULL,sizeof(strTmp));
			sprintf_s(strTmp,sizeof(strTmp),"%s:%d",inet_ntoa(m_pTcpServerLink->getClientAddress().sin_addr),m_pTcpServerLink->getClientAddress().sin_port);
			TRACE("Q_104SlaveTask:a new client connected:%s\n",strTmp);

			Q_104BaseCfg v_basecfg;
			ppp->getBaseSetting(&v_basecfg);
			if(v_basecfg.remServer[0].valid)
			{
			   //���ù��˹���
				char strClientIp[50];
				memset(strClientIp,NULL,sizeof(strClientIp));
				sprintf_s(strClientIp,sizeof(strClientIp),"%s",inet_ntoa(m_pTcpServerLink->getClientAddress().sin_addr));

				//
				bool findflg=false;
				for(int i=0;i<MAX_REMOTE_NUM;i++)//��Ϊ���������
				{
					if(v_basecfg.remServer[i].valid)
					{
						if(strcmp(v_basecfg.remServer[i].ip,strClientIp)==0)
						{
							chid=v_basecfg.remServer[i].port;
							findflg=true;
							break;
						}
					}
				}
				if(!findflg)
				{
					TRACE("Q_104SlaveTask::this ip is not in authentication list\n");
	
                    return false;
				
				}
			}
	    }
		m_clientChannel = new Q_channelBase();
		Q_commonBuffer* bufferReceive = new Q_commonBuffer();
		Q_commonBuffer* bufferSend = new Q_commonBuffer();	 
		m_clientChannel->setReceiveBuffer(bufferReceive);
		m_clientChannel->setSendBuffer(bufferSend);

		/*clinet link info*/
		Q_tcpServerClientLink* linkTcpClient = new Q_tcpServerClientLink();
		linkTcpClient->setLogStatus(m_pTcpServerLink->getLogStatus());

		Q_tcpServerClientLinkSetting* tLinkSetting = new Q_tcpServerClientLinkSetting();
		tLinkSetting->setSocket(m_pTcpServerLink->getClientSocket());
		tLinkSetting->setAddress(m_pTcpServerLink->getClientAddress());

		Q_linkConfig* tLinkConfig = new Q_linkConfig();
		tLinkConfig->setLinkSetting(tLinkSetting);
		tLinkConfig->setReceiveBuffer(bufferReceive);
		tLinkConfig->setSendBuffer(bufferSend);
		tLinkConfig->setLinkSetting(tLinkSetting);

		m_clientChannel->setLinkConfig(tLinkConfig);
		m_clientChannel->setLink(linkTcpClient);
		m_clientChannel->setLinkSetting(tLinkSetting);

		/*clinet protocolsetting config*/
		//���¶�һ���ļ��� 2016-11-18
		//��ʵʱ�����
		//����ͨ��ID��ѯͨ�����е����ݣ�Ȼ�����ж��ӹ�Լ��Ȼ���ٲ�ѯ��Լ���subType������
		/*else
		{
			tags->setAnalogScope(v_param->cfg->YCbegin,v_param->cfg->YCend,(v_param->cfg->YCend-v_param->cfg->YCbegin+1));
			tags->setDigitalScope(v_param->cfg->YXbegin,v_param->cfg->YXend,(v_param->cfg->YXend-v_param->cfg->YXbegin+1));
		}*/
		//��ô��֪���ļ����ء�
		//
		//����֧���ϴ���ң��ң�Ŷ���������2016-11-18 tjs
		Q_IEC104Setting* proSetting=NULL;
		char strFilePath[MAX_PATH];
		memset(strFilePath,0,sizeof(strFilePath));
		if(checkReInitSetting(chid,strFilePath)==true)
		{
			char * pFilePath = getenv( "CCSOSD" );
			char szIniFile[MAX_PATH];
			memset(szIniFile,0,sizeof(szIniFile));

			if( pFilePath == NULL )
				pFilePath = getcwd(szIniFile, MAX_PATH);

			//tjs 2016-11-19  ֧�ֵ�ǰ�������ɱ䣬�����з�����
			char szSysConfigIni[MAX_PATH];
			GetSysConfigFile(szSysConfigIni);

			char szProjectFile[MAX_PATH];
			memset(szProjectFile,0,sizeof(szProjectFile));
			if((GetPrivateProfileString2("RTDB","��ǰ����","support",szProjectFile,MAX_PATH,szSysConfigIni) > 0))
			{
				//sprintf(szIniFile, "/project/%s/Channel/",szIniFile,szProjectFile);
				if(strlen(szProjectFile)<=0)
				{
					strcpy(szProjectFile,"support");
				}
#ifdef UNIX_ENV
				strcpy(szIniFile, pFilePath);
				sprintf(szIniFile, "%s/project/%s/Channel/",szIniFile,szProjectFile);
				
#else
				strcpy(szIniFile, pFilePath);
				sprintf(szIniFile, "%s/project/%s/Channel/",szIniFile,szProjectFile);
#endif	//OS_UNIX

			}
			else
			{
#ifdef UNIX_ENV
				strcpy(szIniFile, pFilePath);
				strcat(szIniFile, "/project/support/Channel/");
#else
				strcpy(szIniFile, pFilePath);
				strcat(szIniFile, "\\project\\support\\Channel\\");
#endif	//OS_UNIX
			}
			//2016-11-19




			char strFilePath2[MAX_PATH];
			sprintf_s(strFilePath2,MAX_PATH,"%s%s",szIniFile,strFilePath);

			proSetting=new Q_IEC104Setting();
			proSetting->setFilePath(strFilePath2);
			if(proSetting->initSetting())
			{
				TRACE("#######IEC104 Slave special read %s success \n",strFilePath2);
			}
			else
			{
					TRACE("#######IEC104 Slave special read %s failed \n",strFilePath2);
			}
			/*����xml���ݣ���ʼ��Э������*/
			//ע��˴�������У�����ͻṫ����ַΪ��
			Q_104BaseCfg v_basecfg;
			proSetting->getBaseSetting(&v_basecfg);

			TRACE("#######IEC104 Slave special client channel,chid=%d\n",chid);
			initNewSetting(chid,proSetting);

			//iecSetting->setChannelType(v_param->cfg->type);
			//iecSetting->setChannelClass(v_param->cfg->classname);
		}
		else
		{
			TRACE("#####IEC104 Slave common client channel,chid=%d\n",chid);
			proSetting = new Q_IEC104Setting(ppp);
		}
		

		proSetting->setChannelID(chid);

		char strTmp[50];
		memset(strTmp,NULL,sizeof(strTmp));
		sprintf_s(strTmp,sizeof(strTmp),"%s:%d",inet_ntoa(m_pTcpServerLink->getClientAddress().sin_addr),m_pTcpServerLink->getClientAddress().sin_port);
		proSetting->setClientAddr(strTmp);
		m_clientChannel->setProtocolSetting(proSetting);

		Q_IEC104CommandManager* pCommandList = new Q_IEC104CommandManager();
		m_clientChannel->setCommandList(pCommandList);
		Q_protocolConfig* protocolConfig = new Q_protocolConfig(); 
		protocolConfig->setProtocolSetting(proSetting);
		protocolConfig->setReceiveBuffer(bufferReceive);
		protocolConfig->setSendBuffer(bufferSend);
		protocolConfig->setProtocolCommandManager(pCommandList);
		m_clientChannel->setProtocolConfig(protocolConfig);

		/*clinet protocol config*/
		Q_protocolIEC104Slave * protocol = NULL;
		if(strcmp(ppp->getChannelClass(), "CommIEC104SlaveSD") == 0)
		{
			protocol = new Q_protocolIEC104SlaveShanDongPlaneCurve();
		}
		else
		{
			protocol = new Q_protocolIEC104Slave();
		}
		m_clientChannel->setProtocol(protocol);	
	//	m_clientChannel->setChannelID(proSetting->getChannelID());
        m_clientChannel->setChannelID(chid);//ԭ��ΪproSetting->getChannelID()
		m_clientChannel->setPrimaryFlag(false);/*һ��Ҫ����Ϊfalse,�����������Ҳ�᲻ͣ������*/
		/*TRACE("Q_104SlaveTask:: a new client create success\n");*/

		return true;
	}
	return false;
}
//�ж��Ƿ���Ҫ����Setting�ļ�
//chid ��ͨ����
//����ֵtrue���¶�ȡ�����ļ���ʹ��Setting
//false�����¶�ȡ�����ļ�
bool Q_104SlaveTask::checkReInitSetting(int chid,char out_strFilePath[])
{
	bool ret=false;
	CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
	if(m_rtdbInf==NULL)
	{
		TRACE("Q_104SlaveTask::checkReInitSetting m_rtdbInf is NULL ,return");
		return false;
	}

	CScadaDataBase *scada= m_rtdbInf->getScadaDb();
	if(scada==NULL)
	{
		TRACE("Q_104SlaveTask::checkReInitSetting scada is NULL ,return");
		return false;
	}
	
	
		//��ͨѶ��·����Լ���������ϳɰ�m_vChannel�õ�������
		CDBTable v_CommLink;
		scada->GetDBTableFromID(v_CommLink,SCADA_TAB_Channel_ID);
		
		int nCount=v_CommLink.GetRecordCount();
		
		SCADA_Channel* pRecord = (SCADA_Channel*)v_CommLink.GetDataAddress();
		
			for(int i=0;i<nCount;i++)
			{
				if(pRecord!=NULL)
				{
					if(pRecord->ID32==chid)
					{

						TRACE("Q_104SlaveTask::checkReInitSetting  find chid=%d in scada channel table \n",chid);
						strncpy(out_strFilePath,pRecord->strProtocolPara,strlen(pRecord->strProtocolPara));
						//�ٲ��Լ���еļ�¼
					
						CDBTable v_ProTbl;
						scada->GetDBTableFromID(v_ProTbl,SCADA_TAB_Protocol_ID);

						int nProCount=v_ProTbl.GetRecordCount();


						SCADA_Protocol* pProRecord = (SCADA_Protocol*)v_ProTbl.GetDataAddress();
						for(int ii=0;ii<nProCount;ii++)
						{
							if(pProRecord!=NULL)
							{

								if(pProRecord->ID32==pRecord->uProtocolID)
								{
									//�жϹ�Լ���������Ƿ�Ϊ2
									if(pProRecord->iProtocolSubType==2)
									{
TRACE("Q_104SlaveTask::checkReInitSetting  find iProtocolSubType==2 in scada protocol table \n",chid);
										ret=true;
										break;
									}
								}
								pProRecord++;
							}

						}
						if(ret)
							break;
					}
					pRecord++;
			}
			
		}
		
	return ret;


}
bool Q_104SlaveTask::initNewSetting(int chid,Q_IEC104Setting* proSetting)
{
	//��ͨ����������ļ���������ȡ�ļ���
	//Ȼ���ң�⡢ң�ű����ҵ���ͨ���Ŀ�ʼ����id����m_tags�ϡ�

	TRACE("Q_104SlaveTask initNewSetting begin\n");

	bool ret=false;
	CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
	if(m_rtdbInf==NULL)
	{
		return false;
	}

	CScadaDataBase *scada= m_rtdbInf->getScadaDb();
	if(scada==NULL)
	{
		return false;
	}
	if(proSetting==NULL)
	{
		return false;
	}
	
	CDBTable AnalogTbl;
	scada->GetDBTableFromID(AnalogTbl,SCADA_TAB_Analog_ID);

	CDBTable DigitTbl;
	scada->GetDBTableFromID(DigitTbl,SCADA_TAB_Digit_ID);

	Q_104StandardTags *tags=new Q_104StandardTags();

	
	//��˶�޸�20150727
	SCADA_Analog curAnalog;
	SCADA_Digit curDigit;
	
	int nAnalogRecordCount = scada->GetTableRecordCount(SCADA_TAB_Analog_ID );
	int last_uTuID=0;

	int beg_id=0,end_id=0;//ң���õ���
	int beg_yx_id=0,end_yx_id=0;

	int YCbegin=0,YCend=0;
	for (int j=0; j<nAnalogRecordCount; j++)
	{
		int nTempid32 = j+1;
		scada->GetElementByID(SCADA_TAB_Analog_ID, nTempid32,&curAnalog);

		if (chid==curAnalog.uTuID)
		{
			if(beg_id==0)
			{
				beg_id=curAnalog.ID32;
				YCbegin=beg_id;
				YCend=beg_id;
				last_uTuID=curAnalog.uTuID;
			}
			else
			{
				end_id=curAnalog.ID32;
			}	
		}
		if(beg_id!=0)
		{
			if((last_uTuID!=0&&last_uTuID!=curAnalog.uTuID)||
				curAnalog.ID32==nAnalogRecordCount)
			{
				YCend=end_id;
				break;
			}
		}

	}
	TRACE("special channel id=%d yc tags begin=%d,end=%d,count=%d\n",chid,YCbegin,YCend,YCend-YCbegin+1);
	tags->setAnalogScope(YCbegin,YCend,YCend-YCbegin+1);
	last_uTuID=0;
	
	int YXbegin=0,YXend=0;
	int nDigitRecordCount = scada->GetTableRecordCount(SCADA_TAB_Digit_ID );
	for (int j=0; j<nDigitRecordCount; j++)
	{
		int nTempid32 = j+1;
		scada->GetElementByID(SCADA_TAB_Digit_ID, nTempid32,&curDigit);

		if (chid==curDigit.uTuID)
		{
			if(beg_yx_id==0)
			{
				beg_yx_id=curDigit.ID32;
				YXbegin=beg_yx_id;
				YXend=beg_yx_id;
				last_uTuID=curDigit.uTuID;
			}
			else
			{
				end_yx_id=curDigit.ID32;
			}	
		}
		if(beg_yx_id!=0)
		{			
			if((last_uTuID!=0&&last_uTuID!=curDigit.uTuID)||
				curDigit.ID32==nDigitRecordCount)
			{
				YXend=end_yx_id;
				break;
			}
		}		
	}

	TRACE("special channel id=%d yx tags begin=%d,end=%d,count=%d\n",chid,YXbegin,YXend,YXend-YXbegin+1);
	tags->setDigitalScope(YXbegin,YXend,YXend-YXbegin+1);
	proSetting->setTagsManager(tags);
	return true;
}