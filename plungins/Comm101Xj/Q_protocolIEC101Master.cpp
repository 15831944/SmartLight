#include "stdafx.h"
#include "Q_ProtocolIEC101Master.h"
#include "tinyxmlinclude.h"


Q_protocolIEC104Master::Q_protocolIEC104Master(void)
{
	bStartFlg=false;
	memset(LastError,0,sizeof(LastError));
}


Q_protocolIEC104Master::~Q_protocolIEC104Master(void)
{
}


bool Q_protocolIEC104Master::startProcess()
{
	printf("Q_protocolIEC104Master::startProcess() m_st=%08x m_pMgr=%08x m_ProSetting=%08x\n",m_st,m_pMgr,m_ProSetting);

	m_st->setDefault();

	printf("Q_protocolIEC104Master::startProcess() 11\n");

	if (m_pMgr != NULL)
	{
		g_rtdbInfIEC104 = m_pMgr->getRtdbInf();
	}

//	memset(&m_LinkData,0,sizeof(SIec101_LINK_LAYER));

//	m_LinkData.Sub_State=IEC101_LINK_PRM_STATE_UNRESET;
	//m_LinkData.Master_State=IEC101_LINK_PRM_STATE_UNRESET;
	m_LinkData.ACD_Sub=0;
	m_LinkData.FCB_Main=0;
	m_LinkData.Sub_Inited_Flag=false;
	m_LinkData.nAckSubCount=0;
	printf("Q_protocolIEC104Master::startProcess() 22\n");

	if(m_ProSetting->getPrintSetting(eComLogTypePrompt))
	{
#ifdef _DEBUG
		sprintf_s(LastError,sizeof(LastError),"初使化开始...");
		//OnShowMsg(LastError,sizeof(LastError));
		//新规约打印
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
	}

	printf("Q_protocolIEC104Master::startProcess() 33\n");

	if(m_bNomalInfoMsg)
	{
		printf("Q_protocolIEC104Master::startProcess() 44\n");
#ifdef _DEBUG
		sprintf_s(LastError,sizeof(LastError),"初使化开始...");
		//OnNomalInfoMsg(LastError,sizeof(LastError));
		//新规约打印
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
#endif
	}

	printf("Q_protocolIEC104Master::startProcess() 55\n");

	m_eProtocolStatus = eCommonStatusNormal;
	if(sendStartDT()==Q_104SUC)
	{
		/*printf("Q_protocolIEC104Master::startProcess() 66\n");
		Q_IEC104StandardCommand *pCommand=new Q_IEC104StandardCommand();
		pCommand->setCommonType(eCommandTypeCycle);
		pCommand->setCommand104Type(eCmdTypeTestFR);
		m_listCmd->addCommand(pCommand);*/

		printf("Q_protocolIEC104Master::startProcess() 77\n");

		if(m_cfgBase->m_bSendIC==true)
		{
			//总召唤
			Q_cmdIEC104 *pCommandIC=new Q_cmdIEC104();
			pCommandIC->setCommonType(eCommandTypeCycle);
			pCommandIC->setCommand104Type(eCmdTypeC_IC);
			pCommandIC->m_eState=eCmdStateIntRequest;
			pCommandIC->setMaxTestNum(50);//tjs 2013-4-7
			pCommandIC->m_nComAsdu=m_cfgBase->m_nAsduAddr;
			pCommandIC->m_bExec=false;
			m_listCmd->addCommand(pCommandIC);
			printf("Q_protocolIEC104Master::startProcess() 88\n");
		}	
		return true;
	}
	else
		return false;

	printf("Q_protocolIEC104Master::startProcess() eee\n");
	return true;	
}


bool Q_protocolIEC104Master::stopProcess()
{
	m_eProtocolStatus = eCommonStatusNull;
	//m_listCmd->clearCommand(); //2013-02-24 清空buffer
	bStartFlg=false;
	m_LinkData.Sub_Inited_Flag=false;
	
	if(m_ProSetting->getPrintSetting(eComLogTypePrompt))
	{
		sprintf_s(LastError,sizeof(LastError),"断开链接...");
		//OnShowMsg(LastError,sizeof(LastError));
		//新规约打印
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);

	}
	if(m_bNomalInfoMsg)
	{

		sprintf_s(LastError,sizeof(LastError),"断开链接...");
		//OnNomalInfoMsg(LastError,sizeof(LastError));
		//新规约打印
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);

	}

	Q_dataManager* p = _GlobalData.getDataManager();
	LPQ_ANALOGUEDATATAG pYC = p->getAnalogData();
	LPQ_DIGITALEDATATAG pYX = p->getDigitalData();

	if (m_tags!=NULL)
	{
		if (NULL!=pYC && m_tags->getAnalogEnd() < p->getAnalogCount())
		{
			for (int i = m_tags->getAnalogBegin();i<m_tags->getAnalogEnd()+1;i++)
			{
				if (pYC[i].datasource!=2)
				{
					pYC[i].quality = 2;
				}
			}
		}

		if (NULL!=pYX && m_tags->getPointEnd() < p->getDigitalCount())
		{
			for (int i = m_tags->getPointBegin();i<m_tags->getPointEnd()+1;i++)
			{
				if (pYX[i].datasource!=2)
				{
					pYX[i].quality = 2;
				}
			}
		}
	}
	return true;
}



void Q_protocolIEC104Master::CP56Time2aToTime(unsigned char *data, SYSTEMTIME *st)
{
	unsigned int mili = data[0] | data[1]<<8;
	st->wSecond		= mili / 1000;
	st->wMilliseconds = mili - st->wSecond*1000;

	st->wMinute		= data[2] & 0x3F;;
	st->wHour		= data[3] & 0x1F;

	st->wDay		= data[4] & 0x1F;
	st->wDayOfWeek = (data[4] & 0xE0 ) >> 5;
	if(st->wDayOfWeek)	// if zero day of week not used.
		st->wDayOfWeek = (st->wDayOfWeek + 1)%7;
	st->wMonth		= data[5] & 0x0F;
	st->wYear		= 2000 + (data[6] & 0x7F);
}


bool Q_protocolIEC104Master::terminateProtocol()
{
	m_ProSetting=NULL;
	return true;
}

int Q_protocolIEC104Master::decodeMsg_U(BYTE *msgbuf, unsigned int len)
{
	//1 请求链路状态   复位远方链路 召唤1级用户数据 召唤用户2级数据 


	if( msgbuf[5] == 0x16 )	//frame end char
	{

		if( MakeCheck( msgbuf+1, 3 ) != msgbuf[4] )	//checksum error
		{
			//m_pChannel->CalFrameCnt(i,IOSCAN_RX_FRAME_CHECKERR);
			return 0;
		}	

		int btFuncCode = msgbuf[1]&CTRLBYTE_FUNC;

		Q_CtrlZone *pi=(Q_CtrlZone *)(msgbuf+1);
		switch( btFuncCode )
		{
		case FUNC_PRM_REQ_LINKSTAT://09
			{
				//if(!m_LinkData.Sub_Inited_Flag)
				//{
				if (pi->prm == 1)
				{
				  linkAckSubLinkStatus();//回复linkstatus
				  m_LinkData.Sub_State=IEC101_LINK_SUB_STATE_SENDSTATUS;
				}
				//发送一个回复


			}
			break;
		case FUNC_PRM_RSTLINK://0
			{
				    //子站可以主动发送复位远方链路
					if (pi->prm == 0)
				    {
						if(IEC101_LINK_PRM_STATE_SENDRST==m_LinkData.Master_State)
						{
                            //子站回复了复位远方链路的请求
						
							//主站向子站建立完成了
							m_LinkData.Master_State=IEC101_LINK_PRM_STATE_WAITDTCONF;
							m_LinkData.Sub_Inited_Flag=true;
							bStartFlg=true;
											//}
						}
						else if(IEC101_LINK_PRM_STATE_WAITDTCONF==m_LinkData.Master_State)
						{
							//在传输过程中由子站发出来的复位远方，不处理
						    //暂时不处理子站发过来的异常报文
							
						}
					}
					else if( IEC101_LINK_SUB_STATE_SENDSTATUS==m_LinkData.Sub_State)
					{
						linkAckSubReset( );
						m_LinkData.Sub_State=IEC101_LINK_SUB_STATE_SENDRSCONF;//重置链路
					}
				
			}
			break;
		case FUNC_PRM_REQ_CLASS2://11
			{
			
						//	IEC101_LINK_PRM_STATE_UNRESET==m_LinkData.Master_State)
					if(IEC101_LINK_PRM_STATE_REQSTATUS==m_LinkData.Master_State||
						IEC101_LINK_PRM_STATE_UNRESET==m_LinkData.Master_State)
					{
						//已请求完成
						//发送复位远方状态
						linkSendLinkReset();
						m_LinkData.Master_State=IEC101_LINK_PRM_STATE_SENDRST;
					}
					else if(IEC101_LINK_PRM_STATE_WAITRSCONF==m_LinkData.Master_State)
					{
						m_LinkData.Master_State=IEC101_LINK_PRM_STATE_WAITDTCONF;
						//表示主站向子站初使化结束
					}
				
			}
			break;
		default :
			break;
		}

	}



	return 0;
}

int Q_protocolIEC104Master::decodeMsg_I(BYTE *msgbuf, unsigned int len)
{
	//printf("--------Q_protocolIEC104Master::decodeMsg_I-----\n");
	Q_IEC104_ASDU_header *header=NULL;
	Q_104ApduEx apdu_ex;
	apdu_ex.len=len;
	apdu_ex.msgbuf=msgbuf;
	apdu_ex.seq=false;

	header = (Q_IEC104_ASDU_header*)msgbuf;

	apdu_ex.num = header->qual & 0x7F;
	if (header->qual & 0x80)
		apdu_ex.seq = true;

	apdu_ex.cause = header->tx_cause_1;	// 传送原因

	apdu_ex.asdu = header->commom_asdu_2 << 8;
	apdu_ex.asdu |= header->commom_asdu_1;

	apdu_ex.type=header->type;

	//TRACE("=====Q_protocolIEC104Master::decodeMsg_I len = %d  m_ProSetting = %08x=====\n",len,m_ProSetting);

#ifdef _DEBUG
	sprintf_s(LastError,sizeof(LastError), "ASDU Address=%d\tTypeID=%d\tCount=%d\tCause=%d\n",
		apdu_ex.asdu, header->type, apdu_ex.num, apdu_ex.cause);
	//OnErrorMsg(m_errorNo, LastError);
	//新规约打印
	SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
	bool scaled=false;
	int ret=Q_104SUC;

	// type identification
	switch (header->type)
	{
	case 1: //Single-point information
		func_M_SP_NA_1(&apdu_ex);
		break;
	case 3:	//Double -point information
		func_M_DP_NA_1(&apdu_ex);//原来就没有将数据写入只sprnitf
		break;
		//case 5: //step position
		//    func_M_ST_NA_1(&apdu_ex);//原来就没有
		//    break;
		//case 7: //32比特串
		//    func_M_BO_NA_1(&apdu_ex);//原来就没有
		//    break;
	case 9:  //Measured Normalized 正常值
	case 11: //Measured Scaled 测量值、标度化值
		scaled=(header->type==11) ? true:false;
		func_M_ME_NA_NB_1(&apdu_ex,scaled);//fs20150703
		break;
	case 13: //Measure value, short floating point value
		func_M_ME_NC_1(&apdu_ex); 
		break;
	case 15: //累计量，一般都是电能量 int 类型
		func_M_IT_NA_1(&apdu_ex);//原来就没有将数据写入sprnitf
		break;
	case 30: //Single-point information with time tag CP56Time2a
		func_M_SP_TB_1(&apdu_ex);//fs20150713
		break;
	case 31://双点带时间格式的
			func_M_DP_TB_1(&apdu_ex);
			break;
		//case 32://步位置信息
		//	func_M_ST_TB_1(msgbuf,len,asdu,seq);
		//	break;
	case 33://32位比特串     
		func_M_BO_TB_1(&apdu_ex);//原来就没有处理
		break;
		//case 34: //带时间格式的规度化值
		//case 35: //带时格式的标度化值
		//	scaled=(header->type==35) ? true:false;
		//	func_M_ME_TD_TE_1(msgbuf,len,asdu,seq,scaled);
		//	break;
	case 36:	// Measure value, short float point value w/CP56Time2a
		func_M_ME_TF_1(&apdu_ex);//fs20150713
		break;
		//case 37://累计值带时间
		//	func_M_IT_TB_1(msgbuf,len,asdu,seq);
		//	break;
	case 21:
		func_M_ME_ND_1(&apdu_ex);//规一化值、无时标、无品质
		break;
	case 45:
		//单点遥控
		func_C_SC_NA_1(&apdu_ex);
		break;
	case 46:
		//双点遥控
		func_C_DC_NA_1(&apdu_ex);
		break;
	case 47:
		//遥调
		func_C_RC_NA_1(&apdu_ex);
		break;
	case 48:
		//设定规一化值,规一化值
		func_C_SE_NA_1(&apdu_ex);
		break;
	case 50:
		//短浮点型
		func_C_SE_NC_1(&apdu_ex);
		break;
	case 58:
		//带时标的遥控命令
		func_C_SC_TA_1(&apdu_ex);
		break;
	case 63:
		//设定带时标的浮点型
		func_C_SE_TC_1(&apdu_ex);
		break;
	case 70:
		//初使化结束
		if(IEC101_LINK_SUB_STATE_SENDRSCONF==m_LinkData.Sub_State)
		{
	        m_LinkData.Sub_State=IEC101_LINK_SUB_STATE_INITFIN;//重置链路
		}
		TRACE("End of Initialization\n");
		break;
	case 100:
		ret=func_Interrogate(&apdu_ex);
		break;
	case 103:
		//时钟同步命令
		func_C_CS(&apdu_ex);
		break;
	default:
		decodeMsg_I_Ex(&apdu_ex);
		break;
	}
	bool bSendSFrame=false;

	if(m_cfgBase!=NULL)
	{
		bSendSFrame=m_cfgBase->m_bSendSFrame;
	}

	//发一条回一条
	send_S_Msg();
	//if(true==bSendSFrame)
	//{
	//	if(m_st->nCurW>=m_st->nW)
	//	{
	//		send_S_Msg();
	//		m_st->nCurW=0;
	//	}
	//}
	return ret;
}

//
//	Interrogation Command confirm
//
int Q_protocolIEC104Master::send_IC_Response(const int &asdu,const int &cause)
{
	/*TRACE("--------------------Q_protocolIEC104Master::send_IC_Response------------------------\n");*/
	Q_IEC104_ASDU msg;
	msg.header.type = 0x64;					// interrogation command   总命令
	msg.header.qual = 0x01;					// number of elements
	msg.header.tx_cause_1 = cause;			// 0x07 act confirm  0x0A 停止发送
	msg.header.tx_cause_2 = 0x00;
	msg.header.commom_asdu_1 = asdu & 0xFF;
	msg.header.commom_asdu_2 = (asdu>>8) & 0xFF;
	// group information
	msg.data[0] = 0x00;
	msg.data[1] = 0x00;//两个字节
	msg.data[2] = 0x14;//总召
	return send_I_Msg((BYTE*)&msg, 9);
}

bool Q_protocolIEC104Master::TxHandler()
{
	return 0;
}

