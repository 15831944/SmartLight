#include "stdafx.h"
#include "Q_protocolIEC101.h"

Q_protocolIEC104::Q_protocolIEC104()
{
	m_st=NULL;
	m_cfgBase=NULL;
	m_cfgSt=NULL;
	m_cfgControl=NULL;
	m_cfgSyncClock=NULL;
	setDefaultValue();
	m_listCmd=NULL;
	m_ProSetting=NULL;
	time(&m_nTimer_U_Test);
    m_tags=NULL;
	
	memset(&m_LinkData, 0, sizeof(SIec101_LINK_LAYER));

}

Q_protocolIEC104::~Q_protocolIEC104()
{
	if (m_st!=NULL)
	{
		delete m_st;
		m_st=NULL;
	}
	if (m_cfgBase!=NULL)
	{
		delete m_cfgBase;
		m_cfgBase=NULL;
	}
	if (m_cfgSt!=NULL)
	{
		delete m_cfgSt;
		m_cfgSt=NULL;
	}
	if (m_cfgControl!=NULL)
	{
		delete m_cfgControl;
		m_cfgControl=NULL;
	}
	if (m_cfgSyncClock!=NULL)
	{
		delete m_cfgSyncClock;
		m_cfgSyncClock=NULL;
	}

	//Q_IEC104CommandManager *m_listCmd; 命令列表不由协议来释放
	//Q_104StandardTags* m_tags; 数据标记用
}
//帧校验和计算 
BYTE Q_protocolIEC104::MakeCheck(BYTE *pBuf, int iNum)
{
	BYTE btCheckSum = 0;
	for (int i = 0; i < iNum; i++)
	{
		btCheckSum += pBuf[i];
	}
	return btCheckSum;
}
bool Q_protocolIEC104::explainData()
{


	bool bRet=true;
	Q_commonBuffer *recvTool=NULL;

	if (m_pProcotolConfig!=NULL)
	{
		recvTool=m_pProcotolConfig->getReceiveBuffer();

		if (recvTool!=NULL)
		{	
			int curLen=recvTool->getDataLength();//当前读缓冲区内容
			//TRACE("Q_protocolIEC104::explainData() recvTool!=NULL curLen = %d \n",curLen);

			while (curLen>MAX_104_PACKET_NUM)
			{
				//返复读内容
				bRet=handleAnalyseData(recvTool);
				if (false==bRet)
					break;
				curLen=recvTool->getDataLength();
			}

			if (curLen>=MIN_FRAME_NUM)
			{
				bRet=handleAnalyseData(recvTool);
			}
		}
		else
		{
			bRet=false;
		}
	}
	else
	{
		bRet= false;
	}
	return bRet;


}


