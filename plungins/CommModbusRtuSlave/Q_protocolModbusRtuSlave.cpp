#include "StdAfx.h"
#include "Q_protocolModbusRtuSlave.h"
#include "tinyxmlinclude.h"
#include "Q_CommUtility.h"




//----------------------------------------------------------------------------
//*
//*
//* Q_protocolModbusTcpServerStandardCommand class
//*
//*
Q_protocolModbusRtuSlaveCommand::Q_protocolModbusRtuSlaveCommand(void)
{
}

Q_protocolModbusRtuSlaveCommand::~Q_protocolModbusRtuSlaveCommand(void)
{
}





//----------------------------------------------------------------------------
//*
//*
//* Q_protocolModbusTcpServerStandardCommandManager class
//*
//*
Q_protocolModbusRtuSlaveCommandManager::Q_protocolModbusRtuSlaveCommandManager(void)
{
}

Q_protocolModbusRtuSlaveCommandManager::~Q_protocolModbusRtuSlaveCommandManager(void)
{
}

bool Q_protocolModbusRtuSlaveCommandManager::addCommand(Q_protocolCommandBase* pCommand)
{
	return false;
}






//----------------------------------------------------------------------------
//*
//*
//* Q_protocolModbusTcpServerStandardSetting class
//*
//*
Q_protocolModbusRtuSlaveSetting::Q_protocolModbusRtuSlaveSetting(void)
{
	memset(LastError,NULL,sizeof(LastError));
}
Q_protocolModbusRtuSlaveSetting::Q_protocolModbusRtuSlaveSetting(Q_protocolModbusRtuSlaveSetting *other)
{
	strcpy_s(m_sFilePath,other->m_sFilePath);
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

	// 	 m_alg_count=other->m_alg_count;
	// 	 m_alg_begin=other->m_alg_begin;
	// 	 m_alg_end=other->m_alg_end;
	// 	 m_pnt_count=other->m_pnt_count;
	// 	 m_pnt_begin=other->m_pnt_begin;
	// 	 m_pnt_end=other->m_pnt_end;
	m_plan_addr_begin=other->m_plan_addr_begin;
	m_plan_addr_end=other->m_plan_addr_end;
	m_plan_count=other->m_plan_count;
	m_wnd = other-> m_wnd;
	m_nMsgID=other->m_nMsgID;
	m_nCmdID=other->m_nCmdID;
	m_channel_id=other->m_channel_id;
}

Q_protocolModbusRtuSlaveSetting::~Q_protocolModbusRtuSlaveSetting(void)
{
	m_vGroup.clear();
	m_vContent.clear();
}

