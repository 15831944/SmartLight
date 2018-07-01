#include "stdafx.h"
#include "Q_protocolIEC104.h"

Q_protocolIEC104::Q_protocolIEC104()
{
	m_st=NULL;
	m_cfgBase=NULL;
	m_cfgSt=NULL;
	m_cfgControl=NULL;
	m_cfgSyncClock=NULL;
	setDefaultValue();
	m_tags=NULL;
	m_listCmd=NULL;
	m_ProSetting=NULL;
	time(&timer_U_Test);//=0;
	timer_U_Testflag=false;

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
bool Q_protocolIEC104::explainData()
{
	/*TRACE("Q_protocolIEC104::explainData() \n");*/
	//TRACE("Q_protocolIEC104::explainData() m_pProcotolConfig = %08x \n",m_pProcotolConfig);	

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
	bool bRet=true;

	char msgBuf[Q_FRAME_CONST2048];
	Q_commonUtility::NU_ZERO(msgBuf,sizeof(msgBuf));

	int res=0;
	res=recvTool->readData(msgBuf,sizeof(msgBuf),false);

	if(res<0)
	{
		//新规约打印
		sprintf_s(LastError,sizeof(LastError),"Read data exception,buffer size too small,Please add receive buffer size.close link\n");
	//	SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
		//tjs 2016-11-25
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
		recvTool->clearBuffer();//清空内容
		//closeLink();
		return false;
	}

	//新规约打印
	SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeRx,(BYTE*)&msgBuf,res);
	SYSTEMTIME st;
	GetLocalTime(st);
	//TRACE("handleAnalyseData ,res = %d \n",st.wHour,st.wMinute,st.wSecond,res);

	int j   = 0;
	int len = 0;//14、不包括长度字节及0x68所以要加2
	while (j < res)
	{
		if (msgBuf[j] == 0x68)
		{
			//帧校验
			len = msgBuf[j+1]&0xFF; //修正原来不加&0xFF发现超过127后得不到正确的数字
			if (len > res-j)
			{
				break;//数据内容不足，不处理
			}			
			else if(len>0)
			{
				Q_IEC104vfyRet ret=verifyFrame((BYTE*)&msgBuf[j],len+2);

				if (j+len+2 <= res) //读到的数能够超过一个帧大小
				{
					// call callback
					m_bSendTestFR=false;

					TRACE("eFrameType = %d\n",ret.m_eFrameType);
					
					if (eFrameType_I==ret.m_eFrameType)
					{
						Q_IEC104_APCI *p = (Q_IEC104_APCI *) &msgBuf[j];

						 m_st->nLastAckRx= (p->field2<<8) | (p->field1);
						 m_st->nLastAckTx = (p->field4<<8) | (p->field3);

						 m_st->nRxCounter=m_st->nLastAckRx+2;
						 if (m_st->nRxCounter> m_cfgBase->m_nFrameI_MaxCounter) //超过最大数时，把接收设置为0
						 {
							 m_st->nRxCounter=0;
						 }

						 //新规约打印
						 //SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeRx,(BYTE*)&msgBuf[j+sizeof(Q_IEC104_APCI)],len+2-sizeof(Q_IEC104_APCI));

                         m_st->nCurW++;
						 int aaa = sizeof(Q_IEC104_APCI);
						 decodeMsg_I((BYTE*)&msgBuf[j+sizeof(Q_IEC104_APCI)], len+2-sizeof(Q_IEC104_APCI));

					}
					else if (eFrameType_U==ret.m_eFrameType)
					{
						SYSTEMTIME st;
						GetLocalTime(st);
						TRACE("%d:%d:%d decodeMsg_U\n",st.wHour,st.wMinute,st.wSecond);
						//新规约打印
						//SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeRx,(BYTE*)&msgBuf[j],len+2);

						decodeMsg_U((BYTE*)&msgBuf[j], len+2);
					}
					else if (eFrameType_S==ret.m_eFrameType)
					{
						//continue;//FORMAT_S报文在应用层上不需要处理
						Q_IEC104_APCI *p = (Q_IEC104_APCI *) &msgBuf[j];
						m_st->nLastAckTx=(p->field4<<8) | (p->field3);
					}
					else
					{
						//错误或异常发生
						//I格式出错，主动关闭链接
						bRet=false;
						//如果帧出现数据大小错误、发送序号、接收序号等错误关闭链路

						m_errorNo=e104ErrNo_FrameVerify;
						OnError(m_errorNo,__LINE__);
						closeLink();
						break;
					}
				}
				else if(len==0)
				{
					TRACE("-------find packet len is zero\n-----");
				}
				recvTool->removeData(len+2);//删除数据内容
				j = j + len + 2;

			}
			else if (len==0)
			{
				j++;
			}
		}
		else
		{
			j++;
		}
	}
	return bRet;
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

Q_IEC104vfyRet Q_protocolIEC104::verifyFrame(BYTE* msgbuf,UINT len)
{
	Q_IEC104vfyRet ret;
	ret.m_bSuc=true;

	WORD usTemp_ServSendNum;
	WORD usTemp_ServRecvNum;

	BYTE ucTemp_Length;

	if ( len> (UINT)m_cfgBase->m_nMaxFrameLen || len<MIN_FRAME_NUM)
	{
		ret.m_bSuc=false;
		ret.m_nErrorCode=e104ErrNo_FrameNum;
		return ret;
	}

	ucTemp_Length = len & 0xff;

	Q_IEC104_APCI *p = (Q_IEC104_APCI*) &msgbuf[0];
	if (!(p->field1 & 0x0001))  //FORMAT_I  I格式的帧
	{

		ret.m_eFrameType=eFrameType_I;

		if (ucTemp_Length < 12) //ASDU I 帧应该达到一个起始长度
		{
			ret.m_bSuc=false;
			ret.m_nErrorCode=e104ErrNo_FrameNum;
			return ret;
		}

		usTemp_ServSendNum = (p->field2<<8) | (p->field1>>1);//对端 发送序列号 相当于本端确认接收序列号 LastAckRx
		usTemp_ServRecvNum= (p->field4<<8) | (p->field3>>1);//对端 接收序列号 相当于本端确认发送的序列号 LastAckTx

		Q_IEC104_ASDU_header *header=(Q_IEC104_ASDU_header*)(msgbuf+6);
		int asdu =0;

		asdu=header->commom_asdu_2 << 8;
		asdu |= header->commom_asdu_1;


		if (asdu!=0xFF && asdu!=0xFFFF)
		{
			if (m_cfgBase->m_nAsduAddr!=asdu)
			{
				ret.m_bSuc=false;
				ret.m_nErrorCode=e104ErrNo_MisAsdu;
				return ret;
			}
		}

		//vfy asdu end
		ret.m_bSuc=true;
		return  ret;
	}
	else if (!(p->field1 & 0x0002))  //FORMAT_S  S格式的帧
	{
		ret.m_eFrameType=eFrameType_S;
		if ( ucTemp_Length != 6 )
		{
			ret.m_bSuc=false;
			ret.m_nErrorCode=e104ErrNo_FrameNum;
			return ret;

		}

		usTemp_ServRecvNum = (p->field4<<8) | p->field3;//对端 接收序列号 相当于本端确认发送的序列号

		if (usTemp_ServRecvNum > m_st->nTxCounter && m_st->nTxCounter != 0)
		{
			ret.m_bSuc=false;
			ret.m_nErrorCode=e104ErrNo_RxOverTx;//发送序列号应该小于接收序列号
			return ret;
		}

		return ret;
	}
	else  //U格式的帧
	{
		ret.m_eFrameType=eFrameType_U;
		if ( ucTemp_Length != 6 )
		{
			ret.m_bSuc=false;
			ret.m_nErrorCode=e104ErrNo_FrameNum;
			return ret;
		}
		return ret;
	}

	return ret;
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
	m_eProtocolStatus=eCommonStatusError;
	/*在创建一个TCP连接后，发送和接收序列号都被设置成0*/
	m_st->setDefault();
	setDefaultValue();

}

bool Q_protocolIEC104::getSockConnectStatus()
{
	return m_eProtocolStatus==eCommonStatusNormal?true:false;
}

void Q_protocolIEC104::setDefaultValue(void)
{
	time(&m_nTimerConnect);//m_nTimerConnect=0;
	time(&m_nTimerConfirm);//=0;
	time(&m_nTimer_S_Ack);//=0;
	m_bTimer_S_Ack=0;
	time(&m_nTimer_U_Test);//=0;

	m_bSendTestFR=false;
	m_bStopSend=false; //默认不能发送数据 //更改为 默认发送数据 2012-05-02
}


bool Q_protocolIEC104::makeFrame()
{
	//TRACE("ms = %d \n",GetTickCount());
	//查询cmdlist中状态不为
	list<Q_IEC104StandardCommand *> v_listCmd;

	int len=m_listCmd->getCommandByStatus(v_listCmd);//查无错误的命令列表
	if (len>0)
	{
		
		////2016-11-17 begin  优先处理遥控遥调
		//bool flg_jump=false;
		//for (IT_CmdListP it=v_listCmd.begin();it!=v_listCmd.end();it++)
		//{
		//	
		//	Q_cmdIEC104 *cur_cmd=(Q_cmdIEC104 *)*it;
		//	if(cur_cmd!=NULL)
		//	{	
		//		 //查询当前未处理的，解状态值也为未处理的命令
		//		switch(cur_cmd->getCommand104Type())
		//		{
		//		    if (cur_cmd->m_eState!=eCmdStateNull)
		//			{
		//			//处理内容
		//		case eCmdTypeC_SC:
		//			handleCommmd_SC(cur_cmd);
		//			flg_jump=true;
		//			memset(LastError,0,sizeof(LastError));
		//			sprintf_s(LastError,sizeof(LastError),"优先处理单点遥控帧,命令列表跳出循环\n");
		//			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)&LastError,strlen(LastError)+1);
		//			break;
		//		case eCmdTypeC_DC:
		//			handleCommmd_SC(cur_cmd);
		//			flg_jump=true;
		//			memset(LastError,0,sizeof(LastError));
		//			sprintf_s(LastError,sizeof(LastError),"优先处理双点遥控遥调帧,命令列表跳出循环\n");
		//			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)&LastError,strlen(LastError)+1);
		//			break;
		//		case eCmdTypeC_SE:
		//			handleCommmd_SE(cur_cmd);
		//			flg_jump=true;
		//			memset(LastError,0,sizeof(LastError));
		//			sprintf_s(LastError,sizeof(LastError),"优先处理设点命令帧,命令列表跳出循环\n");
		//			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)&LastError,strlen(LastError)+1);
		//			break;	
		//		default:
		//				break;

		//			}
		//		}
		//	}
		//	if(flg_jump)
		//	{
		//		break;
		//	}
		//}
		//if(flg_jump)
		//{
		//	
		//	return true;
		//}
		//2016-11-17 优先处理遥控遥调 end
		for (IT_CmdListP it=v_listCmd.begin();it!=v_listCmd.end();it++)
		{
			/*i++;
			sprintf_s(LastError,"cmd no is %d",i);
			writefile(LastError);*/
			Q_cmdIEC104 *cur_cmd=(Q_cmdIEC104 *)*it;
		
			if(cur_cmd!=NULL)
			{	
				/*sprintf_s(LastError,"cmd type is %d",cur_cmd->getCommand104Type());
				writefile(LastError);*/

				if(cur_cmd->getCommand104Type()==eCmdTypeTestFR)
				{
					time(&now);
					if((now-timer_U_Test)>=m_cfgBase->m_nT3)//*(m_nThreadPeriod/500))
					{
						sendTestFR();
						//sendStartDT();
						time(&timer_U_Test);//=0;
					}
//					else
//					{
//						//sendTestFR();
//						timer_U_Test++;
//					}
				    continue;
				}

				 //查询当前未处理的，解状态值也为未处理的命令
				switch(cur_cmd->getCommand104Type())
				{
				    if (cur_cmd->m_eState!=eCmdStateNull)
					{
							//处理内容
					case eCmdTypeC_IC:
						handleCommmd_IC(cur_cmd);
						break;
					case eCmdTypeC_SC:
						handleCommmd_SC(cur_cmd);
						break;
					case eCmdTypeC_DC:
						handleCommmd_SC(cur_cmd);
						break;
					case eCmdTypeC_SE:
						handleCommmd_SE(cur_cmd);
						break;
					case eCmdTypeC_CS:
						handleCommmd_CS(cur_cmd);
						break;
					case eCmdTypeC_RC:
						handleCommmd_RC(cur_cmd);
						break;
					case eCmdTypeAnalogchange:
						{
							bool bsendacbc = m_ProSetting->getIsEnableACSend();
							if(bsendacbc)
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

int Q_protocolIEC104::mirror_I_Msg(BYTE *msgbuf,const UINT &len)
{
	Q_IEC104_ASDU_header *header=(Q_IEC104_ASDU_header*)msgbuf;

	Q_IEC104_ASDU msg;
	msg.header.type = 0x64;					// interrogation command   总命令
	msg.header.qual = header->qual;					// number of elements
	msg.header.tx_cause_1 = header->tx_cause_1;			// 0x07 act confirm  0x0A 停止发送
	msg.header.tx_cause_2 = header->tx_cause_2;
	msg.header.commom_asdu_1 = header->commom_asdu_1 & 0xFF;
	msg.header.commom_asdu_2 = (header->commom_asdu_2) & 0xFF;
	// group information
	memcpy(&(msg.data[0]),&msgbuf[sizeof(Q_IEC104_ASDU_header)],sizeof(BYTE)*(len-sizeof(Q_IEC104_ASDU_header)));
	return send_I_Msg((BYTE*)&msg, len);
}

//
//	TX I-Format Message (always contains ASDU)
//

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
			(*header).len=0x04 + len;

			(*header).field1 = m_st->nTxCounter & 0xFE;
			(*header).field2 = (m_st->nTxCounter>>8) & 0xFF;

			(*header).field3 = m_st->nRxCounter & 0xFE;// I-Format
			(*header).field4 = (m_st->nRxCounter>>8) & 0xFF;


			memcpy(&buf[sizeof(Q_IEC104_APCI)],msgbuf,len);


			bool ret=pSendTool->writeData((char*)&buf,len +sizeof(Q_IEC104_APCI));

			if (ret!=true)
			{
				m_errorNo=e104ErrNo_WriteSend;
				m_eProtocolStatus=eCommonStatusError;
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
			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeTx,(BYTE*)&buf,len +sizeof(Q_IEC104_APCI));


			this->m_bSendTestFR = false;		// reset TestFR time-out
			this->m_bTimer_S_Ack  = false;		// reset ACK time-out.
			//计时清零
			time(&this->m_nTimerConfirm);//=0;
			time(&this->m_nTimer_U_Test);//=0;
			time(&this->m_nTimer_S_Ack);//=0;
		
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
//	TX U-Format Message (APCI only)
//
int Q_protocolIEC104::send_U_Msg(int cmd)
{
	printf("Q_protocolIEC104::send_U_Msg\n");
	if (getSockConnectStatus())
	{
		printf("Q_protocolIEC104:: getSockConnectStatus()=true m_pProcotolConfig=%08x\n",m_pProcotolConfig);
		Q_IEC104_APCI header;
		header.start = 0x68;
		header.len=0x04;
		header.field1 = 0x03 | cmd;			// U-Format
		header.field2 = 0;
		header.field3 = 0;
		header.field4 = 0;

		Q_commonBuffer *pSendTool=NULL;
		if (m_pProcotolConfig!=NULL)
			pSendTool=m_pProcotolConfig->getSendBuffer();

		printf("Q_protocolIEC104:: getSockConnectStatus()=true pSendTool=%08x\n",pSendTool);

		if (pSendTool!=NULL)
		{
			printf("Q_protocolIEC104:: pSendTool!=NULL 111\n",pSendTool);
			bool ret=pSendTool->writeData((char*)&header,sizeof(Q_IEC104_APCI));
			printf("Q_protocolIEC104:: pSendTool!=NULL 222\n",pSendTool);

			if (ret!=true)
			{
				printf("Q_protocolIEC104:: pSendTool!=NULL ret!=true 333\n",pSendTool);
				m_errorNo=e104ErrNo_WriteSend;
				m_eProtocolStatus=eCommonStatusError;
#ifdef _DEBUG
				sprintf_s(LastError,sizeof(LastError),"发送U帧数据失败,当前行:%d\n",__LINE__);

				printf(LastError);

				/*//原CommApp规约打印保存、先保留着
				//OnErrorMsg(LastError,sizeof(LastError));
				*/
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
				TRACE("----%s",LastError);

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

				printf("Q_protocolIEC104:: pSendTool!=NULL m_ProSetting=%08x ChannelID = %d 44444\n",m_ProSetting,m_ProSetting->getChannelID());

				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeTx,(BYTE*)&header,sizeof(Q_IEC104_APCI));


				printf("Q_protocolIEC104:: pSendTool!=NULL 444\n",pSendTool);
				this->m_bSendTestFR = false;		// reset TestFR time-out
				time(&this->m_nTimerConfirm);//=0;
				printf("Q_protocolIEC104:: pSendTool!=NULL 555\n",pSendTool);
			}
		}
		return Q_104SUC;
	}
	else
	{
		m_errorNo=e104ErrNo_Link;
		printf("Q_protocolIEC104::send_U_Msg m_errorNo=e104ErrNo_Link\n");
#ifdef _DEBUG
		sprintf_s(LastError,sizeof(LastError),"网络中断,当前行:%d\n",__LINE__);
		printf(LastError);
		//OnErrorMsg(m_errorNo, LastError);

		//新规约打印
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)&LastError,strlen(LastError)+1);
#endif
		return Q_104FAIL;
	}
}

//
//	TX S-Format Message (APCI only)
//
int Q_protocolIEC104::send_S_Msg(void)
{
	if (getSockConnectStatus())
	{
		Q_IEC104_APCI header;
		header.start  = 0x68;
		header.len    = 0x04;
		header.field1 = 0x01;				// S-Format
		header.field2 = 0;
		int num=m_st->nRxCounter;//原来<<1发现当遇到00 01时出错
		header.field3 = num & 0xFE; //接收序列号低位 且最低位为0
		header.field4 = (num >>8) & 0xFF;//接收序列号高位

		Q_commonBuffer *pSendTool=NULL;
		if (m_pProcotolConfig!=NULL)
			pSendTool=m_pProcotolConfig->getSendBuffer();

		if (pSendTool!=NULL)
		{
			bool ret=pSendTool->writeData((char*)&header,sizeof(Q_IEC104_APCI));

			if (ret!=true)
			{
				m_errorNo=e104ErrNo_WriteSend;
				m_eProtocolStatus=eCommonStatusError;

				sprintf_s(LastError,sizeof(LastError),"发送S帧数据失败,当前行:%d\n",__LINE__);
				//OnErrorMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)&LastError,strlen(LastError)+1);

				return Q_104FAIL;
			}
		}
		

		//新规约打印
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeTx,(BYTE*)&header,sizeof(Q_IEC104_APCI));

		this->m_bSendTestFR = false;		// reset TestFR time-out
		this->m_bTimer_S_Ack  = false;		// reset ACK time-out.

		time(&this->m_nTimerConfirm);//=0;
		return Q_104SUC;
	}
	else
	{
		m_errorNo=e104ErrNo_Link;
#ifdef _DEBUG
		sprintf_s(LastError,sizeof(LastError),"网络中断,当前行:%d\n",__LINE__);
		//OnErrorMsg(m_errorNo, LastError);
		//新规约打印
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
		return Q_104FAIL;
	}

}

//
//	START Command
//
int Q_protocolIEC104::sendStartDT(void)
{
	return send_U_Msg(CMD_STARTDT);
}

//
//	STOP Command
//
int Q_protocolIEC104::sendStopDT(void)
{
	return send_U_Msg(CMD_STOPDT);
}

//
//	TEST Frame Command
//
int Q_protocolIEC104::sendTestFR(void)
{
	m_bSendTestFR=true;
	return send_U_Msg(CMD_TESTFR);

}

void Q_protocolIEC104::writefile(char *log)
{

}

void Q_protocolIEC104::OnError(int code,int lineNum)
{
	char str[200];
	switch(code)
	{
	case e104ErrNo_CmdListNull:
		sprintf_s(str,sizeof(str),"错误码为:%d,消息:得到命令列表失败,行号%d\n",code,lineNum);
		break;
	case e104ErrNo_FrameVerify:
		sprintf_s(str,sizeof(str),"错误码为:%d,消息:帧校验失败,行号%d\n",code,lineNum);
		break;
	case e104ErrNo_BeyondK:
		sprintf_s(str,sizeof(str),"错误码为:%d,停止发送数据，当前发送的未确认的数据个数超过K,行号%d\n",code,lineNum);
		break;

	}

	/*//原CommApp规约打印保存、先保留着
	//OnErrorMsg(str,strlen(str));
	if (m_bErrMsg)
	{
	//	OnErrMsg(str,strlen(str));
	}
	*/
	//新规约打印
	SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)str,strlen(str)+1);
}


void Q_protocolIEC104::SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen)
{
	TRACE("Q_protocolIEC104::SendMsgPrint,g_rtdbInfIEC104=%08x",g_rtdbInfIEC104);

	CFEPFuncRtdb *m_rtdbInf = g_rtdbInfIEC104;

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
		}
	}
}