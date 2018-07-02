#include "stdafx.h"
#include "gSystemDef.h"
#include "Q_ProtocolDBQH.h"
#include "tinyxmlinclude.h"
#include "Q_HttpLink.h"
#include <curl/curl.h>
#include "json/json.h" 
#include <math.h>
Q_ProtocolDBQH *g_Protocol=NULL;
#ifdef UNIX_ENV
#ifdef __cplusplus
extern "C" {
#endif
#endif //OS_UNIX
	int Q_ProtocolDBQH::hex_char_value(char c)   
	{   
		if(c >= '0' && c <= '9')   
			return c - '0';   
		else if(c >= 'a' && c <= 'f')   
			return (c - 'a' + 10);   
		else if(c >= 'A' && c <= 'F')   
			return (c - 'A' + 10);   
	//	assert(0);   
		return 0;   
	}   
	int Q_ProtocolDBQH::hex_to_decimal(const char* szHex, int len)   
	{   
		int result = 0;   
		for(int i = 0; i < len; i++)   
		{   
			result += (int)pow((float)16, (int)len-i-1) * hex_char_value(szHex[i]);   
		}   
		return result;   
	}  
//�������ݷ���
size_t Q_ProtocolDBQH::funcDianBiao(void *strptr, size_t size, size_t nmemb, void *stream)
{
	Json::Reader reader;
	Json::Value root;

	std::string strSQL;
	strSQL=(char *)strptr;

	if (reader.parse(strSQL, root))
	{
		std::string strHour;
		printf("root size=%d\n",root.size());

	
		Q_dataManager *m_pMgr=NULL;
		Q_ProtocolDBQHSetting *m_Setting=NULL;
		if(g_Protocol!=NULL)
		{

			m_pMgr=g_Protocol->getDataManager();
			m_Setting=g_Protocol->getSetting();
		}
		if(m_pMgr==NULL||m_Setting==NULL)
		{
			return 0;
		}
		Q_RECORDDATATAG dataArray;
		dataArray.eDataType=eDataTypeFloat;
		for (int i = 0; i<root.size(); i++)
		{
			strHour = root[i]["datetime"].asString();

			    printf("datetime=%s\n",strHour.c_str());

				double 	dE = root[i]["addEnergy"].asDouble();
				double 	dPower = root[i]["p"].asDouble();
				std::string strMac=root[i]["mac"].asString();
				printf("get current Energy =%f,power=%f,addr=%d\n",dE,dPower,hex_to_decimal(strMac.c_str(),strMac.size()));

				int nDevice=hex_to_decimal(strMac.c_str(),strMac.size());
		
				if(m_Setting!=NULL)
				{
						dataArray.channelID=m_Setting->getChannelID();
				}
			
				dataArray.device=nDevice;
	
				dataArray.no=1;//�й�
				dataArray.fValue=(float)dPower;
				if(m_pMgr!=NULL)
				m_pMgr->recordDataValue(&dataArray,1);
				
				//����
				dataArray.no=5;
				dataArray.fValue=(float)dE;
				if(m_pMgr!=NULL)
				m_pMgr->recordDataValue(&dataArray,1);
				
			
			

		}
	}

	return 0;
}
//�������ݷ���
size_t Q_ProtocolDBQH::funcTransduce(void *strptr, size_t size, size_t nmemb, void *stream)
{
	Json::Reader reader;
	Json::Value root;

	std::string strSQL;
	strSQL=(char *)strptr;

	if (reader.parse(strSQL, root))
	{
		std::string strHour;
		printf("root size=%d\n",root.size());


		Q_dataManager *m_pMgr=NULL;
		Q_ProtocolDBQHSetting *m_Setting=NULL;
		if(g_Protocol!=NULL)
		{
			m_pMgr=g_Protocol->getDataManager();
			m_Setting=g_Protocol->getSetting();
		}
		if(m_pMgr==NULL||m_Setting==NULL)
		{
			return 0;
		}
		Q_RECORDDATATAG dataArray;
		dataArray.eDataType=eDataTypeFloat;
		for (int i = 0; i<root.size(); i++)
		{
			strHour = root[i]["updatedAt"].asString();

			printf("updatedAt=%s\n",strHour.c_str());

			double 	dE = root[i]["energy"].asDouble();
			double 	dPower = root[i]["powerP"].asDouble();
		
			double 	dCurrent = root[i]["current"].asDouble();
			double 	dVoltage = root[i]["voltage"].asDouble();
			double 	dPowerQ = root[i]["powerQ"].asDouble();

			std::string strMac=root[i]["serialNumber"].asString();
			
			// �й� 1
			//�޹�2 
			//�ۼƵ��� 3
			//���� 4
			//��ѹ 5
			
			printf("get current addr=%s:p=%f,q=%f,energy =%f,current=%f,voltage=%f\n",strMac.c_str(),dPower,dPower,dE,dCurrent,dVoltage);

			int nDevice=atoi(strMac.c_str());

			if(m_Setting!=NULL)
			{
				dataArray.channelID=m_Setting->getChannelID();
			}

			dataArray.device=nDevice;

			dataArray.no=1;//�й�
		    dataArray.fValue=(float)dPower;
			m_pMgr->recordDataValue(&dataArray,1);

			dataArray.no=2;//�й�
			dataArray.fValue=(float)dPower;
			m_pMgr->recordDataValue(&dataArray,1);

			dataArray.no=3;//�й�
			dataArray.fValue=(float)dE;
			m_pMgr->recordDataValue(&dataArray,1);

			dataArray.no=4;//�й�
			dataArray.fValue=(float)dCurrent;
			m_pMgr->recordDataValue(&dataArray,1);

			//����
			dataArray.no=5;
			dataArray.fValue=(float)dVoltage;
			m_pMgr->recordDataValue(&dataArray,1);




		}
	}

	return 0;
}
int  mod_init(int dbg_level)
{
	//g_debug_level = dbg_level;

	return 0;
}	

Q_chDownReturn* OUTPUTAPI create_protocol_object(Q_chDownCommPara *v_param)
{
	printf("create_protocol_object CommHttpQHCGQ\n");
	//int ret;
	Q_chDownReturn *chRet=new Q_chDownReturn();

	int ctype = v_param->cfg->type;

	int  n_sleepMills=500;//ԭ���� 1000
	//=======
	Q_taskBase* taskTest = new Q_taskBase();
	taskTest->setBackupMode(eCommonBackupCold);
	taskTest->setRtdbInf(v_param->m_pMgr);//2015-06-23

	bool flgPri=true;

	
	Q_ProtocolDBQHSetting *modbusSetting=new Q_ProtocolDBQHSetting();
	modbusSetting->setFilePath(v_param->strFilePath);

	modbusSetting->initSetting();//����xml����
	
	modbusSetting->setChannelType(v_param->cfg->type);//����ͨ������

	Q_commonBuffer* bufferReceive = new Q_commonBuffer();
	Q_commonBuffer* bufferSend = new Q_commonBuffer();

	Q_channelBase* channelTest = new Q_channelBase();
	channelTest->setChannelID(v_param->cfg->id);

	channelTest->setPrimaryFlag(flgPri);
	channelTest->setReceiveBuffer(bufferReceive);
	channelTest->setSendBuffer(bufferSend);

	Q_ModbusMasterCommandManager* pCommandList = new Q_ModbusMasterCommandManager();
	channelTest->setCommandList(pCommandList);

	Q_HttpLink* linkTcp = new Q_HttpLink();
	Q_HttpLinkSetting* tLinkSetting = new Q_HttpLinkSetting();

	TRACE("User Q_tcpClientLink \r\n");
//	char *tmp=NULL;
	//tmp=modbusSetting->getContentID(2,1);


	Q_linkConfig* tLinkConfig = new Q_linkConfig();
	tLinkConfig->setLinkSetting(tLinkSetting);
	tLinkConfig->setReceiveBuffer(bufferReceive);
	tLinkConfig->setSendBuffer(bufferSend);

	channelTest->setLinkSetting(tLinkSetting);
	channelTest->setLinkConfig(tLinkConfig);
	channelTest->setLink(linkTcp);
	channelTest->setProtocolSetting(modbusSetting);


	Q_protocolConfig* protocolConfig = new Q_protocolConfig();
	protocolConfig->setProtocolSetting(modbusSetting);
	protocolConfig->setReceiveBuffer(bufferReceive);
	protocolConfig->setSendBuffer(bufferSend);
	channelTest->setProtocolConfig(protocolConfig);

	protocolConfig->setProtocolCommandManager(pCommandList);

	Q_ProtocolDBQH* protocol = new Q_ProtocolDBQH();
	protocol->setRtdbInf(v_param->m_pMgr);
	
	g_Protocol=protocol;
	//n_sleepMills=modbusSetting->getSleepPerTimes();
	n_sleepMills=30000;
	protocol->setSendMsg(0);

	modbusSetting->setChannelID(v_param->cfg->id);//����ͨ��id

	channelTest->setProtocol(protocol);	

	//m_GSettting.addChannel(channelTest);

	taskTest->addChannel(channelTest);

	Q_simpleThread* threadTest = new Q_simpleThread();
	threadTest->setThreadPeriod(n_sleepMills);
	threadTest->addTask(taskTest);


	Q_taskRunningUnit* unit = new Q_taskRunningUnit();
	unit->setDriverMode(eCommonModeSingle);
	unit->addTask(taskTest);
	unit->addThread(threadTest);
	//unit->addTask(taskTest);
	//unit->addThread(threadTest);
	
	chRet->unit=unit;
	chRet->task=taskTest;
	sprintf_s(chRet->protocol,sizeof(v_param->cfg->protocol),"%s",v_param->cfg->protocol);
	chRet->channelID=v_param->cfg->id;
	chRet->enabled=v_param->cfg->enabled;
	
	if(chRet)
		return chRet;	
	else
		return NULL;
}
//Ϊ�˱�֤���ٵĶ�̬�����ͷſ��һ�£����Ӵ˷���
void OUTPUTAPI free_protocol_object(Q_chDownReturn* pProtocol)
{
	if ((Q_chDownReturn*)pProtocol)
	{
		delete (Q_chDownReturn*)pProtocol;
		pProtocol = NULL;
	}
}

//�������
// ң����ң��
int OUTPUTAPI add_command(Q_taskBase *task,LPQ_RECORDDATATAG rec)
{
	 int ret=0;
	
	 return ret;
}

//��ӡ���¼�¼
int  OUTPUTAPI print_change_log()
{
	printf("=== CommModbusStd Protocol Dynamic lib====\n");
	printf("=== 2016-07-22====\n");
	printf("1�����Ӵ�ӡ����汾����\n");
	
	return 1;
}

#ifdef UNIX_ENV
#ifdef __cplusplus
}
#endif
#endif //OS_UNIX

