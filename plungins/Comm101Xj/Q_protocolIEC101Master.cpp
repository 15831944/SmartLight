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
		sprintf_s(LastError,sizeof(LastError),"��ʹ����ʼ...");
		//OnShowMsg(LastError,sizeof(LastError));
		//�¹�Լ��ӡ
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
	}

	printf("Q_protocolIEC104Master::startProcess() 33\n");

	if(m_bNomalInfoMsg)
	{
		printf("Q_protocolIEC104Master::startProcess() 44\n");
#ifdef _DEBUG
		sprintf_s(LastError,sizeof(LastError),"��ʹ����ʼ...");
		//OnNomalInfoMsg(LastError,sizeof(LastError));
		//�¹�Լ��ӡ
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
			//���ٻ�
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
	//m_listCmd->clearCommand(); //2013-02-24 ���buffer
	bStartFlg=false;
	m_LinkData.Sub_Inited_Flag=false;
	
	if(m_ProSetting->getPrintSetting(eComLogTypePrompt))
	{
		sprintf_s(LastError,sizeof(LastError),"�Ͽ�����...");
		//OnShowMsg(LastError,sizeof(LastError));
		//�¹�Լ��ӡ
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);

	}
	if(m_bNomalInfoMsg)
	{

		sprintf_s(LastError,sizeof(LastError),"�Ͽ�����...");
		//OnNomalInfoMsg(LastError,sizeof(LastError));
		//�¹�Լ��ӡ
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
	//1 ������·״̬   ��λԶ����· �ٻ�1���û����� �ٻ��û�2������ 


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
				  linkAckSubLinkStatus();//�ظ�linkstatus
				  m_LinkData.Sub_State=IEC101_LINK_SUB_STATE_SENDSTATUS;
				}
				//����һ���ظ�


			}
			break;
		case FUNC_PRM_RSTLINK://0
			{
				    //��վ�����������͸�λԶ����·
					if (pi->prm == 0)
				    {
						if(IEC101_LINK_PRM_STATE_SENDRST==m_LinkData.Master_State)
						{
                            //��վ�ظ��˸�λԶ����·������
						
							//��վ����վ���������
							m_LinkData.Master_State=IEC101_LINK_PRM_STATE_WAITDTCONF;
							m_LinkData.Sub_Inited_Flag=true;
							bStartFlg=true;
											//}
						}
						else if(IEC101_LINK_PRM_STATE_WAITDTCONF==m_LinkData.Master_State)
						{
							//�ڴ������������վ�������ĸ�λԶ����������
						    //��ʱ��������վ���������쳣����
							
						}
					}
					else if( IEC101_LINK_SUB_STATE_SENDSTATUS==m_LinkData.Sub_State)
					{
						linkAckSubReset( );
						m_LinkData.Sub_State=IEC101_LINK_SUB_STATE_SENDRSCONF;//������·
					}
				
			}
			break;
		case FUNC_PRM_REQ_CLASS2://11
			{
			
						//	IEC101_LINK_PRM_STATE_UNRESET==m_LinkData.Master_State)
					if(IEC101_LINK_PRM_STATE_REQSTATUS==m_LinkData.Master_State||
						IEC101_LINK_PRM_STATE_UNRESET==m_LinkData.Master_State)
					{
						//���������
						//���͸�λԶ��״̬
						linkSendLinkReset();
						m_LinkData.Master_State=IEC101_LINK_PRM_STATE_SENDRST;
					}
					else if(IEC101_LINK_PRM_STATE_WAITRSCONF==m_LinkData.Master_State)
					{
						m_LinkData.Master_State=IEC101_LINK_PRM_STATE_WAITDTCONF;
						//��ʾ��վ����վ��ʹ������
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

	apdu_ex.cause = header->tx_cause_1;	// ����ԭ��

	apdu_ex.asdu = header->commom_asdu_2 << 8;
	apdu_ex.asdu |= header->commom_asdu_1;

	apdu_ex.type=header->type;

	//TRACE("=====Q_protocolIEC104Master::decodeMsg_I len = %d  m_ProSetting = %08x=====\n",len,m_ProSetting);

#ifdef _DEBUG
	sprintf_s(LastError,sizeof(LastError), "ASDU Address=%d\tTypeID=%d\tCount=%d\tCause=%d\n",
		apdu_ex.asdu, header->type, apdu_ex.num, apdu_ex.cause);
	//OnErrorMsg(m_errorNo, LastError);
	//�¹�Լ��ӡ
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
		func_M_DP_NA_1(&apdu_ex);//ԭ����û�н�����д��ֻsprnitf
		break;
		//case 5: //step position
		//    func_M_ST_NA_1(&apdu_ex);//ԭ����û��
		//    break;
		//case 7: //32���ش�
		//    func_M_BO_NA_1(&apdu_ex);//ԭ����û��
		//    break;
	case 9:  //Measured Normalized ����ֵ
	case 11: //Measured Scaled ����ֵ����Ȼ�ֵ
		scaled=(header->type==11) ? true:false;
		func_M_ME_NA_NB_1(&apdu_ex,scaled);//fs20150703
		break;
	case 13: //Measure value, short floating point value
		func_M_ME_NC_1(&apdu_ex); 
		break;
	case 15: //�ۼ�����һ�㶼�ǵ����� int ����
		func_M_IT_NA_1(&apdu_ex);//ԭ����û�н�����д��sprnitf
		break;
	case 30: //Single-point information with time tag CP56Time2a
		func_M_SP_TB_1(&apdu_ex);//fs20150713
		break;
	case 31://˫���ʱ���ʽ��
			func_M_DP_TB_1(&apdu_ex);
			break;
		//case 32://��λ����Ϣ
		//	func_M_ST_TB_1(msgbuf,len,asdu,seq);
		//	break;
	case 33://32λ���ش�     
		func_M_BO_TB_1(&apdu_ex);//ԭ����û�д���
		break;
		//case 34: //��ʱ���ʽ�Ĺ�Ȼ�ֵ
		//case 35: //��ʱ��ʽ�ı�Ȼ�ֵ
		//	scaled=(header->type==35) ? true:false;
		//	func_M_ME_TD_TE_1(msgbuf,len,asdu,seq,scaled);
		//	break;
	case 36:	// Measure value, short float point value w/CP56Time2a
		func_M_ME_TF_1(&apdu_ex);//fs20150713
		break;
		//case 37://�ۼ�ֵ��ʱ��
		//	func_M_IT_TB_1(msgbuf,len,asdu,seq);
		//	break;
	case 21:
		func_M_ME_ND_1(&apdu_ex);//��һ��ֵ����ʱ�ꡢ��Ʒ��
		break;
	case 45:
		//����ң��
		func_C_SC_NA_1(&apdu_ex);
		break;
	case 46:
		//˫��ң��
		func_C_DC_NA_1(&apdu_ex);
		break;
	case 47:
		//ң��
		func_C_RC_NA_1(&apdu_ex);
		break;
	case 48:
		//�趨��һ��ֵ,��һ��ֵ
		func_C_SE_NA_1(&apdu_ex);
		break;
	case 50:
		//�̸�����
		func_C_SE_NC_1(&apdu_ex);
		break;
	case 58:
		//��ʱ���ң������
		func_C_SC_TA_1(&apdu_ex);
		break;
	case 63:
		//�趨��ʱ��ĸ�����
		func_C_SE_TC_1(&apdu_ex);
		break;
	case 70:
		//��ʹ������
		if(IEC101_LINK_SUB_STATE_SENDRSCONF==m_LinkData.Sub_State)
		{
	        m_LinkData.Sub_State=IEC101_LINK_SUB_STATE_INITFIN;//������·
		}
		TRACE("End of Initialization\n");
		break;
	case 100:
		ret=func_Interrogate(&apdu_ex);
		break;
	case 103:
		//ʱ��ͬ������
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

	//��һ����һ��
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
	msg.header.type = 0x64;					// interrogation command   ������
	msg.header.qual = 0x01;					// number of elements
	msg.header.tx_cause_1 = cause;			// 0x07 act confirm  0x0A ֹͣ����
	msg.header.tx_cause_2 = 0x00;
	msg.header.commom_asdu_1 = asdu & 0xFF;
	msg.header.commom_asdu_2 = (asdu>>8) & 0xFF;
	// group information
	msg.data[0] = 0x00;
	msg.data[1] = 0x00;//�����ֽ�
	msg.data[2] = 0x14;//����
	return send_I_Msg((BYTE*)&msg, 9);
}

bool Q_protocolIEC104Master::TxHandler()
{
	return 0;
}

int Q_protocolIEC104Master::func_C_SC_NA_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_SP *ps = ( Q_IEC104_ST_C_SP *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;

	if(m_cfgControl->m_YkExecMode==eYtExecModeSelect)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//����ɹ�

			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkSelectCon,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵѡ��ȷ�ϣ���Ϣ���ַ=%d��ֵ=%d\n  ****",adr,val);
					//OnShowMsg(LastError,sizeof(LastError));
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵѡ��ȷ�ϣ���Ϣ���ַ=%d��ֵ=%d\n  ****",adr,val);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//�¹�Լ��ӡ
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

				if (bExist)//����
				{
					if ((YKID32>0) && (&curRelay!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curRelay.selecttime = time;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ѡ��ʱ��
					}
				}/*by fanshuo end*/
				else
				{
#ifdef _DEBUG
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d �յ�ChannelID=%d��device=%d,no=%d��ң��ѡ�񷵻�ȷ�ϣ�����ң�������б��в����ڣ�����\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

					//�¹�Լ��ӡ
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
						sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
						//OnShowMsg(LastError,sizeof(LastError));
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if(true/*m_bInfoMsg*/)
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						int val=ps->sco.scs;
						sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					//ִ�гɹ�

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

					if (bExist)//����
					{
						if ((YKID32>0) && (&curRelay!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							GetLocalTime(time);
							curRelay.exectime = time;
							curRelay.iexecret = 1;
							dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ�гɹ�ʱ��
						}
					}
					/*by fanshuo end*/
					m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkExec,adr);
				}
			}
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//δִ�гɹ�
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
					//�¹�Լ��ӡ
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
					//�¹�Լ��ӡ
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

				if (bExist)//����
				{
					if ((YKID32>0) && (&curRelay!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						curRelay.selecttime = time;
						curRelay.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʧ��ʱ��
					}
				}
				/*by fanshuo end*/

				m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkSelectCon,adr);
			}
		}
		else
		{
			//������47֮���
			//<44> ���� δ֪�����ͱ�ʶ
			//	<45> ���� δ֪�Ĵ���ԭ��
			//	<46> ���� δ֪��Ӧ�÷������ݵ�Ԫ������ַ
			//	<47> ���� δ֪����Ϣ�����ַ

			//δִ�гɹ�
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

			if (bExist)//����
			{
				if ((YKID32>0) && (&curRelay!=NULL))
				{
					CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
					SYSTEMTIME time = {0};
					curRelay.exectime = time;
					curRelay.iexecret = 3;
					dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʧ��ʱ��
				}
			}	
			/*by fanshuo end*/

			if(cmd!=NULL)
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			else
			{
#ifdef _DEBUG
				cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//�¹�Լ��ӡ
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
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
		}
	}
	else if(m_cfgControl->m_YkExecMode==eYtExecModeDirect)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//����ȷ��
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
					//OnShowMsg(LastError,sizeof(LastError));
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//�¹�Լ��ӡ
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

				if (bExist)//����
				{
					if ((YKID32>0) && (&curRelay!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curRelay.exectime = time;
						curRelay.iexecret = 1;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ�гɹ�ʱ��-ֱ��ִ��
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
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ�������趨�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExecAct,adr);
			if(true/*m_bInfoMsg*/)
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ�������趨�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkExecAct,adr);
		}
		else 
		{
			//������47֮���
			//<44> ���� δ֪�����ͱ�ʶ
			//	<45> ���� δ֪�Ĵ���ԭ��
			//	<46> ���� δ֪��Ӧ�÷������ݵ�Ԫ������ַ
			//	<47> ���� δ֪����Ϣ�����ַ

			//δִ�гɹ�
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

			if (bExist)//����
			{
				if ((YKID32>0) && (&curRelay!=NULL))
				{
					CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
					SYSTEMTIME time = {0};
					curRelay.exectime = time;
					curRelay.iexecret = 3;
					dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ʧ��ʱ��
				}
			}	
			/*by fanshuo end*/


			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			//			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
			if(true/*m_bInfoMsg*/)
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//�¹�Լ��ӡ
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

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_DC *ps = ( Q_IEC104_ST_C_DC *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);


	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;


	if (eStation_Slave==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT) //����
		{
			Q_cmdIEC104_DP *cmd=NULL;

			if (1==ps->dco.se)//ѡ��
			{
				Q_cmdIEC104_DP *cmd=new Q_cmdIEC104_DP;

				Q_dataManager* p = _GlobalData.getDataManager();

				LPQ_DIGITALEDATATAG alg=NULL;
				if(m_tags!=NULL)
					p->findDigTag(msg->asdu,adr,m_tags->getAnalogBegin(),m_tags->getAnalogEnd());

				if (alg!=NULL)/*��ѯ��������ݴ���*/
				{
					cmd->m_eState=eCmdStateYkSelect;
					cmd->m_nTxCause=msg->cause;
					cmd->m_nComAsdu=msg->asdu;
					cmd->m_bExec=false;
					cmd->m_cOnOff=ps->dco.scs;
					cmd->m_cQuality=ps->dco.qu;
					cmd->m_nValue=*((BYTE *)&(ps->dco));

					cmd->m_nInfoAddr=adr;//��Ϣ�����ַ

				}
				else
				{
					//����в�����
					cmd->m_bError=true;
#ifdef _DEBUG
					m_errorNo=e104ErrNo_P_UnExist; //��ð���ϸ�����¼һ�¡�
					sprintf_s(LastError,sizeof(LastError), "��Ϣ�����ַ����Ӧ�ĵ�Ų����ڣ���ǰ��ַΪ%d\n",adr);
					//OnErrorMsg(m_errorNo, LastError);
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				m_listCmd->addCommand(cmd);//������ѹջ
			}
			else if (0==ps->dco.se)
			{
				//ִ��
				//���ݵ�š���������Ϊ������ �Ȳ�ѯ����
				Q_cmdIEC104 *v_cmd=NULL;
				//����״ֵ̬��ѯ����
				v_cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkConfirm,adr);
				if (v_cmd!=NULL)
				{
					//����״̬
					Q_cmdIEC104_SP *cmd=(Q_cmdIEC104_SP *)v_cmd;
					cmd->m_bExec=false;
					cmd->m_eState=eCmdStateYkExec;
					cmd->m_nValue=*((BYTE *)&(ps->dco));
				}
				else
				{
#ifdef _DEBUG
					m_errorNo=e104ErrNo_CmdNoSel;
					sprintf_s(LastError,sizeof(LastError), "����ִ�е���ң�������ַ����������豸û��ѡ�񡣵�ǰ�к�:%d\n",__LINE__);
					//OnErrorMsg(m_errorNo, LastError);
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
					return Q_104FAIL;
				}
			}
		}
		else if (msg->cause == CMD_DEACT) //ֹͣ����
		{
			//��������
			Q_cmdIEC104 *v_cmd=NULL;
			//����״ֵ̬��ѯ����
			v_cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkConfirm,adr);
			if (v_cmd!=NULL)
			{
				//����״̬
				Q_cmdIEC104_SP *cmd=(Q_cmdIEC104_SP *)v_cmd;
				cmd->m_bExec=false;
				cmd->m_eState=eCmdStateYkCancelAct;
				cmd->m_nValue=*((BYTE *)&(ps->dco));
			}
			else
			{
				m_errorNo=e104ErrNo_CmdNoSel;
#ifdef _DEBUG
				sprintf_s(LastError,sizeof(LastError), "����ȡ������ң�������ַ����������豸û��ѡ�񡣵�ǰ�к�:%d\n",__LINE__);
				//OnErrorMsg(m_errorNo, LastError);
				//�¹�Լ��ӡ
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
			//ѡ��ȷ��
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//��ִ����ɣ����ǳ������
		}
	}

	return ret;
}

