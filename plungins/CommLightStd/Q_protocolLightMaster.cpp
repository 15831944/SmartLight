#include "stdafx.h"
#include "gSystemDef.h"
#include "Q_protocolModbusMaster.h"
#include "tinyxmlinclude.h"


#ifdef UNIX_ENV
#ifdef __cplusplus
extern "C" {
#endif
#endif //OS_UNIX

int  mod_init(int dbg_level)
{
	//g_debug_level = dbg_level;

	return 0;
}	

Q_chDownReturn* OUTPUTAPI create_protocol_object(Q_chDownCommPara *v_param)
{
	TRACE("create_protocol_object modbusmaster/n");
	//int ret;
	Q_chDownReturn *chRet=new Q_chDownReturn();

	int ctype = v_param->cfg->type;

	int  n_sleepMills=500;//原来是 1000
	//=======
	Q_taskBase* taskTest = new Q_taskBase();
	taskTest->setBackupMode(eCommonBackupCold);
	taskTest->setRtdbInf(v_param->m_pMgr);//2015-06-23

	bool flgPri=true;

	Q_protocolModbusMasterSetting *modbusSetting=new Q_protocolModbusMasterSetting();
	modbusSetting->setFilePath(v_param->strFilePath);

	modbusSetting->initSetting();//解析xml内容

	modbusSetting->setAnalogScope(v_param->cfg->YCbegin,v_param->cfg->YCend,v_param->cfg->YCend-v_param->cfg->YCbegin+1);
	modbusSetting->setDigitalScope(v_param->cfg->YXbegin,v_param->cfg->YXend,v_param->cfg->YXend-v_param->cfg->YXbegin+1);
	
	modbusSetting->setChannelType(v_param->cfg->type);//设置通道类型

	Q_commonBuffer* bufferReceive = new Q_commonBuffer();
	Q_commonBuffer* bufferSend = new Q_commonBuffer();

	Q_channelBase* channelTest = new Q_channelBase();
	channelTest->setChannelID(v_param->cfg->id);

	channelTest->setPrimaryFlag(flgPri);
	channelTest->setReceiveBuffer(bufferReceive);
	channelTest->setSendBuffer(bufferSend);

	Q_ModbusMasterCommandManager* pCommandList = new Q_ModbusMasterCommandManager();
	channelTest->setCommandList(pCommandList);

	Q_tcpClientLink* linkTcp = new Q_tcpClientLink();
	Q_tcpClientLinkSetting* tLinkSetting = new Q_tcpClientLinkSetting();

	TRACE("User Q_tcpClientLink \r\n");
	char *tmp=NULL;
	tmp=modbusSetting->getContentID(2,1);
	if(tmp!=NULL)
	{
		tLinkSetting->setServerIp(tmp);
	}
	tmp=modbusSetting->getContentID(3,1);
	if(tmp!=NULL)
	{
		tLinkSetting->setPort(atoi(tmp));
	}

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

	Q_protocolModbusMaster* protocol = new Q_protocolModbusMaster();
	protocol->setRtdbInf(v_param->m_pMgr);
	
	n_sleepMills=modbusSetting->getSleepPerTimes();

	protocol->setSendMsg(0);

	modbusSetting->setChannelID(v_param->cfg->id);//设置通道id

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
//为了保证开辟的动态库与释放库的一致，增加此方法
void OUTPUTAPI free_protocol_object(Q_chDownReturn* pProtocol)
{
	if ((Q_chDownReturn*)pProtocol)
	{
		delete (Q_chDownReturn*)pProtocol;
		pProtocol = NULL;
	}
}

//添加命令
// 遥调、遥控
int OUTPUTAPI add_command(Q_taskBase *task,LPQ_RECORDDATATAG rec)
{
	 int ret=0;

	 TRACE("modbus-tcp add command");
	 if (rec->eDataType==eDataTypeBit)
	 {
		 Q_protocolModbusMasterCommand* pCommand = new Q_protocolModbusMasterCommand();
		 pCommand->setCommonType(eCommandTypeDialog);
		 pCommand->setDestinationId(rec->device);//设备地址
		 pCommand->setObjectIndex(rec->no);//寄存器开始地址
		 
		 if(rec->bValue)
		    pCommand->setIntValue(true);
		 else
			pCommand->setIntValue(false);
		 pCommand->setCommandModbusType(eModbusCommandRemoteControl);
		 pCommand->setCommandModbusStatus(eModbusCommandSelect);
		 pCommand->setMaxTryCount(10);

		task->addProtocolCommand(pCommand);
	 }
	 else
	 {
		 Q_protocolModbusMasterCommand* pCommand = new Q_protocolModbusMasterCommand();
		 pCommand->setCommonType(eCommandTypeDialog);
		 pCommand->setDestinationId(rec->device);//设备地址
		 pCommand->setObjectIndex(rec->no);//寄存器开始地址

		 if(rec->eDataType==eDataTypeFloat)
		 {
		     pCommand->setAnalogValue(rec->fValue);//调节值
		 }
		 else
		 {
			 pCommand->setIntValue(rec->nValue);//调节值
			 pCommand->setAnalogValue((float)rec->nValue);//调节值
		 }

		 pCommand->setCommandModbusType(eModbusCommandRemoteAdjust);
		 pCommand->setCommandModbusStatus(eModbusCommandSelect);
		 pCommand->setMaxTryCount(5);


		task->addProtocolCommand(pCommand);
			
	 }
	 return ret;
}

//打印更新记录
int  OUTPUTAPI print_change_log()
{
	printf("=== CommModbusStd Protocol Dynamic lib====\n");
	printf("=== 2016-07-22====\n");
	printf("1、增加打印输入版本功能\n");
	printf("=== 2016-09-28====\n");
	printf("1、处理解析遥信不正确的问题\n");
	printf("=== 2016-10-28====\n");
	printf("1、更改发送消息方法，去掉全网同步报文消息的功能，只本机发送即可\n");
	printf("=== 2016-11-25====\n");
	printf("1、更改优化发送消息的内容把固定值改成strlen()+1的方式\n");

	printf("=== 2017-1-10====\n");
	printf("1、增加同时采集0x01 0x02 0x03 0x04功能码的内容，支持与三一重工的风机采集数据\n");
	printf("2、修改湘电的数据采集遥测不正确的缺陷\n");
	printf("=== 2017-1-11====\n");
    printf("1、入库时设备号用功能码代替功能开启(三一重工用)\n");
	printf("->主要原因是因为三一重工存在设备地址一样、设备内编号不一样的不同记录\n");
	printf("->只是召唤的功能码有差异的记录,他们的寄存器号不唯一，所以特殊改\n");
	printf("2、湘电模式增加对四字节INT的处理\n");
	printf("=== 2017-1-12====\n");
	printf("1、增加两字节INT类型上送顺序配置\n");
	printf("=== 2017-1-13====\n");
	printf("1、增加四字节INT类型上送中字节顺序2301 3201的\n");
	printf("2、配置文件召唤遥测、遥信时如果把描述配置了，对应的内容当成功能码用\n");
	printf("=== 2017-3-28====\n");
	printf("modbusTCP主站新增四字节有符号数据遥调\n");
	printf("=== 2017-4-28====\n");
	printf("修正explianDateEx中对某些变量指针判空\n");
	printf("=== 2018-3-22====\n");
	printf("增加对美的空调特殊格式的支持\n");
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
Q_protocolModbusMasterCommand::Q_protocolModbusMasterCommand(void)
{
	m_nDestinationId=0; //站编号
	m_commandCDTType=eModbusCommandTypeNull;//命令类型
	m_commandCDTStatus=eModbusCommandStatusNull;//状态默认设定选择
	m_nObjectIndex=0;//站里面的设备编号
	//m_bOpen=false;/// 数字量
	//m_bMixed = false;
	m_fValue=0.0f;//模拟量
	m_nValue=0;//数字量
	m_nMaxTryCount=5;//最大试验数
	m_nCurTryNum=0;//当前实验数
	m_curIdx=0;
	m_eReqYcType=eExplainYcTypeNull;
	m_nFuncCode=0;
}

Q_protocolModbusMasterCommand::~Q_protocolModbusMasterCommand(void)
{

}
void Q_protocolModbusMasterCommand::setYcRequireType(int n_type)
{

	switch(n_type)
	{
	case 1:
		m_eReqYcType=eExplainYcTypeFloat;
		break;
	case 2:
		m_eReqYcType=eExplainYcTypeInt;
		break;
	case 3:
		m_eReqYcType=eExplainYcTypeMixed;
		break;
	case 4:
		m_eReqYcType=eExplainYcTypeDouble8;
		break;
	case 7:
		m_eReqYcType=eExplainYcTypeIntHuaWei;
		break;
	case 8:
		m_eReqYcType=eExplainYcTypeInt4;
		break;
	case 9:
		m_eReqYcType=eExplainYcTypeUnsignedInt4;
		break;
	case 10:
		m_eReqYcType=eExplainYcTypeUShort;
		break;
	default:
		break;
	}
}

Q_eExplainYcType Q_protocolModbusMasterCommand::getYcReuireType()
{
	return m_eReqYcType;
}
//设置请求的功能码
void  Q_protocolModbusMasterCommand::setFuncCode(int funcode)
{
	m_nFuncCode=funcode;
}
int Q_protocolModbusMasterCommand::getFuncCode()
{
	return m_nFuncCode;

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

bool Q_ModbusMasterCommandManager::addCommand(Q_protocolCommandBase* pCommand)
{
	if(pCommand == NULL)
		return false;

	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i] == NULL)
		{
		//	EnterCriticalSection(&m_csHandle);
			m_csHandle.Lock();
			m_commandList[i] = pCommand;
			m_nCurrentCommandCount++;
			m_csHandle.UnLock();
		//	LeaveCriticalSection(&m_csHandle);
			return true;
		}
	}
	return false;
}
Q_protocolModbusMasterCommand* Q_ModbusMasterCommandManager::findCommand(Q_Modbus_CommandType eType,Q_Modbus_CommandStatus eStatus,int nObjIndex)
{
	Q_protocolModbusMasterCommand* res = NULL;
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i] != NULL)
		{
			Q_protocolModbusMasterCommand* p = (Q_protocolModbusMasterCommand*)m_commandList[i];
			if(p->getCommandModbusType() == eType && p->getCommandModbusStatus() == eStatus)
			{
				if(p->getCommandModbusType() == eModbusCommandRemoteAdjust || p->getCommandModbusType() == eModbusCommandRemoteControl)
				{
					if(p->getCommandModbusStatus() == eStatus)
					{
						if(p->getObjectIndex() == nObjIndex)
						{
							res = p;
							break;
						}
					}
				}
				else
				{
					res = p;
					break;
				}
			}
		}
	}

	return res;
}

bool Q_ModbusMasterCommandManager::removeCommand(Q_Modbus_CommandType eType,Q_Modbus_CommandStatus eStatus,int nObjIndex)
{
	int nIndex = -1;
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i] != NULL)
		{
			Q_protocolModbusMasterCommand* p = (Q_protocolModbusMasterCommand*)m_commandList[i];
			if(p->getCommandModbusType() == eType && p->getCommandModbusStatus() == eStatus)
			{
				if(p->getCommandModbusType() == eModbusCommandRemoteAdjust || p->getCommandModbusType() == eModbusCommandRemoteControl)
				{
					if(p->getCommandModbusStatus() == eModbusCommandSelect)
					{
						if(p->getObjectIndex() == nObjIndex)
						{
							nIndex = i;
							break;
						}
					}
				}
				else
				{
					nIndex = i;
					break;
				}
			}
		}
	}

	if(nIndex >= 0)
	{
		//EnterCriticalSection(&m_csHandle);
			m_csHandle.Lock();
		delete m_commandList[nIndex];
		m_commandList[nIndex] = NULL;
		m_csHandle.UnLock();
		//LeaveCriticalSection(&m_csHandle);
		return true;
	}
	else
	{
		return false;
	}
}

bool Q_ModbusMasterCommandManager::removeCommand(Q_protocolCommandBase* pCommand)
{
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i] == pCommand && pCommand != NULL)
		{
			//EnterCriticalSection(&m_csHandle);
				m_csHandle.Lock();
			delete m_commandList[i];
			m_commandList[i] = NULL;
			//LeaveCriticalSection(&m_csHandle);
				m_csHandle.UnLock();
			return true;
		}
	}
	return false;
}





//----------------------------------------------------------------------------
//*
//*
//* Q_protocolModbusTcpServerStandardSetting class
//*
//*
Q_protocolModbusMasterSetting::Q_protocolModbusMasterSetting(void)
{
	memset(LastError,0,sizeof(LastError));
	memset(m_sFilePath,0,sizeof(m_sFilePath));
	m_alg_count=0;//遥测个数
	m_alg_begin=0;//遥测起始地址
	m_alg_end=0;//遥测结束地址
	m_pnt_count=0;//遥信息个数
	m_pnt_begin=0;//遥信起始地址
	m_pnt_end=0;//遥信结束地址
	m_plan_addr_begin=0;//计划曲线开始地址
	m_plan_addr_end=0;//计划曲线结束地址
	m_plan_count=0;//计划曲线信息个数
	m_alg_cmd_count=0;//遥测命令个数
	m_pnt_cmd_count=0;//遥信命令个数

}

Q_protocolModbusMasterSetting::~Q_protocolModbusMasterSetting(void)
{
	m_vGroup.clear();
	m_vContent.clear();
}

void Q_protocolModbusMasterSetting::setFilePath(char *v_path)
{
	strcpy(m_sFilePath,v_path);
}
bool Q_protocolModbusMasterSetting::initSetting()
{
	return init();
}
bool Q_protocolModbusMasterSetting::init(void)
{
	TiXmlDocument doc(m_sFilePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
#ifdef _DEBUG
		sprintf(LastError, "解析配置文件出错，当前文件:%s，当前行号:%d\n",__FILE__,__LINE__);
		TRACE("解析配置文件出错，当前文件:%s，当前行号:%d\n",__FILE__,__LINE__);
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
		if(4==nID)
		{
			const char *tmp=NULL;
			//m_alg_cmd_count=atoi(itemElement->Attribute("count"));
			for (nodeItem=node->FirstChild("Field");nodeItem;
				nodeItem=nodeItem->NextSibling("Field"))
			{
				itemElement=nodeItem->ToElement();

				ST_ModCmdCfg content;

				content.id=atoi(itemElement->Attribute("id"));
				content.device=atoi(itemElement->Attribute("device"));
				content.nBegin=atoi(itemElement->Attribute("begin"));
				content.nCount=atoi(itemElement->Attribute("count"));
				content.nType=atoi(itemElement->Attribute("type"));

				tmp=itemElement->Attribute("descr");
				if(tmp!=NULL)
				{
					if(strlen(tmp)==0)
					{
						content.code=0;
					}
					else
					{
						content.code=atoi(tmp);
					}
				}
				else
				{
					content.code=0;
				}
				m_vCommand.push_back(content);	
			}
			m_alg_cmd_count = m_vCommand.size();
		}
		else if (5==nID)
		{
				const char *tmp=NULL;
			for (nodeItem=node->FirstChild("Field");nodeItem;
				nodeItem=nodeItem->NextSibling("Field"))
			{
				itemElement=nodeItem->ToElement();

				ST_ModCmdYXCfg content;

				content.id=atoi(itemElement->Attribute("id"));
				content.device=atoi(itemElement->Attribute("device"));
				content.nBegin=atoi(itemElement->Attribute("begin"));
				content.nCount=atoi(itemElement->Attribute("count"));
				content.nType=atoi(itemElement->Attribute("type"));

			
				tmp=itemElement->Attribute("descr");
				if(tmp!=NULL)
				{
					if(strlen(tmp)==0)
					{
						content.code=0;
					}
					else
					{
					content.code=atoi(tmp);
					}
				}
				else
				{
					content.code=0;
				}
				m_vYXCommand.push_back(content);	
			}
			m_pnt_cmd_count = m_vYXCommand.size();
		}
		else
		{
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

	}
	// 	if (m_pnt_cmd_count==0 && m_alg_cmd_count==0)
	// 	{
	// 		CString strErr;
	// 		strErr.Format("modbusTCP规约中没有召唤遥测与遥信配置，文件名%s",m_sFilePath);
	// 		MessageBox(AfxGetMainWnd()->m_hWnd,strErr,"错误",MB_ICONERROR|MB_OK);
	// 		exit(0);
	// 	}
	return true;
}
bool Q_protocolModbusMasterSetting::reloadLog()
{
	TiXmlDocument doc(m_sFilePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
#ifdef _DEBUG
		sprintf_s(LastError,sizeof(LastError),"解析配置文件出错,当前行号:%d\n",__LINE__);
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
char *Q_protocolModbusMasterSetting::getContentID(const UINT &v_idx,const UINT &v_grp)
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
void   Q_protocolModbusMasterSetting::setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_alg_count=nVaildCount;
	m_alg_begin=nAddrBegin;
	m_alg_end=nAddrEnd;
}

void   Q_protocolModbusMasterSetting::setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_pnt_count=nVaildCount;
	m_pnt_begin=nAddrBegin;
	m_pnt_end=nAddrEnd;
}
void Q_protocolModbusMasterSetting::setPlanScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_plan_addr_begin=nAddrBegin;
	m_plan_addr_end=nAddrEnd;
	m_plan_count=nVaildCount;
}
//计划曲线开始地址
int Q_protocolModbusMasterSetting::getPlanAddrBegin()
{
	return m_plan_addr_begin;
}
//计划曲线结束地址
int Q_protocolModbusMasterSetting::getPlanAddrEnd()
{
	return m_plan_addr_end;

}
//计划曲线信息个数
int Q_protocolModbusMasterSetting::getPlanInfoCount()
{
	return m_plan_count;
}
int  Q_protocolModbusMasterSetting::getAnalogBegin()
{
	return m_alg_begin;
}
int  Q_protocolModbusMasterSetting::getAnalogEnd()
{
	return m_alg_end;
}
int    Q_protocolModbusMasterSetting::getPointBegin()
{ 
	return m_pnt_begin;
}
int  Q_protocolModbusMasterSetting::getPointEnd()
{
	return m_pnt_end;
}
int  Q_protocolModbusMasterSetting::getAnalogCount()
{
	return m_alg_count;
}
int  Q_protocolModbusMasterSetting::getPointCount()
{
	return m_pnt_count;
}
int Q_protocolModbusMasterSetting::getRegisterBegin()
{
	int res=0;
	char *tmp=getContentID(3,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
int Q_protocolModbusMasterSetting::getRelayBegin()
{
	int res=0;
	char *tmp=getContentID(4,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}

ModbusFloatUpSeq Q_protocolModbusMasterSetting::getFloatUpSeq()
{
	ModbusFloatUpSeq seq;
	seq=eFloatUpSeq2301;
	//int res=0;
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
		default:
			break;
		}
	}
	return seq;
}
ModbusIntUpSeq Q_protocolModbusMasterSetting::getIntUpSeq()
{
	ModbusIntUpSeq seq;
	seq=eIntUpSeqHighFirst;
	//int res=0;
	char *tmp=getContentID(12,2);
	if(tmp!=NULL)
	{
		int a=atoi(tmp);
		switch(a)
		{
		case 1:
			seq=eIntUpSeqHighFirst;
			break;
		case 2:
			seq=eIntUpSeqLowFirst;
			break;
		case 3:
			seq=eIntUpSeq2301;
			break;
		case 4:
			seq=eIntUpSeq1032;
		default:
			break;
		}
	}
	return seq;
}
ModbusYTMethod Q_protocolModbusMasterSetting::getYTMethod()
{
	ModbusYTMethod seq;
	seq=eYTMethodFloatFun06;
	//int res=0;
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
		case 5:
			seq=eYTMethodIntFun06;
			break;
		case 6:
			seq=eYTMethodFloatFun03;
			break;
		case 7:
			seq=eYTMethodIntFun06_2Bytes;
			break;
		case 8:
			seq=eYTMethodDouble8;
			break;
		case 9:
			seq=eYTMethodInt_4Bytes;
			break;
		case 10:
			seq=eYTMethodTypeUShort;
			break;

		default:
			break;
		}
	}
	return seq;
}