//----------------------------------------------------------------------------
//*
//*
//* Q_protocolModbusTcpServerStandardCommand class
//*
//*
Q_ProtocolDBQHCommand::Q_ProtocolDBQHCommand(void)
{
	
}

Q_ProtocolDBQHCommand::~Q_ProtocolDBQHCommand(void)
{

}





//----------------------------------------------------------------------------
//*
//*
//* Q_protocolModbusTcpServerStandardCommandManager class
//*
//*
Q_ModbusMasterCommandManager::Q_ModbusMasterCommandManager(void)
{
}

Q_ModbusMasterCommandManager::~Q_ModbusMasterCommandManager(void)
{
}
//----------------------------------------------------------------------------
//*
//*
//* Q_protocolModbusTcpServerStandardSetting class
//*
//*
Q_ProtocolDBQHSetting::Q_ProtocolDBQHSetting(void)
{
	

}

Q_ProtocolDBQHSetting::~Q_ProtocolDBQHSetting(void)
{
	m_vGroup.clear();
	m_vContent.clear();
}

void Q_ProtocolDBQHSetting::setFilePath(char *v_path)
{
	strcpy(m_sFilePath,v_path);
}
bool Q_ProtocolDBQHSetting::initSetting()
{
	return init();
}
bool Q_ProtocolDBQHSetting::init(void)
{
	TiXmlDocument doc(m_sFilePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
#ifdef _DEBUG
		sprintf(LastError, "���������ļ�������ǰ�ļ�:%s����ǰ�к�:%d\n",__FILE__,__LINE__);
		TRACE("���������ļ�������ǰ�ļ�:%s����ǰ�к�:%d\n",__FILE__,__LINE__);
		printf(LastError);
#endif
		return false;
	}

	TiXmlNode* node = 0;
	TiXmlNode* root = 0;
	TiXmlNode* nodeItem = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	root = doc.FirstChild( "Setting" );
	todoElement = root->ToElement();

	assert( todoElement  );

	for ( node = todoElement->FirstChild( "Record" );
		node;
		node = node->NextSibling( "Record" ) )
	{
		itemElement=node->ToElement();

		UINT nID=atoi(itemElement->Attribute("id"));
		
		
			ST_ModCfgT group;

			group.id=nID;
			sprintf_s(group.name,sizeof(group.name),"%s",itemElement->Attribute("name"));
			group.type=eModXmlDataTypeString;

			m_vGroup.push_back(group);

			for (nodeItem=node->FirstChild("Field");nodeItem;
				nodeItem=nodeItem->NextSibling("Field"))
			{
				itemElement=nodeItem->ToElement();
				ST_ModCfgT content;
				content.group=group.id;

				content.id=atoi(itemElement->Attribute("id"));
				sprintf_s(content.name,sizeof(content.name),"%s",itemElement->Attribute("name"));
				sprintf_s(content.value,sizeof(content.value),"%s",itemElement->Attribute("value"));

				const char *type=itemElement->Attribute("type");
				if (strcmp(type,"STRING")==0)
				{
					content.type=eModXmlDataTypeString;

				}
				else if (strcmp(type,"ENUM")==0)
				{
					content.type=eModXmlDataTypeEnum;
				}
				else if (strcmp(type,"INT")==0)
				{
					content.type=eModXmlDataTypeInt;
				}
				else if (strcmp(type,"FLOAT")==0)
				{
					content.type=eModXmlDataTypeFloat;
				}
				m_vContent.push_back(content);

			}
		

	}
	// 	if (m_pnt_cmd_count==0 && m_alg_cmd_count==0)
	// 	{
	// 		CString strErr;
	// 		strErr.Format("modbusTCP��Լ��û���ٻ�ң����ң�����ã��ļ���%s",m_sFilePath);
	// 		MessageBox(AfxGetMainWnd()->m_hWnd,strErr,"����",MB_ICONERROR|MB_OK);
	// 		exit(0);
	// 	}
	return true;
}
bool Q_ProtocolDBQHSetting::reloadLog()
{
	TiXmlDocument doc(m_sFilePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
#ifdef _DEBUG
		sprintf_s(LastError,sizeof(LastError),"���������ļ�����,��ǰ�к�:%d\n",__LINE__);
		printf(LastError);
#endif
		return false;
	}

	TiXmlNode* node = 0;
	TiXmlNode* root = 0;
	TiXmlNode* nodeItem = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	root = doc.FirstChild( "Setting" );
	todoElement = root->ToElement();

	assert( todoElement  );

	for ( node = todoElement->FirstChild( "Record" );
		node;
		node = node->NextSibling( "Record" ) )
	{
		itemElement=node->ToElement();


		UINT groupid=atoi(itemElement->Attribute("id"));
		if(groupid==10)
		{

			for (nodeItem=node->FirstChild("Field");nodeItem;
				nodeItem=nodeItem->NextSibling("Field"))
			{
				itemElement=nodeItem->ToElement();


				UINT id=atoi(itemElement->Attribute("id"));

				for (UINT i=0;i<m_vContent.size();i++)
				{
					if (m_vContent[i].group==groupid &&
						m_vContent[i].id==id)
					{
						sprintf_s(m_vContent[i].value,sizeof(m_vContent[i].value),"%s",itemElement->Attribute("value"));
					}
				}


			}
		}
	}

	return true;
}
char *Q_ProtocolDBQHSetting::getContentID(const UINT &v_idx,const UINT &v_grp)
{
	for (UINT i=0;i<m_vContent.size();i++)
	{
		if (m_vContent[i].group==v_grp &&
			m_vContent[i].id==v_idx)
		{
			return m_vContent[i].value;
		}
	}
	return NULL;
}

