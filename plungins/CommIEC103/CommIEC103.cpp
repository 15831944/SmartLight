// CommIEC104.cpp : Defines the exported functions for the DLL application.
//


#include "gSystemDef.h"
#include "tinyxmlinclude.h"
#include "Q_ProtocolIEC103Slave.h"
#include "Q_103SlaveTask.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Q_dataManager *m_pMgrIEC103Slave; /*ModbusSlave��rtdbȫ�ֱ���*/

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
		protocolSetting->setChannelID(v_param->cfg->id);//����ͨ��id
		protocolSetting->setChannelType(v_param->cfg->type);//����ͨ������

		//���Ϊ��̬ͨ���Ļ���Ӧ����ȫ�ֿ��Է��ʵġ�
		Q_103BaseCfg v_BaseCfg;
		protocolSetting->getBaseSettingEx(&v_BaseCfg);
		tcpServerLinkSetting->setPort(v_BaseCfg.localServer.port);

		Q_103BaseCfg v_BaseCfg2;
		protocolSetting->getBaseSetting(&v_BaseCfg2);
		
//��ʱ��ӡһ�¹��˵�IP����
	for(int i=0;i<Q_TCPSERVER_DEFCLIENTCOUNT;i++)
	{
		memcpy(tcpServerLinkSetting->m_FilterCleintIP[i].ip,v_BaseCfg2.remServer[i].ip,sizeof(tcpServerLinkSetting->m_FilterCleintIP[i].ip));
		tcpServerLinkSetting->m_FilterCleintIP[i].nChID=v_BaseCfg2.remServer[i].port;
		tcpServerLinkSetting->m_FilterCleintIP[i].valid=v_BaseCfg2.remServer[i].valid;
		TRACE("%d���˵�IP%s,�˿�%d\r\n",i+1,tcpServerLinkSetting->m_FilterCleintIP[i].ip,tcpServerLinkSetting->m_FilterCleintIP[i].nChID);
	}
////����
		//������������
		Q_103StandardTags *tags=new Q_103StandardTags();

		tags->setAnalogScope(v_param->cfg->YCbegin,v_param->cfg->YCend,v_param->cfg->YCend-v_param->cfg->YCbegin+1);
		tags->setDigitalScope(v_param->cfg->YXbegin,v_param->cfg->YXend,v_param->cfg->YXend-v_param->cfg->YXbegin+1);


		protocolSetting->setTagsManager(tags);




		m_pMgrIEC103Slave= v_param->m_pMgr;/*�趨ModbusSlave��rtdbȫ�ֱ���*/

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
		unitInfo->setRtdbInf(v_param->m_pMgr);/*�趨serverunitInfo��rtdbȫ�ֱ���*/

		tmp=protocolSetting->getContentID(20,1);
		int nWaitMs=5;
		if(tmp!=NULL)
		{
			nWaitMs=atoi(tmp);
		}


		unitInfo->setReadWriteWaitTime(nWaitMs);//5���룬����Ǵ������ݽϿ���ˣ�Ĭ��ԭ������Լ��50����
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

		chRet->taskServer=unitInfo;//��Tcp Server�����õ�20161215
		chRet->bTcpServer=true;

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
		TRACE("----------------iec103 add_command 12222 ");
		//if (rec->eDataType==eDataTypeBit)
		//{
		//	
		//	//yk  ��ʱ��֧��ң�� ����Ĵ�����104��Լ��ң�ش���
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
				pCommand->m_fVal = (rec->fValue);//����ֵ
			}
			else
			{
				pCommand->m_cOnOff = (short)(rec->nValue);
				pCommand->m_fVal = (float)(rec->nValue);//����ֵ
			}

			
			if(task->addProtocolCommand(pCommand))
				ret = 1;
		//}
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
		printf("=== 2017-1-8====\n");
		printf("1���������Ӻ���UDP֡����\n");
		printf("2������ң��ң��д�뷽��\n");
		printf("3�������豸��ַ���˹��ܣ�ʹ��һ��ͨ������̨�����������������\n");
		printf("4������IEC103Slave��Լ��ӡ��Լ���ļ���ʾ��Ϣ\n");
		printf("5�����˵�ַ����ȷ����\n");
		printf("=== 2017-1-8====\n");
		printf("1����UDP���͵�ʱ�������ӳɹ���λ��\n");
		printf("2�����ķ���UDP֡��ʽ\n");
		printf("=== 2017-1-10====\n");
		printf("1�����Ĳ��ִ�ӡ����ʾ��Ϣ�޷�������ʾ��ȱ��\n");
		printf("2��ȥ��m_ProSetting->getPrintSetting��صĴ��룬��Ϊ�˹���ͨ��������ʽ���棬\n");
		printf("3����ӡ��ʾ��Ϣ��OnShowMsg����SendMsgPrint\n");
		printf("4��ȥ��_GlobalData.getDataManager()������m_pMgrIEC103Slave����\n");
		printf("=== 2017-1-11====\n");
		printf("1������ִ��ģʽ\n");
		printf("2����ӡִ��״̬\n");
		printf("=== 2017-2-10====\n");
		printf("1�������˷������ٱ��ĵı������ݾ������Եĺ�ʵ���Ĺ�����\n");
		return 1;
	}

#ifdef UNIX_ENV
#ifdef __cplusplus
}
#endif
#endif //OS_UNIX

