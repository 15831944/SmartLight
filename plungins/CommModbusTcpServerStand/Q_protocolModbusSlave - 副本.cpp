#include "stdafx.h"
#include "gSystemDef.h"
#include "Q_protocolModbusSlave.h"
#include "tinyxmlinclude.h"
#include "Q_modbusSlaveTask.h"

Q_dataManager *m_pMgrModbusSlaveProtocolSetting; /*ModbusSlave��rtdbȫ�ֱ���*/

#ifdef UNIX_ENV
#ifdef __cplusplus
extern "C" {
#endif
#endif //OS_UNIX

int  mod_init(int dbg_level)
{
	return 0;
}	


Q_chDownReturn* OUTPUTAPI create_protocol_object(Q_chDownCommPara *v_param)
{

	TRACE("create_protocol_object Q_protocolModbusSlave");
	int ntype = v_param->cfg->type;

	Q_chDownReturn *chRet=new Q_chDownReturn();

	bool flgPri=true;

	/*tcp server link*/	
	Q_tcpServerLink* tcpServerLink = new Q_tcpServerLink();
	Q_tcpServerLinkSetting* tcpServerLinkSetting = new Q_tcpServerLinkSetting();
	tcpServerLinkSetting->setAnyIp(true);

	/*protocolsetting config*/
	Q_protocolModbusSlaveSetting *protocolSetting = new Q_protocolModbusSlaveSetting();
	protocolSetting->setFilePath(v_param->strFilePath);
	protocolSetting->initSetting();
	protocolSetting->setChannelID(v_param->cfg->id);//����ͨ��id
	protocolSetting->setChannelType(v_param->cfg->type);//����ͨ������

	//���Ϊ��̬ͨ���Ļ���Ӧ����ȫ�ֿ��Է��ʵġ�
	
	
	protocolSetting->setAnalogScope(v_param->cfg->YCbegin,v_param->cfg->YCend,v_param->cfg->YCend-v_param->cfg->YCbegin+1);
	protocolSetting->setDigitalScope(v_param->cfg->YXbegin,v_param->cfg->YXend,v_param->cfg->YXend-v_param->cfg->YXbegin+1);

	
	
	m_pMgrModbusSlaveProtocolSetting = v_param->m_pMgr;/*�趨ModbusSlave��rtdbȫ�ֱ���*/

	char *tmp=NULL;
	tmp=protocolSetting->getContentID(2,1);
	if(tmp!=NULL)
	{
		tcpServerLinkSetting->setServerIp(tmp);
	}

	tmp=protocolSetting->getContentID(3,1);
	if(tmp!=NULL)
	{
		tcpServerLinkSetting->setPort(atoi(tmp));
	}

	tmp= protocolSetting->getContentID(5,10);
	if(tmp != NULL)
	{
		tcpServerLink->setLogStatus(atoi(tmp));
	}
	tcpServerLink->setLinkSetting(tcpServerLinkSetting);

	/*modbus tcp server task*/
	Q_modbusSlaveTask* tcpServerTask = new Q_modbusSlaveTask();
	tcpServerTask->setTaskID(v_param->cfg->id);
	tcpServerTask->setTcpServerLink(tcpServerLink);
	tcpServerTask->setProtocolSetting(protocolSetting);

	/*server unit info*/
	Q_tcpServerUnitInfo* unitInfo = new Q_tcpServerUnitInfo();
	unitInfo->setServerTask(tcpServerTask);
	unitInfo->setRtdbInf(v_param->m_pMgr);/*�趨serverunitInfo��rtdbȫ�ֱ���*/

	/*thread*/
	Q_tcpServerListenThread* listenThread = new Q_tcpServerListenThread();
	listenThread->setThreadPeriod(200);
	listenThread->setServerUnitInfo(unitInfo);
	Q_tcpServerCheckThread* checkThread = new Q_tcpServerCheckThread();
	checkThread->setThreadPeriod(5000);
	checkThread->setServerUnitInfo(unitInfo);

	/*server unit*/
	Q_tcpServerTaskRunningUnit* unit = new Q_tcpServerTaskRunningUnit();
	unit->setCheckThread(checkThread);
	unit->setListenThread(listenThread);
	unit->setServerUnitInfo(unitInfo);


	chRet->unit=unit;
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
	printf("=== CommModbusTcpServerStand Protocol Dynamic lib====\n");
	printf("=== 2016-07-22====\n");
	printf("1�����Ӵ�ӡ����汾����\n");
	printf("=== 2016-10-28====\n");
	printf("1�����ķ�����Ϣ������ȥ��ȫ��ͬ��������Ϣ�Ĺ��ܣ�ֻ�������ͼ���\n");
	printf("=== 2016-11-25====\n");
	printf("1�������Ż�������Ϣ�����ݰѹ̶�ֵ�ĳ�strlen()+1�ķ�ʽ\n");
	printf("=== 2017-1-7====\n");
	printf("1�������ϴ�ң������ʱ֧��ת��ID������\n");
	printf("2�������ϴ�ң������ʱ֧��ת��ID������\n");
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
Q_protocolModbusSlaveCommand::Q_protocolModbusSlaveCommand(void)
{
}

Q_protocolModbusSlaveCommand::~Q_protocolModbusSlaveCommand(void)
{
}





//----------------------------------------------------------------------------
//*
//*
//* Q_protocolModbusTcpServerStandardCommandManager class
//*
//*
Q_protocolModbusSlaveCommandManager::Q_protocolModbusSlaveCommandManager(void)
{
}

Q_protocolModbusSlaveCommandManager::~Q_protocolModbusSlaveCommandManager(void)
{
}

bool Q_protocolModbusSlaveCommandManager::addCommand(Q_protocolCommandBase* pCommand)
{
	return false;
}






//----------------------------------------------------------------------------
//*
//*
//* Q_protocolModbusTcpServerStandardSetting class
//*
//*
Q_protocolModbusSlaveSetting::Q_protocolModbusSlaveSetting(void)
{
	memset(LastError,NULL,sizeof(LastError));
	m_nDebugToolMsgID = 0;
}

Q_protocolModbusSlaveSetting::Q_protocolModbusSlaveSetting(Q_protocolModbusSlaveSetting *other)
{
	 sprintf_s(m_sFilePath,sizeof(other->m_sFilePath),other->m_sFilePath);
     memset(LastError,NULL,sizeof(LastError));
	  m_vContent.clear();
	  m_vGroup.clear();

	 for(vector<ST_ModCfgT>::iterator ia=other->m_vContent.begin();ia!=other->m_vContent.end();ia++)
	 {
		 m_vContent.push_back(*ia);
	 }

	 for(vector<ST_ModCfgT>::iterator ia=other->m_vGroup.begin();ia!=other->m_vGroup.end();ia++)
	 {
		  m_vGroup.push_back(*ia);
	 }

 	 m_alg_count=other->m_alg_count;
 	 m_alg_begin=other->m_alg_begin;
 	 m_alg_end=other->m_alg_end;
 	 m_pnt_count=other->m_pnt_count;
 	 m_pnt_begin=other->m_pnt_begin;
 	 m_pnt_end=other->m_pnt_end;
	 m_wnd = other-> m_wnd;
	 m_nMsgID=other->m_nMsgID;
	 m_nCmdID=other->m_nCmdID;
	 m_nDebugToolMsgID = other->m_nDebugToolMsgID;
	 m_channel_id=other->m_channel_id;
	 m_channel_type= other->m_channel_type;
	 //fs20150820addupchannel
}

Q_protocolModbusSlaveSetting::~Q_protocolModbusSlaveSetting(void)
{
	m_vGroup.clear();
	m_vContent.clear();
}

void Q_protocolModbusSlaveSetting::setFilePath(char *v_path)
{
	sprintf_s(m_sFilePath,sizeof(m_sFilePath),v_path);
}

bool Q_protocolModbusSlaveSetting::initSetting()
{
	return init();
}

bool Q_protocolModbusSlaveSetting::init()
{
	TiXmlDocument doc(m_sFilePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
#ifdef _DEBUG
		sprintf_s(LastError, sizeof(LastError), "���������ļ�������ǰ�ļ�:%s����ǰ�к�:%d\n",__FILE__,__LINE__);
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

		ST_ModCfgT group;

		group.id=atoi(itemElement->Attribute("id"));
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
	return true;
}

char *Q_protocolModbusSlaveSetting::getContentID(const int &v_idx,const int &v_grp)
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

void   Q_protocolModbusSlaveSetting::setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_alg_count=nVaildCount;
	m_alg_begin=nAddrBegin;
	m_alg_end=nAddrEnd;
}

void   Q_protocolModbusSlaveSetting::setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_pnt_count=nVaildCount;
	m_pnt_begin=nAddrBegin;
	m_pnt_end=nAddrEnd;
}

int  Q_protocolModbusSlaveSetting::getAnalogBegin()
{
	return m_alg_begin;
}

int  Q_protocolModbusSlaveSetting::getAnalogEnd()
{
	return m_alg_end;
}

int    Q_protocolModbusSlaveSetting::getPointBegin()
{ 
	return m_pnt_begin;
}

int  Q_protocolModbusSlaveSetting::getPointEnd()
{
	return m_pnt_end;
}

int  Q_protocolModbusSlaveSetting::getAnalogCount()
{
	return m_alg_count;
}

int  Q_protocolModbusSlaveSetting::getPointCount()
{
	return m_pnt_count;
}

int Q_protocolModbusSlaveSetting::getRegisterBegin()
{
	int res=0;
	char *tmp=getContentID(3,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}

int Q_protocolModbusSlaveSetting::getRelayBegin()
{
	int res=0;
	char *tmp=getContentID(4,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}

int Q_protocolModbusSlaveSetting::getYkFunCode()
{
	int res=0;
	char *tmp=getContentID(2,3);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}

int Q_protocolModbusSlaveSetting::getYkOnValue()
{
	int res=0;
	char *tmp=getContentID(3,3);
	if(tmp!=NULL)
	{
		res=strtol(tmp,NULL,16);
	}
	return res;
}

int Q_protocolModbusSlaveSetting::getYkOffValue()
{
	long res=0;
	char *tmp=getContentID(4,3);
	if(tmp!=NULL)
	{
		res=strtol(tmp,NULL,16);
	}
	return (int)res;
}

int Q_protocolModbusSlaveSetting::getYcFunCode()
{
	int res=0;
	char *tmp=getContentID(9,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}

int Q_protocolModbusSlaveSetting::getYcUpType()
{
	int res=0;
	char *tmp=getContentID(1,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}

int Q_protocolModbusSlaveSetting::getYxFunCode()
{
	int res=0;
	char *tmp=getContentID(10,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}

int Q_protocolModbusSlaveSetting::getYtFunCode()
{
/*************ԭʼ **************/
//	int res=0;
//	char *tmp=getContentID(1,3);

	int res=0;
	char *tmp=getContentID(6,3);
	if(tmp!=NULL)
	res=atoi(tmp);
	else
	res=16;
	/*Ĭ��Ϊ0x16������ǰ��ģʽ*/
	
	return res;
}

int Q_protocolModbusSlaveSetting::getYtDataType()
{
	int res=1;
	char *tmp=getContentID(7,3);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}

int Q_protocolModbusSlaveSetting::getYtType()
{
	int res=1;
	char *tmp=getContentID(1,3);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
		/*ԭʼ        */
		/*
		1==���ֽڸ����͹�����Ϊ0x10
		2==���ֽڸ����͹�����Ϊ0x06
		*/
		// 		if(res==1)
		// 		{
		// 			res=0x10;
		// 		}
		// 		else if(res==2)
		// 		{
		// 			res=0x06;
		// 		}
		/*end*/
		/*����Ϊ1д���,2д����*/
	}
	return res;
}

int Q_protocolModbusSlaveSetting::getSyncFirst()
{
	long res=0;
	char *tmp=getContentID(11,2);
	if(tmp!=NULL)
	{
		res=strtol(tmp,NULL,16);
	}
	return (int)res;
}

bool Q_protocolModbusSlaveSetting::getSendClearBuffer()
{
    int res=0;
	char *tmp=getContentID(12,1);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res==0?false:true;
}

int Q_protocolModbusSlaveSetting::getDeviceId()
{
	int res=0;
	char *tmp=getContentID(4,1);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}

ModbusFloatUpSeq Q_protocolModbusSlaveSetting::getFloatUpSeq()
{
	ModbusFloatUpSeq seq;
	seq=eFloatUpSeq2301;
	int res=0;
	char *tmp=getContentID(2,2);
	if(tmp!=NULL)
	{
		int a=atoi(tmp);
		switch(a)
		{
		case 1:
			seq=eFloatUpSeqLowFirst;
			break;
		case 2:
			seq=eFloatUpSeqHighFirst;
			break;
		case 3:
			seq=eFloatUpSeq2301;
			break;
		case 4:
			seq=eFloatUpSeq1032;
		}
	}
	return seq;
}

ModbusYTMethod Q_protocolModbusSlaveSetting::getYTMethod()
{
	ModbusYTMethod seq;
	seq=eYTMethodFloatFun06;
	int res=0;
	char *tmp=getContentID(1,3);
	if(tmp!=NULL)
	{
		int a=atoi(tmp);
		switch(a)
		{
		case 1:
			seq=eYTMethodFloatFun10;
			break;
		case 2:
			seq=eYTMethodFloatFun06;
			break;
		}
	}
	return seq;
}

ModbusFloatUpSeq Q_protocolModbusSlaveSetting::getYTByteSeq()
{
	ModbusFloatUpSeq seq;
	seq=eFloatUpSeq2301;
	int res=0;
 	char *tmp=getContentID(5,3);
	if(tmp!=NULL)
	{
		int a=atoi(tmp);
		switch(a)
		{
		case 1:
			seq=eFloatUpSeqLowFirst;
			break;
		case 2:
			seq=eFloatUpSeqHighFirst;
			break;
		case 3:
			seq=eFloatUpSeq2301;
			break;
		case 4:
			seq=eFloatUpSeq1032;
			break;//fix
        default:
			seq=eFloatUpSeqLowFirst;
			break;
		}
	}
	else
	{
		seq=eFloatUpSeqLowFirst;
	}
	return seq;
}

void  Q_protocolModbusSlaveSetting::SetWndInfo(HWND wnd,int vMsgID,int vCmdID)
{
	m_wnd = wnd;
	m_nMsgID=vMsgID;
	m_nCmdID=vCmdID;
}

HWND Q_protocolModbusSlaveSetting::getWndHwnd()
{
	return m_wnd;
}

int Q_protocolModbusSlaveSetting::getWndMsgID()
{
	return m_nMsgID;
}

int Q_protocolModbusSlaveSetting::getWndCmdID()
{
	return m_nCmdID;
}

 bool Q_protocolModbusSlaveSetting::reloadLog()
 {
	 TiXmlDocument doc(m_sFilePath);  
	 doc.LoadFile();  
	 if(doc.ErrorId() > 0)  
	 {
#ifdef _DEBUG
		 sprintf_s(LastError,sizeof(LastError), "���������ļ�����,��ǰ�к�:%d\n",__LINE__);
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


		 int groupid=atoi(itemElement->Attribute("id"));
		 if(groupid==10)
		 {

			 for (nodeItem=node->FirstChild("Field");nodeItem;
				 nodeItem=nodeItem->NextSibling("Field"))
			 {
				 itemElement=nodeItem->ToElement();


				 int id=atoi(itemElement->Attribute("id"));

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

Q_protocolModbusSlave::Q_protocolModbusSlave()
{
    m_cpuEndian=eBigEndian;//Ĭ���Ǵ��ģʽ
	m_wnd=NULL;
	m_nMsgID=0;
	m_nDebugToolMsgID=0;
}

Q_protocolModbusSlave::~Q_protocolModbusSlave()
{

}

bool Q_protocolModbusSlave::initializeProtocol()
{
	m_PrintSetting = (Q_protocolModbusSlaveSetting *)m_pProcotolConfig->getProtocolSetting();

	if(m_PrintSetting!=NULL)
	{
		m_FloatUpSeq=m_PrintSetting->getFloatUpSeq();
		m_YtByteSeq=m_PrintSetting->getYTByteSeq();
		m_YtMethod=m_PrintSetting->getYTMethod();
	}
	if(Q_commonUtility::isBigEndian())
	{
		m_cpuEndian=eBigEndian;
	}
	else
	{
		m_cpuEndian=eLittleEndian;
	}
 return true;
}

bool Q_protocolModbusSlave::explainData()
{
	printf("Q_protocolModbusSlave::explainData()\n");

//	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	char bufferReceive[Q_FRAME_CONST4096];	
	memset(bufferReceive,NULL,sizeof(bufferReceive));
	int nLength = m_pProcotolConfig->getReceiveBuffer()->getDataLength();
	int nByteCount = 0;
	int nRes = 0;

	TRACE("-----Q_protocolModbusSlave explainData() nchannelid = %d -----\n",m_PrintSetting->getChannelID());

	while(nLength >  MODBUS_FRAMELEN_MIN)
	{
		nByteCount = m_pProcotolConfig->getReceiveBuffer()->readData(bufferReceive,Q_FRAME_DEFAULT_RECEIVEBUFFERSIZE,false);

		int nRes = 0;
		if(nByteCount > 0)
		{
			explainDataEx(bufferReceive,nByteCount,nRes);
		}
		m_pProcotolConfig->getReceiveBuffer()->removeData(nRes);
		nLength -= nRes;
		
	}

	if(nLength >=  MODBUS_FRAMELEN_MIN)
	{
		//TRACE("receive data len %d\n",nLength);
		nByteCount = m_pProcotolConfig->getReceiveBuffer()->readData(bufferReceive,Q_FRAME_DEFAULT_RECEIVEBUFFERSIZE,false);
		if(nByteCount > 0)
		{
			explainDataEx(bufferReceive,nByteCount,nRes);
		}
		m_pProcotolConfig->getReceiveBuffer()->removeData(nRes);
		//TRACE("remove data len %d,remain length %d \n",nRes,m_pProcotolConfig->getReceiveBuffer()->getDataLength());
	}
	return true;
}


bool Q_protocolModbusSlave::explainDataEx(char* buff,int nCount,int& nCurrentPos)
{
	int nIndexCur = 0;
	int nIndexFind = 0;//��Ϣ��Ч�Ŀ�ʼλ��,�׸��ֽ���6���ֽڵ�ǰ���ַ�
	bool bFindSync = false;

//	bFindSync = findSyncIndex(buff,nCount,nIndexCur,nIndexFind);//��ͬ����
//	if(bFindSync == false)
//	{
		//find no sync char
//		nIndexCur = nIndexFind;
//		nCurrentPos = nIndexFind;
//		return false;
//	}
	
	nIndexCur = nIndexFind;
	//CRCУ��

	Q_MODBUS_ASDU *asdu=NULL;
	asdu=(Q_MODBUS_ASDU *)&buff[nIndexCur];
	
	int len=0;	
	len=asdu->header.lenHigh <<8;
	len+=+asdu->header.lenLow;


	//�¹�Լ��ӡ
	SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeRx,(BYTE*)buff,nCount);

	int n_syncFirst=0;

	if(m_PrintSetting!=NULL)
	{
		if(m_PrintSetting->getYcFunCode()==asdu->funcode)
		{
			explainFrameYC(asdu,(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)],len);
		}
		else if(m_PrintSetting->getYxFunCode()==asdu->funcode)
		{
			//��ң��
			explainFrameYX(asdu,(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)],len-2);
		}
		else if(m_PrintSetting->getYkFunCode()==asdu->funcode)
		{
			//ң��
			explainFrameYK(asdu,(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)],len);
		}
		else if(m_PrintSetting->getYtFunCode()==asdu->funcode)
		{
		    //ң��
			if(m_PrintSetting->getYtType() == YtTypeMulti)
				explainFrameYT_M(asdu,(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)],len);
			else
				explainFrameYT(asdu,(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)],len);
		}
		else
		{
			//���Ϸ������룬Ҳ���Գ�Ϊδʶ������
			sendErroCode(asdu,MODBUS_ERROR_CODE_1);
			nIndexCur = nCount;
		}
		nIndexCur=sizeof(Q_MODBUS_HEADER)+len;
	}

	nCurrentPos = nIndexCur;
	return true;
}

bool Q_protocolModbusSlave::findSyncIndex(char* buff,int buffSize,int startPos,int& findPos)
{
	findPos = startPos;
	if(startPos < 0 || startPos >= buffSize || (startPos + MODBUS_FRAMELEN_MIN) > buffSize)
		return false;

	int ret = startPos;
	Q_MODBUS_HEADER *header=NULL;

	int n_syncFirst=0;
	Q_protocolModbusSlaveSetting *aa=(Q_protocolModbusSlaveSetting *)m_pProcotolConfig->getProtocolSetting();

	if(aa!=NULL)
	{
       n_syncFirst=aa->getSyncFirst();
	}
	while((ret + MODBUS_HEADER_LEN) < buffSize)
	{
		header=(Q_MODBUS_HEADER *)&buff[ret];
		if(header!=NULL)
		{
			if(n_syncFirst==header->codeHigh && MODBUS_SYNC_CODE2==header->codeLow
				&&0==header->codeFlgHigh&&0==header->codeFlgLow&&(header->lenLow>0||header->lenHigh>0))
			{
				findPos = ret;
				return true;
			}
			else
			{
				ret += 1;
			}
		}
		else
		{
			break;
		}

	}/// next byte

	findPos = ret;
	return false;
}

//ң��
bool Q_protocolModbusSlave::explainFrameYX(Q_MODBUS_ASDU *header,BYTE *data,const int &len)
{
	TRACE("Q_protocolModbusSlave::explainFrameYX\n");
	int channeltype = m_PrintSetting->getChannelType();

	int nPointEnd = m_PrintSetting->getPointBegin();
	if (nPointEnd == 0)//û��ң�ŵ�
	{
		sendErroCode(header,MODBUS_ERROR_CODE_2);
		return true;
	}

	int nSlaveYXBegin = 0;//Slave��Լ����ʼ���
	int nYXBegin = 0;//YX��ʼ�ţ��±�
	if (m_PrintSetting!=NULL)
	{
		nSlaveYXBegin=m_PrintSetting->getRelayBegin();
	}

	Q_MODBUS_REC_YC *rec=(Q_MODBUS_REC_YC *)data;

	int startAddr=rec->startAddrHigh<<8;
	startAddr+=rec->startAddrLow;

	int oriStartAddr=startAddr;

	int regCount=0;//��ĸ���
	regCount=rec->regCountHigh<<8;
	regCount+=rec->regCountLow;

	//Q_dataManager* p = _GlobalData.getDataManager();

	//int digCount=p->getDigitalCount();


//	nYXBegin = oriStartAddr - nSlaveYXBegin;

	if(regCount/8 + 4 >MODBUS_MAX_PACK_COUNT)
	{
		TRACE("----request data too  big ---\n");
		sendErroCode(header,MODBUS_ERROR_CODE_3);
		return false;
	}

	int syncFirt=m_PrintSetting->getSyncFirst();

	

	Q_MODBUS_ASDU2 asdu;
	memset(&asdu,NULL,sizeof(asdu));
	asdu.header.codeHigh=syncFirt&0xFF;//��Ϣ����趨Ϊ0
	asdu.header.codeLow=MODBUS_SYNC_CODE2&0xFF;
	asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1&0xFF;
	asdu.header.codeFlgLow=MODBUS_SYNC_CODE1&0xFF;
	asdu.addr=header->addr;
	asdu.funcode=header->funcode;

	int  packLen=0;//��Ч��modbus���ĳ��ȣ�������ǰ��TCPͬ��ͷ
	packLen=regCount/8+3;

	if(regCount%8!=0)
		packLen+=1;
		
	asdu.header.lenHigh=(packLen>>8) &0xFF;
	asdu.header.lenLow= packLen&0xFF;

	int pi=0;
	asdu.data[pi++]=(packLen-3)&0xFF;

	Q_DIGITALEDATATAG curs;
	int val=0;
	Mod_DVAL dval;
	memset(&dval,NULL,sizeof(dval));

	if (nYXBegin <0)
	{
		return false;
	}

	//2016-1-7 ����
	CFEPFuncRtdb *m_rtdbInf = m_pMgrModbusSlaveProtocolSetting->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	SCADA_Digit *curDigitBC=NULL;
	SCADA_Digit *zfDigitBC=NULL;
	int nDigitBeginid32 = m_PrintSetting->getPointBegin();

	for(int i=0;i<regCount;i++)
	{
		/*
		Q_RECORDDATATAG2 dataArray;
		dataArray.val.eDataType=eDataTypeBit;
		dataArray.val.channelID=m_PrintSetting->getChannelID();
		dataArray.val.device=header->addr;
		dataArray.val.no = oriStartAddr+i;
		dataArray.val.fValue = 0;

		if (channeltype == 3)//��̬ͨ��
		{
			m_pMgrModbusSlaveProtocolSetting->evalDataValue(&dataArray,1,3,nYXBegin+i+1,2);
		}
		else
		{
			m_pMgrModbusSlaveProtocolSetting->evalDataValue(&dataArray,1);
		}
		*/

		int ninfoaddr = i+nDigitBeginid32;

		if(dbmg_sdb->GetTabDigitByID(ninfoaddr, curDigitBC))
		{
			if(curDigitBC->iZfID>0)
			{
				if(dbmg_sdb->GetTabDigitByID(curDigitBC->iZfID,zfDigitBC))
				{
					curDigitBC->iVal=zfDigitBC->iVal;
					SYSTEMTIME systime;
					GetLocalTime(systime);
					memcpy(&curDigitBC->tSnapTime,&systime,sizeof(curDigitBC->tSnapTime));
				}

			}
		}

		int niRaw = curDigitBC->iVal;

		//curs.bValue = dataArray.val.fValue==1?true:false;
      //  curs.bValue = dataArray.val.nValue>=1?true:false;
         curs.bValue = curDigitBC->iVal>=1?true:false;

		switch(i%8)
		{
		case 0:
			memset(&dval,NULL,sizeof(dval));
			dval.LSB=curs.bValue==true?1:0;
			break;
		case 1:
			dval.v2=curs.bValue==true?1:0;
			break;
		case 2:
			dval.v3=curs.bValue==true?1:0;
			break;
		case 3:
			dval.v4=curs.bValue==true?1:0;
			break;
		case 4:
			dval.v5=curs.bValue==true?1:0;
			break;
		case 5:
			dval.v6=curs.bValue==true?1:0;
			break;
		case 6:
			dval.v7=curs.bValue==true?1:0;
			break;
		case 7:
			dval.MSB=curs.bValue==true?1:0;
			asdu.data[pi++]=(*(BYTE *)&dval)&0xFF;
			break;
		}

		
	}

	if(regCount%8!=0)
	{
		asdu.data[pi++]=(*(BYTE *)&dval)&0xFF;
	}

	return sendData(&asdu,packLen);

	return true;
}

//ң��
bool Q_protocolModbusSlave::explainFrameYC(Q_MODBUS_ASDU *header,BYTE *data,const int &len)
{
	TRACE("Q_protocolModbusSlave::explainFrameYC\n");
	int channeltype = m_PrintSetting->getChannelType();

	int nAnalogEnd = m_PrintSetting->getAnalogEnd();
	if (nAnalogEnd == 0)//û��ң���
	{
		sendErroCode(header,MODBUS_ERROR_CODE_2);
		return true;
	}

	int nSlaveYCBegin = 0;//Slave��Լ����ʼ���
	int nYCBegin = 0;//YC��ʼ�ţ��±�
	if (m_PrintSetting!=NULL)
	{
		nSlaveYCBegin=m_PrintSetting->getRegisterBegin();
	}
	Q_MODBUS_REC_YC *rec=(Q_MODBUS_REC_YC *)data;

	int startAddr=rec->startAddrHigh<<8;
	startAddr+=rec->startAddrLow;

	int oriStartAddr=startAddr;

	startAddr/=2; //�����Ĵ�����һ��ֵ(ÿһ���Ĵ����������ֽ�)
	int regCount=0;
	regCount=rec->regCountHigh<<8;
	regCount+=rec->regCountLow;

	int oriRegCount=regCount;
	//�������
	//�����Ĵ�����һ������ 
	int a = m_PrintSetting->getYcUpType();
	if(m_PrintSetting->getYcUpType()!=YcUpTypeINT2)
	{
         regCount=regCount/2;
	}
	

	int planAddrBegin=0,planAddrEnd=0,planAddrCount=0;


	nYCBegin = (oriStartAddr - nSlaveYCBegin)/2;


	int n_req_cnt=0;
	if(m_PrintSetting->getYcUpType()!=YcUpTypeINT2)
	{
		n_req_cnt=regCount*4+4;
	}
	else
	{
        n_req_cnt=regCount*2+4;
	}

	if(n_req_cnt>MODBUS_MAX_PACK_COUNT)
	{
/*      //ԭCommApp��Լ��ӡ���桢�ȱ�����
		if(m_PrintSetting->getPrintError())
		{
		    memset(m_LastMsg ,NULL,sizeof(m_LastMsg ));
			sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"request data too  big,request count= %d,max count=%d\n",n_req_cnt,MODBUS_MAX_PACK_COUNT);
			writefile(m_LastMsg ,sizeof(m_LastMsg ), 1);
		}
		if (m_bErrMsg)
		{
			memset(m_LastMsg ,NULL,sizeof(m_LastMsg ));
			sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"request data too  big,request count= %d,max count=%d\n",n_req_cnt,MODBUS_MAX_PACK_COUNT);
//			OnErrMsg(m_LastMsg ,sizeof(m_LastMsg ));
		}
*/
		//�¹�Լ��ӡ
		sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"request data too  big,request count= %d,max count=%d\n",n_req_cnt,MODBUS_MAX_PACK_COUNT);
		SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeError,(BYTE*)m_LastMsg ,strlen(m_LastMsg)+1);


		sendErroCode(header,MODBUS_ERROR_CODE_3);
		return false;
	}

	int syncFirt=m_PrintSetting->getSyncFirst();

	Q_MODBUS_ASDU2 asdu;
	memset(&asdu,NULL,sizeof(asdu));
	asdu.header.codeHigh=syncFirt&0xFF;//��Ϣ����趨Ϊ0
	asdu.header.codeLow=MODBUS_SYNC_CODE2&0xFF;
	asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1&0xFF;
	asdu.header.codeFlgLow=MODBUS_SYNC_CODE1&0xFF;
	asdu.addr=header->addr;
	asdu.funcode=header->funcode;
	
	int  packLen=0;
	int pi=0;

	Q_ANALOGUEDATATAG curs;

	//2016-1-7 ����
	CFEPFuncRtdb *m_rtdbInf = m_pMgrModbusSlaveProtocolSetting->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	int nAnalogBeginid32=m_PrintSetting->getAnalogBegin();
	SCADA_Analog *curAnalogAc=NULL;
	SCADA_Analog *zfAnalogAc=NULL;
	//̷��ʤ����end
	
	if(m_PrintSetting->getYcUpType()==YcUpTypeINT2)
	{
		packLen=regCount*2+3;//��Ч��modbus���ĳ��ȣ�������ǰ��TCPͬ��ͷ

		asdu.data[pi++]=(regCount*2)&0xFF;

		//TRACE("startaddr = %d \n",oriStartAddr);
		for(int i=0;i<regCount;i++)
		{
			//TRACE("index = %d \n",nYCBegin+i);
			/*
			Q_RECORDDATATAG2 dataArray;
			dataArray.val.eDataType=eDataTypeInt;
			dataArray.val.channelID=m_PrintSetting->getChannelID();
			dataArray.val.device=header->addr;
			dataArray.val.no = oriStartAddr+i;
			dataArray.val.fValue = 0;

			if (channeltype == 3)//��̬ͨ��
			{
				m_pMgrModbusSlaveProtocolSetting->evalDataValue(&dataArray,1,3,nYCBegin+i+1,1);
			}
			else
			{
				m_pMgrModbusSlaveProtocolSetting->evalDataValue(&dataArray,1);
			}
			
			curs.fValue = dataArray.val.fValue;
			curs.factor = 1;
			*/
			//�õ��ļ�¼ID��
			int nVID = i+nAnalogBeginid32;

		/*	Q_RECORDDATATAG2 dataArray;
			dataArray.val.eDataType=eDataTypeFloat;
			dataArray.val.channelID=m_ProSetting->getChannelID();
			dataArray.val.device=msg->header.commom_asdu_1;
			dataArray.val.no = ninfoaddr;
			dataArray.val.fValue = 0;*/
			float fval=0;
			if(dbmg_sdb->GetTabAnalogByID(nVID,curAnalogAc))
			{
				if(curAnalogAc==NULL)
					continue;

				if(curAnalogAc->iZfID>0)
				{
					if(dbmg_sdb->GetTabAnalogByID(curAnalogAc->iZfID,zfAnalogAc))
					{
						fval=zfAnalogAc->fVal*curAnalogAc->fCoeff;
						curAnalogAc->fVal=fval;
					}
				}
				else
				{
						fval=curAnalogAc->fVal*curAnalogAc->fCoeff;
				}
				GetLocalTime(curAnalogAc->tSnapTime);
			}
			

			//̷��ʤ����end
			BYTE tmp[2];
			memset(tmp,NULL,sizeof(tmp));

			//short s_n=(short)(curs.fValue*curs.factor);//ֻ��˫�ֽ����Ͷ�ϵ��
			short s_n=(short)(fval);//ֻ��˫�ֽ����Ͷ�ϵ��
			memcpy(tmp,&s_n,sizeof(s_n));

			//��ʱ���ĵ���Ϊ�Զ˲�֧�ֱ���ֽ���
			switch(m_FloatUpSeq)
			{
			case eFloatUpSeqLowFirst:
				asdu.data[pi++]=tmp[0];
				asdu.data[pi++]=tmp[1];
				break;
			case eFloatUpSeqHighFirst:
				asdu.data[pi++]=tmp[1];
				asdu.data[pi++]=tmp[0];
				break;
			}		
		}
	}
	else if(m_PrintSetting->getYcUpType()==YcUpTypeFloat)
	{
		packLen=regCount*4+3;//��Ч��modbus���ĳ��ȣ�������ǰ��TCPͬ��ͷ
		asdu.data[pi++]=(regCount*4)&0xFF;

		//TRACE("startaddr = %d \n",oriStartAddr);
		int val=0;
		for(int i=0;i<regCount;i++)
		{
			Q_RECORDDATATAG2 dataArray;
			dataArray.val.eDataType=eDataTypeFloat;
			dataArray.val.channelID=m_PrintSetting->getChannelID();
			dataArray.val.device=header->addr;
			dataArray.val.no = oriStartAddr+2*i;;
			dataArray.val.fValue = 0;

			//TRACE("Q_protocolModbusSlave::explainFrameYC getval channelID = %d,device = %d,index = %d \n",
				//dataArray.val.channelID,dataArray.val.device,dataArray.val.no);

			if (channeltype == 3)//��̬ͨ��
			{
				m_pMgrModbusSlaveProtocolSetting->evalDataValue(&dataArray,1,3,nYCBegin+i+1,1);
			}
			else
			{
				m_pMgrModbusSlaveProtocolSetting->evalDataValue(&dataArray,1);
			}

			curs.fValue = dataArray.val.fValue;
			curs.factor = 1;		

			BYTE tmp[4];
			memset(tmp,NULL,sizeof(tmp));

			memcpy(tmp,&curs.fValue,sizeof(curs.fValue));

			//��ʱ���ĵ���Ϊ�Զ˲�֧�ֱ���ֽ���
			switch(m_FloatUpSeq)
			{
			case eFloatUpSeqLowFirst:
				asdu.data[pi++]=tmp[3];
				asdu.data[pi++]=tmp[2];
				asdu.data[pi++]=tmp[1];
				asdu.data[pi++]=tmp[0];
				break;
			case eFloatUpSeq2301:
				asdu.data[pi++]=tmp[2];
				asdu.data[pi++]=tmp[3];
				asdu.data[pi++]=tmp[0];
				asdu.data[pi++]=tmp[1];
				break;
			case eFloatUpSeq1032:
				asdu.data[pi++]=tmp[1];
				asdu.data[pi++]=tmp[0];
				asdu.data[pi++]=tmp[3];
				asdu.data[pi++]=tmp[2];
				break;	
			case eFloatUpSeqHighFirst:
				asdu.data[pi++]=tmp[0];
				asdu.data[pi++]=tmp[1];
				asdu.data[pi++]=tmp[2];
				asdu.data[pi++]=tmp[3];
				break;
			}		
		}
	}
	else 
	{

		packLen=regCount*4+3;//��Ч��modbus���ĳ��ȣ�������ǰ��TCPͬ��ͷ
		asdu.data[pi++]=(regCount*4)&0xFF;

		//TRACE("startaddr = %d \n",oriStartAddr);
		int val=0;
		for(int i=0;i<regCount;i++)
		{
			//TRACE("index = %d \n",nYCBegin+i);

			Q_RECORDDATATAG2 dataArray;
			dataArray.val.eDataType=eDataTypeFloat;
			dataArray.val.channelID=m_PrintSetting->getChannelID();
			dataArray.val.device=header->addr;
			dataArray.val.no = oriStartAddr+2*i;;
			dataArray.val.fValue = 0;

			if (channeltype == 3)//��̬ͨ��
			{
				m_pMgrModbusSlaveProtocolSetting->evalDataValue(&dataArray,1,3,nYCBegin+i+1,1);
			}
			else
			{
				m_pMgrModbusSlaveProtocolSetting->evalDataValue(&dataArray,1);
			}

			curs.fValue = dataArray.val.fValue;
			curs.factor = 1;


			BYTE tmp[4];
			memset(tmp,NULL,sizeof(tmp));
			val=(int)curs.fValue;

			memcpy(tmp,&val,sizeof(val));

			//��ʱ���ĵ���Ϊ�Զ˲�֧�ֱ���ֽ���
			switch(m_FloatUpSeq)
			{
			case eFloatUpSeqLowFirst:
				asdu.data[pi++]=tmp[3];
				asdu.data[pi++]=tmp[2];
				asdu.data[pi++]=tmp[1];
				asdu.data[pi++]=tmp[0];
				break;
			case eFloatUpSeqHighFirst:
				asdu.data[pi++]=tmp[0];
				asdu.data[pi++]=tmp[1];
				asdu.data[pi++]=tmp[2];
				asdu.data[pi++]=tmp[3];
				break;
			}		
		}
	}
	asdu.header.lenHigh=(packLen>>8) &0xFF;
	asdu.header.lenLow= packLen&0xFF;

	return sendData(&asdu,packLen);
}


//ң��
bool Q_protocolModbusSlave::explainFrameYK(Q_MODBUS_ASDU *header,BYTE *data,const int &len)
{
	TRACE("Q_protocolModbusSlave::explainFrameYK\n");
	int startAddr=data[0]<<8;
	startAddr+=data[1];

	//��˶��д20150728
	//�ж�ң�ص��Ƿ����
	int nPos = 0;
	bool bExist = false;
	int YKtoYXID32 = -1;
	int	YKID32 = -1;	

	Q_RECORDDATATAG dataArray;

	SCADA_Relay curRelay;
	CFEPFuncRtdb *m_rtdbInf = m_pMgrModbusSlaveProtocolSetting->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	int nRelayRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_Relay_ID );
	for (int j=0; j<nRelayRecordCount; j++)
	{
		int nTempid32 = j+1;
		dbmg_sdb->GetElementByID(SCADA_TAB_Relay_ID, nTempid32,&curRelay);

		if (m_PrintSetting->getChannelID() == curRelay.uTuID && header->addr == curRelay.lDPUID &&
			startAddr == curRelay.iNoInDPU)
		{
			bExist = true;
			YKID32 = nTempid32;
			YKtoYXID32 = curRelay.ulYXID32;
			break;
		}
	}

	int nYXbegin = m_PrintSetting->getPointBegin();
	int nYXend = m_PrintSetting->getPointEnd();

//	if (YKtoYXID32 >= nYXbegin && YKtoYXID32 <= nYXend)//��Ӧң�ŵ����д����ֵ
//	{
//		bExist = false;
//	}

	if (YKtoYXID32 >=0)//��Ӧң�ŵ����д����ֵ
	{
		bExist = true;
	}

	if (!bExist)
	{
		if (NULL!=m_PrintSetting)
		{
			sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"�յ�ң�������ң�ص����ò���ȷ���Ӧyxindex����ȷ��Channelid=%d,device=%d,no=%d\n",m_PrintSetting->getChannelID(),header->addr,startAddr);
			TRACE(m_LastMsg );
		}
		if(true)//m_bInfoMsg
		{
			sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"�յ�ң�������ң�ص����ò���ȷ���Ӧyxindex����ȷ��Channelid=%d,device=%d,no=%d\n",m_PrintSetting->getChannelID(),header->addr,startAddr);
			TRACE(m_LastMsg );
		}
		TRACE("----address did not exist---\n");
		sendErroCode(header,MODBUS_ERROR_CODE_2);
		return false;
	}
	else
	{

	}


	bool setStatus=true;
	if(m_PrintSetting!=NULL)
	{
		int aa=data[2]<<8|data[3];
		if(m_PrintSetting->getYkOnValue()==aa)
		{
			setStatus=true;
		}
		else if(m_PrintSetting->getYkOffValue()==aa)
		{
			setStatus=false;
		}
	}
	else
	{
		TRACE("-----status error ----");
		sendErroCode(header,MODBUS_ERROR_CODE_4);
		return false;
	}


	int syncFirt=m_PrintSetting->getSyncFirst();

	Q_MODBUS_ASDU2 asdu;
	memset(&asdu,NULL,sizeof(asdu));
	asdu.header.codeHigh=syncFirt&0xFF;//��Ϣ����趨Ϊ0
	asdu.header.codeLow=MODBUS_SYNC_CODE2;
	asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
	asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
	asdu.addr=header->addr;
	asdu.funcode=header->funcode;

	int packLen=6;

	asdu.header.lenHigh=(packLen>>8) &0xFF;
	asdu.header.lenLow= packLen&0xFF;
	//������״̬����
	memcpy(&asdu.data[0],data,sizeof(char)*4);

	SCADA_Digit tempDigit;
	dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID,YKtoYXID32,&tempDigit);

	if (&tempDigit != NULL)
	{
		//2016-02-23�޸�Ϊֱ��ʹ����������
		dataArray.eDataType=eDataTypeBit;
		dataArray.channelID=tempDigit.uTuID;
		dataArray.nIndex=tempDigit.iIndex;
		dataArray.bValue=setStatus;
		m_pMgrModbusSlaveProtocolSetting->recordDataValue(&dataArray,1,true);
	}

	SYSTEMTIME time;
	GetLocalTime(time);
	curRelay.selecttime = time;
	curRelay.exectime = time;

	if(setStatus==1)
	{
		curRelay.LogicalClose = 1;
		curRelay.LogicalOpen = 0;
	}
	else
	{
		curRelay.LogicalClose = 0;
		curRelay.LogicalOpen = 1;
	}

	dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ʱ��


	if(curRelay.iZF)
	{
		//tjs add  2015-07-30
		//������Ϣ����
		RControl rc;
		rc.iStation =curRelay.izfTuID;
		rc.iYKPoint =curRelay.izfNoInDPU;
		rc.iCommandType = mes_RCExecuteOk;
		rc.iVal = dataArray.bValue;
		rc.iDevice=curRelay.izfDPUID;
		rc.iZf=1;
		rc.iYxID=curRelay.ulYXID32;
		m_rtdbInf->getMainDb()->SendMessage(curRelay.izfProcessID, mes_RCExecute, sizeof(RControl), (char *)&rc);
	} 

	/*ԭ����ת�����ܲ��ִ���
	if(m_wnd!=NULL)
	{
		m_SigArray.channelid=m_PrintSetting->getChannelID();
		m_SigArray.funcode=0;
		m_SigArray.m_vindex=nYXindex;
		m_SigArray.bzf = true;
		m_SigArray.zfchannelid = pYKData[nYKindex].zfchannelid;
		m_SigArray.zfdevice = pYKData[nYKindex].zfdevice;
		m_SigArray.zfno = pYKData[nYKindex].zfno;
		m_SigArray.varType=eModVarTypeYx;
		m_SigArray.m_tag.eDataType=eDataTypeInt;
		m_SigArray.m_tag.bValue=pYKData[nYKindex].bValue;
	}
   */
   //ת����Ĺ����Ժ�Ҫ�����

   return sendData(&asdu,packLen);
}