bool Q_protocolIEC104::handleAnalyseData(Q_commonBuffer *recvTool)
{
	bool bRet = true;

	char msgBuf[Q_FRAME_CONST2048];
	Q_commonUtility::NU_ZERO(msgBuf, sizeof(msgBuf));

	m_LinkData.setDeaultCount();
	int res = 0;
	res = recvTool->readData(msgBuf, sizeof(msgBuf), false);

	if (res < 0)
	{
		//新规约打印
		sprintf_s(LastError, sizeof(LastError), "Read data exception,buffer size too small,Please add receive buffer size.close link\n");
		//tjs 2016-11-25
		SendMsgPrint(m_ProSetting->getChannelID(), eMsgTypeMsg, (BYTE*)LastError, strlen(LastError) + 1);
		recvTool->clearBuffer();//清空内容
		//closeLink();
		return false;
	}



	int j = 0;
	int len = 0;//14、不包括长度字节及0x68所以要加2

	while (j < res)
	{
		if (msgBuf[j] == 0x10)
		{
			//短帧

			//	SYSTEMTIME st;
			//	GetLocalTime(st);
			//TRACE("%d:%d:%d decodeMsg_U\n",st.wHour,st.wMinute,st.wSecond);
			//新规约打印
			//SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeRx,(BYTE*)&msgBuf[j],len+2);
			//新规约打印
			SendMsgPrint(m_ProSetting->getChannelID(), eMsgTypeRx, (BYTE*)&msgBuf[j], 6);

			decodeMsg_U((BYTE*)&msgBuf[j], 6);
			time(&m_nTimerConnect);
			j = j + 6;

		}
		else if (msgBuf[j] == 0x68)
		{
			//帧校验
			len = msgBuf[j + 1] & 0xFF; //修正原来不加&0xFF发现超过127后得不到正确的数字
			if (len > res - j)
			{
				break;//数据内容不足，不处理
			}
			else if (len > 0)
			{
				//Q_IEC104vfyRet ret=verifyFrame((BYTE*)&msgBuf[j],len+2);

				if (j + len + 4 <= res) //读到的数能够超过一个帧大小
				{
					Q_IEC104_APCI *p = (Q_IEC104_APCI *)&msgBuf[j];



					//新规约打印
					//SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeRx,(BYTE*)&msgBuf[j+sizeof(Q_IEC104_APCI)],len+2-sizeof(Q_IEC104_APCI));

					m_st->nCurW++;
					//	int aaa = sizeof(Q_IEC101_APCI);
					SendMsgPrint(m_ProSetting->getChannelID(), eMsgTypeRx, (BYTE*)&msgBuf[j], len + 6);

					decodeMsg_I((BYTE*)&msgBuf[j + sizeof(Q_IEC104_APCI)], len + 4 - sizeof(Q_IEC104_APCI));

					if (!m_LinkData.getMasterInited())
					{
						//如果收到了I帧但是没有链路没有建立起来，再发送报文

						//发送上一条报文
						//因为发现这个链路没有建立起来呢，居然发过来了数据报文
						SendLastShortFrame();
					}


					time(&m_nTimerConnect);
				}
				else if (len == 0)
				{
					TRACE("-------find packet len is zero\n-----");
				}
				//recvTool->removeData(len+2);//删除数据内容
				j = j + len + 4;


			}
			else if (len == 0)
			{
				j++;
			}
		}
		else
		{
			j++;
		}
	}
	//清空
	if (res > 0)
	{
		recvTool->removeData(res);//删除数据内容

	}

	return bRet;
}
void Q_protocolIEC104::SendLastShortFrame()
{
	if (IEC104_LINK_PRM_STATE_SENDRST == m_LinkData.getMaserState())
	{
		linkSendLinkReset();
	}
	if (m_LinkData.getMaserState() == IEC104_LINK_PRM_STATE_REQSTATUS ||
		m_LinkData.getMaserState() == IEC104_LINK_PRM_STATE_NULL)
	{
		sendStartDT();//发送启动帧
	}
}
bool Q_protocolIEC104::initializeProtocol()
{
	m_ProSetting = (Q_IEC104Setting *)(m_pProcotolConfig->getProtocolSetting());

	if (NULL==m_cfgBase)
	{
		m_cfgBase=new Q_104BaseCfg;

		if (NULL != m_ProSetting)
		{
			m_ProSetting->getBaseSetting(m_cfgBase);

			if (NULL==m_cfgSt)
				m_cfgSt=new Q_104StCfg;

			m_ProSetting->getStationSetting(m_cfgSt);

			if (NULL==m_cfgControl)
				m_cfgControl=new Q_104CtrlCfg;

			m_ProSetting->getControlSetting(m_cfgControl);


			if (NULL==m_cfgSyncClock)
				m_cfgSyncClock=new Q_104SyncClockCfg;

			m_ProSetting->getSyncClockSetting(m_cfgSyncClock);
		}
	}
	if (NULL==m_st)
	{
		m_st=new Q_IEC104_Struct;
		m_st->nW=m_cfgBase->m_nW;
		m_st->nK=m_cfgBase->m_nK;

		if (m_pProcotolConfig->getProtocolCommandManager()!=NULL)
		{
			m_listCmd = (Q_IEC104CommandManager *)(m_pProcotolConfig->getProtocolCommandManager());
			if (m_listCmd==NULL)
			{
				m_errorNo=e104ErrNo_CmdListNull;
				OnError(m_errorNo,__LINE__);

				return false;
			}
		}
		else
		{
			m_errorNo=e104ErrNo_CmdListNull;
			OnError(m_errorNo,__LINE__);

			return false;
		}

	}

	if (Q_commonUtility::isBigEndian())
	{
		m_cpuEndian=eBigEndian;
	}
	else
	{
		m_cpuEndian=eLittleEndian;
	}
	m_tags=(Q_104StandardTags*)m_ProSetting->getTagsManager();


	return true;
}