int Q_protocolIEC104Master::func_C_SC_NA_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//对象地址

	Q_IEC104_ST_C_SP *ps = ( Q_IEC104_ST_C_SP *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;

	if(m_cfgControl->m_YkExecMode==eYtExecModeSelect)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//激活成功

			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkSelectCon,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值选择确认，信息体地址=%d，值=%d\n  ****",adr,val);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值选择确认，信息体地址=%d，值=%d\n  ****",adr,val);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				cmd->m_eState=eCmdStateYkConfirm;//select confirm
				cmd->m_nCurTryNum=0;

				/*				Q_dataManager* p = _GlobalData.getDataManager();
				//				LPQ_YKEDATATAG pYKData = p->getYKData();
				int nPos = 0;
				bool bExist = false;
				for (int i=0;i<p->getYKCount();i++)
				{
				if (m_ProSetting->getChannelID()==pYKData[i].channelid && pYKData[i].device==cmd->m_nComAsdu && pYKData[i].no==cmd->m_nInfoAddr)
				{
				bExist = true;
				nPos = i;
				break;
				}
				}
				if (bExist)
				{
				pYKData[nPos].selectreplytime = time(NULL);
				}
				*/

				/*by fanshuo*/
				bool bExist = false;
				int	YKID32 = -1;
				int YKtoYXID32 = -1;
				SCADA_Relay curRelay;
				bExist = FindYK(YKtoYXID32,YKID32,curRelay,(Q_cmdIEC104_SP *)cmd);

				if (bExist)//存在
				{
					if ((YKID32>0) && (&curRelay!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curRelay.selecttime = time;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控选择时间
					}
				}/*by fanshuo end*/
				else
				{
#ifdef _DEBUG
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 收到ChannelID=%d，device=%d,no=%d的遥控选择返回确认，但在遥控命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);

#endif
				}
			}
			else
			{
				Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
				if(cmd!=NULL)
				{
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						int val=ps->sco.scs;
						sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值成功，信息体地址=%d，值=%d  ****\n",adr,val);
						//OnShowMsg(LastError,sizeof(LastError));
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if(true/*m_bInfoMsg*/)
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						int val=ps->sco.scs;
						sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值成功，信息体地址=%d，值=%d  ****\n",adr,val);
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					//执行成功

					/*					Q_dataManager* p = _GlobalData.getDataManager();
					//					LPQ_YKEDATATAG pYKData = p->getYKData();
					for (int i=0;i<p->getYKCount();i++)
					{
					if (m_ProSetting->getChannelID()==pYKData[i].channelid && pYKData[i].device==cmd->m_nComAsdu && pYKData[i].no==cmd->m_nInfoAddr)
					{
					pYKData[i].finishtime = time(NULL);
					pYKData[i].execresult = 1;
					break;
					}
					}
					*/

					/*by fanshuo*/
					bool bExist = false;
					int	YKID32 = -1;
					int YKtoYXID32 = -1;
					SCADA_Relay curRelay;
					bExist = FindYK(YKtoYXID32,YKID32,curRelay,(Q_cmdIEC104_SP *)cmd);

					if (bExist)//存在
					{
						if ((YKID32>0) && (&curRelay!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							GetLocalTime(time);
							curRelay.exectime = time;
							curRelay.iexecret = 1;
							dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行成功时间
						}
					}
					/*by fanshuo end*/
					m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkExec,adr);
				}
			}
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//未执行成功
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkSelectCon,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"Execute command faild.mode Select.Command info:addr=%d,value=%s\n",adr,val==1?"on":"off");
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"Execute command faild.mode Select.Command info:addr=%d,value=%s\n",adr,val==1?"on":"off");
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				/*
				Q_dataManager* p = _GlobalData.getDataManager();
				//				LPQ_YKEDATATAG pYKData = p->getYKData();
				for (int i=0;i<p->getYKCount();i++)
				{
				if (m_ProSetting->getChannelID()==pYKData[i].channelid && pYKData[i].device==cmd->m_nComAsdu && pYKData[i].no==cmd->m_nInfoAddr)
				{
				pYKData[i].selectreplytime = time(NULL);
				pYKData[i].execresult = 3;
				break;
				}
				}
				*/		

				/*by fanshuo*/
				bool bExist = false;
				int	YKID32 = -1;
				int YKtoYXID32 = -1;
				SCADA_Relay curRelay;
				bExist = FindYK(YKtoYXID32,YKID32,curRelay,(Q_cmdIEC104_SP *)cmd);

				if (bExist)//存在
				{
					if ((YKID32>0) && (&curRelay!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						curRelay.selecttime = time;
						curRelay.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行失败时间
					}
				}
				/*by fanshuo end*/

				m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkSelectCon,adr);
			}
		}
		else
		{
			//可能是47之类的
			//<44> ：＝ 未知的类型标识
			//	<45> ：＝ 未知的传送原因
			//	<46> ：＝ 未知的应用服务数据单元公共地址
			//	<47> ：＝ 未知的信息对象地址

			//未执行成功
			/*			Q_dataManager* p = _GlobalData.getDataManager();
			//			LPQ_YKEDATATAG pYKData = p->getYKData();
			for (int i=0;i<p->getYKCount();i++)
			{
			if ((m_ProSetting->getChannelID()==pYKData[i].channelid) && (pYKData[i].device==msg->asdu) && (pYKData[i].no==adr))
			{
			pYKData[i].finishtime = time(NULL);
			pYKData[i].execresult = 3;
			break;
			}
			}
			*/
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkSelectCon,adr);

			/*by fanshuo*/
			bool bExist = false;
			int	YKID32 = -1;
			int YKtoYXID32 = -1;
			SCADA_Relay curRelay;
			bExist = FindYK(YKtoYXID32,YKID32,curRelay,(Q_cmdIEC104_SP *)cmd);

			if (bExist)//存在
			{
				if ((YKID32>0) && (&curRelay!=NULL))
				{
					CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
					SYSTEMTIME time = {0};
					curRelay.exectime = time;
					curRelay.iexecret = 3;
					dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行失败时间
				}
			}	
			/*by fanshuo end*/

			if(cmd!=NULL)
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值失败，信息体地址=%d，值=%d，原因（十进制）=%d  ****\n",adr,val,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			else
			{
#ifdef _DEBUG
				cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值失败，信息体地址=%d，值=%d，原因（十进制）=%d  ****\n",adr,val,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if (false==m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkSelectCon,adr))
			{
				m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkExec,adr);
			}
			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值失败，信息体地址=%d，值=%d，原因（十进制）=%d  ****\n",adr,val,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
		}
	}
	else if(m_cfgControl->m_YkExecMode==eYtExecModeDirect)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//激活确认
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值成功，信息体地址=%d，值=%d  ****\n",adr,val);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值成功，信息体地址=%d，值=%d  ****\n",adr,val);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				/*				Q_dataManager* p = _GlobalData.getDataManager();
				//				LPQ_YKEDATATAG pYKData = p->getYKData();
				for (int i=0;i<p->getYKCount();i++)
				{
				if (m_ProSetting->getChannelID()==pYKData[i].channelid && pYKData[i].device==cmd->m_nComAsdu && pYKData[i].no==cmd->m_nInfoAddr)
				{
				pYKData[i].finishtime = time(NULL);
				pYKData[i].execresult = 1;
				break;
				}
				}
				*/
				/*by fanshuo*/
				bool bExist = false;
				int	YKID32 = -1;
				int YKtoYXID32 = -1;
				SCADA_Relay curRelay;
				bExist = FindYK(YKtoYXID32,YKID32,curRelay,(Q_cmdIEC104_SP *)cmd);

				if (bExist)//存在
				{
					if ((YKID32>0) && (&curRelay!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curRelay.exectime = time;
						curRelay.iexecret = 1;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行成功时间-直接执行
					}
				}	
				/*by fanshuo end*/

				cmd->m_eState=eCmdStateYkExecAct;
				cmd->m_nCurTryNum=0;
			}
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{

			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值结束，设定成功，信息体地址=%d，值=%d  ****\n",adr,val);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExecAct,adr);
			if(true/*m_bInfoMsg*/)
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值结束，设定成功，信息体地址=%d，值=%d  ****\n",adr,val);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkExecAct,adr);
		}
		else 
		{
			//可能是47之类的
			//<44> ：＝ 未知的类型标识
			//	<45> ：＝ 未知的传送原因
			//	<46> ：＝ 未知的应用服务数据单元公共地址
			//	<47> ：＝ 未知的信息对象地址

			//未执行成功
			/*			Q_dataManager* p = _GlobalData.getDataManager();
			//			LPQ_YKEDATATAG pYKData = p->getYKData();
			for (int i=0;i<p->getYKCount();i++)
			{
			if (m_ProSetting->getChannelID()==pYKData[i].channelid && pYKData[i].device==msg->asdu && pYKData[i].no==adr)
			{
			pYKData[i].finishtime = time(NULL);
			pYKData[i].execresult = 3;
			break;
			}
			}
			*/			
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);

			/*by fanshuo*/
			bool bExist = false;
			int	YKID32 = -1;
			int YKtoYXID32 = -1;
			SCADA_Relay curRelay;
			bExist = FindYK(YKtoYXID32,YKID32,curRelay,(Q_cmdIEC104_SP *)cmd);

			if (bExist)//存在
			{
				if ((YKID32>0) && (&curRelay!=NULL))
				{
					CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
					SYSTEMTIME time = {0};
					curRelay.exectime = time;
					curRelay.iexecret = 3;
					dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控失败时间
				}
			}	
			/*by fanshuo end*/


			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值失败，信息体地址=%d，值=%d，原因（十进制）=%d  ****\n",adr,val,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			//			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
			if(true/*m_bInfoMsg*/)
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值失败，信息体地址=%d，值=%d，原因（十进制）=%d  ****\n",adr,val,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkExec,adr);
		}
	}

	return ret;

}

int Q_protocolIEC104Master::func_C_DC_NA_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//对象地址

	Q_IEC104_ST_C_DC *ps = ( Q_IEC104_ST_C_DC *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);


	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;


	if (eStation_Slave==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT) //激活
		{
			Q_cmdIEC104_DP *cmd=NULL;

			if (1==ps->dco.se)//选择
			{
				Q_cmdIEC104_DP *cmd=new Q_cmdIEC104_DP;

				Q_dataManager* p = _GlobalData.getDataManager();

				LPQ_DIGITALEDATATAG alg=NULL;
				if(m_tags!=NULL)
					p->findDigTag(msg->asdu,adr,m_tags->getAnalogBegin(),m_tags->getAnalogEnd());

				if (alg!=NULL)/*查询点表中数据处在*/
				{
					cmd->m_eState=eCmdStateYkSelect;
					cmd->m_nTxCause=msg->cause;
					cmd->m_nComAsdu=msg->asdu;
					cmd->m_bExec=false;
					cmd->m_cOnOff=ps->dco.scs;
					cmd->m_cQuality=ps->dco.qu;
					cmd->m_nValue=*((BYTE *)&(ps->dco));

					cmd->m_nInfoAddr=adr;//信息对象地址

				}
				else
				{
					//点表中不存在
					cmd->m_bError=true;
#ifdef _DEBUG
					m_errorNo=e104ErrNo_P_UnExist; //最好把详细错误记录一下。
					sprintf_s(LastError,sizeof(LastError), "信息对象地址所对应的点号不存在，当前地址为%d\n",adr);
					//OnErrorMsg(m_errorNo, LastError);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				m_listCmd->addCommand(cmd);//把命令压栈
			}
			else if (0==ps->dco.se)
			{
				//执行
				//根据点号、命令类型为单命令 等查询命令
				Q_cmdIEC104 *v_cmd=NULL;
				//根据状态值查询命令
				v_cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkConfirm,adr);
				if (v_cmd!=NULL)
				{
					//更新状态
					Q_cmdIEC104_SP *cmd=(Q_cmdIEC104_SP *)v_cmd;
					cmd->m_bExec=false;
					cmd->m_eState=eCmdStateYkExec;
					cmd->m_nValue=*((BYTE *)&(ps->dco));
				}
				else
				{
#ifdef _DEBUG
					m_errorNo=e104ErrNo_CmdNoSel;
					sprintf_s(LastError,sizeof(LastError), "不能执行单点遥控命令，地址错误或者是设备没有选择。当前行号:%d\n",__LINE__);
					//OnErrorMsg(m_errorNo, LastError);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
					return Q_104FAIL;
				}
			}
		}
		else if (msg->cause == CMD_DEACT) //停止激活
		{
			//撤消命令
			Q_cmdIEC104 *v_cmd=NULL;
			//根据状态值查询命令
			v_cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkConfirm,adr);
			if (v_cmd!=NULL)
			{
				//更新状态
				Q_cmdIEC104_SP *cmd=(Q_cmdIEC104_SP *)v_cmd;
				cmd->m_bExec=false;
				cmd->m_eState=eCmdStateYkCancelAct;
				cmd->m_nValue=*((BYTE *)&(ps->dco));
			}
			else
			{
				m_errorNo=e104ErrNo_CmdNoSel;
#ifdef _DEBUG
				sprintf_s(LastError,sizeof(LastError), "不能取消单点遥控命令，地址错误或者是设备没有选择。当前行号:%d\n",__LINE__);
				//OnErrorMsg(m_errorNo, LastError);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
				return Q_104FAIL;
			}
		}
	}
	else if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//选择确认
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//分执行完成，还是撤消完成
		}
	}

	return ret;
}

int Q_protocolIEC104Master::func_C_SE_NA_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//对象地址

	Q_IEC104_ST_C_SE *ps = ( Q_IEC104_ST_C_SE *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr =  ps->addr1<<8 | ps->addr0;

	if(m_cfgControl->m_YtExecMode==eYtExecModeSelect)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//激活成功
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkSelectCon,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
					memset(LastError,0,sizeof(LastError));
					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					if (pcmd->m_cFuncCode==0x3F)
					{
#ifdef _DEBUG
						float fval=0;
						fval = pcmd->m_fVal;
						sprintf_s(LastError,sizeof(LastError),"设定带时标的短浮点值选择确认，信息体地址=%d,值=%f\n",adr,fval);
#endif					
					}
					else
					{
						int val=0;
						if(ps->s==1)
						{
							short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
							val=-nval;
						}
						else
						{
							val=ps->val2<<8 |ps->val1;
						}
#ifdef _DEBUG
						sprintf_s(LastError,sizeof(LastError),"****  设定归一化值选择确认，信息体地址=%d，值=%d  ****\n",adr,val);
#endif					
					}
#ifdef _DEBUG
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				if(true)
				{
					memset(LastError,0,sizeof(LastError));
					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					if (pcmd->m_cFuncCode==0x3F)
					{
#ifdef _DEBUG
						float fval=0;
						fval = pcmd->m_fVal;
						sprintf_s(LastError,sizeof(LastError),"设定带时标的短浮点值选择确认，信息体地址=%d,值=%f\n",adr,fval);
#endif					
					}
					else
					{
						int val=0;
						if(ps->s==1)
						{
							short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
							val=-nval;
						}
						else
						{
							val=ps->val2<<8 |ps->val1;
						}
#ifdef _DEBUG
						sprintf_s(LastError,sizeof(LastError),"****  设定归一化值选择确认，信息体地址=%d，值=%d  ****\n",adr,val);
#endif					
					}
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}

				cmd->m_eState=eCmdStateYkConfirm;//select confirm
				cmd->m_nCurTryNum=0;

				Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
				Q_RECORDDATATAG ytRec;
				ytRec.eDataType=eDataTypeFloat;
				ytRec.channelID=m_ProSetting->getChannelID();
				ytRec.device=cmd->m_nComAsdu;
				ytRec.no=cmd->m_nInfoAddr;
				ytRec.fValue=pcmd->m_fVal;//赋初值

				if(!m_pMgr->getYtValue(&ytRec))
				{
					sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);

					/*//原CommApp规约打印保存、先保留着
					//OnShowMsg(LastError,sizeof(LastError));
					if (m_bInfoMsg)
					{
					sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					//	OnInfoMsg(LastError,strlen(LastError)+1);
					}
					*/

					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);

					return false;
				}

				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.selecttime = time;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调选择时间
					}
				}
				/*by fanshuo end*/
				else
				{
#ifdef _DEBUG
					SYSTEMTIME st;
					GetLocalTime(&st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 收到ChannelID=%d，device=%d,no=%d的遥调选择返回确认，但在遥调命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

					/*//原CommApp规约打印保存、先保留着
					//OnShowMsg(LastError,sizeof(LastError));
					if(true)
					{
					//	OnInfoMsg(LastError,sizeof(LastError),cmd);
					}
					*/

					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
			}
			else
			{
				Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
				if(cmd!=NULL)
				{
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						int val=0;
						if(ps->s==1)
						{
							short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
							val=-nval;

						}
						else
						{
							val=ps->val2<<8 |ps->val1;
						}
#ifdef _DEBUG
						sprintf_s(LastError,sizeof(LastError),"****  设定归一化值成功，信息体地址=%d，值=%d  ****\n",adr,val);
						//OnShowMsg(LastError,sizeof(LastError));

						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if(true/*m_bInfoMsg*/)
					{
						memset(LastError,0,sizeof(LastError));
						int val=0;
						if(ps->s==1)
						{
							short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
							val=-nval;

						}
						else
						{
							val=ps->val2<<8 |ps->val1;
						}
#ifdef _DEBUG
						sprintf_s(LastError,sizeof(LastError),"****  设定归一化值成功，信息体地址=%d，值=%d  ****\n",adr,val);
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}


					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					Q_RECORDDATATAG ytRec;
					ytRec.eDataType=eDataTypeFloat;
					ytRec.channelID=m_ProSetting->getChannelID();
					ytRec.device=cmd->m_nComAsdu;
					ytRec.no=cmd->m_nInfoAddr;
					ytRec.fValue=pcmd->m_fVal;//赋初值

					if(!m_pMgr->getYtValue(&ytRec))
					{
						sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);

						/*//原CommApp规约打印保存、先保留着
						//OnShowMsg(LastError,sizeof(LastError));
						if (m_bInfoMsg)
						{
						sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
						//	OnInfoMsg(LastError,strlen(LastError)+1);
						}
						*/

						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);

						return false;
					}

					/*by fanshuo*/
					bool bExist = false;
					int	YTID32 = -1;
					int YTtoYCID32 = -1;
					SCADA_SetPoint curSetPoint;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

					if (bExist)//存在
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							GetLocalTime(time);
							curSetPoint.exectime = time;
							curSetPoint.iexecret = 1;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调执行成功时间
						}
					}
					/*by fanshuo end*/


					m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);
				}
			}	
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//激活中止
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
			if(cmd!=NULL)
			{

				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
					memset(LastError,0,sizeof(LastError));
					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					if (pcmd->m_cFuncCode==0x3F)
					{
#ifdef _DEBUG
						float fval=0;
						fval = pcmd->m_fVal;
						sprintf_s(LastError,sizeof(LastError),"设定带时标的短浮点值激活终止，信息体地址=%d,值=%f\n",adr,fval);
#endif						
					}
					else
					{
						int val=0;
						if(ps->s==1)
						{
							short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
							val=-nval;

						}
						else
						{
							val=ps->val2<<8 |ps->val1;
						}
#ifdef _DEBUG
						sprintf_s(LastError,sizeof(LastError),"****  设定归一化值激活终止，信息体地址=%d,值=%d  ****\n",adr,val);
#endif						
					}
#ifdef _DEBUG
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
					memset(LastError,0,sizeof(LastError));
					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					if (pcmd->m_cFuncCode==0x3F)
					{
						float fval=0;
						fval = pcmd->m_fVal;
#ifdef _DEBUG
						sprintf_s(LastError,sizeof(LastError),"设定带时标的短浮点值激活终止，信息体地址=%d,值=%f\n",adr,fval);
#endif						
					}
					else
					{
						int val=0;
						if(ps->s==1)
						{
							short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
							val=-nval;

						}
						else
						{
							val=ps->val2<<8 |ps->val1;
						}
#ifdef _DEBUG
						sprintf_s(LastError,sizeof(LastError),"****  设定归一化值激活终止，信息体地址=%d,值=%d  ****\n",adr,val);
#endif						
					}
#ifdef _DEBUG
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调失败时间，激活中止
					}
				}
				/*by fanshuo end*/
				else
				{
#ifdef _DEBUG
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 收到ChannelID=%d，device=%d,no=%d的遥调执行完成确认，但在遥调命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
			}
		}
		else
		{
			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
				memset(LastError,0,sizeof(LastError));
				int val=0;
				if(ps->s==1)
				{
					short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
					val=-nval;
				}
				else
				{
					val=ps->val2<<8 |ps->val1;
				}
#ifdef _DEBUG
				sprintf_s(LastError,sizeof(LastError),"****  设定命令失败，信息体地址=%d，值=%d，原因（十进制）=%d  ****\n",adr,val,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(true/*m_bInfoMsg*/)
			{
				memset(LastError,0,sizeof(LastError));
				int val=0;
				if(ps->s==1)
				{
					short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
					val=-nval;
				}
				else
				{
					val=ps->val2<<8 |ps->val1;
				}
				Q_cmdIEC104 *pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkSelectCon,adr);
#ifdef _DEBUG				
				sprintf_s(LastError,sizeof(LastError),"****  设定命令失败，信息体地址=%d，值=%d，原因（十进制）=%d  ****\n",adr,val,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),pcmd);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkSelectCon,adr);
			if (NULL==cmd)
			{
				cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
				if (NULL!=cmd)
				{
					/*by fanshuo*/
					bool bExist = false;
					int	YTID32 = -1;
					int YTtoYCID32 = -1;
					SCADA_SetPoint curSetPoint;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

					if (bExist)//存在
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							curSetPoint.exectime = time;
							curSetPoint.iexecret = 3;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调失败时间
						}
					}
					/*by fanshuo end*/
				}
			}
			else
			{
				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调失败时间
					}
				}
				/*by fanshuo end*/
			}
			if(m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkSelectCon,adr)!=true)
			{
				m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);
			}
		}
	}
	else if(m_cfgControl->m_YtExecMode==eYtExecModeDirect)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//激活确认
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
					memset(LastError,0,sizeof(LastError));

					int val=0;
					if(ps->s==1)
					{
						short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
						val=-nval;

					}
					else
					{
						val=ps->val2<<8 |ps->val1;
					}
#ifdef _DEBUG
					sprintf_s(LastError,sizeof(LastError),"****  设定归一化值成功，信息体地址=%d，值=%d  ****\n",adr,val);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				int val=0;
				if(true/*m_bInfoMsg*/)
				{
					memset(LastError,0,sizeof(LastError));

					if(ps->s==1)
					{
						short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
						val=-nval;

					}
					else
					{
						val=ps->val2<<8 |ps->val1;
					}
#ifdef _DEBUG
					sprintf_s(LastError,sizeof(LastError),"****  设定归一化值成功，信息体地址=%d，值=%d  ****\n",adr,val);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.fVal = float(val);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 1;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调成功时间归一化值

						if (YTtoYCID32 > 0)
						{
							SCADA_Analog tempAnalog;
							dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);

							if (&tempAnalog != NULL)
							{
								tempAnalog.fVal = curSetPoint.fVal;
								tempAnalog.tSnapTime = time;
								dbmg_sdb->SetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);
							}
						}
					}	
				}
				/*by fanshuo end*/

				m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);
			}
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
				memset(LastError,0,sizeof(LastError));

				int val=0;
				if(ps->s==1)
				{
					short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
					val=-nval;

				}
				else
				{
					val=ps->val2<<8 |ps->val1;
				}
