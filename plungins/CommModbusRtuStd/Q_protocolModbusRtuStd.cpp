#include "stdafx.h"
#include "gSystemDef.h"
#include "Q_protocolModbusRtuStd.h"
#include "tinyxmlinclude.h"

Q_ProtocolDBQH *g_Protocol=NULL;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
	//int ret;
	TRACE("create_protocol_object_ModbusRTU\n");
	Q_chDownReturn *chRet=new Q_chDownReturn();

	int ctype = v_param->cfg->type;

	int  n_sleepMills=500;//原来是 1000
	//=======
	Q_taskBase* taskTest = new Q_taskBase();
	taskTest->setBackupMode(eCommonBackupCold);
	taskTest->setRtdbInf(v_param->m_pMgr);//2015-06-23

	bool flgPri=true;

	Q_protcolModbusRtuStdSetting *modbusSetting=new Q_protcolModbusRtuStdSetting();
	modbusSetting->setFilePath(v_param->strFilePath);

	modbusSetting->initSetting();//解析xml内容
	printf("v_param->cfg->YCbegin:%d,v_param->cfg->YCend:%d\n", v_param->cfg->YCbegin, v_param->cfg->YCend);
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

	Q_ModbusRtuStdCmdMgr* pCommandList = new Q_ModbusRtuStdCmdMgr();
	channelTest->setCommandList(pCommandList);
	TRACE("Q_serialLink before \r\n");
	//Q_serialLink* linkTcp = new Q_serialLink(); //Q_serialLinkSetting
	//Q_serialLinkSetting* tLinkSetting = new Q_serialLinkSetting();

	TRACE("User Q_serialLink \r\n");
	//char *tmp=NULL;
	//tmp=modbusSetting->getContentID(11,1);
	//if(tmp!=NULL)
	//{
	//	printf("Serial port name :%s\n", tmp);
	//	tLinkSetting->setComPortName(tmp);
	//}
	//tmp=modbusSetting->getContentID(12,1);
	//if(tmp!=NULL)
	//{
	//	tLinkSetting->setBountRate(atoi(tmp));
	//}	
	//tmp=modbusSetting->getContentID(13,1);
	//if(tmp!=NULL)
	//{
	//	tLinkSetting->setDataBit(atoi(tmp));
	//}
	//tmp=modbusSetting->getContentID(14,1);
	//if(tmp!=NULL)
	//{
	//	tLinkSetting->setStopBit(atoi(tmp));
	//}
	//tmp=modbusSetting->getContentID(15,1);
	//if(tmp!=NULL)
	//{
	//	tLinkSetting->setVerifyBit(atoi(tmp));
	//}
	Q_linkConfig* tLinkConfig = new Q_linkConfig();
	//tLinkConfig->setLinkSetting(tLinkSetting);
	//tLinkConfig->setReceiveBuffer(bufferReceive);
	//tLinkConfig->setSendBuffer(bufferSend);

	//channelTest->setLinkSetting(tLinkSetting);
	//channelTest->setLinkConfig(tLinkConfig);
	//channelTest->setLink(linkTcp);
	//channelTest->setProtocolSetting(modbusSetting);

	
	tLinkConfig->setReceiveBuffer(bufferReceive);
	tLinkConfig->setSendBuffer(bufferSend);
	

	if(strcmp(v_param->cfg->link,"TCP")==0)
	{
		Q_tcpClientLink* linkTcp = new Q_tcpClientLink();
		Q_tcpClientLinkSetting* tLinkSetting = new Q_tcpClientLinkSetting();

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
		tLinkConfig->setLinkSetting(tLinkSetting);
		channelTest->setLinkSetting(tLinkSetting);
		channelTest->setLink(linkTcp);

		
		
	}
	else
	{
		//RS-232/RS-485
		Q_serialLink* linkCom = new Q_serialLink();
		Q_serialLinkSetting* tLinkSetting = new Q_serialLinkSetting();

		char *tmp=NULL;
		tmp=modbusSetting->getContentID(11,1);
		if(tmp!=NULL)
		{
			printf("--Serial port name :%s\n", tmp);
			tLinkSetting->setComPortName(tmp);
		}
		tmp=modbusSetting->getContentID(12,1);
		if(tmp!=NULL)
		{
			tLinkSetting->setBountRate(atoi(tmp));
		}	
		tmp=modbusSetting->getContentID(13,1);
		if(tmp!=NULL)
		{
			tLinkSetting->setDataBit(atoi(tmp));
		}	
		tmp=modbusSetting->getContentID(14,1);
		if(tmp!=NULL)
		{
			tLinkSetting->setStopBit(atoi(tmp));
		}	
		tmp=modbusSetting->getContentID(15,1);
		if(tmp!=NULL)
		{
			tLinkSetting->setVerifyBit(atoi(tmp));
		}
		tLinkConfig->setLinkSetting(tLinkSetting);
		channelTest->setLinkSetting(tLinkSetting);
		channelTest->setLink(linkCom);
	}

	channelTest->setLinkConfig(tLinkConfig);



	Q_protocolConfig* protocolConfig = new Q_protocolConfig();
	protocolConfig->setProtocolSetting(modbusSetting);
	protocolConfig->setReceiveBuffer(bufferReceive);
	protocolConfig->setSendBuffer(bufferSend);
	channelTest->setProtocolConfig(protocolConfig);

	protocolConfig->setProtocolCommandManager(pCommandList);

	Q_protocolModbusRtuStd* protocol = new Q_protocolModbusRtuStd();
	protocol->setRtdbInf(v_param->m_pMgr);

	//因为是通过消息中心处理的，不需要传这此参数了tjs 2015.06.03
	//protocol->SetWndInfo(GetSafeHwnd(),WM_COMM_MSG_BEGIN);
	//protocol->setDlgMessageID(WM_DEBUGSERVERCOM);
	//protocol->setDlgHWND(GetSafeHwnd());

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

	if (rec->eDataType==eDataTypeBit)
	{
		Q_protocolModbusRtuStdCommand* pCommand = new Q_protocolModbusRtuStdCommand();
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
		Q_protocolModbusRtuStdCommand* pCommand = new Q_protocolModbusRtuStdCommand();
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
	printf("=== CommModbusRtuStd Protocol Dynamic lib====\n");
	printf("=== 2016-07-22====\n");
	printf("1、增加打印输入版本功能\n");
	printf("=== 2016-09-28====\n");
	printf("1、更改支持串转网的形式\n");
	printf("=== 2016-09-29====\n");
	printf("1、更改发送遥调，更新遥调表内容,遥控暂时没有填加呢\n");
	printf("=== 2016-10-28====\n");
	printf("1、更改发送消息方法，去掉全网同步报文消息的功能，只本机发送即可\n");
	printf("=== 2016-11-25====\n");
	printf("1、更改优化发送消息的内容把固定值改成strlen()+1的方式\n");
	return 1;
}

//----------------------------------------------------------------------------
//*
//*
//* Q_protocolModbusTcpServerStandardCommand class
//*
//*
Q_protocolModbusRtuStdCommand::Q_protocolModbusRtuStdCommand(void)
{
	m_nDestinationId=0; //站编号
	m_commandCDTType=eModbusCommandTypeNull;//命令类型
	m_commandCDTStatus=eModbusCommandStatusNull;//状态默认设定选择
	m_nObjectIndex=0;//站里面的设备编号
	m_bOpen=false;/// 数字量
	m_fValue=0.0f;//模拟量
	m_nValue=0;//数字量
	m_nMaxTryCount=5;//最大试验数
	m_nCurTryNum=0;//当前实验数
	m_curIdx=0;
}

Q_protocolModbusRtuStdCommand::~Q_protocolModbusRtuStdCommand(void)
{
}





//----------------------------------------------------------------------------
//*
//*
//* Q_protocolModbusTcpServerStandardCommandManager class
//*
//*
Q_ModbusRtuStdCmdMgr::Q_ModbusRtuStdCmdMgr(void)
{
}

Q_ModbusRtuStdCmdMgr::~Q_ModbusRtuStdCmdMgr(void)
{
}

bool Q_ModbusRtuStdCmdMgr::addCommand(Q_protocolCommandBase* pCommand)
{
	if(pCommand == NULL)
		return false;

	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i] == NULL)
		{
			//EnterCriticalSection(&m_csHandle);
			m_csHandle.Lock();
			m_commandList[i] = pCommand;
			m_nCurrentCommandCount++;
			//LeaveCriticalSection(&m_csHandle);
			m_csHandle.UnLock();
			return true;
		}
	}
	return false;
}

bool Q_ModbusRtuStdCmdMgr::addCommand(Q_CommandVal *v_val)
{
	if (v_val==NULL)
	{
		return false;
	}
	if (v_val->yt)
	{//yt
		Q_protocolModbusRtuStdCommand* pCommand = new Q_protocolModbusRtuStdCommand();
		pCommand->setCommonType(eCommandTypeDialog);
		pCommand->setDestinationId(v_val->DestinationId);//设备地址
		pCommand->setObjectIndex(v_val->ObjectIndex);//寄存器开始地址
		pCommand->setAnalogValue(v_val->fval);//调节值
		pCommand->setCommandModbusType(eModbusCommandRemoteAdjust);
		pCommand->setCommandModbusStatus(eModbusCommandSelect);
		pCommand->setMaxTryCount(v_val->trycount);
		return addCommand(pCommand);
	}
	else
	{
		Q_protocolModbusRtuStdCommand* pCommand = new Q_protocolModbusRtuStdCommand();
		pCommand->setCommonType(eCommandTypeDialog);
		pCommand->setDestinationId(v_val->DestinationId);//设备地址
		pCommand->setObjectIndex(v_val->ObjectIndex);//寄存器开始地址
		pCommand->setIntValue(v_val->nval);//调节值
		pCommand->setCommandModbusType(eModbusCommandRemoteControl);
		pCommand->setCommandModbusStatus(eModbusCommandSelect);
		pCommand->setMaxTryCount(v_val->trycount);
		return addCommand(pCommand);
	}
}