//ң��
bool Q_protocolModbusSlave::explainFrameYT(Q_MODBUS_ASDU *header,BYTE *data,const int &len)
{
	TRACE("Q_protocolModbusSlave::explainFrameYT\n");
	int startAddr=data[0]<<8;
	startAddr+=data[1];

	//�Ժ��ڴ�λ�ü��� ����Ҫ���������豸��ַ����Ԫ��ַ�ж�ң�����Ƿ����

	//��˶��д20150729
	//�ж�ң�����Ƿ����
	int nPos = 0;
	bool bExist = false;
	int YTtoYCID32 = -1;
	int	YTID32 = -1;	

	Q_RECORDDATATAG dataArray;
	SCADA_SetPoint curSetPoint;
	CFEPFuncRtdb *m_rtdbInf = m_pMgrModbusSlaveProtocolSetting->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	int nSetPointRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_SetPoint_ID );
	for (int j=0; j<nSetPointRecordCount; j++)
	{
		int nTempid32 = j+1;
		dbmg_sdb->GetElementByID(SCADA_TAB_SetPoint_ID, nTempid32,&curSetPoint);

		if (m_PrintSetting->getChannelID() == curSetPoint.uTuID && header->addr == curSetPoint.lDPUID &&
			startAddr == curSetPoint.iNoInDPU)
		{
			bExist = true;
			YTID32 = nTempid32;
			YTtoYCID32 = curSetPoint.ulYCID32;
			break;
		}
	}

	int nYCbegin = m_PrintSetting->getAnalogBegin();
	int nYCend = m_PrintSetting->getAnalogEnd();


	if (YTtoYCID32 >= 0)
	{
		bExist = true;
	}

	int syncFirt=m_PrintSetting->getSyncFirst();
	Q_MODBUS_ASDU2 asdu;
	memset(&asdu,NULL,sizeof(asdu));
	asdu.header.codeHigh=syncFirt&0xFF;//��Ϣ����趨Ϊ0
	asdu.header.codeLow=MODBUS_SYNC_CODE2;
	asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
	asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
	asdu.addr=header->addr;
	asdu.funcode=header->funcode;

	int  packLen=len;

	asdu.header.lenHigh=(packLen>>8) &0xFF;
	asdu.header.lenLow= packLen&0xFF;

	memcpy(&asdu.data[0],data,sizeof(char)*packLen);


	if (!bExist)
	{
		sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"�յ�ң�������ң�������ò���ȷ���Ӧycindex����ȷ��Channelid=%d,device=%d,no=%d\n",m_PrintSetting->getChannelID(),header->addr,startAddr);
		TRACE(m_LastMsg );
		TRACE("----address did not exist---\n");
		sendErroCode(header,MODBUS_ERROR_CODE_2);
		return false;
	}
	else
	{
		if(len == 8)
		{	
			float val=0.0f;
			int nval = 0;
			//���ֽڸ�����
			
			BYTE tmp[4];
			memset(tmp,NULL,sizeof(tmp));

			//�����Ĵ����ڲ�����λ�ȴ���λ��
			switch(m_YtByteSeq)
			{
			case eFloatUpSeqLowFirst:
				tmp[0]=data[5];  //2 3 0 1 
				tmp[1]=data[4];
				tmp[2]=data[3];
				tmp[3]=data[2];
				break;
			case eFloatUpSeq2301:
				tmp[0]=data[4];
				tmp[1]=data[5];
				tmp[2]=data[2];
				tmp[3]=data[3];
				break;
			case eFloatUpSeq1032:
				tmp[0]=data[3];
				tmp[1]=data[2];
				tmp[2]=data[5];
				tmp[3]=data[4];
				break;	
			case eFloatUpSeqHighFirst:
				tmp[0]=data[2];  //2 3 0 1 
				tmp[1]=data[3];
				tmp[2]=data[4];
				tmp[3]=data[5];
				break;
			}


			if (m_PrintSetting->getYtDataType() == YtDataTypeFloat)
			{
				memcpy(&val,tmp,sizeof(val));
			} 
			else
			{
				memcpy(&nval,tmp,sizeof(nval));
				val = (float)nval;
			}

			val = val*curSetPoint.fFactor;  //Modbus��վң��ϵ������

			SCADA_Analog tempAnalog;
			dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);

			if (&tempAnalog != NULL)
			{
				//2016-02-23�޸�Ϊֱ��ʹ����������
				dataArray.eDataType=eDataTypeFloat;
				dataArray.channelID=tempAnalog.uTuID;
				dataArray.nIndex = tempAnalog.iIndex;
				dataArray.fValue=val/tempAnalog.fCoeff;//fs250150824 recordDataValueд�����ԭʼֵraw��ң�����õ��ǹ���ֵ������Ҫ����ϵ����ԭ
				m_pMgrModbusSlaveProtocolSetting->recordDataValue(&dataArray,1,true);//record����ˢ��ʱ���

			}

			if(m_wnd!=NULL)
			{
				m_SigArray.funcode=0;
				m_SigArray.m_vindex=YTtoYCID32;
				m_SigArray.varType=eModVarTypeYc;
				m_SigArray.m_tag.eDataType=eDataTypeFloat;
				m_SigArray.m_tag.fValue=float(nval);
				//::SendMessage(m_wnd,m_nCmdID,(WPARAM)&m_SigArray,(LPARAM)sizeof(m_SigArray));
			}

			sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"�յ�ң������豸��ַ=%d,��Ϣ���ַ=%d,ֵ=%.3f",header->addr,startAddr,val);
			TRACE(m_LastMsg );


			SYSTEMTIME time;
			GetLocalTime(time);
			curSetPoint.selecttime = time;
			curSetPoint.exectime = time;
			curSetPoint.iexecret = 1;
			curSetPoint.fVal = val;
			dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ʱ��

			if(curSetPoint.iZF)//ת������
			{
				RSetpoint rs;
				rs.iStation =curSetPoint.izfTuID;
				rs.iPoint =curSetPoint.izfNoInDPU;
				rs.iCommandType =MES_TYPE_SPOINT;//ң�������룬��ʱ����
				rs.fVal = val;
				rs.iDevice=curSetPoint.izfDPUID;
				rs.iZf=1;
				rs.iYcID=curSetPoint.ulYCID32;
				m_rtdbInf->getMainDb()->SendMessage(curSetPoint.izfProcessID, mes_SPExecute, sizeof(RSetpoint), (char *)&rs);
			}
		}
		else if (len == 6)
		{
			float val=0.0f;
			int nval = 0;
			BYTE tmp[2];
			memset(tmp,NULL,sizeof(tmp));
			switch(m_YtByteSeq)
			{
			case eFloatUpSeqLowFirst:
				tmp[0]=data[3]; 
				tmp[1]=data[2];
				break;	
			case eFloatUpSeqHighFirst:
				tmp[0]=data[2];  
				tmp[1]=data[3];
				break;
			}
			nval = tmp[0]*256 + tmp[1];
			val = (float)nval;
			val = val*curSetPoint.fFactor;  //Modbus��վң��ϵ������

			SCADA_Analog tempAnalog;
			dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);

			if (&tempAnalog != NULL)
			{
				//2016-02-23�޸�Ϊֱ��ʹ����������
				dataArray.eDataType=eDataTypeInt;
				dataArray.channelID=tempAnalog.uTuID;
				dataArray.nIndex = tempAnalog.iIndex;
				dataArray.fValue=val/tempAnalog.fCoeff;//fs250150824 recordDataValueд�����ԭʼֵraw��ң�����õ��ǹ���ֵ������Ҫ����ϵ����ԭ
				m_pMgrModbusSlaveProtocolSetting->recordDataValue(&dataArray,1,true);//record����ˢ��ʱ���

			}

			sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"�յ�ң������豸��ַ=%d,��Ϣ���ַ=%d,ֵ=%.3f",header->addr,startAddr,val);
			TRACE(m_LastMsg );
		
			SYSTEMTIME time;
			GetLocalTime(time);
			curSetPoint.selecttime = time;
			curSetPoint.exectime = time;
			curSetPoint.iexecret = 1;
			curSetPoint.fVal = float(nval);
			dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ʱ��

			if(curSetPoint.iZF)//ת������
			{
				RSetpoint rs;
				rs.iStation =curSetPoint.izfTuID;
				rs.iPoint =curSetPoint.izfNoInDPU;
				rs.iCommandType =MES_TYPE_SPOINT;//ң�������룬��ʱ����
				rs.fVal = val;
				rs.iDevice=curSetPoint.izfDPUID;
				rs.iZf=1;
				rs.iYcID=curSetPoint.ulYCID32;
				m_rtdbInf->getMainDb()->SendMessage(curSetPoint.izfProcessID, mes_SPExecute, sizeof(RSetpoint), (char *)&rs);
			}
		}
		else
		{
			int nVal=asdu.data[2]<<8;
			nVal+=asdu.data[3]&0xFF;

			nVal = nVal*curSetPoint.fFactor==1?true:false;  //Modbus��վң��ϵ������

			SCADA_Analog tempAnalog;
			dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);

			if (&tempAnalog != NULL)
			{
				//2016-02-23�޸�Ϊֱ��ʹ����������
				dataArray.eDataType=eDataTypeInt;
				dataArray.channelID=tempAnalog.uTuID;
				dataArray.nIndex = tempAnalog.iIndex;
				dataArray.fValue=nVal/tempAnalog.fCoeff;//fs250150824 recordDataValueд�����ԭʼֵraw��ң�����õ��ǹ���ֵ������Ҫ����ϵ����ԭ
				m_pMgrModbusSlaveProtocolSetting->recordDataValue(&dataArray,1,true);//record����ˢ��ʱ���
			}

			if(curSetPoint.iZF)//ת��
			{
				RSetpoint rs;
				rs.iStation =curSetPoint.izfTuID;
				rs.iPoint =curSetPoint.izfNoInDPU;
				rs.iCommandType =MES_TYPE_SPOINT;//ң�������룬��ʱ����
				rs.fVal = float(nVal);
				rs.iDevice=curSetPoint.izfDPUID;
				rs.iZf=1;
				rs.iYcID=curSetPoint.ulYCID32;
				m_rtdbInf->getMainDb()->SendMessage(curSetPoint.izfProcessID, mes_SPExecute, sizeof(RSetpoint), (char *)&rs);
		    }
	    }	
	}
	return sendData(&asdu,packLen);
}