void Q_protocolIEC104::onTxMsg(BYTE *msgbuf, unsigned int len)
{
	if(m_ProSetting->getWndMsgID()>0)
	{
		if(m_ProSetting->getPrintSetting(eComLogTypeTotal)==true)
		{
		Q_COM_LOG_INFO log;
		log.chid=m_ProSetting->getChannelID();
		log.type=eMsgTypeTx;
		log.msg=(char*)msgbuf;
		log.len=len;
		/*m_pMgr->SendMessageToRtdb(mes_PROTCOL_PACKET,sizeof(log),(char *)&log); //fsnewrtdb*/
		}
	}
}

void Q_protocolIEC104::onRxMsg(BYTE *msgbuf, unsigned int len)
{
	if(m_ProSetting->getWndMsgID()>0)
	{
		if(m_ProSetting->getPrintSetting(eComLogTypeTotal)==true)
		{
		Q_COM_LOG_INFO log;
		log.chid=m_ProSetting->getChannelID();
		log.type=eMsgTypeRx;
		log.msg=(char *)msgbuf;
		log.len=len;
		/*m_pMgr->SendMessageToRtdb(mes_PROTCOL_PACKET,sizeof(log),(char *)&log); //fsnewrtdb*/
		}
	}
}

void Q_protocolIEC104::OnErrorMsg(char *msg,unsigned int len)
{
	if(m_ProSetting->getWndMsgID()>0)
	{
		if(m_ProSetting->getPrintSetting(eComLogTypeError)==true)
		{
		Q_COM_LOG_INFO log;
		log.chid=m_ProSetting->getChannelID();
		log.type=eMsgTypeError;
		log.msg=msg;
		log.len=len;
		/*m_pMgr->SendMessageToRtdb(mes_PROTCOL_PACKET,sizeof(log),(char *)&log); //fsnewrtdb*/
		/*::SendMessage(m_ProSetting->getWndHwnd(),m_ProSetting->getWndMsgID(),(WPARAM)&log,(LPARAM)sizeof(log));SendMessage一律不用*/
		}
	}
}

void Q_protocolIEC104::OnShowMsg(char *msg,unsigned int len)
{
	if(m_ProSetting->getWndMsgID()>0)
	{
		if(m_ProSetting->getPrintSetting(eComLogTypePrompt))
		{
		Q_COM_LOG_INFO log;
		log.chid=m_ProSetting->getChannelID();
		log.type=eMsgTypeMsg;
		log.msg=msg;
		log.len=len;
		/*m_pMgr->SendMessageToRtdb(mes_PROTCOL_PACKET,sizeof(log),(char *)&log); //fsnewrtdb*/
		}
	}
}

void Q_protocolIEC104::OnShowExplainMsg(char *msg,unsigned int len)
{
	if(m_ProSetting->getWndMsgID()>0)
	{
		if(m_ProSetting->getPrintSetting(eComLogTypeExplain))
		{
		Q_COM_LOG_INFO log;
		log.chid=m_ProSetting->getChannelID();
		log.type=eMsgTypeMsg;
		log.msg=msg;
		log.len=len;
		/*m_pMgr->SendMessageToRtdb(mes_PROTCOL_PACKET,sizeof(log),(char *)&log); //fsnewrtdb*/
		}
	}
}

