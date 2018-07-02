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

	//��tabProtocol.xml�����ļ��е�strClassName�Ƚϣ��������0��Ϊ��վ
	int nMasterSlave = strcmp(chCName,v_param->cfg->classname);
	//if (nchanneltype == 1)//IEC104��վ
	if (nMasterSlave == 0)//IEC104��վ
	{
		Q_chDownReturn *chRet=new Q_chDownReturn;

		int  n_sleepMills=600;/*�̵߳�ִ��ʱ�䣬���Ͳ���֡*/

		Q_taskBase* taskTest = new Q_taskBase();
		taskTest->setBackupMode(eCommonBackupCold);
		taskTest->setRtdbInf(v_param->m_pMgr);/*�趨Q_taskBase�е�rtdbȫ�ֱ���*/
		bool flgPri = false;

		Q_IEC104Setting  pSetting;
		pSetting.setFilePath(v_param->strFilePath);
		pSetting.initSetting();/*����xml���ݣ���ʼ��Э������*/

		Q_104BaseCfg v_BaseCfg;
		pSetting.getBaseSetting(&v_BaseCfg);

		for(int i=0;i<MAX_REMOTE_NUM;i++)
		{
			if(v_BaseCfg.remServer[i].valid)
			{
				/*��д�ڴ�*/
				Q_commonBuffer* bufferReceive = new Q_commonBuffer();
				Q_commonBuffer* bufferSend = new Q_commonBuffer();

				/*��·����*/
				Q_tcpClientLinkSetting* tLinkSetting = new Q_tcpClientLinkSetting();
				tLinkSetting->setServerIp(v_BaseCfg.remServer[i].ip);
				tLinkSetting->setPort(v_BaseCfg.remServer[i].port);

				Q_linkConfig* tLinkConfig = new Q_linkConfig();
				tLinkConfig->setLinkSetting(tLinkSetting);
				tLinkConfig->setReceiveBuffer(bufferReceive);
				tLinkConfig->setSendBuffer(bufferSend);

				/*Э������*/
				Q_IEC104Setting  *iecSetting=new  Q_IEC104Setting();
				iecSetting->setFilePath(v_param->strFilePath);
				iecSetting->initSetting();/*����xml���ݣ���ʼ��Э������*/

				Q_protocolConfig* protocolConfig = new Q_protocolConfig();
				protocolConfig->setProtocolSetting(iecSetting);
				protocolConfig->setReceiveBuffer(bufferReceive);
				protocolConfig->setSendBuffer(bufferSend);

				Q_IEC104CommandManager* pCommandList = new Q_IEC104CommandManager();
				protocolConfig->setProtocolCommandManager(pCommandList);

				/*������������*/
				Q_104StandardTags *tags=new Q_104StandardTags();
				tags->setAnalogScope(v_param->cfg->YCbegin,v_param->cfg->YCend,(v_param->cfg->YCend-v_param->cfg->YCbegin+1));
				tags->setDigitalScope(v_param->cfg->YXbegin,v_param->cfg->YXend,(v_param->cfg->YXend-v_param->cfg->YXbegin+1));
				iecSetting->setTagsManager(tags);

				/*Э��*/	
				Q_protocolIEC104Master* protocol = new Q_protocolIEC104Master();
				protocol->setThreadPeriod(n_sleepMills);/*�趨�̵߳�ִ��ʱ���Ա�ּ��㷢�Ͳ���֡��ʱ��*/
				protocol->setRtdbInf(v_param->m_pMgr); /*�趨Q_prtocolBase�е�rtdbȫ�ֱ���*/
				
				iecSetting->setChannelID(v_param->cfg->id);/*����ͨ��id*/
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
	else/*IEC104��վ*/
	{
		Q_chDownReturn *chRet=new Q_chDownReturn;

		bool flgPri=true;

		/*tcp server link*/
		Q_tcpServerLink* tcpServerLink = new Q_tcpServerLink();
		Q_tcpServerLinkSetting* tcpServerLinkSetting = new Q_tcpServerLinkSetting();
		tcpServerLinkSetting->setAnyIp(true);
		
		Q_IEC104Setting *iecSetting= new Q_IEC104Setting();
		iecSetting->setFilePath(v_param->strFilePath);
		iecSetting->initSetting();/*����xml���ݣ���ʼ��Э������*/
		iecSetting->setChannelID(v_param->cfg->id);
		iecSetting->setChannelType(v_param->cfg->type);
		iecSetting->setChannelClass(v_param->cfg->classname);
		//iecSetting
		/*tags config*/
		Q_104BaseCfg v_BaseCfg;
		iecSetting->getBaseSetting(&v_BaseCfg);
		//IP��·�����
		memcpy(tcpServerLinkSetting->m_FilterCleintIP,v_BaseCfg.remServer,sizeof(tcpServerLinkSetting->m_FilterCleintIP));

		Q_104StandardTags *tags=new Q_104StandardTags();

		if (v_param->cfg->type == 3)//��̬ͨ��
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

		m_pMgrIEC104SlaveProtocolSetting = v_param->m_pMgr;/*�趨iec104slave�е�rtdbȫ�ֱ���*/

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
		Q_104SlaveTask* tcpServerTask = new Q_104SlaveTask();//deleteQ_tcpServerUnitInfo������delete m_tcpServerTask
		tcpServerTask->setTaskID(v_param->cfg->id);
		tcpServerTask->setTcpServerLink(tcpServerLink);
		tcpServerTask->setProtocolSetting(iecSetting);

		/*server unit info*/
		Q_tcpServerUnitInfo* unitInfo = new Q_tcpServerUnitInfo();//delete��Q_tcpServerRunningUnit������delete m_serverUnitInfo
		unitInfo->setServerTask(tcpServerTask);
		unitInfo->setRtdbInf(v_param->m_pMgr);//fs

		/*server thread*/
		Q_tcpServerListenThread* listenThread = new Q_tcpServerListenThread();//delete��Q_tcpServerRunningUnit������delete m_listenThread
		listenThread->setThreadPeriod(500);
		listenThread->setServerUnitInfo(unitInfo);

		Q_tcpServerCheckThread* checkThread = new Q_tcpServerCheckThread();//delete��Q_tcpServerRunningUnit������delete m_checkThread
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


/*Ϊ�˱�֤���ٵĶ�̬�����ͷſ��һ�£����Ӵ˷���*/
void OUTPUTAPI free_protocol_object(Q_chDownReturn* pProtocol)
{
	if ((Q_chDownReturn*)pProtocol)
	{
		delete (Q_chDownReturn*)pProtocol;
		pProtocol = NULL;
	}
}


/*������ң����ң��*/
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
			pCommand->m_fVal = (rec->fValue);//����ֵ
		}
		else
		{
			pCommand->m_cOnOff = (short)(rec->nValue);
			pCommand->m_fVal = (float)(rec->nValue);//����ֵ
		}

		pCommand->setCommand104Type(eCmdTypeC_SE);
		pCommand->m_eState=eCmdStateYkSelect;

		task->addProtocolCommand(pCommand);
	}
	return ret;
}
//��ӡ���¼�¼
int  OUTPUTAPI print_change_log()
{
	printf("=== CommIEC104 Protocol Dynamic lib====\n");
	printf("=== 2016-07-22====\n");
	printf("1�����Ӵ�ӡ����汾����\n");
	printf("=== 2016-10-28====\n");
	printf("1�����ķ�����Ϣ������ȥ��ȫ��ͬ��������Ϣ�Ĺ��ܣ�ֻ�������ͼ���\n");
	printf("=== 2016-11-8====\n");
	printf("1������IEC104��վʱ�����ң�⡢ң�ű�û�����ô�ͨ�������ݣ��ϴ�ʱ��������\n");
	printf("=== 2016-11-18====\n");
	printf("1������ͨ�����,֧��IEC104��վ��Լ��ͨ������ʱ�����ͨ���е����ݿ��Բ�ͬ\n");
	printf("2���ָ�104��վ��Լ��ң��ң�ر��Ĵ�ӡ\n");
	printf("3��������վ�ϴ�SOE�ź�\n");
	printf("=== 2016-11-19===\n");
	printf("1��ͨ�����˸�Ϊ8����ԭ��Ϊ10������8��ͨ��ȫ�����ϵ�ʱ�򣬻�ѵ�filed idΪ20��Ҳ����ip��ַΪ�����������\n");
	printf("2��֧�ֵ�ǰ����·��������\n");
	printf("=== 2016-11-24===\n");
	printf("1�����ȷ��ͱ���ʱ���ȼ�ⵥԪ���ټ������Ƿ����\n");
	printf("=== 2016-11-25====\n");
	printf("1�������Ż�������Ϣ�����ݰѹ̶�ֵ�ĳ�strlen()+1�ķ�ʽ\n");
	return 1;
}
#ifdef UNIX_ENV
#ifdef __cplusplus
}

#endif
#endif //OS_UNIX