void Q_protocolModbusRtuSlaveSetting::setFilePath(char *v_path)
{
	strcpy_s(m_sFilePath,v_path);
}
bool Q_protocolModbusRtuSlaveSetting::initSetting()
{
	return init();
}
bool Q_protocolModbusRtuSlaveSetting::init()
{
	TiXmlDocument doc(m_sFilePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
#ifdef _DEBUG
		sprintf_s(LastError, "解析配置文件出错，当前文件:%s，当前行号:%d\n",__FILE__,__LINE__);
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
		sprintf_s(group.name,"%s",itemElement->Attribute("name"));
		group.type=eModXmlDataTypeString;

		m_vGroup.push_back(group);

		for (nodeItem=node->FirstChild("Field");nodeItem;
			nodeItem=nodeItem->NextSibling("Field"))
		{
			itemElement=nodeItem->ToElement();
			ST_ModCfgT content;
			content.group=group.id;

			content.id=atoi(itemElement->Attribute("id"));
			sprintf_s(content.name,"%s",itemElement->Attribute("name"));
			sprintf_s(content.value,"%s",itemElement->Attribute("value"));

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
char *Q_protocolModbusRtuSlaveSetting::getContentID(const int &v_idx,const int &v_grp)
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
void   Q_protocolModbusRtuSlaveSetting::setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_alg_count=nVaildCount;
	m_alg_begin=nAddrBegin;
	m_alg_end=nAddrEnd;
}

void   Q_protocolModbusRtuSlaveSetting::setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_pnt_count=nVaildCount;
	m_pnt_begin=nAddrBegin;
	m_pnt_end=nAddrEnd;
}

int  Q_protocolModbusRtuSlaveSetting::getAnalogBegin()
{
	return m_alg_begin;
}
int  Q_protocolModbusRtuSlaveSetting::getAnalogEnd()
{
	return m_alg_end;
}
int    Q_protocolModbusRtuSlaveSetting::getPointBegin()
{ 
	return m_pnt_begin;
}
int  Q_protocolModbusRtuSlaveSetting::getPointEnd()
{
	return m_pnt_end;
}
int  Q_protocolModbusRtuSlaveSetting::getAnalogCount()
{
	return m_alg_count;
}
int  Q_protocolModbusRtuSlaveSetting::getPointCount()
{
	return m_pnt_count;
}
int Q_protocolModbusRtuSlaveSetting::getRegisterBegin()
{
	int res=0;
	char *tmp=getContentID(3,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
int Q_protocolModbusRtuSlaveSetting::getRelayBegin()
{
	int res=0;
	char *tmp=getContentID(4,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
int  Q_protocolModbusRtuSlaveSetting::getRegisterWriteBegin()
{
	int res=0;
	char *tmp=getContentID(5,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;

}
int Q_protocolModbusRtuSlaveSetting::getRelayWriteBegin()
{
	int res=0;
	char *tmp=getContentID(6,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
int                 Q_protocolModbusRtuSlaveSetting::getYkFunCode()
{
	int res=0;
	char *tmp=getContentID(2,3);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
int Q_protocolModbusRtuSlaveSetting::getYkOnValue()
{
	int res=0;
	char *tmp=getContentID(3,3);
	if(tmp!=NULL)
	{
		res=strtol(tmp,NULL,16);
	}
	return res;
}
int Q_protocolModbusRtuSlaveSetting::getYkOffValue()
{
	long res=0;
	char *tmp=getContentID(4,3);
	if(tmp!=NULL)
	{
		res=strtol(tmp,NULL,16);
	}
	return (int)res;
}
int Q_protocolModbusRtuSlaveSetting::getYcFunCode()
{
	int res=0;
	char *tmp=getContentID(9,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
int Q_protocolModbusRtuSlaveSetting::getYxFunCode()
{
	int res=0;
	char *tmp=getContentID(10,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
int Q_protocolModbusRtuSlaveSetting::getYtFunCode()
{
	int res=0;
	char *tmp=getContentID(1,3);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
		if(res==1)
		{
			res=0x10;
		}
		else if(res==2)
		{
			res=0x06;
		}
	}
	return res;
}
int Q_protocolModbusRtuSlaveSetting::getSyncFirst()
{
	long res=0;
	char *tmp=getContentID(11,2);
	if(tmp!=NULL)
	{
		res=strtol(tmp,NULL,16);
	}
	return (int)res;
}
bool Q_protocolModbusRtuSlaveSetting::getPrintPacket()
{
	int res=0;
	char *tmp=getContentID(1,10);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res==0?false:true;
}
bool Q_protocolModbusRtuSlaveSetting::getPrintError()
{
	int res=0;
	char *tmp=getContentID(4,10);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res==0?false:true;

}
bool Q_protocolModbusRtuSlaveSetting::getPrintPrompt()
{
	int res=0;
	char *tmp=getContentID(3,10);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res==0?false:true;
}


bool Q_protocolModbusRtuSlaveSetting::getSendClearBuffer()
{
	int res=0;
	char *tmp=getContentID(12,1);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res==0?false:true;
}
int Q_protocolModbusRtuSlaveSetting::getDeviceId()
{
	int res=0;
	char *tmp=getContentID(4,1);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
ModbusFloatUpSeq Q_protocolModbusRtuSlaveSetting::getFloatUpSeq()
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
ModbusYTMethod Q_protocolModbusRtuSlaveSetting::getYTMethod()
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
ModbusFloatUpSeq Q_protocolModbusRtuSlaveSetting::getYTByteSeq()
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
ModbusAnalogUpType Q_protocolModbusRtuSlaveSetting::getYcUpType()
{
	ModbusAnalogUpType seq;
	seq=eModAnalogUpType4;
	int res=0;
	char *tmp=getContentID(1,2);
	if(tmp!=NULL)
	{
		int a=atoi(tmp);
		switch(a)
		{
		case 1:
			seq=eModAnalogUpType4;
			break;
		case 2:
			seq=eModAnalogUpType2;
			break;
		default:
			seq=eModAnalogUpType4;
			break;
		}
	}
	else
	{
		seq=eModAnalogUpType4;
	}
	return seq;
}
void  Q_protocolModbusRtuSlaveSetting::SetWndInfo(HWND wnd,int vMsgID)
{
	m_wnd = wnd;
	m_nMsgID=vMsgID;
}
HWND Q_protocolModbusRtuSlaveSetting::getWndHwnd()
{
	return m_wnd;
}
int Q_protocolModbusRtuSlaveSetting::getWndMsgID()
{
	return m_nMsgID;
}
int Q_protocolModbusRtuSlaveSetting::getWndCmdID()
{
	return m_nCmdID;
}
bool Q_protocolModbusRtuSlaveSetting::reloadLog()
{
	TiXmlDocument doc(m_sFilePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
#ifdef _DEBUG
		sprintf_s(LastError, "解析配置文件出错,当前行号:%d\n",__LINE__);
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
						sprintf_s(m_vContent[i].value,"%s",itemElement->Attribute("value"));
					}
				}


			}
		}
	}

	return true;
}
Q_protocolModbusRtuSlave::Q_protocolModbusRtuSlave()
{
	m_cpuEndian=eBigEndian;//默认是大端模式
	m_wnd=NULL;
	m_nMsgID=0;
}
Q_protocolModbusRtuSlave::~Q_protocolModbusRtuSlave()
{
	m_bPrint=false;
}
bool Q_protocolModbusRtuSlave::initializeProtocol()
{
	m_PrintSetting= (Q_protocolModbusRtuSlaveSetting *)m_pProcotolConfig->getProtocolSetting();

	if(m_PrintSetting!=NULL)
	{
		m_FloatUpSeq=m_PrintSetting->getFloatUpSeq();
		m_YtByteSeq=m_PrintSetting->getYTByteSeq();
		m_YtMethod=m_PrintSetting->getYTMethod();
		m_bPrint=m_PrintSetting->getPrintPacket();
	}
	if (Q_CommUtility::isBigEndian())
	{
		m_cpuEndian=eBigEndian;
	}
	else
	{
		m_cpuEndian=eLittleEndian;
	}
	return true;
}
bool Q_protocolModbusRtuSlave::explainData()
{

	//char bufferReceive[Q_FRAME_CONST4096];	
	//memset(bufferReceive,NULL,sizeof(bufferReceive));
	//int nLength = m_pProcotolConfig->getReceiveBuffer()->getDataLength();

	//int nByteCount = 0;

	//if(nLength >= 5)
	//{
	//	nByteCount = m_pProcotolConfig->getReceiveBuffer()->readData(bufferReceive,256,false);

	//	if(nByteCount > 0)
	//	{
	//			explainDataEx(bufferReceive,nByteCount);
	//			m_pProcotolConfig->getReceiveBuffer()->removeData(nByteCount);	
	//	}
	//}
	//return true;

	char bufferReceive[Q_FRAME_CONST4096];	
	memset(bufferReceive,NULL,sizeof(bufferReceive));
	int nLength = m_pProcotolConfig->getReceiveBuffer()->getDataLength();
	int nByteCount = 0;
	int nRes = 0;

	while(nLength > 8)
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

	if(nLength >= 8)
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
bool Q_protocolModbusRtuSlave::explainDataEx(char* buff,int nCount,int& nCurrentPos)
{
	int nIndexCur = 0;

	Q_MODBUS_ASDU_RTU *asdu=NULL;
	asdu=(Q_MODBUS_ASDU_RTU *)&buff[0];
	int len=8;
    nCurrentPos=8;
	BOOL m_crcCheckOk;
	if(m_PrintSetting!=NULL)
	{
		m_crcCheckOk = TRUE;
		if(nCount>256)
		{
			m_crcCheckOk=FALSE;
		}
		else
		{
			int nCrcCodeRight=CRC16((BYTE*)buff,nCount-2);
			int nCrcCodeRecv=*(WORD*)(&asdu->data[nCount-4]);
			if(nCrcCodeRecv!=nCrcCodeRight)
			{
				memset(m_LastError,NULL,sizeof(m_LastError));
				sprintf_s(m_LastError,"CRC code different,recv=%d,compute %d!",nCrcCodeRecv,nCrcCodeRight);

				Q_COM_LOG_INFO log;
				log.chid=m_PrintSetting->getChannelID();
				log.type=eMsgTypeMsg;
				log.msg=(char*)m_LastError;
				log.len=sizeof(m_LastError);
				::SendMessage(m_PrintSetting->getWndHwnd(),m_PrintSetting->getWndMsgID(),(WPARAM)&log,(LPARAM)sizeof(log));

				m_crcCheckOk = FALSE;
				return false;
			}
		}
	}

	if(m_PrintSetting!=NULL&&m_PrintSetting->getPrintPacket())
	{
		writefile(&buff[nIndexCur],len, 2);
	}
	if (m_bRecvMsg)
	{
		OnRecvMsg(&buff[nIndexCur],len);
	}

	if(m_PrintSetting!=NULL)
	{
		if(m_PrintSetting->getYcFunCode()==asdu->funcode)
		{
			explainFrameYC(asdu,(BYTE *)&buff[2]);
		}
		else if(m_PrintSetting->getYxFunCode()==asdu->funcode)
		{
			//读遥信
			explainFrameYX(asdu,(BYTE *)&buff[2]);
		}
		else if(m_PrintSetting->getYkFunCode()==asdu->funcode)
		{
			//遥控
			explainFrameYK(asdu,(BYTE *)&buff[2]);
		}
		else if(m_PrintSetting->getYtFunCode()==asdu->funcode)
		{
			//遥调
			if(m_PrintSetting->getYtFunCode()==0x10)
				explainFrameYT_M(asdu,(BYTE *)&buff[2]);
			else
				explainFrameYT(asdu,(BYTE *)&buff[2]);
		}
		else
		{
			//不合法功能码，也可以称为未识别功能码
			sendErroCode(asdu,MODBUS_ERROR_CODE_1);
			nIndexCur = nCount;
		}
	}

	return true;
}
//遥信
bool Q_protocolModbusRtuSlave::explainFrameYX(Q_MODBUS_ASDU_RTU *header,BYTE *data)
{
	int nSlaveYXBegin = 0;//Slave规约的起始点号
	int nYXBegin = 0;//YX起始号，下标
	if (m_PrintSetting!=NULL)
	{
		nSlaveYXBegin=m_PrintSetting->getRelayBegin();
	}

	Q_MODBUS_REC_YC *rec=(Q_MODBUS_REC_YC *)header->data;

	int startAddr=rec->startAddrHigh<<8;
	startAddr+=rec->startAddrLow;

	int oriStartAddr=startAddr;

	int regCount=0;//点的个数
	regCount=rec->regCountHigh<<8;
	regCount+=rec->regCountLow;

	Q_dataManager* p = _GlobalData.getDataManager();


	int digCount=m_PrintSetting->getPointCount();

	nYXBegin=m_PrintSetting->getPointBegin();

	Q_MODBUS_ASDU_RTU asdu;
	memset(&asdu,NULL,sizeof(asdu));
	if (oriStartAddr<nSlaveYXBegin || digCount < regCount || nSlaveYXBegin+digCount < oriStartAddr+regCount)
	{
		if(m_PrintSetting->getPrintError())
		{
			memset(m_LastError,NULL,sizeof(m_LastError));
			sprintf_s(m_LastError,sizeof(m_LastError),"主站请求不合法数据地址Channelid=%d,StartAddr =%d,count=%d\n",m_PrintSetting->getChannelID(),oriStartAddr,regCount);
			writefile(m_LastError,sizeof(m_LastError), 1);
		}
		if (m_bErrMsg)
		{
			memset(m_LastError,NULL,sizeof(m_LastError));
			sprintf_s(m_LastError,sizeof(m_LastError),"主站请求不合法数据地址Channelid=%d,StartAddr =%d,count=%d\n",m_PrintSetting->getChannelID(),oriStartAddr,regCount);
			OnErrMsg(m_LastError,sizeof(m_LastError));
		}
		sendErroCode(header,MODBUS_ERROR_CODE_2);
		return false;
	}

	if(regCount/8 + 4 >MODBUS_MAX_PACK_COUNT)
	{
		TRACE("----request data too  big ---\n");
		sendErroCode(header,MODBUS_ERROR_CODE_3);
		return false;
	}
	
	asdu.addr=header->addr;
	asdu.funcode=header->funcode;

	int  packLen=0;//有效的modbus报文长度，不包括前段TCP同步头
	packLen=regCount/8+5;

	if(regCount%8!=0)
		packLen+=1;


	int pi=0;
	asdu.data[pi++]=(packLen-5)&0xFF;


	//p->EnterMutex();
	LPQ_DIGITALEDATATAG tags=p->getDigitalData();
	LPQ_DIGITALEDATATAG curs=NULL;

	int val=0;
	Mod_DVAL dval;
	memset(&dval,NULL,sizeof(dval));

	if (nYXBegin <0)
	{
		return false;
	}


	for(int i=0;i<regCount;i++)
	{
		curs=(LPQ_DIGITALEDATATAG)&tags[nYXBegin+i];
		if(nYXBegin+i<p->getDigitalCount())
		{
			switch(i%8)
			{
			case 0:
				memset(&dval,NULL,sizeof(dval));
				dval.LSB=curs->bValue==true?1:0;
				break;
			case 1:
				dval.v2=curs->bValue==true?1:0;
				break;
			case 2:
				dval.v3=curs->bValue==true?1:0;
				break;
			case 3:
				dval.v4=curs->bValue==true?1:0;
				break;
			case 4:
				dval.v5=curs->bValue==true?1:0;
				break;
			case 5:
				dval.v6=curs->bValue==true?1:0;
				break;
			case 6:
				dval.v7=curs->bValue==true?1:0;
				break;
			case 7:
				dval.MSB=curs->bValue==true?1:0;
				asdu.data[pi++]=(*(BYTE *)&dval)&0xFF;
				break;
			}
		}
		
	}

	if(regCount%8!=0)
	{
		asdu.data[pi++]=(*(BYTE *)&dval)&0xFF;
	}
	WORD crc=CRC16((BYTE*)&asdu,regCount*2+3);
	asdu.data[pi++]=(crc)& 0xFF;
	asdu.data[pi++]=(crc >>8)& 0xFF;

	return sendData(&asdu,packLen);

	return true;
}
//遥测
bool Q_protocolModbusRtuSlave::explainFrameYC(Q_MODBUS_ASDU_RTU *header,BYTE *data)
{
	int nSlaveYCBegin = 0;//Slave规约的起始点号
	int nYCBegin = 0;//YC起始号，下标
	if (m_PrintSetting!=NULL)
	{
		nSlaveYCBegin=m_PrintSetting->getRegisterBegin();
	}
	Q_MODBUS_REC_YC *rec=(Q_MODBUS_REC_YC *)header->data;

	int startAddr=rec->startAddrHigh<<8;
	startAddr+=rec->startAddrLow;
	
	int regCount=0;
	regCount=rec->regCountHigh<<8;
	regCount+=rec->regCountLow;

	Q_dataManager* p = _GlobalData.getDataManager();
	int algCount=m_PrintSetting->getAnalogCount();

	nYCBegin=m_PrintSetting->getAnalogBegin();
	
	LPQ_ANALOGUEDATATAG tags=p->getAnalogData();
	LPQ_ANALOGUEDATATAG curs=NULL;

	int packLen=0;

    Q_MODBUS_ASDU_RTU asdu;
	ModbusAnalogUpType uptype=m_PrintSetting->getYcUpType();
	if(uptype==eModAnalogUpType4)
	{
	}
	else
	{
		/*if(regCount*2+5>MODBUS_MAX_PACK_COUNT)
		{
			if(m_PrintSetting->getPrintError())
			{
				memset(m_LastError,NULL,sizeof(m_LastError));
				sprintf_s(m_LastError,sizeof(m_LastError),"request data too  big,request count= %d,max count=%d\n",regCount*4+4,MODBUS_MAX_PACK_COUNT);
				writefile(m_LastError,sizeof(m_LastError), 1);
			}
			sendErroCode(header,MODBUS_ERROR_CODE_3);
			return false;
		}*/
		packLen=regCount*2+5;
		
		
		memset(&asdu,NULL,sizeof(asdu));

		asdu.addr=header->addr;
		asdu.funcode=header->funcode;

		int pi=0;
		asdu.data[pi++]=(regCount*2)&0xFF;

		short int val=0;
		for(int i=0;i<regCount;i++)
		{
			
			if(nYCBegin+i<p->getAnalogCount())
			{
				curs=(LPQ_ANALOGUEDATATAG)&tags[nYCBegin+i];
				if(curs->eDataType==eDataTypeFloat)
				{	
					val=(short)(curs->fValue*curs->factor);
				}
				else if(curs->eDataType==eDataTypeInt)
				{
					val=(short)(curs->nValue*curs->factor);
				}
			}
			//临时更改的因为对端不支持别的字节序
			
				asdu.data[pi]=val>>8&0xFF;
				asdu.data[pi+1]=val&0xFF;
			pi=pi+2;
		}
		//pi=pi-2;
		WORD crc=CRC16((BYTE*)&asdu,regCount*2+3);
		asdu.data[pi++]=(crc)& 0xFF;
		asdu.data[pi++]=(crc >>8)& 0xFF;

	}
	
	return sendData(&asdu,packLen);
}
//遥控
bool Q_protocolModbusRtuSlave::explainFrameYK(Q_MODBUS_ASDU_RTU *header,BYTE *data)
{

	return true;
}
//遥调
bool Q_protocolModbusRtuSlave::explainFrameYT(Q_MODBUS_ASDU_RTU *header,BYTE *data)
{
	return true;
}
//遥调
bool Q_protocolModbusRtuSlave::explainFrameYT_M(Q_MODBUS_ASDU_RTU *header,BYTE *data)
{
	return true;
}
bool Q_protocolModbusRtuSlave::sendErroCode(Q_MODBUS_ASDU_RTU *header,BYTE code)
{

	Q_MODBUS_ASDU_RTU asdu;
	memset(&asdu,NULL,sizeof(asdu));

	asdu.addr=header->addr;

	asdu.funcode=header->funcode|0x80;
	asdu.data[0]=code;
	WORD crc = CRC16((BYTE*)&asdu,3);
	asdu.data[1] = crc & 0xff;
	asdu.data[2] = (crc>>8)&0xff;

	return sendData(&asdu,5);

}
bool Q_protocolModbusRtuSlave::sendData(Q_MODBUS_ASDU_RTU *asdu,int len)
{

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		if(m_PrintSetting->getSendClearBuffer())
			pSendTool->clearBuffer();

		bool ret=pSendTool->writeData((char*)asdu,len);
		if(false==ret)
		{
			if(m_PrintSetting!=NULL)
			{
				if(m_PrintSetting->getPrintError())
				{
					memset(m_LastError,NULL,sizeof(m_LastError));
					sprintf_s(m_LastError,sizeof(m_LastError),"write buffer failed,current size = %d,and max size =%d!\n",pSendTool->getDataLength(),pSendTool->getMaxSize());
					writefile(m_LastError,sizeof(m_LastError), 1);

					writefile((char*)asdu,len, 3);
				}
				if (m_bErrMsg)
				{
					memset(m_LastError,NULL,sizeof(m_LastError));
					sprintf_s(m_LastError,sizeof(m_LastError),"write buffer failed,current size = %d,and max size =%d!\n",pSendTool->getDataLength(),pSendTool->getMaxSize());
					OnErrMsg(m_LastError,sizeof(m_LastError));
				}
				if (m_bSendMsg)
				{
					OnSendMsg((char*)asdu,len);
				}
			}
		}
		else
		{
			if(m_PrintSetting!=NULL&&m_PrintSetting->getPrintPacket()==true)
			{
				writefile((char*)asdu,len, 3);
			}
			if (m_bSendMsg)
			{
				OnSendMsg((char*)asdu,len);
			}
		}

		return ret;
	}
	return false;

}
bool Q_protocolModbusRtuSlave::makeFrame()
{
	return true;
}
bool Q_protocolModbusRtuSlave::terminateProtocol()
{	
	/*Q_protocolCdtStandardSetting* pSetting = (Q_protocolCdtStandardSetting*)m_pProcotolConfig->getProtocolSetting();
	pSetting->getTagsManager()->terminateTags();*/
	//因为发现当随机断客户端时，锁没有释放，于是就不处理了。
	/*  Q_dataManager* p = _GlobalData.getDataManager();
	p->LeaveMutex();
	TRACE(" --- terminate leave mutex ---\n");*/
	return true;

}
bool Q_protocolModbusRtuSlave::startProcess()
{


	m_eProtocolStatus = eCommonStatusNormal;
	m_yc_idx=0;
	if(NULL!=m_PrintSetting &&m_PrintSetting->getPrintPrompt())
	{
		sprintf_s(m_LastError,sizeof(m_LastError),"初使化开始...");
		writefile(m_LastError,sizeof(m_LastError),0);
	}
	if(m_bNomalInfoMsg)
	{
		sprintf_s(m_LastError,sizeof(m_LastError),"初使化开始...");
		OnNomalInfoMsg(m_LastError,sizeof(m_LastError));
	}

	return true;
}
bool Q_protocolModbusRtuSlave::stopProcess()
{
	if(NULL!=m_PrintSetting &&m_PrintSetting->getPrintPrompt())
	{
		sprintf_s(m_LastError,sizeof(m_LastError),"断开链接...");
		writefile(m_LastError,sizeof(m_LastError),0);
	}
	if(m_bNomalInfoMsg)
	{
		sprintf_s(m_LastError,sizeof(m_LastError),"断开链接...");
		OnNomalInfoMsg(m_LastError,sizeof(m_LastError));
	}
	m_eProtocolStatus = eCommonStatusNull;
	return true;
}
void Q_protocolModbusRtuSlave::writefile(char *msg,int len, int type)
{
	if(m_nMsgID>0)
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
		::SendMessage(m_PrintSetting->getWndHwnd(),m_nMsgID,(WPARAM)&log,(LPARAM)sizeof(log));
	}
}
void Q_protocolModbusRtuSlave::SetWndInfo(HWND wnd,int MsgID)
{
	m_wnd = wnd;
	m_nMsgID=MsgID;
}

WORD Q_protocolModbusRtuSlave::CRC16(BYTE* data, WORD len)
{
	WORD crc=0xffff;
	for(WORD i=0;i<len;i++){
		crc^=*(data+i);
		for(WORD j=0;j<8;j++){
			if(crc&1)
			{
				crc>>=1;
				crc^=0xa001;
			}else{
				crc>>=1;
			}
		}
	}
	return crc;
}