//ң��
bool Q_protocolModbusSlave::explainFrameYT_M(Q_MODBUS_ASDU *header,BYTE *data,const int &len)
{
	TRACE("Q_protocolModbusSlave::explainFrameYT_M\n");
	int startAddr=data[0]<<8;
	startAddr+=data[1];

	//��˶��д20150729
	//�ж�ң�����Ƿ����
	int nPos = 0;
	bool bExist = false;
	int YTtoYCID32 = -1;
	int	YTID32 = -1;	

	Q_RECORDDATATAG dataArray;
	SCADA_SetPoint curSetPoint;
	CFEPFuncRtdb *m_rtdbInf = m_pMgrModbusSlaveProtocolSetting->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	int nSetPointRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_SetPoint_ID );
	for (int j=0; j<nSetPointRecordCount; j++)
	{
		int nTempid32 = j+1;
		dbmg_sdb->GetElementByID(SCADA_TAB_SetPoint_ID, nTempid32,&curSetPoint);

		if (m_PrintSetting->getChannelID() == curSetPoint.uTuID && header->addr == curSetPoint.lDPUID &&
			startAddr == curSetPoint.iNoInDPU)
		{
			bExist = true;
			YTID32 = nTempid32;
			YTtoYCID32 = curSetPoint.ulYCID32;
			break;
		}
	}

	int nYCbegin = m_PrintSetting->getAnalogBegin();
	int nYCend = m_PrintSetting->getAnalogEnd();

