
#include "gSystemDef.h"
//#include "Q_ProtocolIEC104Master.h"
#include "Q_frameInclude.h"
#include "Q_runingUnitDefine.h"
#include "Q_protocolIEC101.h"
#include "Q_protocolIEC101Slave.h"


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

		Q_chDownReturn *chRet=new Q_chDownReturn;

		int  n_sleepMills=500;/*�̵߳�ִ��ʱ�䣬���Ͳ���֡*/

		Q_taskBase* taskTest = new Q_taskBase();
		taskTest->setBackupMode(eCommonBackupCold);
		taskTest->setRtdbInf(v_param->m_pMgr);/*�趨Q_taskBase�е�rtdbȫ�ֱ���*/
		bool flgPri = false;

		Q_IEC104Setting  pSetting;
		pSetting.setFilePath(v_param->strFilePath);
		pSetting.initSetting();/*����xml���ݣ���ʼ��Э������*/

		Q_104BaseCfg v_BaseCfg;
		pSetting.getBaseSetting(&v_BaseCfg);

	
		
				/*��д�ڴ�*/
				Q_commonBuffer* bufferReceive = new Q_commonBuffer();
				Q_commonBuffer* bufferSend = new Q_commonBuffer();

				/*��·����*/

				//Q_tcpClientLinkSetting* tLinkSetting = new Q_tcpClientLinkSetting();
				//tLinkSetting->setServerIp(v_BaseCfg.remServer[i].ip);
				//tLinkSetting->setPort(v_BaseCfg.remServer[i].port);

				Q_linkConfig* tLinkConfig = new Q_linkConfig();
			//	tLinkConfig->setLinkSetting(tLinkSetting);
				tLinkConfig->setReceiveBuffer(bufferReceive);
				tLinkConfig->setSendBuffer(bufferSend);



				/*Э������*/
				Q_IEC104Setting  *iecSetting=new  Q_IEC104Setting();
				iecSetting->setFilePath(v_param->strFilePath);
				iecSetting->initSetting();/*����xml���ݣ���ʼ��Э������*/
				iecSetting->setChannelID(v_param->cfg->id);
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

				Q_protocolIEC104* protocol;
				
				/*Э��*/	
				if(strcmp(v_param->cfg->classname,"Master")==0)
				{
					// protocol=new Q_protocolIEC104Master();
				}
				else
				{
					protocol=new Q_protocolIEC104Slave();
				
				}

				
				protocol->setThreadPeriod(n_sleepMills);/*�趨�̵߳�ִ��ʱ���Ա�ּ��㷢�Ͳ���֡��ʱ��*/
				protocol->setRtdbInf(v_param->m_pMgr); /*�趨Q_prtocolBase�е�rtdbȫ�ֱ���*/
				
				iecSetting->setChannelID(v_param->cfg->id);/*����ͨ��id*/
				iecSetting->setChannelType(v_param->cfg->type);

			//	Q_tcpClientLink* linkTcp = new Q_tcpClientLink();
				Q_channelBase* channelTest = new Q_channelBase();

				if(strcmp(v_param->cfg->link,"TCP")==0)
				{
					Q_tcpClientLink* linkTcp = new Q_tcpClientLink();
					Q_tcpClientLinkSetting* tLinkSetting = new Q_tcpClientLinkSetting();

					char *tmp=NULL;
					tmp=iecSetting->getContentID(4,1);
					if(tmp!=NULL)
					{
						tLinkSetting->setServerIp(tmp);
					}
					tmp=iecSetting->getContentID(5,1);
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
					tmp=iecSetting->getContentID(11,1);
					if(tmp!=NULL)
					{
						printf("--Serial port name :%s\n", tmp);
						tLinkSetting->setComPortName(tmp);
					}
					tmp=iecSetting->getContentID(12,1);
					if(tmp!=NULL)
					{
						tLinkSetting->setBountRate(atoi(tmp));
					}	
					tmp=iecSetting->getContentID(13,1);
					if(tmp!=NULL)
					{
						tLinkSetting->setDataBit(atoi(tmp));
					}	
					tmp=iecSetting->getContentID(14,1);
					if(tmp!=NULL)
					{
						tLinkSetting->setStopBit(atoi(tmp));
					}	
					tmp=iecSetting->getContentID(15,1);
					if(tmp!=NULL)
					{
						tLinkSetting->setVerifyBit(atoi(tmp));
					}
					tLinkConfig->setLinkSetting(tLinkSetting);
					channelTest->setLinkSetting(tLinkSetting);
					channelTest->setLink(linkCom);
				}

				channelTest->setLinkConfig(tLinkConfig);
			
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
			//	channelTest->setLinkSetting(tLinkSetting);
				channelTest->setLinkConfig(tLinkConfig);
			//	channelTest->setLink(linkTcp);
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
	printf("=== CommIEC101 Protocol Dynamic lib====\n");
	printf("=== 2018-02-04====\n");
	
	return 1;
}
#ifdef UNIX_ENV
#ifdef __cplusplus
}

#endif
#endif //OS_UNIX