void Q_protocolIEC104::closeLink()
{
	if (nCloseLinkCount >= 3)
	{
	  m_eProtocolStatus = eCommonStatusError;
	 /*在创建一个TCP连接后，发送和接收序列号都被设置成0*/
	  m_st->setDefault();
	  setDefaultValue();
    }
	else
	{
		nCloseLinkCount++;
		linkSendLinkReset();
		m_LinkData.setMasterState(IEC104_LINK_PRM_STATE_SENDRST);
	}

}

bool Q_protocolIEC104::getSockConnectStatus()
{
	return m_eProtocolStatus==eCommonStatusNormal?true:false;
}

void Q_protocolIEC104::setDefaultValue(void)
{
	time(&m_nTimerConnect);//m_nTimerConnect=0;
	//time(&m_nTimerConfirm);//=0;
//	time(&m_nTimer_S_Ack);//=0;
	m_bTimer_S_Ack=0;
	time(&m_nTimer_U_Test);//=0;

	m_bSendTestFR=false;
	m_bStopSend=false; //默认不能发送数据 //更改为 默认发送数据 2012-05-02
}


bool Q_protocolIEC104::makeFrame()
{

	if (!m_LinkData.getMasterInited())
	{
		return true;
	}

       if (!m_LinkData.getSlaveInited())
       {
        //判断一下如果长时间没有收到子站的回复，主动断开链路

		   time_t now;
		   time(&now);
		   if (now - m_nTimerConnect >= m_cfgBase->m_nT3)
		   {
			   closeLink();

		   }
       }
	   if (!m_LinkData.getSlaveInited())
	   {
		   printf("makeFrame() return getSlaveInited() is false\n");
		   return true;
	   }

	//TRACE("ms = %d \n",GetTickCount());
	//查询cmdlist中状态不为
	list<Q_IEC104StandardCommand *> v_listCmd;

	int len=m_listCmd->getCommandByStatus(v_listCmd);//查无错误的命令列表

	if (len>0)
	{
		for (IT_CmdListP it = v_listCmd.begin(); it != v_listCmd.end(); it++)
		{
			Q_cmdIEC104 *cur_cmd = (Q_cmdIEC104 *)*it;

			if (cur_cmd != NULL)
			{
				//查询当前未处理的，解状态值也为未处理的命令
				switch (cur_cmd->getCommand104Type())
				{
					if (cur_cmd->m_eState != eCmdStateNull)
					{
						//处理内容
				case eCmdTypeC_IC:
					handleCommmd_IC(cur_cmd);
					break;
				
				case eCmdTypeAnalogchange:
					{
						bool bsendacbc = m_ProSetting->getIsEnableACSend();
						if (bsendacbc)
						{
							handleCommmd_AC(cur_cmd);
							handleCommmd_BC(cur_cmd);
							handleCommmd_BC_SOE(cur_cmd);
						}
					}
					break;
				case eCmdTypeC_EI:
					break;
				default:
					handleCommmd_Ex(cur_cmd);
					break;

					}
				}
			}
		}
	}
	return true;
}
int Q_protocolIEC104::send_I_Msg(BYTE *msgbuf, UINT len)
{
	//assert((int)msgbuf!=0xcccccccc);
	if (m_bStopSend)
	{
#ifdef _DEBUG
		m_errorNo=e104ErrNo_BeyondK;
		sprintf_s(LastError,sizeof(LastError),"停止发送数据，当前发送的未确认的数据个数超过K,当前行:%d\n",__LINE__);
		
		/*//原CommApp规约打印保存、先保留着
		//OnErrorMsg(LastError,sizeof(LastError));
		*/

		//新规约打印
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)&LastError,strlen(LastError)+1);