#ifdef _DEBUG
				sprintf_s(LastError,sizeof(LastError),"****  设定归一化值成功，信息体地址=%d，值=%d  ****\n",adr,val);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(true/*m_bInfoMsg*/)
			{
				memset(LastError,0,sizeof(LastError));

				int val=0;
				if(ps->s==1)
				{
					short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
					val=-nval;

				}
				else
				{
					val=ps->val2<<8 |ps->val1;
				}
#ifdef _DEBUG
				sprintf_s(LastError,sizeof(LastError),"****  设定归一化值成功，信息体地址=%d，值=%d  ****\n",adr,val);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(cmd!=NULL)
			{
				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 1;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调执行成功时间-直接模式
					}
				}
				/*by fanshuo end*/

			}
			m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
		}
		else 
		{
			//可能是47之类的
			//  <44> ：＝ 未知的类型标识
			//	<45> ：＝ 未知的传送原因
			//	<46> ：＝ 未知的应用服务数据单元公共地址
			//	<47> ：＝ 未知的信息对象地址

			//未执行成功
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
				memset(LastError,0,sizeof(LastError));

				int val=0;
				if(ps->s==1)
				{
					short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
					val=-nval;
				}
				else
				{
					val=ps->val2<<8 |ps->val1;
				}
#ifdef _DEBUG
				sprintf_s(LastError,sizeof(LastError),"****  设定归一化值失败，信息体地址=%d，值=%d，原因（十进制）=%d  ****\n",adr,val,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(true/*m_bInfoMsg*/)
			{
				memset(LastError,0,sizeof(LastError));

				int val=0;
				if(ps->s==1)
				{
					short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
					val=-nval;
				}
				else
				{
					val=ps->val2<<8 |ps->val1;
				}
#ifdef _DEBUG
				sprintf_s(LastError,sizeof(LastError),"****  设定归一化值失败，信息体地址=%d，值=%d，原因（十进制）=%d  ****\n",adr,val,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(cmd!=NULL)
			{
				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						//GetLocalTime(time);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调失败时间-直接模式
					}
				}
				/*by fanshuo end*/


			}
			m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);
		}
	}

	return ret;

}

int Q_protocolIEC104Master::func_C_SE_NC_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//对象地址

	Q_IEC104_ST_C_SE3 *ps = ( Q_IEC104_ST_C_SE3 *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;

	if(m_cfgControl->m_YtExecMode==eYtExecModeSelect)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//激活成功
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkSelectCon,adr);

			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					float val = 0;
					memcpy(&val,ps->val,sizeof(float));
					sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值选择确认，信息体地址=%d,值=%f  ****\n",adr,val);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					float val = 0;
					memcpy(&val,ps->val,sizeof(float));
					sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值选择确认，信息体地址=%d,值=%f  ****\n",adr,val);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				cmd->m_eState=eCmdStateYkConfirm;//select confirm
				cmd->m_nCurTryNum=0;
			}
			else
			{
				Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
				if(cmd!=NULL)
				{
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						float val = 0;
						memcpy(&val,ps->val,sizeof(float));
						sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值成功，信息体地址=%d,值=%f ****\n",adr,val);
						//OnShowMsg(LastError,sizeof(LastError));
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if(true/*m_bInfoMsg*/)
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						float val = 0;
						memcpy(&val,ps->val,sizeof(float));
						sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值成功，信息体地址=%d,值=%f ****\n",adr,val);
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					//执行成功
					m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);

				}
			}
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//未执行成功
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
			if(cmd!=NULL)
			{
				m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					float val = 0;
					memcpy(&val,ps->val,sizeof(float));
					sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值激活终止，信息体地址=%d,值=%f  ****\n",adr,val);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					float val = 0;
					memcpy(&val,ps->val,sizeof(float));
					sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值激活终止，信息体地址=%d,值=%f  ****\n",adr,val);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
			}
			else
			{	
				Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
				if(cmd!=NULL)
				{
					m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						float val = 0;
						memcpy(&val,ps->val,sizeof(float));
						sprintf_s(LastError,sizeof(LastError),"****  设定命令失败，信息体地址=%d,值=%f\n  ****",adr,val);
						//OnShowMsg(LastError,sizeof(LastError));
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if(true/*m_bInfoMsg*/)
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						float val = 0;
						memcpy(&val,ps->val,sizeof(float));
						sprintf_s(LastError,sizeof(LastError),"****  设定命令失败，信息体地址=%d,值=%f\n  ****",adr,val);
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
				}
			}
		}
		else
		{
			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				float val = 0;
				memcpy(&val,ps->val,sizeof(float));
				sprintf_s(LastError,sizeof(LastError),"****  设定浮点值失败，信息体地址=%d，值=%f，原因（十进制）=%d  ****\n",adr,val,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(true/*m_bInfoMsg*/)
			{
				Q_cmdIEC104 *pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkSelectCon,adr);
				if (NULL==pcmd)
				{
					pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					float val = 0;
					memcpy(&val,ps->val,sizeof(float));
					sprintf_s(LastError,sizeof(LastError),"****  设定浮点值失败，信息体地址=%d，值=%f，原因（十进制）=%d  ****\n",adr,val,msg->cause);
					//OnInfoMsg(LastError,sizeof(LastError),pcmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				else
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					float val = 0;
					memcpy(&val,ps->val,sizeof(float));
					sprintf_s(LastError,sizeof(LastError),"****  设定浮点值失败，信息体地址=%d，值=%f，原因（十进制）=%d  ****\n",adr,val,msg->cause);
					//OnInfoMsg(LastError,sizeof(LastError),pcmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
			}

			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkSelectCon,adr);
			if (NULL==cmd)
			{
				cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
				if (NULL!=cmd)
				{

					/*					Q_dataManager* p = _GlobalData.getDataManager();
					//					LPQ_YTEDATATAG pYTData = p->getYTData();
					for (int i=0;i<p->getYTCount();i++)
					{
					if (m_ProSetting->getChannelID()==pYTData[i].channelid && pYTData[i].device==cmd->m_nComAsdu && pYTData[i].no==cmd->m_nInfoAddr)
					{
					pYTData[i].finishtime=time(NULL);
					pYTData[i].execresult = 3;
					break;
					}
					}
					*/
					/*by fanshuo*/
					bool bExist = false;
					int	YTID32 = -1;
					int YTtoYCID32 = -1;
					SCADA_SetPoint curSetPoint;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

					if (bExist)//存在
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							//GetLocalTime(time);
							curSetPoint.exectime = time;
							curSetPoint.iexecret = 3;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调失败时间-设置浮点值
						}
					}
					/*by fanshuo end*/
				}
			}
			else
			{

				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						//GetLocalTime(time);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥控执行失败时间
					}
				}
				/*by fanshuo end*/

			}
			if(m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkSelectCon,adr)!=true)
			{
				m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);
			}
		}
	}
	else if(m_cfgControl->m_YtExecMode==eYtExecModeDirect)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//激活确认
			Q_cmdIEC104*cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
					Q_cmdIEC104_SE*v_cmd=(Q_cmdIEC104_SE*)cmd;
					if(v_cmd!=NULL)
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command ActCon success .Mode Exec.Command info:addr=%d,value=%.3f\n",adr,v_cmd->m_fVal);

						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}

				}

				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);

						float val = 0;
						memcpy(&val,ps->val,sizeof(float));

						curSetPoint.fVal = val;
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 1;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调成功时间归一化值

						if (YTtoYCID32 > 0)
						{
							SCADA_Analog tempAnalog;
							dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);

							if (&tempAnalog != NULL)
							{
								tempAnalog.fVal = val;
								tempAnalog.tSnapTime = time;
								dbmg_sdb->SetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);
							}
						}
					}	
				}
				/*by fanshuo end*/

				cmd->m_eState=eCmdStateYkExecAct;
				cmd->m_nCurTryNum=0;
			}
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//未执行成功

			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"Execute command term success.Mode Exec.Command info:addr=%d\n",adr);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(true/*m_bInfoMsg*/)
			{
				Q_cmdIEC104*pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
#ifdef _DEBUG				
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"Execute command term success.Mode Exec.Command info:addr=%d\n",adr);
				//OnInfoMsg(LastError,sizeof(LastError),pcmd);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
		}
		else 
		{
			//可能是47之类的
			//<44> ：＝ 未知的类型标识
			//	<45> ：＝ 未知的传送原因
			//	<46> ：＝ 未知的应用服务数据单元公共地址
			//	<47> ：＝ 未知的信息对象地址


			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值失败，信息体地址=%d，原因（十进制）=%d\n",adr,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(true/*m_bInfoMsg*/)
			{
				Q_cmdIEC104*pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
#ifdef _DEBUG				
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值失败，信息体地址=%d，原因（十进制）=%d\n",adr,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),pcmd);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			//未执行成功
			m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);
		}
	}
	return ret;
}




int Q_protocolIEC104Master::func_C_SC_TA_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;
	DWORD adr;//对象地址

	Q_IEC104_ST_C_SC_TA1 *ps = ( Q_IEC104_ST_C_SC_TA1 *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;

	if(m_cfgControl->m_YkExecMode==eYtExecModeSelect)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//激活成功

			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkSelectCon,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值选择确认，信息体地址=%d，值=%d\n  ****",adr,val);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值选择确认，信息体地址=%d，值=%d\n  ****",adr,val);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				cmd->m_eState=eCmdStateYkConfirm;//select confirm
				cmd->m_nCurTryNum=0;

				/*				Q_dataManager* p = _GlobalData.getDataManager();
				//				LPQ_YKEDATATAG pYKData = p->getYKData();
				int nPos = 0;
				bool bExist = false;
				for (int i=0;i<p->getYKCount();i++)
				{
				if (m_ProSetting->getChannelID()==pYKData[i].channelid && pYKData[i].device==cmd->m_nComAsdu && pYKData[i].no==cmd->m_nInfoAddr)
				{
				bExist = true;
				nPos = i;
				break;
				}
				}
				if (bExist)
				{
				pYKData[nPos].selectreplytime = time(NULL);
				}
				*/

				/*by fanshuo*/
				bool bExist = false;
				int	YKID32 = -1;
				int YKtoYXID32 = -1;
				SCADA_Relay curRelay;
				bExist = FindYK(YKtoYXID32,YKID32,curRelay,(Q_cmdIEC104_SP *)cmd);

				if (bExist)//存在
				{
					if ((YKID32>0) && (&curRelay!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curRelay.selecttime = time;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行时间
					}
				}	
				/*by fanshuo end*/
				else
				{
#ifdef _DEBUG
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 收到ChannelID=%d，device=%d,no=%d的遥控选择返回确认，但在遥控命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

					/*//原CommApp规约打印保存、先保留着
					//OnShowMsg(LastError,sizeof(LastError));
					if(true)
					{
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					}
					*/

					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
			}
			else
			{
				Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
				if(cmd!=NULL)
				{
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						int val=ps->sco.scs;
						sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值成功，信息体地址=%d，值=%d  ****\n",adr,val);
						//OnShowMsg(LastError,sizeof(LastError));
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if(true/*m_bInfoMsg*/)
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						int val=ps->sco.scs;
						sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值成功，信息体地址=%d，值=%d  ****\n",adr,val);
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}


					/*by fanshuo*/
					bool bExist = false;
					int	YKID32 = -1;
					int YKtoYXID32 = -1;
					SCADA_Relay curRelay;
					bExist = FindYK(YKtoYXID32,YKID32,curRelay,(Q_cmdIEC104_SP *)cmd);

					if (bExist)//存在
					{
						if ((YKID32>0) && (&curRelay!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							GetLocalTime(time);
							curRelay.exectime = time;
							curRelay.iexecret = 1;
							dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行时间
						}
					}	
					/*by fanshuo end*/

					m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkExec,adr);
				}
			}

		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//未执行成功
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkSelectCon,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"Execute command faild.mode Select.Command info:addr=%d,value=%s\n",adr,val==1?"on":"off");
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"Execute command faild.mode Select.Command info:addr=%d,value=%s\n",adr,val==1?"on":"off");
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				/*				Q_dataManager* p = _GlobalData.getDataManager();
				//				LPQ_YKEDATATAG pYKData = p->getYKData();
				for (int i=0;i<p->getYKCount();i++)
				{
				if (m_ProSetting->getChannelID()==pYKData[i].channelid && pYKData[i].device==cmd->m_nComAsdu && pYKData[i].no==cmd->m_nInfoAddr)
				{
				pYKData[i].selectreplytime = time(NULL);
				pYKData[i].execresult = 3;
				break;
				}
				}
				*/
				/*by fanshuo*/
				bool bExist = false;
				int	YKID32 = -1;
				int YKtoYXID32 = -1;
				SCADA_Relay curRelay;
				bExist = FindYK(YKtoYXID32,YKID32,curRelay,(Q_cmdIEC104_SP *)cmd);

				if (bExist)//存在
				{
					if ((YKID32>0) && (&curRelay!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						curRelay.selecttime = time;
						curRelay.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行时间
					}
				}	
				/*by fanshuo end*/

				m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkSelectCon,adr);
			}
		}
		else
		{
			//可能是47之类的
			//<44> ：＝ 未知的类型标识
			//	<45> ：＝ 未知的传送原因
			//	<46> ：＝ 未知的应用服务数据单元公共地址
			//	<47> ：＝ 未知的信息对象地址

			//未执行成功
			/*			Q_dataManager* p = _GlobalData.getDataManager();
			//			LPQ_YKEDATATAG pYKData = p->getYKData();
			for (int i=0;i<p->getYKCount();i++)
			{
			if (m_ProSetting->getChannelID()==pYKData[i].channelid && pYKData[i].device==msg->asdu && pYKData[i].no==adr)
			{
			pYKData[i].finishtime = time(NULL);
			pYKData[i].execresult = 3;
			break;
			}
			}
			*/
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkSelectCon,adr);
			/*by fanshuo*/
			bool bExist = false;
			int	YKID32 = -1;
			int YKtoYXID32 = -1;
			SCADA_Relay curRelay;
			bExist = FindYK(YKtoYXID32,YKID32,curRelay,(Q_cmdIEC104_SP *)cmd);

			if (bExist)//存在
			{
				if ((YKID32>0) && (&curRelay!=NULL))
				{
					CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
					SYSTEMTIME time = {0};
					curRelay.selecttime = time;
					curRelay.iexecret = 3;
					dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行时间
				}
			}	
			/*by fanshuo end*/


			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值失败，信息体地址=%d，值=%d，原因（十进制）=%d  ****\n",adr,val,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(true/*m_bInfoMsg*/)
			{
				Q_cmdIEC104 *pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkSelectCon,adr);
				if (pcmd!=NULL)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值失败，信息体地址=%d，值=%d，原因（十进制）=%d  ****\n",adr,val,msg->cause);
					//OnInfoMsg(LastError,sizeof(LastError),pcmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				else
				{
					pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值失败，信息体地址=%d，值=%d，原因（十进制）=%d  ****\n",adr,val,msg->cause);
					//OnInfoMsg(LastError,sizeof(LastError),pcmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
			}
			if (false==m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkSelectCon,adr))
			{
				m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkExec,adr);
			}
		}
	}
	else if(m_cfgControl->m_YkExecMode==eYtExecModeDirect)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//激活确认
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值成功，信息体地址=%d，值=%d  ****\n",adr,val);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值成功，信息体地址=%d，值=%d  ****\n",adr,val);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}


				/*				Q_dataManager* p = _GlobalData.getDataManager();
				//				LPQ_YKEDATATAG pYKData = p->getYKData();
				for (int i=0;i<p->getYKCount();i++)
				{
				if (m_ProSetting->getChannelID()==pYKData[i].channelid && pYKData[i].device==cmd->m_nComAsdu && pYKData[i].no==cmd->m_nInfoAddr)
				{
				pYKData[i].finishtime = time(NULL);
				pYKData[i].execresult = 1;
				break;
				}
				}
				*/
				/*by fanshuo*/
				bool bExist = false;
				int	YKID32 = -1;
				int YKtoYXID32 = -1;
				SCADA_Relay curRelay;
				bExist = FindYK(YKtoYXID32,YKID32,curRelay,(Q_cmdIEC104_SP *)cmd);

				if (bExist)//存在
				{
					if ((YKID32>0) && (&curRelay!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curRelay.exectime = time;
						curRelay.iexecret = 1;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行时间
					}
				}
				/*by fanshuo end*/

				cmd->m_eState=eCmdStateYkExecAct;
				cmd->m_nCurTryNum=0;
			}
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{

			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值结束，设定成功，信息体地址=%d，值=%d  ****\n",adr,val);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);

#endif
			}
			if(true/*m_bInfoMsg*/)
			{
#ifdef _DEBUG
				Q_cmdIEC104 *pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExecAct,adr);
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值结束，设定成功，信息体地址=%d，值=%d  ****\n",adr,val);
				//OnInfoMsg(LastError,sizeof(LastError),pcmd);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkExecAct,adr);
		}
		else 
		{
			//可能是47之类的
			//<44> ：＝ 未知的类型标识
			//	<45> ：＝ 未知的传送原因
			//	<46> ：＝ 未知的应用服务数据单元公共地址
			//	<47> ：＝ 未知的信息对象地址

			//未执行成功
			/*			Q_dataManager* p = _GlobalData.getDataManager();
			//			LPQ_YKEDATATAG pYKData = p->getYKData();
			for (int i=0;i<p->getYKCount();i++)
			{
			if (m_ProSetting->getChannelID()==pYKData[i].channelid && pYKData[i].device==msg->asdu && pYKData[i].no==adr)
			{
			pYKData[i].finishtime = time(NULL);
			pYKData[i].execresult = 3;
			break;
			}
			}
			*/			
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
			/*by fanshuo*/
			bool bExist = false;
			int	YKID32 = -1;
			int YKtoYXID32 = -1;
			SCADA_Relay curRelay;
			bExist = FindYK(YKtoYXID32,YKID32,curRelay,(Q_cmdIEC104_SP *)cmd);

			if (bExist)//存在
			{
				if ((YKID32>0) && (&curRelay!=NULL))
				{
					CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
					SYSTEMTIME time = {0};
					curRelay.exectime = time;
					curRelay.iexecret = 3;
					dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行时间
				}
			}
			/*by fanshuo end*/

			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值失败，信息体地址=%d，值=%d，原因（十进制）=%d  ****\n",adr,val,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}

			if(true/*m_bInfoMsg*/)
			{
#ifdef _DEBUG
				Q_cmdIEC104 *pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值失败，信息体地址=%d，值=%d，原因（十进制）=%d  ****\n",adr,val,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),pcmd);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkExec,adr);
		}
	}

	return ret;
}