Q_protocolModbusRtuStdCommand* Q_ModbusRtuStdCmdMgr::findCommand(Q_Modbus_CommandType eType,Q_Modbus_CommandStatus eStatus,int nObjIndex)
{
	Q_protocolModbusRtuStdCommand* res = NULL;
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i] != NULL)
		{
			Q_protocolModbusRtuStdCommand* p = (Q_protocolModbusRtuStdCommand*)m_commandList[i];
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
bool Q_ModbusRtuStdCmdMgr::removeCommand(Q_Modbus_CommandType eType,Q_Modbus_CommandStatus eStatus,int nObjIndex)
{
	int nIndex = -1;
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i] != NULL)
		{
			Q_protocolModbusRtuStdCommand* p = (Q_protocolModbusRtuStdCommand*)m_commandList[i];
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
		//LeaveCriticalSection(&m_csHandle);
		m_csHandle.UnLock();
		return true;
	}
	else
	{
		return false;
	}
}

bool Q_ModbusRtuStdCmdMgr::removeCommand(Q_protocolCommandBase* pCommand)
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
Q_protcolModbusRtuStdSetting::Q_protcolModbusRtuStdSetting(void)
{
	memset(LastError,NULL,sizeof(LastError));
}
Q_protcolModbusRtuStdSetting::~Q_protcolModbusRtuStdSetting(void)
{
	m_vGroup.clear();
	m_vContent.clear();
}

void Q_protcolModbusRtuStdSetting::setFilePath(char *v_path)
{
	strcpy(m_sFilePath,v_path);
}
bool Q_protcolModbusRtuStdSetting::initSetting()
{
	return init();
}
bool Q_protcolModbusRtuStdSetting::init()
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

		int nID=atoi(itemElement->Attribute("id"));
		if(4==nID)
		{
			//m_alg_cmd_count=atoi(itemElement->Attribute("count"));
			for (nodeItem=node->FirstChild("Field");nodeItem;
				nodeItem=nodeItem->NextSibling("Field"))
			{
				itemElement=nodeItem->ToElement();

				ST_ModCmdCfg content;

				content.id=atoi(itemElement->Attribute("id"));
				content.nBegin=atoi(itemElement->Attribute("begin"));
				content.nCount=atoi(itemElement->Attribute("count"));
				content.nType=atoi(itemElement->Attribute("type"));
				content.device=atoi(itemElement->Attribute("device"));
				m_vCommand.push_back(content);	
			}
			m_alg_cmd_count = m_vCommand.size();
		}
		else if (5==nID)
		{
			for (nodeItem=node->FirstChild("Field");nodeItem;
				nodeItem=nodeItem->NextSibling("Field"))
			{
				itemElement=nodeItem->ToElement();

				ST_ModCmdYXCfg content;

				content.id=atoi(itemElement->Attribute("id"));
				content.nBegin=atoi(itemElement->Attribute("begin"));
				content.nCount=atoi(itemElement->Attribute("count"));
				content.nType=atoi(itemElement->Attribute("type"));
				content.device=atoi(itemElement->Attribute("device"));
				m_vYXCommand.push_back(content);	
			}
			m_pnt_cmd_count = m_vYXCommand.size();
		}
		else
		{
			ST_ModCfgT group;

			group.id=nID;
			sprintf(group.name,"%s",itemElement->Attribute("name"));
			group.type=eModXmlDataTypeString;

			m_vGroup.push_back(group);

			for (nodeItem=node->FirstChild("Field");nodeItem;
				nodeItem=nodeItem->NextSibling("Field"))
			{
				itemElement=nodeItem->ToElement();
				ST_ModCfgT content;
				content.group=group.id;

				content.id=atoi(itemElement->Attribute("id"));
				printf("name:%s--",itemElement->Attribute("name"));
				printf("valu:%s\n",itemElement->Attribute("value"));
				sprintf(content.name,"%s",itemElement->Attribute("name"));
				sprintf(content.value,"%s",itemElement->Attribute("value"));

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

	return true;
}
bool Q_protcolModbusRtuStdSetting::reloadLog()
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
						sprintf(m_vContent[i].value,"%s",itemElement->Attribute("value"));
					}
				}


			}
		}
	}

	return true;
}
char *Q_protcolModbusRtuStdSetting::getContentID(const int &v_idx,const int &v_grp)
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
void   Q_protcolModbusRtuStdSetting::setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_alg_count=nVaildCount;
	m_alg_begin=nAddrBegin;
	m_alg_end=nAddrEnd;
}