int Q_protocolIEC104Master::func_C_SE_NA_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_SE *ps = ( Q_IEC104_ST_C_SE *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr =  ps->addr1<<8 | ps->addr0;

	if(m_cfgControl->m_YtExecMode==eYtExecModeSelect)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//����ɹ�
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
						sprintf_s(LastError,sizeof(LastError),"�趨��ʱ��Ķ̸���ֵѡ��ȷ�ϣ���Ϣ���ַ=%d,ֵ=%f\n",adr,fval);
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
						sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵѡ��ȷ�ϣ���Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
#endif					
					}
#ifdef _DEBUG
					//OnShowMsg(LastError,sizeof(LastError));
					//�¹�Լ��ӡ
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
						sprintf_s(LastError,sizeof(LastError),"�趨��ʱ��Ķ̸���ֵѡ��ȷ�ϣ���Ϣ���ַ=%d,ֵ=%f\n",adr,fval);
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
						sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵѡ��ȷ�ϣ���Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
#endif					
					}
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//�¹�Լ��ӡ
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
				ytRec.fValue=pcmd->m_fVal;//����ֵ

				if(!m_pMgr->getYtValue(&ytRec))
				{
					sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);

					/*//ԭCommApp��Լ��ӡ���桢�ȱ�����
					//OnShowMsg(LastError,sizeof(LastError));
					if (m_bInfoMsg)
					{
					sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					//	OnInfoMsg(LastError,strlen(LastError)+1);
					}
					*/

					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);

					return false;
				}

				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//����
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.selecttime = time;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ѡ��ʱ��
					}
				}
				/*by fanshuo end*/
				else
				{
#ifdef _DEBUG
					SYSTEMTIME st;
					GetLocalTime(&st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d �յ�ChannelID=%d��device=%d,no=%d��ң��ѡ�񷵻�ȷ�ϣ�����ң�������б��в����ڣ�����\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

					/*//ԭCommApp��Լ��ӡ���桢�ȱ�����
					//OnShowMsg(LastError,sizeof(LastError));
					if(true)
					{
					//	OnInfoMsg(LastError,sizeof(LastError),cmd);
					}
					*/

					//�¹�Լ��ӡ
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
						sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
						//OnShowMsg(LastError,sizeof(LastError));

						//�¹�Լ��ӡ
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
						sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}


					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					Q_RECORDDATATAG ytRec;
					ytRec.eDataType=eDataTypeFloat;
					ytRec.channelID=m_ProSetting->getChannelID();
					ytRec.device=cmd->m_nComAsdu;
					ytRec.no=cmd->m_nInfoAddr;
					ytRec.fValue=pcmd->m_fVal;//����ֵ

					if(!m_pMgr->getYtValue(&ytRec))
					{
						sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);

						/*//ԭCommApp��Լ��ӡ���桢�ȱ�����
						//OnShowMsg(LastError,sizeof(LastError));
						if (m_bInfoMsg)
						{
						sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
						//	OnInfoMsg(LastError,strlen(LastError)+1);
						}
						*/

						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);

						return false;
					}

					/*by fanshuo*/
					bool bExist = false;
					int	YTID32 = -1;
					int YTtoYCID32 = -1;
					SCADA_SetPoint curSetPoint;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

					if (bExist)//����
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							GetLocalTime(time);
							curSetPoint.exectime = time;
							curSetPoint.iexecret = 1;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ�гɹ�ʱ��
						}
					}
					/*by fanshuo end*/


					m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);
				}
			}	
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//������ֹ
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
						sprintf_s(LastError,sizeof(LastError),"�趨��ʱ��Ķ̸���ֵ������ֹ����Ϣ���ַ=%d,ֵ=%f\n",adr,fval);
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
						sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵ������ֹ����Ϣ���ַ=%d,ֵ=%d  ****\n",adr,val);