//	if (YTtoYCID32 >= nYCbegin && YTtoYCID32 <= nYCend)//��Ӧң������д����ֵ
//	{
//		bExist = false;
//	}

	if (YTtoYCID32 >= 0)
	{
		bExist = true;
	}

	if (!bExist)
	{
		if (NULL!=m_PrintSetting)
		{
			sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"�յ�ң�������ң�������ò���ȷ���Ӧycindex����ȷ��Channelid=%d,device=%d,no=%d\n",m_PrintSetting->getChannelID(),header->addr,startAddr);
			TRACE(m_LastMsg );
		}
		TRACE("----address did not exist---\n");
		sendErroCode(header,MODBUS_ERROR_CODE_2);
		return false;
	}
	else
	{

	}

	int syncFirt=m_PrintSetting->getSyncFirst();
	Q_MODBUS_ASDU2 asdu;
	memset(&asdu,NULL,sizeof(asdu));
	asdu.header.codeHigh=syncFirt&0xFF;//��Ϣ����趨Ϊ0
	asdu.header.codeLow=MODBUS_SYNC_CODE2;
	asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
	asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
	asdu.addr=header->addr;
	asdu.funcode=header->funcode;

	int regCount=data[2]<<8;
	regCount+=data[3];//�Ĵ�������
 
	
	int nByteCount=data[4];
	int packLen=len;

	asdu.header.lenHigh=(packLen>>8) &0xFF;
	asdu.header.lenLow= packLen&0xFF;

	memcpy(&asdu.data[0],data,nByteCount+5);