int Q_protocolModbusMasterSetting::getAlgCmdCount()
{
	return m_alg_cmd_count;
}
int Q_protocolModbusMasterSetting::getPntCmdCount()
{
	return m_pnt_cmd_count;
}

int Q_protocolModbusMasterSetting::getPerTime()
{
	int res=0;
	char *tmp=getContentID(6,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
int Q_protocolModbusMasterSetting::getSleepPerTimes()
{
	int res=0;
	char *tmp=getContentID(7,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
int Q_protocolModbusMasterSetting::getReadRegisterCode()
{
	int res=0;
	char *tmp=getContentID(9,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	else
	{
		res=3;//标准
	}
	return res;
}
int Q_protocolModbusMasterSetting::getReadYXRegisterCode()
{
	int res=0;
	char *tmp=getContentID(10,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	else
	{
		res=1;//标准
	}
	return res;
}
int Q_protocolModbusMasterSetting::getSyncFirstCode()
{
	int res=0;
	char *tmp=getContentID(11,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}

//update by tjs 2015.04.20
//写线圈即遥调功能码
int Q_protocolModbusMasterSetting::getWriteRegisterCode()
{
	int res=0;
	char *tmp=getContentID(13,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	else
	{
		res=16;//标准
	}
	return res;
}
//写线圈即遥控功能码
int Q_protocolModbusMasterSetting::getWriteYxRegisterCode()
{
	int res=0;
	char *tmp=getContentID(14,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	else
	{
		res=5;//标准
	}
	return res;
}
int Q_protocolModbusMasterSetting::getReadRegisterCodeEx()
{
	int res=0;
	char *tmp=getContentID(15,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	else
	{
		res=3;//标准
	}
	return res;
}
int Q_protocolModbusMasterSetting::getReadYXRegisterCodeEx()
{
	int res=0;
	char *tmp=getContentID(16,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	else
	{
		res=1;//标准
	}
	return res;
}
	bool Q_protocolModbusMasterSetting::getStartCodeAutoFlg()
	{
		bool res=false;
		char *tmp=getContentID(17,2);
		if(tmp!=NULL)
		{
			int nflg=atoi(tmp);
			if(nflg>0)
			{
				res=true;
			}

		}
		
		return res;
	}

	bool Q_protocolModbusMasterSetting::getFuncCodeEqualDeviceFlg()
	{
		
			bool res=false;
			char *tmp=getContentID(18,2);
			if(tmp!=NULL)
			{
				int nflg=atoi(tmp);
				if(nflg>0)
				{
					res=true;
				}

			}

			return res;
	}

	ModbusIntUpSeq Q_protocolModbusMasterSetting::getInt2BytesUpSeq()
	{
		ModbusIntUpSeq seq;
		seq=eIntUpSeqHighFirst;
		//int res=0;
		char *tmp=getContentID(19,2);
		if(tmp!=NULL)
		{
			int a=atoi(tmp);
			switch(a)
			{
			case 1:
				seq=eIntUpSeqHighFirst;
				break;
			case 2:
				seq=eIntUpSeqLowFirst;
				break;
			default:
				break;
			}
		}
		return seq;
	}

Q_protocolModbusMaster::Q_protocolModbusMaster()
{
	m_FloatUpSeq = eFloatUpSeqLowFirst;
	m_YtMethod = eYTMethodFloatFun10;
	m_IntUpSeq = eIntUpSeqHighFirst;
	m_cpuEndian=eBigEndian;//默认是大端模式
	m_LastCmd=NULL;
	m_nMsgID=0;
	m_PrintSetting=NULL;
	//m_wnd=NULL;
	memset(m_LastMsg,0,sizeof(m_LastMsg));
	memset(m_LastMsg,0,sizeof(m_LastMsg));
}
Q_protocolModbusMaster::~Q_protocolModbusMaster()
{

}
bool Q_protocolModbusMaster::initializeProtocol()
{
	return true;
}

bool Q_protocolModbusMaster::explainData()
{
#ifdef __Q_DEBUG_CDTPROTOCOL__
	sprintf_s(m_LastMsg,Q_FRAME_CONST128,"explain cdt frame......");
	Q_commonUtility::writeLog(__Q_DEBUG_CDTPROTOCOL__,m_LastMsg);
#endif

	char bufferReceive[Q_FRAME_CONST4096];	
	memset(bufferReceive,0,sizeof(bufferReceive));
	int nLength = m_pProcotolConfig->getReceiveBuffer()->getDataLength();
	int nByteCount = 0;
	int nRes = 0;
 //	TRACE("-----Q_protocolModbusMaster explainData() nchannelid = %d -----\n",m_PrintSetting->getChannelID());
//	sprintf_s(m_LastMsg,sizeof(m_LastMsg),"-----Q_protocolModbusMaster explainData() nchannelid = %d -----\n",m_PrintSetting->getChannelID());
	//SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

	while(nLength > Q_FRAME_CONST512)
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

	if(nLength >= MODBUS_FRAMELEN_MIN)
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

bool Q_protocolModbusMaster::explainDataEx(char* buff,int nCount,int& nCurrentPos)
{
	int nIndexCur = 0;
	int nIndexFind = 0;//信息有效的开始位置,首个字节是6个字节的前导字符
	bool bFindSync = false;

	bFindSync = findSyncIndex(buff,nCount,nIndexCur,nIndexFind);//找同步字
	if(bFindSync == false)
	{
		//find no sync char
		nIndexCur = nIndexFind;
		nCurrentPos = nIndexFind;
		return false;
	}

	nIndexCur = nIndexFind;
	//CRC校验

	Q_MODBUS_ASDU *asdu=NULL;
	asdu=(Q_MODBUS_ASDU *)&buff[nIndexCur];
	int len=asdu->header.lenHigh <<8;
	len+=+asdu->header.lenLow;

	//int nmsglen = 0;
	//if ((len+sizeof(Q_MODBUS_HEADER))>strlen(m_LastMsg)+1)
	//{
	//	nmsglen = strlen(m_LastMsg)+1;
	//}
	//else
	//{
	//	nmsglen = len+sizeof(Q_MODBUS_HEADER);
	//}

	
	if(m_PrintSetting!=NULL)
	{
		//2018-03-22 兼容一下美的空调的前报文
	//因为美的空调的数据是第6个字节一直为6是不正确的
	/*
	00 00 00 00 00 06 01 04 40 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ...

	//遥信这样的
	00 00 00 00 00 06 01 01 0B 00 00 00 00 00 00 00 00 00 00 00
	*/
		if(asdu->funcode<0x80)
		{
			BYTE *dt=(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)];

			int nByteCount=dt[0]&0xFF;
			if(nByteCount+3>len)
			{
				len=nByteCount+3;
			}
			
		}
		
	}
	SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeRx,(BYTE *)&buff[nIndexCur],len+sizeof(Q_MODBUS_HEADER));

	if((asdu->funcode & 0x80)==0x80)
	{
         explainError(asdu);
	}
	else
	{
		if(m_PrintSetting!=NULL)
		{
		/*	sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Walk explainFrame ASDU code=%d",asdu->funcode);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);*/
			//2017-04-28 add
			if(m_PrintSetting!=NULL&&m_LastCmd!=NULL)
			{
				//2017-04-28 end 因为发现代县风电场的崩溃
			//2017-04-28 
			if((m_PrintSetting->getReadYXRegisterCode()==asdu->funcode ||m_PrintSetting->getReadYXRegisterCodeEx()==asdu->funcode)&& m_LastCmd->getCommandModbusType() == eModbusCommandPoint)
			{
				//读遥信
				explainFrameYX(asdu,(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)],len-2);
			}
			else if((m_PrintSetting->getReadRegisterCode()==asdu->funcode||m_PrintSetting->getReadRegisterCodeEx()==asdu->funcode)&& m_LastCmd->getCommandModbusType() == eModbusCommandMeasure)
			{
				//读遥测
				explainFrameYC(asdu,(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)],len);
			}
			else if(m_PrintSetting->getWriteYxRegisterCode()==asdu->funcode)
			{
				//遥控
				explainFrameYK(asdu,(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)],len);

			}
			else if(m_PrintSetting->getWriteRegisterCode()==asdu->funcode)
			{

				//区分yt与yt_m只是打印是否发送成功，m是多寄存器没有m是单寄存器
				if(MODBUS_YT_M==m_PrintSetting->getWriteRegisterCode()||MODBUS_YT_M_03==m_PrintSetting->getWriteRegisterCode())
				{
					explainFrameYT_M(asdu,(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)],len);
				}
				else
				{
					explainFrameYT(asdu,(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)],len);
				}
			}
			else
			{
				//不合法功能码，也可以称为未识别功能码
				//sendErroCode(asdu,MODBUS_ERROR_CODE_1);
				nIndexCur = nCount;
			}
			}
			//	update by tjs 2015.04.20
			//   else if(MODBUS_YK==asdu->funcode)
			//{
			//	//遥控
			//	explainFrameYK(asdu,(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)],len);
			//}
			//   else if(MODBUS_YT==asdu->funcode)
			//{
			//	explainFrameYT(asdu,(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)],len);
			//}
			//   else if(MODBUS_YT_M==asdu->funcode)
			//{
			//	//遥调
			//	explainFrameYT_M(asdu,(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)],len);
			//}
			//else if(MODBUS_YT_M_03==asdu->funcode)
			//{
			//	//遥调
			//	explainFrameYT_M(asdu,(BYTE *)&buff[nIndexCur+sizeof(Q_MODBUS_ASDU)],len);
			//}
			//else
			//{
			//	//不合法功能码，也可以称为未识别功能码
			//	sendErroCode(asdu,MODBUS_ERROR_CODE_1);
			//	nIndexCur = nCount;
			//}
		}
	}
	nIndexCur=sizeof(Q_MODBUS_HEADER)+len;

	nCurrentPos = nIndexCur;
	return true;
}
bool Q_protocolModbusMaster::findSyncIndex(char* buff,int buffSize,int startPos,int& findPos)
{
	findPos = startPos;
	if(startPos < 0 || startPos >= buffSize || (startPos + MODBUS_FRAMELEN_MIN) > buffSize)
		return false;

	int firstcode=0;

	if(m_PrintSetting!=NULL)
	{
		firstcode=m_PrintSetting->getSyncFirstCode();
	}

	int ret = startPos;
	Q_MODBUS_HEADER *header=NULL;
	while((ret + MODBUS_HEADER_LEN) < buffSize)
	{
		header=(Q_MODBUS_HEADER *)&buff[ret];
		if(header!=NULL)
		{
			if(firstcode==header->codeHigh && MODBUS_SYNC_CODE2==header->codeLow
				&&header->lenLow>0)
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

//遥信
bool Q_protocolModbusMaster::explainFrameYX(Q_MODBUS_ASDU *header,BYTE *data,const int &len)
{
	//新规约打印
	/*sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Walk explainFrameYX");
	SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);*/

	Q_ModbusMasterCommandManager* pManager = (Q_ModbusMasterCommandManager*)m_pProcotolConfig->getProtocolCommandManager();
	if(pManager != NULL)
	{
		int startAddr=0;

		if(m_LastCmd!=NULL)
			startAddr=m_LastCmd->getObjectIndex();
		else
			return false;
		/*sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Walk explainFrameYX 2");
		SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);*/

		Q_protocolModbusMasterCommand* cmd=pManager->findCommand(eModbusCommandPoint,eModbusCommandAct,startAddr);
		if (NULL==cmd)
		{
			return false;
		}
		else
		{
			/*sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Walk explainFrameYX3");
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);*/

			int nByteCount=0;
			//if(data[0]!=NULL)
			//{
				nByteCount=data[0]&0xFF;
			//}

			int i=0;
			//Q_dataManager* p = _GlobalData.getDataManager();

			int tempbvalue = 0;
			int tempno = 0;

			//2017-1-11 
			//入库时设备号用功能码代替功能开启(三一重工用)
			//主要原因是因为三一重工存在设备地址一样、设备内编号不一样的不同记录
			//只是召唤的功能码有差异的记录,他们的寄存器号不唯一，所以特殊改
			int nDeviceAddr=header->addr;
			if(m_PrintSetting->getFuncCodeEqualDeviceFlg())
			{
				if(m_LastCmd!=NULL)
					nDeviceAddr=m_LastCmd->getFuncCode();

			}
			//end

			Q_RECORDDATATAG dataArray[8];
			switch(cmd->getPointType())
			{
			case 1:
				
				//int t_last_data_idx=0;
				//int nYCBegin = m_PrintSetting->getPointBegin();
				Mod_DVAL dval;


				for(i =0;i<nByteCount;i++)
				{

					memcpy(&dval,&data[1+i],sizeof(char));

					dataArray[0].bValue=(dval.LSB & 1) ==1?true:false;
					dataArray[1].bValue=(dval.v2 & 1 )==1?true:false;
					dataArray[2].bValue=(dval.v3 & 1 )==1?true:false;
					dataArray[3].bValue=(dval.v4 & 1 )==1?true:false;
					dataArray[4].bValue=(dval.v5 & 1 )==1?true:false;
					dataArray[5].bValue=(dval.v6 &1 )==1?true:false;
					dataArray[6].bValue=(dval.v7 & 1)==1?true:false;
					dataArray[7].bValue=(dval.MSB &1 )==1?true:false;

					for(int j=0;j<8;j++)
					{
						dataArray[j].eDataType=eDataTypeBit;
                        dataArray[j].channelID=m_PrintSetting->getChannelID();//必填内容
						dataArray[j].device=nDeviceAddr;
						dataArray[j].no=startAddr+i*8+j;//序号
						
						//sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Walk explainFrameYX4=,chid=%d,device=%d,no=%d",dataArray[j].channelID,dataArray[j].device,dataArray[j].no);
					//	SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);
						
						m_pMgr->recordDataValue(&dataArray[j],1);//tjs upt 2016-09-28
						
					}
				
				//	m_pMgr->recordDataValue(&dataArray[0],8);//tjs upt 
				}
			     
				
				break;
			case 2:
				Q_RECORDDATATAG dataArray_a;
				dataArray_a.eDataType=eDataTypeBit;
				//short类型两字节int
				for (int tt = 0; tt < (nByteCount/2); tt++)
				{
					tempbvalue = data[2+2*tt];
					tempno = tt + startAddr;
					if (tempbvalue == 1)
					{
						dataArray_a.bValue = true;
					} 
					else
					{
						dataArray_a.bValue = false;
					}
					dataArray_a.channelID=m_PrintSetting->getChannelID();//必填内容
					dataArray_a.device=header->addr;//地址
					dataArray_a.no=tempno;//序号

					m_pMgr->recordDataValue(&dataArray_a,1);
					
				}
				break;
			default:
				break;
			}
			cmd->setCommandModbusStatus(eModbusCommandActCon);
			return true;
		}
	}
	
	return true;
}



//遥测
bool Q_protocolModbusMaster::explainFrameYC(Q_MODBUS_ASDU *header,BYTE *data,const int &len)
{
	Q_ModbusMasterCommandManager* pManager = (Q_ModbusMasterCommandManager*)m_pProcotolConfig->getProtocolCommandManager();
	if(pManager != NULL)
	{
		int startAddr=0;

		if(m_LastCmd!=NULL)
			startAddr=m_LastCmd->getObjectIndex();
		else
			return false;
		//00 00 00 00 00 7B 01 03 78 00 01 40 50 00 29 00 00 A4 10 00 15 00 00 07 D0 00 01 00 00 07 D0 00 01 00 00 27 10 00 05 00 01 4F F0 00 00 56 C4 00 00 23 28 00 00 3C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 5A 5A FF FF FF F8 00 07 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 01 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 01
		Q_protocolModbusMasterCommand* cmd=pManager->findCommand(eModbusCommandMeasure,eModbusCommandAct,startAddr);
		if(cmd!=NULL)
		{

		//	sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Yc Require Type=%d",cmd->getYcReuireType());
		//	SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

			if(cmd->getYcReuireType()==eExplainYcTypeFloat)
			{
				explainFrameYCFloat(header,data,len,cmd,startAddr);
				
			}
			else if (cmd->getYcReuireType()==eExplainYcTypeMixed)//湘电混合模式
			{
					explainFrameYCMixed(header,data,len,cmd,startAddr);
			}
			else if(cmd->getYcReuireType()==eExplainYcTypeInt)
			{
					return explainFrameYCInt(header,data,len,cmd,startAddr);
			}
			else if(cmd->getYcReuireType()==eExplainYcTypeDouble8)
			{
					explainFrameYCDouble8(header,data,len,cmd,startAddr);
			}
			else if(cmd->getYcReuireType() == eExplainYcTypeIntHuaWei)
			{
					explainFrameYCHuaWei(header,data,len,cmd,startAddr);
			}
			else if(cmd->getYcReuireType() == eExplainYcTypeInt4)
			{
				 explainFrameYCInt4(header,data,len,cmd,startAddr);
			}
			else if(cmd->getYcReuireType() == eExplainYcTypeUnsignedInt4)
			{
				//处理四字节无符号类型
				explainFrameYCUnsignedInt4(header,data,len,cmd,startAddr);
			
			}
			else if(cmd->getYcReuireType()==eExplainYcTypeUShort)
			{
				explainFrameYCUShort(header,data,len,cmd,startAddr);
			}


			cmd->setCommandModbusStatus(eModbusCommandActCon);
			
		}
		else
		{
			return false;
		}
	}
	return true;
}
//遥控
bool Q_protocolModbusMaster::explainFrameYK(Q_MODBUS_ASDU *header,BYTE *data,const int &len)
{

	int startAddr=data[0]<<8;
	startAddr+=data[1];

	bool setStatus=true;
	if(0xFF==data[2]&&0x00==data[3])
	{
		setStatus=true;
	}
	else if(0x00==data[2]&&0x00==data[3])
	{
		setStatus=false;
	}
	else
	{
		TRACE("-----status error ----");
		sendErroCode(header,MODBUS_ERROR_CODE_4);
		return false;
	}

	Q_ModbusMasterCommandManager* pManager = (Q_ModbusMasterCommandManager*)m_pProcotolConfig->getProtocolCommandManager();
	if(pManager != NULL)
	{
		Q_protocolModbusMasterCommand* cmd=pManager->findCommand(eModbusCommandRemoteControl,eModbusCommandAct,startAddr);
		if(cmd!=NULL)
		{
			if (startAddr==cmd->getObjectIndex() && (int)setStatus==cmd->getIntValue())
			{
/*				bool bExist = false;
				int YKtoYXID32 = -1;
				int	YKID32 = -1;	

				SCADA_Relay curRelay;
				CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
				CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

				int nRelayRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_Relay_ID);
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

				SYSTEMTIME time;
				GetLocalTime(time);
				curRelay.exectime = time;
				curRelay.iexecret = 1;
				dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//

				//Q_dataManager* p = _GlobalData.getDataManager();
				LPQ_YKEDATATAG pYKData = m_pMgr->getYKData();
				for (int i=0;i<m_pMgr->getYKCount();i++)
				{
					if (m_pProcotolConfig->getProtocolSetting()->getChannelID()  ==pYKData[i].channelid && pYKData[i].device==header->addr && pYKData[i].no==startAddr)
					{
						pYKData[i].finishtime = time(NULL);
						pYKData[i].execresult = 1;
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote control execute success,device addr :%d;start reg:%d,value is %d",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getIntValue());
						OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
						if (m_bInfoMsg)
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote control execute success,device addr :%d;start reg:%d,value is %d",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getIntValue());
							OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						cmd->setCommandModbusStatus(eModbusCommandActCon);
						break;
					}
				}
*/

				return true;
			}
		}
	}
	return false;

	// 	int algCount=p->getDigitalCount();
	// 
	// 	int nSetAddrBegin=700;//要从配置文件中取得才可以。
	// 
	// 	int nIdx=startAddr-nSetAddrBegin;
	// 	if(nIdx>algCount)
	// 	{//地址是从0x00开始的
	// 		TRACE("----address did not exist---\n");
	// 		sendErroCode(header,MODBUS_ERROR_CODE_2);
	// 		return false;
	// 	}
	// 	Q_MODBUS_ASDU2 asdu;
	// 	memset(&asdu,NULL,sizeof(asdu));
	// 	asdu.header.codeHigh=MODBUS_SYNC_CODE_H;//信息编号设定为0
	// 	asdu.header.codeLow=MODBUS_SYNC_CODE2;
	// 	asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
	// 	asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
	// 	asdu.addr=header->addr;
	// 	asdu.funcode=header->funcode;
	// 
	// 	int  packLen=6;
	// 
	// 	asdu.header.lenHigh=(packLen>>8) &0xFF;
	// 	asdu.header.lenLow= packLen&0xFF;
	// 
	// 	int pi=4;
	// 	//复制置状态内容
	// 	memcpy(&asdu.data[0],data,sizeof(char)*4);
	// 	//p->EnterMutex();
	// 	LPQ_DIGITALEDATATAG tags=p->getDigitalData();
	// 	LPQ_DIGITALEDATATAG curs=NULL;
	// 	if(nIdx>=0)
	// 		curs=&tags[nIdx];
	// 
	// 	if(curs!=NULL)
	// 	{
	// 		curs->bValue=setStatus;
	// 	}
	// 	//p->LeaveMutex();
	// 
	// 	return sendData(&asdu,packLen);
}
//遥调
bool Q_protocolModbusMaster::explainFrameYT(Q_MODBUS_ASDU *header,BYTE *data,const int &len)
{
	int startAddr=data[0]<<8;
	startAddr+=data[1];

	Q_ModbusMasterCommandManager* pManager = (Q_ModbusMasterCommandManager*)m_pProcotolConfig->getProtocolCommandManager();
	if(pManager != NULL)
	{
		Q_protocolModbusMasterCommand* cmd=pManager->findCommand(eModbusCommandRemoteAdjust,eModbusCommandAct,startAddr);
		if(cmd!=NULL)
		{
			int nYTMenthod = 0;
			if (m_PrintSetting!=NULL)
			{
				nYTMenthod = m_PrintSetting->getYTMethod();
			}
			//执行成功
			switch(nYTMenthod)
			{
			case eYTMethodFloatFun06:
				{
					BYTE tmp[4];
					memset(tmp,0,sizeof(tmp));

					//Q_dataManager* p = _GlobalData.getDataManager();
					//原来有更新完成时间的代码 2015.06.30
					switch(m_FloatUpSeq)
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
					default:
						break;
					}

					float aa=0.0f;
					memcpy(&aa,tmp,sizeof(aa));

					float vv=cmd->getAnalogValue();
					if(vv>aa-0.001 && vv<aa+0.001 )
					{
						cmd->setCommandModbusStatus(eModbusCommandActCon);
						/*//原CommApp规约打印保存、先保留着
						if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %.3f",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getAnalogValue());
//							OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
						}
						if (m_bInfoMsg)
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %.3f",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getAnalogValue());
//							OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						*/

						//新规约打印
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %.3f",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getAnalogValue());
						SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

					}
					else
					{
						/*//原CommApp规约打印保存、先保留着
						if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!And original value is %.3f and current value %.3f",cmd->getAnalogValue(),aa);
//							OnErrorMsg(m_LastMsg,sizeof(m_LastMsg));
						}

						if (m_bErrMsg)
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!And original value is %.3f and current value %.3f",cmd->getAnalogValue(),aa);
//							OnErrMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						*/

						//新规约打印
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!And original value is %.3f and current value %.3f",cmd->getAnalogValue(),aa);
						SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeError,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

						cmd->setCommandModbusStatus(eModbusCommandActError);
					}
				}
				break;
			case eYTMethodIntFun06:
			case eYTMethodIntFun06_2Bytes:
				{
					int n=0;

					//更新完成时间代码

					int nRetVal=0;

					if(m_PrintSetting->getYTMethod()==eYTMethodIntFun06_2Bytes)
					{
						BYTE tmp[4];
						memset(tmp,0,sizeof(tmp));

						switch(m_IntUpSeq)
						{
						case eIntUpSeqHighFirst:
							tmp[0]=data[3];
							tmp[1]=data[2];
							break;
						case eIntUpSeqLowFirst:
							tmp[0]=data[2];
							tmp[1]=data[3];
							break;
						default:
							break;
						}

						memcpy(&nRetVal,tmp,sizeof(nRetVal));
						n=cmd->getIntValue();
					}
					else
					{

						BYTE tmp[4];
						memset(tmp,0,sizeof(tmp));

						switch(m_IntUpSeq)
						{
						case eIntUpSeqHighFirst:
							tmp[0]=data[2];
							tmp[1]=data[3];
							tmp[2]=data[4];
							tmp[3]=data[5];
							break;
						case eIntUpSeqLowFirst:
							tmp[0]=data[5];
							tmp[1]=data[4];
							tmp[2]=data[3];
							tmp[3]=data[2];
							break;
						default:
							break;
						}

						memcpy(&nRetVal,tmp,sizeof(nRetVal));
						n =(int)cmd->getAnalogValue();
					}


					if(nRetVal==n)
					{
						cmd->setCommandModbusStatus(eModbusCommandActCon);
						/*//原CommApp规约打印保存、先保留着
						if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %d",cmd->getDestinationId(),cmd->getObjectIndex(),n);
//							OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
						}
						if (m_bInfoMsg)
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %d",cmd->getDestinationId(),cmd->getObjectIndex(),n);
//							OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						*/
						//新规约打印
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %d",cmd->getDestinationId(),cmd->getObjectIndex(),n);
						SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);


						/*
						//新添加一条命令
						//if(m_PrintSetting->getYTMethod()==eYTMethodIntFun06_2Bytes)
						//{
						//	if(pYTData[nIdxCur].bzf &&
						//		pYTData[nIdxCur].no==cmd->getObjectIndex()&&
						//		pYTData[nIdxCur].channelid==pYTData[nIdxCur].zfchannelid&&
						//		pYTData[nIdxCur].device==pYTData[nIdxCur].zfdevice)
						//	{

						//		//因为湘电规约较特殊，需连续发送两个命令才可以实现一次结果
						//		Q_ModbusMasterCommandManager* pManager = (Q_ModbusMasterCommandManager*)m_pProcotolConfig->getProtocolCommandManager();
						//		if(pManager == NULL)
						//		{
						//			return false;
						//		}
						//		else
						//		{

						//			Q_protocolModbusMasterCommand* pNextCmd = new Q_protocolModbusMasterCommand();
						//			pNextCmd->setCommonType(eCommandTypeDialog);


						//			pNextCmd ->setDestinationId(pYTData[nIdxCur].zfdevice);//设备地址
						//			pNextCmd ->setObjectIndex(pYTData[nIdxCur].zfno);//寄存器开始地址

						//			pNextCmd ->setAnalogValue(cmd->getAnalogValue());//调节值

						//			pNextCmd ->setCommandModbusType(eModbusCommandRemoteAdjust);
						//			pNextCmd ->setCommandModbusStatus(eModbusCommandSelect);
						//			pNextCmd ->setMaxTryCount(10);
						//			pManager->addCommand(pNextCmd);

						//			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"启动特殊添加命令Remote adjust execute success,device addr :%d;start reg:%d,value is %.3f.",pNextCmd->getDestinationId(),pNextCmd->getObjectIndex(),cmd->getAnalogValue());
									OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
						//		}
						//	}
						//}
						*/
					}
					else
					{
						/*//原CommApp规约打印保存、先保留着
						if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!And original value is %.2f and current value %d",cmd->getAnalogValue(),n);
						//	OnErrorMsg(m_LastMsg,sizeof(m_LastMsg));
						}
						if (m_bErrMsg)
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!And original value is %.2f and current value %d",cmd->getAnalogValue(),n);
						//	OnErrMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						*/

						//新规约打印
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!And original value is %.2f and current value %d",cmd->getAnalogValue(),n);
						SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeError,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

						cmd->setCommandModbusStatus(eModbusCommandActError);
					}
				}
				break;
			case eYTMethodDouble8:
				{
					BYTE tmp[8];
					memset(tmp,0,sizeof(tmp));

				//更新完成时间代码
					switch(m_FloatUpSeq)
					{
					case eFloatUpSeqHighFirst:
						tmp[0]=data[2];
						tmp[1]=data[3];
						tmp[2]=data[4];
						tmp[3]=data[5];
						tmp[4]=data[6];
						tmp[5]=data[7];
						tmp[6]=data[8];
						tmp[7]=data[9];
						break;
					case eFloatUpSeqLowFirst:
						tmp[0]=data[9];
						tmp[1]=data[8];
						tmp[2]=data[7];
						tmp[3]=data[6];
						tmp[4]=data[5];
						tmp[5]=data[4];
						tmp[6]=data[3];
						tmp[7]=data[2];
						break;

					default:
						break;
					}

					double aa=0;
					memcpy(&aa,tmp,sizeof(aa));

					float vv=cmd->getAnalogValue();
					if(vv>aa-0.001 && vv<aa+0.001 )
					{
						cmd->setCommandModbusStatus(eModbusCommandActCon);

						/*//原CommApp规约打印保存、先保留着
						if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %.3f",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getAnalogValue());
						//	OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
						}
						if(true)
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %.3f",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getAnalogValue());
						//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1,cmd);
						}
						*/

						//新规约打印
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %.3f",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getAnalogValue());
						SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);
					}
					else
					{
						/*//原CommApp规约打印保存、先保留着
						if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!And original value is %.3f and current value %.3f",cmd->getAnalogValue(),aa);
						//	OnErrorMsg(m_LastMsg,sizeof(m_LastMsg));
						}

						if (m_bErrMsg)
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!And original value is %.3f and current value %.3f",cmd->getAnalogValue(),aa);
						//	OnErrMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						*/
						//新规约打印
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!And original value is %.3f and current value %.3f",cmd->getAnalogValue(),aa);
						SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeError,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

						cmd->setCommandModbusStatus(eModbusCommandActError);
					}
				}
				break;
			default:
				{
					cmd->setCommandModbusStatus(eModbusCommandActError);
				}
				break;
			}


		}
		return true;
	}
	else
	{
		return false;
	}	
}
//遥调
bool Q_protocolModbusMaster::explainFrameYT_M(Q_MODBUS_ASDU *header,BYTE *data,const int &len)
{
	int startAddr=data[0]<<8;
	startAddr+=data[1];

	int nRegCount=data[2]<<8;
	nRegCount+=data[3];


	Q_ModbusMasterCommandManager* pManager = (Q_ModbusMasterCommandManager*)m_pProcotolConfig->getProtocolCommandManager();
	if(pManager != NULL)
	{
		Q_protocolModbusMasterCommand* cmd=pManager->findCommand(eModbusCommandRemoteAdjust,eModbusCommandAct,startAddr);
		if(cmd!=NULL)
		{
			
			float vv=cmd->getAnalogValue();
			if(nRegCount==sizeof(vv)/2)
			{
				cmd->setCommandModbusStatus(eModbusCommandActCon);

				/*//原CommApp规约打印保存、先保留着
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %.3f",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getAnalogValue());
				//OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
				if (m_bInfoMsg)
				{
					sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %.3f",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getAnalogValue());
				//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
				}
				*/

				//新规约打印
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %.3f",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getAnalogValue());
				SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);
			}
			else
			{
				/*//原CommApp规约打印保存、先保留着
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!");
				//OnErrorMsg(m_LastMsg,sizeof(m_LastMsg));
				if (m_bErrMsg)
				{
					sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!");
				//	OnErrMsg(m_LastMsg,strlen(m_LastMsg)+1);
				}
				*/

				//新规约打印
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!");
				SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeError,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);


				cmd->setCommandModbusStatus(eModbusCommandActError);
			}

		}
		return true;
	}
	else
	{
		return false;
	}	
}
bool Q_protocolModbusMaster::sendErroCode(Q_MODBUS_ASDU *header,BYTE code)
{

	Q_MODBUS_ASDU2 asdu;
	memset(&asdu,0,sizeof(asdu));

	int firstcode=0;

	if(m_PrintSetting!=NULL)
		firstcode=m_PrintSetting->getSyncFirstCode();

	asdu.header.codeHigh=firstcode;//信息编号设定为0
	asdu.header.codeLow=MODBUS_SYNC_CODE2;
	asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
	asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
	asdu.addr=header->addr;
	asdu.header.lenLow=0x03;
	asdu.header.lenHigh=0x00;
	asdu.funcode=header->funcode|0x80;
	asdu.data[0]=code;

	return sendData(&asdu,3);
	return true;


}
bool Q_protocolModbusMaster::sendData(Q_MODBUS_ASDU2 *asdu,int len)
{
	//TRACE("-----Q_protocolModbusMaster send data channelid = %d -----\n",m_PrintSetting->getChannelID());

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		//新规约打印
		SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(BYTE *)asdu,sizeof(asdu->header)+len);

		bool ret=pSendTool->writeData((char*)asdu,sizeof(asdu->header)+len);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
		}

		return ret;
	}
	return false;
}