#endif						
					}
#ifdef _DEBUG
					//OnShowMsg(LastError,sizeof(LastError));
					//�¹�Լ��ӡ
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
						sprintf_s(LastError,sizeof(LastError),"�趨��ʱ��Ķ̸���ֵ������ֹ����Ϣ���ַ=%d,ֵ=%f\n",adr,fval);
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
						sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵ������ֹ����Ϣ���ַ=%d,ֵ=%d  ****\n",adr,val);
#endif						
					}
#ifdef _DEBUG
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//����
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ʧ��ʱ�䣬������ֹ
					}
				}
				/*by fanshuo end*/
				else
				{
#ifdef _DEBUG
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d �յ�ChannelID=%d��device=%d,no=%d��ң��ִ�����ȷ�ϣ�����ң�������б��в����ڣ�����\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

					//�¹�Լ��ӡ
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
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ʧ�ܣ���Ϣ���ַ=%d��ֵ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
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
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ʧ�ܣ���Ϣ���ַ=%d��ֵ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),pcmd);
				//�¹�Լ��ӡ
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

					if (bExist)//����
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							curSetPoint.exectime = time;
							curSetPoint.iexecret = 3;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ʧ��ʱ��
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

				if (bExist)//����
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ʧ��ʱ��
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
			//����ȷ��
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
					sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
					//OnShowMsg(LastError,sizeof(LastError));
					//�¹�Լ��ӡ
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
					sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//����
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
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң���ɹ�ʱ���һ��ֵ

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
				sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
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
				sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//�¹�Լ��ӡ
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

				if (bExist)//����
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 1;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ�гɹ�ʱ��-ֱ��ģʽ
					}
				}
				/*by fanshuo end*/

			}
			m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
		}
		else 
		{
			//������47֮���
			//  <44> ���� δ֪�����ͱ�ʶ
			//	<45> ���� δ֪�Ĵ���ԭ��
			//	<46> ���� δ֪��Ӧ�÷������ݵ�Ԫ������ַ
			//	<47> ���� δ֪����Ϣ�����ַ

			//δִ�гɹ�
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
				sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
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
				sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//�¹�Լ��ӡ
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

				if (bExist)//����
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						//GetLocalTime(time);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ʧ��ʱ��-ֱ��ģʽ
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

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_SE3 *ps = ( Q_IEC104_ST_C_SE3 *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;

	if(m_cfgControl->m_YtExecMode==eYtExecModeSelect)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//����ɹ�
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkSelectCon,adr);

			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					float val = 0;
					memcpy(&val,ps->val,sizeof(float));
					sprintf_s(LastError,sizeof(LastError),"****  �趨�̸���ֵѡ��ȷ�ϣ���Ϣ���ַ=%d,ֵ=%f  ****\n",adr,val);
					//OnShowMsg(LastError,sizeof(LastError));
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					float val = 0;
					memcpy(&val,ps->val,sizeof(float));
					sprintf_s(LastError,sizeof(LastError),"****  �趨�̸���ֵѡ��ȷ�ϣ���Ϣ���ַ=%d,ֵ=%f  ****\n",adr,val);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//�¹�Լ��ӡ
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
						sprintf_s(LastError,sizeof(LastError),"****  �趨�̸���ֵ�ɹ�����Ϣ���ַ=%d,ֵ=%f ****\n",adr,val);
						//OnShowMsg(LastError,sizeof(LastError));
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if(true/*m_bInfoMsg*/)
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						float val = 0;
						memcpy(&val,ps->val,sizeof(float));
						sprintf_s(LastError,sizeof(LastError),"****  �趨�̸���ֵ�ɹ�����Ϣ���ַ=%d,ֵ=%f ****\n",adr,val);
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					//ִ�гɹ�
					m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);

				}
			}
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//δִ�гɹ�
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
					sprintf_s(LastError,sizeof(LastError),"****  �趨�̸���ֵ������ֹ����Ϣ���ַ=%d,ֵ=%f  ****\n",adr,val);
					//OnShowMsg(LastError,sizeof(LastError));
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					float val = 0;
					memcpy(&val,ps->val,sizeof(float));
					sprintf_s(LastError,sizeof(LastError),"****  �趨�̸���ֵ������ֹ����Ϣ���ַ=%d,ֵ=%f  ****\n",adr,val);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//�¹�Լ��ӡ
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
						sprintf_s(LastError,sizeof(LastError),"****  �趨����ʧ�ܣ���Ϣ���ַ=%d,ֵ=%f\n  ****",adr,val);
						//OnShowMsg(LastError,sizeof(LastError));
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if(true/*m_bInfoMsg*/)
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						float val = 0;
						memcpy(&val,ps->val,sizeof(float));
						sprintf_s(LastError,sizeof(LastError),"****  �趨����ʧ�ܣ���Ϣ���ַ=%d,ֵ=%f\n  ****",adr,val);
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//�¹�Լ��ӡ
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
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%f��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
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
					sprintf_s(LastError,sizeof(LastError),"****  �趨����ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%f��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
					//OnInfoMsg(LastError,sizeof(LastError),pcmd);
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				else
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					float val = 0;
					memcpy(&val,ps->val,sizeof(float));
					sprintf_s(LastError,sizeof(LastError),"****  �趨����ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%f��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
					//OnInfoMsg(LastError,sizeof(LastError),pcmd);
					//�¹�Լ��ӡ
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

					if (bExist)//����
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							//GetLocalTime(time);
							curSetPoint.exectime = time;
							curSetPoint.iexecret = 3;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ʧ��ʱ��-���ø���ֵ
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

				if (bExist)//����
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						//GetLocalTime(time);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ��ʧ��ʱ��
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
			//����ȷ��
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

						//�¹�Լ��ӡ
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

				if (bExist)//����
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
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң���ɹ�ʱ���һ��ֵ

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
			//δִ�гɹ�

			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"Execute command term success.Mode Exec.Command info:addr=%d\n",adr);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
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
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
		}
		else 
		{
			//������47֮���
			//<44> ���� δ֪�����ͱ�ʶ
			//	<45> ���� δ֪�Ĵ���ԭ��
			//	<46> ���� δ֪��Ӧ�÷������ݵ�Ԫ������ַ
			//	<47> ���� δ֪����Ϣ�����ַ


			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"****  �趨�̸���ֵʧ�ܣ���Ϣ���ַ=%d��ԭ��ʮ���ƣ�=%d\n",adr,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(true/*m_bInfoMsg*/)
			{
				Q_cmdIEC104*pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
#ifdef _DEBUG				
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"****  �趨�̸���ֵʧ�ܣ���Ϣ���ַ=%d��ԭ��ʮ���ƣ�=%d\n",adr,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),pcmd);
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			//δִ�гɹ�
			m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);
		}
	}
	return ret;
}