int Q_protocolIEC104Master::func_C_SE_TC_1(Q_104ApduEx *msg)
{
	//cause test:1 ok:1 activeOK:6
	// 	int nCause = msg->cause & 0x1F;
	// 	int nActiveOK = msg->cause >>6 & 0x01;
	// 	int nTest = msg->cause >> 7 & 0x01;
	int ret=Q_104SUC;

	DWORD adr;//对象地址

	Q_IEC104_ST_C_SE_TC1 *ps = ( Q_IEC104_ST_C_SE_TC1 *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;

	if(m_cfgControl->m_YtExecMode==eYtExecModeSelect)
	{
		if (msg->cause== CMD_ACT_CON)
		{
			//激活成功
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkSelectCon,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					sprintf_s(LastError,sizeof(LastError),"****  设定带时标的短浮点值选择确认，信息体地址=%d，值=%f  ****\n",adr,pcmd->m_fVal);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					sprintf_s(LastError,sizeof(LastError),"****  设定带时标的短浮点值选择确认，信息体地址=%d，值=%f  ****\n",adr,pcmd->m_fVal);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				cmd->m_eState=eCmdStateYkConfirm;//select confirm
				cmd->m_nCurTryNum=0;


				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.selecttime = time;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥控执行时间
					}
				}
				/*by fanshuo end*/
				else
				{
#ifdef _DEBUG
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 收到ChannelID=%d，device=%d,no=%d的遥调选择返回确认，但在遥调命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

					/*//原CommApp规约打印保存、先保留着
					//OnShowMsg(LastError,sizeof(LastError));
					if(true)
					{
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					}
					*/
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
			}
			else
			{
				Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
				if(cmd!=NULL)
				{
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
						sprintf_s(LastError,sizeof(LastError),"****  设定带时标的短浮点值成功，信息体地址=%d，值=%f  ****\n",adr,pcmd->m_fVal);
						//OnShowMsg(LastError,sizeof(LastError));
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if(true/*m_bInfoMsg*/)
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
						sprintf_s(LastError,sizeof(LastError),"****  设定带时标的短浮点值成功，信息体地址=%d，值=%f  ****\n",adr,pcmd->m_fVal);
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}

					/*by fanshuo*/
					bool bExist = false;
					int	YTID32 = -1;
					int YTtoYCID32 = -1;
					SCADA_SetPoint curSetPoint;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

					if (bExist)//存在
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							GetLocalTime(time);
							curSetPoint.exectime = time;
							curSetPoint.iexecret = 1;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥控执行时间
						}
					}
					/*by fanshuo end*/

					m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);
				}
			}
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//激活中止
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					sprintf_s(LastError,sizeof(LastError),"Execute command term success.Mode Exec.Command info:addr=%d,value=%f\n",adr,pcmd->m_fVal);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					sprintf_s(LastError,sizeof(LastError),"Execute command term success.Mode Exec.Command info:addr=%d,value=%f\n",adr,pcmd->m_fVal);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 1;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥控执行时间
					}
				}
				/*by fanshuo end*/
				else
				{
#ifdef _DEBUG
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 收到ChannelID=%d，device=%d,no=%d的遥调执行完成确认，但在遥调命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

					/*//原CommApp规约打印保存、先保留着
					//OnShowMsg(LastError,sizeof(LastError));
					if(true)
					{
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					}
					*/

					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
			}
		}
		else
		{
			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"****  设定带时标的浮点值失败，信息体地址=%d，原因（十进制）=%d  ****\n",adr,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(true/*m_bInfoMsg*/)
			{
				Q_cmdIEC104 *pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkSelectCon,adr);
				if (pcmd!=NULL)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					sprintf_s(LastError,sizeof(LastError),"****  设定带时标的浮点值失败，信息体地址=%d，原因（十进制）=%d  ****\n",adr,msg->cause);
					//OnInfoMsg(LastError,sizeof(LastError),pcmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				else
				{
#ifdef _DEBUG
					pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
					memset(LastError,0,sizeof(LastError));
					sprintf_s(LastError,sizeof(LastError),"****  设定带时标的浮点值失败，信息体地址=%d，原因（十进制）=%d  ****\n",adr,msg->cause);
					//OnInfoMsg(LastError,sizeof(LastError),pcmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
			}
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkSelectCon,adr);
			if (NULL==cmd)
			{
				cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
				if (NULL!=cmd)
				{
					/*
					Q_dataManager* p = _GlobalData.getDataManager();
					//					LPQ_YTEDATATAG pYTData = p->getYTData();
					for (int i=0;i<p->getYTCount();i++)
					{
					if (m_ProSetting->getChannelID()==pYTData[i].channelid && pYTData[i].device==cmd->m_nComAsdu && pYTData[i].no==cmd->m_nInfoAddr)
					{
					pYTData[i].finishtime=time(NULL);
					pYTData[i].execresult = 3;
					break;
					}
					}
					*/
					/*by fanshuo*/
					bool bExist = false;
					int	YTID32 = -1;
					int YTtoYCID32 = -1;
					SCADA_SetPoint curSetPoint;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

					if (bExist)//存在
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							GetLocalTime(time);
							curSetPoint.exectime = time;
							curSetPoint.iexecret = 3;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥控执行时间
						}
					}
					/*by fanshuo end*/
				}
			}
			else
			{
				/*
				Q_dataManager* p = _GlobalData.getDataManager();
				//				LPQ_YTEDATATAG pYTData = p->getYTData();
				for (int i=0;i<p->getYTCount();i++)
				{
				if (m_ProSetting->getChannelID()==pYTData[i].channelid && pYTData[i].device==cmd->m_nComAsdu && pYTData[i].no==cmd->m_nInfoAddr)
				{
				pYTData[i].selectreplytime=time(NULL);
				pYTData[i].execresult = 3;
				break;
				}
				}
				*/
				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.selecttime = time;
						curSetPoint.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥控执行时间
					}
				}
				/*by fanshuo end*/

			}

			if(m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkSelectCon,adr)!=true)
			{
				m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);
			}
		}
	}
	else if(m_cfgControl->m_YtExecMode==eYtExecModeDirect)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//激活确认
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					sprintf_s(LastError,sizeof(LastError),"****  设定带时标的浮点值成功，信息体地址=%d，值=%f  ****\n",adr,pcmd->m_fVal);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					sprintf_s(LastError,sizeof(LastError),"****  设定带时标的浮点值成功，信息体地址=%d，值=%f  ****\n",adr,pcmd->m_fVal);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				/*
				Q_dataManager* p = _GlobalData.getDataManager();
				//				LPQ_YTEDATATAG pYTData = p->getYTData();
				for (int i=0;i<p->getYTCount();i++)
				{
				if (m_ProSetting->getChannelID()==pYTData[i].channelid && pYTData[i].device==cmd->m_nComAsdu && pYTData[i].no==cmd->m_nInfoAddr)
				{
				pYTData[i].finishtime = time(NULL);
				pYTData[i].execresult = 1;
				break;
				}
				}
				*/

				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 1;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥控执行时间
					}
				}
				/*by fanshuo end*/

				m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);
				//6.6
				// 				cmd->m_eState=eCmdStateYkExecAct;
				// 				cmd->m_nCurTryNum=0;
			}
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
				sprintf_s(LastError,sizeof(LastError),"Execute command term success.Mode Exec.Command info:addr=%d,value=%f\n",adr,pcmd->m_fVal);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(true/*m_bInfoMsg*/)
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
				sprintf_s(LastError,sizeof(LastError),"Execute command term success.Mode Exec.Command info:addr=%d,value=%f\n",adr,pcmd->m_fVal);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(cmd!=NULL)
			{
				/*
				Q_dataManager* p = _GlobalData.getDataManager();
				//				LPQ_YTEDATATAG pYTData = p->getYTData();
				for (int i=0;i<p->getYTCount();i++)
				{
				if (m_ProSetting->getChannelID()==pYTData[i].channelid && pYTData[i].device==cmd->m_nComAsdu && pYTData[i].no==cmd->m_nInfoAddr)
				{
				pYTData[i].finishtime = time(NULL);
				pYTData[i].execresult = 1;
				break;
				}
				}
				*/
				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 1;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥控执行时间
					}
				}
				/*by fanshuo end*/
			}
			m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
		}
		else 
		{

			//未执行成功
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"****  设定带时标的浮点值失败，信息体地址=%d，原因（十进制）=%d  ****\n",adr,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(true/*m_bInfoMsg*/)
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"****  设定带时标的浮点值失败，信息体地址=%d，原因（十进制）=%d  ****\n",adr,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(cmd!=NULL)
			{
				/*
				Q_dataManager* p = _GlobalData.getDataManager();
				//				LPQ_YTEDATATAG pYTData = p->getYTData();
				for (int i=0;i<p->getYTCount();i++)
				{
				if (m_ProSetting->getChannelID()==pYTData[i].channelid && pYTData[i].device==cmd->m_nComAsdu && pYTData[i].no==cmd->m_nInfoAddr)
				{
				pYTData[i].finishtime = time(NULL);
				pYTData[i].execresult = 3;
				break;
				}
				}
				*/
				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥控执行时间
					}
				}
				/*by fanshuo end*/
			}
			m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);
		}
	}

	return ret;
}

int Q_protocolIEC104Master::func_C_RC_NA_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//对象地址

	Q_IEC104_ST_C_RC *ps = ( Q_IEC104_ST_C_RC *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);


	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;


	if (eStation_Slave==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT) //激活
		{
			Q_cmdIEC104_SP *cmd=NULL;

			if (1==ps->rco.se)//选择
			{
				Q_cmdIEC104_RC *cmd=new Q_cmdIEC104_RC;

				Q_dataManager* p = _GlobalData.getDataManager();
				LPQ_ANALOGUEDATATAG alg=NULL;
				if(m_tags!=NULL)
					p->findAlgTag(msg->asdu,adr,m_tags->getAnalogBegin(),m_tags->getAnalogEnd());

				if (alg!=NULL)/*查询点表中数据处在*/
				{
					cmd->m_eState=eCmdStateYkSelect;
					cmd->m_nTxCause=msg->cause;
					cmd->m_nComAsdu=msg->asdu;
					cmd->m_bExec=false;


					cmd->m_cOnOff=ps->rco.rcs&0xFF;

					cmd->m_cQuality=ps->rco.qu;

					//用于发送确认激活报文用
					memcpy((void *)&cmd->m_nValue,(void *)&ps->rco,sizeof(cmd->m_nValue));
					cmd->m_nInfoAddr=adr;//信息对象地址

				}
				else
				{
					//点表中不存在
					cmd->m_bError=true;
					m_errorNo=e104ErrNo_P_UnExist; //最好把详细错误记录一下。
#ifdef _DEBUG
					sprintf_s(LastError,sizeof(LastError), "信息对象地址所对应的点号不存在，当前地址为%d\n",adr);
					//OnErrorMsg(m_errorNo, LastError);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				m_listCmd->addCommand(cmd);//把命令压栈
			}
			else if (0==ps->rco.se)
			{
				//执行
				//根据点号、命令类型为单命令 等查询命令
				Q_cmdIEC104 *v_cmd=NULL;
				//根据状态值查询命令
				v_cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkConfirm,adr);
				if (v_cmd!=NULL)
				{
					//更新状态
					Q_cmdIEC104_RC *cmd=(Q_cmdIEC104_RC *)v_cmd;
					cmd->m_bExec=false;
					cmd->m_eState=eCmdStateYkExec;
				}
				else
				{
					m_errorNo=e104ErrNo_CmdNoSel;
#ifdef _DEBUG
					sprintf_s(LastError,sizeof(LastError), "不能执行单点遥控命令，地址错误或者是设备没有选择。当前行号:%d\n",__LINE__);
					//OnErrorMsg(m_errorNo, LastError);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
					return Q_104FAIL;
				}
			}

		}
		else if (msg->cause == CMD_DEACT) //停止激活
		{
			//撤消命令
			Q_cmdIEC104 *v_cmd=NULL;
			//根据状态值查询命令
			v_cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkConfirm,adr);
			if (v_cmd!=NULL)
			{
				//更新状态
				Q_cmdIEC104_RC *cmd=(Q_cmdIEC104_RC *)v_cmd;
				cmd->m_bExec=false;
				cmd->m_eState=eCmdStateYkCancelAct;
			}
			else
			{

				m_errorNo=e104ErrNo_CmdNoSel;
#ifdef _DEBUG
				sprintf_s(LastError,sizeof(LastError), "不能取消单点遥控命令，地址错误或者是设备没有选择。当前行号:%d\n",__LINE__);
				//OnErrorMsg(m_errorNo, LastError);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
				return Q_104FAIL;
			}

		}

	}
	else if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//选择确认
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//分执行完成，还是撤消完成
		}
	}

	return ret;

}

int Q_protocolIEC104Master::func_C_CS(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//对象地址

	Q_IEC104_ST_C_CS *ps = ( Q_IEC104_ST_C_CS *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);


	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;


	if (eStation_Slave==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT) //激活
		{
			Q_cmdIEC104_CS *cmd=new Q_cmdIEC104_CS;
			cmd->setCommand104Type(eCmdTypeC_SC);//单点遥控

			cmd->m_eState=eCmdStateSync;
			cmd->m_nTxCause=msg->cause;
			cmd->m_nComAsdu=msg->asdu;
			cmd->m_bExec=false;
			cmd->m_nInfoAddr=adr;//信息对象地址
			cmd->m_nPointNo=0;//TODO:点表中点号
			memcpy(cmd->m_cTime,ps->val,sizeof(cmd->m_cTime));

			m_listCmd->addCommand(cmd);//把命令压栈
		}
	}
	else if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//选择确认
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//分执行完成，还是撤消完成
		}
	}
	return ret;

}

int Q_protocolIEC104Master::func_Interrogate(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	//查询 传送原因为CMD_ACT,类型为总召，且未处理的，且
	//Interrogation Command (Group %d): Activation Confirmation.

	sprintf_s(LastError,sizeof(LastError),"execute Interrogate begin ... \n");
	//OnShowMsg(LastError,sizeof(LastError));n

	list<Q_IEC104StandardCommand *> v_listCmd;
	int len=m_listCmd->getCommandByStatus(v_listCmd);//查无错误的命令列表
	if (len>0)
	{
		for (IT_CmdListP it=v_listCmd.begin();it!=v_listCmd.end();it++)
		{
			Q_cmdIEC104 *cur_cmd=(Q_cmdIEC104 *)*it;
			if(cur_cmd!=NULL)
			{
				if(cur_cmd->getCommand104Type()==eCmdTypeTestFR)
					continue;

				//查询当前未处理的，解状态值也为未处理的命令
				if (cur_cmd->m_eState!=eCmdStateEnd && cur_cmd->m_eState!=eCmdStateNull)
				{
					//处理内容
					int icmd = cur_cmd->getCommand104Type();
					switch (icmd)
					{
					case eCmdTypeC_IC:
						sprintf_s(LastError,sizeof(LastError),"execute Interrogate end ...,status=%d\n",cur_cmd->m_eState);
						//OnShowMsg(LastError,sizeof(LastError));n

						if (msg->cause == CMD_ACT_CON)
						{
							if(cur_cmd->m_eState==eCmdStateIntRequest)
							{
								cur_cmd->m_eState=eCmdStateIntRequestAct;//请求确认完成
							}
							m_LinkData.nRecvCount=1;
						}
						else if (msg->cause ==CMD_DEACT_TERM)
						{
							//查询状态更新状态值
							if(cur_cmd->m_eState==eCmdStateIntRequestAct)
							{
								cur_cmd->m_eState=eCmdStateIntEnd;
								cur_cmd->m_bExec=false;
							}
							//Interrogation Command (Group %d): Activation Termination.
							//cur_cmd->setCommand104Type(eCmdTypeTestFR);
						}

						sprintf_s(LastError,sizeof(LastError),"execute Interrogate end ...,status=%d\n",cur_cmd->m_eState);
						//OnShowMsg(LastError,sizeof(LastError));n

					}
				}
			}
		}
	}
	sprintf_s(LastError,sizeof(LastError),"execute Interrogate end ...,len=%d\n",len);
	//OnShowMsg(LastError,sizeof(LastError));n

	return ret;
}

void Q_protocolIEC104Master::func_M_SP_NA_1(Q_104ApduEx *msg)
{
	UINT pos=0;
	DWORD adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq)
	{
		Q_SP104 *ps = (Q_SP104*)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);


		adr = ps->addr1<<8 | ps->addr0;

#ifdef _DEBUG
		sprintf_s(LastError,sizeof(LastError), "SP_%d: %d\tValid: %d\tBlocked: %d\n",
			adr, ps->siq.spi, ps->siq.iv, ps->siq.bl);
		//OnErrorMsg(m_errorNo, LastError);n
#endif
		// call callback //TODO:

		pos +=2;//地址域
		Q_SIQ104	*pi;
		while (pos < msg->len)
		{
			pi = (Q_SIQ104*)&(msg->msgbuf[pos]);

			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeBit;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
			dataArray.bValue = pi->spi==1?true:false;

			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			pos+=sizeof(Q_SIQ104);
			adr++;
		}
	}
	else
	{

		while (pos < msg->len)
		{
			Q_SP104 *ps = (Q_SP104*)&(msg->msgbuf[pos]);

			adr =  ps->addr1<<8 | ps->addr0;

			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeBit;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
			dataArray.bValue = ps->siq.spi==1?true:false;

			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			pos+=sizeof(Q_SP104);
		}
	}
}


void Q_protocolIEC104Master::func_M_DP_NA_1(Q_104ApduEx *msg)
{
	/*
	unsigned int pos;
	unsigned long adr;//对象地址
	if (msg->seq)
	{
	Q_DP104 *ps = (Q_DP104*)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;

	#ifdef _DEBUG
	sprintf_s(LastError,sizeof(LastError), "DP_%d: %d\tValid: %d\tBlocked: %d\n",
	adr, ps->diq.dpi, ps->diq.iv, ps->diq.bl);
	//OnErrorMsg(m_errorNo, LastError);n
	#endif
	// call callback //TODO:

	pos = sizeof(Q_IEC104_ASDU_header)+sizeof(Q_DP104);
	Q_DIQ104	*pi;
	while (pos < msg->len)
	{
	adr++;
	pi = (Q_DIQ104*)&(msg->msgbuf[pos]);
	#ifdef _DEBUG
	sprintf_s(LastError,sizeof(LastError), "DP_%d: %d\tValid: %d\tBlocked: %d\n",
	adr, pi->dpi, pi->iv, pi->bl);
	//OnErrorMsg(m_errorNo, LastError);n
	#endif
	// call callback TODO::
	pos++;
	}
	}
	else
	{
	pos = sizeof(Q_IEC104_ASDU_header);
	while (pos < msg->len)
	{
	Q_DP104 *ps = (Q_DP104*)&(msg->msgbuf[pos]);


	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;

	#ifdef _DEBUG
	sprintf_s(LastError,sizeof(LastError), "DP_%d: %d\tValid: %d\tBlocked: %d\n",
	adr, ps->diq.dpi, ps->diq.iv, ps->diq.bl);
	//OnErrorMsg(m_errorNo, LastError);n
	#endif
	// call callback //TODO::
	pos+=sizeof(Q_DP104);
	}
	}

	*/

	unsigned int pos;
	unsigned long adr;//对象地址
	if (msg->seq)
	{
		Q_DP104 *ps = (Q_DP104*)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);


		adr =  ps->addr1<<8 | ps->addr0;

#ifdef _DEBUG
		sprintf_s(LastError,sizeof(LastError), "DP_%d: %d\tValid: %d\tBlocked: %d\n",
			adr, ps->diq.dpi, ps->diq.iv, ps->diq.bl);
		//OnErrorMsg(m_errorNo, LastError);n
#endif
		// call callback //TODO:


		pos = sizeof(Q_IEC104_ASDU_header)+2;
		Q_DIQ104	*pi;
		while (pos < msg->len)
		{

			pi = (Q_DIQ104*)&(msg->msgbuf[pos]);
#ifdef _DEBUG
			sprintf_s(LastError,sizeof(LastError), "DP_%d: %d\tValid: %d\tBlocked: %d\n",
				adr, pi->dpi, pi->iv, pi->bl);
			//OnErrorMsg(m_errorNo, LastError);n
#endif

			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeBit;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;

			if (pi->dpi==2)
			{
				dataArray.bValue=true;
			}
			else
			{
				dataArray.bValue=false;
			}

			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			adr++;
			pos++;
		}
	}
	else
	{
		pos = sizeof(Q_IEC104_ASDU_header);
		while (pos < msg->len)
		{
			Q_DP104 *ps = (Q_DP104*)&(msg->msgbuf[pos]);
			adr =  ps->addr1<<8 | ps->addr0;

#ifdef _DEBUG
			sprintf_s(LastError, "DP_%d: %d\tValid: %d\tBlocked: %d\n",
				adr, ps->diq.dpi, ps->diq.iv, ps->diq.bl);
			//OnErrorMsg(m_errorNo, LastError);n
#endif
			// call callback //TODO::
			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeBit;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
			if (ps->diq.dpi==2)
			{
				dataArray.bValue=true;
			}
			else
			{
				dataArray.bValue=false;
			}

			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713
			pos+=sizeof(Q_DP104);
		}
	}
}