#endif
		return Q_104FAIL;
	}

	if (getSockConnectStatus())
	{

		Q_commonBuffer *pSendTool=NULL;
		if (m_pProcotolConfig!=NULL)
			pSendTool=m_pProcotolConfig->getSendBuffer();

		if (pSendTool!=NULL)
		{
			char buf[Q_FRAME_CONST1024];
						
			Q_IEC104_APCI *header = (Q_IEC104_APCI*)buf;
			(*header).start = 0x68;
			(*header).len=0x03 + len;//0x03 不包括第二个68
			(*header).len2= 0x03 + len;
			(*header).start2 = 0x68;
			//(*header).control = 0x11|64|16|(m_LinkData.FCB_Main*0x40);
			Q_CtrlZone pi;
			
			pi.fun = 0x04;
			pi.dir = 1;
			pi.prm= 1;
			pi.fcv = 0;
			pi.fcb=0;
			/*
			if (m_LinkData.FCB_Main == 1)
			{
				pi.fcb = 1;
			}
			else
			{
				pi.fcb = 0;
			}
			*/
			(*header).control = *((BYTE *)&pi);
			printf("=======####send_I_Msg control %d\n", header->control);
			short addr =(short)m_cfgBase->m_nAsduAddr;

			memcpy(header->addr1,&addr,sizeof(addr));

			//68 0C 0C 68 73 01 00 64 01 06 00 01 00 00 00 14 F4 16

			//(*header).field2 = (m_st->nTxCounter>>8) & 0xFF;

			//(*header).field3 = m_st->nRxCounter & 0xFE;// I-Format
			//(*header).field4 = (m_st->nRxCounter>>8) & 0xFF;


			    memcpy(&buf[sizeof(Q_IEC104_APCI)],msgbuf,len);

		

				buf[sizeof(Q_IEC104_APCI)+len]= MakeCheck( (BYTE *)buf+4,len+3);

				buf[sizeof(Q_IEC104_APCI)+len+1]= 0x16;


			bool ret=pSendTool->writeData((char*)&buf,len +sizeof(Q_IEC104_APCI)+2);

			if (ret!=true)
			{
				m_errorNo = e104ErrNo_WriteSend;
				m_eProtocolStatus = eCommonStatusError;
#ifdef _DEBUG
				sprintf_s(LastError,sizeof(LastError),"发送I帧数据失败,当前行:%d\n",__LINE__);

				/*//原CommApp规约打印保存、先保留着
				//OnErrorMsg(LastError,sizeof(LastError));
				*/
				//新规约打印
				//原来是固定的是strlen(LastError)+1字节，这样容易给带来崩溃,谭井胜tjs 2016-11-25
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)&LastError,strlen(LastError)+1);
#endif
				return Q_104FAIL;
			}

			//新规约打印
			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeTx,(BYTE*)&buf,len +sizeof(Q_IEC104_APCI)+2);

			m_LinkData.FCB_Main = !m_LinkData.FCB_Main;

			this->m_bSendTestFR = false;		// reset TestFR time-out
			this->m_bTimer_S_Ack  = false;		// reset ACK time-out.
			//计时清零
			//time(&this->m_nTimerConfirm);//=0;
			
		//	time(&this->m_nTimer_U_Test);//=0;
			
		//	time(&this->m_nTimer_S_Ack);//=0;
		
			m_st->nTxCounter+=2;	
			if (m_st->nTxCounter>m_cfgBase->m_nFrameI_MaxCounter)
			{
				m_st->nTxCounter=0;
			}
		
			return Q_104SUC;
		}
		else
		{
			m_errorNo=e104ErrNo_Link;
#ifdef _DEBUG
			sprintf_s(LastError,sizeof(LastError),"网络中断,当前行:%d\n",__LINE__);

			/*//原CommApp规约打印保存、先保留着
			//OnErrorMsg(LastError,sizeof(LastError));
			*/
			//新规约打印
			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
			return Q_104FAIL;
		}
	}
	return Q_104SUC;
}
int Q_protocolIEC104::send_I_MsgInit(BYTE *msgbuf, UINT len)
{
	//assert((int)msgbuf!=0xcccccccc);
	if (m_bStopSend)
	{
#ifdef _DEBUG
		m_errorNo=e104ErrNo_BeyondK;
		sprintf_s(LastError,sizeof(LastError),"停止发送数据，当前发送的未确认的数据个数超过K,当前行:%d\n",__LINE__);
		
		/*//原CommApp规约打印保存、先保留着
		//OnErrorMsg(LastError,sizeof(LastError));
		*/

		//新规约打印
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)&LastError,strlen(LastError)+1);