bool Q_protocolModbusMaster::makeFrameYT(Q_protocolModbusMasterCommand*pCommand,int nFirstCommandIndex)
{
	TRACE("Q_protocolModbusMaster::makeFrameYT");
	Q_ModbusMasterCommandManager* pManager = (Q_ModbusMasterCommandManager*)m_pProcotolConfig->getProtocolCommandManager();
	if(pManager == NULL)
	{
		return false;
	}

	if(pCommand->getCommandModbusStatus()== eModbusCommandSelect)
	{
		if (m_LastCmd!=NULL)
		{
			if (m_LastCmd->getCommandModbusType()== eModbusCommandMeasure)
			{
				//遥测 其实我不明白为什么会遥调里面，判断遥测、遥信;不知道谁写的，
				//防止上一次发送的是遥测命令而没有回来的时候，发送此次命令会干扰结果。，
				if (m_LastCmd->getCommandModbusStatus() == eModbusCommandAct)
				{
					if(m_LastCmd->getCurTryNum()>=m_LastCmd->getMaxTryCount())
					{
						if(m_LastCmd->getCommonType()==eCommandTypeCycle)
						{
							setNextAnalog(m_LastCmd);
							m_LastCmd->setCurTryNum(0);
							m_LastCmd = NULL;
						}
					}
					else 
					{
						int curNum=m_LastCmd->getCurTryNum();
						m_LastCmd->setCurTryNum(curNum+1);
					}
				}
				else if (m_LastCmd->getCommandModbusStatus() == eModbusCommandActCon)
				{
					if(m_LastCmd->getCommonType()==eCommandTypeCycle)
					{
						setNextAnalog(m_LastCmd);
						m_LastCmd->setCurTryNum(0);
					}
				}
				else if (m_LastCmd->getCommandModbusStatus() == eModbusCommandSelect)
				{
					m_LastCmd = NULL;
					if(m_YtMethod==eYTMethodFloatFun06)
					{
						handleFrameYT(pCommand);
					}
					else if (m_YtMethod==eYTMethodFloatFun10)
					{
						handleFrameYT_M(pCommand);
					}
					else if (m_YtMethod==eYTMethodFloatFun03)
					{//float非标准03
						handleFrameYT_M_03(pCommand);
					}
					else if (m_YtMethod==eYTMethodDouble8)
					{
						handleFrameYtFloat64Ex(pCommand);
					}
					else if (m_YtMethod==eYTMethodInt_4Bytes)
					{
						handleFrameYtInt4(pCommand);
					}
					else if (m_YtMethod==eYTMethodTypeUShort)
					{
						handleFrameYtInt(pCommand);
					}
					else
					{
						handleFrameYtInt(pCommand);
					}

				}
			}
			else if (m_LastCmd->getCommandModbusType()==eModbusCommandPoint)
			{
				//遥信
				if (m_LastCmd->getCommandModbusStatus() == eModbusCommandAct)
				{
					if(m_LastCmd->getCurTryNum()>=m_LastCmd->getMaxTryCount())
					{
						if(m_LastCmd->getCommonType()==eCommandTypeCycle)
						{
							setNextDigital(m_LastCmd);
							m_LastCmd->setCurTryNum(0);
							m_LastCmd = NULL;
						}
					}
					else 
					{
						int curNum=m_LastCmd->getCurTryNum();
						m_LastCmd->setCurTryNum(curNum+1);
					}
				}
				else if (m_LastCmd->getCommandModbusStatus() == eModbusCommandActCon)
				{
					if(m_LastCmd->getCommonType()==eCommandTypeCycle)
					{
						setNextDigital(m_LastCmd);
						m_LastCmd->setCurTryNum(0);
					}
				}
				else if (m_LastCmd->getCommandModbusStatus() == eModbusCommandSelect)
				{
					m_LastCmd = NULL;
					if(m_YtMethod==eYTMethodFloatFun06)
					{
						handleFrameYT(pCommand);
					}
					else if (m_YtMethod==eYTMethodFloatFun10)
					{
						handleFrameYT_M(pCommand);
					}
					else if (m_YtMethod==eYTMethodFloatFun03)
					{//float非标准03
						handleFrameYT_M_03(pCommand);
					}
					else if (m_YtMethod==eYTMethodDouble8)
					{
						handleFrameYtFloat64Ex(pCommand);
					}
					else if (m_YtMethod==eYTMethodInt_4Bytes)
					{
						handleFrameYtInt4(pCommand);
					}
					else
					{
						handleFrameYtInt(pCommand);
					}
				}

			}
			else if (m_LastCmd->getCommandModbusType()== eModbusCommandRemoteAdjust)
			{
				//如果上一条就是遥调
				if (m_LastCmd->getCommandModbusStatus() == eModbusCommandActCon ||
					m_LastCmd->getCommandModbusStatus()==eModbusCommandActError)
				{

					if(m_YtMethod==eYTMethodFloatFun06)
					{
						handleFrameYT(pCommand);
					}
					else if (m_YtMethod==eYTMethodFloatFun10)
					{
						handleFrameYT_M(pCommand);
					}
					else if (m_YtMethod==eYTMethodFloatFun03)
					{//float非标准03
						handleFrameYT_M_03(pCommand);
					}
					else if (m_YtMethod==eYTMethodDouble8)
					{
						handleFrameYtFloat64Ex(pCommand);
					}
					else if (m_YtMethod==eYTMethodInt_4Bytes)
					{
						handleFrameYtInt4(pCommand);
					}
					else if (m_YtMethod==eYTMethodTypeUShort)
					{
						handleFrameYtInt(pCommand);
					}
					else
					{
						handleFrameYtInt(pCommand);
					}

					pManager->removeCommand(m_LastCmd);
					m_LastCmd = NULL;
				}
				else 
				{
					if(m_PrintSetting!=NULL)
					{
						/*//原CommApp规约打印保存、先保留着
						sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Previous YT/YK Command is not finish,delete this command,device =%d,addr=%d \n",pCommand->getDestinationId(),pCommand->getObjectIndex());
						OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
						*/

						//新规约打印
						sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Previous YT/YK Command is not finish,delete this command,device =%d,addr=%d \n",pCommand->getDestinationId(),pCommand->getObjectIndex());
						SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

						pManager->removeCommand(pCommand);
						pCommand = NULL;
					}
				}
			}
			else
			{
				pManager->removeCommand(pCommand);
				pCommand = NULL;
			}
		}
		else
		{
			if(m_YtMethod==eYTMethodFloatFun06)
			{
				handleFrameYT(pCommand);
			}
			else if (m_YtMethod==eYTMethodFloatFun10)
			{
				handleFrameYT_M(pCommand);
			}
			else if (m_YtMethod==eYTMethodFloatFun03)
			{//float非标准03
				handleFrameYT_M_03(pCommand);
			}
			else if (m_YtMethod==eYTMethodDouble8)
			{
				handleFrameYtFloat64Ex(pCommand);
			}
			else if (m_YtMethod==eYTMethodInt_4Bytes)
			{
				handleFrameYtInt4(pCommand);
			}	
			else if (m_YtMethod==eYTMethodTypeUShort)
			{
					handleFrameYtInt(pCommand);
			}	
			else
			{
				handleFrameYtInt(pCommand);
			}
		}
	}
	else if(pCommand->getCommandModbusStatus() == eModbusCommandAct)
	{
		if(pCommand->getCurTryNum()>=pCommand->getMaxTryCount())
		{
			/*//原CommApp规约打印保存、先保留着
			if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
			{
				sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Beyond command max try count,delete command.max count=%d\n",pCommand->getMaxTryCount());
			//	OnShowMsg(m_LastMsg,strlen(m_LastMsg)+1);
			}
			if (m_bInfoMsg)
			{
				sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Beyond command max try count,delete command.max count=%d\n",pCommand->getMaxTryCount());
			//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
			}
			*/

			//新规约打印
			sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Beyond command max try count,delete command.max count=%d\n",pCommand->getMaxTryCount());
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

			pManager->removeCommandByIndex(nFirstCommandIndex);
		}
		else 
		{
			int curNum=pCommand->getCurTryNum();
			pCommand->setCurTryNum(curNum+1);
		}
		//执行完的结果删除记录
	}
	else
	{
		/*//原CommApp规约打印保存、先保留着
		if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
		{
			sprintf_s(m_LastMsg,Q_FRAME_CONST128,"delete command.command status %d\n",pCommand->getCommandModbusStatus());
		//	OnShowMsg(m_LastMsg,strlen(m_LastMsg)+1);
		}
		if (m_bInfoMsg)
		{
			sprintf_s(m_LastMsg,Q_FRAME_CONST128,"delete command.command status %d\n",pCommand->getCommandModbusStatus());
		//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
		}
		*/

		//新规约打印
		sprintf_s(m_LastMsg,Q_FRAME_CONST128,"delete command.command status %d\n",pCommand->getCommandModbusStatus());
		SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

		if (m_LastCmd == pCommand)
		{
			m_LastCmd = NULL;
		}
		//pManager->removeCommandByIndex(nFirstCommandIndex);
		pManager->removeCommand(pCommand);
	}
	return true;
}