void Q_protocolIEC104Master::func_M_ME_NA_NB_1(Q_104ApduEx *msg,const bool &scaled)
{

	UINT pos;
	DWORD adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);

	if (msg->seq) //有序
	{
		//个数是几个

		adr = msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos];
		//adr = msg->msgbuf[pos+2] | msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos]<<16;

		pos += 2;
		while (pos < msg->len)
		{
			Q_MS104Seq *pf=(Q_MS104Seq *)&(msg->msgbuf[pos]);
			short val=0;

			//默认低字节在前
			if(m_cfgBase->m_eIntSeq==eFloatSeqLittle)
			{
				val=(pf->val[1]<<8) | pf->val[0];
			}
			else
			{
				val=(pf->val[0]<<8) | pf->val[1];
			}

#ifdef _DEBUG
			sprintf_s(LastError,
				"SFP_%d: %3.1f\tOFlow: %d\tValid: %d\tBlocked: %d\n",
				adr, val, pf->qds.ov, pf->qds.iv, pf->qds.bl);
			//OnErrorMsg(m_errorNo, LastError);n
#endif
			// call callback //TODO::

			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeFloat;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
			dataArray.fValue = val;

			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			adr++;
			pos+=sizeof(Q_MS104Seq);
		}
	}
	else
	{
		//非有序
		while (pos < msg->len)
		{
			Q_MS104 *pf = (Q_MS104*)&(msg->msgbuf[pos]);

			adr = pf->addr1<<8 | pf->addr0;

			short val=0;
			if(m_cfgBase->m_eIntSeq==eFloatSeqLittle)
			{
				val=(pf->seqv.val[1]<<8) | pf->seqv.val[0];
			}
			else
			{
				val=(pf->seqv.val[0]<<8) | pf->seqv.val[1];
			}
#ifdef _DEBUG
			sprintf_s(LastError,
				"SFP_%d: %3.1f\tOFlow: %d\tValid: %d\tBlocked: %d\n",
				adr, val, pf->seqv.qds.ov, pf->seqv.qds.iv, pf->seqv.qds.bl);
			//OnErrorMsg(m_errorNo, LastError);n
#endif
			// call callback //TODO:

			//添加内容;
			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeFloat;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
			dataArray.fValue = val;

			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713
			pos+=sizeof(Q_MS104);
		}
	}

}

void Q_protocolIEC104Master::func_M_ME_NC_1(Q_104ApduEx *msg)
{ 
	//printf("--------Q_protocolIEC104Master::func_M_ME_NC_1-----111-----\n");
	int suc_count=0;
	UINT pos;
	DWORD adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq) //顺序的
	{
		//printf("--------Q_protocolIEC104Master::func_M_ME_NC_1-----222-----\n");
		//adr = msg->msgbuf[pos+2]<<16 | msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos];
		//信息元素地址为2个字节
		adr = msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos];
		pos+=2;
		while (pos < msg->len)
		{
			Q_MF104Seq *pf=(Q_MF104Seq *)&(msg->msgbuf[pos]);

			BYTE tmp[4];
			float val =0;
			if(m_cfgBase->m_eFloatSeq==eFloatSeqBig)
			{

				tmp[0]=pf->val[3];//从高到低
				tmp[1]=pf->val[2];
				tmp[2]=pf->val[1];
				tmp[3]=pf->val[0];
			}
			else
			{
				tmp[0]=pf->val[0];
				tmp[1]=pf->val[1];
				tmp[2]=pf->val[2];
				tmp[3]=pf->val[3];
			}

			memcpy(&val,tmp,sizeof(val));


			//添加内容;
			//
			//fs20150703
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeFloat;

			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
			dataArray.fValue=val;

			//printf("--------Q_protocolIEC104Master::func_M_ME_NC_1-----333 %d,%d,%d,%f-----\n",
			//	dataArray.channelID,dataArray.device,dataArray.no,dataArray.fValue);
			//系数统一在recordDataValue方法处理了 comment by tjs 2016-06-21
			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			else
			{
				suc_count++;
			}

			adr++;
			pos+=sizeof(Q_MF104Seq);
		}

	}
	else
	{
		//printf("--------Q_protocolIEC104Master::func_M_ME_NC_1-----222-2-----\n");
		while (pos < msg->len)
		{
			Q_MF104 *pf = (Q_MF104*)&(msg->msgbuf[pos]);


			//adr = pf->addr2<<16 | pf->addr1<<8 | pf->addr0;
			adr =  pf->addr1<<8 | pf->addr0;

			BYTE tmp[4];
			float val =0;
			if(m_cfgBase->m_eFloatSeq==eFloatSeqBig)
			{

				tmp[0]=pf->seqv.val[3];
				tmp[1]=pf->seqv.val[2];
				tmp[2]=pf->seqv.val[1];
				tmp[3]=pf->seqv.val[0];
			}
			else
			{
				tmp[0]=pf->seqv.val[0];
				tmp[1]=pf->seqv.val[1];
				tmp[2]=pf->seqv.val[2];
				tmp[3]=pf->seqv.val[3];
			}

			memcpy(&val,tmp,sizeof(val));



			//添加内容;
			//fs20150703
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeFloat;

			int nchal = m_ProSetting->getChannelID();

			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
			dataArray.fValue=val;

			//	printf("--------Q_protocolIEC104Master::func_M_ME_NC_1----- chid=%d,device=%d,no=%d,val=%f-----\n",
			//dataArray.channelID,dataArray.device,dataArray.no,dataArray.fValue);

			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			else
			{
				suc_count++;
			}

			pos+=sizeof(Q_MF104);
		}

	}
#ifdef _DEBUG
	sprintf_s(LastError,"程序退出类型标识为0x13数据解析,此帧共有%d个遥测数据，成功入库个数%d\n",msg->num,suc_count);
#endif
	if (msg->num==suc_count)
	{
#ifdef _DEBUG
		//OnShowMsg(LastError,sizeof(LastError));
		//新规约打印
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
		if(m_bNomalInfoMsg)
		{
			//OnNomalInfoMsg(LastError,sizeof(LastError));
			//新规约打印
			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
		}
	}
	else
	{
#ifdef _DEBUG

		//新规约打印
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);

#endif
	}
}

void Q_protocolIEC104Master::func_M_ME_ND_1(Q_104ApduEx *msg)
{
	UINT pos;
	DWORD adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);

	if (msg->seq) //有序
	{
		adr = msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos];
		pos += 2;
		while (pos < msg->len)
		{
			Q_MS104Seq_21 *pf=(Q_MS104Seq_21 *)&(msg->msgbuf[pos]);
			short val=(pf->val[1]<<8) | pf->val[0];


			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeFloat;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
			dataArray.fValue= val;

			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			adr++;
			pos+=sizeof(Q_MS104Seq_21);
		}
	}
	else
	{
		//非有序
		while (pos < msg->len)
		{
			Q_MS104_21 *pf = (Q_MS104_21*)&(msg->msgbuf[pos]);
			adr =  pf->addr1<<8 | pf->addr0;
			short val=(pf->seqv.val[1]<<8) | pf->seqv.val[0];
			//添加内容;
			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeFloat;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
			dataArray.fValue= val;

			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			pos+=sizeof(Q_MS104_21);
		}
	}
}

void Q_protocolIEC104Master::func_M_IT_NA_1(Q_104ApduEx *msg)
{
	UINT pos;
	DWORD adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq) //顺序的
	{

		adr = msg->msgbuf[pos+2]<<16 | msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos];


		pos+=3;
		while (pos < msg->len)
		{
			Q_MF104Seq *pf=(Q_MF104Seq *)&(msg->msgbuf[pos]);
			int *val = (int *)&pf->val[0]; //好像得改一下

#ifdef _DEBUG
			sprintf_s(LastError,sizeof(LastError),
				"SFP_%d: %3.1f\tOFlow: %d\tValid: %d\tBlocked: %d\n",
				adr, *val, pf->qds.ov, pf->qds.iv, pf->qds.bl);
			//OnErrorMsg(m_errorNo, LastError);nn
#endif
			// call callback //TODO:

			adr++;
			pos+=sizeof(Q_MF104Seq);
		}

	}
	else
	{
		while (pos < msg->len)
		{
			Q_MF104 *pf = (Q_MF104 *)&(msg->msgbuf[pos]);
			adr =pf->addr1<<8 | pf->addr0;





			int *val = (int *)&pf->seqv.val[0];
#ifdef _DEBUG
			sprintf_s(LastError,sizeof(LastError),
				"SFP_%d: %3.1f\tOFlow: %d\tValid: %d\tBlocked: %d\n",
				adr, *val, pf->seqv.qds.ov, pf->seqv.qds.iv, pf->seqv.qds.bl);
			//OnErrorMsg(m_errorNo, LastError);nn
#endif
			// call callback  //TODO:


			pos+=sizeof(Q_MF104);
		}
	}
}

void Q_protocolIEC104Master::func_M_ST_NA_1(Q_104ApduEx *msg)
{
	UINT pos;
	DWORD adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq) //有序
	{
		//个数是几个
		adr = msg->msgbuf[pos+2]<<16 | msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos];

		pos += 3;
		while (pos < msg->len)
		{
			Q_M_ST104Seq *pf=(Q_M_ST104Seq *)&(msg->msgbuf[pos]);
			int val=0;
			if (pf->val&0x40)
			{
				//大于64
				val=pf->val-128;

			}
			else
			{
				val=(int)pf->val;
			}
#ifdef _DEBUG
			sprintf_s(LastError,sizeof(LastError),
				"SFP_%d: %3.1f\tOFlow: %d\tValid: %d\tBlocked: %d\n",
				adr, val, pf->qds.ov, pf->qds.iv, pf->qds.bl);
			//OnErrorMsg(m_errorNo, LastError);nn
#endif
			// call callback://TODO:

			adr++;
			pos+=sizeof(Q_M_ST104Seq);
		}
	}
	else
	{
		//非有序
		while (pos < msg->len)
		{
			Q_M_ST104 *pf = (Q_M_ST104 *)&(msg->msgbuf[pos]);


			adr = pf->addr2<<16 | pf->addr1<<8 | pf->addr0;

			int val;
			if (pf->mst.val&0x40)
			{
				//大于64
				val=pf->mst.val-128;

			}
			else
			{
				val=(int)pf->mst.val;
			}

#ifdef _DEBUG
			sprintf_s(LastError,sizeof(LastError),
				"SFP_%d: %3.1f\tOFlow: %d\tValid: %d\tBlocked: %d\n",
				adr,val, pf->mst.qds.ov, pf->mst.qds.iv, pf->mst.qds.bl);
			//OnErrorMsg(m_errorNo, LastError);nn
#endif
			// call callback //TODO::
			pos+=sizeof(Q_M_ST104);
		}
	}
}

void Q_protocolIEC104Master::func_M_BO_NA_1(Q_104ApduEx *msg)
{
	// actual time
	unsigned int pos;
	unsigned long adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq) //顺序的
	{
		adr = msg->msgbuf[pos+2]<<16 | msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos];

		pos+=3;
		while (pos < msg->len)
		{
			Q_BO104Seq *pf=(Q_BO104Seq *)&(msg->msgbuf[pos]);
#ifdef _DEBUG
			sprintf_s(LastError,sizeof(LastError),
				"M_BO_%d: %d\t %d\t %d\t %d\t OFlow: %d\tValid: %d\tBlocked: %d\n",
				adr, pf->val[0],pf->val[1],pf->val[2],pf->val[3], pf->qds.ov, pf->qds.iv, pf->qds.bl);
			//OnErrorMsg(m_errorNo, LastError);nn
#endif
			// call callback //TODO:

			adr++;
			pos+=sizeof(Q_BO104Seq);
		}

	}
	else
	{
		while (pos < msg->len)
		{
			Q_BO104 *pf = (Q_BO104 *)&(msg->msgbuf[pos]);

			adr =  pf->addr1<<8 | pf->addr0;


#ifdef _DEBUG
			sprintf_s(LastError,sizeof(LastError),
				"M_BO_%d: %d\t %d\t %d\t %d\t OFlow: %d\tValid: %d\tBlocked: %d\n",
				adr, pf->seqv.val[0], pf->seqv.val[1], pf->seqv.val[2], pf->seqv.val[3],pf->seqv.qds.ov, pf->seqv.qds.iv, pf->seqv.qds.bl);
			printf(LastError);
#endif
			// call callback //TODO:


			pos+=sizeof(Q_BO104);
		}
	}
}