#endif
		return Q_104FAIL;
	}

	if (getSockConnectStatus())
	{

		Q_commonBuffer *pSendTool=NULL;
		if (m_pProcotolConfig!=NULL)
			pSendTool=m_pProcotolConfig->getSendBuffer();

		if (pSendTool!=NULL)
		{
			char buf[Q_FRAME_CONST1024];
						
			Q_IEC104_APCI *header = (Q_IEC104_APCI*)buf;
			(*header).start = 0x68;
			(*header).len=0x03 + len;//0x03 不包括第二个68
			(*header).len2= 0x03 + len;
			(*header).start2 = 0x68;
			//(*header).control = 0x11|64|16|(m_LinkData.FCB_Main*0x40);
			Q_CtrlZone pi;
			
			pi.fun = 0x04;
			pi.dir = 1;
			pi.prm= 1;
			if (m_LinkData.FCB_Main == 1)
			{
				pi.fcb = 1;
			}
			else
			{
				pi.fcb = 0;
			}
			pi.fcv = 0;
			(*header).control = *((BYTE *)&pi);
			printf("=======####send_I_Msg control %d\n", header->control);
			short addr =(short)m_cfgBase->m_nAsduAddr;

			memcpy(header->addr1,&addr,sizeof(addr));

			//68 0C 0C 68 73 01 00 64 01 06 00 01 00 00 00 14 F4 16

			//(*header).field2 = (m_st->nTxCounter>>8) & 0xFF;

			//(*header).field3 = m_st->nRxCounter & 0xFE;// I-Format
			//(*header).field4 = (m_st->nRxCounter>>8) & 0xFF;


			    memcpy(&buf[sizeof(Q_IEC104_APCI)],msgbuf,len);

		

				buf[sizeof(Q_IEC104_APCI)+len]= MakeCheck( (BYTE *)buf+4,len+3);

				buf[sizeof(Q_IEC104_APCI)+len+1]= 0x16;


			bool ret=pSendTool->writeData((char*)&buf,len +sizeof(Q_IEC104_APCI)+2);

			if (ret!=true)
			{
				m_errorNo = e104ErrNo_WriteSend;
				m_eProtocolStatus = eCommonStatusError;
#ifdef _DEBUG
				sprintf_s(LastError,sizeof(LastError),"发送I帧数据失败,当前行:%d\n",__LINE__);

				/*//原CommApp规约打印保存、先保留着
				//OnErrorMsg(LastError,sizeof(LastError));
				*/
				//新规约打印
				//原来是固定的是strlen(LastError)+1字节，这样容易给带来崩溃,谭井胜tjs 2016-11-25
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)&LastError,strlen(LastError)+1);
#endif
				return Q_104FAIL;
			}

			//新规约打印
			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeTx,(BYTE*)&buf,len +sizeof(Q_IEC104_APCI)+2);

			m_LinkData.FCB_Main = !m_LinkData.FCB_Main;

			this->m_bSendTestFR = false;		// reset TestFR time-out
			this->m_bTimer_S_Ack  = false;		// reset ACK time-out.
			//计时清零
			//time(&this->m_nTimerConfirm);//=0;
			
		//	time(&this->m_nTimer_U_Test);//=0;
			
		//	time(&this->m_nTimer_S_Ack);//=0;
		
			m_st->nTxCounter+=2;	
			if (m_st->nTxCounter>m_cfgBase->m_nFrameI_MaxCounter)
			{
				m_st->nTxCounter=0;
			}
		
			return Q_104SUC;
		}
		else
		{
			m_errorNo=e104ErrNo_Link;
#ifdef _DEBUG
			sprintf_s(LastError,sizeof(LastError),"网络中断,当前行:%d\n",__LINE__);

			/*//原CommApp规约打印保存、先保留着
			//OnErrorMsg(LastError,sizeof(LastError));
			*/
			//新规约打印
			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
			return Q_104FAIL;
		}
	}
	return Q_104SUC;
}

