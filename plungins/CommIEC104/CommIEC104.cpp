// CommIEC104.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "gSystemDef.h"
#include "Q_104SlaveTask.h"
#include "Q_ProtocolIEC104Slave.h"
#include "Q_ProtocolIEC104Master.h"
#include "tinyxmlinclude.h"

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
	//int nchanneltype = 1;
	//nchanneltype = v_param->cfg->nchanneltype;
	//v_param->cfg
	char chCName[50];
	string strClassNameM = "CommIEC104Master";
	int a = 0;
	int nstrlen = strClassNameM.length();
	for(a=0; a<nstrlen; a++)
	{
		chCName[a] = strClassNameM[a];
	}
	chCName[a] = '\0';

	//与tabProtocol.xml配置文件中的strClassName比较，结果等于0即为主站
	int nMasterSlave = strcmp(chCName,v_param->cfg->classname);
	//if (nchanneltype == 1)//IEC104主站
	if (nMasterSlave == 0)//IEC104主站
	{
		Q_chDownReturn *chRet=new Q_chDownReturn;

		int  n_sleepMills=600;/*线程的执行时间，发送测试帧*/

		Q_taskBase* taskTest = new Q_taskBase();
		taskTest->setBackupMode(eCommonBackupCold);
		taskTest->setRtdbInf(v_param->m_pMgr);/*设定Q_taskBase中的rtdb全局变量*/
		bool flgPri = false;

		Q_IEC104Setting  pSetting;
		pSetting.setFilePath(v_param->strFilePath);
		pSetting.initSetting();/*解析xml内容，初始化协议配置*/

		Q_104BaseCfg v_BaseCfg;
		pSetting.getBaseSetting(&v_BaseCfg);

		for(int i=0;i<MAX_REMOTE_NUM;i++)
		{
			if(v_BaseCfg.remServer[i].valid)
			{
				/*读写内存*/
				Q_commonBuffer* bufferReceive = new Q_commonBuffer();
				Q_commonBuffer* bufferSend = new Q_commonBuffer();

				/*链路配置*/
				Q_tcpClientLinkSetting* tLinkSetting = new Q_tcpClientLinkSetting();
				tLinkSetting->setServerIp(v_BaseCfg.remServer[i].ip);
				tLinkSetting->setPort(v_BaseCfg.remServer[i].port);

				Q_linkConfig* tLinkConfig = new Q_linkConfig();
				tLinkConfig->setLinkSetting(tLinkSetting);
				tLinkConfig->setReceiveBuffer(bufferReceive);
				tLinkConfig->setSendBuffer(bufferSend);

				/*协议配置*/
				Q_IEC104Setting  *iecSetting=new  Q_IEC104Setting();
				iecSetting->setFilePath(v_param->strFilePath);
				iecSetting->initSetting();/*解析xml内容，初始化协议配置*/

				Q_protocolConfig* protocolConfig = new Q_protocolConfig();
				protocolConfig->setProtocolSetting(iecSetting);
				protocolConfig->setReceiveBuffer(bufferReceive);
				protocolConfig->setSendBuffer(bufferSend);

				Q_IEC104CommandManager* pCommandList = new Q_IEC104CommandManager();
				protocolConfig->setProtocolCommandManager(pCommandList);

				/*数据索引配置*/
				Q_104StandardTags *tags=new Q_104StandardTags();
				tags->setAnalogScope(v_param->cfg->YCbegin,v_param->cfg->YCend,(v_param->cfg->YCend-v_param->cfg->YCbegin+1));
				tags->setDigitalScope(v_param->cfg->YXbegin,v_param->cfg->YXend,(v_param->cfg->YXend-v_param->cfg->YXbegin+1));
				iecSetting->setTagsManager(tags);

				/*协议*/	
				Q_protocolIEC104Master* protocol = new Q_protocolIEC104Master();
				protocol->setThreadPeriod(n_sleepMills);/*设定线程的执行时间以便粗计算发送测试帧的时间*/
				protocol->setRtdbInf(v_param->m_pMgr); /*设定Q_prtocolBase中的rtdb全局变量*/
				
				iecSetting->setChannelID(v_param->cfg->id);/*设置通道id*/
				iecSetting->setChannelType(v_param->cfg->type);

				Q_tcpClientLink* linkTcp = new Q_tcpClientLink();
				Q_channelBase* channelTest = new Q_channelBase();
				channelTest->setChannelID(v_param->cfg->id);

				if(!flgPri)
				{
					channelTest->setPrimaryFlag(true);
				}
				else
				{
					flgPri=true;
					channelTest->setPrimaryFlag(false);
				}

				channelTest->setReceiveBuffer(bufferReceive);
				channelTest->setSendBuffer(bufferSend);
				channelTest->setLinkSetting(tLinkSetting);
				channelTest->setLinkConfig(tLinkConfig);
				channelTest->setLink(linkTcp);
				channelTest->setProtocolSetting(iecSetting);
				channelTest->setProtocolConfig(protocolConfig);
				channelTest->setProtocol(protocol);	
				channelTest->setCommandList(pCommandList);

				taskTest->addChannel(channelTest);

				Q_simpleThread* threadTest = new Q_simpleThread();
				threadTest->setThreadPeriod(n_sleepMills);
				threadTest->addTask(taskTest);

				Q_taskRunningUnit* unit = new Q_taskRunningUnit();
				unit->setDriverMode(eCommonModeSingle);
				//unit->addTask(taskTest);
				//unit->addThread(threadTest);
				unit->addTask(taskTest);
				unit->addThread(threadTest);

				chRet->unit=unit;
				chRet->task=taskTest;
				sprintf_s(chRet->protocol,sizeof(v_param->cfg->protocol),v_param->cfg->protocol);
				chRet->channelID=v_param->cfg->id;
				chRet->enabled=v_param->cfg->enabled;

				if(chRet)
					return chRet;	
				else
					return NULL;
			}
		}
	}
	else/*IEC104子站*/
	{
		Q_chDownReturn *chRet=new Q_chDownReturn;

		bool flgPri=true;

		/*tcp server link*/
		Q_tcpServerLink* tcpServerLink = new Q_tcpServerLink();
		Q_tcpServerLinkSetting* tcpServerLinkSetting = new Q_tcpServerLinkSetting();
		tcpServerLinkSetting->setAnyIp(true);
		
		Q_IEC104Setting *iecSetting= new Q_IEC104Setting();
		iecSetting->setFilePath(v_param->strFilePath);
		iecSetting->initSetting();/*解析xml内容，初始化协议配置*/
		iecSetting->setChannelID(v_param->cfg->id);
		iecSetting->setChannelType(v_param->cfg->type);
		iecSetting->setChannelClass(v_param->cfg->classname);
		//iecSetting
		/*tags config*/
		Q_104BaseCfg v_BaseCfg;
		iecSetting->getBaseSetting(&v_BaseCfg);
		//IP链路层过滤
		memcpy(tcpServerLinkSetting->m_FilterCleintIP,v_BaseCfg.remServer,sizeof(tcpServerLinkSetting->m_FilterCleintIP));

		Q_104StandardTags *tags=new Q_104StandardTags();

		if (v_param->cfg->type == 3)//组态通道
		{
			int nZTChlYCBegin = iecSetting->getZTChlYCBegin();
			int nZTChlYXBegin =	iecSetting->getZTChlYXBegin();

			int nAnalogRecordCount = v_param->m_pMgr->getRtdbInf()->getScadaDb()->GetTableRecordCount(SCADA_TAB_Analog_ID);
			int nDigitRecordCount = v_param->m_pMgr->getRtdbInf()->getScadaDb()->GetTableRecordCount(SCADA_TAB_Digit_ID);

			tags->setAnalogScope(1,nAnalogRecordCount,nAnalogRecordCount);
			tags->setDigitalScope(1,nDigitRecordCount,nDigitRecordCount);
		}
		else
		{
			tags->setAnalogScope(v_param->cfg->YCbegin,v_param->cfg->YCend,(v_param->cfg->YCend-v_param->cfg->YCbegin+1));
			tags->setDigitalScope(v_param->cfg->YXbegin,v_param->cfg->YXend,(v_param->cfg->YXend-v_param->cfg->YXbegin+1));
		}

		iecSetting->setTagsManager(tags);

		m_pMgrIEC104SlaveProtocolSetting = v_param->m_pMgr;/*设定iec104slave中的rtdb全局变量*/

		char *tmp=NULL;

		tmp=iecSetting->getContentID(2,1);
		if(tmp!=NULL)
		{
			tcpServerLinkSetting->setServerIp(tmp);
		}

		tmp=iecSetting->getContentID(3,1);
		if(tmp!=NULL)
		{
			tcpServerLinkSetting->setPort(atoi(tmp));
		}

		tmp= iecSetting->getContentID(5,10);
		if(tmp != NULL)
		{
			tcpServerLink->setLogStatus(atoi(tmp));
		}

		tcpServerLink->setLinkSetting(tcpServerLinkSetting);

		/*tcp server task*/
		Q_104SlaveTask* tcpServerTask = new Q_104SlaveTask();//deleteQ_tcpServerUnitInfo析构中delete m_tcpServerTask
		tcpServerTask->setTaskID(v_param->cfg->id);
		tcpServerTask->setTcpServerLink(tcpServerLink);
		tcpServerTask->setProtocolSetting(iecSetting);

		/*server unit info*/
		Q_tcpServerUnitInfo* unitInfo = new Q_tcpServerUnitInfo();//delete在Q_tcpServerRunningUnit析构中delete m_serverUnitInfo
		unitInfo->setServerTask(tcpServerTask);
		unitInfo->setRtdbInf(v_param->m_pMgr);//fs

		/*server thread*/
		Q_tcpServerListenThread* listenThread = new Q_tcpServerListenThread();//delete在Q_tcpServerRunningUnit析构中delete m_listenThread
		listenThread->setThreadPeriod(500);
		listenThread->setServerUnitInfo(unitInfo);

		Q_tcpServerCheckThread* checkThread = new Q_tcpServerCheckThread();//delete在Q_tcpServerRunningUnit析构中delete m_checkThread
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

	return NULL;
}