int Q_protocolIEC104Master::handleCommmd_SC(Q_cmdIEC104 *v_cmd)
{

	TRACE("--------------------Q_protocolIEC104Master::handleCommmd_SC ------------------------\n m_cfgControl->m_YtExecMode = %d",
		m_cfgControl->m_YkExecMode);

	int bRet=Q_104SUC;
	BYTE FuncCode;

	v_cmd->m_bExec=true;
	Q_cmdIEC104_SP *cmd=NULL;
	if(m_cfgControl->m_YkExecMode==eYtExecModeSelect)
	{
		int icmd = v_cmd->m_eState;
		switch (icmd)
		{
		case eCmdStateYkSelect:
			{
				cmd = (Q_cmdIEC104_SP *)v_cmd;
				if(cmd!=NULL)
				{
					Q_IEC104_ST_C_SC st;
					memset(&st,0,sizeof(Q_IEC104_ST_C_SC));
					st.addr0=cmd->m_nInfoAddr&0xFF;
					st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
					st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;

					st.sco.scs=cmd->m_cOnOff>=1?1:0;
					st.sco.se=1;//选择

					switch(m_cfgControl->m_YKAsduType)
					{
					case eYKAsdu2D:
						FuncCode=0x2D;
						break;
					case eYKAsdu3A:
						FuncCode=0x3A;
						break;
					default:
						FuncCode=0x2D;
						break;
					}

					/*by fanshuo*/
					bool bExist = false;
					int	YKID32 = -1;
					int YKtoYXID32 = -1;
					SCADA_Relay curRelay;
					bExist = FindYK(YKtoYXID32,YKID32,curRelay,cmd);

					if (bExist)//存在
					{
						if ((YKID32>0) && (&curRelay!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							curRelay.exectime = time;
							GetLocalTime(time);
							curRelay.selecttime = time;
							curRelay.iexecret = 0;
							dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行时间
						}
					}
					/*by fanshuo end*/
					else
					{
#ifdef _DEBUG
						SYSTEMTIME st;
						GetLocalTime(st);
						sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 处理ChannelID=%d，device=%d,no=%d的遥控选择时，在遥控命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
							m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

						/*//原CommApp规约打印保存、先保留着
						//OnShowMsg(LastError,sizeof(LastError));
						if(true)
						{
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						}
						*/
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}

					if (FuncCode == 0x3A)
					{
						Q_IEC104_ST_C_SC_TA1 st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SC_TA1));
						SYSTEMTIME sttime;
						GetLocalTime(sttime);					
						unsigned int m;
						m = sttime.wMilliseconds + 1000 * sttime.wSecond;
						st.CP56Time[0] = 0;/*m & 0xFF;*/
						st.CP56Time[1] = 0;/*(m & 0xFF00)>>8;*/
						st.CP56Time[2] = sttime.wMinute & 0x00FF;	// add valid flag and genuine flag
						st.CP56Time[3] = sttime.wHour & 0x00FF;	// add summer flag
						st.CP56Time[4] = (BYTE)sttime.wDay; /*((sttime.wDayOfWeek%7)&0x03)<<5 | (sttime.wDay&0x1F);*/
						st.CP56Time[5] = sttime.wMonth & 0x0F;
						st.CP56Time[6] = sttime.wYear - 2000;
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;
						st.sco.scs=cmd->m_cOnOff>=1?1:0;
						st.sco.se=1;//选择
						sendYkCommand1(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}
					else
					{
						Q_IEC104_ST_C_SC st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SC));
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;

						st.sco.scs=cmd->m_cOnOff>=1?1:0;
						st.sco.se=1;//选择

						sendYkCommand(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}



					cmd->m_cQuality=0;
					v_cmd->m_eState=eCmdStateYkSelectCon;
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						if (FuncCode == 0x3A)
						{
							sprintf_s(LastError,sizeof(LastError),"****  带时标单点遥控，模式：选择，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnShowMsg(LastError,sizeof(LastError));
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						} 
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值，模式：选择，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnShowMsg(LastError,sizeof(LastError));
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						}
					}
					if(true/*m_bInfoMsg*/)
					{
						memset(LastError,0,sizeof(LastError));
						if (FuncCode == 0x3A)
						{
							sprintf_s(LastError,sizeof(LastError),"****  带时标单点遥控，模式：选择，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						} 
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值，模式：选择，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						}
					}
				}
			}
			break;
		case eCmdStateYkConfirm:
			{
				cmd = (Q_cmdIEC104_SP *)v_cmd;
				if(cmd!=NULL)
				{
					Q_IEC104_ST_C_SP st;
					memset(&st,0,sizeof(Q_IEC104_ST_C_SP));
					st.addr0=cmd->m_nInfoAddr&0xFF;
					st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
					st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;
					st.sco.scs=cmd->m_cOnOff>=1?1:0;
					st.sco.se=0;//执行

					switch(m_cfgControl->m_YKAsduType)
					{
					case eYKAsdu2D:
						FuncCode=0x2D;
						break;
					case eYKAsdu3A:
						FuncCode=0x3A;
						break;
					default:
						FuncCode=0x2D;
						break;
					}


					/*by fanshuo*/
					bool bExist = false;
					int	YKID32 = -1;
					int YKtoYXID32 = -1;
					SCADA_Relay curRelay;
					bExist = FindYK(YKtoYXID32,YKID32,curRelay,cmd);

					if (bExist)//存在
					{
						if ((YKID32>0) && (&curRelay!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};

							GetLocalTime(time);
							curRelay.exectime = time;
							curRelay.iexecret = 6;

							if (cmd->m_cOnOff == 1)
							{
								curRelay.LogicalClose = 1;
								curRelay.LogicalOpen = 0;
							}
							else
							{
								curRelay.LogicalClose = 0;
								curRelay.LogicalOpen = 1;
							}

							dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行时间


							if (YKtoYXID32>0)
							{
								Q_RECORDDATATAG dataArray;
								SCADA_Digit tempDigit;
								dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID,YKtoYXID32,&tempDigit);
								if (&tempDigit != NULL)
								{
									//dataArray.channelID=tempDigit.uTuID;
									//dataArray.device=tempDigit.lDPUID;
									//dataArray.no=tempDigit.iNoInDPU;
									//2016-02-23修改为直接使用索引更新
									dataArray.eDataType=eDataTypeBit;
									dataArray.channelID=tempDigit.uTuID;
									dataArray.nIndex=tempDigit.iIndex;
									dataArray.bValue=cmd->m_cOnOff==1?true:false;
									m_pMgr->recordDataValue(&dataArray,1,true);
								}
							}
						}
					}
					/*by fanshuo end*/
					else
					{
#ifdef _DEBUG
						SYSTEMTIME st;
						GetLocalTime(st);
						sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 处理ChannelID=%d，device=%d,no=%d的遥控确认执行时，但在遥控命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
							m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

						/*//原CommApp规约打印保存、先保留着
						//OnShowMsg(LastError,sizeof(LastError));
						if(true)
						{
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						}
						*/

						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if (FuncCode == 0x3A)
					{
						Q_IEC104_ST_C_SC_TA1 st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SC_TA1));
						SYSTEMTIME sttime;
						GetLocalTime(sttime);
						unsigned int m;
						m = sttime.wMilliseconds + 1000 * sttime.wSecond;
						st.CP56Time[0] = 0;/*m & 0xFF;*/
						st.CP56Time[1] = 0;/*(m & 0xFF00)>>8;*/
						st.CP56Time[2] = sttime.wMinute & 0x00FF;	// add valid flag and genuine flag
						st.CP56Time[3] = sttime.wHour & 0x00FF;	// add summer flag
						st.CP56Time[4] = (BYTE)sttime.wDay; /*((sttime.wDayOfWeek%7)&0x03)<<5 | (sttime.wDay&0x1F);*/
						st.CP56Time[5] = sttime.wMonth & 0x0F;
						st.CP56Time[6] = sttime.wYear - 2000;
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;
						st.sco.scs=cmd->m_cOnOff>=1?1:0;
						st.sco.se=0;//执行
						sendYkCommand1(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}
					else
					{
						Q_IEC104_ST_C_SC st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SC));
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;

						st.sco.scs=cmd->m_cOnOff>=1?1:0;
						st.sco.se=0;//执行

						sendYkCommand(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}
					cmd->m_cQuality=0;

					v_cmd->m_eState=eCmdStateYkExec;
					v_cmd->m_nCurTryNum=0;

					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						if (FuncCode == 0x3A)
						{
							sprintf_s(LastError,sizeof(LastError),"****  带时标单点遥控，模式：执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnShowMsg(LastError,sizeof(LastError));
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						} 
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值，模式：执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnShowMsg(LastError,sizeof(LastError));
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						}
					}
					if(true/*m_bInfoMsg*/)
					{
						memset(LastError,0,sizeof(LastError));
						if (FuncCode == 0x3A)
						{
							sprintf_s(LastError,sizeof(LastError),"****  带时标单点遥控，模式：执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						} 
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值，模式：执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						}
					}
				}
			}
			break;
		case eCmdStateYkSelectCon:
		case eCmdStateYkExec:
			v_cmd->m_nCurTryNum++;
			if(v_cmd->m_nCurTryNum>=v_cmd->m_nMaxTryCount)
			{
				v_cmd->m_nCurTryNum=0;
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
					cmd = (Q_cmdIEC104_SP *)v_cmd;
					memset(LastError,0,sizeof(LastError));
					if(cmd!=NULL)
					{
						sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete single point command.Command info:device=%d addr=%d,value=%d\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
					}
					else
					{
						sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete single point command.\n");
					}
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if(true/*m_bInfoMsg*/)
				{
					cmd = (Q_cmdIEC104_SP *)v_cmd;
					memset(LastError,0,sizeof(LastError));
					if(cmd!=NULL)
					{
						sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete single point command.Command info:device=%d addr=%d,value=%d\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
					}
					else
					{
						sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete single point command.\n");
					}
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}

				//fs20150811
				cmd = (Q_cmdIEC104_SP *)v_cmd;
				Q_RECORDDATATAG ykRec;
				ykRec.eDataType=eDataTypeFloat;
				ykRec.channelID=m_ProSetting->getChannelID();
				ykRec.device=cmd->m_nComAsdu;
				ykRec.no=cmd->m_nInfoAddr;
				ykRec.fValue=float(cmd->m_cOnOff>=1?1:0);//赋初值

				if(!m_pMgr->getYtValue(&ykRec))
				{
					sprintf_s(LastError,sizeof(LastError),"Send YK command faild,did not find config in yk.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					/*//原CommApp规约打印保存、先保留着
					//OnShowMsg(LastError,sizeof(LastError));
					if (m_bInfoMsg)
					{
					sprintf_s(LastError,sizeof(LastError),"Send YK command faild,did not find config in yk.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					//	OnInfoMsg(LastError,strlen(LastError)+1);
					}
					*/
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);

					return false;
				}

				if(m_pMgr->handleMasterYk(&ykRec)!=0)
				{
					/*//原CommApp规约打印保存、先保留着
					sprintf_s(LastError,sizeof(LastError),"Send YK command faild,did not find config.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					//OnShowMsg(LastError,sizeof(LastError));
					if (m_bInfoMsg)
					{
					sprintf_s(LastError,sizeof(LastError),"Send YK command faild,did not find config.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					//	OnInfoMsg(LastError,strlen(LastError)+1);
					}
					*/

					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}


				/*				Q_dataManager* p = _GlobalData.getDataManager();
				LPQ_YKEDATATAG pYKData=p->getYKData();
				int nCount = p->getYKCount();
				int nChanID = m_ProSetting->getChannelID();
				for (int i=0;i<nCount;i++)
				{
				if (nChanID==pYKData[i].channelid && v_cmd->m_nComAsdu==pYKData[i].device && v_cmd->m_nInfoAddr ==pYKData[i].no)
				{
				pYKData[i].execresult = 2;
				break;
				}
				}
				*/

				/*by fanshuo*/
				bool bExist = false;
				int	YKID32 = -1;
				int YKtoYXID32 = -1;
				SCADA_Relay curRelay;
				bExist = FindYK(YKtoYXID32,YKID32,curRelay,cmd);

				if (bExist)//存在
				{
					if ((YKID32>0) && (&curRelay!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						curRelay.iexecret = 2;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行时间
					}
				}
				/*by fanshuo end*/

				m_listCmd->removeCommand(v_cmd);
			}
			//超时无返回删除命令
			break;
		}
	}
	else if(m_cfgControl->m_YkExecMode==eYtExecModeDirect)
	{//直接执行
		v_cmd->m_nCurTryNum++;
		if(v_cmd->m_nCurTryNum>=v_cmd->m_nMaxTryCount)
		{
			v_cmd->m_nCurTryNum=0;

			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
				cmd = (Q_cmdIEC104_SP *)v_cmd;
				memset(LastError,0,sizeof(LastError));
				if(cmd!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete  single point command.Command info:addr=%d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
				}
				else
				{
					sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete single point command.\n");
				}
				//OnShowMsg(LastError,sizeof(LastError));

				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
			}
			if(true/*m_bInfoMsg*/)
			{
				cmd = (Q_cmdIEC104_SP *)v_cmd;
				memset(LastError,0,sizeof(LastError));
				if(cmd!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete  single point command.Command info:addr=%d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
				}
				else
				{
					sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete single point command.\n");
				}
				//OnInfoMsg(LastError,sizeof(LastError),cmd);

				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
			}

			/*			Q_dataManager* p = _GlobalData.getDataManager();
			//			LPQ_YKEDATATAG pYKData = p->getYKData();
			for (int i=0;i<p->getYKCount();i++)
			{
			if (m_ProSetting->getChannelID()==pYKData[i].channelid && pYKData[i].device==cmd->m_nComAsdu && pYKData[i].no==cmd->m_nInfoAddr)
			{
			pYKData[i].execresult = 2;
			break;
			}
			}
			*/
			/*by fanshuo*/
			bool bExist = false;
			int	YKID32 = -1;
			int YKtoYXID32 = -1;
			SCADA_Relay curRelay;
			bExist = FindYK(YKtoYXID32,YKID32,curRelay,cmd);

			if (bExist)//存在
			{
				if ((YKID32>0) && (&curRelay!=NULL))
				{
					CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
					curRelay.iexecret = 2;
					dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行时间
				}
			}
			/*by fanshuo end*/

			m_listCmd->removeCommand(v_cmd);
		}

		int icmd = v_cmd->m_eState;
		switch (icmd)
		{
		case eCmdStateYkSelect:
			{
				cmd = (Q_cmdIEC104_SP *)v_cmd;
				if(cmd!=NULL)
				{
					Q_IEC104_ST_C_SC st;
					memset(&st,0,sizeof(Q_IEC104_ST_C_SC));
					st.addr0=cmd->m_nInfoAddr&0xFF;
					st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
					st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;
					st.sco.scs=cmd->m_cOnOff>=1?1:0;
					st.sco.se=0;
					switch(m_cfgControl->m_YKAsduType)
					{
					case eYKAsdu2D:
						FuncCode=0x2D;
						break;
					case eYKAsdu3A:
						FuncCode=0x3A;
						break;
					default:
						FuncCode=0x2D;
						break;
					}


					/*by fanshuo*/
					bool bExist = false;
					int	YKID32 = -1;
					int YKtoYXID32 = -1;
					SCADA_Relay curRelay;
					bExist = FindYK(YKtoYXID32,YKID32,curRelay,cmd);

					if (bExist)//存在
					{
						if ((YKID32>0) && (&curRelay!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							GetLocalTime(time);
							curRelay.exectime = time;

							if (cmd->m_cOnOff == 1)
							{
								curRelay.LogicalClose = 1;
								curRelay.LogicalOpen = 0;
							}
							else
							{
								curRelay.LogicalClose = 0;
								curRelay.LogicalOpen = 1;
							}

							dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//写入遥控执行时间-直接执行模式

							if (YKtoYXID32>0)
							{
								Q_RECORDDATATAG dataArray;
								SCADA_Digit tempDigit;
								dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID,YKtoYXID32,&tempDigit);
								if (&tempDigit != NULL)
								{
									//2016-02-23修改为直接使用索引更新
									dataArray.eDataType=eDataTypeBit;
									dataArray.channelID=tempDigit.uTuID;
									dataArray.nIndex=tempDigit.iIndex;	
									dataArray.bValue=cmd->m_cOnOff==1?true:false;
									m_pMgr->recordDataValue(&dataArray,1,true);
								}
							}
						}
					}
					/*by fanshuo end*/
					else
					{
#ifdef _DEBUG
						SYSTEMTIME st;
						GetLocalTime(st);
						sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 处理ChannelID=%d，device=%d,no=%d的遥控确认执行时，但在遥控命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
							m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

						/*
						OnShowMsg(LastError,sizeof(LastError));
						if(true)
						{
						OnInfoMsg(LastError,sizeof(LastError),cmd);
						}
						*/
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if (FuncCode == 0x3A)
					{
						Q_IEC104_ST_C_SC_TA1 st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SC_TA1));
						SYSTEMTIME sttime;
						GetLocalTime(sttime);					
						unsigned int m;
						m = sttime.wMilliseconds + 1000 * sttime.wSecond;
						st.CP56Time[0] = 0;/*m & 0xFF;*/
						st.CP56Time[1] = 0;/*(m & 0xFF00)>>8;*/
						st.CP56Time[2] = sttime.wMinute & 0x00FF;	// add valid flag and genuine flag
						st.CP56Time[3] = sttime.wHour & 0x00FF;	// add summer flag
						st.CP56Time[4] = (BYTE)sttime.wDay; /*((sttime.wDayOfWeek%7)&0x03)<<5 | (sttime.wDay&0x1F);*/
						st.CP56Time[5] = sttime.wMonth & 0x0F;
						st.CP56Time[6] = sttime.wYear - 2000;
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;
						st.sco.scs=cmd->m_cOnOff>=1?1:0;
						st.sco.se=0;//执行
						sendYkCommand1(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}
					else
					{
						Q_IEC104_ST_C_SC st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SC));
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;

						st.sco.scs=cmd->m_cOnOff>=1?1:0;
						st.sco.se=0;//执行

						sendYkCommand(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}

					cmd->m_cQuality=0;
					v_cmd->m_eState=eCmdStateYkExec;

					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						if (FuncCode == 0x3A)
						{
							sprintf_s(LastError,sizeof(LastError),"****  带时标单点遥控，模式：直接执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnShowMsg(LastError,sizeof(LastError));
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						} 
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值，模式：直接执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnShowMsg(LastError,sizeof(LastError));
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						}
					}
					if(true/*m_bInfoMsg*/)
					{
						memset(LastError,0,sizeof(LastError));
						if (FuncCode == 0x3A)
						{
							sprintf_s(LastError,sizeof(LastError),"****  带时标单点遥控，模式：直接执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						} 
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值，模式：直接执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						}
					}
				}
			}
			break;
		}

	}

	return bRet;
}


int Q_protocolIEC104Master::handleCommmd_DC(Q_cmdIEC104 *v_cmd)
{
	int bRet=Q_104SUC;

	v_cmd->m_bExec=true;//标析

	if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		int icmd = v_cmd->m_eState;
		switch (icmd)
		{
		case eCmdStateYkSelect:
			{
				//发送确认帧
				Q_cmdIEC104_DP *cmd =(Q_cmdIEC104_DP *)v_cmd;
				if (NULL != cmd)
				{
					/*
					if (sendYkResponse(cmd->m_nComAsdu,CMD_ACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2E,sizeof(cmd->m_nValue))==Q_104FAIL)
					{
					//发送报文出错
					m_errorNo=e104Errno_CmdSend;
					v_cmd->m_eState=eCmdStateYkConfirm;
					v_cmd->m_bError=true;
					sprintf_s(LastError,sizeof(LastError), "发送双点遥控选择确认信息出错，当前行号:%d\n",__LINE__);
					//OnErrorMsg(m_errorNo, LastError);nn
					bRet=false;
					}
					else
					{
					v_cmd->m_eState=eCmdStateYkConfirm;
					}
					*/
				}

			}
			break;
		case eCmdStateYkExec:
			{
				//执行
				Q_cmdIEC104_DP *cmd = (Q_cmdIEC104_DP *)v_cmd;
				Q_RECORDDATATAG dataArray;

				dataArray.eDataType=eDataTypeInt;
				dataArray.nValue=(int)cmd->m_cOnOff;
				dataArray.nIndex=v_cmd->m_nPointNo;
				Q_dataManager* p = _GlobalData.getDataManager();

				//更新实时库
				if (bRet=p->recordDataValue(&dataArray,1))
				{
					//更新成功
					v_cmd->m_eState=eCmdStateYkConfirm;

					//执行完成 //TODO:
					//查询看是否返回执行后的值 如果要求返回则返回

					v_cmd->m_eState=eCmdStateEnd;

				}
				else
				{
					v_cmd->m_bError=true;
					v_cmd->m_eState=eCmdStateYkConfirm;
					m_errorNo=e104Errno_UptDb;
#ifdef _DEBUG
					sprintf_s(LastError,sizeof(LastError), "执行双点遥控，更新实时库出错，当前行号:%d\n",__LINE__);
					//OnErrorMsg(m_errorNo, LastError);nn
#endif
					bRet=false;

				}

			}
			break;
		case eCmdStateYkCancelAct:
			{
				//取消命令
				//发送确认帧
				Q_cmdIEC104_DP *cmd = (Q_cmdIEC104_DP *)v_cmd;
				if (NULL != cmd)
				{
					v_cmd->m_bError=true;
					/*
					int  nRet=sendYkResponse(cmd->m_nComAsdu,CMD_DEACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2E,sizeof(cmd->m_nValue));
					if (nRet==Q_104FAIL)
					{
					m_errorNo=e104Errno_CmdSend;
					#ifdef _DEBUG
					sprintf_s(LastError,sizeof(LastError), "发送双点遥控取消确认出错，当前行号:%d\n",__LINE__);
					//OnErrorMsg(m_errorNo, LastError);nn
					bRet=false;
					#endif
					}
					else
					{
					v_cmd->m_eState=eCmdStateYkCancelAct;
					}
					*/
					v_cmd->m_eState=eCmdStateEnd;
				}
			}
			break;
		}

	}
	else if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		int icmd = v_cmd->m_eState;
		switch (icmd)
		{
		case eCmdStateIntEnd:
			{
				//Interrogation Command (Group %d): Activation Termination.
			}
			break;
		}

	}
	return bRet;
}

int Q_protocolIEC104Master::handleCommmd_IC(Q_cmdIEC104 *v_cmd)
{
	int ret=Q_104SUC;

	int icmd = v_cmd->m_eState;    
	/*TRACE("--------------------Q_protocolIEC104Master::handleCommmd_IC icmd = %d------------------------\n",icmd);*/
	switch (icmd)
	{
	case eCmdStateIntRequest:
		{
			/*TRACE("--------------------Q_protocolIEC104Master::handleCommmd_IC bStartFlg=%d  v_cmd->m_bExec=%d------------------------\n",bStartFlg,v_cmd->m_bExec);*/
			if(bStartFlg==true&&v_cmd->m_bExec==false)
			{
				//开始链路了之后方可方送总召报文
				send_IC_Response(m_cfgBase->m_nAsduAddr,CMD_ACT);
				v_cmd->m_bExec=true;
				v_cmd->setTestNumDefault();
				time(&timer_ICTimeflag);
			}
			else
			{
				v_cmd->addTestNum();
				if(v_cmd->isBeyondTestNum())
				{
					v_cmd->m_eState=eCmdStateIntRequest;
					v_cmd->m_nInfoAddr=0;
					v_cmd->m_bExec=false;
					v_cmd->setTestNumDefault();
				//	closeLink();
				}
			}
		}
		break;
	case eCmdStateIntRequestAct:
		{
			//Interrogation Command (Group %d): Activation Confirmation.
			//可以采集数据了
			v_cmd->addTestNum();
			if(v_cmd->isBeyondTestNum() || time_to_IC())
			{
				v_cmd->m_eState=eCmdStateIntRequest;
				v_cmd->m_nInfoAddr=0;
				v_cmd->m_bExec=false;
				v_cmd->setTestNumDefault();
			//	closeLink();
			}

		}
		break;
	case eCmdStateIntEnd:
		{
			//v_cmd->m_nInfoAddr++;
			//等待几秒中再发总召
			//if(v_cmd->m_nInfoAddr>10)
			if(time_to_IC())
			{
				v_cmd->m_eState=eCmdStateIntRequest;
				//v_cmd->m_nInfoAddr=0;
				v_cmd->m_bExec=false;
			}
			v_cmd->setTestNumDefault();
			//Interrogation Command (Group %d): Activation Termination.
			//需要等待一段时间再召换
			
		}
		break;
	}
	return Q_104SUC;
}

int Q_protocolIEC104Master::handleCommmd_CS(Q_cmdIEC104 *v_cmd)
{
	int ret=Q_104SUC;

	v_cmd->m_bExec=true;

	if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		int icmd = v_cmd->m_eState;
		switch (icmd)
		{
		case eCmdStateSync:
			{
				//静态补偿时间，更新系统时间 TODO:
				Q_cmdIEC104_CS *cmd = (Q_cmdIEC104_CS *)v_cmd;
				v_cmd->m_eState=eCmdStateSyncAct;//请求确认完成
				if (cmd!=NULL)
				{
#ifdef WINDOWS_ENV
					Q_CP56Time2a t2(cmd->m_cTime);
					SetLocalTime(&(t2._GetSystemTime(m_cfgSyncClock->m_nFixUsecs)));//静态补偿几秒钟。
#endif
#ifdef UNIX_ENV
					SYSTEMTIME		stime;

					CP56Time2aToTime(cmd->m_cTime,&stime);

					struct tm * time;
					struct timeval tv;
					time_t timep,timeq;
					int rec = -1;

					time->tm_sec = stime.wSecond;
					time->tm_min = stime.wMinute;
					time->tm_hour = stime.wHour;
					time->tm_mday = stime.wDay;
					time->tm_mon = stime.wMonth;
					time->tm_year = stime.wYear;

					timeq = mktime(time);
					tv.tv_sec = (long)timeq;
					tv.tv_usec = 0;
					rec = settimeofday(&tv,NULL);

#endif
				}

				v_cmd->m_bExec=true;
				v_cmd->m_eState=eCmdStateEnd;

			}
			break;
		}

	}
	else if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		int icmd = v_cmd->m_eState;
		switch (icmd)
		{
		case eCmdStateSyncAct:
			{
				//Interrogation Command (Group %d): Activation Confirmation.
			}
			break;
		}

	}
	return Q_104SUC;
}