Q_ProtocolDBQH::Q_ProtocolDBQH()
{


	m_cpuEndian=eBigEndian;//Ĭ���Ǵ��ģʽ
	m_LastCmd=NULL;
	m_nMsgID=0;
	m_PrintSetting=NULL;
	//m_wnd=NULL;
	memset(m_LastMsg,0,sizeof(m_LastMsg));
	memset(m_LastMsg,0,sizeof(m_LastMsg));

	m_firstRun=true;
}
Q_ProtocolDBQH::~Q_ProtocolDBQH()
{

}
bool Q_ProtocolDBQH::initializeProtocol()
{
	return true;
}

bool Q_ProtocolDBQH::explainData()
{
 
	return true;
}

bool Q_ProtocolDBQH::explainDataEx(char* buff,int nCount,int& nCurrentPos)
{
	
	return true;
}
//��������
bool Q_ProtocolDBQH::handleFrameDianBiao()
{
	CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(curl, CURLOPT_URL, "http://101.132.142.49:5000/realtimepower");
  
	// ���ý������ݵĴ������ʹ�ű���
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, funcDianBiao);
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);
	
  }
  return true;
}
//������������
bool Q_ProtocolDBQH::handleFrameTransduce()
{
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) 
   {
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/getMeterData?username=guest&password=guest");
  
	// ���ý������ݵĴ������ʹ�ű���
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,funcTransduce);
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  return true;
}
bool Q_ProtocolDBQH::makeFrame()
{
  time_t cur;
  if(!m_firstRun)
  {
	  //����30���һ��
	  time(&cur);
	  if(cur<m_now+30)
	  {
		  return true;
	  }
	
  }
  else
  {
	  m_firstRun=false;
  }

  handleFrameDianBiao();
  handleFrameTransduce();
  

  	time(&m_now);

	return true;
}