//���ֽڸ�����
	float val=0.0f;
	int nval = 0;


/*update by tjs 2015.04.17 begin*/
	if (m_PrintSetting->getYtDataType() == YtDataTypeINT2)//���ֽ�����
	{
		BYTE tmp[2];
		for(int i=0;i<nByteCount;i=i+2,YTtoYCID32++)
		{
			memset(tmp,NULL,sizeof(tmp));
			switch(m_YtByteSeq)
			{
			case eFloatUpSeqLowFirst:
				tmp[0] = data[i+6];
				tmp[1] = data[i+5];
				break;
			case eFloatUpSeqHighFirst:
				tmp[0] = data[i+5];
				tmp[1] = data[i+6];
				break;
			}
			nval = tmp[0]*256 + tmp[1];
			val = (float)nval;
			val = val*curSetPoint.fFactor;  //Modbus��վң��ϵ������


			SCADA_Analog tempAnalog;
			dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);

			if (&tempAnalog != NULL)
			{
				//2016-02-23�޸�Ϊֱ��ʹ����������
				dataArray.eDataType=eDataTypeFloat;
				dataArray.channelID=tempAnalog.uTuID;
				dataArray.nIndex = tempAnalog.iIndex;
				dataArray.fValue=val/tempAnalog.fCoeff;//fs250150824 recordDataValueд�����ԭʼֵraw��ң�����õ��ǹ���ֵ������Ҫ����ϵ����ԭ
				m_pMgrModbusSlaveProtocolSetting->recordDataValue(&dataArray,1,true);//record����ˢ��ʱ���

			}

			if (NULL!=m_PrintSetting)
			{
				sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"�յ�ң������豸��ַ=%d,��Ϣ���ַ=%d,ֵ=%.3f",header->addr,startAddr+(i/2),val);
				TRACE("m_LastMsg ");
			}

			SYSTEMTIME time;
			GetLocalTime(time);
			dbmg_sdb->GetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);
			curSetPoint.selecttime = time;
			curSetPoint.exectime = time;
			curSetPoint.iexecret = 1;
			curSetPoint.fVal = val;
			dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ʱ��

			if(curSetPoint.iZF)//ת��
			{
				RSetpoint rs;
				rs.iStation =curSetPoint.izfTuID;
				rs.iPoint =curSetPoint.izfNoInDPU;
				rs.iCommandType =MES_TYPE_SPOINT;//ң�������룬��ʱ����
				rs.fVal = val;
				rs.iDevice=curSetPoint.izfDPUID;
				rs.iZf=1;
				//2016-05-18
				rs.iYcID=curSetPoint.ulYCID32;
				//����ת����ң����,����ת�����㷨���̵��޷�����

				m_rtdbInf->getMainDb()->SendMessage(curSetPoint.izfProcessID, mes_SPExecute, sizeof(RSetpoint), (char *)&rs);
			}

			YTID32++;
		}
	}
	else
	{
		/*update by tjs 2015.04.17 end*/
		BYTE tmp[4];
		/*for(int i=0;i<nByteCount;i=i+4)*/
		/*update  byt tjs  2015.04.17**/
		for(int i=0;i<nByteCount;i=i+4,YTtoYCID32++)
		{
			memset(tmp,NULL,sizeof(tmp));
			//�����Ĵ����ڲ�����λ�ȴ���λ��
			switch(m_YtByteSeq)
			{
			case eFloatUpSeqLowFirst:
				tmp[0]=data[i+8];  //2 3 0 1 
				tmp[1]=data[i+7];
				tmp[2]=data[i+6];
				tmp[3]=data[i+5];
				break;
			case eFloatUpSeq2301:
				tmp[0]=data[i+7];
				tmp[1]=data[i+8];
				tmp[2]=data[i+5];
				tmp[3]=data[i+6];
				break;
			case eFloatUpSeq1032:
				tmp[0]=data[i+6];
				tmp[1]=data[i+5];
				tmp[2]=data[i+8];
				tmp[3]=data[i+7];
				break;	
			case eFloatUpSeqHighFirst:
				tmp[0]=data[i+5];  //2 3 0 1 
				tmp[1]=data[i+6];
				tmp[2]=data[i+7];
				tmp[3]=data[i+8];
				break;
			}


			if (m_PrintSetting->getYtDataType() == YtDataTypeFloat)
			{
				memcpy(&val,tmp,sizeof(val));
			} 
			else
			{
				memcpy(&nval,tmp,sizeof(nval));
				val = (float)nval;
			}

			val = val*curSetPoint.fFactor;  //Modbus��վң��ϵ������

			SCADA_Analog tempAnalog;
			dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);

			if (&tempAnalog != NULL)
			{
				//2016-02-23�޸�Ϊֱ��ʹ����������
				dataArray.eDataType=eDataTypeFloat;
				dataArray.channelID=tempAnalog.uTuID;
				dataArray.nIndex = tempAnalog.iIndex;
				dataArray.fValue=val/tempAnalog.fCoeff;//fs250150824 recordDataValueд�����ԭʼֵraw��ң�����õ��ǹ���ֵ������Ҫ����ϵ����ԭ
				m_pMgrModbusSlaveProtocolSetting->recordDataValue(&dataArray,1,true);//record����ˢ��ʱ���
			}

			if (NULL!=m_PrintSetting)
			{
				sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"�յ�ң������豸��ַ=%d,��Ϣ���ַ=%d,ֵ=%.3f",header->addr,startAddr+(i/4),val);
				TRACE(m_LastMsg );
			}

			SYSTEMTIME time;
			GetLocalTime(time);
			dbmg_sdb->GetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);
			curSetPoint.selecttime = time;
			curSetPoint.exectime = time;
			curSetPoint.iexecret = 1;
			curSetPoint.fVal = val;
			dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ʱ��

			if(curSetPoint.iZF)//ת������
			{
				RSetpoint rs;
				rs.iStation =curSetPoint.izfTuID;
				rs.iPoint =curSetPoint.izfNoInDPU;
				rs.iCommandType =MES_TYPE_SPOINT;//ң�������룬��ʱ����
				rs.fVal = val;
				rs.iDevice=curSetPoint.izfDPUID;
				rs.iZf=1;
				rs.iYcID=curSetPoint.ulYCID32;
				m_rtdbInf->getMainDb()->SendMessage(curSetPoint.izfProcessID, mes_SPExecute, sizeof(RSetpoint), (char *)&rs);
			}

			YTID32++;
		}
	}

	return sendData(&asdu,packLen);
}