//
//	TX S-Format Message (APCI only)
//
int Q_protocolIEC104::send_S_Msg(void)
{
	linkAckSubReset();
	return 0;
}





void Q_protocolIEC104::SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen)
{
	//TRACE("Q_protocolIEC104::SendMsgPrint,g_rtdbInfIEC101=%08x",g_pMgr->getRtdbInf());
	/*
	CFEPFuncRtdb *m_rtdbInf =g_pMgr->getRtdbInf();

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

	if (bSend == true)
	{
		//只给本机发送报文,同时FindProcessByProType由判断进程状态是否正确的情况呀
		SYSTEM_Process * pProcess = m_rtdbInf->getMainDb()->FindProcessByProType(pt_DBGSVR, m_rtdbInf->getMainDb()->GetThisNodeID());
		if(pProcess==NULL)
		{
			TRACE("Q_protocolIEC104::send message process id did not find\n");

			return ;
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
				//TRACE("Q_protocolIEC104::send message process id=%d",pProcess->iProcessID);
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
			//TRACE("Q_protocolIEC104::send messae process id=%d",pProcess->iProcessID);
			m_rtdbInf->getMainDb()->SendMessageTo(pProcess->ID32,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
			/*char strMsg[200] = {0};
			sprintf_s(strMsg,sizeof(strMsg),"MsgPrint:stid = %d msgtype id = %d message = %s",MsgPrint.ichannelid,MsgPrint.msgtype,MsgPrint.msg);*/
			//printf("%s\n",strMsg);
		//}

//	}
	
}

//
//	START Command
//
int Q_protocolIEC104::sendStartDT(void)
{
	time_t now;
	time(&now);
	m_LinkData.FCB_Main = 1;
	if (now - m_LinkData.LastTxTime>1)
	{
		m_LinkData.LastTxTime = now;
		//看看上一次发送的时间，如果超过时间再发
		//if (m_cfgBase->m_nAsduLen == 2)
		//{
			BYTE pBuf[6] = { 0 };
			pBuf[0] = 0x10;
			//pBuf[1] = pFuncCode | CTRLBYTE_DIR | (CTRLBYTE_FCB_OR_ACD *ACD_Flag);
			pBuf[1] = 0xC9 ;


			short nTmp = (short)m_cfgBase->m_nAsduAddr;
			memcpy(pBuf + 2, &nTmp, sizeof(short));
			pBuf[4] = MakeCheck(&pBuf[1], 3);
			pBuf[5] = 0x16;
			sendData((char*)pBuf, 6);

			//请求远方链路状态
			m_LinkData.setMasterState(IEC104_LINK_PRM_STATE_REQSTATUS);
			//m_LinkData.FCB_Main=!m_LinkData.FCB_Main;
		//}

	}


	return 0;
}