bool Q_protocolModbusMaster::makeFrame()
{
	Q_ModbusMasterCommandManager* pManager = (Q_ModbusMasterCommandManager*)m_pProcotolConfig->getProtocolCommandManager();
	if(pManager == NULL)
	{
		return false;
	}
	int nFirstCommandIndex = -1;
	int bControlCommand = false;//查看有没有遥调、遥控命令，优先处理
	int aaa = 0;
	for (int ii=0;ii<Q_COMMANDLIST_MAX ; ii++)
	{
		Q_protocolModbusMasterCommand* pCommand = (Q_protocolModbusMasterCommand*)pManager->getCommandByIndex(ii);
		if (pCommand!=NULL)
		{
			if (nFirstCommandIndex<0)
			{
				nFirstCommandIndex = ii;
			}
			if (!bControlCommand)
			{
				if (pCommand->getCommandModbusType()==eModbusCommandRemoteAdjust || pCommand->getCommandModbusType()==eModbusCommandRemoteControl)
				{
					bControlCommand = true;
					nFirstCommandIndex = ii;
				}
			}
			aaa++;
		}
	}
	Q_protocolModbusMasterCommand* pCommand = (Q_protocolModbusMasterCommand*)pManager->getCommandByIndex(nFirstCommandIndex);
	if(pCommand != NULL)
	{
		switch(pCommand->getCommandModbusType())
		{
		case eModbusCommandRemoteAdjust:
			{
				makeFrameYT(pCommand,nFirstCommandIndex);
				break;
			}
		case eModbusCommandMeasure:
			{
				if(pCommand->getCommandModbusStatus()== eModbusCommandSelect)
				{
					//判断是否超出最大的测试次数
					handleFrameYC(pCommand);
					//bJumpFlg=true;
				}
				else if(pCommand->getCommandModbusStatus() == eModbusCommandAct)
				{
					if(pCommand->getCurTryNum()>=pCommand->getMaxTryCount())
					{
						if(pCommand->getCommonType()==eCommandTypeCycle)
						{
							/*//原CommApp规约打印保存、先保留着
							if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintTranslate())
							{
								sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Read YC,Beyond command max try count,throw the frame and next,max count=%d\n",pCommand->getMaxTryCount());
							//	OnShowMsg(m_LastMsg,strlen(m_LastMsg)+1);
							}
							if (m_bInfoMsg)
							{
								sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Read YC,Beyond command max try count,throw the frame and next,max count=%d\n",pCommand->getMaxTryCount());
							//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
							}
							*/

							//新规约打印
							sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Read YC,Beyond command max try count,throw the frame and next,max count=%d\n",pCommand->getMaxTryCount());
							SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

							setNextAnalog(pCommand);
							pCommand->setCurTryNum(0);
						}
					}
					else 
					{
						int curNum=pCommand->getCurTryNum();
						pCommand->setCurTryNum(curNum+1);
					}
					//执行完的结果删除记录
				}
				else if (pCommand->getCommandModbusStatus() == eModbusCommandActCon)
				{
					if(pCommand->getCommonType()==eCommandTypeCycle)
					{
						setNextAnalog(pCommand);
						pCommand->setCurTryNum(0);
					}
				}
				else
				{
					if(pCommand->getCommonType()!=eCommandTypeCycle)
					{
						/*//原CommApp规约打印保存、先保留着
						if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintTranslate())
						{
							sprintf_s(m_LastMsg,Q_FRAME_CONST128,"delete Read YC command.command status %d\n",pCommand->getCommandModbusStatus());
						//	OnShowMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						if (m_bInfoMsg)
						{
							sprintf_s(m_LastMsg,Q_FRAME_CONST128,"delete Read YC command.command status %d\n",pCommand->getCommandModbusStatus());
						//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						*/

						//新规约打印
						sprintf_s(m_LastMsg,Q_FRAME_CONST128,"delete Read YC command.command status %d\n",pCommand->getCommandModbusStatus());
						SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

						pCommand->setCommandModbusStatus(eModbusCommandSelect);
						pCommand->setCurTryNum(0);
					}
					else
					{
						//等待下一个执行时间
						if(pCommand->getCurTryNum()>=pCommand->getMaxTryCount())
						{
							pCommand->setCommandModbusStatus(eModbusCommandSelect);
							pCommand->setCurTryNum(0);
						}
						else 
						{
							int curNum=pCommand->getCurTryNum();
							pCommand->setCurTryNum(curNum+1);
						}
					}

				}
				break;
			}
		case eModbusCommandPoint:
			{
				if(pCommand->getCommandModbusStatus()== eModbusCommandSelect)
				{
					//判断是否超出最大的测试次数
					handleFrameYX(pCommand);
					//bJumpFlg=true;
				}
				else if(pCommand->getCommandModbusStatus() == eModbusCommandAct)
				{
					if(pCommand->getCurTryNum()>=pCommand->getMaxTryCount())
					{
						if(pCommand->getCommonType()==eCommandTypeCycle)
						{
							/*//原CommApp规约打印保存、先保留着
							if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintTranslate())
							{
								sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Read YX value,Beyond command max try count,throw the frame and next,max count=%d\n",pCommand->getMaxTryCount());
								OnShowMsg(m_LastMsg,strlen(m_LastMsg)+1);
							}
							if (m_bInfoMsg)
							{
								sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Read YX value,Beyond command max try count,throw the frame and next,max count=%d\n",pCommand->getMaxTryCount());
								OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
							}
							*/

							//新规约打印
							sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Read YX value,Beyond command max try count,throw the frame and next,max count=%d\n",pCommand->getMaxTryCount());
							SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

							setNextDigital(pCommand);
							pCommand->setCurTryNum(0);
						}
					}
					else 
					{
						int curNum=pCommand->getCurTryNum();
						pCommand->setCurTryNum(curNum+1);
					}
					//执行完的结果删除记录
				}
				else if (pCommand->getCommandModbusStatus() == eModbusCommandActCon)
				{
					if(pCommand->getCommonType()==eCommandTypeCycle)
					{
						setNextDigital(pCommand);
						pCommand->setCurTryNum(0);
					}
				}
				else
				{
					if(pCommand->getCommonType()!=eCommandTypeCycle)
					{
						/*//原CommApp规约打印保存、先保留着
						if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintTranslate())
						{
							sprintf_s(m_LastMsg,Q_FRAME_CONST128,"delete Read YX command.command status %d\n",pCommand->getCommandModbusStatus());
						//	OnShowMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						if (m_bInfoMsg)
						{
							sprintf_s(m_LastMsg,Q_FRAME_CONST128,"delete Read YX command.command status %d\n",pCommand->getCommandModbusStatus());
						//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						*/

						//新规约打印
						sprintf_s(m_LastMsg,Q_FRAME_CONST128,"delete Read YX command.command status %d\n",pCommand->getCommandModbusStatus());
						SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

						pCommand->setCommandModbusStatus(eModbusCommandSelect);
						pCommand->setCurTryNum(0);
					}
					else
					{
						//等待下一个执行时间
						if(pCommand->getCurTryNum()>=pCommand->getMaxTryCount())
						{
							pCommand->setCommandModbusStatus(eModbusCommandSelect);
							pCommand->setCurTryNum(0);
						}
						else 
						{
							int curNum=pCommand->getCurTryNum();
							pCommand->setCurTryNum(curNum+1);
						}
					}

				}
				break;
			}
		case eModbusCommandRemoteControl:
			{
				if(pCommand->getCommandModbusStatus()== eModbusCommandSelect)
				{
					//判断是否超出最大的测试次数
					handelFrameYK(pCommand);
					//bJumpFlg=true;
				}
				else if(pCommand->getCommandModbusStatus() == eModbusCommandAct)
				{
					if(pCommand->getCurTryNum()>=pCommand->getMaxTryCount())
					{
						/*//原CommApp规约打印保存、先保留着
						if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
						{
							sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Beyond command max try count,delete command.max count=%d\n",pCommand->getMaxTryCount());
						//	OnShowMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						if (m_bInfoMsg)
						{
							sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Beyond command max try count,delete command.max count=%d\n",pCommand->getMaxTryCount());
						//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						*/

						//新规约打印
						sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Beyond command max try count,delete command.max count=%d\n",pCommand->getMaxTryCount());
						SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

						pManager->removeCommandByIndex(nFirstCommandIndex/*i*/);

					}
					else 
					{
						int curNum=pCommand->getCurTryNum();
						pCommand->setCurTryNum(curNum+1);
					}
					//执行完的结果删除记录
				}
				else
				{
					/*//原CommApp规约打印保存、先保留着
					if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
					{
						sprintf_s(m_LastMsg,Q_FRAME_CONST128,"delete command.command status %d\n",pCommand->getCommandModbusStatus());
					//	OnShowMsg(m_LastMsg,strlen(m_LastMsg)+1);
					}
					if (m_bInfoMsg)
					{
						sprintf_s(m_LastMsg,Q_FRAME_CONST128,"delete command.command status %d\n",pCommand->getCommandModbusStatus());
					//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
					}
					*/

					//新规约打印
					sprintf_s(m_LastMsg,Q_FRAME_CONST128,"delete command.command status %d\n",pCommand->getCommandModbusStatus());
					SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

					if (m_LastCmd == pCommand)
					{
						m_LastCmd = NULL;
					}
					//pManager->removeCommandByIndex(nFirstCommandIndex);
					pManager->removeCommand(pCommand);
				}
			}
			break;
		default:
			{
				pManager->removeCommandByIndex(nFirstCommandIndex/*i*/);
			}
			break;
		}
	}
	return true;
}
bool Q_protocolModbusMaster::handleFrameYX(Q_protocolModbusMasterCommand* pCommand)
{
	int startAddr=pCommand->getObjectIndex();

	Q_MODBUS_ASDU2 asdu;
	memset(&asdu,0,sizeof(asdu));

	int firstcode=0;

	if(m_PrintSetting!=NULL)
	{
	//	asdu.funcode=m_PrintSetting->getReadYXRegisterCode();
		asdu.funcode=pCommand->getFuncCode();
		firstcode=m_PrintSetting->getSyncFirstCode();
	}
	else
	{
		asdu.funcode=MODBUS_YX;
	}
	asdu.header.codeHigh=firstcode;//信息编号设定为0
	asdu.header.codeLow=MODBUS_SYNC_CODE2;
	asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
	asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
	asdu.addr=pCommand->getDestinationId()&0xFF;


	int  packLen=0x06;

	asdu.header.lenHigh=(packLen>>8) &0xFF;
	asdu.header.lenLow= packLen&0xFF;

	asdu.data[0]=(startAddr>>8) & 0xFF;
	asdu.data[1]=startAddr&0xFF;

	int nCount=pCommand->getIntValue();

	asdu.data[2]=(nCount>>8 )& 0xFF;
	asdu.data[3]=nCount & 0xFF;
	pCommand->setCommandModbusStatus(eModbusCommandAct);

	m_LastCmd=pCommand;

	return sendData(&asdu,packLen);
}

bool Q_protocolModbusMaster::handleFrameYC(Q_protocolModbusMasterCommand* pCommand)
{
	//TRACE("-----handleFrameYC------- \n");
	int startAddr=pCommand->getObjectIndex();

	Q_MODBUS_ASDU2 asdu;
	memset(&asdu,0,sizeof(asdu));

	int firstcode=0;

	if(m_PrintSetting!=NULL)
	{
		//asdu.funcode=m_PrintSetting->getReadRegisterCode();
		asdu.funcode=pCommand->getFuncCode();
		firstcode=m_PrintSetting->getSyncFirstCode();
	}
	else
	{
		asdu.funcode=MODBUS_YC;
	}
	asdu.header.codeHigh=firstcode;//信息编号设定为0
	asdu.header.codeLow=MODBUS_SYNC_CODE2;
	asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
	asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
	asdu.addr=pCommand->getDestinationId()&0xFF;



	int  packLen=0x06;

	asdu.header.lenHigh=(packLen>>8) &0xFF;
	asdu.header.lenLow= packLen&0xFF;

	asdu.data[0]=(startAddr>>8) & 0xFF;
	asdu.data[1]=startAddr&0xFF;

	int nCount=pCommand->getIntValue();

	asdu.data[2]=(nCount>>8 )& 0xFF;
	asdu.data[3]=nCount & 0xFF;
	pCommand->setCommandModbusStatus(eModbusCommandAct);

	m_LastCmd=pCommand;
	//TRACE("-----sendData------- \n");
	return sendData(&asdu,packLen);
}