bool Q_ProtocolDBQH::terminateProtocol()
{
	return true;

}
bool Q_ProtocolDBQH::startProcess()
{
		m_eProtocolStatus = eCommonStatusNormal;
		m_PrintSetting= (Q_ProtocolDBQHSetting *)m_pProcotolConfig->getProtocolSetting();

		if(m_PrintSetting!=NULL)
		{
			//�¹�Լ��ӡ
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"��ʹ����ʼ...");
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

		}

		printf("Q_protocolModbusMaster::startProcess() eee\n");

	return true;
}
bool Q_ProtocolDBQH::stopProcess()
{
	return true;
}
void Q_ProtocolDBQH::SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen)
{
	//TRACE("Q_protocolIEC104Master::SendMsgPrint,g_rtdbInfIEC104=%08x",m_pMgr->getRtdbInf());
			
	//g_rtdbInfIEC104 = m_pMgr->getRtdbInf();
	//CFEPFuncRtdb *m_rtdbInf = g_rtdbInfIEC104;

	CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
	//������ж���״̬�ͷ���������Ϣ���߸�������

	bool bSend = false;

	SCADA_TUnit ElementTUnit;
	m_rtdbInf->getScadaDb()->GetElementByID(SCADA_TAB_TUnit_ID,nchannelid,&ElementTUnit);

	if (ElementTUnit.iRecordFlag != 0)
	{
		bSend = true;
	}
	else
	{
		bSend = false;
		return;
	}

	if (nmsgtype == eMsgTypeTx || nmsgtype == eMsgTypeRx)//������Ϣ��������Ϣ
	{
		if (ElementTUnit.iRecPacketFlg != 0)//������Ϣ���俪��
		{
			bSend = true;
		}
		else
		{
			bSend = false;
			return;
		}
	}
	else if (nmsgtype == eMsgTypeMsg || nmsgtype == eMsgTypeError || nmsgtype == eMsgTypeCh)//��ʾ��Ϣ����Ϣֵ��������Ϣ�����ݽ��͡���Լ����
	{
		if (ElementTUnit.iRecPromptFlg != 0)//��ʾ��Ϣ���俪��
		{
			bSend = true;
		}
		else
		{
			bSend = false;
			return;
		}
	}

	if (nlen>=sizeof(m_LastMsg))
	{
		nlen = sizeof(m_LastMsg);
	}

	if (bSend == true)
	{
		SYSTEM_Process *pProcess= m_rtdbInf->getMainDb()->FindProcessByProType(pt_DBGSVR);
		if(pProcess==NULL)
		{
			TRACE("process DBGSVR not exist in did sendMsgPrint exit\n");
			return ;
		}
		else
		{
			if((pProcess->iFlag&ps_Run)!=ps_Run)
			{
				//���̲����У��˳�
				TRACE("process DBGSVR not run sendMsgPrint exit\n");
				return ;
			}
		}

		if (nlen>Q_MSGTRAN_MAXMSGLEN)
		{
			int sendcs = 0;
			sendcs = nlen/Q_MSGTRAN_MAXMSGLEN;
			int sendys = nlen%Q_MSGTRAN_MAXMSGLEN;

			for (int n=0; n<=sendcs; n++)
			{	
				MesMsgPrint MsgPrint = {0};
				MsgPrint.ichannelid=nchannelid;
				nmsgtype = nmsgtype|0x0080;
				MsgPrint.msgtype=nmsgtype;

				if (n == sendcs)
				{
					MsgPrint.nlen = sendys;
					memcpy(MsgPrint.msg,mse+n*Q_MSGTRAN_MAXMSGLEN,sendys);
				}
				else
				{
					MsgPrint.nlen = Q_MSGTRAN_MAXMSGLEN;
					memcpy(MsgPrint.msg,mse+n*Q_MSGTRAN_MAXMSGLEN,Q_MSGTRAN_MAXMSGLEN);
				}

				//m_rtdbInf->getMainDb()->SendMessage(pt_DBGSVR,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
				

				m_rtdbInf->getMainDb()->SendMessageTo(pProcess->ID32,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
			}
		}
		else
		{		
			MesMsgPrint MsgPrint = {0};
			MsgPrint.ichannelid=nchannelid;
			MsgPrint.msgtype=nmsgtype;
			MsgPrint.nlen = nlen;
			memcpy(MsgPrint.msg,mse,nlen);
			//m_rtdbInf->getMainDb()->SendMessage(pt_DBGSVR,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
			//2016-10-28 tjs ��Ϊ��Ӧ�ø�����������Ϣ
			

			m_rtdbInf->getMainDb()->SendMessageTo(pProcess->ID32,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
			/*char strMsg[200] = {0};
			sprintf_s(strMsg,sizeof(strMsg),"MsgPrint:stid = %d msgtype id = %d message = %s",MsgPrint.ichannelid,MsgPrint.msgtype,MsgPrint.msg);
			TRACE("strMsg");*/
		}
	}
}
Q_dataManager * Q_ProtocolDBQH::getDataManager()
{
	return m_pMgr;
}
Q_ProtocolDBQHSetting* Q_ProtocolDBQH::getSetting()
{
	return m_PrintSetting;
}