int Q_protocolIEC104Master::func_C_SC_TA_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;
	DWORD adr;//�����ַ

	Q_IEC104_ST_C_SC_TA1 *ps = ( Q_IEC104_ST_C_SC_TA1 *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;

	if(m_cfgControl->m_YkExecMode==eYtExecModeSelect)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//����ɹ�

			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkSelectCon,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵѡ��ȷ�ϣ���Ϣ���ַ=%d��ֵ=%d\n  ****",adr,val);
					//OnShowMsg(LastError,sizeof(LastError));
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵѡ��ȷ�ϣ���Ϣ���ַ=%d��ֵ=%d\n  ****",adr,val);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//�¹�Լ��ӡ
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

				if (bExist)//����
				{
					if ((YKID32>0) && (&curRelay!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curRelay.selecttime = time;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʱ��
					}
				}	
				/*by fanshuo end*/
				else
				{
#ifdef _DEBUG
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d �յ�ChannelID=%d��device=%d,no=%d��ң��ѡ�񷵻�ȷ�ϣ�����ң�������б��в����ڣ�����\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

					/*//ԭCommApp��Լ��ӡ���桢�ȱ�����
					//OnShowMsg(LastError,sizeof(LastError));
					if(true)
					{
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					}
					*/

					//�¹�Լ��ӡ
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
						sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
						//OnShowMsg(LastError,sizeof(LastError));
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if(true/*m_bInfoMsg*/)
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						int val=ps->sco.scs;
						sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}


					/*by fanshuo*/
					bool bExist = false;
					int	YKID32 = -1;
					int YKtoYXID32 = -1;
					SCADA_Relay curRelay;
					bExist = FindYK(YKtoYXID32,YKID32,curRelay,(Q_cmdIEC104_SP *)cmd);

					if (bExist)//����
					{
						if ((YKID32>0) && (&curRelay!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							GetLocalTime(time);
							curRelay.exectime = time;
							curRelay.iexecret = 1;
							dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʱ��
						}
					}	
					/*by fanshuo end*/

					m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkExec,adr);
				}
			}

		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//δִ�гɹ�
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
					//�¹�Լ��ӡ
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
					//�¹�Լ��ӡ
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

				if (bExist)//����
				{
					if ((YKID32>0) && (&curRelay!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						curRelay.selecttime = time;
						curRelay.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʱ��
					}
				}	
				/*by fanshuo end*/

				m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkSelectCon,adr);
			}
		}
		else
		{
			//������47֮���
			//<44> ���� δ֪�����ͱ�ʶ
			//	<45> ���� δ֪�Ĵ���ԭ��
			//	<46> ���� δ֪��Ӧ�÷������ݵ�Ԫ������ַ
			//	<47> ���� δ֪����Ϣ�����ַ

			//δִ�гɹ�
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

			if (bExist)//����
			{
				if ((YKID32>0) && (&curRelay!=NULL))
				{
					CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
					SYSTEMTIME time = {0};
					curRelay.selecttime = time;
					curRelay.iexecret = 3;
					dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʱ��
				}
			}	
			/*by fanshuo end*/


			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
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
					sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
					//OnInfoMsg(LastError,sizeof(LastError),pcmd);
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				else
				{
					pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
					//OnInfoMsg(LastError,sizeof(LastError),pcmd);
					//�¹�Լ��ӡ
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
			//����ȷ��
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
					//OnShowMsg(LastError,sizeof(LastError));
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					int val=ps->sco.scs;
					sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//�¹�Լ��ӡ
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

				if (bExist)//����
				{
					if ((YKID32>0) && (&curRelay!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curRelay.exectime = time;
						curRelay.iexecret = 1;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʱ��
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
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ�������趨�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);

#endif
			}
			if(true/*m_bInfoMsg*/)
			{
#ifdef _DEBUG
				Q_cmdIEC104 *pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExecAct,adr);
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ�������趨�ɹ�����Ϣ���ַ=%d��ֵ=%d  ****\n",adr,val);
				//OnInfoMsg(LastError,sizeof(LastError),pcmd);
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			m_listCmd->removeCommand(eCmdTypeC_SC,eCmdStateYkExecAct,adr);
		}
		else 
		{
			//������47֮���
			//<44> ���� δ֪�����ͱ�ʶ
			//	<45> ���� δ֪�Ĵ���ԭ��
			//	<46> ���� δ֪��Ӧ�÷������ݵ�Ԫ������ַ
			//	<47> ���� δ֪����Ϣ�����ַ

			//δִ�гɹ�
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

			if (bExist)//����
			{
				if ((YKID32>0) && (&curRelay!=NULL))
				{
					CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
					SYSTEMTIME time = {0};
					curRelay.exectime = time;
					curRelay.iexecret = 3;
					dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʱ��
				}
			}
			/*by fanshuo end*/

			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}

			if(true/*m_bInfoMsg*/)
			{
#ifdef _DEBUG
				Q_cmdIEC104 *pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkExec,adr);
				memset(LastError,0,sizeof(LastError));
				int val=ps->sco.scs;
				sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵʧ�ܣ���Ϣ���ַ=%d��ֵ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,val,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),pcmd);
				//�¹�Լ��ӡ
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

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_SE_TC1 *ps = ( Q_IEC104_ST_C_SE_TC1 *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;

	if(m_cfgControl->m_YtExecMode==eYtExecModeSelect)
	{
		if (msg->cause== CMD_ACT_CON)
		{
			//����ɹ�
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkSelectCon,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��Ķ̸���ֵѡ��ȷ�ϣ���Ϣ���ַ=%d��ֵ=%f  ****\n",adr,pcmd->m_fVal);
					//OnShowMsg(LastError,sizeof(LastError));
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��Ķ̸���ֵѡ��ȷ�ϣ���Ϣ���ַ=%d��ֵ=%f  ****\n",adr,pcmd->m_fVal);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//�¹�Լ��ӡ
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

				if (bExist)//����
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.selecttime = time;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ��ʱ��
					}
				}
				/*by fanshuo end*/
				else
				{
#ifdef _DEBUG
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d �յ�ChannelID=%d��device=%d,no=%d��ң��ѡ�񷵻�ȷ�ϣ�����ң�������б��в����ڣ�����\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

					/*//ԭCommApp��Լ��ӡ���桢�ȱ�����
					//OnShowMsg(LastError,sizeof(LastError));
					if(true)
					{
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					}
					*/
					//�¹�Լ��ӡ
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
						sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��Ķ̸���ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%f  ****\n",adr,pcmd->m_fVal);
						//OnShowMsg(LastError,sizeof(LastError));
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if(true/*m_bInfoMsg*/)
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
						sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��Ķ̸���ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%f  ****\n",adr,pcmd->m_fVal);
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}

					/*by fanshuo*/
					bool bExist = false;
					int	YTID32 = -1;
					int YTtoYCID32 = -1;
					SCADA_SetPoint curSetPoint;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

					if (bExist)//����
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							GetLocalTime(time);
							curSetPoint.exectime = time;
							curSetPoint.iexecret = 1;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ��ʱ��
						}
					}
					/*by fanshuo end*/

					m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExec,adr);
				}
			}
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//������ֹ
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
					//�¹�Լ��ӡ
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				/*by fanshuo*/
				bool bExist = false;
				int	YTID32 = -1;
				int YTtoYCID32 = -1;
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,(Q_cmdIEC104_SE*)cmd);

				if (bExist)//����
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 1;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ��ʱ��
					}
				}
				/*by fanshuo end*/
				else
				{
#ifdef _DEBUG
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d �յ�ChannelID=%d��device=%d,no=%d��ң��ִ�����ȷ�ϣ�����ң�������б��в����ڣ�����\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

					/*//ԭCommApp��Լ��ӡ���桢�ȱ�����
					//OnShowMsg(LastError,sizeof(LastError));
					if(true)
					{
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					}
					*/

					//�¹�Լ��ӡ
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
				sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��ĸ���ֵʧ�ܣ���Ϣ���ַ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
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
					sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��ĸ���ֵʧ�ܣ���Ϣ���ַ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,msg->cause);
					//OnInfoMsg(LastError,sizeof(LastError),pcmd);
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				else
				{
#ifdef _DEBUG
					pcmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
					memset(LastError,0,sizeof(LastError));
					sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��ĸ���ֵʧ�ܣ���Ϣ���ַ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,msg->cause);
					//OnInfoMsg(LastError,sizeof(LastError),pcmd);
					//�¹�Լ��ӡ
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

					if (bExist)//����
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							GetLocalTime(time);
							curSetPoint.exectime = time;
							curSetPoint.iexecret = 3;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ��ʱ��
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

				if (bExist)//����
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.selecttime = time;
						curSetPoint.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ��ʱ��
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
			//����ȷ��
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
			if(cmd!=NULL)
			{
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��ĸ���ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%f  ****\n",adr,pcmd->m_fVal);
					//OnShowMsg(LastError,sizeof(LastError));
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					memset(LastError,0,sizeof(LastError));
					Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
					sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��ĸ���ֵ�ɹ�����Ϣ���ַ=%d��ֵ=%f  ****\n",adr,pcmd->m_fVal);
					//OnInfoMsg(LastError,sizeof(LastError),cmd);
					//�¹�Լ��ӡ
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

				if (bExist)//����
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 1;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ��ʱ��
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
				//�¹�Լ��ӡ
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
				//�¹�Լ��ӡ
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

				if (bExist)//����
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 1;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ��ʱ��
					}
				}
				/*by fanshuo end*/
			}
			m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
		}
		else 
		{

			//δִ�гɹ�
			Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkExec,adr);
			if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��ĸ���ֵʧ�ܣ���Ϣ���ַ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,msg->cause);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}
			if(true/*m_bInfoMsg*/)
			{
#ifdef _DEBUG
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��ĸ���ֵʧ�ܣ���Ϣ���ַ=%d��ԭ��ʮ���ƣ�=%d  ****\n",adr,msg->cause);
				//OnInfoMsg(LastError,sizeof(LastError),cmd);
				//�¹�Լ��ӡ
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

				if (bExist)//����
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						SYSTEMTIME time = {0};
						GetLocalTime(time);
						curSetPoint.exectime = time;
						curSetPoint.iexecret = 3;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ��ʱ��
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

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_RC *ps = ( Q_IEC104_ST_C_RC *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);


	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;


	if (eStation_Slave==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT) //����
		{
			Q_cmdIEC104_SP *cmd=NULL;

			if (1==ps->rco.se)//ѡ��
			{
				Q_cmdIEC104_RC *cmd=new Q_cmdIEC104_RC;

				Q_dataManager* p = _GlobalData.getDataManager();
				LPQ_ANALOGUEDATATAG alg=NULL;
				if(m_tags!=NULL)
					p->findAlgTag(msg->asdu,adr,m_tags->getAnalogBegin(),m_tags->getAnalogEnd());

				if (alg!=NULL)/*��ѯ��������ݴ���*/
				{
					cmd->m_eState=eCmdStateYkSelect;
					cmd->m_nTxCause=msg->cause;
					cmd->m_nComAsdu=msg->asdu;
					cmd->m_bExec=false;


					cmd->m_cOnOff=ps->rco.rcs&0xFF;

					cmd->m_cQuality=ps->rco.qu;

					//���ڷ���ȷ�ϼ������
					memcpy((void *)&cmd->m_nValue,(void *)&ps->rco,sizeof(cmd->m_nValue));
					cmd->m_nInfoAddr=adr;//��Ϣ�����ַ

				}
				else
				{
					//����в�����
					cmd->m_bError=true;
					m_errorNo=e104ErrNo_P_UnExist; //��ð���ϸ�����¼һ�¡�
#ifdef _DEBUG
					sprintf_s(LastError,sizeof(LastError), "��Ϣ�����ַ����Ӧ�ĵ�Ų����ڣ���ǰ��ַΪ%d\n",adr);
					//OnErrorMsg(m_errorNo, LastError);
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}

				m_listCmd->addCommand(cmd);//������ѹջ
			}
			else if (0==ps->rco.se)
			{
				//ִ��
				//���ݵ�š���������Ϊ������ �Ȳ�ѯ����
				Q_cmdIEC104 *v_cmd=NULL;
				//����״ֵ̬��ѯ����
				v_cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkConfirm,adr);
				if (v_cmd!=NULL)
				{
					//����״̬
					Q_cmdIEC104_RC *cmd=(Q_cmdIEC104_RC *)v_cmd;
					cmd->m_bExec=false;
					cmd->m_eState=eCmdStateYkExec;
				}
				else
				{
					m_errorNo=e104ErrNo_CmdNoSel;
#ifdef _DEBUG
					sprintf_s(LastError,sizeof(LastError), "����ִ�е���ң�������ַ����������豸û��ѡ�񡣵�ǰ�к�:%d\n",__LINE__);
					//OnErrorMsg(m_errorNo, LastError);
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
					return Q_104FAIL;
				}
			}

		}
		else if (msg->cause == CMD_DEACT) //ֹͣ����
		{
			//��������
			Q_cmdIEC104 *v_cmd=NULL;
			//����״ֵ̬��ѯ����
			v_cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkConfirm,adr);
			if (v_cmd!=NULL)
			{
				//����״̬
				Q_cmdIEC104_RC *cmd=(Q_cmdIEC104_RC *)v_cmd;
				cmd->m_bExec=false;
				cmd->m_eState=eCmdStateYkCancelAct;
			}
			else
			{

				m_errorNo=e104ErrNo_CmdNoSel;
#ifdef _DEBUG
				sprintf_s(LastError,sizeof(LastError), "����ȡ������ң�������ַ����������豸û��ѡ�񡣵�ǰ�к�:%d\n",__LINE__);
				//OnErrorMsg(m_errorNo, LastError);
				//�¹�Լ��ӡ
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
			//ѡ��ȷ��
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//��ִ����ɣ����ǳ������
		}
	}

	return ret;

}