int Q_protocolIEC104Master::handleCommmd_SE(Q_cmdIEC104 *v_cmd)
{

	TRACE("--------------------Q_protocolIEC104Master::handleCommmd_SE ------------------------\n m_cfgControl->m_YtExecMode = %d",
		m_cfgControl->m_YtExecMode);

	int bRet=Q_104SUC;
	//根据配置更改 m_cFuncCode=0x30

	v_cmd->m_bExec=true;//标度化
	Q_cmdIEC104_SE *cmd=NULL;

	if(m_cfgControl->m_YtExecMode==eYtExecModeSelect)
	{
		int icmd = v_cmd->m_eState;
		switch (icmd)
		{
		case eCmdStateYkSelect:
			{
				cmd = (Q_cmdIEC104_SE *)v_cmd;			
				//IEC104主站遥调系数处理
				float fFactor = m_pMgr->getYtcoefficient(m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);
				cmd->m_fVal = cmd->m_fVal*fFactor;
				cmd->m_cOnOff = short(cmd->m_cOnOff*fFactor);

				if(cmd!=NULL)
				{
					switch(m_cfgControl->m_YsetAsduType)
					{
					case eYsetAsdu30:
						cmd->m_cFuncCode=0x30;
						break;
					case eYsetAsdu31:
						cmd->m_cFuncCode=0x30;
						break;
					case eYsetAsdu32:
						cmd->m_cFuncCode=0x32;
						break;
					case eYsetAsdu3F:
						cmd->m_cFuncCode=0x3F;
						break;
					default:
						cmd->m_cFuncCode=0x30;
						break;
					}

					/*by fanshuo*/
					bool bExist = false;
					int	YTID32 = -1;
					int YTtoYCID32 = -1;
					SCADA_SetPoint curSetPoint;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

					if (bExist)//存在
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							curSetPoint.exectime = time;

							if (cmd->m_cFuncCode==0x3F||cmd->m_cFuncCode==0x32)
							{//浮点
								curSetPoint.fVal = cmd->m_fVal;

							}
							else
							{
								curSetPoint.fVal = cmd->m_cOnOff;

							}

							GetLocalTime(time);
							curSetPoint.selecttime = time;
							curSetPoint.iexecret = 0;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥遥调选择时间
						}
					}
					/*by fanshuo end*/
					else
					{
#ifdef _DEBUG
						SYSTEMTIME st;
						GetLocalTime(st);
						sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 处理ChannelID=%d，device=%d,no=%d的遥调选择时，在遥控调命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
							m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);

#endif
					}


					if (cmd->m_cFuncCode==0x3F)
					{
						Q_IEC104_ST_C_SE_TC1 st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SE_TC1));
						SYSTEMTIME sttime;
						GetLocalTime(sttime);
						unsigned int m;
						m = sttime.wMilliseconds + 1000 * sttime.wSecond;
						st.CP56Time[0] = 0;/*m & 0xFF;*/
						st.CP56Time[1] = 0;/*(m & 0xFF00)>>8;*/
						st.CP56Time[2] = sttime.wMinute & 0x00FF;	// add valid flag and genuine flag
						st.CP56Time[3] = sttime.wHour & 0x00FF;	// add summer flag
						st.CP56Time[4] = (BYTE)sttime.wDay; /*((sttime.wDayOfWeek%7)&0x03)<<5 | (sttime.wDay&0x1F);*/
						st.CP56Time[5] = sttime.wMonth & 0x0F;
						st.CP56Time[6] = sttime.wYear - 2000;
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;

						memcpy(st.val,&cmd->m_fVal,sizeof(float));

						st.qos.se=1;//选择
						sendYtCommandFloatWithTime(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}
					else if (cmd->m_cFuncCode==0x32)
					{//浮点
						Q_IEC104_ST_C_SE3 st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SE3));
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;
						memcpy(st.val,&cmd->m_fVal,sizeof(float));


						st.qos.se=1;//选择
						sendYtCommand3(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}
					else	
					{
						Q_IEC104_ST_C_SE st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SE));
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;
						if(cmd->m_cOnOff>=0)
						{

							int vv=(int)curSetPoint.fVal;
							if(m_cfgControl->getAsdu48Mode==0)
							{
								st.val1=cmd->m_cOnOff & 0xFF;
								st.val2=(cmd->m_cOnOff>>8) & 0x7F;

								st.s=0;
							}
							else if(m_cfgControl->getAsdu48Mode>0)
							{
								int n=(int)(cmd->m_cOnOff*32767/m_cfgControl->getAsdu48Mode);

								st.val1=n& 0xFF;
								st.val2=(n>>8) & 0x7F;
								st.s=0;
							}
						}
						else
						{
							//负数用补码表示
							int n=~(abs(cmd->m_cOnOff))+1;

							st.val1= n& 0xFF;
							st.val2=(n>>8) &0x7F;
							st.s=1;
						}
						st.qos.se=1;//选择
						sendYtCommand(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}				

					cmd->m_cQuality=0;
					v_cmd->m_eState=eCmdStateYkSelectCon;
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						if (cmd->m_cFuncCode==0x3F)
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定带时标的短浮点值，模式：选择，公共地址=%d，信息体地址=%d，值=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else if (cmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值，模式：选择，公共地址=%d，信息体地址=%d，值=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定归一化值，模式：选择，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						//OnShowMsg(LastError,sizeof(LastError));
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
					}
					if(true/*m_bInfoMsg*/)
					{
						memset(LastError,0,sizeof(LastError));
						if (cmd->m_cFuncCode==0x3F)
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定带时标的短浮点值，模式：选择，公共地址=%d，信息体地址=%d，值=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else if (cmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值，模式：选择，公共地址=%d，信息体地址=%d，值=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定归一化值，模式：选择，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
					}
				}
			}
			break;
		case eCmdStateYkConfirm:
			{
				cmd = (Q_cmdIEC104_SE *)v_cmd;
				//IEC104主站遥调系数处理
				//float fFactor = m_pMgr->getYtcoefficient(m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);
				//cmd->m_fVal = cmd->m_fVal*fFactor;
				//cmd->m_cOnOff = cmd->m_cOnOff*fFactor;

				if(cmd!=NULL)
				{

					/*by fanshuo*/
					bool bExist = false;
					int	YTID32 = -1;
					int YTtoYCID32 = -1;
					SCADA_SetPoint curSetPoint;
					float fdataval = 0;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

					if (bExist)//存在
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							if (cmd->m_cFuncCode==0x3F||cmd->m_cFuncCode==0x32)
							{//浮点
								curSetPoint.fVal = cmd->m_fVal;
								fdataval = curSetPoint.fVal ; 
							}
							else
							{
								curSetPoint.fVal = cmd->m_cOnOff;
								fdataval =curSetPoint.fVal; 
							}

							GetLocalTime(time);
							curSetPoint.exectime = time;
							curSetPoint.iexecret = 6;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调执行时间

							if (YTtoYCID32 > 0)
							{
								SCADA_Analog tempAnalog;
								dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);

								if (&tempAnalog != NULL)
								{
									tempAnalog.fVal = fdataval;
									tempAnalog.tSnapTime = time;
									dbmg_sdb->SetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);
								}
							}
						}
					}
					/*by fanshuo end*/
					else
					{
#ifdef _DEBUG
						SYSTEMTIME st;
						GetLocalTime(st);
						sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 处理ChannelID=%d，device=%d,no=%d的遥调选择执行时，在遥调命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
							m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

						/*
						//OnShowMsg(LastError,sizeof(LastError));
						if(true)
						{
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						}
						*/
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}

					if (cmd->m_cFuncCode==0x3F)
					{
						Q_IEC104_ST_C_SE_TC1 st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SE_TC1));
						SYSTEMTIME sttime;
						GetLocalTime(sttime);
						unsigned int m;
						m = sttime.wMilliseconds + 1000 * sttime.wSecond;
						st.CP56Time[0] = m & 0xFF;
						st.CP56Time[1] = (m & 0xFF00)>>8;
						st.CP56Time[2] = sttime.wMinute & 0x00FF;	// add valid flag and genuine flag
						st.CP56Time[3] = sttime.wHour & 0x00FF;	// add summer flag
						st.CP56Time[4] = ((sttime.wDayOfWeek%7)&0x03)<<5 | (sttime.wDay&0x1F);
						st.CP56Time[5] = sttime.wMonth & 0x0F;
						st.CP56Time[6] = sttime.wYear - 2000;
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;

						memcpy(st.val,&cmd->m_fVal,sizeof(float));


						sendYtCommandFloatWithTime(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}
					else if (cmd->m_cFuncCode==0x32)
					{
						Q_IEC104_ST_C_SE3 st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SE3));
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;
						memcpy(st.val,&cmd->m_fVal,sizeof(float));



						st.qos.se=0;//执行
						sendYtCommand3(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}
					else
					{
						Q_IEC104_ST_C_SE st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SE));
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;
						if(cmd->m_cOnOff>=0)
						{
							int vv=(int)curSetPoint.fVal;
							if(m_cfgControl->getAsdu48Mode==0)
							{
								st.val1=cmd->m_cOnOff & 0xFF;
								st.val2=(cmd->m_cOnOff>>8) & 0x7F;

							}
							else if(m_cfgControl->getAsdu48Mode>0)
							{
								int n=(int)(cmd->m_fVal*32767/m_cfgControl->getAsdu48Mode);

								st.val1=n& 0xFF;
								st.val2=(n>>8) & 0x7F;
							}
						}
						else
						{
							//负数用补码表示
							int n=~(abs(cmd->m_cOnOff))+1;

							st.val1= n& 0xFF;
							st.val2=(n>>8) &0x7F;
							st.s=1;
						}
						st.qos.se=0;//执行
						sendYtCommand(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}
					cmd->m_cQuality=0;
					cmd->m_eState=eCmdStateYkExec;
					cmd->m_nCurTryNum=0;

					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						if (cmd->m_cFuncCode==0x3F)
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定带时标的短浮点值，模式：执行，公共地址=%d，信息体地址=%d，值=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else if (cmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值，模式：执行，公共地址=%d，信息体地址=%d，值=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定归一化值，模式：执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						//OnShowMsg(LastError,sizeof(LastError));
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if(true/*m_bInfoMsg*/)
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						if (cmd->m_cFuncCode==0x3F)
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定带时标的短浮点值，模式：执行，公共地址=%d，信息体地址=%d，值=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else if (cmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值，模式：执行，公共地址=%d，信息体地址=%d，值=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定归一化值，模式：执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//新规约打印
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
				}

			}
			break;
		case eCmdStateYkSelectCon:
		case eCmdStateYkExec:
		case eCmdStateYkExecAct:
			v_cmd->m_nCurTryNum++;
			if(v_cmd->m_nCurTryNum>=v_cmd->m_nMaxTryCount)
			{
				v_cmd->m_nCurTryNum=0;

				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					cmd = (Q_cmdIEC104_SE *)v_cmd;
					//IEC104主站遥调系数处理
					//float fFactor = m_pMgr->getYtcoefficient(m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);
					//cmd->m_fVal = cmd->m_fVal*fFactor;
					//cmd->m_cOnOff = cmd->m_cOnOff*fFactor;

					memset(LastError,0,sizeof(LastError));
					if(cmd!=NULL)
					{
						sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete command.Command info:addr=%d,value=%d or %f\n",cmd->m_nInfoAddr,cmd->m_cOnOff,cmd->m_fVal);
					}
					else
					{
						sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete command.\n");
					}
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					cmd = (Q_cmdIEC104_SE *)v_cmd;
					//IEC104主站遥调系数处理
					//float fFactor = m_pMgr->getYtcoefficient(m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);
					//cmd->m_fVal = cmd->m_fVal*fFactor;
					//cmd->m_cOnOff = cmd->m_cOnOff*fFactor;


					memset(LastError,0,sizeof(LastError));
					if(cmd!=NULL)
					{
						sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete command.Command info:addr=%d,value=%d or %f\n",cmd->m_nInfoAddr,cmd->m_cOnOff,cmd->m_fVal);
					}
					else
					{
						sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete command.\n");
					}
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}



				Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
				Q_RECORDDATATAG ytRec;
				ytRec.eDataType=eDataTypeFloat;
				ytRec.channelID=m_ProSetting->getChannelID();
				ytRec.device=cmd->m_nComAsdu;
				ytRec.no=cmd->m_nInfoAddr;
				ytRec.fValue=pcmd->m_fVal;//赋初值

				if(!m_pMgr->getYtValue(&ytRec))
				{
					sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);

					/*//原CommApp规约打印保存、先保留着
					//OnShowMsg(LastError,sizeof(LastError));
					if (m_bInfoMsg)
					{
					sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					//	OnInfoMsg(LastError,strlen(LastError)+1);
					}
					*/

					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
					return false;
				}

				if(m_pMgr->handleMasterYt(&ytRec)!=0)
				{
					sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					/*//原CommApp规约打印保存、先保留着
					OnShowMsg(LastError,sizeof(LastError));
					if (m_bInfoMsg)
					{
					sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					OnInfoMsg(LastError,strlen(LastError)+1);
					}
					*/
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}


				/*				Q_dataManager* p = _GlobalData.getDataManager();
				LPQ_YTEDATATAG pYTData=p->getYTData();
				int nCount = p->getYTCount();
				int nChanID = m_ProSetting->getChannelID();
				for (int i=0;i<nCount;i++)
				{
				if (nChanID==pYTData[i].channelid && v_cmd->m_nComAsdu==pYTData[i].device && v_cmd->m_nInfoAddr ==pYTData[i].no)
				{
				pYTData[i].execresult = 2;
				break;
				}
				}
				*/
				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//存在
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						curSetPoint.iexecret = 2;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调执行时间
					}
				}
				/*by fanshuo end*/

				m_listCmd->removeCommand(v_cmd);
			}
			//超时无返回删除命令
			break;
		}
	}
	////////直接执行模式////////////////////////////////////////
	else if(m_cfgControl->m_YtExecMode==eYtExecModeDirect)
	{
		TRACE("=======m_cfgControl->m_YtExecMode==eYtExecModeDirect 11111/n");
		v_cmd->m_nCurTryNum++;
		if(v_cmd->m_nCurTryNum>=v_cmd->m_nMaxTryCount)
		{
			v_cmd->m_nCurTryNum=0;	
			if(m_ProSetting->getPrintSetting(eComLogTypePrompt)==true)
			{
#ifdef _DEBUG
				cmd = (Q_cmdIEC104_SE *)v_cmd;
				//IEC104主站遥调系数处理
				//float fFactor = m_pMgr->getYtcoefficient(m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);
				//cmd->m_fVal = cmd->m_fVal*fFactor;
				//cmd->m_cOnOff = cmd->m_cOnOff*fFactor;

				memset(LastError,0,sizeof(LastError));
				if(cmd!=NULL)
				{
					if(cmd->m_cFuncCode==0x30)
					{
						sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete command.Command info:addr=%d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);

					}
					else if(cmd->m_cFuncCode==0x32||cmd->m_cFuncCode==0x3F)
					{
						sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete command.Command info:addr=%d,value=%f\n",cmd->m_nInfoAddr,cmd->m_fVal);
					}
				}
				else
				{
					sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete command.\n");
				}
				//OnShowMsg(LastError,sizeof(LastError));
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}

			if(true/*m_bInfoMsg*/)
			{
#ifdef _DEBUG
				cmd = (Q_cmdIEC104_SE *)v_cmd;
				//IEC104主站遥调系数处理
				//float fFactor = m_pMgr->getYtcoefficient(m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);
				//cmd->m_fVal = cmd->m_fVal*fFactor;
				//cmd->m_cOnOff = cmd->m_cOnOff*fFactor;

				memset(LastError,0,sizeof(LastError));
				if(cmd!=NULL)
				{
					if(cmd->m_cFuncCode==0x30)
					{
						sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete command.Command info:addr=%d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);

					}
					else if(cmd->m_cFuncCode==0x32||cmd->m_cFuncCode==0x3F)
					{
						sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete command.Command info:addr=%d,value=%f\n",cmd->m_nInfoAddr,cmd->m_fVal);
					}
				}
				else
				{
					sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete command.\n");
				}
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//新规约打印
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			Q_dataManager* p = _GlobalData.getDataManager();
			LPQ_YTEDATATAG pYTData=p->getYTData();
			int nCount = p->getYTCount();
			int nChanID = m_ProSetting->getChannelID();
			for (int i=0;i<nCount;i++)
			{
				if (nChanID==pYTData[i].channelid && v_cmd->m_nComAsdu==pYTData[i].device && v_cmd->m_nInfoAddr ==pYTData[i].no)
				{
					pYTData[i].execresult = 2;
					break;
				}
			}
			m_listCmd->removeCommand(v_cmd);
		}

		int icmd = v_cmd->m_eState;
		switch (icmd)
		{
		case eCmdStateYkSelect:
			{
				cmd = (Q_cmdIEC104_SE *)v_cmd;
				//IEC104主站遥调系数处理
				float fFactor = m_pMgr->getYtcoefficient(m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);
				cmd->m_fVal = cmd->m_fVal*fFactor;
				cmd->m_cOnOff = short(cmd->m_cOnOff*fFactor);

				if(cmd!=NULL)
				{
					switch(m_cfgControl->m_YsetAsduType)
					{
					case eYsetAsdu30:
						cmd->m_cFuncCode=0x30;
						break;
					case eYsetAsdu31:
						cmd->m_cFuncCode=0x30;
						break;
					case eYsetAsdu32:
						cmd->m_cFuncCode=0x32;
						break;
					case eYsetAsdu3F:
						cmd->m_cFuncCode=0x3F;
						break;
					default:
						cmd->m_cFuncCode=0x30;
						break;
					}

					/*by fanshuo*/
					/*					bool bExist = false;
					int	YTID32 = -1;
					int YTtoYCID32 = -1;
					SCADA_SetPoint curSetPoint;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

					if (bExist)//存在
					{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
					CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
					SYSTEMTIME time = {0};
					curSetPoint.selecttime = time;

					if (cmd->m_cFuncCode==0x3F||cmd->m_cFuncCode==0x32)
					{//浮点
					curSetPoint.fVal = cmd->m_fVal;
					}
					else
					{
					curSetPoint.fVal = cmd->m_cOnOff;
					}

					GetLocalTime(time);
					curSetPoint.exectime = time;
					curSetPoint.iexecret = 1;
					dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调执行时间-直接执行模式

					if (YTtoYCID32 > 0)
					{
					Q_RECORDDATATAG dataArray;

					SCADA_Analog tempAnalog;
					dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);

					if (&tempAnalog != NULL)
					{
					tempAnalog.fVal = curSetPoint.fVal;
					tempAnalog.tSnapTime = time;
					dbmg_sdb->SetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);
					}
					}						
					}
					}
					else
					{
					#ifdef _DEBUG
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 处理ChannelID=%d，device=%d,no=%d的遥调选择执行时，在遥调命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
					m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
					#endif
					}

					*/
					if(cmd->m_cFuncCode==0x30)
					{
						Q_IEC104_ST_C_SE st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SE));
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;
						if(cmd->m_cOnOff>=0)
						{
							if(m_cfgControl->getAsdu48Mode==0)
							{
								st.val1=cmd->m_cOnOff & 0xFF;
								st.val2=(cmd->m_cOnOff>>8) & 0x7F;
								st.s=0;
							}
							else if(m_cfgControl->getAsdu48Mode>0)
							{
								int n=(int)(cmd->m_fVal*32767/m_cfgControl->getAsdu48Mode);
								st.val1=n& 0xFF;
								st.val2=(n>>8) & 0x7F;
								st.s=0;
							}
						}
						else
						{
							//负数用补码表示，本来就是补码吗
							//负数用补码表示
							int n=~(abs(cmd->m_cOnOff))+1;
							st.val1= n& 0xFF;
							st.val2=(n>>8) &0x7F;
							st.s=1;
						}
						st.qos.se=0;

						TRACE("=======m_cfgControl->m_YtExecMode==eYtExecModeDirect m_cFuncCode==0x30 st.qos.se=%d 11111/n",
							st.qos.se);

						sendYtCommand(v_cmd->m_nComAsdu,CMD_ACT,&st);
						cmd->m_cQuality=0;
						cmd->m_eState=eCmdStateYkExec;
						if(m_ProSetting->getPrintSetting(eComLogTypePrompt)==true)
						{
#ifdef _DEBUG
							memset(LastError,0,sizeof(LastError));
							sprintf_s(LastError,sizeof(LastError),"****  设定归一化值，模式：直接执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnShowMsg(LastError,sizeof(LastError));
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
						}
						if(true/*m_bInfoMsg*/)
						{
#ifdef _DEBUG
							memset(LastError,0,sizeof(LastError));
							sprintf_s(LastError,sizeof(LastError),"****  设定归一化值，模式：直接执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
						}
					}
					else if(cmd->m_cFuncCode==0x32)
					{

						Q_IEC104_ST_C_SE3 st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SE));
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;

						memcpy(&st.val,&cmd->m_fVal,sizeof(st.val));
						st.qos.se=0;

						TRACE("=======m_cfgControl->m_YtExecMode==eYtExecModeDirect m_cFuncCode==0x32 st.qos.se=%d 11111/n",
							st.qos.se);


						sendYtCommand3(v_cmd->m_nComAsdu,CMD_ACT,&st);
						cmd->m_cQuality=0;
						cmd->m_eState=eCmdStateYkExec;
						if(m_ProSetting->getPrintSetting(eComLogTypePrompt)==true)
						{
#ifdef _DEBUG
							memset(LastError,0,sizeof(LastError));
							sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值，模式：直接执行，公共地址=%d,信息体地址=%d,值=%.3f.\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
							//OnShowMsg(LastError,sizeof(LastError));
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
						}
						if(true/*m_bInfoMsg*/)
						{
#ifdef _DEBUG
							memset(LastError,0,sizeof(LastError));
							sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值，模式：直接执行，公共地址=%d,信息体地址=%d,值=%.3f.\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
						}
					}
					else if (cmd->m_cFuncCode==0x3F)
					{
						Q_IEC104_ST_C_SE_TC1 st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SE_TC1));
						SYSTEMTIME sttime;
						GetLocalTime(sttime);
						unsigned int m;
						m = sttime.wMilliseconds + 1000 * sttime.wSecond;
						st.CP56Time[0] = m & 0xFF;
						st.CP56Time[1] = (m & 0xFF00)>>8;
						st.CP56Time[2] = sttime.wMinute & 0x00FF;	// add valid flag and genuine flag
						st.CP56Time[3] = sttime.wHour & 0x00FF;	// add summer flag
						st.CP56Time[4] = ((sttime.wDayOfWeek%7)&0x03)<<5 | (sttime.wDay&0x1F);
						st.CP56Time[5] = sttime.wMonth & 0x0F;
						st.CP56Time[6] = sttime.wYear - 2000;
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;
						memcpy(st.val,&cmd->m_fVal,sizeof(float));
						sendYtCommandFloatWithTime(v_cmd->m_nComAsdu,CMD_ACT,&st);

						cmd->m_cQuality=0;
						cmd->m_eState=eCmdStateYkExec;
						if(m_ProSetting->getPrintSetting(eComLogTypePrompt)==true)
						{
#ifdef _DEBUG
							memset(LastError,0,sizeof(LastError));
							sprintf_s(LastError,sizeof(LastError),"****  设定带时标的短浮点值，模式：直接执行，公共地址=%d，信息体地址=%d，值=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
							//OnShowMsg(LastError,sizeof(LastError));
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
						}
						if(true/*m_bInfoMsg*/)
						{
#ifdef _DEBUG
							memset(LastError,0,sizeof(LastError));
							sprintf_s(LastError,sizeof(LastError),"****  设定带时标的短浮点值，模式：直接执行，公共地址=%d，信息体地址=%d，值=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//新规约打印
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
						}
					}
				}
			}
			break;
		}

	}

	return bRet;
}