bool Q_protocolModbusMaster::handleFrameYT_M(Q_protocolModbusMasterCommand* pCommand)
{
	if(pCommand->getCommandModbusStatus() == eModbusCommandSelect)
	{
		int startAddr=pCommand->getObjectIndex();
		int firstcode=0;
		if(m_PrintSetting!=NULL)
		{
			firstcode=m_PrintSetting->getSyncFirstCode();
		}
		
		Q_RECORDDATATAG ytRec;
		ytRec.eDataType=eDataTypeFloat;
		ytRec.channelID=m_pProcotolConfig->getProtocolSetting()->getChannelID();
		ytRec.device=pCommand->getDestinationId();
		ytRec.no=startAddr;
		ytRec.fValue=pCommand->getAnalogValue();//赋初值

		if(!m_pMgr->getYtValue(&ytRec))
		{
			/*//原CommApp规约打印保存、先保留着
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			//OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
			if (m_bInfoMsg)
			{
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
			}
			*/

			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in SetPoint.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

			return false;
		}

		Q_MODBUS_ASDU2 asdu;
		memset(&asdu,0,sizeof(asdu));
		asdu.header.codeHigh=firstcode;//信息编号设定为0
		asdu.header.codeLow=MODBUS_SYNC_CODE2;
		asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
		asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
		asdu.addr=pCommand->getDestinationId()&0xFF;
		asdu.funcode=MODBUS_YT_M;
		int  packLen=0x0B;
		asdu.header.lenHigh=(packLen>>8) &0xFF;
		asdu.header.lenLow= packLen&0xFF;
		asdu.data[0]=startAddr>>8 &0xFF;
		asdu.data[1]=startAddr&0xFF;
		asdu.data[2]=0x00;
		asdu.data[3]=0x02;
		asdu.data[4]=0x04;

	    float val=ytRec.fValue;//系数已处理过


		//四字节浮点型

		// val在内存中是高字节在前，低字节在后，顺序为 3,2,1,0 转为2,3,0,1
		BYTE tmp[4];
		memset(tmp,0,sizeof(tmp));
		memcpy(tmp,&val,sizeof(tmp));

		//两个寄存器内部，低位先传高位后传,同时寄存器内部高位在前，低位在后
		switch(m_FloatUpSeq)
		{
		case eFloatUpSeqLowFirst:
			asdu.data[5]=tmp[3];
			asdu.data[6]=tmp[2];
			asdu.data[7]=tmp[1];
			asdu.data[8]=tmp[0];
			break;
		case eFloatUpSeq2301:
			asdu.data[5]=tmp[2];
			asdu.data[6]=tmp[3];
			asdu.data[7]=tmp[0];
			asdu.data[8]=tmp[1];
			break;
		case eFloatUpSeq1032:
			asdu.data[5]=tmp[1];
			asdu.data[6]=tmp[0];
			asdu.data[7]=tmp[3];
			asdu.data[8]=tmp[2];
			break;	
		case eFloatUpSeqHighFirst:
			asdu.data[5]=tmp[0];
			asdu.data[6]=tmp[1];
			asdu.data[7]=tmp[2];
			asdu.data[8]=tmp[3];
			break;
		}


		pCommand->setCommandModbusStatus(eModbusCommandAct);

		if(m_pMgr->handleMasterYt(&ytRec)!=0)
		{
			/*//原CommApp规约打印保存、先保留着
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command,device:%d register : %d value:%.3f",pCommand->getDestinationId(),startAddr,val);
			//OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
			if (m_bInfoMsg)
			{
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command,device:%d register : %d value:%.3f",pCommand->getDestinationId(),startAddr,val);
			//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
			}
			*/

			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command,device:%d register : %d value:%.3f",pCommand->getDestinationId(),startAddr,val);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);


		}
		
		

		return sendData(&asdu,packLen);
	}

	return true;
}

bool Q_protocolModbusMaster::handleFrameYT_M_03(Q_protocolModbusMasterCommand* pCommand)
{
	if(pCommand->getCommandModbusStatus() == eModbusCommandSelect)
	{
		int startAddr=pCommand->getObjectIndex();
		int firstcode=0;
		if(m_PrintSetting!=NULL)
		{
			firstcode=m_PrintSetting->getSyncFirstCode();
		}
		Q_MODBUS_ASDU2 asdu;
		memset(&asdu,0,sizeof(asdu));
		asdu.header.codeHigh=firstcode;//信息编号设定为0
		asdu.header.codeLow=MODBUS_SYNC_CODE2;
		asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
		asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
		asdu.addr=pCommand->getDestinationId()&0xFF;
		asdu.funcode=MODBUS_YT_M_03;
		int  packLen=0x0B;
		asdu.header.lenHigh=(packLen>>8) &0xFF;
		asdu.header.lenLow= packLen&0xFF;
		asdu.data[0]=startAddr>>8 &0xFF;
		asdu.data[1]=startAddr&0xFF;
		asdu.data[2]=0x00;
		asdu.data[3]=0x02;
		asdu.data[4]=0x04;
		float val=pCommand->getAnalogValue();
		//四字节浮点型

		// val在内存中是高字节在前，低字节在后，顺序为 3,2,1,0 转为2,3,0,1
		BYTE tmp[4];
		memset(tmp,0,sizeof(tmp));
		memcpy(tmp,&val,sizeof(tmp));

		//两个寄存器内部，低位先传高位后传,同时寄存器内部高位在前，低位在后
		switch(m_FloatUpSeq)
		{
		case eFloatUpSeqLowFirst:
			asdu.data[5]=tmp[3];
			asdu.data[6]=tmp[2];
			asdu.data[7]=tmp[1];
			asdu.data[8]=tmp[0];
			break;
		case eFloatUpSeq2301:
			asdu.data[5]=tmp[2];
			asdu.data[6]=tmp[3];
			asdu.data[7]=tmp[0];
			asdu.data[8]=tmp[1];
			break;
		case eFloatUpSeq1032:
			asdu.data[5]=tmp[1];
			asdu.data[6]=tmp[0];
			asdu.data[7]=tmp[3];
			asdu.data[8]=tmp[2];
			break;	
		case eFloatUpSeqHighFirst:
			asdu.data[5]=tmp[0];
			asdu.data[6]=tmp[1];
			asdu.data[7]=tmp[2];
			asdu.data[8]=tmp[3];
			break;
		default:
			break;
		}


		pCommand->setCommandModbusStatus(eModbusCommandAct);

		sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command,device:%d register : %d value:%f",pCommand->getDestinationId(),startAddr,val);

		/*//原CommApp规约打印保存、先保留着
		//OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
		*/
		SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);


		m_LastCmd=pCommand;


		int nPos = 0;
		bool bExist = false;
		int YTtoYCID32 = -1;
		int	YTID32 = -1;	

		SCADA_SetPoint curSetPoint;
		CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
		CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

		int nSetPointRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_SetPoint_ID );
		for (int j=0; j<nSetPointRecordCount; j++)
		{
			int nTempid32 = j+1;
			dbmg_sdb->GetElementByID(SCADA_TAB_SetPoint_ID, nTempid32,&curSetPoint);

			if (m_PrintSetting->getChannelID() == curSetPoint.uTuID && asdu.addr == curSetPoint.lDPUID &&
				startAddr == curSetPoint.iNoInDPU)
			{
				bExist = true;
				YTID32 = nTempid32;
				YTtoYCID32 = curSetPoint.ulYCID32;
				break;
			}
		}

		if (bExist)//存在
		{
			curSetPoint.fVal = val;
			curSetPoint.iexecret = 0;
			dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调选择时间
		}

/*		//Q_dataManager* p = _GlobalData.getDataManager();
		LPQ_YTEDATATAG pYTData = m_pMgr->getYTData();
		for (int i=0;i<m_pMgr->getYTCount();i++)
		{
			if (m_pProcotolConfig->getProtocolSetting()->getChannelID()  ==pYTData[i].channelid && pYTData[i].device==asdu.addr && pYTData[i].no==startAddr)
			{
				pYTData[i].fValue = val;
				pYTData[i].exectime = time(NULL);
				pYTData[i].finishtime = 0;
				pYTData[i].execresult = 0;
				break;
			}
		}
*/
		return sendData(&asdu,packLen);
	}
	return true;
}

bool Q_protocolModbusMaster::handleFrameYtInt(Q_protocolModbusMasterCommand* pCommand)
{
	if(pCommand->getCommandModbusStatus() == eModbusCommandSelect)
	{
		int startAddr=pCommand->getObjectIndex();

		int firstcode=0;

		if(m_PrintSetting!=NULL)
		{
			firstcode=m_PrintSetting->getSyncFirstCode();
		}

		Q_MODBUS_ASDU2 asdu;
		memset(&asdu,0,sizeof(asdu));
		asdu.header.codeHigh=firstcode;//信息编号设定为0
		asdu.header.codeLow=MODBUS_SYNC_CODE2;
		asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
		asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
		asdu.addr=pCommand->getDestinationId()&0xFF;
		if(m_PrintSetting->getYTMethod()==eYTMethodTypeUShort)
		{
			asdu.funcode=MODBUS_YT_M;
		}
		else
		{
			asdu.funcode=MODBUS_YT;
		}
		

		//查询遥调索引
		int curYtIndex=-1;
		//Q_dataManager* p = _GlobalData.getDataManager();
		Q_RECORDDATATAG ytRec;
		ytRec.eDataType=eDataTypeFloat;
		ytRec.channelID=m_pProcotolConfig->getProtocolSetting()->getChannelID();
		ytRec.device=pCommand->getDestinationId();
		ytRec.no=startAddr;
		ytRec.fValue=pCommand->getAnalogValue();//赋初值

		if(!m_pMgr->getYtValue(&ytRec))
		{
			/*//原CommApp规约打印保存、先保留着
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			//OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
			if (m_bInfoMsg)
			{
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
			}
			*/

			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,handleFrameYtInt,did not find config in SetPoint.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

			return false;
		}

		int  packLen=0;


		asdu.data[0]=startAddr>>8 &0xFF;
		asdu.data[1]=startAddr&0xFF;

		float val=pCommand->getAnalogValue();


		//整形

		int nVal=(int)(ytRec.fValue);

		if(m_PrintSetting->getYTMethod()==eYTMethodIntFun06)
		{
			packLen=0x08;

			asdu.header.lenHigh=(packLen>>8) &0xFF;
			asdu.header.lenLow= packLen&0xFF;

			BYTE tmp[4];
			memset(tmp,0,sizeof(tmp));

			memcpy(tmp,&nVal,sizeof(tmp));
			switch(m_IntUpSeq)
			{
			case eIntUpSeqLowFirst:
				asdu.data[2]=tmp[3];
				asdu.data[3]=tmp[2];
				asdu.data[4]=tmp[1];
				asdu.data[5]=tmp[0];
				break;
			case eIntUpSeqHighFirst:
				asdu.data[2]=tmp[0];
				asdu.data[3]=tmp[1];
				asdu.data[4]=tmp[2];
				asdu.data[5]=tmp[3];
				break;
			case eFloatUpSeq2301:
				asdu.data[2]=tmp[2];
				asdu.data[3]=tmp[3];
				asdu.data[4]=tmp[0];
				asdu.data[5]=tmp[1];
				break;
			case eFloatUpSeq1032:
				asdu.data[2]=tmp[1];
				asdu.data[3]=tmp[0];
				asdu.data[4]=tmp[3];
				asdu.data[5]=tmp[2];
				break;	
			default:
				break;
			}
		}
		else  if(m_PrintSetting->getYTMethod()==eYTMethodTypeUShort)
		{
			packLen=0x07;

			asdu.header.lenHigh=(packLen>>8) &0xFF;
			asdu.header.lenLow= packLen&0xFF;


			unsigned short sval=(unsigned short)nVal;
			BYTE tmp2[2];
			memset(tmp2,0,sizeof(tmp2));
			memcpy(tmp2,&sval,sizeof(tmp2));

			asdu.data[2]=0x02;
			//整形
			switch(m_IntUpSeq)
			{
			case eIntUpSeqHighFirst:
				asdu.data[3]=tmp2[1];
				asdu.data[4]=tmp2[0];
				break;
			case eIntUpSeqLowFirst:
				asdu.data[4]=tmp2[0];
				asdu.data[3]=tmp2[1];
				break;
			default:
				break;
			}					
			pCommand->setIntValue(nVal);
		}
		else
		{
			packLen=0x06;

			asdu.header.lenHigh=(packLen>>8) &0xFF;
			asdu.header.lenLow= packLen&0xFF;

		
				short sval=(short)nVal;
				BYTE tmp2[2];
				memset(tmp2,0,sizeof(tmp2));
				memcpy(tmp2,&sval,sizeof(tmp2));

				//整形
				switch(m_IntUpSeq)
				{
				case eIntUpSeqHighFirst:
					asdu.data[2]=tmp2[1];
					asdu.data[3]=tmp2[0];
					break;
				case eIntUpSeqLowFirst:
					asdu.data[2]=tmp2[0];
					asdu.data[3]=tmp2[1];
					break;
				default:
					break;
				}					
				pCommand->setIntValue(nVal);


		}
		pCommand->setCommandModbusStatus(eModbusCommandAct);

		m_LastCmd=pCommand;

	
		//新规约打印
		sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Send YT command,device:%d register : %d value:%d",pCommand->getDestinationId(),startAddr,nVal);
		SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

		/*	pYTData[curYtIndex].fValue = val;
		pYTData[curYtIndex].exectime = time(NULL);
		pYTData[curYtIndex].finishtime = 0;
		pYTData[curYtIndex].execresult = 0;*/

		return sendData(&asdu,packLen);
	}
	return true;

}

bool Q_protocolModbusMaster::handleFrameYT(Q_protocolModbusMasterCommand* pCommand)
{
	if(pCommand->getCommandModbusStatus() == eModbusCommandSelect)
	{
		int startAddr=pCommand->getObjectIndex();

		int firstcode=0;

		if(m_PrintSetting!=NULL)
		{
			firstcode=m_PrintSetting->getSyncFirstCode();
		}

		Q_MODBUS_ASDU2 asdu;
		memset(&asdu,0,sizeof(asdu));
		asdu.header.codeHigh=firstcode;//信息编号设定为0
		asdu.header.codeLow=MODBUS_SYNC_CODE2;
		asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
		asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
		asdu.addr=pCommand->getDestinationId()&0xFF;
		asdu.funcode=MODBUS_YT;

		//查询遥调索引
		//int curYtIndex=-1;
		//Q_dataManager* p = _GlobalData.getDataManager();

		Q_RECORDDATATAG ytRec;
		ytRec.eDataType=eDataTypeFloat;
		ytRec.channelID=m_pProcotolConfig->getProtocolSetting()->getChannelID();
		ytRec.device=pCommand->getDestinationId();
		ytRec.no=startAddr;
		ytRec.fValue=pCommand->getAnalogValue();//赋初值

		if(!m_pMgr->getYtValue(&ytRec))
		{
			/*//原CommApp规约打印保存、先保留着
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			//OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
			if (m_bInfoMsg)
			{
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
			}
			*/

			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,handleFrameYT,did not find config in SetPoint.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);
			return false;
		}

		int  packLen=0x08;

		asdu.header.lenHigh=(packLen>>8) &0xFF;
		asdu.header.lenLow= packLen&0xFF;

		asdu.data[0]=startAddr>>8 &0xFF;
		asdu.data[1]=startAddr&0xFF;

		float val=ytRec.fValue;//系数已处理过
		//四字节浮点型

		// val在内存中是高字节在前，低字节在后，顺序为 3,2,1,0 转为2,3,0,1
		BYTE tmp[4];
		memset(tmp,0,sizeof(tmp));
		memcpy(tmp,&val,sizeof(tmp));

		//两个寄存器内部，低位先传高位后传,同时寄存器内部高位在前，低位在后
		switch(m_FloatUpSeq)
		{
		case eFloatUpSeqLowFirst:
			asdu.data[2]=tmp[3];
			asdu.data[3]=tmp[2];
			asdu.data[4]=tmp[1];
			asdu.data[5]=tmp[0];
			break;
		case eFloatUpSeq2301:
			asdu.data[2]=tmp[2];
			asdu.data[3]=tmp[3];
			asdu.data[4]=tmp[0];
			asdu.data[5]=tmp[1];
			break;
		case eFloatUpSeq1032:
			asdu.data[2]=tmp[1];
			asdu.data[3]=tmp[0];
			asdu.data[4]=tmp[3];
			asdu.data[5]=tmp[2];
			break;	
		case eFloatUpSeqHighFirst:
			asdu.data[2]=tmp[0];
			asdu.data[3]=tmp[1];
			asdu.data[4]=tmp[2];
			asdu.data[5]=tmp[3];
			break;
		default:
			break;
		}

		pCommand->setCommandModbusStatus(eModbusCommandAct);

		m_LastCmd=pCommand;

		/*//原CommApp规约打印保存、先保留着
		if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
		{
			sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Send YT command,device:%d register : %d value:%.3f",pCommand->getDestinationId(),startAddr,val);
		//	OnShowMsg(m_LastMsg,strlen(m_LastMsg)+1);
		}
		if (m_bInfoMsg)
		{
			sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Send YT command,device:%d register : %d value:%.3f",pCommand->getDestinationId(),startAddr,val);
		//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
		}
		*/

		//新规约打印
		sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Send YT command,device:%d register : %d value:%.3f",pCommand->getDestinationId(),startAddr,val);
		SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

	
		if(m_pMgr->handleMasterYt(&ytRec)!=0)
		{
			/*//原CommApp规约打印保存、先保留着
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			//OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
			if (m_bInfoMsg)
			{
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
			}
			*/

			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

		}

		return sendData(&asdu,packLen);
	}
	return true;
}