bool Q_protocolModbusSlave::sendErroCode(Q_MODBUS_ASDU *header,BYTE code)
{

	Q_MODBUS_ASDU2 asdu;

	int syncFirt=m_PrintSetting->getSyncFirst();

	memset(&asdu,NULL,sizeof(asdu));
	asdu.header.codeHigh=syncFirt&0xFF;//��Ϣ����趨Ϊ0
	asdu.header.codeLow=MODBUS_SYNC_CODE2;
	asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
	asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
	asdu.addr=header->addr;
	
	asdu.header.lenLow=0x03;
	asdu.header.lenHigh=0x00;
	
	
	asdu.funcode=header->funcode|0x80;
	asdu.data[0]=code;

	return sendData(&asdu,3);

}

bool Q_protocolModbusSlave::sendData(Q_MODBUS_ASDU2 *asdu,int len)
{
	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		if(m_PrintSetting->getSendClearBuffer())
		    pSendTool->clearBuffer();

		bool ret=pSendTool->writeData((char*)asdu,sizeof(asdu->header)+len);

		if(false==ret)
		{
			if(m_PrintSetting!=NULL)
			{
				//�¹�Լ��ӡ
				memset(m_LastMsg ,NULL,sizeof(m_LastMsg ));
				sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"write buffer failed,current size = %d,and max size =%d!\n",pSendTool->getDataLength(),pSendTool->getMaxSize());
				SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeError,(BYTE*)m_LastMsg ,strlen(m_LastMsg)+1);
			}
		}
		else
		{
			//�¹�Լ��ӡ
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(BYTE*)asdu,sizeof(asdu->header)+len);
		}

		return ret;
	}
	return false;

}