void   Q_protcolModbusRtuStdSetting::setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_pnt_count=nVaildCount;
	m_pnt_begin=nAddrBegin;
	m_pnt_end=nAddrEnd;
}
void Q_protcolModbusRtuStdSetting::setPlanScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_plan_addr_begin=nAddrBegin;
	m_plan_addr_end=nAddrEnd;
	m_plan_count=nVaildCount;
}
//计划曲线开始地址
int Q_protcolModbusRtuStdSetting::getPlanAddrBegin()
{
	return m_plan_addr_begin;
}
//计划曲线结束地址
int Q_protcolModbusRtuStdSetting::getPlanAddrEnd()
{
	return m_plan_addr_end;

}
//计划曲线信息个数
int Q_protcolModbusRtuStdSetting::getPlanInfoCount()
{
	return m_plan_count;
}
int  Q_protcolModbusRtuStdSetting::getAnalogBegin()
{
	return m_alg_begin;
}
int  Q_protcolModbusRtuStdSetting::getAnalogEnd()
{
	return m_alg_end;
}
int    Q_protcolModbusRtuStdSetting::getPointBegin()
{ 
	return m_pnt_begin;
}
int  Q_protcolModbusRtuStdSetting::getPointEnd()
{
	return m_pnt_end;
}
int  Q_protcolModbusRtuStdSetting::getAnalogCount()
{
	return m_alg_count;
}
int  Q_protcolModbusRtuStdSetting::getPointCount()
{
	return m_pnt_count;
}
int Q_protcolModbusRtuStdSetting::getRegisterBegin()
{
	int res=0;
	char *tmp=getContentID(3,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
int Q_protcolModbusRtuStdSetting::getRelayBegin()
{
	int res=0;
	char *tmp=getContentID(4,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
int Q_protcolModbusRtuStdSetting::getDeviceId()
{
	int res=0;
	char *tmp=getContentID(10,1);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
ModbusFloatUpSeq Q_protcolModbusRtuStdSetting::getFloatUpSeq()
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
ModbusCheckCodSeq Q_protcolModbusRtuStdSetting::getCheckCodeSeq()
{
	ModbusCheckCodSeq seq;
	seq=eCheckCodeSeqHighFirst;
	int res=0;
	char *tmp=getContentID(13,2);
	if(tmp!=NULL)
	{
		int a=atoi(tmp);
		switch(a)
		{
		case 1:
			seq=eCheckCodeSeqHighFirst;
			break;
		case 2:
			seq=eCheckCodeSeqLowFirst;
			break;
		}
	}
	return seq;
}
ModbusIntUpSeq Q_protcolModbusRtuStdSetting::getIntUpSeq()
{
	ModbusIntUpSeq seq;
	seq=eIntUpSeqHighFirst;
	int res=0;
	char *tmp=getContentID(11,2);
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
		}
	}
	return seq;
}
int Q_protcolModbusRtuStdSetting::getCRCErrorHandle()//连续10帧数据CRC校验错误是否判定为通道断开0否1是
{
	int res=0;
	char *tmp=getContentID(13,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);		
	}
	return res;
}
NewModbusYTMethod Q_protcolModbusRtuStdSetting::getYTMethod()
{
	NewModbusYTMethod seq;
	seq=eYtFloat;
	int res=0;
	char *tmp=getContentID(1,3);
	if(tmp!=NULL)
	{
		int a=atoi(tmp);
		switch(a)
		{
		case 1:
			seq=eYtFloat;
			break;
		case 2:
			seq=eYtInt2;
			break;
		case 3:
			seq=eYtInt4;
		default:
			break;
		}
	}
	return seq;
}
int Q_protcolModbusRtuStdSetting::getAlgCmdCount()
{
	return m_alg_cmd_count;
}
int Q_protcolModbusRtuStdSetting::getPntCmdCount()
{
	return m_pnt_cmd_count;
}
int Q_protcolModbusRtuStdSetting::getPerTime()
{
	int res=0;
	char *tmp=getContentID(6,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
int Q_protcolModbusRtuStdSetting::getSleepPerTimes()
{
	int res=0;
	char *tmp=getContentID(7,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}
int Q_protcolModbusRtuStdSetting::getReadRegisterCode()
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
int Q_protcolModbusRtuStdSetting::getReadYXRegisterCode()
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

int Q_protcolModbusRtuStdSetting::getYTCode()
{
	int res=-1;
	int code=-1;
	//char *tmp=getContentID(12,2);
	char *tmp=getContentID(1,3);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	printf("res:%d\n",res);
	switch(res)
	{
	case 1: 
		code = 0x10;
		break;
	case 2:
		code = 0x06;
		break;
	case 5:
		code = 0x06;//非标准
		break;
	case 6:
		code = 0x03;//非标准
		break;
	
	}
	printf("YTCode:%d\n", code);
	return code;
}
int Q_protcolModbusRtuStdSetting::getYKCode()
{
	int res=-1;
	//char *tmp=getContentID(13,2);
	char *tmp=getContentID(2,3);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	printf("YKCode:%d\n", res);
	return res;
}

bool Q_protcolModbusRtuStdSetting::getPrintPacket()
{
	int res=0;
	char *tmp=getContentID(1,10);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res==0?false:true;
}
bool Q_protcolModbusRtuStdSetting::getPrintPrompt()
{
	int res=0;
	char *tmp=getContentID(3,10);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res==0?false:true;
}
bool Q_protcolModbusRtuStdSetting::getPrintError()
{
	int res=0;
	char *tmp=getContentID(4,10);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res==0?false:true;
}

bool Q_protcolModbusRtuStdSetting::getPrintTranslate()
{
	int res=0;
	char *tmp=getContentID(2,10);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res==0?false:true;
}
Q_protocolModbusRtuStd::Q_protocolModbusRtuStd()
{
	m_cpuEndian=eBigEndian;//默认是大端模式
	m_LastCmd=NULL;
	m_LastCmdForCmd=NULL;
	m_nMsgID=0;
	m_PrintSetting=NULL;
	m_ReciveTime=time(NULL);
	m_CRCErroCount = 0;//CRC校验错误帧计数器
}
Q_protocolModbusRtuStd::~Q_protocolModbusRtuStd()
{

}
bool Q_protocolModbusRtuStd::initializeProtocol()
{
	return true;
}

bool Q_protocolModbusRtuStd::explainData()
{
	
	char bufferReceive[Q_FRAME_CONST4096];	
	memset(bufferReceive,NULL,sizeof(bufferReceive));
	int nLength = m_pProcotolConfig->getReceiveBuffer()->getDataLength();
	int nByteCount = 0;
	int nRes = 0;
	time_t now = time(NULL);
	if(now-m_ReciveTime > 30)//如果超过30秒没有收到数据，说明通道中断
	{
		m_eProtocolStatus = eCommonStatusError;
		printf("modbusRtu主站超过30秒没有收到数据，认为通道断开\r\n");
	}
	if(nLength >= 5)
	{
		m_ReciveTime = time(NULL);//记录接收数据时间
		nByteCount = m_pProcotolConfig->getReceiveBuffer()->readData(bufferReceive,256,false);
		if(nByteCount > 0)
		{
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeRx,(BYTE*)bufferReceive,nByteCount); //打印收到的报文
			explainDataEx(bufferReceive,nByteCount,nRes);
		}
		if(nRes>0)
		m_pProcotolConfig->getReceiveBuffer()->removeData(nRes);
	}	
	return true;
}
bool Q_protocolModbusRtuStd::explainDataEx(char* buff,int nCount,int& nCurrentPos)
{
	int nIndexCur = 0;
	int nIndexFind = 0;//信息有效的开始位置,首个字节是6个字节的前导字符
	bool bFindSync = false;

	//CRC校验

	Q_MODBUS_ASDU_RTU *asdu=NULL;
	asdu=(Q_MODBUS_ASDU_RTU *)&buff[0];
	

	if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPacket())
	{
		onRxMsg((BYTE *)&buff[nIndexCur],nCount);
	}
	if (m_bRecvMsg)
	{
		OnRecvMsg((char *)&buff[nIndexCur],nCount);
	}
	if((asdu->funcode & 0x80)==0x80)
	{
		nCurrentPos=5;
		if(m_LastCmd!=NULL||m_LastCmdForCmd!=NULL)
		{
			Q_ModbusRtuStdCmdMgr* pManager = (Q_ModbusRtuStdCmdMgr*)m_pProcotolConfig->getProtocolCommandManager();
			if(pManager != NULL)
			{ 
				//执行失败
				//移除上一个命令
				if(m_LastCmd->getCommonType()!=eCommandTypeCycle)
				{
					if (eModbusCommandRemoteAdjust==m_LastCmd->getCommandModbusType() && eModbusCommandAct==m_LastCmd->getCommandModbusStatus())
					{
						Q_dataManager* p = _GlobalData.getDataManager();
						LPQ_YTEDATATAG pYTData = p->getYTData();
						for (int i=0;i<p->getYTCount();i++)
						{
							if (m_pProcotolConfig->getProtocolSetting()->getChannelID()  ==pYTData[i].channelid && pYTData[i].device==m_LastCmd->getDestinationId() && pYTData[i].no==m_LastCmd->getObjectIndex())
							{
								pYTData[i].finishtime = time(NULL);
								pYTData[i].execresult = 3;
								break;
							}
						}

						if (NULL!=m_PrintSetting && m_PrintSetting->getPrintPrompt())
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"YT exec failed ,device=%d,no=%d,value=%f\n",m_LastCmd->getDestinationId(),m_LastCmd->getObjectIndex(),m_LastCmd->getAnalogValue());
							OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
						}
						if(true/*m_bInfoMsg*/)
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"YT exec failed ,device=%d,no=%d,value=%f\n",m_LastCmd->getDestinationId(),m_LastCmd->getObjectIndex(),m_LastCmd->getAnalogValue());
							OnInfoMsg(m_LastMsg,sizeof(m_LastMsg),m_LastCmd);
						}
					}
					else if (eModbusCommandRemoteControl==m_LastCmd->getCommandModbusType() && eModbusCommandAct==m_LastCmd->getCommandModbusStatus())
					{
						Q_dataManager* p = _GlobalData.getDataManager();
						LPQ_YKEDATATAG pYKData = p->getYKData();
						for (int i=0;i<p->getYKCount();i++)
						{
							if (m_pProcotolConfig->getProtocolSetting()->getChannelID()  ==pYKData[i].channelid && pYKData[i].device==m_LastCmd->getDestinationId() && pYKData[i].no==m_LastCmd->getObjectIndex())
							{
								pYKData[i].finishtime = time(NULL);
								pYKData[i].execresult = 3;
								break;
							}
						}

						if (NULL!=m_PrintSetting && m_PrintSetting->getPrintPrompt())
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"YK exec failed ,device=%d,no=%d,value=%d\n",m_LastCmd->getDestinationId(),m_LastCmd->getObjectIndex(),m_LastCmd->getIntValue());
							OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
						}
						if(true/*m_bInfoMsg*/)
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"YK exec failed ,device=%d,no=%d,value=%d\n",m_LastCmd->getDestinationId(),m_LastCmd->getObjectIndex(),m_LastCmd->getIntValue());
							OnInfoMsg(m_LastMsg,sizeof(m_LastMsg),m_LastCmd);
						}
					}
					pManager->removeCommand(m_LastCmd);
					m_LastCmd=NULL;
				}
				else
				{
					if(m_LastCmd->getCommandModbusType()==eModbusCommandMeasure || m_LastCmd->getCommandModbusType()==eModbusCommandPoint)
					{
						if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
						{
							char chErr[64] = {0};
							if (asdu->funcode == 0x83 || asdu->funcode == 0x81)
							{
								sprintf_s(chErr,sizeof(chErr),"读取的寄存器范围异常");
							}
							else
							{
								sprintf_s(chErr,sizeof(chErr),"未知");
							}
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Modbus异常响应，device=%d,功能码%02x,%s\n",asdu->addr,asdu->funcode,chErr);
							OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
						}
						if(m_bNomalInfoMsg)
						{
							char chErr[64] = {0};
							if (asdu->funcode == 0x83 || asdu->funcode == 0x81)
							{
								sprintf_s(chErr,sizeof(chErr),"读取的寄存器范围异常");
							}
							else
							{
								sprintf_s(chErr,sizeof(chErr),"未知");
							}
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Modbus异常响应，device=%d,功能码%02x,%s\n",asdu->addr,asdu->funcode,chErr);
							OnNomalInfoMsg(m_LastMsg,sizeof(m_LastMsg));
						}
						m_LastCmd->setCommandModbusStatus(eModbusCommandActCon);
						if (m_LastCmd->getCommandModbusType()==eModbusCommandMeasure)
						{
							if(m_PrintSetting->getAlgCmdCount()>0)
							{
								if(m_LastCmd->m_curIdx+1<m_PrintSetting->getAlgCmdCount())
									m_LastCmd->m_curIdx+=1;
								else
									m_LastCmd->m_curIdx=0;

								m_LastCmd->setObjectIndex(m_PrintSetting->m_vCommand[m_LastCmd->m_curIdx].nBegin);
								m_LastCmd->setIntValue(m_PrintSetting->m_vCommand[m_LastCmd->m_curIdx].nCount);
							}
						}
						else if (m_LastCmd->getCommandModbusType()==eModbusCommandPoint)
						{
							if(m_PrintSetting->getPntCmdCount()>0)
							{
								if(m_LastCmd->m_curIdx+1<m_PrintSetting->getPntCmdCount())
									m_LastCmd->m_curIdx+=1;
								else
									m_LastCmd->m_curIdx=0;

								m_LastCmd->setObjectIndex(m_PrintSetting->m_vYXCommand[m_LastCmd->m_curIdx].nBegin);
								m_LastCmd->setIntValue(m_PrintSetting->m_vYXCommand[m_LastCmd->m_curIdx].nCount);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		nCurrentPos=nCount;

		BOOL m_crcCheckOk;
		if(m_PrintSetting!=NULL)
		{
			int nByteCount=0;
			
			//增加一个判断报文内容是否完整的信息,2013-10-26
			if(m_PrintSetting->getReadYXRegisterCode()==asdu->funcode||
				m_PrintSetting->getReadRegisterCode()==asdu->funcode)
			{
					nByteCount=buff[2];
					if(nByteCount+5>nCount)
					{
						//数据内容不全不处理
						nCurrentPos=0;
						return false;
					}
					
			}

			int nCrcCodeRight=CRC16((BYTE*)buff,nCount-2);
			int nCrcCodeRecv=*(WORD*)(&asdu->data[nCount-4]);
			if(nCrcCodeRecv!=nCrcCodeRight)
			{
				if(m_CRCErroHandle == 1)
				{
					m_CRCErroCount++;
					if(m_CRCErroCount>10)
					{						
						m_eProtocolStatus = eCommonStatusError;	
						printf("CRC code different,recv=%d\r\n",m_CRCErroCount);
						m_CRCErroCount = 12;//防止溢出
					}
				}
				memset(m_LastMsg,NULL,sizeof(m_LastMsg));
				sprintf(m_LastMsg,"CRC code different,recv=%d,compute %d!",nCrcCodeRecv,nCrcCodeRight);
				OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
				if(m_bNomalInfoMsg)
				{
					OnNomalInfoMsg(m_LastMsg,sizeof(m_LastMsg));
				}
				m_crcCheckOk = FALSE;
				return false;
			}
			else
			{
				if(m_CRCErroHandle == 1)
				{
					m_eProtocolStatus = eCommonStatusNormal;//只有CRC正确时才把通道状态设置为正常
				}
				m_CRCErroCount = 0;
			}
		
            

			
			
			if(m_PrintSetting->getReadYXRegisterCode()==asdu->funcode )
			{
				if(m_LastCmd!=NULL)
				{
                  if(m_LastCmd->getCommandModbusType() == eModbusCommandPoint)
				  {
					  //读遥信
					  explainFrameYX(asdu,(BYTE*)&buff[2]);//从数据区开始，到crc校验结束
				  }
				}

				
			}
			else if(m_PrintSetting->getReadRegisterCode()==asdu->funcode )
			{
				if(m_LastCmd!=NULL)
				{
                  if(m_LastCmd->getCommandModbusType() == eModbusCommandMeasure)
				  {
					  //读遥测
					  explainFrameYC(asdu,(BYTE*)&buff[2]);
				  }
				}
				
			}
			else if(m_PrintSetting->getYTCode()==asdu->funcode)
			{
				if (m_YtMethod==eYtFloat)
				{
					explainFrameYT_M(asdu,(BYTE*)&buff[2]);
				}
				else if (m_YtMethod==eYtInt2)
				{
					explainFrameYT(asdu,(BYTE*)&buff[2]);
				}
				else
				{//eYtInt4
					explainFrameYT_Int4(asdu,(BYTE*)&buff[2]);
				}
			}
			else if(m_PrintSetting->getYKCode()==asdu->funcode)
			{
				//读遥测
				explainFrameYK(asdu,(BYTE*)&buff[2]);
			}
			else
			{
				//不合法功能码，也可以称为未识别功能码
				sendErroCode(asdu,MODBUS_ERROR_CODE_1);
				nIndexCur = nCount;
			}
		}
	}
	return true;
}
//遥信
bool Q_protocolModbusRtuStd::explainFrameYX(Q_MODBUS_ASDU_RTU *asdu,BYTE *data)
{
	Q_ModbusRtuStdCmdMgr* pManager = (Q_ModbusRtuStdCmdMgr*)m_pProcotolConfig->getProtocolCommandManager();
	if(pManager != NULL)
	{
		int startAddr=0;

		if(m_LastCmd!=NULL)
			startAddr=m_LastCmd->getObjectIndex();
		else
			return false;

		Q_protocolModbusRtuStdCommand* cmd=pManager->findCommand(eModbusCommandPoint,eModbusCommandAct,startAddr);
		if(cmd!=NULL)
		{
			int nByteCount=0;
			if(data[0]!=NULL)
			{
				nByteCount=data[0]&0xFF;
			}

			int pi=1;

			Q_dataManager* p = _GlobalData.getDataManager();
			Q_RECORDDATATAG dataArray[8];
			for(int i=0;i<8;i++)
			{
             dataArray[i].eDataType=eDataTypeBit;
			}
			

			int i=0;
			//int t_last_data_idx=0;
			int nYCBegin = m_PrintSetting->getPointBegin();
			Mod_DVAL dval;
			
			//printf("explainFrameYX1\n");
			for(int i=0;i<nByteCount;i++)
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
					if(fillTagInformation(&dataArray[j],startAddr+i*8+j,asdu->addr)==true)
					{
						//printf("explainFrameYX5,no=%d\n", startAddr+i*8+j);
						dataArray[j].channelID=m_PrintSetting->getChannelID();
						dataArray[j].device=asdu->addr;
						dataArray[j].no=startAddr+i*8+j;
						if(dataArray[j].no >= 0)
						{
							m_pMgr->recordDataValue(&dataArray[j],1);
						}								
					}	
				}
				
				
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
//遥测
bool Q_protocolModbusRtuStd::explainFrameYC(Q_MODBUS_ASDU_RTU *asdu,BYTE *data)
{
	Q_ModbusRtuStdCmdMgr* pManager = (Q_ModbusRtuStdCmdMgr*)m_pProcotolConfig->getProtocolCommandManager();
	if(pManager != NULL)
	{
		int startAddr=0;

		if(m_LastCmd!=NULL)
			startAddr=m_LastCmd->getObjectIndex();
		else
			return false;

		Q_protocolModbusRtuStdCommand* cmd=pManager->findCommand(eModbusCommandMeasure,eModbusCommandAct,startAddr);
		if(cmd!=NULL)
		{			
			int nByteCount=0;
			if(data[0]!=NULL)
			{
				nByteCount=data[0]&0xFF;
			}			
			int pi=1;
			Q_dataManager* p = _GlobalData.getDataManager();
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeFloat;

			int i=0;
			//int t_last_data_idx=0;
			
			//int nYCBegin = m_PrintSetting->getAnalogBegin();
			BYTE tmp[4];
			
			if(cmd->getSwitchValue()==true)
			{
				//处理四字节FLOAT类型
				printf("float type\n");
				while(pi<nByteCount+1)
				{					
					if(fillTagInformation(&dataArray,startAddr+i,asdu->addr)==true)
					{						
						dataArray.channelID=m_PrintSetting->getChannelID();
						dataArray.device=asdu->addr;
						dataArray.no=startAddr+i;
						//dataArray.nIndex=startAddr+i-m_PrintSetting->getAnalogBegin();
						//printf("m_pMgr->getAnalogCount()=%d,m_PrintSetting->getAnalogBegin()=%d,m_PrintSetting->getAnalogCount()=%d\n",m_pMgr->getAnalogCount(),m_PrintSetting->getAnalogBegin(), m_PrintSetting->getAnalogCount());
						if (dataArray.no >=0)
						{							
							memset(tmp,NULL,sizeof(tmp));							
							//if(p->getAnalogData()[dataArray.nIndex-1].eDataType==eDataTypeFloat)
							{				
								//printf("m_FloatUpSeq:%d\n", m_FloatUpSeq);
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
								}
								float vv=0.0f;
								memcpy(&vv,tmp,sizeof(vv));
								dataArray.eDataType=eDataTypeFloat;
								//if(factor>0.999999 && factor<1.000001)
								//{
									dataArray.fValue=vv;
								//}
								//else
								//{
									//dataArray.fValue=vv*factor;
								//}
								//printf("vv:%f\n",vv);
								m_pMgr->recordDataValue(&dataArray,1);
							}
						}	
						pi+=4;
						i+=1;
					}
					else
					{
						if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Can't find right YC point in realDB,Please check yc.xml ,Channel=%d,device=%d,addr=%d",m_PrintSetting->getChannelID(),cmd->getDestinationId(),cmd->getObjectIndex());
							OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
						}
						if(m_bNomalInfoMsg)
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Can't find right YC point in realDB,Please check yc.xml ,Channel=%d,device=%d,addr=%d",m_PrintSetting->getChannelID(),cmd->getDestinationId(),cmd->getObjectIndex());
							OnNomalInfoMsg(m_LastMsg,sizeof(m_LastMsg));
						}
						break;
					}	
				}
			}
			else
			{
				printf("int type\n");
				while(pi<nByteCount+1)
				{
					if(fillTagInformation(&dataArray,startAddr+i,asdu->addr)==true)
					{
						dataArray.channelID=m_PrintSetting->getChannelID();
						dataArray.device=asdu->addr;
						dataArray.no=startAddr+i;
						//if (dataArray.nIndex >0  && dataArray.nIndex <= p->getAnalogCount())
						if(dataArray.no >= 0)
						{
							//t_last_data_idx=dataArray.nIndex;
							//float factor=p->getAnalogData()[dataArray.nIndex-1].factor;
							memset(tmp,NULL,sizeof(tmp));
							//if(p->getAnalogData()[dataArray.nIndex-1].eDataType==eDataTypeFloat)
							{
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
								}

								short n=0;
								memcpy(&n,tmp,sizeof(n));

								//dataArray.fValue=n*factor;
								dataArray.fValue=n;
								m_pMgr->recordDataValue(&dataArray,1);

							}
						}
					}
					pi+=2;
					i+=1;
				}
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
void Q_protocolModbusRtuStd::setNextDigital(Q_protocolModbusRtuStdCommand* cmd)
{
//add by tjs 2015-3-23
	if(m_PrintSetting->getPntCmdCount()>0)
	{

	if(cmd->m_curIdx+1!=m_PrintSetting->getPntCmdCount())
	{
		cmd->m_curIdx+=1;
		cmd->setDestinationId(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].device);
		cmd->setObjectIndex(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].nBegin);
		cmd->setIntValue(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].nCount);

		cmd->setCommandModbusType(eModbusCommandPoint);
		cmd->setCommandModbusStatus(eModbusCommandSelect);

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

			if(m_PrintSetting->m_vCommand[0].nType==1)
				cmd->setSwitchValue(true);//FLOAT类型
			else
				cmd->setSwitchValue(false);//INT类型

			cmd->setCommandModbusType(eModbusCommandMeasure);
			cmd->setCommandModbusStatus(eModbusCommandSelect);

		}
		else
		{
			cmd->setDestinationId(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].device);
			cmd->setObjectIndex(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].nBegin);
			cmd->setIntValue(m_PrintSetting->m_vYXCommand[cmd->m_curIdx].nCount);

			cmd->setCommandModbusType(eModbusCommandPoint);
			cmd->setCommandModbusStatus(eModbusCommandSelect);
		}
		//更改读遥信数据
	}
	}
}
void Q_protocolModbusRtuStd::setNextAnalog(Q_protocolModbusRtuStdCommand* cmd)
{
	//等待下一个执行时间

	if(cmd->m_curIdx+1!=m_PrintSetting->getAlgCmdCount())
	{
		cmd->m_curIdx+=1;
		cmd->setCommandModbusStatus(eModbusCommandSelect);
		cmd->setDestinationId(m_PrintSetting->m_vCommand[cmd->m_curIdx].device);
		cmd->setObjectIndex(m_PrintSetting->m_vCommand[cmd->m_curIdx].nBegin);
		cmd->setIntValue(m_PrintSetting->m_vCommand[cmd->m_curIdx].nCount);
		if(m_PrintSetting->m_vCommand[cmd->m_curIdx].nType==1)
		{
			cmd->setSwitchValue(true);
		}
		else
		{
			cmd->setSwitchValue(false);
		}
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

			cmd->setCommandModbusType(eModbusCommandPoint);
			cmd->setCommandModbusStatus(eModbusCommandSelect);

		}
		else
		{
// 			cmd->setDestinationId(m_PrintSetting->m_vCommand[cmd->m_curIdx].device);
// 			cmd->setObjectIndex(m_PrintSetting->m_vCommand[cmd->m_curIdx].nBegin);
// 			cmd->setIntValue(m_PrintSetting->m_vCommand[cmd->m_curIdx].nCount);
// 			if(m_PrintSetting->m_vCommand[cmd->m_curIdx].nType==1)
// 			{
// 				cmd->setSwitchValue(true);
// 			}
// 			else
// 			{
// 				cmd->setSwitchValue(false);
// 			}
// 			cmd->setCommandModbusType(eModbusCommandMeasure);
// 			cmd->setCommandModbusStatus(eModbusCommandSelect);
			cmd->setDestinationId(m_PrintSetting->m_vCommand[0].device);
			cmd->setObjectIndex(m_PrintSetting->m_vCommand[0].nBegin);
			cmd->setIntValue(m_PrintSetting->m_vCommand[0].nCount);

			if(m_PrintSetting->m_vCommand[0].nType==1)
				cmd->setSwitchValue(true);//FLOAT类型
			else
				cmd->setSwitchValue(false);//INT类型

			cmd->setCommandModbusType(eModbusCommandMeasure);
			cmd->setCommandModbusStatus(eModbusCommandSelect);
		}
	}
}

