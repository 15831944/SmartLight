// CommIEC104.cpp : Defines the exported functions for the DLL application.
//


#include "gSystemDef.h"
#include "tinyxmlinclude.h"
#include "Q_ProtocolIEC103Slave.h"
#include "Q_103SlaveTask.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Q_dataManager *m_pMgrIEC103Slave; /*ModbusSlave的rtdb全局变量*/

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

		//1.tcp server link	
		Q_tcpServerLink* tcpServerLink = new Q_tcpServerLink();
		Q_tcpServerLinkSetting* tcpServerLinkSetting = new Q_tcpServerLinkSetting();
		tcpServerLinkSetting->setAnyIp(true);
		tcpServerLink->setLinkSetting(tcpServerLinkSetting);

		//2.protocol
		/*protocolsetting config*/
		Q_IEC103Setting *protocolSetting = new Q_IEC103Setting();
		protocolSetting->setFilePath(v_param->strFilePath);
		protocolSetting->initSetting();
		protocolSetting->setChannelID(v_param->cfg->id);//设置通道id
		protocolSetting->setChannelType(v_param->cfg->type);//设置通道类型

		//如果为组态通道的话，应该是全局可以访问的。
		Q_103BaseCfg v_BaseCfg;
		protocolSetting->getBaseSettingEx(&v_BaseCfg);
		tcpServerLinkSetting->setPort(v_BaseCfg.localServer.port);

		Q_103BaseCfg v_BaseCfg2;
		protocolSetting->getBaseSetting(&v_BaseCfg2);
		
//临时打印一下过滤的IP内容
	for(int i=0;i<Q_TCPSERVER_DEFCLIENTCOUNT;i++)
	{
		memcpy(tcpServerLinkSetting->m_FilterCleintIP[i].ip,v_BaseCfg2.remServer[i].ip,sizeof(tcpServerLinkSetting->m_FilterCleintIP[i].ip));
		tcpServerLinkSetting->m_FilterCleintIP[i].nChID=v_BaseCfg2.remServer[i].port;
		tcpServerLinkSetting->m_FilterCleintIP[i].valid=v_BaseCfg2.remServer[i].valid;
		TRACE("%d过滤的IP%s,端口%d\r\n",i+1,tcpServerLinkSetting->m_FilterCleintIP[i].ip,tcpServerLinkSetting->m_FilterCleintIP[i].nChID);
	}
////结束
		//数据索引配置
		Q_103StandardTags *tags=new Q_103StandardTags();

		tags->setAnalogScope(v_param->cfg->YCbegin,v_param->cfg->YCend,v_param->cfg->YCend-v_param->cfg->YCbegin+1);
		tags->setDigitalScope(v_param->cfg->YXbegin,v_param->cfg->YXend,v_param->cfg->YXend-v_param->cfg->YXbegin+1);


		protocolSetting->setTagsManager(tags);




		m_pMgrIEC103Slave= v_param->m_pMgr;/*设定ModbusSlave的rtdb全局变量*/

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

		/*103 tcp server task*/
		Q_103SlaveTask* tcpServerTask = new Q_103SlaveTask();
		tcpServerTask->setTaskID(v_param->cfg->id);
		tcpServerTask->setTcpServerLink(tcpServerLink);
		tcpServerTask->setProtocolSetting(protocolSetting);



		/*server unit info*/
		Q_tcpServerUnitInfo* unitInfo = new Q_tcpServerUnitInfo();
		unitInfo->setServerTask(tcpServerTask);
		unitInfo->setRtdbInf(v_param->m_pMgr);/*设定serverunitInfo的rtdb全局变量*/

		tmp=protocolSetting->getContentID(20,1);
		int nWaitMs=5;
		if(tmp!=NULL)
		{
			nWaitMs=atoi(tmp);
		}


		unitInfo->setReadWriteWaitTime(nWaitMs);//5毫秒，这个是处理数据较快的了，默认原电力规约是50毫秒
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

		chRet->unit=unit;				
		sprintf_s(chRet->protocol,sizeof(v_param->cfg->protocol),"%s",v_param->cfg->protocol);
		chRet->channelID=v_param->cfg->id;
		chRet->enabled=v_param->cfg->enabled;	

		chRet->taskServer=unitInfo;//给Tcp Server特殊用的20161215
		chRet->bTcpServer=true;

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
		TRACE("----------------iec103 add_command 12222 ");
		//if (rec->eDataType==eDataTypeBit)
		//{
		//	
		//	//yk  暂时不支持遥控 下面的代码是104规约的遥控代码
		//	/*Q_cmdIEC104_SP *pCommand= new Q_cmdIEC104_SP();
		//	pCommand->setCommonType(eCommandTypeDialog);
		//	pCommand->m_nComAsdu=(rec->device);

		//	pCommand->m_nInfoAddr=(rec->no);
		//	pCommand->m_cOnOff=(rec->bValue);
		//	pCommand->setCommand104Type(eCmdTypeC_SC);
		//	pCommand->m_eState=eCmdStateYkSelect;

		//	task->addProtocolCommand(pCommand);*/

		//}	
		//else
		//{
			//yt
			Q_cmdIEC103_SE *pCommand= new Q_cmdIEC103_SE();
			pCommand->setCommonType(eCommandTypeDialog);
			pCommand->m_nComAsdu=rec->device;
			pCommand->m_nInfoAddr=rec->no;
			pCommand->setCommand103Type(e103CmdTypeYT);
			pCommand->m_eState=e103CmdStateYTSelect;

			TRACE("----------------iec103 add_command 1,device=%d,no=%dval=%.2f \r\n ",rec->device,rec->no,rec->fValue);
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

			
			if(task->addProtocolCommand(pCommand))
				ret = 1;
		//}
		return ret;
	}
	//打印更新记录
	int  OUTPUTAPI print_change_log()
	{
		printf("=== CommModbusTcpServerStand Protocol Dynamic lib====\n");
		printf("=== 2016-07-22====\n");
		printf("1、增加打印输入版本功能\n");
		printf("=== 2016-10-28====\n");
		printf("1、更改发送消息方法，去掉全网同步报文消息的功能，只本机发送即可\n");
		printf("=== 2016-11-25====\n");
		printf("1、更改优化发送消息的内容把固定值改成strlen()+1的方式\n");
		printf("=== 2017-1-8====\n");
		printf("1、开启连接后发送UDP帧功能\n");
		printf("2、更改遥测遥信写入方法\n");
		printf("3、启用设备地址过滤功能，使用一条通道的两台逆变器都可正常采数\n");
		printf("4、更改IEC103Slave规约打印规约报文及提示信息\n");
		printf("5、过滤地址不正确修正\n");
		printf("=== 2017-1-8====\n");
		printf("1、把UDP发送的时间在连接成功的位置\n");
		printf("2、更改发送UDP帧方式\n");
		printf("=== 2017-1-10====\n");
		printf("1、更改部分打印的提示信息无法正常显示的缺陷\n");
		printf("2、去掉m_ProSetting->getPrintSetting相关的代码，因为此功能通过其它方式代替，\n");
		printf("3、打印提示信息用OnShowMsg不用SendMsgPrint\n");
		printf("4、去掉_GlobalData.getDataManager()代码用m_pMgrIEC103Slave代替\n");
		printf("=== 2017-1-11====\n");
		printf("1、增加执行模式\n");
		printf("2、打印执行状态\n");
		printf("=== 2017-2-10====\n");
		printf("1、更改了发送总召报文的报文内容经与南自的核实而改过来的\n");
		return 1;
	}

#ifdef UNIX_ENV
#ifdef __cplusplus
}
#endif
#endif //OS_UNIX