bool Q_protocolModbusSlave::makeFrame()
{
	return true;
}

bool Q_protocolModbusSlave::terminateProtocol()
{	
	/*Q_protocolCdtStandardSetting* pSetting = (Q_protocolCdtStandardSetting*)m_pProcotolConfig->getProtocolSetting();
	pSetting->getTagsManager()->terminateTags();*/
	//��Ϊ���ֵ�����Ͽͻ���ʱ����û���ͷţ����ǾͲ������ˡ�  
    /*
    Q_dataManager* p = _GlobalData.getDataManager();
	p->LeaveMutex();
	TRACE(" --- terminate leave mutex ---\n");
    */

	m_PrintSetting=NULL;
	return true;
}

bool Q_protocolModbusSlave::startProcess()
{
	printf("Q_protocolModbusSlave::startProcess()\n");
	m_eProtocolStatus = eCommonStatusNormal;
	m_yc_idx=0;

	//�¹�Լ��ӡ
	sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"��ʹ����ʼ...");
	SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg ,strlen(m_LastMsg)+1);

	printf("Q_protocolModbusSlave::startProcess() eee\n");

	return true;
}


bool Q_protocolModbusSlave::stopProcess()
{
/*  //ԭCommApp��Լ��ӡ���桢�ȱ�����
	if(NULL!=m_PrintSetting &&m_PrintSetting->getPrintPrompt())
	{
		sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"�Ͽ�����...");
		writefile(m_LastMsg ,sizeof(m_LastMsg ),0);
	}
	if(m_bNomalInfoMsg)
	{
		sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"�Ͽ�����...");
//		OnNomalInfoMsg(m_LastMsg ,sizeof(m_LastMsg ));
	}
*/

	//�¹�Լ��ӡ
	sprintf_s(m_LastMsg ,sizeof(m_LastMsg ),"�Ͽ�����...");
	SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg ,strlen(m_LastMsg)+1);