bool Q_protocolModbusMaster::handleFrameYtInt4(Q_protocolModbusMasterCommand* pCommand)
{
	if(pCommand->getCommandModbusStatus() == eModbusCommandSelect)
	{
		int startAddr=pCommand->getObjectIndex();
		int firstcode=0;
		if(m_PrintSetting!=NULL)
		{
			firstcode=m_PrintSetting->getSyncFirstCode();
		}
		
		Q_RECORDDATATAG ytRec;
		ytRec.eDataType=eDataTypeFloat;
		ytRec.channelID=m_pProcotolConfig->getProtocolSetting()->getChannelID();
		ytRec.device=pCommand->getDestinationId();
		ytRec.no=startAddr;
		ytRec.fValue=pCommand->getAnalogValue();//赋初值

		if(!m_pMgr->getYtValue(&ytRec))
		{
			/*//原CommApp规约打印保存、先保留着
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			//OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
			if (m_bInfoMsg)
			{
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
			}
			*/

			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in SetPoint.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

			return false;
		}

		Q_MODBUS_ASDU2 asdu;
		memset(&asdu,0,sizeof(asdu));
		asdu.header.codeHigh=firstcode;//信息编号设定为0
		asdu.header.codeLow=MODBUS_SYNC_CODE2;
		asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
		asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
		asdu.addr=pCommand->getDestinationId()&0xFF;
		asdu.funcode=MODBUS_YT_M;
		int  packLen=0x0B;
		asdu.header.lenHigh=(packLen>>8) &0xFF;
		asdu.header.lenLow= packLen&0xFF;
		asdu.data[0]=startAddr>>8 &0xFF;
		asdu.data[1]=startAddr&0xFF;
		asdu.data[2]=0x00;
		asdu.data[3]=0x02;
		asdu.data[4]=0x04;

	    float val=ytRec.fValue;//系数已处理过


		//四字节浮点型

		// val在内存中是高字节在前，低字节在后，顺序为 3,2,1,0 转为2,3,0,1
		BYTE tmp[4];
		memset(tmp,0,sizeof(tmp));
		if(ytRec.fValue<0)
		{
			int val = (int)(ytRec.fValue);
			memcpy(tmp,&val,sizeof(tmp));
		}
		else
		{
			unsigned int val = (unsigned int)(ytRec.fValue);
			memcpy(tmp,&val,sizeof(tmp));
		}

		//两个寄存器内部，低位先传高位后传,同时寄存器内部高位在前，低位在后
		switch(m_FloatUpSeq)
		{
		case eFloatUpSeqLowFirst:
			asdu.data[5]=tmp[3];
			asdu.data[6]=tmp[2];
			asdu.data[7]=tmp[1];
			asdu.data[8]=tmp[0];
			break;
		case eFloatUpSeq2301:
			asdu.data[5]=tmp[2];
			asdu.data[6]=tmp[3];
			asdu.data[7]=tmp[0];
			asdu.data[8]=tmp[1];
			break;
		case eFloatUpSeq1032:
			asdu.data[5]=tmp[1];
			asdu.data[6]=tmp[0];
			asdu.data[7]=tmp[3];
			asdu.data[8]=tmp[2];
			break;	
		case eFloatUpSeqHighFirst:
			asdu.data[5]=tmp[0];
			asdu.data[6]=tmp[1];
			asdu.data[7]=tmp[2];
			asdu.data[8]=tmp[3];
			break;
		}


		pCommand->setCommandModbusStatus(eModbusCommandAct);

		if(m_pMgr->handleMasterYt(&ytRec)!=0)
		{
			/*//原CommApp规约打印保存、先保留着
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command,device:%d register : %d value:%.3f",pCommand->getDestinationId(),startAddr,val);
			//OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
			if (m_bInfoMsg)
			{
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command,device:%d register : %d value:%.3f",pCommand->getDestinationId(),startAddr,val);
			//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
			}
			*/

			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command,device:%d register : %d value:%.3f",pCommand->getDestinationId(),startAddr,val);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);


		}
		
		

		return sendData(&asdu,packLen);
	}

	return true;
}
bool Q_protocolModbusMaster::handleFrameYtFloat64Ex(Q_protocolModbusMasterCommand* pCommand)
{
	if(pCommand->getCommandModbusStatus() == eModbusCommandSelect)
	{
		int startAddr=pCommand->getObjectIndex();

		int firstcode=0;

		if(m_PrintSetting!=NULL)
		{
			firstcode=m_PrintSetting->getSyncFirstCode();
		}

		Q_MODBUS_ASDU2 asdu;
		memset(&asdu,0,sizeof(asdu));
		asdu.header.codeHigh=firstcode;//信息编号设定为0
		asdu.header.codeLow=MODBUS_SYNC_CODE2;
		asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
		asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
		asdu.addr=pCommand->getDestinationId()&0xFF;
		asdu.funcode = m_PrintSetting->getWriteRegisterCode();

		//查询遥调索引
		int curYtIndex=-1;
		//Q_dataManager* p = _GlobalData.getDataManager();
		LPQ_YTEDATATAG pYTData = m_pMgr->getYTData();
		for (int i=0;i<m_pMgr->getYTCount();i++)
		{
			if (m_pProcotolConfig->getProtocolSetting()->getChannelID()  ==pYTData[i].channelid && pYTData[i].device==pCommand->getDestinationId()&& pYTData[i].no==startAddr)
			{
				curYtIndex=i;
				break;
			}
		}
		if(curYtIndex<0)
		{
			/*//原CommApp规约打印保存、先保留着
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			//OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
			if(true)
			{
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1,pCommand);
			}
			*/

			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

			return false;
		}

		int  packLen=15;

		asdu.header.lenHigh=(packLen>>8) &0xFF;
		asdu.header.lenLow= packLen&0xFF;

		asdu.data[0]=startAddr>>8 &0xFF;
		asdu.data[1]=startAddr&0xFF;
		asdu.data[2]=0x00;
		asdu.data[3]=0x04;
		asdu.data[4]=0x08;


		//8字节浮点型
		double dwVal=pCommand->getAnalogValue()*pYTData[curYtIndex].factor;
		//short nVal=(short)dwVal;
		BYTE tempValue[8];
		memset(tempValue,0,sizeof(tempValue));
		memcpy(tempValue,&dwVal,sizeof(tempValue));

		//两个寄存器内部，低位先传高位后传,同时寄存器内部高位在前，低位在后
		switch(m_FloatUpSeq)
		{
		case eFloatUpSeqHighFirst:
			asdu.data[5]=tempValue[0];
			asdu.data[6]=tempValue[1];
			asdu.data[7]=tempValue[2];
			asdu.data[8]=tempValue[3];
			asdu.data[9]=tempValue[4];
			asdu.data[10]=tempValue[5];
			asdu.data[11]=tempValue[6];
			asdu.data[12]=tempValue[7];
			break;
		case eFloatUpSeqLowFirst:
			asdu.data[5]=tempValue[7];
			asdu.data[6]=tempValue[6];
			asdu.data[7]=tempValue[5];
			asdu.data[8]=tempValue[4];
			asdu.data[9]=tempValue[3];
			asdu.data[10]=tempValue[2];
			asdu.data[11]=tempValue[1];
			asdu.data[12]=tempValue[0];
			break;
		default:
			break;
		}


		pCommand->setCommandModbusStatus(eModbusCommandAct);

		m_LastCmd=pCommand;

		/*//原CommApp规约打印保存、先保留着
		if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
		{
			sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Send YT command,device:%d register : %d value:%.3f,factor=%.4f",pCommand->getDestinationId(),startAddr,dwVal,pYTData[curYtIndex].factor);
		//	OnShowMsg(m_LastMsg,strlen(m_LastMsg)+1);
		}
		if(true)
		{
			sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Send YT command,device:%d register : %d value:%.3f,factor=%.4f",pCommand->getDestinationId(),startAddr,dwVal,pYTData[curYtIndex].factor);
		//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1,pCommand);
		}
		*/
		//新规约打印
		sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Send YT command,device:%d register : %d value:%.3f,factor=%.4f",pCommand->getDestinationId(),startAddr,dwVal,pYTData[curYtIndex].factor);
		SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);


		pYTData[curYtIndex].fValue = (float)dwVal;
		pYTData[curYtIndex].exectime = time(NULL);
		pYTData[curYtIndex].finishtime = 0;
		pYTData[curYtIndex].execresult = 0;

		return sendData(&asdu,packLen);
	}
	return true;
}

bool Q_protocolModbusMaster::handelFrameYK(Q_protocolModbusMasterCommand* pCommand)
{
	TRACE("Q_protocolModbusMaster::handelFrameYK tttttttttttttttttttttttttttttttttttttttttt\n");

	if(pCommand->getCommandModbusStatus() == eModbusCommandSelect)
	{
		int startAddr=pCommand->getObjectIndex();

		int firstcode=0;

		if(m_PrintSetting!=NULL)
		{
			firstcode=m_PrintSetting->getSyncFirstCode();
		}

		Q_MODBUS_ASDU2 asdu;
		memset(&asdu,0,sizeof(asdu));
		asdu.header.codeHigh=firstcode;
		asdu.header.codeLow=MODBUS_SYNC_CODE2;
		asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
		asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
		asdu.addr=pCommand->getDestinationId()&0xFF;
		asdu.funcode=MODBUS_YK;//写单个线圈

		TRACE("Q_protocolModbusMaster::handelFrameYK asdu.addr=%d, startAddr=%d ttttttttttttttttttttttttt\n",asdu.addr,startAddr);

		int  packLen=0x06;

		asdu.header.lenHigh=(packLen>>8) &0xFF;
		asdu.header.lenLow= packLen&0xFF;

		asdu.data[0]=startAddr>>8 &0xFF;
		asdu.data[1]=startAddr&0xFF;

		bool bVal=pCommand->getIntValue()==0? false:true;

		if (true==bVal)
		{
			asdu.data[2]=0xff;
			asdu.data[3]=0x00;
			// 			asdu.data[4]=0x0;
			// 			asdu.data[5]=0x0;
		}
		else
		{
			asdu.data[2]=0x00;
			asdu.data[3]=0x00;
			// 			asdu.data[4]=0x0;
			// 			asdu.data[5]=0x0;
		}

		pCommand->setCommandModbusStatus(eModbusCommandAct);

		m_LastCmd=pCommand;

		/*//原CommApp规约打印保存、先保留着
		if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
		{
			sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Send YK command,device:%d register : %d value:%d",pCommand->getDestinationId(),startAddr,bVal);
		//	OnShowMsg(m_LastMsg,strlen(m_LastMsg)+1);
		}

		if (m_bInfoMsg)
		{
			sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Send YK command,device:%d register : %d value:%d",pCommand->getDestinationId(),startAddr,bVal);
		//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
		}
		*/

		//新规约打印
		sprintf_s(m_LastMsg,Q_FRAME_CONST128,"Send YK command,device:%d register : %d value:%d",pCommand->getDestinationId(),startAddr,bVal);
		SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);


		//Q_dataManager* p = _GlobalData.getDataManager();
		Q_RECORDDATATAG dataArray;
		dataArray.channelID=m_pProcotolConfig->getProtocolSetting()->getChannelID();
		dataArray.device=asdu.addr;
		dataArray.no=startAddr;
		dataArray.bValue=bVal;//tjs 2015-07-16 add

		m_pMgr->handleMasterYk(&dataArray);
		
		return sendData(&asdu,packLen);
	}
	return true;
}
bool Q_protocolModbusMaster::makeFrameEx(Q_protocolModbusMasterCommand* pCommand)
{
	if(pCommand != NULL)
	{
		switch(pCommand->getCommandModbusType())
		{
		case eModbusCommandRemoteAdjust:
			{
				handleFrameYT(pCommand);
				break;
			}
		default:
			break;
		}
	}
	return false;

}

bool Q_protocolModbusMaster::terminateProtocol()
{	
	/*Q_protocolCdtStandardSetting* pSetting = (Q_protocolCdtStandardSetting*)m_pProcotolConfig->getProtocolSetting();
	pSetting->getTagsManager()->terminateTags();*/
	//因为发现当随机断客户端时，锁没有释放，于是就不处理了。
	/*  Q_dataManager* p = _GlobalData.getDataManager();
	p->LeaveMutex();
	TRACE(" --- terminate leave mutex ---\n");*/
	m_PrintSetting=NULL;
	return true;
}

bool Q_protocolModbusMaster::startProcess()
{
	printf("Q_protocolModbusMaster::startProcess()\n");
	m_eProtocolStatus = eCommonStatusNormal;
	if (Q_commonUtility::isBigEndian())
	{
		m_cpuEndian=eBigEndian;
	}
	else
	{
		m_cpuEndian=eLittleEndian;
	}
	m_LastCmd=NULL;



	m_PrintSetting= (Q_protocolModbusMasterSetting *)m_pProcotolConfig->getProtocolSetting();

	if(m_PrintSetting!=NULL)
	{
		//新规约打印
		sprintf_s(m_LastMsg,sizeof(m_LastMsg),"初使化开始...");
		SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

		m_FloatUpSeq=m_PrintSetting->getFloatUpSeq();
		m_YtMethod=m_PrintSetting->getYTMethod();
		m_IntUpSeq=m_PrintSetting->getIntUpSeq();
		m_Int2BytesUpSeq=m_PrintSetting->getInt2BytesUpSeq();
		int n=m_PrintSetting->getAlgCmdCount();//得到读遥测个数
		if(n>0)
		{

			Q_protocolModbusMasterCommand* pCommand = new Q_protocolModbusMasterCommand();
			pCommand->setCommonType(eCommandTypeCycle);
			pCommand->setDestinationId(m_PrintSetting->m_vCommand[0].device);
			pCommand->setMaxTryCount(m_PrintSetting->getPerTime());	
			pCommand->setObjectIndex(m_PrintSetting->m_vCommand[0].nBegin);
			pCommand->setIntValue(m_PrintSetting->m_vCommand[0].nCount);

			//update by tjs 2015.04.20 
			pCommand->setYcRequireType(m_PrintSetting->m_vCommand[0].nType);

			//2017-1-10 增加功能码值的请求
			if(m_PrintSetting->m_vCommand[0].code!=0)
			{
				pCommand->setFuncCode(m_PrintSetting->m_vCommand[0].code);
			}
			else
			{
				pCommand->setFuncCode(m_PrintSetting->getReadRegisterCode());
			}
			//end 2017-1-10
			pCommand->m_curIdx=0;

			pCommand->setCommandModbusType(eModbusCommandMeasure);
			pCommand->setCommandModbusStatus(eModbusCommandSelect);

			m_pProcotolConfig->getProtocolCommandManager()->addCommand(pCommand);
		}
		else if (m_PrintSetting->getPntCmdCount()>0)
		{
			Q_protocolModbusMasterCommand* pCommand = new Q_protocolModbusMasterCommand();
			pCommand->setCommonType(eCommandTypeCycle);

			pCommand->setDestinationId(m_PrintSetting->m_vYXCommand[0].device);
			pCommand->setMaxTryCount(m_PrintSetting->getPerTime());

			pCommand->setObjectIndex(m_PrintSetting->m_vYXCommand[0].nBegin);
			pCommand->setIntValue(m_PrintSetting->m_vYXCommand[0].nCount);

			pCommand->setPointType(m_PrintSetting->m_vYXCommand[0].nCount);


			//2017-1-10 增加功能码值的请求
			if(m_PrintSetting->m_vYXCommand[0].code!=0)
			{
				pCommand->setFuncCode(m_PrintSetting->m_vYXCommand[0].code);
			}
			else
			{
				pCommand->setFuncCode(m_PrintSetting->getReadYXRegisterCode());
			}
			//end 2017-1-10

			pCommand->m_curIdx=0;

			pCommand->setCommandModbusType(eModbusCommandPoint);
			pCommand->setCommandModbusStatus(eModbusCommandSelect);

			m_pProcotolConfig->getProtocolCommandManager()->addCommand(pCommand);
		}
	}

	printf("Q_protocolModbusMaster::startProcess() eee\n");
	return true;
}
bool Q_protocolModbusMaster::stopProcess()
{
	if (m_PrintSetting!=NULL)
	{
        /*//原CommApp规约打印保存、先保留着
		if(m_PrintSetting->getPrintPrompt())
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"断开链接...");
		//	OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
		}
		if (m_bInfoMsg)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"断开链接...");
		//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
		}
		*/

		//新规约打印
		sprintf_s(m_LastMsg,sizeof(m_LastMsg),"断开链接...");
		SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

	}

	//Q_dataManager* p = _GlobalData.getDataManager();
	LPQ_ANALOGUEDATATAG pYC = m_pMgr->getAnalogData();
	LPQ_DIGITALEDATATAG pYX = m_pMgr->getDigitalData();
	if (NULL!=pYC && m_PrintSetting->getAnalogEnd() < m_pMgr->getAnalogCount())
	{
		for (int i = m_PrintSetting->getAnalogBegin();i<m_PrintSetting->getAnalogEnd()+1;i++)
		{
			if (pYC[i].datasource!=2)
			{
				pYC[i].quality = 2;
			}
		}
	}

	if (NULL!=pYX && m_PrintSetting->getPointEnd() < m_pMgr->getDigitalCount())
	{
		for (int i = m_PrintSetting->getPointBegin();i<m_PrintSetting->getPointEnd()+1;i++)
		{
			if (pYX[i].datasource!=2)
			{
				pYX[i].quality = 2;
			}
		}
	}
	m_eProtocolStatus = eCommonStatusNull;
	return true;
}
bool Q_protocolModbusMaster::fillTagInformation(LPQ_RECORDDATATAG pData,int nAdr,int nDevID)
{
	//Q_dataManager* p = _GlobalData.getDataManager();
	int nAddrBegin=0;
	int nAddrEnd=0;
	if(pData->eDataType!=eDataTypeBit)
	{
		if(m_PrintSetting!=NULL)
		{
			nAddrBegin=m_PrintSetting->getAnalogBegin();
			nAddrEnd=m_PrintSetting->getAnalogEnd();

		}

		LPQ_ANALOGUEDATATAG alg=m_pMgr->findAlgTag(nDevID,nAdr,nAddrBegin,nAddrEnd);
		if(alg != NULL)
		{
			pData->nIndex =alg->nIndex;
			return true;
		}
	}
	else
	{
		if(m_PrintSetting!=NULL)
		{
			nAddrBegin=m_PrintSetting->getPointBegin();
			nAddrEnd=m_PrintSetting->getPointEnd();
		}
		LPQ_DIGITALEDATATAG  dig=m_pMgr->findDigTag(nDevID,nAdr,nAddrBegin,nAddrEnd);
		if(dig!=NULL)
		{
			pData->nIndex = dig->nIndex;
			return true;
		}

	}
	return false;
}