int Q_protocolIEC104Master::func_C_CS(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_CS *ps = ( Q_IEC104_ST_C_CS *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);


	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;


	if (eStation_Slave==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT) //����
		{
			Q_cmdIEC104_CS *cmd=new Q_cmdIEC104_CS;
			cmd->setCommand104Type(eCmdTypeC_SC);//����ң��

			cmd->m_eState=eCmdStateSync;
			cmd->m_nTxCause=msg->cause;
			cmd->m_nComAsdu=msg->asdu;
			cmd->m_bExec=false;
			cmd->m_nInfoAddr=adr;//��Ϣ�����ַ
			cmd->m_nPointNo=0;//TODO:����е��
			memcpy(cmd->m_cTime,ps->val,sizeof(cmd->m_cTime));

			m_listCmd->addCommand(cmd);//������ѹջ
		}
	}
	else if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT_CON)
		{
			//ѡ��ȷ��
		}
		else if (msg->cause ==CMD_DEACT_TERM)
		{
			//��ִ����ɣ����ǳ������
		}
	}
	return ret;

}

int Q_protocolIEC104Master::func_Interrogate(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	//��ѯ ����ԭ��ΪCMD_ACT,����Ϊ���٣���δ����ģ���
	//Interrogation Command (Group %d): Activation Confirmation.

	sprintf_s(LastError,sizeof(LastError),"execute Interrogate begin ... \n");
	//OnShowMsg(LastError,sizeof(LastError));n

	list<Q_IEC104StandardCommand *> v_listCmd;
	int len=m_listCmd->getCommandByStatus(v_listCmd);//���޴���������б�
	if (len>0)
	{
		for (IT_CmdListP it=v_listCmd.begin();it!=v_listCmd.end();it++)
		{
			Q_cmdIEC104 *cur_cmd=(Q_cmdIEC104 *)*it;
			if(cur_cmd!=NULL)
			{
				if(cur_cmd->getCommand104Type()==eCmdTypeTestFR)
					continue;

				//��ѯ��ǰδ����ģ���״ֵ̬ҲΪδ���������
				if (cur_cmd->m_eState!=eCmdStateEnd && cur_cmd->m_eState!=eCmdStateNull)
				{
					//��������
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
								cur_cmd->m_eState=eCmdStateIntRequestAct;//����ȷ�����
							}
							m_LinkData.nRecvCount=1;
						}
						else if (msg->cause ==CMD_DEACT_TERM)
						{
							//��ѯ״̬����״ֵ̬
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
	DWORD adr;//�����ַ
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

		pos +=2;//��ַ��
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
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
	unsigned long adr;//�����ַ
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
	unsigned long adr;//�����ַ
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
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
	DWORD adr;//�����ַ
	pos = sizeof(Q_IEC104_ASDU_header);

	if (msg->seq) //����
	{
		//�����Ǽ���

		adr = msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos];
		//adr = msg->msgbuf[pos+2] | msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos]<<16;

		pos += 2;
		while (pos < msg->len)
		{
			Q_MS104Seq *pf=(Q_MS104Seq *)&(msg->msgbuf[pos]);
			short val=0;

			//Ĭ�ϵ��ֽ���ǰ
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
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
		//������
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

			//�������;
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
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
	DWORD adr;//�����ַ
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq) //˳���
	{
		//printf("--------Q_protocolIEC104Master::func_M_ME_NC_1-----222-----\n");
		//adr = msg->msgbuf[pos+2]<<16 | msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos];
		//��ϢԪ�ص�ַΪ2���ֽ�
		adr = msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos];
		pos+=2;
		while (pos < msg->len)
		{
			Q_MF104Seq *pf=(Q_MF104Seq *)&(msg->msgbuf[pos]);

			BYTE tmp[4];
			float val =0;
			if(m_cfgBase->m_eFloatSeq==eFloatSeqBig)
			{

				tmp[0]=pf->val[3];//�Ӹߵ���
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


			//�������;
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
			//ϵ��ͳһ��recordDataValue���������� comment by tjs 2016-06-21
			if(!m_pMgr->recordDataValue(&dataArray,1))
			{
				if(m_ProSetting!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
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



			//�������;
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
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
	sprintf_s(LastError,"�����˳����ͱ�ʶΪ0x13���ݽ���,��֡����%d��ң�����ݣ��ɹ�������%d\n",msg->num,suc_count);
#endif
	if (msg->num==suc_count)
	{
#ifdef _DEBUG
		//OnShowMsg(LastError,sizeof(LastError));
		//�¹�Լ��ӡ
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
		if(m_bNomalInfoMsg)
		{
			//OnNomalInfoMsg(LastError,sizeof(LastError));
			//�¹�Լ��ӡ
			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
		}
	}
	else
	{
#ifdef _DEBUG

		//�¹�Լ��ӡ
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);

#endif
	}
}

void Q_protocolIEC104Master::func_M_ME_ND_1(Q_104ApduEx *msg)
{
	UINT pos;
	DWORD adr;//�����ַ
	pos = sizeof(Q_IEC104_ASDU_header);

	if (msg->seq) //����
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
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
		//������
		while (pos < msg->len)
		{
			Q_MS104_21 *pf = (Q_MS104_21*)&(msg->msgbuf[pos]);
			adr =  pf->addr1<<8 | pf->addr0;
			short val=(pf->seqv.val[1]<<8) | pf->seqv.val[0];
			//�������;
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
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
	DWORD adr;//�����ַ
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq) //˳���
	{

		adr = msg->msgbuf[pos+2]<<16 | msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos];


		pos+=3;
		while (pos < msg->len)
		{
			Q_MF104Seq *pf=(Q_MF104Seq *)&(msg->msgbuf[pos]);
			int *val = (int *)&pf->val[0]; //����ø�һ��

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
	DWORD adr;//�����ַ
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq) //����
	{
		//�����Ǽ���
		adr = msg->msgbuf[pos+2]<<16 | msg->msgbuf[pos+1]<<8 | msg->msgbuf[pos];

		pos += 3;
		while (pos < msg->len)
		{
			Q_M_ST104Seq *pf=(Q_M_ST104Seq *)&(msg->msgbuf[pos]);
			int val=0;
			if (pf->val&0x40)
			{
				//����64
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
		//������
		while (pos < msg->len)
		{
			Q_M_ST104 *pf = (Q_M_ST104 *)&(msg->msgbuf[pos]);


			adr = pf->addr2<<16 | pf->addr1<<8 | pf->addr0;

			int val;
			if (pf->mst.val&0x40)
			{
				//����64
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
	unsigned long adr;//�����ַ
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq) //˳���
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
					st.sco.se=1;//ѡ��

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

					if (bExist)//����
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
							dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʱ��
						}
					}
					/*by fanshuo end*/
					else
					{
#ifdef _DEBUG
						SYSTEMTIME st;
						GetLocalTime(st);
						sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d ����ChannelID=%d��device=%d,no=%d��ң��ѡ��ʱ����ң�������б��в����ڣ�����\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
							m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

						/*//ԭCommApp��Լ��ӡ���桢�ȱ�����
						//OnShowMsg(LastError,sizeof(LastError));
						if(true)
						{
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						}
						*/
						//�¹�Լ��ӡ
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
						st.sco.se=1;//ѡ��
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
						st.sco.se=1;//ѡ��

						sendYkCommand(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}



					cmd->m_cQuality=0;
					v_cmd->m_eState=eCmdStateYkSelectCon;
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						if (FuncCode == 0x3A)
						{
							sprintf_s(LastError,sizeof(LastError),"****  ��ʱ�굥��ң�أ�ģʽ��ѡ�񣬹�����ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnShowMsg(LastError,sizeof(LastError));
							//�¹�Լ��ӡ
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						} 
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ��ģʽ��ѡ�񣬹�����ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnShowMsg(LastError,sizeof(LastError));
							//�¹�Լ��ӡ
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						}
					}
					if(true/*m_bInfoMsg*/)
					{
						memset(LastError,0,sizeof(LastError));
						if (FuncCode == 0x3A)
						{
							sprintf_s(LastError,sizeof(LastError),"****  ��ʱ�굥��ң�أ�ģʽ��ѡ�񣬹�����ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//�¹�Լ��ӡ
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						} 
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ��ģʽ��ѡ�񣬹�����ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//�¹�Լ��ӡ
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
					st.sco.se=0;//ִ��

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

					if (bExist)//����
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

							dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʱ��


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
									//2016-02-23�޸�Ϊֱ��ʹ����������
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
						sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d ����ChannelID=%d��device=%d,no=%d��ң��ȷ��ִ��ʱ������ң�������б��в����ڣ�����\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
							m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

						/*//ԭCommApp��Լ��ӡ���桢�ȱ�����
						//OnShowMsg(LastError,sizeof(LastError));
						if(true)
						{
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						}
						*/

						//�¹�Լ��ӡ
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
						st.sco.se=0;//ִ��
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
						st.sco.se=0;//ִ��

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
							sprintf_s(LastError,sizeof(LastError),"****  ��ʱ�굥��ң�أ�ģʽ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnShowMsg(LastError,sizeof(LastError));
							//�¹�Լ��ӡ
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						} 
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ��ģʽ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnShowMsg(LastError,sizeof(LastError));
							//�¹�Լ��ӡ
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						}
					}
					if(true/*m_bInfoMsg*/)
					{
						memset(LastError,0,sizeof(LastError));
						if (FuncCode == 0x3A)
						{
							sprintf_s(LastError,sizeof(LastError),"****  ��ʱ�굥��ң�أ�ģʽ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//�¹�Լ��ӡ
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						} 
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ��ģʽ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//�¹�Լ��ӡ
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
					//�¹�Լ��ӡ
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}

				//fs20150811
				cmd = (Q_cmdIEC104_SP *)v_cmd;
				Q_RECORDDATATAG ykRec;
				ykRec.eDataType=eDataTypeFloat;
				ykRec.channelID=m_ProSetting->getChannelID();
				ykRec.device=cmd->m_nComAsdu;
				ykRec.no=cmd->m_nInfoAddr;
				ykRec.fValue=float(cmd->m_cOnOff>=1?1:0);//����ֵ

				if(!m_pMgr->getYtValue(&ykRec))
				{
					sprintf_s(LastError,sizeof(LastError),"Send YK command faild,did not find config in yk.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					/*//ԭCommApp��Լ��ӡ���桢�ȱ�����
					//OnShowMsg(LastError,sizeof(LastError));
					if (m_bInfoMsg)
					{
					sprintf_s(LastError,sizeof(LastError),"Send YK command faild,did not find config in yk.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					//	OnInfoMsg(LastError,strlen(LastError)+1);
					}
					*/
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);

					return false;
				}

				if(m_pMgr->handleMasterYk(&ykRec)!=0)
				{
					/*//ԭCommApp��Լ��ӡ���桢�ȱ�����
					sprintf_s(LastError,sizeof(LastError),"Send YK command faild,did not find config.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					//OnShowMsg(LastError,sizeof(LastError));
					if (m_bInfoMsg)
					{
					sprintf_s(LastError,sizeof(LastError),"Send YK command faild,did not find config.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					//	OnInfoMsg(LastError,strlen(LastError)+1);
					}
					*/

					//�¹�Լ��ӡ
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

				if (bExist)//����
				{
					if ((YKID32>0) && (&curRelay!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						curRelay.iexecret = 2;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʱ��
					}
				}
				/*by fanshuo end*/

				m_listCmd->removeCommand(v_cmd);
			}
			//��ʱ�޷���ɾ������
			break;
		}
	}
	else if(m_cfgControl->m_YkExecMode==eYtExecModeDirect)
	{//ֱ��ִ��
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

				//�¹�Լ��ӡ
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

				//�¹�Լ��ӡ
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

			if (bExist)//����
			{
				if ((YKID32>0) && (&curRelay!=NULL))
				{
					CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
					curRelay.iexecret = 2;
					dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʱ��
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

					if (bExist)//����
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

							dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʱ��-ֱ��ִ��ģʽ

							if (YKtoYXID32>0)
							{
								Q_RECORDDATATAG dataArray;
								SCADA_Digit tempDigit;
								dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID,YKtoYXID32,&tempDigit);
								if (&tempDigit != NULL)
								{
									//2016-02-23�޸�Ϊֱ��ʹ����������
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
						sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d ����ChannelID=%d��device=%d,no=%d��ң��ȷ��ִ��ʱ������ң�������б��в����ڣ�����\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
							m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

						/*
						OnShowMsg(LastError,sizeof(LastError));
						if(true)
						{
						OnInfoMsg(LastError,sizeof(LastError),cmd);
						}
						*/
						//�¹�Լ��ӡ
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
						st.sco.se=0;//ִ��
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
						st.sco.se=0;//ִ��

						sendYkCommand(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}

					cmd->m_cQuality=0;
					v_cmd->m_eState=eCmdStateYkExec;

					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						if (FuncCode == 0x3A)
						{
							sprintf_s(LastError,sizeof(LastError),"****  ��ʱ�굥��ң�أ�ģʽ��ֱ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnShowMsg(LastError,sizeof(LastError));
							//�¹�Լ��ӡ
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						} 
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ��ģʽ��ֱ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnShowMsg(LastError,sizeof(LastError));
							//�¹�Լ��ӡ
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						}
					}
					if(true/*m_bInfoMsg*/)
					{
						memset(LastError,0,sizeof(LastError));
						if (FuncCode == 0x3A)
						{
							sprintf_s(LastError,sizeof(LastError),"****  ��ʱ�굥��ң�أ�ģʽ��ֱ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//�¹�Լ��ӡ
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
						} 
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨����ң��ֵ��ģʽ��ֱ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//�¹�Լ��ӡ
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

	v_cmd->m_bExec=true;//����

	if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		int icmd = v_cmd->m_eState;
		switch (icmd)
		{
		case eCmdStateYkSelect:
			{
				//����ȷ��֡
				Q_cmdIEC104_DP *cmd =(Q_cmdIEC104_DP *)v_cmd;
				if (NULL != cmd)
				{
					/*
					if (sendYkResponse(cmd->m_nComAsdu,CMD_ACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2E,sizeof(cmd->m_nValue))==Q_104FAIL)
					{
					//���ͱ��ĳ���
					m_errorNo=e104Errno_CmdSend;
					v_cmd->m_eState=eCmdStateYkConfirm;
					v_cmd->m_bError=true;
					sprintf_s(LastError,sizeof(LastError), "����˫��ң��ѡ��ȷ����Ϣ������ǰ�к�:%d\n",__LINE__);
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
				//ִ��
				Q_cmdIEC104_DP *cmd = (Q_cmdIEC104_DP *)v_cmd;
				Q_RECORDDATATAG dataArray;

				dataArray.eDataType=eDataTypeInt;
				dataArray.nValue=(int)cmd->m_cOnOff;
				dataArray.nIndex=v_cmd->m_nPointNo;
				Q_dataManager* p = _GlobalData.getDataManager();

				//����ʵʱ��
				if (bRet=p->recordDataValue(&dataArray,1))
				{
					//���³ɹ�
					v_cmd->m_eState=eCmdStateYkConfirm;

					//ִ����� //TODO:
					//��ѯ���Ƿ񷵻�ִ�к��ֵ ���Ҫ�󷵻��򷵻�

					v_cmd->m_eState=eCmdStateEnd;

				}
				else
				{
					v_cmd->m_bError=true;
					v_cmd->m_eState=eCmdStateYkConfirm;
					m_errorNo=e104Errno_UptDb;
#ifdef _DEBUG
					sprintf_s(LastError,sizeof(LastError), "ִ��˫��ң�أ�����ʵʱ�������ǰ�к�:%d\n",__LINE__);
					//OnErrorMsg(m_errorNo, LastError);nn
#endif
					bRet=false;

				}

			}
			break;
		case eCmdStateYkCancelAct:
			{
				//ȡ������
				//����ȷ��֡
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
					sprintf_s(LastError,sizeof(LastError), "����˫��ң��ȡ��ȷ�ϳ�����ǰ�к�:%d\n",__LINE__);
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
				//��ʼ��·��֮�󷽿ɷ������ٱ���
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
			//���Բɼ�������
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
			//�ȴ��������ٷ�����
			//if(v_cmd->m_nInfoAddr>10)
			if(time_to_IC())
			{
				v_cmd->m_eState=eCmdStateIntRequest;
				//v_cmd->m_nInfoAddr=0;
				v_cmd->m_bExec=false;
			}
			v_cmd->setTestNumDefault();
			//Interrogation Command (Group %d): Activation Termination.
			//��Ҫ�ȴ�һ��ʱ�����ٻ�
			
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
				//��̬����ʱ�䣬����ϵͳʱ�� TODO:
				Q_cmdIEC104_CS *cmd = (Q_cmdIEC104_CS *)v_cmd;
				v_cmd->m_eState=eCmdStateSyncAct;//����ȷ�����
				if (cmd!=NULL)
				{
#ifdef WINDOWS_ENV
					Q_CP56Time2a t2(cmd->m_cTime);
					SetLocalTime(&(t2._GetSystemTime(m_cfgSyncClock->m_nFixUsecs)));//��̬���������ӡ�
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
	//�������ø��� m_cFuncCode=0x30

	v_cmd->m_bExec=true;//��Ȼ�
	Q_cmdIEC104_SE *cmd=NULL;

	if(m_cfgControl->m_YtExecMode==eYtExecModeSelect)
	{
		int icmd = v_cmd->m_eState;
		switch (icmd)
		{
		case eCmdStateYkSelect:
			{
				cmd = (Q_cmdIEC104_SE *)v_cmd;			
				//IEC104��վң��ϵ������
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

					if (bExist)//����
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							curSetPoint.exectime = time;

							if (cmd->m_cFuncCode==0x3F||cmd->m_cFuncCode==0x32)
							{//����
								curSetPoint.fVal = cmd->m_fVal;

							}
							else
							{
								curSetPoint.fVal = cmd->m_cOnOff;

							}

							GetLocalTime(time);
							curSetPoint.selecttime = time;
							curSetPoint.iexecret = 0;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ңң��ѡ��ʱ��
						}
					}
					/*by fanshuo end*/
					else
					{
#ifdef _DEBUG
						SYSTEMTIME st;
						GetLocalTime(st);
						sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d ����ChannelID=%d��device=%d,no=%d��ң��ѡ��ʱ����ң�ص������б��в����ڣ�����\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
							m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

						//�¹�Լ��ӡ
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

						st.qos.se=1;//ѡ��
						sendYtCommandFloatWithTime(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}
					else if (cmd->m_cFuncCode==0x32)
					{//����
						Q_IEC104_ST_C_SE3 st;
						memset(&st,0,sizeof(Q_IEC104_ST_C_SE3));
						st.addr0=cmd->m_nInfoAddr&0xFF;
						st.addr1=(cmd->m_nInfoAddr>>8) &0xFF;
						st.addr2=(cmd->m_nInfoAddr>>16) &0xFF;
						memcpy(st.val,&cmd->m_fVal,sizeof(float));


						st.qos.se=1;//ѡ��
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
							//�����ò����ʾ
							int n=~(abs(cmd->m_cOnOff))+1;

							st.val1= n& 0xFF;
							st.val2=(n>>8) &0x7F;
							st.s=1;
						}
						st.qos.se=1;//ѡ��
						sendYtCommand(v_cmd->m_nComAsdu,CMD_ACT,&st);
					}				

					cmd->m_cQuality=0;
					v_cmd->m_eState=eCmdStateYkSelectCon;
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						if (cmd->m_cFuncCode==0x3F)
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��Ķ̸���ֵ��ģʽ��ѡ�񣬹�����ַ=%d����Ϣ���ַ=%d��ֵ=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else if (cmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨�̸���ֵ��ģʽ��ѡ�񣬹�����ַ=%d����Ϣ���ַ=%d��ֵ=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵ��ģʽ��ѡ�񣬹�����ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						//OnShowMsg(LastError,sizeof(LastError));
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
					}
					if(true/*m_bInfoMsg*/)
					{
						memset(LastError,0,sizeof(LastError));
						if (cmd->m_cFuncCode==0x3F)
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��Ķ̸���ֵ��ģʽ��ѡ�񣬹�����ַ=%d����Ϣ���ַ=%d��ֵ=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else if (cmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨�̸���ֵ��ģʽ��ѡ�񣬹�����ַ=%d����Ϣ���ַ=%d��ֵ=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵ��ģʽ��ѡ�񣬹�����ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
					}
				}
			}
			break;
		case eCmdStateYkConfirm:
			{
				cmd = (Q_cmdIEC104_SE *)v_cmd;
				//IEC104��վң��ϵ������
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

					if (bExist)//����
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							SYSTEMTIME time = {0};
							if (cmd->m_cFuncCode==0x3F||cmd->m_cFuncCode==0x32)
							{//����
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
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ��ʱ��

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
						sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d ����ChannelID=%d��device=%d,no=%d��ң��ѡ��ִ��ʱ����ң�������б��в����ڣ�����\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
							m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

						/*
						//OnShowMsg(LastError,sizeof(LastError));
						if(true)
						{
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						}
						*/
						//�¹�Լ��ӡ
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



						st.qos.se=0;//ִ��
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
							//�����ò����ʾ
							int n=~(abs(cmd->m_cOnOff))+1;

							st.val1= n& 0xFF;
							st.val2=(n>>8) &0x7F;
							st.s=1;
						}
						st.qos.se=0;//ִ��
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
							sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��Ķ̸���ֵ��ģʽ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else if (cmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨�̸���ֵ��ģʽ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵ��ģʽ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						//OnShowMsg(LastError,sizeof(LastError));
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
					}
					if(true/*m_bInfoMsg*/)
					{
#ifdef _DEBUG
						memset(LastError,0,sizeof(LastError));
						if (cmd->m_cFuncCode==0x3F)
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��Ķ̸���ֵ��ģʽ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else if (cmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨�̸���ֵ��ģʽ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵ��ģʽ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						//OnInfoMsg(LastError,sizeof(LastError),cmd);
						//�¹�Լ��ӡ
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
					//IEC104��վң��ϵ������
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}
				if(true/*m_bInfoMsg*/)
				{
#ifdef _DEBUG
					cmd = (Q_cmdIEC104_SE *)v_cmd;
					//IEC104��վң��ϵ������
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
				}



				Q_cmdIEC104_SE *pcmd = (Q_cmdIEC104_SE *)cmd;
				Q_RECORDDATATAG ytRec;
				ytRec.eDataType=eDataTypeFloat;
				ytRec.channelID=m_ProSetting->getChannelID();
				ytRec.device=cmd->m_nComAsdu;
				ytRec.no=cmd->m_nInfoAddr;
				ytRec.fValue=pcmd->m_fVal;//����ֵ

				if(!m_pMgr->getYtValue(&ytRec))
				{
					sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);

					/*//ԭCommApp��Լ��ӡ���桢�ȱ�����
					//OnShowMsg(LastError,sizeof(LastError));
					if (m_bInfoMsg)
					{
					sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config in yt.xml.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					//	OnInfoMsg(LastError,strlen(LastError)+1);
					}
					*/

					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
					return false;
				}

				if(m_pMgr->handleMasterYt(&ytRec)!=0)
				{
					sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					/*//ԭCommApp��Լ��ӡ���桢�ȱ�����
					OnShowMsg(LastError,sizeof(LastError));
					if (m_bInfoMsg)
					{
					sprintf_s(LastError,sizeof(LastError),"Send YT command faild,did not find config.device:%d register : %d ",cmd->m_nComAsdu,cmd->m_nInfoAddr);
					OnInfoMsg(LastError,strlen(LastError)+1);
					}
					*/
					//�¹�Լ��ӡ
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

				if (bExist)//����
				{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
						CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
						CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
						curSetPoint.iexecret = 2;
						dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ��ʱ��
					}
				}
				/*by fanshuo end*/

				m_listCmd->removeCommand(v_cmd);
			}
			//��ʱ�޷���ɾ������
			break;
		}
	}
	////////ֱ��ִ��ģʽ////////////////////////////////////////
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
				//IEC104��վң��ϵ������
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
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
			}

			if(true/*m_bInfoMsg*/)
			{
#ifdef _DEBUG
				cmd = (Q_cmdIEC104_SE *)v_cmd;
				//IEC104��վң��ϵ������
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
				//�¹�Լ��ӡ
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
				//IEC104��վң��ϵ������
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

					if (bExist)//����
					{
					if ((YTID32>0) && (&curSetPoint!=NULL))
					{
					CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
					SYSTEMTIME time = {0};
					curSetPoint.selecttime = time;

					if (cmd->m_cFuncCode==0x3F||cmd->m_cFuncCode==0x32)
					{//����
					curSetPoint.fVal = cmd->m_fVal;
					}
					else
					{
					curSetPoint.fVal = cmd->m_cOnOff;
					}

					GetLocalTime(time);
					curSetPoint.exectime = time;
					curSetPoint.iexecret = 1;
					dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ��ʱ��-ֱ��ִ��ģʽ

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
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d ����ChannelID=%d��device=%d,no=%d��ң��ѡ��ִ��ʱ����ң�������б��в����ڣ�����\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
					m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);

					//�¹�Լ��ӡ
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
							//�����ò����ʾ���������ǲ�����
							//�����ò����ʾ
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
							sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵ��ģʽ��ֱ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnShowMsg(LastError,sizeof(LastError));
							//�¹�Լ��ӡ
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
						}
						if(true/*m_bInfoMsg*/)
						{
#ifdef _DEBUG
							memset(LastError,0,sizeof(LastError));
							sprintf_s(LastError,sizeof(LastError),"****  �趨��һ��ֵ��ģʽ��ֱ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//�¹�Լ��ӡ
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
							sprintf_s(LastError,sizeof(LastError),"****  �趨�̸���ֵ��ģʽ��ֱ��ִ�У�������ַ=%d,��Ϣ���ַ=%d,ֵ=%.3f.\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
							//OnShowMsg(LastError,sizeof(LastError));
							//�¹�Լ��ӡ
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
						}
						if(true/*m_bInfoMsg*/)
						{
#ifdef _DEBUG
							memset(LastError,0,sizeof(LastError));
							sprintf_s(LastError,sizeof(LastError),"****  �趨�̸���ֵ��ģʽ��ֱ��ִ�У�������ַ=%d,��Ϣ���ַ=%d,ֵ=%.3f.\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//�¹�Լ��ӡ
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
							sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��Ķ̸���ֵ��ģʽ��ֱ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
							//OnShowMsg(LastError,sizeof(LastError));
							//�¹�Լ��ӡ
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
#endif
						}
						if(true/*m_bInfoMsg*/)
						{
#ifdef _DEBUG
							memset(LastError,0,sizeof(LastError));
							sprintf_s(LastError,sizeof(LastError),"****  �趨��ʱ��Ķ̸���ֵ��ģʽ��ֱ��ִ�У�������ַ=%d����Ϣ���ַ=%d��ֵ=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
							//OnInfoMsg(LastError,sizeof(LastError),cmd);
							//�¹�Լ��ӡ
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

	v_cmd->m_bExec=true;//����

	if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		int icmd = v_cmd->m_eState;
		switch (icmd)
		{
		case eCmdStateYkSelect:
			{
				//����ȷ��֡
				Q_cmdIEC104_RC *cmd = (Q_cmdIEC104_RC *)v_cmd;
				if (NULL != cmd)
				{
					//if (sendYkResponse(cmd->m_nComAsdu,CMD_ACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2F,sizeof(cmd->m_nValue))==Q_104FAIL)
					//{
					//    //���ͱ��ĳ���
					//    m_errorNo=e104Errno_CmdSend;
					//    v_cmd->m_eState=eCmdStateYkConfirm;
					//    v_cmd->m_bError=true;
					//    sprintf_s(LastError,sizeof(LastError), "������������,ѡ��ȷ�ϳ�����ǰ�к�:%d\n",__LINE__);
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
				//ִ��
				Q_cmdIEC104_RC *cmd = (Q_cmdIEC104_RC *)v_cmd;
				Q_RECORDDATATAG dataArray;

				dataArray.eDataType=eDataTypeInt;
				dataArray.nValue=cmd->m_cOnOff;
				dataArray.nIndex=v_cmd->m_nPointNo;
				Q_dataManager* p = _GlobalData.getDataManager();

				//����ʵʱ��
				//������Ҫ���´���һ�£���������Ľ���δ֪
				//	RCS  =0  ��������1  ����һ����2  ����һ����3  ��������
				if (1==1)
				{
					//���³ɹ�
					v_cmd->m_eState=eCmdStateYkConfirm;

					//ִ����� //TODO:
					//��ѯ���Ƿ񷵻�ִ�к��ֵ ���Ҫ�󷵻��򷵻�

					v_cmd->m_eState=eCmdStateEnd;

				}
				else
				{
					v_cmd->m_bError=true;
					v_cmd->m_eState=eCmdStateYkConfirm;
					m_errorNo=e104Errno_UptDb;
#ifdef _DEBUG
					sprintf_s(LastError, sizeof(LastError),"ִ�������������ʵʱ�������ǰ�к�:%d\n",__LINE__);
					//OnErrorMsg(m_errorNo, LastError);nn
#endif
					bRet=false;

				}

			}
			break;
		case eCmdStateYkCancelAct:
			{
				//ȡ������
				//����ȷ��֡
				Q_cmdIEC104_RC *cmd = (Q_cmdIEC104_RC *)v_cmd;
				if (NULL != cmd)
				{
					v_cmd->m_bError=true;
					//                int  nRet=sendYkResponse(cmd->m_nComAsdu,CMD_DEACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2F,sizeof(cmd->m_nValue));
					//                if (nRet==Q_104FAIL)
					//                {
					//                    m_errorNo=e104Errno_CmdSend;
					//#ifdef _DEBUG
					//                    sprintf_s(LastError,sizeof(LastError), "������������ȡ��ȷ�ϳ�����ǰ�к�:%d\n",__LINE__);
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
	msg.header.type =0x30;					// �趨����
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
	msg.header.type =0x3F;					// �趨����
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
	msg.header.type =0x32;					// �趨����
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
	msg.header.type =0x2D;					// ң������
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
	msg.header.type =0x3A;					// ң������
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
	DWORD adr;//�����ַ
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq)
	{
		Q_SP104 *ps = (Q_SP104*)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);
		adr =  ps->addr1<<8 | ps->addr0;

		pos +=2;//��ַ��
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError)," func_M_SP_TB_1 Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			pos+=(sizeof(Q_SIQ104)+7);//CP56Time2aʱ��վ7���ֽڣ���������
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"func_M_SP_TB_1 Can't find right YX point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			pos+=(sizeof(Q_SP104)+7);//CP56Time2aʱ��վ7���ֽڣ���������
		}
	}


}

void Q_protocolIEC104Master::func_M_ME_TF_1(Q_104ApduEx *msg)
{
	int suc_count=0;
	UINT pos;
	DWORD adr;//�����ַ
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq) //˳���
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			pos+=(sizeof(Q_MF104) +7);
		}

	}
#ifdef _DEBUG
	sprintf_s(LastError,sizeof(LastError),"�����˳����ͱ�ʶΪ0x24���ݽ���,��֡����%d��ң�����ݣ��ɹ�������%d\n",msg->num,suc_count);
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
	//�ж�ң�ص��Ƿ����
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

	if (nmsgtype == eMsgTypeTx || nmsgtype == eMsgTypeRx)//������Ϣ��������Ϣ
	{
		if (ElementTUnit.iRecPacketFlg != 0)//������Ϣ���俪��
		{
			bSend = true;
		}
		else
		{
			bSend = false;
			return;
		}
	}
	else if (nmsgtype == eMsgTypeMsg || nmsgtype == eMsgTypeError || nmsgtype == eMsgTypeCh)//��ʾ��Ϣ����Ϣֵ��������Ϣ�����ݽ��͡���Լ����
	{
		if (ElementTUnit.iRecPromptFlg != 0)//��ʾ��Ϣ���俪��
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
		//������ж���״̬�ͷ���������Ϣ���߸�������
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
				//���̲����У��˳�
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
				//2016-10-28 tjs ��Ϊ��Ӧ�ø�����������Ϣ


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
			//2016-10-28 tjs ��Ϊ��Ӧ�ø�����������Ϣ


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
	DWORD adr;//�����ַ
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq)
	{
		Q_DP104 *ps = (Q_DP104*)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);
		adr =  ps->addr1<<8 | ps->addr0;

		pos +=2;//��ַ��
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"func_M_DP_TB_1 Can't find right YX point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			pos+=(sizeof(Q_DIQ104)+7);//CP56Time2aʱ��վ7���ֽڣ���������
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
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if (m_bInfoMsg)
				{
					sprintf_s(LastError,sizeof(LastError),"func_M_DP_TB_1 Can't find right YX point in realDB,Channel=%d,device=%d,addr=%d",m_ProSetting->getChannelID(),msg->asdu,adr);
					//OnInfoMsg(LastError,strlen(LastError)+1);
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
			}
			//fs20150713

			pos+=(sizeof(Q_DP104)+7);//CP56Time2aʱ��վ7���ֽڣ���������
		}
	}

#ifdef _DEBUG
	sprintf_s(LastError,sizeof(LastError),"�����˳����ͱ�ʶΪ0x1E���ݽ���,��֡����%d��ң�����ݣ��ɹ�������%d\n",msg->num,suc_count);
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