// 	Q_dataManager* p = _GlobalData.getDataManager();
// 	LPQ_ANALOGUEDATATAG pYC = p->getAnalogData();
// 	LPQ_DIGITALEDATATAG pYX = p->getDigitalData();
// 	if (NULL!=pYC && m_PrintSetting->getAnalogEnd() < p->getAnalogCount())
// 	{
// 		for (int i = m_PrintSetting->getAnalogBegin();i<m_PrintSetting->getAnalogEnd();i++)
// 		{
// 			pYC[i].quality = 2;
// 		}
// 	}
// 
// 	if (NULL!=pYX && m_PrintSetting->getPointEnd() < p->getDigitalCount())
// 	{
// 		for (int i = m_PrintSetting->getPointBegin();i<m_PrintSetting->getPointEnd();i++)
// 		{
// 			pYX[i].quality = 2;
// 		}
// 	}

	m_eProtocolStatus = eCommonStatusNull;
	return true;
}

void Q_protocolModbusSlave::writefile(char *msg,int len, int type)
{
	if(m_PrintSetting->getWndMsgID()>0)
	{
		Q_COM_LOG_INFO log;
		log.chid=m_PrintSetting->getChannelID();
		if(type==1)
			log.type=eMsgTypeError;
		else if(type==0)
			log.type=eMsgTypeMsg;
		else if(type==2)
			log.type=eMsgTypeRx;
		else if(type==3)
			log.type=eMsgTypeTx;
		log.msg=(char*)msg;
		log.len=len;
		//::SendMessage(m_PrintSetting->getWndHwnd(),m_PrintSetting->getWndMsgID(),(WPARAM)&log,(LPARAM)sizeof(log));
	}
}

bool Q_protocolModbusSlave::readFile(char *v_path,vector<MOD_PLAN_CURVE> &vec)
{
/*	CString strFile;
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	CString curRunPath = CString(buffer);
	curRunPath = curRunPath.Left(curRunPath.ReverseFind('\\'));

	strFile.Format("%s\\data\\%s",curRunPath,v_path);

	TiXmlDocument doc(strFile);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
		TRACE("--------read plan xml failed\n");
		return false;
	}

	TiXmlNode* node = 0;
	TiXmlNode* root = 0;
	TiXmlNode* nodeItem = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	root = doc.FirstChild( "Record" );
	todoElement = root->ToElement();
	assert( todoElement  );

	vec.clear();//2012-5-20 add
	bool ret=false;
	for (nodeItem=todoElement->FirstChild("Field");nodeItem;
		nodeItem=nodeItem->NextSibling("Field"))
	{
		itemElement=nodeItem->ToElement();

		MOD_PLAN_CURVE plan;
		plan.exec=atoi(itemElement->Attribute("se"))==0?true:false;
		plan.id=atoi(itemElement->Attribute("id"));
		plan.val=atoi(itemElement->Attribute("value"));
		vec.push_back(plan);

	}
*/
	return true;
}

void Q_protocolModbusSlave::SetWndInfo(HWND wnd,int MsgID,int CmdID)
{
	m_wnd = wnd;
	m_nMsgID=MsgID;
	m_nCmdID=CmdID;
}


void Q_protocolModbusSlave::SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen)
{
	
	//���ж�һ�½��̵�״̬�����򲻷��͡�

  CFEPFuncRtdb *m_rtdbInf = m_pMgrModbusSlaveProtocolSetting->getRtdbInf();



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
		}
	}

	if (bSend == true)
	{
		//������ж���״̬�ͷ���������Ϣ���߸�������
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
				//2016-10-28 tjs ��Ϊ��Ӧ�ø�����������Ϣ
				

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
			sprintf(strMsg,"MsgPrint:stid = %d msgtype id = %d message = %s",MsgPrint.ichannelid,MsgPrint.msgtype,MsgPrint.msg);
			printf("%s\n",strMsg);*/
			//		TRACE("MsgPrint:stid = %d msgtype id = %d message = %s",MsgPrint.ichannelid,MsgPrint.msgtype,MsgPrint.msg);
		}
	}
}