//命令轮询效果：alg0-alg1-alg2 - dig0-dig1 - alg0-alg1-alg2 - dig0-dig1
//alg命令、dig命令的个数、配置，从xml配置文件中读取。
void Q_protocolModbusMaster::setNextAnalog(Q_protocolModbusMasterCommand* cmd)
{
	//等待下一个执行时间
	//alg0123 dig012 alg0123 dig012

	if(m_PrintSetting->getAlgCmdCount()!=0 && cmd->m_curIdx+1!=m_PrintSetting->getAlgCmdCount())
	{
		cmd->m_curIdx+=1;
		cmd->setCommandModbusStatus(eModbusCommandSelect);
		cmd->setCommandModbusType(eModbusCommandMeasure);
		cmd->setObjectIndex(m_PrintSetting->m_vCommand[cmd->m_curIdx].nBegin);
		cmd->setIntValue(m_PrintSetting->m_vCommand[cmd->m_curIdx].nCount);
		cmd->setDestinationId(m_PrintSetting->m_vCommand[cmd->m_curIdx].device);

		cmd->setYcRequireType(m_PrintSetting->m_vCommand[cmd->m_curIdx].nType);//update by tjs 2015.04.20
		
		//2017-1-10 增加请求的功能码配置 begin
		if(m_PrintSetting->m_vCommand[cmd->m_curIdx].code>0)
		  cmd->setFuncCode(m_PrintSetting->m_vCommand[cmd->m_curIdx].code);
		else
			cmd->setFuncCode(m_PrintSetting->getReadRegisterCode());
		//2017-1-10 增加请求的功能码配置 end
		//if(m_PrintSetting->m_vCommand[cmd->m_curIdx].nType==1)
		//{//float
		//	cmd->setSwitchValue(true);
		//}
		//else if (m_PrintSetting->m_vCommand[cmd->m_curIdx].nType==3)
		//{
		//	cmd->setSwitchValue(false);
		//	cmd->setBMixed(true);
		//}
		//else
		//{
		//	cmd->setSwitchValue(false);
		//	cmd->setBMixed(false);
		//}
	}
	else
	{
		cmd->m_curIdx=0;

		int n=m_PrintSetting->getPntCmdCount();
		if(n>0)
		{
			cmd->setDestinationId(m_PrintSetting->m_vYXCommand[0].device);
			cmd->setObjectIndex(m_PrintSetting->m_vYXCommand[0].nBegin);
			cmd->setIntValue(m_PrintSetting->m_vYXCommand[0].nCount);
			cmd->setPointType(m_PrintSetting->m_vYXCommand[0].nType);


			//2017-1-10 增加请求的功能码配置 begin
			if(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].code>0)
				cmd->setFuncCode(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].code);
			else
				cmd->setFuncCode(m_PrintSetting->getReadYXRegisterCode());
			//2017-1-10 增加请求的功能码配置 end

			cmd->setCommandModbusType(eModbusCommandPoint);
			cmd->setCommandModbusStatus(eModbusCommandSelect);

		}
		else
		{
			cmd->setObjectIndex(m_PrintSetting->m_vCommand[cmd->m_curIdx].nBegin);
			cmd->setIntValue(m_PrintSetting->m_vCommand[cmd->m_curIdx].nCount);
			cmd->setDestinationId(m_PrintSetting->m_vCommand[cmd->m_curIdx].device);

			cmd->setYcRequireType(m_PrintSetting->m_vCommand[cmd->m_curIdx].nType);//update by tjs 2015.04.20

			//2017-1-10 增加请求的功能码配置 begin
			if(m_PrintSetting->m_vCommand[cmd->m_curIdx].code>0)
				cmd->setFuncCode(m_PrintSetting->m_vCommand[cmd->m_curIdx].code);
			else
				cmd->setFuncCode(m_PrintSetting->getReadRegisterCode());
			/*if(m_PrintSetting->m_vCommand[cmd->m_curIdx].nType==1)
			{
			cmd->setSwitchValue(true);
			}
			else if (m_PrintSetting->m_vCommand[cmd->m_curIdx].nType==3)
			{
			cmd->setSwitchValue(false);
			cmd->setBMixed(true);
			}
			else
			{
			cmd->setSwitchValue(false);
			cmd->setBMixed(false);
			}*/
			cmd->setCommandModbusType(eModbusCommandMeasure);
			cmd->setCommandModbusStatus(eModbusCommandSelect);
		}
	}
}

void Q_protocolModbusMaster::setNextDigital(Q_protocolModbusMasterCommand* cmd)
{

	if(m_PrintSetting->getPntCmdCount()!=0 && cmd->m_curIdx+1!=m_PrintSetting->getPntCmdCount())
	{
		cmd->m_curIdx+=1;
		cmd->setDestinationId(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].device);
		cmd->setObjectIndex(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].nBegin);
		cmd->setIntValue(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].nCount);
		cmd->setPointType(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].nType);

		cmd->setCommandModbusType(eModbusCommandPoint);
		cmd->setCommandModbusStatus(eModbusCommandSelect);


		//2017-1-10 增加请求的功能码配置 begin
		if(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].code>0)
			cmd->setFuncCode(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].code);
		else
			cmd->setFuncCode(m_PrintSetting->getReadYXRegisterCode());
		//2017-1-10 增加请求的功能码配置 end

	}
	else
	{
		cmd->m_curIdx=0;
		int n=m_PrintSetting->getAlgCmdCount();
		if(n>0)
		{
			cmd->setDestinationId(m_PrintSetting->m_vCommand[0].device);
			cmd->setObjectIndex(m_PrintSetting->m_vCommand[0].nBegin);
			cmd->setIntValue(m_PrintSetting->m_vCommand[0].nCount);

			cmd->setYcRequireType(m_PrintSetting->m_vCommand[0].nType);//update by tjs 2015.04.20


			//2017-1-10 增加请求的功能码配置 begin
			if(m_PrintSetting->m_vCommand[cmd->m_curIdx].code>0)
				cmd->setFuncCode(m_PrintSetting->m_vCommand[cmd->m_curIdx].code);
			else
				cmd->setFuncCode(m_PrintSetting->getReadRegisterCode());

			//if(m_PrintSetting->m_vCommand[0].nType==1)
			//	cmd->setSwitchValue(true);//FLOAT类型
			//else if (m_PrintSetting->m_vCommand[0].nType==3)
			//{
			//	cmd->setSwitchValue(false);//INT类型
			//	cmd->setBMixed(true);
			//}
			//else
			//{
			//	cmd->setSwitchValue(false);//INT类型
			//	cmd->setBMixed(false);
			//}


			cmd->setCommandModbusType(eModbusCommandMeasure);
			cmd->setCommandModbusStatus(eModbusCommandSelect);

		}
		else
		{
			cmd->setDestinationId(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].device);
			cmd->setObjectIndex(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].nBegin);
			cmd->setIntValue(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].nCount);
			cmd->setPointType(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].nType);

			cmd->setCommandModbusType(eModbusCommandPoint);
			cmd->setCommandModbusStatus(eModbusCommandSelect);

			//2017-1-10 增加请求的功能码配置 begin
			if(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].code>0)
				cmd->setFuncCode(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].code);
			else
				cmd->setFuncCode(m_PrintSetting->getReadYXRegisterCode());
			//2017-1-10 增加请求的功能码配置 end
		}
		//更改读遥信数据
	}
}
//处理错误消息
bool Q_protocolModbusMaster::explainError(Q_MODBUS_ASDU *asdu)
{
	if(m_LastCmd!=NULL)
	{
		Q_ModbusMasterCommandManager* pManager = (Q_ModbusMasterCommandManager*)m_pProcotolConfig->getProtocolCommandManager();
		if(pManager != NULL)
		{ 
			//执行失败
			//移除上一个命令
			if(m_LastCmd->getCommonType()!=eCommandTypeCycle)
			{
				if (eModbusCommandRemoteAdjust==m_LastCmd->getCommandModbusType() 
					&& eModbusCommandAct==m_LastCmd->getCommandModbusStatus())
				{
					//Q_dataManager* p = _GlobalData.getDataManager();
					LPQ_YTEDATATAG pYTData = m_pMgr->getYTData();
					for (int i=0;i<m_pMgr->getYTCount();i++)
					{
						if (m_pProcotolConfig->getProtocolSetting()->getChannelID()  ==pYTData[i].channelid && pYTData[i].device==m_LastCmd->getDestinationId() && pYTData[i].no==m_LastCmd->getObjectIndex())
						{
							pYTData[i].finishtime = time(NULL);
							pYTData[i].execresult = 3;
							break;
						}
					}

                    /*//原CommApp规约打印保存、先保留着
					if (NULL!=m_PrintSetting && m_PrintSetting->getPrintPrompt())
					{
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"YT exec failed ,device=%d,no=%d,value=%f\n",m_LastCmd->getDestinationId(),m_LastCmd->getObjectIndex(),m_LastCmd->getAnalogValue());
					//	OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
					}
					if (m_bInfoMsg)
					{
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"YT exec failed ,device=%d,no=%d\n",m_LastCmd->getDestinationId(),m_LastCmd->getObjectIndex());
					//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
					}
					*/

					//新规约打印
					sprintf_s(m_LastMsg,sizeof(m_LastMsg),"YT exec failed ,device=%d,no=%d\n",m_LastCmd->getDestinationId(),m_LastCmd->getObjectIndex());
					SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

				}
				else if (eModbusCommandRemoteControl==m_LastCmd->getCommandModbusType() 
					&& eModbusCommandAct==m_LastCmd->getCommandModbusStatus())
				{
					//Q_dataManager* p = _GlobalData.getDataManager();
					LPQ_YKEDATATAG pYKData = m_pMgr->getYKData();
					for (int i=0;i<m_pMgr->getYKCount();i++)
					{
						if (m_pProcotolConfig->getProtocolSetting()->getChannelID()  ==pYKData[i].channelid && pYKData[i].device==m_LastCmd->getDestinationId() && pYKData[i].no==m_LastCmd->getObjectIndex())
						{
							pYKData[i].finishtime = time(NULL);
							pYKData[i].execresult = 3;
							break;
						}
					}

                     /*//原CommApp规约打印保存、先保留着
					if (NULL!=m_PrintSetting && m_PrintSetting->getPrintPrompt())
					{
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"YK exec failed ,device=%d,no=%d,value=%d\n",m_LastCmd->getDestinationId(),m_LastCmd->getObjectIndex(),m_LastCmd->getIntValue());
					//	OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
					}
					if (m_bInfoMsg)
					{
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"YK exec failed ,device=%d,no=%d,value=%d\n",m_LastCmd->getDestinationId(),m_LastCmd->getObjectIndex(),m_LastCmd->getIntValue());
					//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
					}
					*/

					//新规约打印
					sprintf_s(m_LastMsg,sizeof(m_LastMsg),"YK exec failed ,device=%d,no=%d,value=%d\n",m_LastCmd->getDestinationId(),m_LastCmd->getObjectIndex(),m_LastCmd->getIntValue());
					SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

				}
				pManager->removeCommand(m_LastCmd);
				m_LastCmd=NULL;
			}
			else
			{
				//遥测、遥信
				if(m_LastCmd->getCommandModbusType()==eModbusCommandMeasure)
				{
					//
					if(m_PrintSetting!=NULL)
					{
						char chErr[64] = {0};
						if ((asdu->funcode & 0x80) == m_PrintSetting->getReadRegisterCode())
						{
							sprintf_s(chErr,sizeof(chErr),"读取的寄存器范围异常");
						}
						else
						{
							sprintf_s(chErr,sizeof(chErr),"未知");
						}

						/*//原CommApp规约打印保存、先保留着
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Modbus异常响应，device=%d,功能码%02x,%s\n",asdu->addr,asdu->funcode,chErr);
						//OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
						if (m_bInfoMsg)
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Modbus异常响应，device=%d,功能码%02x,%s\n",asdu->addr,asdu->funcode,chErr);
						//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						*/

						//新规约打印
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Modbus异常响应，device=%d,功能码%02x,%s\n",asdu->addr,asdu->funcode,chErr);
						SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

					}
					m_LastCmd->setCommandModbusStatus(eModbusCommandActCon);

					if(m_PrintSetting->getAlgCmdCount()>0)
					{
						if(m_LastCmd->m_curIdx+1!=m_PrintSetting->getAlgCmdCount())
							m_LastCmd->m_curIdx+=1;
						else
							m_LastCmd->m_curIdx=0;
						m_LastCmd->setDestinationId(m_PrintSetting->m_vCommand[m_LastCmd->m_curIdx].device);
						m_LastCmd->setObjectIndex(m_PrintSetting->m_vCommand[m_LastCmd->m_curIdx].nBegin);
						m_LastCmd->setIntValue(m_PrintSetting->m_vCommand[m_LastCmd->m_curIdx].nCount);
					}
				}
				else if(m_LastCmd->getCommandModbusType()==eModbusCommandPoint)
				{
					//
					if(m_PrintSetting!=NULL)
					{
						char chErr[64] = {0};
						if ((asdu->funcode&0x80)==m_PrintSetting->getReadYXRegisterCode())
						{
							sprintf_s(chErr,sizeof(chErr),"读取的线圈范围异常");
						}
						else
						{
							sprintf_s(chErr,sizeof(chErr),"未知");
						}

						/*//原CommApp规约打印保存、先保留着
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Modbus异常响应，device=%d,功能码%02x,%s\n",asdu->addr,asdu->funcode,chErr);
						//OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
						if (m_bInfoMsg)
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Modbus异常响应，device=%d,功能码%02x,%s\n",asdu->addr,asdu->funcode,chErr);
						//	OnInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						*/

						//新规约打印
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Modbus异常响应，device=%d,功能码%02x,%s\n",asdu->addr,asdu->funcode,chErr);
						SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

					}
					m_LastCmd->setCommandModbusStatus(eModbusCommandActCon);

					if(m_PrintSetting->getAlgCmdCount()>0)
					{
						//if(m_LastCmd->m_curIdx+1!=m_PrintSetting->getAlgCmdCount())//会崩溃
						if(m_LastCmd->m_curIdx+1<m_PrintSetting->getAlgCmdCount())//
							m_LastCmd->m_curIdx+=1;
						else
							m_LastCmd->m_curIdx=0;
						m_LastCmd->setDestinationId(m_PrintSetting->m_vCommand[m_LastCmd->m_curIdx].device);
						m_LastCmd->setObjectIndex(m_PrintSetting->m_vCommand[m_LastCmd->m_curIdx].nBegin);
						m_LastCmd->setIntValue(m_PrintSetting->m_vCommand[m_LastCmd->m_curIdx].nCount);
					}
				}
			}
		}
	}
	return true;
}


void Q_protocolModbusMaster::SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen)
{
	CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
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

	if (nmsgtype == eMsgTypeTx || nmsgtype == eMsgTypeRx)//发送消息、接收消息
	{
		if (ElementTUnit.iRecPacketFlg != 0)//报文信息传输开启
		{
			bSend = true;
		}
		else
		{
			bSend = false;
			return;
		}
	}
	else if (nmsgtype == eMsgTypeMsg || nmsgtype == eMsgTypeError || nmsgtype == eMsgTypeCh)//提示信息、信息值、错误消息,数据解释、规约解析
	{
		if (ElementTUnit.iRecPromptFlg != 0)//提示信息传输开启
		{
			bSend = true;
		}
		else
		{
			bSend = false;
			return;
		}
	}

	if (bSend == true)
	{
		//如果不判断运状态就发命令，会把消息总线给堆满了
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
				//进程不运行，退出
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
				//2016-10-28 tjs 因为不应该给备都发送消息
				

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
			//2016-10-28 tjs 因为不应该给备都发送消息
			

			m_rtdbInf->getMainDb()->SendMessageTo(pProcess->ID32,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
			/*char strMsg[200] = {0};
			sprintf(strMsg,"MsgPrint:stid = %d msgtype id = %d message = %s",MsgPrint.ichannelid,MsgPrint.msgtype,MsgPrint.msg);*/
		}
	}
}