//遥控
bool Q_protocolModbusRtuStd::explainFrameYK(Q_MODBUS_ASDU_RTU *asdu,BYTE *data)
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
		sendErroCode(asdu,MODBUS_ERROR_CODE_4);
		return false;
	}

	Q_ModbusRtuStdCmdMgr* pManager = (Q_ModbusRtuStdCmdMgr*)m_pProcotolConfig->getProtocolCommandManager();
	if(pManager != NULL)
	{
		Q_protocolModbusRtuStdCommand* cmd=pManager->findCommand(eModbusCommandRemoteControl,eModbusCommandAct,startAddr);
		if(cmd!=NULL)
		{
			if (startAddr==cmd->getObjectIndex() && (int)setStatus==cmd->getIntValue())
			{
				Q_dataManager* p = _GlobalData.getDataManager();
				LPQ_YKEDATATAG pYKData = p->getYKData();
				for (int i=0;i<p->getYKCount();i++)
				{
					if (m_pProcotolConfig->getProtocolSetting()->getChannelID()  ==pYKData[i].channelid && pYKData[i].device==asdu->addr && pYKData[i].no==startAddr)
					{
						pYKData[i].finishtime = time(NULL);
						pYKData[i].execresult = 1;
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote control execute success,device addr :%d;start reg:%d,value is %d",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getIntValue());
						OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
						if(true/*m_bInfoMsg*/)
						{
							sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote control execute success,device addr :%d;start reg:%d,value is %d",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getIntValue());
							OnInfoMsg(m_LastMsg,sizeof(m_LastMsg),cmd);
						}
						cmd->setCommandModbusStatus(eModbusCommandActCon);
						break;
					}
				}
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
	// 	Q_MODBUS_ASDU_RTU asdu;
	// 	memset(&asdu,NULL,sizeof(asdu));
	// 	asdu.header.codeHigh=MODBUS_SYNC_CODE_H;//信息编号设定为0
	// 	asdu.header.codeLow=MODBUS_SYNC_CODE2;
	// 	asdu.header.codeFlgHigh=MODBUS_SYNC_CODE1;
	// 	asdu.header.codeFlgLow=MODBUS_SYNC_CODE1;
	// 	asdu.addr=asdu->addr;
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
bool Q_protocolModbusRtuStd::explainFrameYT(Q_MODBUS_ASDU_RTU *asdu,BYTE *data)
{
	int startAddr=data[0]<<8;
	startAddr+=data[1];

	Q_ModbusRtuStdCmdMgr* pManager = (Q_ModbusRtuStdCmdMgr*)m_pProcotolConfig->getProtocolCommandManager();
	if(pManager != NULL)
	{
		Q_protocolModbusRtuStdCommand* cmd=pManager->findCommand(eModbusCommandRemoteAdjust,eModbusCommandAct,startAddr);
		if(cmd!=NULL)
		{
			//执行成功
			BYTE tmp[4];
			memset(tmp,NULL,sizeof(tmp));

			Q_RECORDDATATAG ytRec;
			ytRec.eDataType=eDataTypeFloat;
			ytRec.channelID=m_pProcotolConfig->getProtocolSetting()->getChannelID();
			ytRec.device=cmd->getDestinationId();
			ytRec.no=startAddr;
			ytRec.fValue=cmd->getAnalogValue();//赋初值

			if(!m_pMgr->getYtValue(&ytRec))
			{
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",cmd->getDestinationId(),startAddr);
				SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);
				return false;
			}

			switch(m_IntUpSeq)
			{
			case eIntUpSeqHighFirst:
				tmp[0]=data[2];
				tmp[1]=data[3];
				break;
			case eIntUpSeqLowFirst:
				tmp[1]=data[2];
				tmp[0]=data[3];
				break;
			}

			short val=tmp[0]<<8|tmp[1];

			float vv=cmd->getAnalogValue();
			if(vv==val)
			{
				    cmd->setCommandModbusStatus(eModbusCommandActCon);
				
					sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %.3f",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getAnalogValue());
					OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
			
			}
			else
			{
					sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!And original value is %f and current value %d",cmd->getAnalogValue(),val);
					OnErrorMsg(m_LastMsg,sizeof(m_LastMsg));
				
				cmd->setCommandModbusStatus(eModbusCommandActError);
			}
			if(m_pMgr->handleMasterYt(&ytRec)!=0)
			{

				//新规约打印
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config.device:%d register : %d ",cmd->getDestinationId(),startAddr);
				SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

			}

		

		}
		return true;
	}
	else
	{
		return false;
	}	
}

bool Q_protocolModbusRtuStd::explainFrameYT_Int4(Q_MODBUS_ASDU_RTU *asdu,BYTE *data)
{
	int startAddr=data[0]<<8;
	startAddr+=data[1];

	int nRegCount=data[2]<<8;
	nRegCount+=data[3];

	Q_ModbusRtuStdCmdMgr* pManager = (Q_ModbusRtuStdCmdMgr*)m_pProcotolConfig->getProtocolCommandManager();
	if(pManager != NULL)
	{
		Q_protocolModbusRtuStdCommand* cmd=pManager->findCommand(eModbusCommandRemoteAdjust,eModbusCommandAct,startAddr);
		if(cmd!=NULL)
		{
			Q_dataManager* p = _GlobalData.getDataManager();
			LPQ_YTEDATATAG pYTData = p->getYTData();
			for (int i=0;i<p->getYTCount();i++)
			{
				if (m_pProcotolConfig->getProtocolSetting()->getChannelID()  ==pYTData[i].channelid && pYTData[i].device==asdu->addr && pYTData[i].no==startAddr)
				{
					pYTData[i].finishtime = time(NULL);
					pYTData[i].execresult = 1;
					break;
				}
			}


			float vv=cmd->getAnalogValue();
			if(nRegCount==sizeof(vv)/2)
			{
				cmd->setCommandModbusStatus(eModbusCommandActCon);

				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %.3f",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getAnalogValue());
				OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
				if(true/*m_bInfoMsg*/)
				{
					OnInfoMsg(m_LastMsg,sizeof(m_LastMsg),cmd);
				}
			}
			else
			{
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!");
				OnErrorMsg(m_LastMsg,sizeof(m_LastMsg));
				if (m_bErrMsg)
				{
					sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!");
					OnErrMsg(m_LastMsg,sizeof(m_LastMsg));
				}
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

//遥调
bool Q_protocolModbusRtuStd::explainFrameYT_M(Q_MODBUS_ASDU_RTU *asdu,BYTE *data)
{
	int startAddr=data[0]<<8;
	startAddr+=data[1];

	int nRegCount=data[2]<<8;
	nRegCount+=data[3];

	Q_ModbusRtuStdCmdMgr* pManager = (Q_ModbusRtuStdCmdMgr*)m_pProcotolConfig->getProtocolCommandManager();
	if(pManager != NULL)
	{
		Q_protocolModbusRtuStdCommand* cmd=pManager->findCommand(eModbusCommandRemoteAdjust,eModbusCommandAct,startAddr);
		if(cmd!=NULL)
		{
			Q_dataManager* p = _GlobalData.getDataManager();
			LPQ_YTEDATATAG pYTData = p->getYTData();
			for (int i=0;i<p->getYTCount();i++)
			{
				if (m_pProcotolConfig->getProtocolSetting()->getChannelID()  ==pYTData[i].channelid && pYTData[i].device==asdu->addr && pYTData[i].no==startAddr)
				{
					pYTData[i].finishtime = time(NULL);
					pYTData[i].execresult = 1;
					break;
				}
			}


			float vv=cmd->getAnalogValue();
			if(nRegCount==sizeof(vv)/2)
			{
				cmd->setCommandModbusStatus(eModbusCommandActCon);

					sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust execute success,device addr :%d;start reg:%d,value is %.3f",cmd->getDestinationId(),cmd->getObjectIndex(),cmd->getAnalogValue());
					OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
					if(true/*m_bInfoMsg*/)
					{
						OnInfoMsg(m_LastMsg,sizeof(m_LastMsg),cmd);
					}
				}
				else
				{
					sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!");
					OnErrorMsg(m_LastMsg,sizeof(m_LastMsg));
					if (m_bErrMsg)
					{
						sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Remote adjust return value different!");
						OnErrMsg(m_LastMsg,sizeof(m_LastMsg));
					}
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
bool Q_protocolModbusRtuStd::sendErroCode(Q_MODBUS_ASDU_RTU *header,BYTE code)
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
bool Q_protocolModbusRtuStd::sendData(Q_MODBUS_ASDU_RTU *asdu,int len)
{

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPacket())
		{
			onTxMsg((BYTE *)asdu,len);
		}
		if (m_bSendMsg)
		{
			OnSendMsg((char *)asdu,len);
		}
		bool ret=pSendTool->writeData((char*)asdu,len);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
		}
		if(true == ret)
		{
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(BYTE*)asdu,len); //打印发送的报文
		}
		return ret;
	}
	return false;

}
bool Q_protocolModbusRtuStd::makeFrame()
{
	Q_ModbusRtuStdCmdMgr* pManager = (Q_ModbusRtuStdCmdMgr*)m_pProcotolConfig->getProtocolCommandManager();
	if(pManager != NULL)
	{
		bool bJumpFlg=false;//目的是一条命令一条命令的处理
		for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
		{
			Q_protocolModbusRtuStdCommand* pCommand = (Q_protocolModbusRtuStdCommand*)pManager->getCommandByIndex(i);
			if(pCommand != NULL)
			{
				switch(pCommand->getCommandModbusType())
				{
				case eModbusCommandRemoteAdjust:
					{
						if(pCommand->getCommandModbusStatus()== eModbusCommandSelect)
						{
							printf("m_YtMethod:%d\n", m_YtMethod);
							//判断是否超出最大的测试次数
							if(m_YtMethod==eYtFloat)
							{
								handleFrameYT_M(pCommand);
							}
							else if (m_YtMethod==eYtInt2)
							{
								handleFrameYT(pCommand);
							}
							else
							{
								handleFrameYT_Int4(pCommand);
							}
							bJumpFlg=true;
						}
						else if(pCommand->getCommandModbusStatus() == eModbusCommandAct)
						{
							if(pCommand->getCurTryNum()>=pCommand->getMaxTryCount())
							{
								if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
								{
									sprintf_s(chInfo,Q_FRAME_CONST128,"Beyond command max try count,delete command.max count=%d\n",pCommand->getMaxTryCount());
									OnShowMsg(chInfo,strlen(chInfo)+1);
								}
								if(true/*m_bInfoMsg*/)
								{
									sprintf_s(chInfo,Q_FRAME_CONST128,"Beyond command max try count,delete command.max count=%d\n",pCommand->getMaxTryCount());
									OnInfoMsg(chInfo,strlen(chInfo)+1,pCommand);
								}
								pManager->removeCommandByIndex(i);

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
							if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintError())
							{
								sprintf_s(chInfo,Q_FRAME_CONST128,"delete command.command status %d\n",pCommand->getCommandModbusStatus());
								OnShowMsg(chInfo,strlen(chInfo)+1);
							}
							if(true/*m_bInfoMsg*/)
							{
								sprintf_s(chInfo,Q_FRAME_CONST128,"delete command.command status %d\n",pCommand->getCommandModbusStatus());
								OnInfoMsg(chInfo,strlen(chInfo)+1,pCommand);
							}
							pManager->removeCommandByIndex(i);

						}
						break;
					}
				case eModbusCommandMeasure:
					{
						if(pCommand->getCommandModbusStatus()== eModbusCommandSelect)
						{
							//判断是否超出最大的测试次数
							handleFrameYC(pCommand);
							bJumpFlg=true;
						}
						else if(pCommand->getCommandModbusStatus() == eModbusCommandAct)
						{
							if(pCommand->getCurTryNum()>=pCommand->getMaxTryCount())
							{
								if(pCommand->getCommonType()==eCommandTypeCycle)
								{
									if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintTranslate())
									{
										sprintf_s(chInfo,Q_FRAME_CONST128,"Read YC,Beyond command max try count,throw the frame and next,max count=%d\n",pCommand->getMaxTryCount());
										OnShowMsg(chInfo,strlen(chInfo)+1);
									}
									if(m_bNomalInfoMsg)
									{
										sprintf_s(chInfo,Q_FRAME_CONST128,"Read YC,Beyond command max try count,throw the frame and next,max count=%d\n",pCommand->getMaxTryCount());
										OnNomalInfoMsg(chInfo,strlen(chInfo)+1);
									}
									if(m_PrintSetting->getAlgCmdCount()>0)
									{
										if(pCommand->m_curIdx+1!=m_PrintSetting->getAlgCmdCount())
											pCommand->m_curIdx+=1;
										else
											pCommand->m_curIdx=0;

										pCommand->setDestinationId(m_PrintSetting->m_vCommand[pCommand->m_curIdx].device);
										pCommand->setObjectIndex(m_PrintSetting->m_vCommand[pCommand->m_curIdx].nBegin);
										pCommand->setIntValue(m_PrintSetting->m_vCommand[pCommand->m_curIdx].nCount);
										if(m_PrintSetting->m_vCommand[pCommand->m_curIdx].nType==1)
										{
											pCommand->setSwitchValue(true);//FLOAT类型
										}
										else
										{
											pCommand->setSwitchValue(false);//INT类型
										}
									}
									pCommand->setCommandModbusStatus(eModbusCommandSelect);
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
						else
						{
							if(pCommand->getCommonType()!=eCommandTypeCycle)
							{
								if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintError())
								{
									sprintf_s(chInfo,Q_FRAME_CONST128,"delete Read YC command.command status %d\n",pCommand->getCommandModbusStatus());
									OnShowMsg(chInfo,strlen(chInfo)+1);
								}
								if(m_bNomalInfoMsg)
								{
									sprintf_s(chInfo,Q_FRAME_CONST128,"delete Read YC command.command status %d\n",pCommand->getCommandModbusStatus());
									OnNomalInfoMsg(chInfo,strlen(chInfo)+1);
								}
								pCommand->setCommandModbusStatus(eModbusCommandSelect);
								pCommand->setCurTryNum(0);
							}
							else
							{
								//等待下一个执行时间
								if(pCommand->getCurTryNum()>=pCommand->getMaxTryCount())
								{
									setNextAnalog(pCommand);
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
							bJumpFlg=true;
						}
						else if(pCommand->getCommandModbusStatus() == eModbusCommandAct)
						{
							if(pCommand->getCurTryNum()>=pCommand->getMaxTryCount())
							{
								if(pCommand->getCommonType()==eCommandTypeCycle)
								{
									if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintTranslate())
									{
										sprintf_s(chInfo,Q_FRAME_CONST128,"Read YX value,Beyond command max try count,throw the frame and next,max count=%d\n",pCommand->getMaxTryCount());
										OnShowMsg(chInfo,strlen(chInfo)+1);
									}
									if(m_bNomalInfoMsg)
									{
										sprintf_s(chInfo,Q_FRAME_CONST128,"Read YX value,Beyond command max try count,throw the frame and next,max count=%d\n",pCommand->getMaxTryCount());
										OnNomalInfoMsg(chInfo,strlen(chInfo)+1);
									}
									if(m_PrintSetting->getPntCmdCount()>0)
									{
										if(pCommand->m_curIdx+1!=m_PrintSetting->getPntCmdCount())
											pCommand->m_curIdx+=1;
										else
											pCommand->m_curIdx=0;

										pCommand->setDestinationId(m_PrintSetting->m_vYXCommand[pCommand->m_curIdx].device);
										pCommand->setObjectIndex(m_PrintSetting->m_vYXCommand[pCommand->m_curIdx].nBegin);
										pCommand->setIntValue(m_PrintSetting->m_vYXCommand[pCommand->m_curIdx].nCount);
									}
									pCommand->setCommandModbusStatus(eModbusCommandSelect);
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
						else
						{
							if(pCommand->getCommonType()!=eCommandTypeCycle)
							{
								if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintError())
								{
									sprintf_s(chInfo,Q_FRAME_CONST128,"delete Read YX command.command status %d\n",pCommand->getCommandModbusStatus());
									OnShowMsg(chInfo,strlen(chInfo)+1);
								}
								if(m_bNomalInfoMsg)
								{
									sprintf_s(chInfo,Q_FRAME_CONST128,"delete Read YX command.command status %d\n",pCommand->getCommandModbusStatus());
									OnNomalInfoMsg(chInfo,strlen(chInfo)+1);
								}
								pCommand->setCommandModbusStatus(eModbusCommandSelect);
								pCommand->setCurTryNum(0);
							}
							else
							{
								//等待下一个执行时间
								if(pCommand->getCurTryNum()>=pCommand->getMaxTryCount())
								{
									setNextDigital(pCommand);
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
							bJumpFlg=true;
						}
						else if(pCommand->getCommandModbusStatus() == eModbusCommandAct)
						{
							if(pCommand->getCurTryNum()>=pCommand->getMaxTryCount())
							{
								if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
								{
									sprintf_s(chInfo,Q_FRAME_CONST128,"Beyond command max try count,delete command.max count=%d\n",pCommand->getMaxTryCount());
									OnShowMsg(chInfo,strlen(chInfo)+1);
								}
								if(true/*m_bInfoMsg*/)
								{
									sprintf_s(chInfo,Q_FRAME_CONST128,"Beyond command max try count,delete command.max count=%d\n",pCommand->getMaxTryCount());
									OnInfoMsg(chInfo,strlen(chInfo)+1,pCommand);
								}
								pManager->removeCommandByIndex(i);

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
							if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintError())
							{
								sprintf_s(chInfo,Q_FRAME_CONST128,"delete command.command status %d\n",pCommand->getCommandModbusStatus());
								OnShowMsg(chInfo,strlen(chInfo)+1);
							}
							if(true/*m_bInfoMsg*/)
							{
								sprintf_s(chInfo,Q_FRAME_CONST128,"delete command.command status %d\n",pCommand->getCommandModbusStatus());
								OnInfoMsg(chInfo,strlen(chInfo)+1,pCommand);
							}
							pManager->removeCommandByIndex(i);

						}
						break;
					}
					break;
				default:
					{
						pManager->removeCommandByIndex(i);
					}
				}
			}
			if(true==bJumpFlg)
			{
				break;
			}

		}/// next command
	}
	return true;
}
bool Q_protocolModbusRtuStd::handleFrameYX(Q_protocolModbusRtuStdCommand* pCommand)
{
	Q_MODBUS_ASDU_RTU asdu;
	memset(&asdu,NULL,sizeof(asdu));

	if(m_PrintSetting!=NULL)
	{
		asdu.funcode=m_PrintSetting->getReadYXRegisterCode();
	}
	else
	{
		asdu.funcode=MODBUS_YX;
	}
	asdu.addr=pCommand->getDestinationId()&0xFF;


	int startAddr=pCommand->getObjectIndex();

	asdu.data[0]=(startAddr>>8) & 0xFF;
	asdu.data[1]=startAddr&0xFF;

	int nCount=pCommand->getIntValue();

	asdu.data[2]=(nCount>>8 )& 0xFF;
	asdu.data[3]=nCount & 0xFF;

	WORD crc=CRC16((BYTE*)&asdu,6);

	asdu.data[4]=(crc)& 0xFF;
	asdu.data[5]=(crc >>8)& 0xFF;

	pCommand->setCommandModbusStatus(eModbusCommandAct);

	m_LastCmd=pCommand;

	return sendData(&asdu,8);

	

}

bool Q_protocolModbusRtuStd::handleFrameYC(Q_protocolModbusRtuStdCommand* pCommand)
{
	

	Q_MODBUS_ASDU_RTU asdu;
	memset(&asdu,NULL,sizeof(asdu));


	if(m_PrintSetting!=NULL)
	{
		asdu.funcode=m_PrintSetting->getReadRegisterCode();
	}
	else
	{
		asdu.funcode=MODBUS_YC;
	}
	asdu.addr=pCommand->getDestinationId()&0xFF;

	
	int startAddr=pCommand->getObjectIndex();

	asdu.data[0]=(startAddr>>8) & 0xFF;
	asdu.data[1]=startAddr&0xFF;

	int nCount=pCommand->getIntValue();

	asdu.data[2]=(nCount>>8 )& 0xFF;
	asdu.data[3]=nCount & 0xFF;

	WORD crc=CRC16((BYTE*)&asdu,6);

	asdu.data[4]=(crc)& 0xFF;
	asdu.data[5]=(crc >>8)& 0xFF;
	
	pCommand->setCommandModbusStatus(eModbusCommandAct);

	m_LastCmd=pCommand;

	return sendData(&asdu,8);

}
bool Q_protocolModbusRtuStd::handleFrameYT_M(Q_protocolModbusRtuStdCommand* pCommand)
{
	if(pCommand->getCommandModbusStatus() == eModbusCommandSelect)
	{
		int startAddr=pCommand->getObjectIndex();

		Q_MODBUS_ASDU_RTU asdu;
		memset(&asdu,NULL,sizeof(asdu));

		asdu.addr=pCommand->getDestinationId()&0xFF;
		//asdu.funcode=MODBUS_YT_M;
		asdu.funcode=m_PrintSetting->getYTCode();
		int  packLen=0;

		  

			Q_RECORDDATATAG ytRec;
			ytRec.eDataType=eDataTypeFloat;
			ytRec.channelID=m_pProcotolConfig->getProtocolSetting()->getChannelID();
			ytRec.device=pCommand->getDestinationId();
			ytRec.no=startAddr;
			ytRec.fValue=pCommand->getAnalogValue();//赋初值
			if(!m_pMgr->getYtValue(&ytRec))
			{
				sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
				SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);
				return false;
			}
			float val=ytRec.fValue;

			packLen=13;
			asdu.data[0]=startAddr>>8 &0xFF;
			asdu.data[1]=startAddr&0xFF;
			asdu.data[2]=0x00;
			asdu.data[3]=0x02;
			asdu.data[4]=0x04;

			//四字节浮点型

			// val在内存中是高字节在前，低字节在后，顺序为 3,2,1,0 转为2,3,0,1
			BYTE tmp[4];
			memset(tmp,NULL,sizeof(tmp));
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

			WORD crc=CRC16((BYTE*)&asdu,11);
			asdu.data[9]=(crc)& 0xFF;
			asdu.data[10]=(crc >>8)& 0xFF;

		pCommand->setCommandModbusStatus(eModbusCommandAct);
		sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command,device:%d register : %d value:%f",pCommand->getDestinationId(),startAddr,val);
		OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
		if(true/*m_bInfoMsg*/)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command,device:%d register : %d value:%f",pCommand->getDestinationId(),startAddr,val);
			OnInfoMsg(m_LastMsg,sizeof(m_LastMsg),pCommand);
		}
		m_LastCmdForCmd=pCommand;

		/*
		Q_dataManager* p = _GlobalData.getDataManager();
		LPQ_YTEDATATAG pYTData = p->getYTData();
		for (int i=0;i<p->getYTCount();i++)
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

bool Q_protocolModbusRtuStd::handleFrameYT_Int4(Q_protocolModbusRtuStdCommand* pCommand)
{
	if(pCommand->getCommandModbusStatus() == eModbusCommandSelect)
	{
		int startAddr=pCommand->getObjectIndex();

		Q_MODBUS_ASDU_RTU asdu;
		memset(&asdu,NULL,sizeof(asdu));

		asdu.addr=pCommand->getDestinationId()&0xFF;
		//asdu.funcode=MODBUS_YT;
		asdu.funcode = m_PrintSetting->getYTCode();
		asdu.data[0]=startAddr>>8 &0xFF;
		asdu.data[1]=startAddr&0xFF;

		asdu.data[2]=0x00;
		asdu.data[3]=0x02;
		asdu.data[4]=0x04;

		//float val=pCommand->getAnalogValue();

		Q_RECORDDATATAG ytRec;
		ytRec.eDataType=eDataTypeFloat;
		ytRec.channelID=m_pProcotolConfig->getProtocolSetting()->getChannelID();
		ytRec.device=pCommand->getDestinationId();
		ytRec.no=startAddr;
		ytRec.fValue=pCommand->getAnalogValue();//赋初值
		if(!m_pMgr->getYtValue(&ytRec))
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);
			return false;
		}

		int nVal=(int)ytRec.fValue;

		// val在内存中是高字节在前，低字节在后，顺序为 3,2,1,0 转为2,3,0,1
		BYTE tmp[4];
		memset(tmp,NULL,sizeof(tmp));
		memcpy(tmp,&nVal,sizeof(tmp));

		//两个寄存器内部，低位先传高位后传,同时寄存器内部高位在前，低位在后

		switch(m_IntUpSeq)
		{
		case eIntUpSeqHighFirst:
			asdu.data[5]=tmp[3];
			asdu.data[6]=tmp[2];
			asdu.data[7]=tmp[1];
			asdu.data[8]=tmp[0];
			break;
		case eIntUpSeqLowFirst:
			asdu.data[5]=tmp[0];
			asdu.data[6]=tmp[1];
			asdu.data[7]=tmp[2];
			asdu.data[8]=tmp[3];
			break;
		}

		WORD crc=CRC16((BYTE*)&asdu,11);

		asdu.data[9]=(crc)& 0xFF;
		asdu.data[10]=(crc >>8)& 0xFF;

		pCommand->setCommandModbusStatus(eModbusCommandAct);

		m_LastCmdForCmd=pCommand;

		if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
		{
			sprintf_s(chInfo,Q_FRAME_CONST128,"Send YT command,device:%d register : %d value:%f",pCommand->getDestinationId(),startAddr,ytRec.fValue);
			OnShowMsg(chInfo,strlen(chInfo)+1);
		}
		if(m_pMgr->handleMasterYt(&ytRec)!=0)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

		}
		else
		{
			sprintf_s(chInfo,Q_FRAME_CONST128,"Send YT command,device:%d register : %d value:%f",pCommand->getDestinationId(),startAddr,ytRec.fValue);
			OnShowMsg(chInfo,strlen(chInfo)+1);
		}

		return sendData(&asdu,13);
	}
	return true;
}

bool Q_protocolModbusRtuStd::handleFrameYT(Q_protocolModbusRtuStdCommand* pCommand)
{
	if(pCommand->getCommandModbusStatus() == eModbusCommandSelect)
	{
		int startAddr=pCommand->getObjectIndex();

		Q_MODBUS_ASDU_RTU asdu;
		memset(&asdu,NULL,sizeof(asdu));
	
		asdu.addr=pCommand->getDestinationId()&0xFF;
		//asdu.funcode=MODBUS_YT;
		asdu.funcode = m_PrintSetting->getYTCode();
		asdu.data[0]=startAddr>>8 &0xFF;
		asdu.data[1]=startAddr&0xFF;

		//float val=pCommand->getAnalogValue();
		//short nVal=(short)val;

		Q_RECORDDATATAG ytRec;
		ytRec.eDataType=eDataTypeFloat;
		ytRec.channelID=m_pProcotolConfig->getProtocolSetting()->getChannelID();
		ytRec.device=pCommand->getDestinationId();
		ytRec.no=startAddr;
		ytRec.fValue=pCommand->getAnalogValue();//赋初值
		if(!m_pMgr->getYtValue(&ytRec))
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);
			return false;
		}
		short nVal=(short)ytRec.fValue;

		// val在内存中是高字节在前，低字节在后，顺序为 3,2,1,0 转为2,3,0,1
		BYTE tmp[2];
		memset(tmp,NULL,sizeof(tmp));
		memcpy(tmp,&nVal,sizeof(tmp));

		//两个寄存器内部，低位先传高位后传,同时寄存器内部高位在前，低位在后

		switch(m_IntUpSeq)
		{
		case eIntUpSeqHighFirst:
			asdu.data[2]=tmp[1];
			asdu.data[3]=tmp[0];
			break;
		case eIntUpSeqLowFirst:
			asdu.data[2]=tmp[0];
			asdu.data[3]=tmp[1];
			break;
		}

		WORD crc=CRC16((BYTE*)&asdu,6);

		asdu.data[4]=(crc)& 0xFF;
		asdu.data[5]=(crc >>8)& 0xFF;

		pCommand->setCommandModbusStatus(eModbusCommandAct);

		m_LastCmdForCmd=pCommand;

		if(m_pMgr->handleMasterYt(&ytRec)!=0)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"Send YT command faild,did not find config.device:%d register : %d ",pCommand->getDestinationId(),startAddr);
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

		}
		else
		{
			sprintf_s(chInfo,Q_FRAME_CONST128,"Send YT command,device:%d register : %d value:%f",pCommand->getDestinationId(),startAddr,ytRec.fValue);
			OnShowMsg(chInfo,strlen(chInfo)+1);
		}

		return sendData(&asdu,8);
	}
	return true;

}
bool Q_protocolModbusRtuStd::handelFrameYK(Q_protocolModbusRtuStdCommand* pCommand)
{
	if(pCommand->getCommandModbusStatus() == eModbusCommandSelect)
	{
		int startAddr=pCommand->getObjectIndex();

		Q_MODBUS_ASDU_RTU asdu;
		memset(&asdu,NULL,sizeof(asdu));
		
		asdu.addr=pCommand->getDestinationId()&0xFF;
		asdu.funcode=m_PrintSetting->getYKCode();//写单个线圈

		asdu.data[0]=startAddr>>8 &0xFF;
		asdu.data[1]=startAddr&0xFF;

		bool bVal=pCommand->getIntValue()==0? false:true;

		if (true==bVal)
		{
			asdu.data[2]=0xff;
			asdu.data[3]=0x00;
		}
		else
		{
			asdu.data[2]=0x00;
			asdu.data[3]=0x00;
		}
		WORD crc=CRC16((BYTE*)&asdu,6);

		asdu.data[4]=(crc)& 0xFF;
		asdu.data[5]=(crc >>8)& 0xFF;

		pCommand->setCommandModbusStatus(eModbusCommandAct);

		m_LastCmdForCmd=pCommand;

		if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
		{
			sprintf_s(chInfo,Q_FRAME_CONST128,"Send YK command,device:%d register : %d value:%d",pCommand->getDestinationId(),startAddr,bVal);
			OnShowMsg(chInfo,strlen(chInfo)+1);
		}
		if(true/*m_bInfoMsg*/)
		{
			sprintf_s(chInfo,Q_FRAME_CONST128,"Send YK command,device:%d register : %d value:%d",pCommand->getDestinationId(),startAddr,bVal);
			OnInfoMsg(chInfo,strlen(chInfo)+1,pCommand);
		}
		Q_dataManager* p = _GlobalData.getDataManager();
		LPQ_YKEDATATAG pYKData = p->getYKData();
		for (int i=0;i<p->getYKCount();i++)
		{
			if (m_pProcotolConfig->getProtocolSetting()->getChannelID()  ==pYKData[i].channelid && pYKData[i].device==asdu.addr && pYKData[i].no==startAddr)
			{
				pYKData[i].bValue = bVal;
				pYKData[i].exectime = time(NULL);
				pYKData[i].finishtime = 0;
				pYKData[i].execresult = 0;
				break;
			}
		}
		return sendData(&asdu,8);
	}
	return true;
}
bool Q_protocolModbusRtuStd::makeFrameEx(Q_protocolModbusRtuStdCommand* pCommand)
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
		}
	}
	return false;

}
bool Q_protocolModbusRtuStd::terminateProtocol()
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
bool Q_protocolModbusRtuStd::startProcess()
{	
	if (Q_commonUtility::isBigEndian())
	{
		m_cpuEndian=eBigEndian;
	}
	else
	{
		m_cpuEndian=eLittleEndian;
	}
	m_LastCmd=NULL;
	m_LastCmdForCmd=NULL;



	m_PrintSetting= (Q_protcolModbusRtuStdSetting *)m_pProcotolConfig->getProtocolSetting();

	if(m_PrintSetting!=NULL)
	{
		m_FloatUpSeq=m_PrintSetting->getFloatUpSeq();
		m_YtMethod=m_PrintSetting->getYTMethod();
		m_IntUpSeq=m_PrintSetting->getIntUpSeq();
		m_CheckCodeSeq = m_PrintSetting->getCheckCodeSeq();
		m_CRCErroHandle = m_PrintSetting->getCRCErrorHandle();		
		int n=m_PrintSetting->getAlgCmdCount();//得到读遥测个数
		if(n>0)
		{
			
				Q_protocolModbusRtuStdCommand* pCommand = new Q_protocolModbusRtuStdCommand();
				pCommand->setCommonType(eCommandTypeCycle);

				pCommand->setDestinationId(m_PrintSetting->m_vCommand[0].device);
				pCommand->setMaxTryCount(m_PrintSetting->getPerTime());


				pCommand->setObjectIndex(m_PrintSetting->m_vCommand[0].nBegin);
				pCommand->setIntValue(m_PrintSetting->m_vCommand[0].nCount);

				if(m_PrintSetting->m_vCommand[0].nType==1)
					pCommand->setSwitchValue(true);//FLOAT类型
				else
					pCommand->setSwitchValue(false);//INT类型
				pCommand->m_curIdx=0;

				pCommand->setCommandModbusType(eModbusCommandMeasure);
				pCommand->setCommandModbusStatus(eModbusCommandSelect);

				m_pProcotolConfig->getProtocolCommandManager()->addCommand(pCommand);
			
		}
		else
		{
			int n=m_PrintSetting->getPntCmdCount();//得到读遥信个数
			if(n>0)
			{

				Q_protocolModbusRtuStdCommand* pCommand = new Q_protocolModbusRtuStdCommand();
				pCommand->setCommonType(eCommandTypeCycle);

				pCommand->setDestinationId(m_PrintSetting->m_vYXCommand[0].device);
				pCommand->setMaxTryCount(m_PrintSetting->getPerTime());


				pCommand->setObjectIndex(m_PrintSetting->m_vYXCommand[0].nBegin);
				pCommand->setIntValue(m_PrintSetting->m_vYXCommand[0].nCount);
				pCommand->m_curIdx=0;
				pCommand->setCommandModbusType(eModbusCommandPoint);
				pCommand->setCommandModbusStatus(eModbusCommandSelect);

				m_pProcotolConfig->getProtocolCommandManager()->addCommand(pCommand);

			}
		}
		
	}
	if(m_CRCErroHandle != 1)//如果不开启“连续10帧数据CRC校验错误是否判定为通道断开0否1是”功能
	{
		m_eProtocolStatus = eCommonStatusNormal; //lqq 20161227
	}
	return true;
}
bool Q_protocolModbusRtuStd::stopProcess()
{
	sprintf_s(m_LastMsg,sizeof(m_LastMsg),"断开链接...");
	OnShowMsg(m_LastMsg,sizeof(m_LastMsg));
	if(m_bNomalInfoMsg)
	{
		OnNomalInfoMsg(m_LastMsg,sizeof(m_LastMsg));
	}
	Q_dataManager* p = _GlobalData.getDataManager();
	LPQ_ANALOGUEDATATAG pYC = p->getAnalogData();
	LPQ_DIGITALEDATATAG pYX = p->getDigitalData();
	if (NULL!=pYC && m_PrintSetting->getAnalogEnd() < p->getAnalogCount())
	{
		for (int i = m_PrintSetting->getAnalogBegin();i<m_PrintSetting->getAnalogEnd()+1;i++)
		{
			/*if (pYC[i].datasource!=2)
			{
				pYC[i].quality = 2;
			}*/
		}
	}

	if (NULL!=pYX && m_PrintSetting->getPointEnd() < p->getDigitalCount())
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
bool Q_protocolModbusRtuStd::fillTagInformation(LPQ_RECORDDATATAG pData,int nAdr,int nDevID)
{
	/*Q_dataManager* p = _GlobalData.getDataManager();
	int nAddrBegin=0;
	int nAddrEnd=0;

	printf("fillTagInformation1\n");
	if(pData->eDataType!=eDataTypeBit)
	{
	
		if(m_PrintSetting!=NULL)
		{
			nAddrBegin=m_PrintSetting->getAnalogBegin();
			nAddrEnd=m_PrintSetting->getAnalogEnd();

		}
		printf("fillTagInformation2\n");
		printf("nDevID=%d,nAdr=%d,nAddrBegin=%d,nAddrEnd=%d\n", nDevID,nAdr,nAddrBegin, nAddrEnd );
		LPQ_ANALOGUEDATATAG alg=p->findAlgTag(nDevID,nAdr,nAddrBegin,nAddrEnd);
		if(alg != NULL)
		{
			printf("fillTagInformation3\n");
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
		LPQ_DIGITALEDATATAG  dig=p->findDigTag(nDevID,nAdr,nAddrBegin,nAddrEnd);
		if(dig!=NULL)
		{
			pData->nIndex = dig->nIndex;
			return true;
		}

	}
	printf("fillTagInformation8\n");
	return false;*/
	return true;
}
void Q_protocolModbusRtuStd::SetWndInfo(HWND wnd,int vMsgID)
{
	m_wnd = wnd;
	m_nMsgID=vMsgID;
}

void Q_protocolModbusRtuStd::onTxMsg(BYTE *msgbuf, unsigned int len)
{
	if(m_nMsgID>0)
	{
		if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPacket())
		{
			Q_COM_LOG_INFO log;
			log.chid=m_PrintSetting->getChannelID();
			log.type=eMsgTypeTx;
			log.msg=(char*)msgbuf;
			log.len=len;
			//::SendMessage(m_wnd,m_nMsgID,(WPARAM)&log,(LPARAM)sizeof(log));
		}
	}

}
void Q_protocolModbusRtuStd::onRxMsg(BYTE *msgbuf, unsigned int len)
{
	if(m_nMsgID>0)
	{
		if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPacket())
		{
			Q_COM_LOG_INFO log;
			log.chid=m_PrintSetting->getChannelID();
			log.type=eMsgTypeRx;
			log.msg=(char *)msgbuf;
			log.len=len;
			//::SendMessage(m_wnd,m_nMsgID,(WPARAM)&log,(LPARAM)sizeof(log));
		}
	}

}
void Q_protocolModbusRtuStd::OnErrorMsg(char *msg,unsigned int len)
{
	if(m_nMsgID>0)
	{
		if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintError())
		{
			Q_COM_LOG_INFO log;
			log.chid=m_PrintSetting->getChannelID();
			log.type=eMsgTypeError;
			log.msg=msg;
			log.len=len;
			//::SendMessage(m_wnd,m_nMsgID,(WPARAM)&log,(LPARAM)sizeof(log));
		}
	}
}
void Q_protocolModbusRtuStd::OnShowMsg(char *msg,unsigned int len)
{
	if(m_nMsgID>0)
	{
		if(m_PrintSetting!=NULL&&true==m_PrintSetting->getPrintPrompt())
		{
			Q_COM_LOG_INFO log;
			log.chid=m_PrintSetting->getChannelID();
			log.type=eMsgTypeMsg;
			log.msg=msg;
			log.len=len;
			//::SendMessage(m_wnd,m_nMsgID,(WPARAM)&log,(LPARAM)sizeof(log));
		}
	}
}
WORD Q_protocolModbusRtuStd::CRC16(BYTE* data, WORD len)
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
	if(m_CheckCodeSeq != 2)
	{
		return crc;
	}
	else
	{
		WORD crcDown = crc&0x00ff;
		WORD crcUp = crc&0xff00;
		WORD crcRevers = (crcDown << 8 ) | (crcUp >> 8);
		return crcRevers;
	}
}
void Q_protocolModbusRtuStd::SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen)
{
	//TRACE("Q_protocolIEC104Master::SendMsgPrint,g_rtdbInfIEC104=%08x",m_pMgr->getRtdbInf());
			
	//g_rtdbInfIEC104 = m_pMgr->getRtdbInf();
	//CFEPFuncRtdb *m_rtdbInf = g_rtdbInfIEC104;

	CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
	//如果不判断运状态就发命令，会把消息总线给堆满了

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
	else if (nmsgtype == eMsgTypeMsg || nmsgtype == eMsgTypeError || nmsgtype == eMsgTypeCh)//提示信息、信息值、错误消息、数据解释、规约解析
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
			sprintf_s(strMsg,sizeof(strMsg),"MsgPrint:stid = %d msgtype id = %d message = %s",MsgPrint.ichannelid,MsgPrint.msgtype,MsgPrint.msg);
			TRACE("strMsg");*/
		}
	}
}

#ifdef UNIX_ENV
#ifdef __cplusplus
}

#endif
#endif //OS_UNIX