int Q_protocolIEC104::sendData(char *buff, int nLen)
{
	if (getSockConnectStatus())
	{

		Q_commonBuffer *pSendTool = NULL;
		if (m_pProcotolConfig != NULL)
			pSendTool = m_pProcotolConfig->getSendBuffer();

	

		if (pSendTool != NULL)
		{
			
			bool ret = pSendTool->writeData((char*)buff, nLen);


			if (ret != true)
			{
				//printf("Q_protocolIEC104:: pSendTool!=NULL ret!=true 333\n", pSendTool);
				m_errorNo = e104ErrNo_WriteSend;
				m_eProtocolStatus = eCommonStatusError;
#ifdef _DEBUG
				sprintf_s(LastError, sizeof(LastError), "发送U帧数据失败,当前行:%d\n", __LINE__);

				printf(LastError);

				/*//原CommApp规约打印保存、先保留着
				//OnErrorMsg(LastError,sizeof(LastError));
				*/
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(), eMsgTypeError, (BYTE*)LastError, strlen(LastError) + 1);
				TRACE("----%s", LastError);

#endif
				return Q_104FAIL;
			}
			else
			{
				/*//原CommApp规约打印保存、先保留着
				//this->onTxMsg((BYTE*)&header,sizeof(header));
				if (m_bSendMsg)
				{
				//	OnSendMsg((char*)&header,sizeof(header));
				}
				*/

				//printf("Q_protocolIEC104:: pSendTool!=NULL m_ProSetting=%08x ChannelID = %d 44444\n", m_ProSetting, m_ProSetting->getChannelID());

				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(), eMsgTypeTx, (BYTE*)buff, nLen);


			//	printf("Q_protocolIEC104:: pSendTool!=NULL 444\n", pSendTool);
				this->m_bSendTestFR = false;		// reset TestFR time-out
				//time(&this->m_nTimerConfirm);//=0;
				//printf("Q_protocolIEC104:: pSendTool!=NULL 555\n", pSendTool);
			}
		}
		return Q_104SUC;
	}
	else
	{
		m_errorNo = e104ErrNo_Link;
	//	printf("Q_protocolIEC104::send_U_Msg m_errorNo=e104ErrNo_Link\n");
#ifdef _DEBUG
		sprintf_s(LastError, sizeof(LastError), "网络中断,当前行:%d\n", __LINE__);
		//printf(LastError);
		//OnErrorMsg(m_errorNo, LastError);

		//新规约打印
		SendMsgPrint(m_ProSetting->getChannelID(), eMsgTypeError, (BYTE*)&LastError, strlen(LastError) + 1);
#endif
		return Q_104FAIL;
	}
}

int Q_protocolIEC104::linkSendLinkReset()
{
	//SIec101Data *p101;
	///p101 = m_pIec101Data +iLoopCnt;
	createFixedFrame(FUNC_SUB_CONF, m_cfgBase->m_nAsduAddr,1);
	//	if (m_LinkData.Sub_Inited_Flag == 0)
	//	m_LinkData.Sub_Inited_Flag = 1;
	m_LinkData.FCB_Main = !m_LinkData.FCB_Main;
	return 1;
}
void Q_protocolIEC104::OnError(int errorCode,int lineNum)
{

}
//回复子站链路请求
int Q_protocolIEC104::linkAckSubLinkStatus()
{
	if (m_LinkData.getAckLinkStatusTimes() == 0)
	{
		//保证同样的请求只回复一帧
		createFixedFrame(FUNC_SUB_LINKOK, m_cfgBase->m_nAsduAddr,0);
	}
	m_LinkData.addAckLinkStatusTimes();
	
	return 1;
}
//回复子站请求
int Q_protocolIEC104::linkAckSubReset()
{
	if (m_LinkData.getAckResetLinkTimes() == 0)
	{
		createFixedFrame(FUNC_SUB_CONF, m_cfgBase->m_nAsduAddr,0);
	}
	m_LinkData.addAckResetLinkTimes();

	return 1;
}

void Q_protocolIEC104::createFixedFrame(BYTE pFuncCode, int rtuAddr, int fcb)
{
	if (m_cfgBase->m_nAsduLen == 2)
	{
		BYTE pBuf[6] = { 0 };
		pBuf[0] = 0x10;
		//pBuf[1] = pFuncCode | CTRLBYTE_DIR | (CTRLBYTE_FCB_OR_ACD *ACD_Flag);


		pBuf[1] = pFuncCode | fcb * 0x40|0x80;//fcv无效
		short nTmp = (short)rtuAddr;
		memcpy(pBuf + 2, &nTmp, sizeof(short));
		pBuf[4] = MakeCheck(&pBuf[1], 3);
		pBuf[5] = 0x16;
		sendData((char*)pBuf, 6);
	}
}