bool Q_protocolModbusMaster::explainFrameYCFloat(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr)
{
	//处理四字节FLOAT类型
	int nByteCount=0;

	nByteCount=data[0]&0xFF;

	int pi=1;

	Q_RECORDDATATAG dataArray;
	dataArray.eDataType=eDataTypeFloat;
	int i=0;

	BYTE tmp[4];

	float vv=0.0f;
	while(pi<nByteCount+1)
	{
		dataArray.channelID=m_PrintSetting->getChannelID();
		dataArray.device=header->addr;
		dataArray.no=startAddr+i;

		memset(tmp,0,sizeof(tmp));

		switch(m_FloatUpSeq)
		{
		case eFloatUpSeqLowFirst:
			tmp[0]=data[pi+3];
			tmp[1]=data[pi+2];
			tmp[2]=data[pi+1];
			tmp[3]=data[pi];
			break;
		case eFloatUpSeq2301:
			tmp[0]=data[pi+2];
			tmp[1]=data[pi+3];
			tmp[2]=data[pi];
			tmp[3]=data[pi+1];
			break;
		case eFloatUpSeq1032:
			tmp[0]=data[pi+1];
			tmp[1]=data[pi];
			tmp[2]=data[pi+3];
			tmp[3]=data[pi+2];
			break;	
		case eFloatUpSeqHighFirst:
			tmp[0]=data[pi];
			tmp[1]=data[pi+1];
			tmp[2]=data[pi+2];
			tmp[3]=data[pi+3];
			break;
		default:
			break;
		}

		memcpy(&vv,tmp,sizeof(vv));
		dataArray.fValue=vv;
		//系数在统一处理
		if(!m_pMgr->recordDataValue(&dataArray,1))
		{				
			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Can't find right YC point in realDB,Please check yc.xml ,Channel=%d,device=%d,addr=%d",m_PrintSetting->getChannelID(),cmd->getDestinationId(),startAddr+i);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);
		}
		pi+=4;
		i+=2;
	}
	return true;
}
bool Q_protocolModbusMaster::explainFrameYCMixed(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr)
{
	//处理四字节FLOAT类型
	int nByteCount=0;

	nByteCount=data[0]&0xFF;

	int pi=1;

	Q_RECORDDATATAG dataArray;
	dataArray.eDataType=eDataTypeFloat;
	int i=0;

	BYTE tmp[4];
	//2017-1-11 
	//入库时设备号用功能码代替功能开启(三一重工用)
	//主要原因是因为三一重工存在设备地址一样、设备内编号不一样的不同记录
	//只是召唤的功能码有差异的记录,他们的寄存器号不唯一，所以特殊改
	int nDeviceAddr=header->addr;
	if(m_PrintSetting->getFuncCodeEqualDeviceFlg())
	{
		if(m_LastCmd!=NULL)
			nDeviceAddr=m_LastCmd->getFuncCode();

	}
	//end
	int lastStep=0;
	while(pi<nByteCount+1)
	{
		//if (fillTagInformation(&dataArray,startAddr+i,header->addr)==true)
		//{
		dataArray.channelID=m_PrintSetting->getChannelID();
		//dataArray.device=header->addr;
		dataArray.device=nDeviceAddr;
		dataArray.no=startAddr+i;
		dataArray.nIndex=startAddr+i-m_PrintSetting->getAnalogBegin();

		//AnalogTable pAnalog; 
		//if(m_pMgr->getRtdbInf()->rtdb_get_analog_iNo(dataArray.nIndex,&pAnalog))
		//	m_pMgr->
		//tjs 2017-1-10 begin因为发现以前樊硕这里写错了，所以更改一下，但没有测试过
		stdevKey vkey;
		vkey.stid=m_PrintSetting->getChannelID();
		vkey.device=nDeviceAddr;
		vkey.no=startAddr+i;

		int v_index=0;
		BOOL ret=m_pMgr->getRtdbInf()->getScadaDb()->GetAnalogIndexByKey(vkey,v_index);
		//tjs 2017-1-10 begin因为发现以前樊硕这里写错了，所以更改一下，但没有测试过

		SCADA_Analog scadaalg;
		BOOL flg=m_pMgr->getRtdbInf()->getScadaDb()->GetElementByIndex(SCADA_TAB_Analog_ID,v_index,&scadaalg);
		if(flg)
		{
			memset(tmp,0,sizeof(tmp));
			//if (pAnalog.ID32==eDataTypeFloat)//四字节 
			//原来的代码这不是瞎写吗
			if (scadaalg.iDataType==eDataTypeFloat)//四字节 
			{
				if (pi > nByteCount - 3)
				{
					//新规约打印
					sprintf_s(m_LastMsg,sizeof(m_LastMsg),"报文长度与配置不符，请检查模拟量表中device=%d,no=%d到no=%d的type或召唤数量。",nDeviceAddr,startAddr,startAddr+i);
					SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);
					break;
				}

				switch(m_FloatUpSeq)
				{
				case eFloatUpSeqLowFirst:
					tmp[0]=data[pi+3];
					tmp[1]=data[pi+2];
					tmp[2]=data[pi+1];
					tmp[3]=data[pi];
					break;
				case eFloatUpSeq2301:
					tmp[0]=data[pi+2];
					tmp[1]=data[pi+3];
					tmp[2]=data[pi];
					tmp[3]=data[pi+1];
					break;
				case eFloatUpSeq1032:
					tmp[0]=data[pi+1];
					tmp[1]=data[pi];
					tmp[2]=data[pi+3];
					tmp[3]=data[pi+2];
					break;	
				case eFloatUpSeqHighFirst:
					tmp[0]=data[pi];
					tmp[1]=data[pi+1];
					tmp[2]=data[pi+2];
					tmp[3]=data[pi+3];
					break;
				default:
					break;
				}


				float vv=0;
				memcpy(&vv,tmp,sizeof(vv));
				dataArray.eDataType=eDataTypeFloat;
				dataArray.fValue=vv;//不需要处理系数，统一处理
				m_pMgr->recordDataValue(&dataArray,1);

				pi+=4;
				i+=2;
				lastStep=4;

			}
			else if (scadaalg.iDataType==eDataTypeInt)//四字节
			{
				switch(m_IntUpSeq)
				{
				case eIntUpSeqHighFirst:
					tmp[0]=data[pi+3];
					tmp[1]=data[pi+2];
					tmp[2]=data[pi+1];
					tmp[3]=data[pi];
					break;
				case eIntUpSeqLowFirst:
					tmp[0]=data[pi];
					tmp[1]=data[pi+1];
					tmp[2]=data[pi+2];
					tmp[3]=data[pi+3];
					break;
				case eIntUpSeq2301:
					tmp[0]=data[pi+2];
					tmp[1]=data[pi+3];
					tmp[2]=data[pi];
					tmp[3]=data[pi+1];
					break;
				case eIntUpSeq1032:
					tmp[0]=data[pi+1];
					tmp[1]=data[pi];
					tmp[2]=data[pi+3];
					tmp[3]=data[pi+2];
					break;	
				default:
					break;
				}
				int n=0;
				memcpy(&n,tmp,sizeof(n));
				dataArray.nValue =n;
				dataArray.eDataType=eDataTypeInt;
				m_pMgr->recordDataValue(&dataArray,1);

				pi+=4;
				lastStep=4;
				i+=2;
			}	
			else if (scadaalg.iDataType==eDataTypeBit)//两字节
			{
				switch(m_Int2BytesUpSeq)
				{
				case eIntUpSeqHighFirst:
					tmp[0]=data[pi+1];
					tmp[1]=data[pi];
					break;
				case eIntUpSeqLowFirst:
					tmp[0]=data[pi];
					tmp[1]=data[pi+1];
					break;
				default:
					break;
				}
				short n=0;
				memcpy(&n,tmp,sizeof(n));
				dataArray.fValue = n;
				dataArray.eDataType=eDataTypeFloat;
				m_pMgr->recordDataValue(&dataArray,1);

				pi+=2;
				lastStep=2;
				i+=1;
			}	
		}
		else
		{
			pi+=lastStep;//防止死锁的问题
			i+=1;
		}

	}
	return true;
}
bool Q_protocolModbusMaster::explainFrameYCInt(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr)
{
	//处理四字节FLOAT类型
	int nByteCount=0;

	nByteCount=data[0]&0xFF;

	int pi=1;

	Q_RECORDDATATAG dataArray;
	dataArray.eDataType=eDataTypeFloat;
	int i=0;

	BYTE tmp[4];

	//int 类型
	int nDeviceAddr=header->addr;
	if(m_PrintSetting->getFuncCodeEqualDeviceFlg())
	{
		if(m_LastCmd!=NULL)
			nDeviceAddr=m_LastCmd->getFuncCode();

	}

	while(pi<nByteCount+1)
	{
		//dataArray.channelID=m_PrintSetting->getChannelID();
		//dataArray.device=header->addr;
		////dataArray.no=startAddr+i;
		////dataArray.nIndex=startAddr+i-m_PrintSetting->getAnalogBegin();
		//dataArray.nIndex=startAddr+i;

		dataArray.channelID=m_PrintSetting->getChannelID();
		dataArray.device=nDeviceAddr;
		dataArray.no=startAddr+i;
		memset(tmp,0,sizeof(tmp));


		switch(m_IntUpSeq)
		{
		case eIntUpSeqHighFirst:
			tmp[0]=data[pi+1];
			tmp[1]=data[pi];
			break;
		case eIntUpSeqLowFirst:
			tmp[0]=data[pi];
			tmp[1]=data[pi+1];
			break;
		default:
			break;
		}

		short n=0;
		memcpy(&n,tmp,sizeof(n));
		dataArray.eDataType=eDataTypeInt;
		dataArray.nValue=n;

		if(!m_pMgr->recordDataValue(&dataArray,1))
		{
			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Can't find right YC point in realDB,Please check yc.xml ,Channel=%d,device=%d,addr=%d",m_PrintSetting->getChannelID(),cmd->getDestinationId(),startAddr+i);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

		}	
		pi+=2;
		i+=1;
	}
	return true;
}

bool Q_protocolModbusMaster::explainFrameYCDouble8(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr)
{
	//处理四字节FLOAT类型
	int nByteCount=0;

	nByteCount=data[0]&0xFF;

	int pi=1;

	Q_RECORDDATATAG dataArray;
	dataArray.eDataType=eDataTypeFloat;
	int i=0;
    BYTE tempValue[8];
				//处理8字节FLOAT类型
				while(pi<nByteCount+1)
				{

					dataArray.channelID=m_PrintSetting->getChannelID();
					//dataArray.device=header->addr;
					//dataArray.no=startAddr+i;
					dataArray.nIndex=startAddr+i-m_PrintSetting->getAnalogBegin();
					memset(tempValue,0,sizeof(tempValue));


					switch(m_FloatUpSeq)
					{
					case eFloatUpSeqHighFirst:
						tempValue[0]=data[pi];
						tempValue[1]=data[pi+1];
						tempValue[2]=data[pi+2];
						tempValue[3]=data[pi+3];
						tempValue[4]=data[pi+4];
						tempValue[5]=data[pi+5];
						tempValue[6]=data[pi+6];
						tempValue[7]=data[pi+7];
						break;
					case eFloatUpSeqLowFirst:
						tempValue[0]=data[pi+7];
						tempValue[1]=data[pi+6];
						tempValue[2]=data[pi+5];
						tempValue[3]=data[pi+4];
						tempValue[4]=data[pi+3];
						tempValue[5]=data[pi+2];
						tempValue[6]=data[pi+1];
						tempValue[7]=data[pi];
						break;
					default:
						break;
					}

					double vv=0;
					memcpy(&vv,tempValue,sizeof(vv));

					dataArray.eDataType=eDataTypeFloat;

					dataArray.fValue=(float)vv;

					if(!m_pMgr->recordDataValue(&dataArray,1))
					{
						/*//原CommApp规约打印保存、先保留着
						if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Can't find right YC point in realDB,Please check yc.xml ,Channel=%d,device=%d,addr=%d",m_PrintSetting->getChannelID(),cmd->getDestinationId(),startAddr+i);
						//	OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
						}
						if(m_bNomalInfoMsg)
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Can't find right YC point in realDB,Please check yc.xml ,Channel=%d,device=%d,addr=%d",m_PrintSetting->getChannelID(),cmd->getDestinationId(),startAddr+i);
						//	OnNomalInfoMsg(m_LastMsg,strlen(m_LastMsg)+1);
						}
						*/

						//新规约打印
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Can't find right YC point in realDB,Please check yc.xml ,Channel=%d,device=%d,addr=%d",m_PrintSetting->getChannelID(),cmd->getDestinationId(),startAddr+i);
						SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);
					}

					pi+=8;
					i+=4;
				}
	return true;
}

bool Q_protocolModbusMaster::explainFrameYCHuaWei(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr)
{
	//处理四字节FLOAT类型
	int nByteCount=0;

	nByteCount=data[0]&0xFF;

	int pi=1;

	Q_RECORDDATATAG dataArray;
	dataArray.eDataType=eDataTypeFloat;
	int i=0;

	BYTE tmp[4];

	//int 类型
	nByteCount = len-2;//华为逆变器的modbusTcp子站报文里边没有数据字节个数
	pi -= 1; 
	while(pi<nByteCount)
	{
		//dataArray.channelID=m_PrintSetting->getChannelID();
		//dataArray.device=header->addr;
		////dataArray.no=startAddr+i;
		////dataArray.nIndex=startAddr+i-m_PrintSetting->getAnalogBegin();
		//dataArray.nIndex=startAddr+i;

		dataArray.channelID=m_PrintSetting->getChannelID();
		dataArray.device=header->addr;
		dataArray.no=startAddr+i;
		memset(tmp,0,sizeof(tmp));


		switch(m_IntUpSeq)
		{
		case eIntUpSeqHighFirst:
			tmp[0]=data[pi+1];
			tmp[1]=data[pi];
			break;
		case eIntUpSeqLowFirst:
			tmp[0]=data[pi];
			tmp[1]=data[pi+1];
			break;
		default:
			break;
		}

		short n=0;
		memcpy(&n,tmp,sizeof(n));
		dataArray.eDataType=eDataTypeInt;
		dataArray.nValue=n;

		if(!m_pMgr->recordDataValue(&dataArray,1))
		{
			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Can't find right YC point in realDB,Please check yc.xml ,Channel=%d,device=%d,addr=%d",m_PrintSetting->getChannelID(),cmd->getDestinationId(),startAddr+i);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

		}	
		pi+=2;
		i+=1;
	}
	return true;
}

bool Q_protocolModbusMaster::explainFrameYCInt4(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr)
{
	//处理四字节有符号整形类型
	int nByteCount=0;

	nByteCount=data[0]&0xFF;

	int pi=1;

	Q_RECORDDATATAG dataArray;
	dataArray.eDataType=eDataTypeFloat;
	int i=0;

	BYTE tmp[4];
	long vv=0;
	while(pi<nByteCount+1)
	{
		dataArray.channelID=m_PrintSetting->getChannelID();
		dataArray.device=header->addr;
		dataArray.no=startAddr+i;

		memset(tmp,0,sizeof(tmp));

		switch(m_FloatUpSeq)
		{
		case eFloatUpSeqLowFirst:
			tmp[0]=data[pi+3];
			tmp[1]=data[pi+2];
			tmp[2]=data[pi+1];
			tmp[3]=data[pi];
			break;
		case eFloatUpSeq2301:
			tmp[0]=data[pi+2];
			tmp[1]=data[pi+3];
			tmp[2]=data[pi];
			tmp[3]=data[pi+1];
			break;
		case eFloatUpSeq1032:
			tmp[0]=data[pi+1];
			tmp[1]=data[pi];
			tmp[2]=data[pi+3];
			tmp[3]=data[pi+2];
			break;	
		case eFloatUpSeqHighFirst:
			tmp[0]=data[pi];
			tmp[1]=data[pi+1];
			tmp[2]=data[pi+2];
			tmp[3]=data[pi+3];
			break;
		default:
			break;
		}

		//memcpy(&vv,tmp,sizeof(vv));
		vv=0;					
		if(tmp[3]&0x80)//如果是负数,把二进制负数转换为有符号的四字节整形负数
		{
			unsigned long ulVv=0;
			ulVv+=tmp[0];
			ulVv+=tmp[1]*256;
			ulVv+=tmp[2]*256*256;
			ulVv+=tmp[3]*256*256*256;

			ulVv-=1;
			BYTE fanMa[4];//得到负数的反码
			fanMa[0]=(ulVv&0x000000FF);
			fanMa[1]=(ulVv&0x0000FF00) >> 8;
			fanMa[2]=(ulVv&0x00FF0000) >> 16;
			fanMa[3]=(ulVv&0xFF000000) >> 24;

			fanMa[0]=~fanMa[0];//逐位取反，得到负数的绝对值
			fanMa[1]=~fanMa[1];
			fanMa[2]=~fanMa[2];
			fanMa[3]=~fanMa[3];

			ulVv=0;
			ulVv+=fanMa[0];
			ulVv+=fanMa[1]*256;
			ulVv+=fanMa[2]*256*256;
			ulVv+=fanMa[3]*256*256*256;

			vv=-1*ulVv;
		}
		else
		{
			vv+=tmp[0];
			vv+=tmp[1]*256;
			vv+=tmp[2]*256*256;
			vv+=tmp[3]*256*256*256;
		}
		dataArray.fValue=vv;
		//系数在统一处理
		if(!m_pMgr->recordDataValue(&dataArray,1))
		{				
			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Can't find right YC point in realDB,Please check yc.xml ,Channel=%d,device=%d,addr=%d",m_PrintSetting->getChannelID(),cmd->getDestinationId(),startAddr+i);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);
		}
		pi+=4;
		i+=2;
	}
	return true;
}

bool Q_protocolModbusMaster::explainFrameYCUnsignedInt4(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr)
{
	//处理四字节FLOAT类型
	int nByteCount=0;

	nByteCount=data[0]&0xFF;

	int pi=1;

	Q_RECORDDATATAG dataArray;
	dataArray.eDataType=eDataTypeFloat;
	int i=0;

	BYTE tmp[4];

	unsigned long vv=0;
	while(pi<nByteCount+1)
	{
		dataArray.channelID=m_PrintSetting->getChannelID();
		dataArray.device=header->addr;
		dataArray.no=startAddr+i;

		memset(tmp,0,sizeof(tmp));

		switch(m_FloatUpSeq)
		{
		case eFloatUpSeqLowFirst:
			tmp[0]=data[pi+3];
			tmp[1]=data[pi+2];
			tmp[2]=data[pi+1];
			tmp[3]=data[pi];
			break;
		case eFloatUpSeq2301:
			tmp[0]=data[pi+2];
			tmp[1]=data[pi+3];
			tmp[2]=data[pi];
			tmp[3]=data[pi+1];
			break;
		case eFloatUpSeq1032:
			tmp[0]=data[pi+1];
			tmp[1]=data[pi];
			tmp[2]=data[pi+3];
			tmp[3]=data[pi+2];
			break;	
		case eFloatUpSeqHighFirst:
			tmp[0]=data[pi];
			tmp[1]=data[pi+1];
			tmp[2]=data[pi+2];
			tmp[3]=data[pi+3];
			break;
		default:
			break;
		}

		//memcpy(&vv,tmp,sizeof(vv));
		vv=0;
		vv+=tmp[0];
		vv+=tmp[1]*256;
		vv+=tmp[2]*256*256;
		vv+=tmp[3]*256*256*256;
		dataArray.fValue=vv;
		//系数在统一处理
		if(!m_pMgr->recordDataValue(&dataArray,1))
		{				
			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Can't find right YC point in realDB,Please check yc.xml ,Channel=%d,device=%d,addr=%d",m_PrintSetting->getChannelID(),cmd->getDestinationId(),startAddr+i);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);
		}
		pi+=4;
		i+=2;
	}
	return true;
}

bool Q_protocolModbusMaster::explainFrameYCUShort(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr)
{
	//处理四字节FLOAT类型
	int nByteCount=0;

	nByteCount=data[0]&0xFF;

	int pi=1;

	Q_RECORDDATATAG dataArray;
	dataArray.eDataType=eDataTypeFloat;
	int i=0;

	BYTE tmp[4];

	//int 类型
	int nDeviceAddr=header->addr;
	if(m_PrintSetting->getFuncCodeEqualDeviceFlg())
	{
		if(m_LastCmd!=NULL)
			nDeviceAddr=m_LastCmd->getFuncCode();

	}

	while(pi<nByteCount+1)
	{

		dataArray.channelID=m_PrintSetting->getChannelID();
		dataArray.device=nDeviceAddr;
		dataArray.no=startAddr+i;
		memset(tmp,0,sizeof(tmp));


		switch(m_IntUpSeq)
		{
		case eIntUpSeqHighFirst:
			tmp[0]=data[pi+1];
			tmp[1]=data[pi];
			break;
		case eIntUpSeqLowFirst:
			tmp[0]=data[pi];
			tmp[1]=data[pi+1];
			break;
		default:
			break;
		}

		unsigned short n=0;
		memcpy(&n,tmp,sizeof(n));

		dataArray.fValue=(float)n;
		//sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Channel=%d,device=%d,addr=%d,data=%.2f,sdata=%d",m_PrintSetting->getChannelID(),cmd->getDestinationId(),startAddr+i,dataArray.fValue,n);
		//SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

		if(!m_pMgr->recordDataValue(&dataArray,1))
		{
			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Can't find right YC point in realDB,Please check yc.xml ,Channel=%d,device=%d,addr=%d",m_PrintSetting->getChannelID(),cmd->getDestinationId(),startAddr+i);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

		}	
		pi+=2;
		i+=1;
	}
	return true;
}