/*为了保证开辟的动态库与释放库的一致，增加此方法*/
void OUTPUTAPI free_protocol_object(Q_chDownReturn* pProtocol)
{
	if ((Q_chDownReturn*)pProtocol)
	{
		delete (Q_chDownReturn*)pProtocol;
		pProtocol = NULL;
	}
}


/*添加命令，遥调、遥控*/
int OUTPUTAPI add_command(Q_taskBase *task,LPQ_RECORDDATATAG rec)
{
	int ret=0;

	if (rec->eDataType==eDataTypeBit)
	{
		Q_cmdIEC104_SP *pCommand= new Q_cmdIEC104_SP();
		pCommand->setCommonType(eCommandTypeDialog);
		pCommand->m_nComAsdu=(rec->device);

		pCommand->m_nInfoAddr=(rec->no);
		pCommand->m_cOnOff=(rec->bValue);
		pCommand->setCommand104Type(eCmdTypeC_SC);
		pCommand->m_eState=eCmdStateYkSelect;

		task->addProtocolCommand(pCommand);

	}	
	else
	{
		Q_cmdIEC104_SE *pCommand = new Q_cmdIEC104_SE();
		pCommand->setCommonType(eCommandTypeDialog);
		pCommand->m_nComAsdu=(rec->device);
		pCommand->m_nInfoAddr=(rec->no);

		if(rec->eDataType==eDataTypeFloat)
		{
			pCommand->m_cOnOff = (short)(rec->fValue);
			pCommand->m_fVal = (rec->fValue);//调节值
		}
		else
		{
			pCommand->m_cOnOff = (short)(rec->nValue);
			pCommand->m_fVal = (float)(rec->nValue);//调节值
		}

		pCommand->setCommand104Type(eCmdTypeC_SE);
		pCommand->m_eState=eCmdStateYkSelect;

		task->addProtocolCommand(pCommand);
	}
	return ret;
}
//打印更新记录
int  OUTPUTAPI print_change_log()
{
	printf("=== CommIEC104 Protocol Dynamic lib====\n");
	printf("=== 2016-07-22====\n");
	printf("1、增加打印输入版本功能\n");
	printf("=== 2016-10-28====\n");
	printf("1、更改发送消息方法，去掉全网同步报文消息的功能，只本机发送即可\n");
	printf("=== 2016-11-8====\n");
	printf("1、更新IEC104子站时，如果遥测、遥信表没有配置此通道的数据，上传时跳过数据\n");
	printf("=== 2016-11-18====\n");
	printf("1、更改通道检查,支持IEC104子站规约多通道上送时，多个通道中的数据可以不同\n");
	printf("2、恢复104子站规约中遥调遥控报文打印\n");
	printf("3、增加子站上传SOE信号\n");
	printf("=== 2016-11-19===\n");
	printf("1、通道过滤改为8个，原来为10个，当8个通道全部用上的时候，会把第filed id为20的也当成ip地址为过滤引起崩溃\n");
	printf("2、支持当前工程路径可配置\n");
	printf("=== 2016-11-24===\n");
	printf("1、优先发送报文时优先检测单元表再检查进程是否存在\n");
	printf("=== 2016-11-25====\n");
	printf("1、更改优化发送消息的内容把固定值改成strlen()+1的方式\n");
	return 1;
}
#ifdef UNIX_ENV
#ifdef __cplusplus
}

#endif
#endif //OS_UNIX