int Q_protocolIEC104Master::handleCommmd_RC(Q_cmdIEC104 *v_cmd)
{
	int bRet=Q_104SUC;

	v_cmd->m_bExec=true;//标析

	if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		int icmd = v_cmd->m_eState;
		switch (icmd)
		{
		case eCmdStateYkSelect:
			{
				//发送确认帧
				Q_cmdIEC104_RC *cmd = (Q_cmdIEC104_RC *)v_cmd;
				if (NULL != cmd)
				{
					//if (sendYkResponse(cmd->m_nComAsdu,CMD_ACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2F,sizeof(cmd->m_nValue))==Q_104FAIL)
					//{
					//    //发送报文出错
					//    m_errorNo=e104Errno_CmdSend;
					//    v_cmd->m_eState=eCmdStateYkConfirm;
					//    v_cmd->m_bError=true;
					//    sprintf_s(LastError,sizeof(LastError), "发送升降命令,选择确认出错，当前行号:%d\n",__LINE__);
					//    //OnErrorMsg(m_errorNo, LastError);nn
					//    bRet=false;
					//}
					//else
					//{
					//    v_cmd->m_eState=eCmdStateYkConfirm;
					//}
				}

			}
			break;
		case eCmdStateYkExec:
			{
				//执行
				Q_cmdIEC104_RC *cmd = (Q_cmdIEC104_RC *)v_cmd;
				Q_RECORDDATATAG dataArray;

				dataArray.eDataType=eDataTypeInt;
				dataArray.nValue=cmd->m_cOnOff;
				dataArray.nIndex=v_cmd->m_nPointNo;
				Q_dataManager* p = _GlobalData.getDataManager();

				//更新实时库
				//这里需要重新处理一下．升降命令的解析未知
				//	RCS  =0  ：不允许；1  ：降一步；2  ：升一步；3  ：不允许
				if (1==1)
				{
					//更新成功
					v_cmd->m_eState=eCmdStateYkConfirm;

					//执行完成 //TODO:
					//查询看是否返回执行后的值 如果要求返回则返回

					v_cmd->m_eState=eCmdStateEnd;

				}
				else
				{
					v_cmd->m_bError=true;
					v_cmd->m_eState=eCmdStateYkConfirm;
					m_errorNo=e104Errno_UptDb;
#ifdef _DEBUG
					sprintf_s(LastError, sizeof(LastError),"执行升降命令，更新实时库出错，当前行号:%d\n",__LINE__);
					//OnErrorMsg(m_errorNo, LastError);nn
#endif
					bRet=false;

				}

			}
			break;
		case eCmdStateYkCancelAct:
			{
				//取消命令
				//发送确认帧
				Q_cmdIEC104_RC *cmd = (Q_cmdIEC104_RC *)v_cmd;
				if (NULL != cmd)
				{
					v_cmd->m_bError=true;
					//                int  nRet=sendYkResponse(cmd->m_nComAsdu,CMD_DEACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2F,sizeof(cmd->m_nValue));
					//                if (nRet==Q_104FAIL)
					//                {
					//                    m_errorNo=e104Errno_CmdSend;
					//#ifdef _DEBUG
					//                    sprintf_s(LastError,sizeof(LastError), "发送升降命令取消确认出错，当前行号:%d\n",__LINE__);
					//                    //OnErrorMsg(m_errorNo, LastError);nn
					//                    bRet=false;
					//#endif
					//                }
					//                else
					//                {
					//                    v_cmd->m_eState=eCmdStateYkCancelAct;
					//                }

					// v_cmd->m_eState=eCmdStateEnd;
				}
			}
			break;
		}

	}
	else if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		int icmd = v_cmd->m_eState;
		switch (icmd)
		{
		case eCmdStateIntEnd:
			{
				//Interrogation Command (Group %d): Activation Termination.
			}
			break;
		}

	}
	return bRet;
}
int Q_protocolIEC104Master::sendYtCommand(const int &asdu,const int &cause,Q_IEC104_ST_C_SE *st)
{
	Q_IEC104_ASDU msg;
	msg.header.type =0x30;					// 设定命令
	msg.header.qual =0x01 ;					// number of elements
	msg.header.tx_cause_1 = cause;			//
	msg.header.tx_cause_2 = 0x00;
	msg.header.commom_asdu_1 = asdu & 0xFF;
	msg.header.commom_asdu_2 = (asdu>>8) & 0xFF;
	// group information
	memcpy(&msg.data[0],st,sizeof(Q_IEC104_ST_C_SE));

	return send_I_Msg((BYTE*)&msg,sizeof(Q_IEC104_ST_C_SE)+6);
}

int Q_protocolIEC104Master::sendYtCommandFloatWithTime(const int &asdu,const int &cause,Q_IEC104_ST_C_SE_TC1 *st)
{
	Q_IEC104_ASDU msg;
	msg.header.type =0x3F;					// 设定命令
	msg.header.qual =0x01 ;					// number of elements
	msg.header.tx_cause_1 = cause;			//
	msg.header.tx_cause_2 = 0x00;
	msg.header.commom_asdu_1 = asdu & 0xFF;
	msg.header.commom_asdu_2 = (asdu>>8) & 0xFF;
	// group information
	memcpy(&msg.data[0],st,sizeof(Q_IEC104_ST_C_SE_TC1));
	return send_I_Msg((BYTE*)&msg,sizeof(Q_IEC104_ST_C_SE_TC1)+6);
}
int Q_protocolIEC104Master::sendYtCommand3(const int &asdu,const int &cause,Q_IEC104_ST_C_SE3 *st)
{
	Q_IEC104_ASDU msg;
	msg.header.type =0x32;					// 设定命令
	msg.header.qual =0x01 ;					// number of elements
	msg.header.tx_cause_1 = cause;			//
	msg.header.tx_cause_2 = 0x00;
	msg.header.commom_asdu_1 = asdu & 0xFF;
	msg.header.commom_asdu_2 = (asdu>>8) & 0xFF;
	// group information
	memcpy(&msg.data[0],st,sizeof(Q_IEC104_ST_C_SE3));

	return send_I_Msg((BYTE*)&msg,sizeof(Q_IEC104_ST_C_SE3)+6);
}

int Q_protocolIEC104Master::sendYkCommand(const int &asdu,const int &cause,Q_IEC104_ST_C_SP *st)
{
	Q_IEC104_ASDU msg;
	msg.header.type =0x2D;					// 遥控命令
	msg.header.qual =0x01 ;					// number of elements
	msg.header.tx_cause_1 = cause;			//
	msg.header.tx_cause_2 = 0x00;
	msg.header.commom_asdu_1 = asdu & 0xFF;
	msg.header.commom_asdu_2 = (asdu>>8) & 0xFF;
	// group information
	memcpy(&msg.data[0],st,sizeof(Q_IEC104_ST_C_SP));

	return send_I_Msg((BYTE*)&msg,sizeof(Q_IEC104_ST_C_SP)+6);
}

int Q_protocolIEC104Master::sendYkCommand1(const int &asdu,const int &cause,Q_IEC104_ST_C_SC_TA1 *st)
{
	Q_IEC104_ASDU msg;
	msg.header.type =0x3A;					// 遥控命令
	msg.header.qual =0x01 ;					// number of elements
	msg.header.tx_cause_1 = cause;			//
	msg.header.tx_cause_2 = 0x00;
	msg.header.commom_asdu_1 = asdu & 0xFF;
	msg.header.commom_asdu_2 = (asdu>>8) & 0xFF;
	// group information
	memcpy(&msg.data[0],st,sizeof(Q_IEC104_ST_C_SC_TA1));

	return send_I_Msg((BYTE*)&msg,sizeof(Q_IEC104_ST_C_SC_TA1)+6);
}


void Q_protocolIEC104Master::func_M_SP_TB_1(Q_104ApduEx *msg)
{
	int suc_count=0;
	UINT pos;
	DWORD adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq)
	{
		Q_SP104 *ps = (Q_SP104*)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);
		adr =  ps->addr1<<8 | ps->addr0;

		pos +=2;//地址域
		Q_SIQ104  *pi;


		while (pos < msg->len)
		{
			pi = (Q_SIQ104*)&(msg->msgbuf[pos]);

			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeBit;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
			dataArray.bValue=pi->spi==1?true:false;

			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"func_M_SP_TB_1 Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError)," func_M_SP_TB_1 Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			pos+=(sizeof(Q_SIQ104)+7);//CP56Time2a时间站7个字节，不解析了
			adr++;
		}
	}
	else
	{
		while (pos < msg->len)
		{
			Q_SP104 *ps = (Q_SP104*)&(msg->msgbuf[pos]);

			adr =  ps->addr1<<8 | ps->addr0;


			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeBit;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
			dataArray.bValue=ps->siq.spi==1?true:false;

			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"func_M_SP_TB_1 Can't find right YX point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"func_M_SP_TB_1 Can't find right YX point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			pos+=(sizeof(Q_SP104)+7);//CP56Time2a时间站7个字节，不解析了
		}
	}


}

void Q_protocolIEC104Master::func_M_ME_TF_1(Q_104ApduEx *msg)
{
	int suc_count=0;
	UINT pos;
	DWORD adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq) //顺序的
	{
		adr = msg->msgbuf[pos+2]<<16 | msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos];

		while (pos < msg->len)
		{
			pos+=3;//fix by yf
			Q_MF104Seq *pf=(Q_MF104Seq *)&(msg->msgbuf[pos]);
			float *val = (float *)&pf->val[0]; 


			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeFloat;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
			dataArray.fValue= *val;

			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			adr++;
			pos+=(sizeof(Q_MF104) +7);
		}

	}
	else
	{
		while (pos < msg->len)
		{
			Q_MF104 *pf = (Q_MF104*)&(msg->msgbuf[pos]);

			adr =pf->addr1<<8 | pf->addr0;
			float *val = (float*)&pf->seqv.val[0];

			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeFloat;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
			dataArray.fValue= *val;

			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			pos+=(sizeof(Q_MF104) +7);
		}

	}
#ifdef _DEBUG
	sprintf_s(LastError,sizeof(LastError),"程序退出类型标识为0x24数据解析,此帧共有%d个遥测数据，成功入库个数%d\n",msg->num,suc_count);
#endif
	if (msg->num==suc_count)
	{
		//OnShowMsg(LastError,sizeof(LastError));nn
	}
	else
	{
		//OnErrorMsg(LastError,sizeof(LastError));nn
	}

}

void Q_protocolIEC104Master::func_M_BO_TB_1(Q_104ApduEx *msg)
{
	return;
}

bool Q_protocolIEC104Master::time_to_IC()
{
	bool ret = false;
	time(&now);
	if((now - timer_ICTimeflag) >= m_cfgSt->n_IcInterval)
	{
		ret = true;
	}
	return ret;
}

bool Q_protocolIEC104Master::FindYK(int &YKtoYXID32, int &YKID32, SCADA_Relay &curRelay, Q_cmdIEC104_SP*cmd)
{
	//判断遥控点是否存在
	bool bExist = false;

	CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	int nRelayRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_Relay_ID );
	for (int j=0; j<nRelayRecordCount; j++)
	{
		int nTempid32 = j+1;
		dbmg_sdb->GetElementByID(SCADA_TAB_Relay_ID, nTempid32,&curRelay);

		if (m_ProSetting->getChannelID() == curRelay.uTuID && cmd->m_nComAsdu == curRelay.lDPUID &&
			cmd->m_nInfoAddr == curRelay.iNoInDPU)
		{
			bExist = true;
			YKID32 = nTempid32;
			YKtoYXID32 = curRelay.ulYXID32;
			break;
		}
	}
	return bExist;
}

bool Q_protocolIEC104Master::FindYT(int &YTtoYCID32, int &YTID32, SCADA_SetPoint &curSetPoint, Q_cmdIEC104_SE*cmd)
{
	int nPos = 0;
	bool bExist = false;

	CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	int nSetPointRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_SetPoint_ID );
	for (int j=0; j<nSetPointRecordCount; j++)
	{
		int nTempid32 = j+1;
		dbmg_sdb->GetElementByID(SCADA_TAB_SetPoint_ID, nTempid32,&curSetPoint);

		if (m_ProSetting->getChannelID() == curSetPoint.uTuID && cmd->m_nComAsdu == curSetPoint.lDPUID &&
			cmd->m_nInfoAddr == curSetPoint.iNoInDPU)
		{
			bExist = true;
			YTID32 = nTempid32;
			YTtoYCID32 = curSetPoint.ulYCID32;
			break;
		}
	}

	return bExist;
}

void Q_protocolIEC104Master::SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen)
{
	//TRACE("Q_protocolIEC104Master::SendMsgPrint,g_rtdbInfIEC104=%08x",m_pMgr->getRtdbInf());

	g_rtdbInfIEC104 = m_pMgr->getRtdbInf();
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

	if (nlen>=170)
	{
		nlen = 170;
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
			sprintf_s(strMsg,sizeof(strMsg),"MsgPrint:stid = %d msgtype id = %d message = %s",MsgPrint.ichannelid,MsgPrint.msgtype,MsgPrint.msg);
			TRACE("strMsg");*/
		}
	}
}
void Q_protocolIEC104Master::func_M_DP_TB_1(Q_104ApduEx *msg)
{

	int suc_count=0;
	UINT pos;
	DWORD adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq)
	{
		Q_DP104 *ps = (Q_DP104*)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);
		adr =  ps->addr1<<8 | ps->addr0;

		pos +=2;//地址域
		Q_DIQ104  *pi;


		while (pos < msg->len)
		{
			pi = (Q_DIQ104 *)&(msg->msgbuf[pos]);

			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeBit;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
		    if (pi->dpi==2)
			{
				dataArray.bValue=true;
			}
			else
			{
				dataArray.bValue=false;
			}
			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"func_M_DP_TB_1 Can't find right YX point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"func_M_DP_TB_1 Can't find right YX point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			pos+=(sizeof(Q_DIQ104)+7);//CP56Time2a时间站7个字节，不解析了
			adr++;
		}
	}
	else
	{
		while (pos < msg->len)
		{
			Q_DP104 *ps = (Q_DP104*)&(msg->msgbuf[pos]);

			adr = ps->addr1<<8 | ps->addr0;


			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeBit;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=msg->asdu;
			dataArray.no=adr;
			if (ps->diq.dpi==2)
			{
				dataArray.bValue=true;
			}
			else
			{
				dataArray.bValue=false;
			}

			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"func_M_DP_TB_1 Can't find right YX point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"func_M_DP_TB_1 Can't find right YX point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			pos+=(sizeof(Q_DP104)+7);//CP56Time2a时间站7个字节，不解析了
		}
	}

#ifdef _DEBUG
	sprintf_s(LastError,sizeof(LastError),"程序退出类型标识为0x1E数据解析,此帧共有%d个遥信数据，成功入库个数%d\n",msg->num,suc_count);
#endif	
	if (msg->num==suc_count)
	{
		//OnShowMsg(LastError,sizeof(LastError));nn
	}
	else
	{
		//OnErrorMsg(LastError,sizeof(LastError));nn
	}

}
