#include "stdafx.h"
#include "Q_ProtocolIEC104Slave.h"
#include "Q_IEC104CommFun.h"
#include "Q_104CP56Time2a.h"
#include "tinyxmlinclude.h"
//////
Q_dataManager *m_pMgrIEC104SlaveProtocolSetting; /*����IEC104Slave��rtdbȫ�ֱ���*/

Q_protocolIEC104Slave::Q_protocolIEC104Slave(void)
{
	m_nExecCmdMsgID=0;
	last_sel_cOnOff = 3;
	last_sel_nInfoAddr = -1;
	last_exe_cOnOff = 3;
	last_exe_nInfoAddr = -1;
	time(&exe_timestmp);
	time(&sel_timestmp);
	m_YtExeMode=eYtExecModeNull;
	ACsendno = 0;
	BCsendno = 0;
	bZongZhaoComplete = false;
}

Q_protocolIEC104Slave::~Q_protocolIEC104Slave(void)
{
	deleteVarData();
}

bool Q_protocolIEC104Slave::initializeProtocol()
{
	Q_protocolIEC104::initializeProtocol();
	initVaribleData();
	return true;
}


/*��˶���±�д201500801*/
/*��ʼ��ң��ң���������������ڱ仯����*/
void Q_protocolIEC104Slave::initVaribleData()
{
	m_vecVarYC.clear();
	m_vecVarYX.clear();

	bZongZhaoComplete = false;

	int nAnalogRecord = m_tags->getAnalogCount();
	CScadaDataBase *dbmg_sda=NULL;
	/*if(g_rtdbInfIEC104!=NULL)
	{	
	dbmg_sda=g_rtdbInfIEC104->getScadaDb();
	}*/
	//SCADA_Analog    *Analog;
	//SCADA_Digit   *digit;
	for(int i=0;i<nAnalogRecord;i++)
	{
		IEC104_VARIBLE_YC *yc=new IEC104_VARIBLE_YC;
		yc->id=m_tags->getAnalogBegin()+i;

		//2016-06-08 ����ת��������
		//if(dbmg_sda!=NULL)
		//{	
		//	if(dbmg_sda->GetTabAnalogByID(yc->id,Analog))
		//	{
		//		if(Analog!=NULL)
		//		yc->m_nZfID=Analog->iZfID;
		//	}
		////	BOOL	GetTabDigitByID( UINT iID, SCADA_Digit* & pDigit );
		//}

		yc->value=0;
		m_vecVarYC.push_back(yc);
	}

	int nDigitRecord = m_tags->getPointCount();
	for(int i=0;i<nDigitRecord;i++)
	{

		IEC104_VARIBLE_YX *yx=new IEC104_VARIBLE_YX;
		yx->id=m_tags->getPointBegin()+i;
		yx->value=false;

		//if(dbmg_sda!=NULL)
		//{	

		//	if(dbmg_sda->GetTabDigitByID(yx->id, digit))
		//	{
		//		if(digit!=NULL)
		//		yx->m_nZfID=digit->iZfID;
		//	}
		//	//	BOOL	GetTabDigitByID( UINT iID, SCADA_Digit* & pDigit );
		//}

		m_vecVarYX.push_back(yx);

		IEC104_VARIBLE_SOE *soe=new IEC104_VARIBLE_SOE;
		soe->id=m_tags->getPointBegin()+i;
		soe->value=false;
		soe->changed=false;
		m_vecVarSOE.push_back(soe);

	}
}


/*��ղ�ң������*/
void Q_protocolIEC104Slave::deleteVarData()
{	
	for(vector<IEC104_VARIBLE_YC *>::iterator it=m_vecVarYC.begin();it!=m_vecVarYC.end();it++)
	{
		if(*it!=NULL)
		{
			delete (*it);
			*it=NULL;
		}
	}
	for(vector<IEC104_VARIBLE_YX *>::iterator it=m_vecVarYX.begin();it!=m_vecVarYX.end();it++)
	{
		if(*it!=NULL)
		{
			delete (*it);
			*it=NULL;
		}
	}
	for(vector<IEC104_VARIBLE_SOE *>::iterator it=m_vecVarSOE.begin();it!=m_vecVarSOE.end();it++)
	{
		if(*it!=NULL)
		{
			delete (*it);
			*it=NULL;
		}
	}
	m_vecVarYC.clear();
	m_vecVarYX.clear();
	m_vecVarSOE.clear();
}

bool Q_protocolIEC104Slave::startProcess()
{
	printf("Q_protocolIEC104Slave::startProcess()\n");
	m_eProtocolStatus = eCommonStatusNormal;
	Q_IEC104Setting *setting=(Q_IEC104Setting *)m_pProcotolConfig->getProtocolSetting();

	if (m_pMgrIEC104SlaveProtocolSetting != NULL)
	{
		g_rtdbInfIEC104 = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
	}

	if(setting!=NULL)
	{
		SetSigExecCmd(setting->getCmdSignalID());
	}

	m_YkExeMode = m_cfgControl->m_YkExecMode;

	sprintf_s(LastError,sizeof(LastError),"��ʹ����ʼ...�ͻ���IP:%s",setting->getClientAddr());

	//�¹�Լ��ӡ
	SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);

	Q_cmdIEC104_AC *pCommand=new Q_cmdIEC104_AC();
	pCommand->setCommonType(eCommandTypeCycle);
	pCommand->setCommand104Type(eCmdTypeAnalogchange);
	pCommand->m_cOnOff = m_cfgBase->m_bSendAC;
	pCommand->m_cSendSOE = m_cfgBase->m_bSendSOE;
	pCommand->m_nTime = m_cfgBase->m_acTime;
	pCommand->asdu_addr = m_cfgBase->m_nAsduAddr;
	pCommand->yxbegin = m_cfgSt->m_YxBeginIndex;
	pCommand->ycbegin = m_cfgSt->m_YcBeginIndex;
	m_listCmd->addCommand(pCommand);

	printf("Q_protocolIEC104Slave::startProcess() eee\n");

	return true;
}

bool Q_protocolIEC104Slave::stopProcess()
{
	Q_IEC104Setting *setting=(Q_IEC104Setting *)m_pProcotolConfig->getProtocolSetting();
	if(setting==NULL)
		return false;

	sprintf_s(LastError,sizeof(LastError),"�Ͽ�����...�ͻ���IP:%s",setting->getClientAddr());

	//�¹�Լ��ӡ
	SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);

	m_listCmd->clearCommand(); //2014-03-14 added

	m_eProtocolStatus = eCommonStatusNull;
	return true;
}

bool Q_protocolIEC104Slave::terminateProtocol()
{
	printf("Q_protocolIEC104Slave::terminateProtocol()\r\n");
	//deleteVarData();
	return true;
}


//	Interrogation Command confirm
//
int Q_protocolIEC104Slave::send_IC_Response(int asdu,int cause)
{
	Q_IEC104_ASDU msg;
	msg.header.type = 0x64;					// interrogation command   ������
	msg.header.qual = 0x01;					// number of elements
	msg.header.tx_cause_1 = cause;			// 0x07 act confirm  0x0A ֹͣ����
	msg.header.tx_cause_2 = 0x00;
	msg.header.commom_asdu_1 = asdu & 0xFF;
	msg.header.commom_asdu_2 = (asdu>>8) & 0xFF;
	// group information
	msg.data[0] = 0x00;
	msg.data[1] = 0x00;
	msg.data[2] = 0x00;
	msg.data[3] = 20;
	return send_I_Msg((BYTE*)&msg, 10);
}

int Q_protocolIEC104Slave::send_CS_Response(int asdu,int cause,BYTE *data,const UINT &len)
{
	Q_IEC104_ASDU msg;
	msg.header.type = 0x67;					// ʱ��ͬ��
	msg.header.qual = 0x01;					// number of elements
	msg.header.tx_cause_1 = cause;
	msg.header.tx_cause_2 = 0x00;
	msg.header.commom_asdu_1 = asdu & 0xFF;
	msg.header.commom_asdu_2 = (asdu>>8) & 0xFF;
	// group information
	msg.data[0] = 0x00;
	msg.data[1] = 0x00;
	msg.data[2] = 0x00;
	memcpy(&(msg.data[3]),data,sizeof(char)*len);
	return send_I_Msg((BYTE*)&msg, len+9);
}

int Q_protocolIEC104Slave::send_C_TS_TA_1_Response(int asdu,int cause,BYTE *data,const UINT &len)
{
	//68 16 02 00 08 00 
	//6B 01 06 00 
	//01 00 
	//00 00 
	//00 00 
	//00 71 9A 30 10 06 09 0D
	Q_IEC104_ASDU msg;
	msg.header.type = 0x6B;					//
	msg.header.qual = 0x01;					// number of elements
	msg.header.tx_cause_1 = cause;
	msg.header.tx_cause_2 = 0x00;
	msg.header.commom_asdu_1 = asdu & 0xFF;
	msg.header.commom_asdu_2 = (asdu>>8) & 0xFF;

	memcpy(&(msg.data[0]),data,sizeof(char)*len);
	return send_I_Msg((BYTE*)&msg, len+5);
}


int Q_protocolIEC104Slave::sendYkResponse(const int &asdu,const int &cause,const int &adr,BYTE *value,const int &type,const UINT &len)
{
	Q_IEC104_ASDU msg;
	msg.header.type = type;					// ����ң��
	msg.header.qual = 0x01;					// number of elements
	msg.header.tx_cause_1 = cause;			//
	msg.header.tx_cause_2 = 0x00;
	msg.header.commom_asdu_1 = asdu & 0xFF;
	msg.header.commom_asdu_2 = (asdu>>8) & 0xFF;
	// group information
	msg.data[0] =adr&0xFF;
	msg.data[1] = (adr>>8)&0xFF;
	msg.data[2] = (adr>>16)&0xFF;

	msg.data[3] = (*value) &0xFF;
	return send_I_Msg((BYTE*)&msg, 10);
}

int Q_protocolIEC104Slave::sendInitFinished(int asdu)
{
	Q_IEC104_ASDU msg;
	msg.header.type = 0x46;					//
	msg.header.qual = 0x01;					// number of elements
	msg.header.tx_cause_1 = 0x01;			//
	msg.header.tx_cause_2 = 0x00;
	msg.header.commom_asdu_1 = asdu & 0xFF;
	msg.header.commom_asdu_2 = (asdu>>8) & 0xFF;
	// group information
	msg.data[0] = 0x00;
	msg.data[1] = 0x00;
	msg.data[2] = 0x00;
	msg.data[3] = 0x00;
	return send_I_Msg((BYTE*)&msg, 10);
}

int Q_protocolIEC104Slave::decodeMsg_U(BYTE *msgbuf, unsigned int len)
{
	//�����Զ˷������Ĳ��Ա��ģ����Ͳ���ȷ��

	int ret=Q_104SUC;
	Q_IEC104_APCI *p = (Q_IEC104_APCI*)msgbuf;

	WORD usTemp_ServSendNum=(p->field2<<8) | p->field1;//�Զ� �������к� �൱�ڱ���ȷ�Ͻ������к�

	if (usTemp_ServSendNum & CMD_STARTDT) //STARTDT  ��Ч
	{
		ret=send_U_Msg(CMD_STARTDT_CON);// ������������ȷ��
		m_bStopSend=false;
	}
	else if (usTemp_ServSendNum & CMD_STOPDT) //STOPDT ��Ч
	{
		ret=send_U_Msg(CMD_STOPDT_CON);//  ����ֹͣ����ȷ��
		m_bStopSend=true;
	}

	//Test Frame
	if (usTemp_ServSendNum & CMD_TESTFR) //�൱��10����64
	{
		ret=send_U_Msg(CMD_TESTFR_CON);//  ���Ͳ���ȷ��
	}
	else if (usTemp_ServSendNum& CMD_TESTFR_CON) //���յ�ȷ��
	{
		m_bSendTestFR=false;
		time(&m_nTimer_U_Test);//=0;
	}
	return ret;

}

int Q_protocolIEC104Slave::decodeMsg_I(BYTE *msgbuf, unsigned int len)
{
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

#ifdef _TEST104SLAVE
	sprintf_s(LastError,sizeof(LastError), "ASDU Address=%d\tTypeID=%d\tCount=%d\tCause=%d\n",
		apdu_ex.asdu, header->type, apdu_ex.num, apdu_ex.cause);
	//OnErrorMsg(m_errorNo, LastError);nn
#endif
	bool scaled=false;
	int ret=Q_104SUC;
	// type identification
	switch (header->type)
	{	
	case 45:
		//����ң��
		func_C_SC_NA_1(&apdu_ex);//fs20150728
		break;
	case 46:
		//˫��ң��
		func_C_DC_NA_1(&apdu_ex);//fs20150728
		break;
	case 47:
		//ң��
		func_C_RC_NA_1(&apdu_ex);//��������ʱ��֧��
		break;
	case 48:
		//�趨��һ��ֵ,��һ��ֵ
		func_C_SE_NA_1(&apdu_ex);//fs20150728
		break;
	case 49:
		//��Ȼ�ֵ
		func_C_SE_NB_1(&apdu_ex);//fs20150728
		break;
	case 50: 
		//�趨��һ��ֵ,�̸�����ֵ
		func_C_SE_NC_1(&apdu_ex);//fs20150728
		break;
	case 70:
		TRACE("End of Initialization\n");
		break;
	case 100:
		ret=func_Interrogate(&apdu_ex);//fs20150727
		break;
	case 103:
		//ʱ��ͬ������
		func_C_CS(&apdu_ex);
		break;
	case 107:
		//��ʱ��Ĳ�������0x6b
		func_C_TS_TA_1(&apdu_ex);
		break;
	default:
		decodeMsg_I_Ex(&apdu_ex);
		break;
	}

	return ret;
}

bool Q_protocolIEC104Slave::TxHandler()
{
	while (1)
	{
		MilliSleep(1000);
		//if (!getSockConnectStatus())
		//{
		//    // time-out for reconnection. ���ӽ����ĳ�ʱ
		//    m_nTimerConnect++;
		//    if (m_nTimerConnect > m_cfgBase->m_nT0)
		//    {
		//        m_nTimerConnect = 0;
		//        /*	if(master->OpenLink(master->RemoteHost, master->RemotePort) == EOK)
		//        		sendStartDT();*/
		//    }
		//}
		/* else
		{*/

		//����ȷ�ϼ����Ϳ���
		// maximun number of frames received before master send ACK.

		if (m_st->nRxCounter - m_st->nLastAckRx >= m_cfgBase->m_nW)
		{
			send_S_Msg();
		}

		//���ͻ����APDU�ĳ�ʱ

		// time-out of waiting ACK to Sended APDU o TestFR
		time(&now);
		m_nTimerConfirm = now - m_nTimerConfirm;
		if (m_nTimerConfirm > m_cfgBase->m_nT1)
		{
			time(&m_nTimerConfirm);//=0;

			if (m_bSendTestFR)
			{
#ifdef _TEST104SLAVE
				sprintf_s(LastError,sizeof(LastError),"����ţ�%d,����TestFR��ʱ,�ر���·,��ǰ��:%d\n",m_errorNo,__LINE__);
				//OnErrorMsg(LastError, strlen(LastError));
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
				closeLink();

			}
			//S���ʽ��ҲҪ�ر�

			if (m_st->nTxCounter - m_st->nLastAckTx > 6)
			{
#ifdef _TEST104SLAVE
				sprintf_s(LastError,sizeof(LastError),"����APDU��ʱ,�ر���·,��ǰ��:%d\n",__LINE__);
				//OnErrorMsg(LastError, sizeof(LastError));
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)LastError,strlen(LastError)+1);
#endif
				closeLink();
			}
		}
		//�����ݱ���ʱȷ�ϵĳ�ʱ

		// time-out for no data to send, send S-frame to ack data received.

		time(&now);
		m_nTimer_S_Ack = now - m_nTimer_S_Ack;
		if (m_nTimer_S_Ack > m_cfgBase->m_nT2)
		{
			time(&m_nTimer_S_Ack);//=0;

			if (m_st->nTxCounter-m_st->nLastAckTx > 0)
				send_S_Msg();

			m_bTimer_S_Ack = true;
		}

		//���ڿ���״̬�·��Ͳ���֡�ĳ�ʱ  ���Ӳ���

		// time-out for idle status, send test frame to check link state.
		time(&now);
		m_nTimer_U_Test = now- m_nTimer_U_Test;
		if (m_nTimer_U_Test > m_cfgBase->m_nT3)
		{
			time(&m_nTimer_U_Test);//=0;
			// if (m_bSendTestFR)
			sendTestFR();
		}

		//��δȷ��I��ʽAPDU�ﵽk��ʱ�����ͷ�ֹͣ����
		if ((m_st->nTxCounter - m_st->nLastAckTx) >= m_cfgBase->m_nK)
		{
			m_bStopSend=true;
		}
		else
		{
			if (m_bStopSend)
				m_bStopSend=false;
		}

		/* }*/
	}
	return 0;
}

int Q_protocolIEC104Slave::func_C_SC_NA_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_SC *ps = ( Q_IEC104_ST_C_SC*)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;


	if (eStation_Slave==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT) //����
		{
			Q_cmdIEC104_SP *cmd=NULL;

			if (1==ps->sco.se)//ѡ��
			{
				Q_cmdIEC104_SP *cmd=new Q_cmdIEC104_SP;


				//bool bExist=m_dataMgr->checkExist(adr,ePtDataTypeSP,cmd->m_nPointNo);
				//if (bExist)/*��ѯ��������ݴ���*/
				if(true)
				{
					cmd->m_eState=eCmdStateYkSelect;
					cmd->m_nTxCause=msg->cause;
					cmd->m_nComAsdu=msg->asdu;
					cmd->m_bExec=false;
					cmd->m_cOnOff=ps->sco.scs;
					cmd->m_cQuality=ps->sco.qu;
					cmd->m_nValue=*((BYTE *)&(ps->sco));

					cmd->m_nInfoAddr=adr;//��Ϣ�����ַ
					//�ظ����ѹջ
					// 					time(&now);
					// 					if (cmd->m_cOnOff != last_sel_cOnOff || cmd->m_nInfoAddr != last_sel_nInfoAddr
					// 						|| (now - sel_timestmp) > 2)
					// 					{
					last_sel_cOnOff = cmd->m_cOnOff;
					last_sel_nInfoAddr = cmd->m_nInfoAddr;
					/*						TRACE("addcommand sel stamp %d \n",now - sel_timestmp);*/
					m_listCmd->addCommand(cmd);//������ѹջ
					// 					}
					// 					time(&sel_timestmp);					

				}
				else
				{
					//����в�����
					cmd->m_bError=true;
					m_errorNo=e104ErrNo_P_UnExist; //��ð���ϸ�����¼һ�¡�
					sprintf_s(LastError, sizeof(LastError),"��Ϣ�����ַ����Ӧ�ĵ�Ų����ڣ���ǰ��ַΪ%d\n",adr);
					//OnErrorMsg(m_errorNo, LastError);nn
				}

			}
			else if (0==ps->sco.se)
			{
				if (eYtExecModeSelect == m_YkExeMode)
				{
					//ѡ��ִ��
					//���ݵ�š���������Ϊ������ �Ȳ�ѯ����
					Q_cmdIEC104 *v_cmd = m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkConfirm,adr);
					//����״ֵ̬��ѯ����
					//v_cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SC,eCmdStateYkConfirm,adr);
					if (v_cmd!=NULL)
					{
						//����״̬
						Q_cmdIEC104_SP *cmd=(Q_cmdIEC104_SP *)(v_cmd);
						cmd->m_bExec=false;
						cmd->m_eState=eCmdStateYkExec;
						cmd->m_nValue=*((BYTE *)&(ps->sco));
					}
					else
					{
						m_errorNo=e104ErrNo_CmdNoSel;
						sprintf_s(LastError, sizeof(LastError),"����ִ�е���ң�������ַ����������豸û��ѡ�񡣵�ǰ�к�:%d\n",__LINE__);
						//OnErrorMsg(m_errorNo, LastError);nn
						return Q_104FAIL;
					}
				} 
				else
				{
					//ֱ��ִ��
					Q_cmdIEC104_SP *cmd=new Q_cmdIEC104_SP;

					cmd->m_nTxCause=msg->cause;
					cmd->m_nComAsdu=msg->asdu;
					cmd->m_cOnOff=ps->sco.scs;
					cmd->m_cQuality=ps->sco.qu;
					cmd->m_nInfoAddr=adr;//��Ϣ�����ַ
					cmd->m_bExec=false;
					cmd->m_eState=eCmdStateYkExec;
					cmd->m_nValue=*((BYTE *)&(ps->sco));

					//�ظ����ѹջ
					// 					time(&now);
					// 
					// 					if (cmd->m_cOnOff != last_exe_cOnOff || cmd->m_nInfoAddr != last_exe_nInfoAddr
					// 						|| (now - exe_timestmp) > 2)
					// 					{
					last_exe_cOnOff = cmd->m_cOnOff;
					last_exe_nInfoAddr = cmd->m_nInfoAddr;
					/*						TRACE("AddCommand exe_timestmp %d \n",now - exe_timestmp);*/
					m_listCmd->addCommand(cmd);//������ѹջ
					// 					}
					// 					time(&exe_timestmp);

					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command .Mode Exec.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
						//OnShowMsg(LastError,sizeof(LastError));
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
					}
					if(true/*m_bInfoMsg*/)
					{
						sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command .Mode Exec.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
						//OnInfoMsg(LastError,sizeof(LastError));
						//�¹�Լ��ӡ
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
					}

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
				Q_cmdIEC104_SP *cmd=(Q_cmdIEC104_SP *)(v_cmd);
				cmd->m_bExec=false;
				cmd->m_eState=eCmdStateYkCancelAct;
				cmd->m_nValue=*((BYTE *)&(ps->sco));
			}
			else
			{
				m_errorNo=e104ErrNo_CmdNoSel;
				sprintf_s(LastError, sizeof(LastError),"����ȡ������ң�������ַ����������豸û��ѡ�񡣵�ǰ�к�:%d\n",__LINE__);
				//OnErrorMsg(m_errorNo, LastError);nn
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

int Q_protocolIEC104Slave::func_C_DC_NA_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_DC *ps = (Q_IEC104_ST_C_DC *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);


	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;


	if (eStation_Slave==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT) //����
		{
			Q_cmdIEC104_DP *cmd=NULL;

			if (1==ps->dco.se)//ѡ��
			{
				Q_cmdIEC104_DP *cmd=new Q_cmdIEC104_DP;

				//bool bExist=m_dataMgr->checkExist(adr,ePtDataTypeDP,cmd->m_nPointNo);
				//if (bExist)/*��ѯ��������ݴ���*/
				if(true)
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
					m_errorNo=e104ErrNo_P_UnExist; //��ð���ϸ�����¼һ�¡�
					sprintf_s(LastError, sizeof(LastError),"��Ϣ�����ַ����Ӧ�ĵ�Ų����ڣ���ǰ��ַΪ%d\n",adr);
					//OnErrorMsg(m_errorNo, LastError);nn
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
					m_errorNo=e104ErrNo_CmdNoSel;
					sprintf_s(LastError, sizeof(LastError),"����ִ�е���ң�������ַ����������豸û��ѡ�񡣵�ǰ�к�:%d\n",__LINE__);
					//OnErrorMsg(m_errorNo, LastError);nn
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
				sprintf_s(LastError, sizeof(LastError),"����ȡ������ң�������ַ����������豸û��ѡ�񡣵�ǰ�к�:%d\n",__LINE__);
				//OnErrorMsg(m_errorNo, LastError);nn
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


int Q_protocolIEC104Slave::func_C_SE_NA_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_SE *ps = ( Q_IEC104_ST_C_SE *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;


	//��˶�޸�20150728
	//�ж�ң�����Ƿ����
	int nPos = 0;
	bool bExist = false;
	int	YTID32 = -1;	

	SCADA_SetPoint curSetPoint;
	CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	int nSetPointRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_SetPoint_ID );
	for (int j=0; j<nSetPointRecordCount; j++)
	{
		int nTempid32 = j+1;
		dbmg_sdb->GetElementByID(SCADA_TAB_SetPoint_ID, nTempid32,&curSetPoint);

		if (m_ProSetting->getChannelID() == curSetPoint.uTuID && msg->asdu == curSetPoint.lDPUID &&
			adr == curSetPoint.iNoInDPU)
		{
			bExist = true;
			YTID32 = nTempid32;
			break;
		}
	}

	if (msg->cause == CMD_ACT)
	{
		if (bExist)
		{
			if(ps->qos.se==1) //ѡ��ִ��
			{
				Q_cmdIEC104_SE *cmd=new Q_cmdIEC104_SE();
				cmd->m_nInfoAddr=adr;
				if(ps->s==1)
				{
					short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
					cmd->m_cOnOff=-nval;
				}
				else
				{
					cmd->m_cOnOff=ps->val2<<8 |ps->val1;
				}

				memcpy(&cmd->m_cQuality,&ps->qos,sizeof(cmd->m_cQuality));
				cmd->m_eState=eCmdStateYkSelectCon;
				cmd->m_nComAsdu=msg->asdu;
				cmd->setCommonType(eCommandTypeOnce);
				cmd->m_cFuncCode=0x30;
				if(m_listCmd!=NULL)
				{
					/*					TRACE("AddCommand Line 691 \n");*/
					m_listCmd->addCommand(cmd);
				}

				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
					memset(LastError,0,sizeof(LastError));
					sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command Select Confirm.Mode Select.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
					//	TRACE("Execute SetPoint command Select Confirm.Mode Select.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
					//					OnShowMsg(LastError,sizeof(LastError));nn
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
				}
				if(true/*m_bInfoMsg*/)
				{
					memset(LastError,0,sizeof(LastError));
					sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command Select Confirm.Mode Select.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
					//	TRACE("Execute SetPoint command Select Confirm.Mode Select.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
					//					OnInfoMsg(LastError,sizeof(LastError));nn
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
				}

				//��˶�޸�
				//д��ѡ��ʱ��
				SYSTEMTIME time;
				GetLocalTime(time);
				curSetPoint.selecttime = time;
				dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ѡ��ʱ��
				curSetPoint.fVal = cmd->m_cOnOff;
				m_YtExeMode=eYtExecModeSelect;
			}
			else
			{
				if(m_YtExeMode==eYtExecModeSelect)
				{
					//ѡ��ִ��
					//����ȷ��
					Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkSelectCon,adr);
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
							sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command ActCon success .Mode Exec.Command info:addr=%d,value=%d\n",adr,val);
							//	TRACE("Execute SetPoint command ActCon success .Mode Exec.Command info:addr=%d,value=%d\n",adr,val);
							//							OnShowMsg(LastError,sizeof(LastError));nn
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
						}
						if(true/*m_bInfoMsg*/)
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
							memset(LastError,0,sizeof(LastError));
							sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command ActCon success .Mode Exec.Command info:addr=%d,value=%d\n",adr,val);
							//TRACE("Execute SetPoint command ActCon success .Mode Exec.Command info:addr=%d,value=%d\n",adr,val);
							//OnInfoMsg(LastError,sizeof(LastError));nn
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
						}
						cmd->m_eState=eCmdStateYkExecAct;//select confirm
					}
				}
				else
				{
					//����������ӵ�CommandList��
					Q_cmdIEC104_SE *cmd=new Q_cmdIEC104_SE();
					cmd->m_nInfoAddr=adr;
					if(ps->s==1)
					{
						short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
						cmd->m_cOnOff=-nval;
					}
					else
					{
						cmd->m_cOnOff=ps->val2<<8 |ps->val1;
					}
					memcpy(&cmd->m_cQuality,&ps->qos,sizeof(cmd->m_cQuality));
					cmd->m_eState=eCmdStateYkExecAct;
					cmd->m_nComAsdu=msg->asdu;
					cmd->m_cFuncCode=0x30;
					cmd->setCommonType(eCommandTypeOnce);
					if(m_listCmd!=NULL)
					{
						/*						TRACE("AddCommand Line 759 \n");*/
						m_listCmd->addCommand(cmd);
					}
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command .Mode Exec.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
						//TRACE("Execute SetPoint command .Mode Exec.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
						//OnShowMsg(LastError,sizeof(LastError))nn;
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
					}
					if(true/*m_bInfoMsg*/)
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command .Mode Exec.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
						//TRACE("Execute SetPoint command .Mode Exec.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
						//OnInfoMsg(LastError,sizeof(LastError));nn
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
					}
					m_YtExeMode=eYtExecModeDirect;			
				}
			}

			sendYtCommand(msg->asdu,CMD_ACT_CON,ps);
		}
		else
		{//�㲻����
			if(true==m_ProSetting->getPrintSetting(eComLogTypeError))
			{
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"�յ���ң������֡�����õ���YT�б��в�����: ChannelID=%d,device=%d,addr=%d\n",m_ProSetting->getChannelID(),msg->asdu,adr);
				//TRACE("�յ���ң������֡�����õ���YT�б��в�����: ChannelID=%d,device=%d,addr=%d\n",m_ProSetting->getChannelID(),msg->asdu,adr);		
				//OnShowMsg(LastError,sizeof(LastError));nn
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
			}
			if (m_bErrMsg)
			{
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"�յ���ң������֡�����õ���YT�б��в�����: ChannelID=%d,device=%d,addr=%d\n",m_ProSetting->getChannelID(),msg->asdu,adr);
				//TRACE("�յ���ң������֡�����õ���YT�б��в�����: ChannelID=%d,device=%d,addr=%d\n",m_ProSetting->getChannelID(),msg->asdu,adr);	
				//OnErrMsg(LastError,sizeof(LastError));nn
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
			}
			sendYtCommand(msg->asdu,CMD_KNOWN_INFO_ADDR,ps);
		}
	}
	else if (msg->cause ==CMD_DEACT)
	{
		//δִ�гɹ�
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
			sprintf_s(LastError,sizeof(LastError),"Execute cancel exec set point command.Command info:addr=%d,value=%d\n",adr,val);
			//TRACE("Execute cancel exec set point command.Command info:addr=%d,value=%d\n",adr,val);
			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
			//OnShowMsg(LastError,sizeof(LastError));nn
		}
		if(true)
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
			sprintf_s(LastError,sizeof(LastError),"Execute cancel exec set point command.Command info:addr=%d,value=%d\n",adr,val);
			//TRACE("Execute cancel exec set point command.Command info:addr=%d,value=%d\n",adr,val);
			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
			//OnInfoMsg(LastError,sizeof(LastError));nn
		}

		//��˶�޸�
		//д��ѡ��ʱ��
		curSetPoint.iexecret = 4;
		dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ�н��
		m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
		sendYtCommand(msg->asdu,CMD_DEACT_CON,ps);
	}
	return ret;
}

int Q_protocolIEC104Slave::func_C_SE_NB_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_SE *ps = ( Q_IEC104_ST_C_SE *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;

	//��˶�޸�20150728
	//�ж�ң�����Ƿ����
	int nPos = 0;
	bool bExist = false;
	int	YTID32 = -1;	

	//Q_RECORDDATATAG dataArray;

	SCADA_SetPoint curSetPoint;
	CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	int nSetPointRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_SetPoint_ID );
	for (int j=0; j<nSetPointRecordCount; j++)
	{
		int nTempid32 = j+1;
		dbmg_sdb->GetElementByID(SCADA_TAB_SetPoint_ID, nTempid32,&curSetPoint);

		if (m_ProSetting->getChannelID() == curSetPoint.uTuID && msg->asdu == curSetPoint.lDPUID &&
			adr == curSetPoint.iNoInDPU)
		{
			bExist = true;
			YTID32 = nTempid32;
			break;
		}
	}

	if (msg->cause == CMD_ACT)
	{
		if (bExist)
		{
			if (ps->qos.se==1) //ң��ѡ��
			{
				Q_cmdIEC104_SE *cmd=new Q_cmdIEC104_SE();
				cmd->m_nInfoAddr=adr;
				if(ps->s==1)
				{
					short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
					cmd->m_cOnOff=-nval;
				}
				else
				{
					cmd->m_cOnOff=ps->val2<<8 |ps->val1;
				}
				memcpy(&cmd->m_cQuality,&ps->qos,sizeof(cmd->m_cQuality));
				cmd->m_eState=eCmdStateYkSelectCon;
				cmd->m_nComAsdu=msg->asdu;
				cmd->setCommonType(eCommandTypeOnce);
				cmd->m_cFuncCode=0x31;
				if(m_listCmd!=NULL)
				{
					m_listCmd->addCommand(cmd);
				}
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
					memset(LastError,0,sizeof(LastError));
					sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command Select Confirm.Mode Select.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
					//TRACE("Execute SetPoint command Select Confirm.Mode Select.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
					//OnShowMsg(LastError,sizeof(LastError));nn
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
				}
				if(true/*m_bInfoMsg*/)
				{
					sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command Select Confirm.Mode Select.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
					//TRACE("Execute SetPoint command Select Confirm.Mode Select.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
					//OnInfoMsg(LastError,sizeof(LastError));nn
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
				}

				//��˶�޸�
				//д��ѡ��ʱ��
				SYSTEMTIME time;
				GetLocalTime(time);
				curSetPoint.selecttime = time;
				dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ѡ��ʱ��
				curSetPoint.fVal = cmd->m_cOnOff;
				m_YtExeMode=eYtExecModeSelect;
			}
			else
			{
				if (m_YtExeMode==eYtExecModeSelect)
				{
					//ѡ��ִ��
					//����ȷ��
					Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkSelectCon,adr);
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
							sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command ActCon success .Mode Exec.Command info:addr=%d,value=%d\n",adr,val);
							//TRACE("Execute SetPoint command ActCon success .Mode Exec.Command info:addr=%d,value=%d\n",adr,val);
							//OnShowMsg(LastError,sizeof(LastError));
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
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
							sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command ActCon success .Mode Exec.Command info:addr=%d,value=%d\n",adr,val);
							//	TRACE("Execute SetPoint command ActCon success .Mode Exec.Command info:addr=%d,value=%d\n",adr,val);
							//OnInfoMsg(LastError,sizeof(LastError));nn
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
						}
						//						pYTData[nPos].exectime = time(NULL);
						cmd->m_eState=eCmdStateYkExecAct;//select confirm
					}
				}
				else
				{
					//����������ӵ�CommandList��
					Q_cmdIEC104_SE *cmd=new Q_cmdIEC104_SE();
					cmd->m_nInfoAddr=adr;
					if(ps->s==1)
					{
						short nval=(~((ps->val2)<<8 |ps->val1)+1)&0x7FFF;
						cmd->m_cOnOff=-nval;
					}
					else
					{
						cmd->m_cOnOff=ps->val2<<8 |ps->val1;
					}
					memcpy(&cmd->m_cQuality,&ps->qos,sizeof(cmd->m_cQuality));
					cmd->m_eState=eCmdStateYkExecAct;
					cmd->m_nComAsdu=msg->asdu;
					cmd->m_cFuncCode=0x31;
					cmd->setCommonType(eCommandTypeOnce);
					if(m_listCmd!=NULL)
					{
						m_listCmd->addCommand(cmd);
					}
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command .Mode Exec.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
						//TRACE("Execute SetPoint command .Mode Exec.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
						//OnShowMsg(LastError,sizeof(LastError));nn
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
					}
					if(true/*m_bInfoMsg*/)
					{
						sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command .Mode Exec.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
						//TRACE("Execute SetPoint command .Mode Exec.Command info:addr=%d,value=%d\n",adr,cmd->m_cOnOff);
						//OnInfoMsg(LastError,sizeof(LastError));nn
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
					}
					m_YtExeMode=eYtExecModeDirect;
					//					pYTData[nPos].exectime = time(NULL);
					//					pYTData[nPos].selecttime = 0;
					//					pYTData[nPos].selectreplytime = 0;
					//					pYTData[nPos].finishtime = 0;
					//					pYTData[nPos].execresult = 0;
				}
			}
			sendYtCommand(msg->asdu,CMD_ACT_CON,ps);
		}
		else
		{//�㲻����
			if(true==m_ProSetting->getPrintSetting(eComLogTypeError))
			{
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"�յ���ң������֡�����õ���YT�б��в�����: ChannelID=%d,device=%d,addr=%d\n",m_ProSetting->getChannelID(),msg->asdu,adr);
				//TRACE("�յ���ң������֡�����õ���YT�б��в�����: ChannelID=%d,device=%d,addr=%d\n",m_ProSetting->getChannelID(),msg->asdu,adr);
				//OnShowMsg(LastError,sizeof(LastError));nn
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
			}
			if (m_bErrMsg)
			{
				memset(LastError,0,sizeof(LastError));
				sprintf_s(LastError,sizeof(LastError),"�յ���ң������֡�����õ���YT�б��в�����: ChannelID=%d,device=%d,addr=%d\n",m_ProSetting->getChannelID(),msg->asdu,adr);
				//TRACE("�յ���ң������֡�����õ���YT�б��в�����: ChannelID=%d,device=%d,addr=%d\n",m_ProSetting->getChannelID(),msg->asdu,adr);	
				//OnInfoMsg(LastError,sizeof(LastError));nn
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
			}
			sendYtCommand(msg->asdu,CMD_KNOWN_INFO_ADDR,ps);
		}
	}
	else if (msg->cause == CMD_DEACT)
	{
		//δִ�гɹ�
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
			sprintf_s(LastError,sizeof(LastError),"Execute cancel exec set point command.Command info:addr=%d,value=%d\n",adr,val);
			//TRACE("Execute cancel exec set point command.Command info:addr=%d,value=%d\n",adr,val);
			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
			//OnShowMsg(LastError,sizeof(LastError));nn
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
			memset(LastError,0,sizeof(LastError));
			sprintf_s(LastError,sizeof(LastError),"Execute cancel exec set point command.Command info:addr=%d,value=%d\n",adr,val);
			//	TRACE("Execute cancel exec set point command.Command info:addr=%d,value=%d\n",adr,val);
			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
			//OnInfoMsg(LastError,sizeof(LastError));nn
		}

		//��˶�޸�
		//д��ѡ��ʱ��
		curSetPoint.iexecret = 4;
		dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��execresult

		m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
		sendYtCommand(msg->asdu,CMD_DEACT_CON,ps);
	}
	return ret;
}

int Q_protocolIEC104Slave::func_C_SE_NC_1(Q_104ApduEx *msg)//fs20150728
{
	int ret=Q_104SUC;

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_SE3 *ps = ( Q_IEC104_ST_C_SE3 *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;

	//��˶�޸�20150728
	//�ж�ң�����Ƿ����
	int nPos = 0;
	bool bExist = false;
	int	YTID32 = -1;	

	//Q_RECORDDATATAG dataArray;

	SCADA_SetPoint curSetPoint;
	CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	int nSetPointRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_SetPoint_ID );
	for (int j=0; j<nSetPointRecordCount; j++)
	{
		int nTempid32 = j+1;
		dbmg_sdb->GetElementByID(SCADA_TAB_SetPoint_ID, nTempid32, &curSetPoint);

		if (m_ProSetting->getChannelID() == curSetPoint.uTuID && msg->asdu == curSetPoint.lDPUID &&
			adr == curSetPoint.iNoInDPU)
		{
			bExist = true;
			YTID32 = nTempid32;
			break;
		}
	}

	if (msg->cause == CMD_ACT)
	{
		if (bExist)
		{
			if(ps->qos.se==1) //ѡ��ִ��
			{
				Q_cmdIEC104_SE *cmd=new Q_cmdIEC104_SE();
				cmd->m_nInfoAddr=adr;

				memcpy(&cmd->m_fVal,&ps->val,sizeof(cmd->m_fVal)); //Ĭ���ǵ��ֽ���ǰ
				memcpy(&cmd->m_cQuality,&ps->qos,sizeof(cmd->m_cQuality));

				//IEC104��վ��ң��ϵ�����յ���������������������ֵ�������
				float fFaocter = m_pMgrIEC104SlaveProtocolSetting->getYtcoefficient(m_ProSetting->getChannelID(),msg->asdu,adr);
				cmd->m_fVal = cmd->m_fVal*fFaocter;

				cmd->m_eState=eCmdStateYkSelectCon;
				cmd->m_nComAsdu=msg->asdu;
				cmd->setCommonType(eCommandTypeOnce);
				cmd->m_cFuncCode=0x32;

				if(m_listCmd!=NULL)
				{
					/*					TRACE("AddCommand Line 863 \n");*/
					m_listCmd->addCommand(cmd);
				}

				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
					memset(LastError,0,sizeof(LastError));
					sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command Select Confirm.Mode Select.Command info:addr=%d,value=%.3f\n",adr,cmd->m_fVal);
					//TRACE("Execute SetPoint command Select Confirm.Mode Select.Command info:addr=%d,value=%.3f\n",adr,cmd->m_fVal);
					//OnShowMsg(LastError,sizeof(LastError));nn
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);

				}
				if(true/*m_bInfoMsg*/)
				{
					memset(LastError,0,sizeof(LastError));
					sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command Select Confirm.Mode Select.Command info:addr=%d,value=%.3f\n",adr,cmd->m_fVal);
					//TRACE("Execute SetPoint command Select Confirm.Mode Select.Command info:addr=%d,value=%.3f\n",adr,cmd->m_fVal);
					//OnInfoMsg(LastError,sizeof(LastError));nn
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);

				}

				//��˶�޸�
				//д��ѡ��ʱ��
				SYSTEMTIME time;
				GetLocalTime(time);
				curSetPoint.selecttime = time;
				dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ѡ��ʱ��
				curSetPoint.fVal = cmd->m_cOnOff;
				m_YtExeMode=eYtExecModeSelect;
			}
			else
			{
				if(m_YtExeMode==eYtExecModeSelect)
				{
					//ѡ��ִ��
					//����ȷ��
					Q_cmdIEC104 *cmd=m_listCmd->getCommandByStatus(eCmdTypeC_SE,eCmdStateYkSelectCon,adr);
					if(cmd!=NULL)
					{
						if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
						{
							memset(LastError,0,sizeof(LastError));

							float ff=0.0f;
							memcpy(&ff,&ps->val,sizeof(ff));
							sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command ActCon success .Mode Exec.Command info:addr=%d,value=%d\n",adr,ff);
							//TRACE("Execute SetPoint command ActCon success .Mode Exec.Command info:addr=%d,value=%d\n",adr,ff);
							//OnShowMsg(LastError,sizeof(LastError));nn
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
						}
						if(true/*m_bInfoMsg*/)
						{
							memset(LastError,0,sizeof(LastError));
							float ff=0.0f;
							memcpy(&ff,&ps->val,sizeof(ff));
							sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command ActCon success .Mode Exec.Command info:addr=%d,value=%d\n",adr,ff);
							//TRACE("Execute SetPoint command ActCon success .Mode Exec.Command info:addr=%d,value=%d\n",adr,ff);
							//OnInfoMsg(LastError,sizeof(LastError));nn
							SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
						}
						cmd->m_eState=eCmdStateYkExecAct;//select confirm
					}
				}
				else
				{
					//����������ӵ�CommandList��
					Q_cmdIEC104_SE *cmd=new Q_cmdIEC104_SE();
					cmd->m_nInfoAddr=adr;

					memcpy(&cmd->m_fVal,&ps->val,sizeof(cmd->m_fVal)); //Ĭ���ǵ��ֽ���ǰ

					//IEC104��վ��ң��ϵ�����յ���������������������ֵ�������
					float fFaocter = m_pMgrIEC104SlaveProtocolSetting->getYtcoefficient(m_ProSetting->getChannelID(),msg->asdu,adr);
					cmd->m_fVal = cmd->m_fVal*fFaocter;

					memcpy(&cmd->m_cQuality,&ps->qos,sizeof(cmd->m_cQuality));
					cmd->m_eState=eCmdStateYkExecAct;
					cmd->m_nComAsdu=msg->asdu;
					cmd->m_cFuncCode=0x32;
					cmd->setCommonType(eCommandTypeOnce);

					if(m_listCmd!=NULL)
					{
						/*						TRACE("AddCommand Line 918 \n");*/
						m_listCmd->addCommand(cmd);
					}

					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));

						sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command .Mode Exec.Command info:addr=%d,value=%.3f\n",adr,cmd->m_fVal);
						//TRACE("Execute SetPoint command .Mode Exec.Command info:addr=%d,value=%.3f\n",adr,cmd->m_fVal);
						//OnShowMsg(LastError,sizeof(LastError));nn
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
					}
					if(true/*m_bInfoMsg*/)
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Execute SetPoint command .Mode Exec.Command info:addr=%d,value=%.3f\n",adr,cmd->m_fVal);
						//TRACE("Execute SetPoint command .Mode Exec.Command info:addr=%d,value=%.3f\n",adr,cmd->m_fVal);
						//OnInfoMsg(LastError,sizeof(LastError));nn
						SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
					}
					m_YtExeMode=eYtExecModeDirect;
				}

			}
		}
		sendYtCommand3(msg->asdu,CMD_ACT_CON,ps);
	}
	else if (msg->cause ==CMD_DEACT)
	{
		//δִ�гɹ�
		m_listCmd->removeCommand(eCmdTypeC_SE,eCmdStateYkExecAct,adr);
		if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
		{
			memset(LastError,0,sizeof(LastError));

			float ff=0.0f;
			memcpy(&ff,&ps->val,sizeof(ff));
			sprintf_s(LastError,sizeof(LastError),"Execute cancel exec set point command .Command info:addr=%d,value=%.3f\n",adr,ff);
			//TRACE("Execute cancel exec set point command .Command info:addr=%d,value=%.3f\n",adr,ff);
			//OnShowMsg(LastError,sizeof(LastError));nn
			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
		}
		if(true/*m_bInfoMsg*/)
		{
			memset(LastError,0,sizeof(LastError));
			float ff=0.0f;
			memcpy(&ff,&ps->val,sizeof(ff));
			sprintf_s(LastError,sizeof(LastError),"Execute cancel exec set point command .Command info:addr=%d,value=%.3f\n",adr,ff);
			//TRACE("Execute cancel exec set point command .Command info:addr=%d,value=%.3f\n",adr,ff);
			//OnInfoMsg(LastError,sizeof(LastError));nn
			SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);
		}
		//��˶�޸�
		//д��
		curSetPoint.iexecret = 4;
		dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��execresult
		sendYtCommand3(msg->asdu,CMD_DEACT_CON,ps);
	}
	return ret;
}

int Q_protocolIEC104Slave::func_C_RC_NA_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_RC *ps = ( Q_IEC104_ST_C_RC *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;


	//��˶�޸�20150728
	//�ж�ң�����Ƿ����
	int nPos = 0;
	bool bExist = false;
	int	YTID32 = -1;	

	//Q_RECORDDATATAG dataArray;

	SCADA_SetPoint curSetPoint;
	CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	int nSetPointRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_SetPoint_ID );
	for (int j=0; j<nSetPointRecordCount; j++)
	{
		int nTempid32 = j+1;
		dbmg_sdb->GetElementByID(SCADA_TAB_SetPoint_ID, nTempid32,&curSetPoint);

		if (m_ProSetting->getChannelID() == curSetPoint.uTuID && msg->asdu == curSetPoint.lDPUID &&
			adr == curSetPoint.iNoInDPU)
		{
			bExist = true;
			YTID32 = nTempid32;
			break;
		}
	}

	if (eStation_Slave==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT) //����
		{
			Q_cmdIEC104_SP *cmd=NULL;

			if (1==ps->rco.se)//ѡ��
			{
				Q_cmdIEC104_RC *cmd=new Q_cmdIEC104_RC;

				//ע�⣺Ŀǰ��֪���������������ݴ���������Ĭ��ȫ��һ�£�
				//bool bExist=m_dataMgr->checkExist(adr,ePtDataTypeNull,cmd->m_nPointNo);
				//if (bExist)/*��ѯ��������ݴ���*/
				if(true)
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
					sprintf_s(LastError, sizeof(LastError),"��Ϣ�����ַ����Ӧ�ĵ�Ų����ڣ���ǰ��ַΪ%d\n",adr);
					//OnErrorMsg(m_errorNo, LastError);nn
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
					sprintf_s(LastError, sizeof(LastError),"����ִ�е���ң�������ַ����������豸û��ѡ�񡣵�ǰ�к�:%d\n",__LINE__);
					//OnErrorMsg(m_errorNo, LastError);nn
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
				sprintf_s(LastError, sizeof(LastError),"����ȡ������ң�������ַ����������豸û��ѡ�񡣵�ǰ�к�:%d\n",__LINE__);
				//OnErrorMsg(m_errorNo, LastError);nn
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
int Q_protocolIEC104Slave::func_C_CS(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_CS *ps = ( Q_IEC104_ST_C_CS *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;

	if (eStation_Slave==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT) //����
		{
			//Q_cmdIEC104_CS *cmd=new Q_cmdIEC104_CS;
			//cmd->setCommand104Type(eCmdTypeC_SC);//����ң��

			//cmd->m_eState=eCmdStateSync;
			//cmd->m_nTxCause=msg->cause;
			//cmd->m_nComAsdu=msg->asdu;
			//cmd->m_bExec=false;
			//cmd->m_nInfoAddr=adr;//��Ϣ�����ַ
			//cmd->m_nPointNo=0;//TODO:����е��
			//memcpy(cmd->m_cTime,ps->val,sizeof(cmd->m_cTime));

			////m_listCmd->addCommand(cmd);//������ѹջ

			//��Ϊ��ʱ��ͬ��������ֱ�Ӵ���

			//��̬����ʱ�䣬����ϵͳʱ�� TODO:
			Q_CP56Time2a t2(ps->val);
			if(m_cfgSyncClock->m_bExecCS)
			{
				SYSTEMTIME temp = t2._GetSystemTime(m_cfgSyncClock->m_nFixUsecs);
				SYSTEMTIME *pSysTime = &temp;//��̬���������ӡ�
#ifdef WINDOWS_ENV
				SetLocalTime( pSysTime );
#else
				char szCmd[64];
				char szMon[5];
				char szdate[30];

				switch (pSysTime->wMonth)
				{
				case 1:
					sprintf(szMon,"JAN");
					break;
				case 2:
					sprintf(szMon,"FEB");
					break;
				case 3:
					sprintf(szMon,"MAR");
					break;
				case 4:
					sprintf(szMon,"APR");
					break;
				case 5:
					sprintf(szMon,"MAY");
					break;
				case 6:
					sprintf(szMon,"JUN");
					break;
				case 7:
					sprintf(szMon,"JUL");
					break;
				case 8:
					sprintf(szMon,"AUG");
					break;
				case 9:
					sprintf(szMon,"SEP");
					break;
				case 10:
					sprintf(szMon,"OCT");
					break;
				case 11:
					sprintf(szMon,"NOV");
					break;
				case 12:
					sprintf(szMon,"DEC");
					break;
				default:
					sprintf(szMon,"DEC");
					break;
				}

				sprintf(szCmd, "date -s ");

				sprintf(szdate, "%d %s %04d %d:%d:%d#", 
					pSysTime->wDay, szMon, pSysTime->wYear,
					pSysTime->wHour, pSysTime->wMinute, pSysTime->wSecond);

				szCmd[8] = '"';

				for (int aaa = 0; aaa<sizeof(szdate); aaa++)
				{
					szCmd[9+aaa] = szdate[aaa];
				}

				for (int bbb = 0; bbb<sizeof(szCmd); bbb++)
				{
					if (szCmd[bbb] == '#')
					{	
						szCmd[bbb] = '"';
						break;
					}
				}	

				TRACE("command set time : %s",szCmd);
				system(szCmd);
#endif

			}

			//����ʱ��ͬ��ȷ��
			if (m_cfgSyncClock->m_eActMethord=eCS_Act_Master)
			{
				send_CS_Response(msg->asdu,CMD_ACT_CON,ps->val,sizeof(ps->val));
			}
			else if (m_cfgSyncClock->m_eActMethord=eCS_Act_MasterFix)
			{
				//��ʱ��eCS_Act_Masterһ��
				send_CS_Response(msg->asdu,CMD_ACT_CON,ps->val,sizeof(ps->val));
			}
			else if (m_cfgSyncClock->m_eActMethord=eCS_Act_OwnSysBf)
			{
				//��ʱ��eCS_Act_Masterһ��
				send_CS_Response(msg->asdu,CMD_ACT_CON,ps->val,sizeof(ps->val));
			}
			else if (m_cfgSyncClock->m_eActMethord=eCS_Act_OwnSysAf)
			{
				//��ʱ��eCS_Act_Masterһ��
				send_CS_Response(msg->asdu,CMD_ACT_CON,ps->val,sizeof(ps->val));
			}
		}
	}

	return ret;

}

int Q_protocolIEC104Slave::func_C_TS_TA_1(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;

	DWORD adr;//�����ַ

	Q_IEC104_ST_C_TS_TA_1 *ps = ( Q_IEC104_ST_C_TS_TA_1 *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	adr = ps->addr2<<16 | ps->addr1<<8 | ps->addr0;

	if (eStation_Slave==m_cfgBase->m_eCurStType)
	{
		if (msg->cause == CMD_ACT) //����
		{
			if (m_cfgSyncClock->m_eActMethord=eCS_Act_Master)
			{
				send_C_TS_TA_1_Response(msg->asdu,CMD_ACT_CON,&ps->addr0,13);
			}
			else if (m_cfgSyncClock->m_eActMethord=eCS_Act_MasterFix)
			{
				//��ʱ��eCS_Act_Masterһ��
				send_C_TS_TA_1_Response(msg->asdu,CMD_ACT_CON,&ps->addr0,13);
			}
			else if (m_cfgSyncClock->m_eActMethord=eCS_Act_OwnSysBf)
			{
				//��ʱ��eCS_Act_Masterһ��
				send_C_TS_TA_1_Response(msg->asdu,CMD_ACT_CON,&ps->addr0,13);
			}
			else if (m_cfgSyncClock->m_eActMethord=eCS_Act_OwnSysAf)
			{
				//��ʱ��eCS_Act_Masterһ��
				send_C_TS_TA_1_Response(msg->asdu,CMD_ACT_CON,&ps->addr0,13);
			}
		}
	}
	return ret;
}

int Q_protocolIEC104Slave::func_Interrogate(Q_104ApduEx *msg)
{
	int ret=Q_104SUC;
	if (msg->cause == CMD_ACT) //����
	{
		Q_cmdIEC104_IC *cmd=new Q_cmdIEC104_IC;

		cmd->setCommand104Type(eCmdTypeC_IC);
		cmd->m_eState=eCmdStateIntRequest;
		cmd->m_nTxCause=msg->cause;
		cmd->m_nComAsdu=msg->asdu;
		//cmd->m_nComAsdu=m_cfgBase->m_nAsduAddr;//fs20150822��վ���ٻ���ַ�б������þ���	
		cmd->m_bExec=false;
		cmd->m_cQuality=msg->msgbuf[9]; //��10�ֽھ���

		m_listCmd->addCommand(cmd);//������ѹջ
	}
	else if (msg->cause == CMD_DEACT) //ֹͣ����
	{
		//��ѯ�����״ֵ̬������
		send_IC_Response(msg->asdu,CMD_DEACT_TERM); //ֹͣ����ȷ��
	}

	return ret;
}

int Q_protocolIEC104Slave::handleCommmd_SC(Q_cmdIEC104 *v_cmd)
{
	int bRet=Q_104SUC;
	v_cmd->m_bExec=true;//����

	switch (v_cmd->m_eState)
	{
	case eCmdStateYkSelect:
		{
			//����ȷ��֡
			Q_cmdIEC104_SP *cmd = (Q_cmdIEC104_SP *)v_cmd;
			if (NULL != cmd)
			{
				//��˶�޸�20150728�ж�ң�ص��Ƿ����
				int nPos = 0;
				bool bExist = false;
				int	YKID32 = -1;	

				SCADA_Relay curRelay;
				CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
				CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

				int nRelayRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_Relay_ID );
				for (int j=0; j<nRelayRecordCount; j++)
				{
					int nTempid32 = j+1;
					dbmg_sdb->GetElementByID(SCADA_TAB_Relay_ID, nTempid32,&curRelay);

					if (m_ProSetting->getChannelID() == curRelay.uTuID && v_cmd->m_nComAsdu == curRelay.lDPUID &&
						v_cmd->m_nInfoAddr == curRelay.iNoInDPU)
					{
						bExist = true;
						YKID32 = nTempid32;
						break;
					}
				}

				if (bExist)//����
				{
					v_cmd->m_eState=eCmdStateYkConfirm;

					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Exec remote single control command select.mode select. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
						TRACE("Exec remote single control command select.mode select. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
					}
					if(true)//m_bInfoMsg
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Exec remote single control command select.mode select. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
						TRACE("Exec remote single control command select.mode select. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);							
					}

					SYSTEMTIME time;
					GetLocalTime(time);
					curRelay.selecttime = time;
					dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ѡ��ʱ��
					sendYkResponse(cmd->m_nComAsdu,CMD_ACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2D,sizeof(cmd->m_nValue));
				}
				else//�����ڣ���ӡ������Ϣ
				{
					v_cmd->m_eState=eCmdStateEnd;
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Exec remote single control command select failed. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
						TRACE("Exec remote single control command select failed. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);	
					}
					if(true)//m_bInfoMsg
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Exec remote single control command select failed. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
						TRACE("Exec remote single control command select failed. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
					}
				}
			}
		}
		break;
	case eCmdStateYkExec:
		{
			//ִ��
			//��˶��д20150728�ж�ң�ص��Ƿ����
			int nPos = 0;
			bool bExist = false;
			int YKtoYXID32 = -1;
			int	YKID32 = -1;	

			Q_cmdIEC104_SP *cmd = (Q_cmdIEC104_SP *)v_cmd;
			Q_RECORDDATATAG dataArray;


			SCADA_Relay curRelay;
			CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
			CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

			int nRelayRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_Relay_ID );
			for (int j=0; j<nRelayRecordCount; j++)
			{
				int nTempid32 = j+1;
				dbmg_sdb->GetElementByID(SCADA_TAB_Relay_ID, nTempid32,&curRelay);

				if (m_ProSetting->getChannelID() == curRelay.uTuID && v_cmd->m_nComAsdu == curRelay.lDPUID &&
					v_cmd->m_nInfoAddr == curRelay.iNoInDPU)
				{
					bExist = true;
					YKID32 = nTempid32;
					YKtoYXID32 = curRelay.ulYXID32;
					break;
				}
			}

			if(bExist)//����ڸ���ʵʱ��
			{
				int nYXbegin = m_tags->getPointBegin();
				int nYXend = m_tags->getPointEnd();


				SCADA_Digit tempDigit;
				dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID,YKtoYXID32,&tempDigit);

				if (&tempDigit != NULL)
				{
					//2016-02-23�޸�Ϊֱ��ʹ����������
					dataArray.eDataType=eDataTypeBit;
					dataArray.channelID=tempDigit.uTuID;
					dataArray.nIndex=tempDigit.iIndex;
					dataArray.bValue=(cmd->m_cOnOff==1?true:false);
					m_pMgrIEC104SlaveProtocolSetting->recordDataValue(&dataArray,1,true);
				}

				if(curRelay.iZF)
				{
					//tjs add  2015-07-30
					//������Ϣ����

					RControl rc;
					rc.iStation =curRelay.izfTuID;
					rc.iYKPoint =curRelay.izfNoInDPU;
					rc.iYxID=curRelay.ulYXID32;//add by tjs 2015-12-9
					rc.iCommandType = mes_RCExecuteOk;
					rc.iVal = dataArray.bValue;
					rc.iDevice=curRelay.izfDPUID;
					rc.iZf=1;
					//2016-05-18 ���ӹ�����ң��ID������ת�����㷨�����޷���ȷ����
					rc.iYxID=curRelay.ulYXID32;
					m_rtdbInf->getMainDb()->SendMessage(curRelay.izfProcessID, mes_RCExecute, sizeof(RControl), (char *)&rc);
				} 

				SYSTEMTIME time;
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

				dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʱ��


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
						m_pMgrIEC104SlaveProtocolSetting->recordDataValue(&dataArray,1,true);
					}
				}

				sendYkResponse(cmd->m_nComAsdu,CMD_ACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2D,sizeof(cmd->m_nValue));
				//sendYkResponse(cmd->m_nComAsdu,CMD_DEACT_TERM,cmd->m_nInfoAddr,&cmd->m_nValue,0x2D,sizeof(cmd->m_nValue));
			}
			else//�����ڣ���ӡ������Ϣ
			{
				v_cmd->m_eState=eCmdStateEnd;
				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
					memset(LastError,0,sizeof(LastError));
					sprintf_s(LastError,sizeof(LastError),"Exec remote single control command Exec failed. addr is %d,value=%d YK Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
					TRACE("Exec remote single control command select failed. addr is %d,value=%d YK Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);

				}
				if(true)
				{
					memset(LastError,0,sizeof(LastError));
					sprintf_s(LastError,sizeof(LastError),"Exec remote single control command Exec failed. addr is %d,value=%d YK Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
					TRACE("Exec remote single control command select failed. addr is %d,value=%d YK Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
				}
			}
			m_listCmd->removeCommand(v_cmd);
		}
		break;
	case eCmdStateYkCancelAct:
		{
			//ȡ������
			//����ȷ��֡
			Q_cmdIEC104_SP *cmd = (Q_cmdIEC104_SP *)v_cmd;
			if (NULL != cmd)
			{
				v_cmd->m_bError=true;
				int  nRet=sendYkResponse(cmd->m_nComAsdu,CMD_DEACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2D,sizeof(cmd->m_nValue));
				if (nRet==Q_104FAIL)
				{
					m_errorNo=e104Errno_CmdSend;
#ifdef _TEST104SLAVE
					sprintf_s(LastError, sizeof(LastError),"���͵���ؿ�ȡ��ȷ�ϳ�����ǰ�к�:%d\n",__LINE__);
					//OnErrorMsg(m_errorNo, LastError);nn
					bRet=false;
#endif
				}
				else
				{
					v_cmd->m_eState=eCmdStateYkCancelAct;
				}

				v_cmd->m_eState=eCmdStateEnd;
			}
		}
		break;
	}

	return bRet;
}


int Q_protocolIEC104Slave::handleCommmd_DC(Q_cmdIEC104 *v_cmd)
{
	int bRet=Q_104SUC;

	v_cmd->m_bExec=true;//����

	if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		switch (v_cmd->m_eState)
		{
		case eCmdStateYkSelect:
			{
				//����ȷ��֡
				Q_cmdIEC104_DP *cmd =(Q_cmdIEC104_DP *)v_cmd;

				//��˶��д
				//�ж�ң�ص��Ƿ����
				int nPos = 0;
				bool bExist = false;
				int	YKID32 = -1;	

				SCADA_Relay curRelay;
				CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
				CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

				int nRelayRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_Relay_ID );
				for (int j=0; j<nRelayRecordCount; j++)
				{
					int nTempid32 = j+1;
					dbmg_sdb->GetElementByID(SCADA_TAB_Relay_ID, nTempid32,&curRelay);

					if (m_ProSetting->getChannelID() == curRelay.uTuID && v_cmd->m_nComAsdu == curRelay.lDPUID &&
						v_cmd->m_nInfoAddr == curRelay.iNoInDPU)
					{
						bExist = true;
						YKID32 = nTempid32;
						break; 
					}
				}

				if (bExist)//����
				{
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Exec remote single control command select.mode select. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
						TRACE("Exec remote single control command select.mode select. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
					}
					if(true)//m_bInfoMsg
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Exec remote single control command select.mode select. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
						TRACE("Exec remote single control command select.mode select. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);							
					}

					if (NULL != cmd)
					{
						if (sendYkResponse(cmd->m_nComAsdu,CMD_ACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2E,sizeof(cmd->m_nValue))==Q_104FAIL)
						{
							//���ͱ��ĳ���
							m_errorNo=e104Errno_CmdSend;
							v_cmd->m_eState=eCmdStateYkConfirm;
							v_cmd->m_bError=true;
							sprintf_s(LastError, sizeof(LastError),"����˫��ң��ѡ��ȷ����Ϣ������ǰ�к�:%d\n",__LINE__);
							//OnErrorMsg(m_errorNo, LastError);nn
							bRet=false;
						}
						else
						{
							v_cmd->m_eState=eCmdStateYkConfirm;
						}
					}

					SYSTEMTIME time;
					GetLocalTime(time);
					curRelay.selecttime = time;
					dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ѡ��ʱ��
				}
				else//�����ڣ���ӡ������Ϣ
				{
					v_cmd->m_eState=eCmdStateEnd;
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Exec remote single control command select failed. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
						TRACE("Exec remote single control command select failed. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);	
					}
					if(true)//m_bInfoMsg
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Exec remote single control command select failed. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
						TRACE("Exec remote single control command select failed. addr is %d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
					}
				}

			}
			break;
		case eCmdStateYkExec:
			{
				//ִ��
				//��˶��д20150728
				//�ж�ң�ص��Ƿ����
				int nPos = 0;
				bool bExist = false;
				int YKtoYXID32 = -1;
				int	YKID32 = -1;	

				Q_cmdIEC104_DP *cmd = (Q_cmdIEC104_DP *)v_cmd;
				Q_RECORDDATATAG dataArray;


				SCADA_Relay curRelay;
				CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
				CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

				int nRelayRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_Relay_ID );
				for (int j=0; j<nRelayRecordCount; j++)
				{
					int nTempid32 = j+1;
					dbmg_sdb->GetElementByID(SCADA_TAB_Relay_ID, nTempid32,&curRelay);

					if (m_ProSetting->getChannelID() == curRelay.uTuID && v_cmd->m_nComAsdu == curRelay.lDPUID &&
						v_cmd->m_nInfoAddr == curRelay.iNoInDPU)
					{
						bExist = true;
						YKID32 = nTempid32;
						YKtoYXID32 = curRelay.ulYXID32;
						break;
					}
				}

				if(bExist)//����ڸ���ʵʱ��
				{
					int nYXbegin = m_tags->getPointBegin();
					int nYXend = m_tags->getPointEnd();

					if (YKtoYXID32 >= nYXbegin && YKtoYXID32 <= nYXend)//��Ӧң�ŵ����д����ֵ
					{
						SCADA_Digit tempDigit;
						dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID,YKtoYXID32,&tempDigit);

						if (&tempDigit != NULL)
						{
							//2016-02-23�޸�Ϊֱ��ʹ����������
							dataArray.eDataType=eDataTypeBit;
							dataArray.channelID=tempDigit.uTuID;
							dataArray.nIndex=tempDigit.iIndex;
							dataArray.bValue=(cmd->m_cOnOff==1?true:false);
							m_pMgrIEC104SlaveProtocolSetting->recordDataValue(&dataArray,1,true);

						}

						/*//add by tjs 2015-04-15
						m_SigArray.channelid=m_ProSetting->getChannelID();
						m_SigArray.bzf=curRelay.iZF;
						m_SigArray.zfchannelid=curRelay.izfTuID;
						m_SigArray.zfdevice=curRelay.izfDPUID;
						m_SigArray.zfno=curRelay.izfNoInDPU;
						*/

						if(curRelay.iZF)
						{
							//tjs add  2015-07-30
							//������Ϣ����

							RControl rc;
							rc.iStation =curRelay.izfTuID;
							rc.iYKPoint =curRelay.izfNoInDPU;
							rc.iYxID=curRelay.ulYXID32;//add by tjs 2015-12-9
							rc.iCommandType = mes_RCExecuteOk;
							rc.iVal = dataArray.bValue;
							rc.iDevice=curRelay.izfDPUID;
							rc.iZf=1;
							//2016-05-18 ���ӹ�����ң��ID������ת�����㷨�����޷���ȷ����
							rc.iYxID=curRelay.ulYXID32;
							m_rtdbInf->getMainDb()->SendMessage(curRelay.izfProcessID, mes_RCExecute, sizeof(RControl), (char *)&rc);
						}

						SYSTEMTIME time;
						GetLocalTime(time);
						curRelay.exectime = time;
						dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,YKID32,&curRelay);//д��ң��ִ��ʱ��

						sendYkResponse(cmd->m_nComAsdu,CMD_ACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2E,sizeof(cmd->m_nValue));
						sendYkResponse(cmd->m_nComAsdu,CMD_DEACT_TERM,cmd->m_nInfoAddr,&cmd->m_nValue,0x2E,sizeof(cmd->m_nValue));
					}
					else//�����ڴ�ӡ������Ϣ
					{
						if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
						{
							memset(LastError,0,sizeof(LastError));
							sprintf_s(LastError,sizeof(LastError),"Exec remote single control command Exec failed. addr is %d,value=%d YKtoTC Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
							TRACE("Exec remote single control command select failed. addr is %d,value=%d YKtoTC Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);

						}
						if(true)
						{
							memset(LastError,0,sizeof(LastError));
							sprintf_s(LastError,sizeof(LastError),"Exec remote single control command Exec failed. addr is %d,value=%d YKtoTC Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
							TRACE("Exec remote single control command select failed. addr is %d,value=%d YKtoTC Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
					}
				}
				else//�����ڣ���ӡ������Ϣ
				{
					v_cmd->m_eState=eCmdStateEnd;
					if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Exec remote single control command Exec failed. addr is %d,value=%d YK Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
						TRACE("Exec remote single control command select failed. addr is %d,value=%d YK Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);

					}
					if(true)
					{
						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError),"Exec remote single control command Exec failed. addr is %d,value=%d YK Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
						TRACE("Exec remote single control command select failed. addr is %d,value=%d YK Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
					}
				}
				m_listCmd->removeCommand(v_cmd);
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
					int  nRet=sendYkResponse(cmd->m_nComAsdu,CMD_DEACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2E,sizeof(cmd->m_nValue));
					if (nRet==Q_104FAIL)
					{
						m_errorNo=e104Errno_CmdSend;
#ifdef _TEST104SLAVE
						sprintf_s(LastError, sizeof(LastError),"����˫��ң��ȡ��ȷ�ϳ�����ǰ�к�:%d\n",__LINE__);
						//OnErrorMsg(m_errorNo, LastError);nn
						bRet=false;
#endif
					}
					else
					{
						v_cmd->m_eState=eCmdStateYkCancelAct;
					}

					v_cmd->m_eState=eCmdStateEnd;
				}
			}
			break;
		}

	}
	else if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		switch (v_cmd->m_eState)
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

int Q_protocolIEC104Slave::handleCommmd_IC(Q_cmdIEC104 *v_cmd)
{
	int ret=Q_104SUC;

	v_cmd->m_bExec=true;
	switch (v_cmd->m_eState)
	{
	case eCmdStateIntRequest:
		{
			//��������ȷ����
			send_IC_Response(v_cmd->m_nComAsdu,CMD_ACT_CON);
			v_cmd->m_eState=eCmdStateIntRequestAct;//����ȷ�����
		}
		break;
	case eCmdStateIntRequestAct:
		{
			//��վ����������������
			SendInterrogateData(v_cmd);
			v_cmd->m_eState=eCmdStateIntSendData;
		}
		break;
	case eCmdStateIntSendData:
		{
			//��վ����������������
			SendInterrogateData(v_cmd);
		}
		break;

	case eCmdStateIntSendDataFin:
		{
			bZongZhaoComplete = true;
			send_IC_Response(v_cmd->m_nComAsdu,CMD_DEACT_TERM);//�������ٽ���
			v_cmd->m_eState=eCmdStateIntEnd;
			Q_IEC104CommandManager *cmdList=(Q_IEC104CommandManager *)m_pProcotolConfig->getProtocolCommandManager();
			if(cmdList!=NULL)
			{
				cmdList->removeCommand(v_cmd);
				//�Ƴ�����
			}
		}
		break;
	}

	return ret;
}
int Q_protocolIEC104Slave::handleCommmd_CS(Q_cmdIEC104 *v_cmd)
{
	int ret=Q_104SUC;

	v_cmd->m_bExec=true;

	//��̬����ʱ�䣬����ϵͳʱ�� TODO:
	Q_cmdIEC104_CS *cmd = (Q_cmdIEC104_CS *)v_cmd;
	v_cmd->m_eState=eCmdStateSyncAct;//����ȷ�����
	if (cmd!=NULL)
	{
#ifdef WINDOWS_ENV
		Q_CP56Time2a t2(cmd->m_cTime);
		SetLocalTime(&(t2._GetSystemTime(m_cfgSyncClock->m_nFixUsecs)));//��̬���������ӡ�
#endif
	}

	//����ʱ��ͬ��ȷ��
	if (m_cfgSyncClock->m_eActMethord=eCS_Act_Master)
	{
		send_CS_Response(cmd->m_nComAsdu,CMD_ACT_CON,cmd->m_cTime,sizeof(cmd->m_cTime));
	}
	else if (m_cfgSyncClock->m_eActMethord=eCS_Act_MasterFix)
	{
		//��ʱ��eCS_Act_Masterһ��
		send_CS_Response(cmd->m_nComAsdu,CMD_ACT_CON,cmd->m_cTime,sizeof(cmd->m_cTime));
	}
	else if (m_cfgSyncClock->m_eActMethord=eCS_Act_OwnSysBf)
	{
		//��ʱ��eCS_Act_Masterһ��
		send_CS_Response(cmd->m_nComAsdu,CMD_ACT_CON,cmd->m_cTime,sizeof(cmd->m_cTime));
	}
	else if (m_cfgSyncClock->m_eActMethord=eCS_Act_OwnSysAf)
	{
		//��ʱ��eCS_Act_Masterһ��
		send_CS_Response(cmd->m_nComAsdu,CMD_ACT_CON,cmd->m_cTime,sizeof(cmd->m_cTime));
	}
	v_cmd->m_bExec=true;
	v_cmd->m_eState=eCmdStateEnd;

	return Q_104SUC;
}

int Q_protocolIEC104Slave::handleCommmd_SE(Q_cmdIEC104 *v_cmd)//fs20150728
{
	int bRet=Q_104SUC;

	v_cmd->m_bExec=true;//��Ȼ�
	Q_cmdIEC104_SE *cmd=NULL;
	switch (v_cmd->m_eState)
	{
	case eCmdStateYkSelectCon:
		{
			cmd = (Q_cmdIEC104_SE *)v_cmd;
			v_cmd->m_nCurTryNum++;
			if(v_cmd->m_nCurTryNum>=v_cmd->m_nMaxTryCount)
			{
				v_cmd->m_nCurTryNum=0;

				if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
				{
					cmd = (Q_cmdIEC104_SE *)v_cmd;
					memset(LastError,0,sizeof(LastError));
					if(cmd!=NULL)
					{
						if(cmd->m_cFuncCode==0x30)
						{
							sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete command.Command info:addr=%d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						else if(cmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete command.Command info:addr=%d,value=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
					}
					else
					{
						sprintf_s(LastError,sizeof(LastError),"Beyond timeout,delete command.\n");
					}
					//OnShowMsg(LastError,sizeof(LastError));nn
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				if(true/*m_bInfoMsg*/)
				{
					//OnInfoMsg(LastError,sizeof(LastError));nn
					//�¹�Լ��ӡ
					SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
				}
				m_YtExeMode=eYtExecModeNull;

				m_listCmd->removeCommand(v_cmd);
			}
			//��ʱ�޷���ɾ������
			break;
		}
	case eCmdStateYkExecAct:
		{
			//׼��ִ������
			cmd = (Q_cmdIEC104_SE *)v_cmd;

			if(cmd!=NULL)
			{
				if(cmd->m_nExecFlg>0)
				{
					//��˶�޸�20150728
					//�ж�ң�����Ƿ����
					int nPos = 0;
					bool bExist = false;
					int YTtoYCID32 = -1;
					int	YTID32 = -1;	

					Q_cmdIEC104_SE *cmd = (Q_cmdIEC104_SE *)v_cmd;
					Q_RECORDDATATAG dataArray;
					SCADA_SetPoint curSetPoint;
					CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

					int nSetPointRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_SetPoint_ID );
					for (int j=0; j<nSetPointRecordCount; j++)
					{
						int nTempid32 = j+1;
						dbmg_sdb->GetElementByID(SCADA_TAB_SetPoint_ID, nTempid32,&curSetPoint);

						if (m_ProSetting->getChannelID() == curSetPoint.uTuID && v_cmd->m_nComAsdu == curSetPoint.lDPUID &&
							v_cmd->m_nInfoAddr == curSetPoint.iNoInDPU)
						{
							bExist = true;
							YTID32 = nTempid32;
							YTtoYCID32 = curSetPoint.ulYCID32;
							break;
						}
					}


					if (bExist)//����
					{
						int nYCbegin = m_tags->getAnalogBegin();
						int nYCend = m_tags->getAnalogEnd();

						if(cmd->m_cFuncCode==0x30||cmd->m_cFuncCode==0x31)
						{
							SCADA_Analog tempAnalog;
							dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);

							if (&tempAnalog != NULL)
							{
								//dataArray.channelID=tempAnalog.uTuID;
								//dataArray.device=tempAnalog.lDPUID;
								//dataArray.no=tempAnalog.iNoInDPU;

								//2016-02-23�޸�Ϊֱ��ʹ����������
								dataArray.eDataType=eDataTypeFloat;
								dataArray.channelID=tempAnalog.uTuID;
								dataArray.nIndex = tempAnalog.iIndex;
								//dataArray.fValue=(cmd->m_cOnOff)/tempAnalog.fCoeff;//fs20150824
								//dataArray.fValue=(cmd->m_cOnOff)*tempAnalog.fCoeff;//tjs20160621
								dataArray.fValue=cmd->m_cOnOff;//tjs20161215
								m_pMgrIEC104SlaveProtocolSetting->recordDataValue(&dataArray,1,true);//record����ˢ��ʱ���
							}

							if(curSetPoint.iZF)
							{
								RSetpoint rs;
								rs.iStation =curSetPoint.izfTuID;
								rs.iPoint =curSetPoint.izfNoInDPU;
								rs.iYcID=curSetPoint.ulYCID32;
								rs.iCommandType =MES_TYPE_SPOINT;//ң�������룬��ʱ����
								rs.fVal = cmd->m_cOnOff;
								rs.iDevice=curSetPoint.izfDPUID;
								rs.iZf=1;
								//2016-05-18 ���ӹ�����ң��ID������ת�����㷨�����޷���ȷ����
								rs.iYcID=curSetPoint.ulYCID32;
								m_rtdbInf->getMainDb()->SendMessage(curSetPoint.izfProcessID, mes_SPExecute, sizeof(RSetpoint), (char *)&rs);
							}

							SYSTEMTIME time;
							GetLocalTime(time);
							curSetPoint.exectime = time;
							curSetPoint.iexecret = 1;
							curSetPoint.fVal = float((cmd->m_cOnOff)*1);
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ��ʱ��

						}
						else if (cmd->m_cFuncCode==0x32)
						{
							SCADA_Analog tempAnalog;
							dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,YTtoYCID32,&tempAnalog);

							if (&tempAnalog != NULL)
							{
								//2016-02-23�޸�Ϊֱ��ʹ����������
								dataArray.eDataType=eDataTypeFloat;
								dataArray.channelID=tempAnalog.uTuID;
								dataArray.nIndex = tempAnalog.iIndex;
								//dataArray.fValue=(cmd->m_fVal)/tempAnalog.fCoeff;//fs20150824
								//dataArray.fValue=(cmd->m_fVal)*tempAnalog.fCoeff;//tjs20160621
								dataArray.fValue=cmd->m_fVal;//tjs20161215
								m_pMgrIEC104SlaveProtocolSetting->recordDataValue(&dataArray,1,true);//record����ˢ��ʱ���
							}

							if(curSetPoint.iZF)
							{
								RSetpoint rs;
								rs.iStation =curSetPoint.izfTuID;
								rs.iPoint =curSetPoint.izfNoInDPU;
								rs.iYcID=curSetPoint.ulYCID32;
								rs.iCommandType =MES_TYPE_SPOINT;//ң�������룬��ʱ����
								rs.fVal = cmd->m_fVal;
								rs.iDevice=curSetPoint.izfDPUID;
								rs.iZf=1;
								//2016-05-18 ���ӹ�����ң��ID������ת�����㷨�����޷���ȷ����
								rs.iYcID=curSetPoint.ulYCID32;
								m_rtdbInf->getMainDb()->SendMessage(curSetPoint.izfProcessID, mes_SPExecute, sizeof(RSetpoint), (char *)&rs);
							}

							SYSTEMTIME time;
							GetLocalTime(time);
							curSetPoint.exectime = time;
							curSetPoint.iexecret = 1;
							curSetPoint.fVal = (cmd->m_fVal)*1;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//д��ң��ִ��ʱ��
						}
					}
					else//�����ڣ���ӡ������Ϣ
					{
						v_cmd->m_eState=eCmdStateEnd;
						if(true==m_ProSetting->getPrintSetting(eComLogTypePrompt))
						{
							memset(LastError,0,sizeof(LastError));
							sprintf_s(LastError,sizeof(LastError),"Exec remote set point command fin.mode exec failed. addr is %d,value=%d YT Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
							TRACE("Exec remote single control command select failed. addr is %d,value=%d YT Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);

						}
						if(true)
						{
							memset(LastError,0,sizeof(LastError));
							sprintf_s(LastError,sizeof(LastError),"Exec remote set point command fin.mode exec failed. addr is %d,value=%d YT Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
							TRACE("Exec remote single control command select failed. addr is %d,value=%d YT Tag is not finded\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
					}

					m_listCmd->removeCommand(v_cmd);
					m_YtExeMode=eYtExecModeNull;
				}
				else
				{
					cmd->m_nExecFlg=1;
				}

			}
		}
		break;
	}
	return bRet;
}



int Q_protocolIEC104Slave::handleCommmd_RC(Q_cmdIEC104 *v_cmd)
{
	int bRet=Q_104SUC;

	v_cmd->m_bExec=true;//����

	if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		switch (v_cmd->m_eState)
		{
		case eCmdStateYkSelect:
			{
				//����ȷ��֡
				Q_cmdIEC104_RC *cmd = (Q_cmdIEC104_RC *)v_cmd;
				if (NULL != cmd)
				{
					if (sendYkResponse(cmd->m_nComAsdu,CMD_ACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2F,sizeof(cmd->m_nValue))==Q_104FAIL)
					{
						//���ͱ��ĳ���
						m_errorNo=e104Errno_CmdSend;
						v_cmd->m_eState=eCmdStateYkConfirm;
						v_cmd->m_bError=true;
						sprintf_s(LastError, sizeof(LastError),"������������,ѡ��ȷ�ϳ�����ǰ�к�:%d\n",__LINE__);
						//OnErrorMsg(m_errorNo, LastError);nn
						bRet=false;
					}
					else
					{
						v_cmd->m_eState=eCmdStateYkConfirm;
					}
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
					sprintf_s(LastError, sizeof(LastError),"ִ�������������ʵʱ�������ǰ�к�:%d\n",__LINE__);
					//OnErrorMsg(m_errorNo, LastError);nn
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
					int  nRet=sendYkResponse(cmd->m_nComAsdu,CMD_DEACT_CON,cmd->m_nInfoAddr,&cmd->m_nValue,0x2F,sizeof(cmd->m_nValue));
					if (nRet==Q_104FAIL)
					{
						m_errorNo=e104Errno_CmdSend;
#ifdef _TEST104SLAVE
						sprintf_s(LastError, sizeof(LastError),"������������ȡ��ȷ�ϳ�����ǰ�к�:%d\n",__LINE__);
						//OnErrorMsg(m_errorNo, LastError);nn
						bRet=false;
#endif
					}
					else
					{
						v_cmd->m_eState=eCmdStateYkCancelAct;
					}

					v_cmd->m_eState=eCmdStateEnd;
				}
			}
			break;
		}
	}
	else if (eStation_Master==m_cfgBase->m_eCurStType)
	{
		switch (v_cmd->m_eState)
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

int Q_protocolIEC104Slave::handleCommmd_AC(Q_cmdIEC104 *v_cmd)
{
	if (bZongZhaoComplete == 0)
	{
		return 0;
	}

	int channeltype = m_ProSetting->getChannelType();

	if(m_vecVarYC.size()<=0) 
		return 0;

	if (m_vecVarYC[0] == NULL)
	{
		return 0;
	}

	Q_cmdIEC104_AC *cmd = (Q_cmdIEC104_AC *)v_cmd;
	time_t tt;
	int temp[Q_FRAME_CONST25] = {};//�洢ʵʱ������
	int tempno = 0;
	Q_IEC104_ASDU msg;
	msg.header.commom_asdu_1=m_cfgBase->m_nAsduAddr & 0xFF;
	msg.header.commom_asdu_2=(m_cfgBase->m_nAsduAddr>>8) & 0xFF;
	msg.header.tx_cause_1 = 0x03;   //����ԭ��ͻ��---�̶�
	msg.header.tx_cause_2 = 0x00;

	BYTE tmp[4];

	int startno = m_tags->getAnalogBegin();
	int endno = m_tags->getAnalogEnd();

	tt= time(NULL);
	if (cmd->m_cOnOff == false || tt - cmd->timeflag < cmd->m_nTime)
	{
		return 0;
	}
	cmd->timeflag = time(NULL);

	SCADA_Analog *curAnalogAc=NULL;
	CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	if (ACsendno >= endno)
	{
		ACsendno = startno;
	}

	if(ACsendno==0)
		ACsendno=startno;

	float val=0;
	SCADA_Analog *zfAnalogAc=NULL;
	float vdiff=0;
	for (int i = ACsendno; i <= endno; i++)
	{
		int nTempid32 = ACsendno;
		if(dbmg_sdb->GetTabAnalogByID(nTempid32,curAnalogAc))
		{
			if(curAnalogAc==NULL)
				continue;

			vdiff=curAnalogAc->fVal;

			if(curAnalogAc->iZfID>0)
			{
				if(dbmg_sdb->GetTabAnalogByID(curAnalogAc->iZfID,zfAnalogAc))
				{
					if(curAnalogAc==NULL)
						continue;

					vdiff=zfAnalogAc->fVal;
				}
			}

			val= m_vecVarYC[ACsendno-startno]->value - vdiff;//ԭ����fRaw 2015-12-10 tjs

			if(val>curAnalogAc->fDeadSec||val<-curAnalogAc->fDeadSec)//�仯���������ж�
			{
				m_vecVarYC[ACsendno-startno]->value = vdiff;//���¸��ĺ��ֵ�������Ƿ��ϴ�tjs

				curAnalogAc->fVal=vdiff;

				SYSTEMTIME systime;
				GetLocalTime(systime);
				memcpy(&curAnalogAc->tSnapTime,&systime,sizeof(curAnalogAc->tSnapTime));

				temp[tempno] = i;
				tempno++;

				if(tempno >= Q_FRAME_CONST25)
				{
					ACsendno = i;
					break;
				}
			}
		}



		ACsendno++;
	}

	if(tempno<=0)
	{
		return true ;//û������
	}

	int begin=m_tags->getAnalogBegin();
	int idx=0;

	if(m_cfgSt->m_AsdnYcType==eAsduYcType13)
	{
		Q_MF104 meq;
		float fval=0;
		int n_idx=0;
		while (tempno >= m_cfgSt->n_YcNumPerFrame)
		{
			msg.header.qual =m_cfgSt->n_YcNumPerFrame&0xFF;      //�ɱ�ṹ�޶���---��֯���ĺ����
			msg.header.type = 0x0D;                              //�������ͣ�13���̸���---��ʱ�̶�

			for (int j = 0; j < m_cfgSt->n_YcNumPerFrame; j++)
			{
				int nTempid32 = temp[j+n_idx];
				dbmg_sdb->GetTabAnalogByID( nTempid32,curAnalogAc);
				//idx=curAnalogAc->iNoInDPU+m_cfgSt->m_YcBeginIndex-1; //ԭ���� 
				idx=curAnalogAc->iNoInDPU;
				//if (channeltype == 3)//��̬ͨ��
				//{
				//	idx=nTempid32+m_cfgSt->m_YcBeginIndex-1;
				//}

				meq.addr0=idx & 0xFF;
				meq.addr1=idx >>8 & 0xFF;
				meq.addr2=idx >>16 & 0xFF;

				memset(tmp,0,sizeof(tmp));
				fval=curAnalogAc->fVal*curAnalogAc->fCoeff;//��ǰ�ټ���ϵ����
				memcpy(tmp, &fval, sizeof(fval));

				if(m_cfgBase->m_eFloatSeq==eFloatSeqLittle)
				{
					meq.seqv.val[0]=tmp[3];
					meq.seqv.val[1]=tmp[2];
					meq.seqv.val[2]=tmp[1];
					meq.seqv.val[3]=tmp[0];
				}
				else
				{
					meq.seqv.val[0]=tmp[0];
					meq.seqv.val[1]=tmp[1];
					meq.seqv.val[2]=tmp[2];
					meq.seqv.val[3]=tmp[3];
				}

				memcpy(msg.data + j * sizeof(Q_MF104), &meq, sizeof(Q_MF104));
			}

			send_I_Msg((BYTE*)&msg, sizeof(meq)*m_cfgSt->n_YcNumPerFrame+6);	//��֯���ķ��� 
			tempno -= m_cfgSt->n_YcNumPerFrame;
			n_idx+=m_cfgSt->n_YcNumPerFrame;
		}

		if (tempno > 0 && tempno < m_cfgSt->n_YcNumPerFrame)
		{
			msg.header.qual = tempno;       //�ɱ�ṹ�޶���---��֯���ĺ����
			msg.header.type = 0x0D;         //�������ͣ�13���̸���---��ʱ�̶�

			for (int no = 0; no < tempno; no++)
			{
				int nTempid32 = temp[no+n_idx];
				dbmg_sdb->GetTabAnalogByID(nTempid32,curAnalogAc);
				//	idx=curAnalogAc->iNoInDPU+m_cfgSt->m_YcBeginIndex-1;
				idx=curAnalogAc->iNoInDPU;

				//if (channeltype == 3)//��̬ͨ��
				//{
				//	idx=nTempid32+m_cfgSt->m_YcBeginIndex-1;
				//}

				meq.addr0=idx & 0xFF;
				meq.addr1=idx >>8 & 0xFF;
				meq.addr2=idx >>16 & 0xFF;

				memset(tmp,0,sizeof(tmp));
				//	fval=curAnalogAc->fVal;//fRawԭ��*curAnalogAc->fCoeff
				fval=curAnalogAc->fVal*curAnalogAc->fCoeff;
				memcpy(tmp,&fval, sizeof(fval));

				if(m_cfgBase->m_eFloatSeq==eFloatSeqLittle)
				{
					meq.seqv.val[0]=tmp[3];//�ӵ͵���
					meq.seqv.val[1]=tmp[2];
					meq.seqv.val[2]=tmp[1];
					meq.seqv.val[3]=tmp[0];
				}
				else
				{
					meq.seqv.val[0]=tmp[0];
					meq.seqv.val[1]=tmp[1];
					meq.seqv.val[2]=tmp[2];
					meq.seqv.val[3]=tmp[3];
				}

				memcpy(msg.data + no*sizeof(Q_MF104),&meq,sizeof(Q_MF104));
			}

			send_I_Msg((BYTE*)&msg, sizeof(meq)*tempno+6);	//��֯���ķ���
			memset(temp, 0, sizeof(temp));	//����仯��������
			tempno = 0;
		}
		else 
		{
			memset(temp, 0, sizeof(temp));
			return 0;
		}
	}
	else if(m_cfgSt->m_AsdnYcType==eAsduYcType21)//21 fun code
	{
		msg.header.type=0x15 & 0xFF;
		//��˳����
		int pi=0;
		int n_idx = 0;
		Q_MF104_21 meq;
		SHORT nValue=0;
		//һ�������߲����� tjs 2014-3-14
		while (tempno >= m_cfgSt->n_YcNumPerFrame)
		{
			msg.header.qual = m_cfgSt->n_YcNumPerFrame&0xFF;         //�ɱ�ṹ�޶���---��֯���ĺ����

			for (int j = 0; j < m_cfgSt->n_YcNumPerFrame; j++)
			{
				int nTempid32 = temp[j+n_idx];
				dbmg_sdb->GetTabAnalogByID( nTempid32,curAnalogAc);
				//idx=curAnalogAc->iNoInDPU+m_cfgSt->m_YcBeginIndex-1;
				idx=curAnalogAc->iNoInDPU;
				//if (channeltype == 3)//��̬ͨ��
				//{
				//	idx=nTempid32+m_cfgSt->m_YcBeginIndex-1;
				//}

				meq.addr0=idx & 0xFF;
				meq.addr1=idx >>8 & 0xFF;
				meq.addr2=idx >>16 & 0xFF;

				memset(tmp,0,sizeof(tmp));

				nValue = (int)(curAnalogAc->fVal*curAnalogAc->fCoeff);//��fVal

				if(m_cfgBase->m_eIntSeq==eIntSeqLittle)
				{
					meq.seqv.val[0]=nValue&0xff;
					meq.seqv.val[1]=nValue>>8;
				}
				else if(m_cfgBase->m_eIntSeq==eIntSeqBig)
				{
					meq.seqv.val[0]=nValue>>8;
					meq.seqv.val[1]=nValue&0xff;
				}

				memcpy(msg.data + j * sizeof(meq), &meq, sizeof(meq));
			}

			send_I_Msg((BYTE*)&msg, sizeof(meq)*m_cfgSt->n_YcNumPerFrame+6);	//��֯���ķ��� 

			memset(temp, 0, sizeof(temp));	//����仯��������
			tempno -= m_cfgSt->n_YcNumPerFrame;
			n_idx+=m_cfgSt->n_YcNumPerFrame;

		}

		if (tempno > 0 && tempno < m_cfgSt->n_YcNumPerFrame)
		{
			msg.header.qual = tempno;         //�ɱ�ṹ�޶���---��֯���ĺ����

			for (int no = 0; no < tempno; no++)
			{
				int nTempid32 = temp[no+n_idx];
				dbmg_sdb->GetTabAnalogByID(  nTempid32,curAnalogAc);
				//idx=curAnalogAc->iNoInDPU+m_cfgSt->m_YcBeginIndex-1;
				idx=curAnalogAc->iNoInDPU;
				//if (channeltype == 3)//��̬ͨ��
				//{
				//	idx=nTempid32+m_cfgSt->m_YcBeginIndex-1;
				//}

				meq.addr0=idx & 0xFF;
				meq.addr1=idx >>8 & 0xFF;
				meq.addr2=idx >>16 & 0xFF;

				SHORT nValue = (int)(curAnalogAc->fVal*curAnalogAc->fCoeff);//ԭ����fRaw


				if(m_cfgBase->m_eIntSeq==eIntSeqLittle)
				{
					meq.seqv.val[0]=nValue&0xff;
					meq.seqv.val[1]=nValue>>8;
				}
				else if(m_cfgBase->m_eIntSeq==eIntSeqBig)
				{
					meq.seqv.val[1]=nValue&0xff;
					meq.seqv.val[0]=nValue>>8;
				}

				memcpy(msg.data + no*sizeof(Q_MF104_21),&meq,sizeof(Q_MF104_21));
			}

			send_I_Msg((BYTE*)&msg, sizeof(Q_MF104_21)*tempno+6);	//��֯���ķ���
			memset(temp, 0, sizeof(temp));	//����仯��������
			tempno = 0;
		}
		else 
		{
			memset(temp, 0, sizeof(temp));
			return 0;
		}
	}
	else if(m_cfgSt->m_AsdnYcType==eAsduYcType9)//9
	{
		msg.header.type=0x09 & 0xFF;
		//��˳����
		SHORT nValue =0;
		int n_idx = 0;

		//��˳����
		int pi=0;
		Q_MS104 meq;
		memset(&meq,0,sizeof(meq));

		//һ�������߲����� tjs 2014-3-14
		while (tempno >= m_cfgSt->n_YcNumPerFrame)
		{	
			msg.header.qual = m_cfgSt->n_YcNumPerFrame&0xFF;         //�ɱ�ṹ�޶���---��֯���ĺ����

			for (int j = 0; j < m_cfgSt->n_YcNumPerFrame; j++)
			{
				int nTempid32 = temp[j+n_idx];
				dbmg_sdb->GetTabAnalogByID(  nTempid32,curAnalogAc);
				//idx=curAnalogAc->iNoInDPU+m_cfgSt->m_YcBeginIndex-1;
				idx=curAnalogAc->iNoInDPU;
				//if (channeltype == 3)//��̬ͨ��
				//{
				//	idx=nTempid32+m_cfgSt->m_YcBeginIndex-1;
				//}

				meq.addr0=idx & 0xFF;
				meq.addr1=idx >>8 & 0xFF;
				meq.addr2=idx >>16 & 0xFF;

				memset(tmp,0,sizeof(tmp));

				nValue = (int)(curAnalogAc->fVal*curAnalogAc->fCoeff);//ԭ������fRaw

				if(m_cfgBase->m_eIntSeq==eIntSeqLittle)
				{
					meq.seqv.val[0]=nValue&0xff;
					meq.seqv.val[1]=nValue>>8;
				}
				else if(m_cfgBase->m_eIntSeq==eIntSeqBig)
				{
					meq.seqv.val[0]=nValue>>8;
					meq.seqv.val[1]=nValue&0xff;
				}

				memcpy(msg.data + j * sizeof(meq), &meq, sizeof(meq));
			}

			send_I_Msg((BYTE*)&msg, sizeof(meq)*m_cfgSt->n_YcNumPerFrame+6);	//��֯���ķ��� 
			memset(temp, 0, sizeof(temp));	//����仯��������
			tempno -= m_cfgSt->n_YcNumPerFrame;
			n_idx+=m_cfgSt->n_YcNumPerFrame;
		}

		if (tempno > 0 && tempno < m_cfgSt->n_YcNumPerFrame)
		{
			msg.header.qual = tempno;         //�ɱ�ṹ�޶���---��֯���ĺ����

			for (int no = 0; no < tempno; no++)
			{
				//Q_CommUtility::NU_ZERO(&meq,sizeof(meq));

				int nTempid32 = temp[no+n_idx];
				dbmg_sdb->GetTabAnalogByID( nTempid32,curAnalogAc);
				//idx=curAnalogAc->iNoInDPU+m_cfgSt->m_YcBeginIndex-1;
				idx=curAnalogAc->iNoInDPU;
				//if (channeltype == 3)//��̬ͨ��
				//{
				//	idx=nTempid32+m_cfgSt->m_YcBeginIndex-1;
				//}

				meq.addr0=idx & 0xFF;
				meq.addr1=idx >>8 & 0xFF;
				meq.addr2=idx >>16 & 0xFF;

				nValue = (int)(curAnalogAc->fVal);;

				if(m_cfgBase->m_eIntSeq==eIntSeqLittle)
				{
					meq.seqv.val[0]=nValue&0xff;
					meq.seqv.val[1]=nValue>>8;
				}
				else if(m_cfgBase->m_eIntSeq==eIntSeqBig)
				{
					meq.seqv.val[0]=nValue>>8;
					meq.seqv.val[1]=nValue&0xff;
				}

				memcpy(msg.data + no*sizeof(meq),&meq,sizeof(meq));
			}

			send_I_Msg((BYTE*)&msg, sizeof(meq)*tempno+6);	//��֯���ķ���

			memset(temp, 0, sizeof(temp));
			tempno = 0;
		}
		else 
		{
			memset(temp, 0, sizeof(temp));
			return 0;
		}
	}
	return 0;
}


int Q_protocolIEC104Slave::handleCommmd_BC(Q_cmdIEC104 *v_cmd)
{
	if (bZongZhaoComplete == 0)
	{
		return 0;
	}

	if(m_vecVarYX.size()<=0)
		return 0;

	if (m_vecVarYX[0] == NULL)
	{
		return 0;
	}

	int channeltype = m_ProSetting->getChannelType();

	Q_cmdIEC104_AC *cmd = (Q_cmdIEC104_AC *)v_cmd;
	time_t tt;
	tt= time(NULL);
	//TRACE("handleCommmd_BC");

	CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();


	if (cmd->m_cOnOff == false || tt - cmd->timeflag < cmd->m_nTime)
	{
		return 0;
	}

	cmd->timeflag = time(NULL);
	int temp[Q_FRAME_CONST32] = {};
	int tempno = 0;
	Q_IEC104_ASDU msg;


	//�ж��Ƿ��б仯
	//�ж��Ƿ��б仯
	int startno = m_tags->getPointBegin();
	int endno = m_tags->getPointEnd();


	if (BCsendno >= endno)
	{
		BCsendno = startno;
	}
	if(BCsendno<=0)
		BCsendno=startno;
	SCADA_Digit *curDigitBC=NULL;
	SCADA_Digit *zfDigitBC=NULL;
	for (int i = BCsendno; i <= endno; i++)
	{

		if (BCsendno < 0)
		{
			return 0;
		}

		int nId32 = BCsendno;
		if(dbmg_sdb->GetTabDigitByID(nId32, curDigitBC))
		{
			curDigitBC->iVal==1?true:false;
			if(curDigitBC->iZfID>0)
			{
				if(dbmg_sdb->GetTabDigitByID(curDigitBC->iZfID,zfDigitBC))
				{
					curDigitBC->iVal=zfDigitBC->iVal;
					SYSTEMTIME systime;
					GetLocalTime(systime);
					memcpy(&curDigitBC->tSnapTime,&systime,sizeof(curDigitBC->tSnapTime));
				}

			}
		}


		bool btemp = curDigitBC->iVal==1?true:false;
		if(m_vecVarYX[BCsendno-startno]->value != btemp)
		{
			m_vecVarYX[BCsendno-startno]->value = curDigitBC->iVal==1?true:false;
			temp[tempno] = i;//
			tempno++;
			if(tempno >= Q_FRAME_CONST32)
			{
				BCsendno= i + 1;
				break;
			}
		}
		BCsendno++;
	}

	//Q_SP104 siq;
	Q_SP104 siqsets;
	if (tempno > 0)
	{
		msg.header.commom_asdu_1 = cmd->asdu_addr;//������ַ
		msg.header.commom_asdu_2 = 0x00;
		msg.header.qual = tempno;         //�ɱ�ṹ�޶���---��֯���ĺ����
		msg.header.tx_cause_1 = 0x03;   //����ԭ��ͻ��---�̶�
		msg.header.tx_cause_2 = 0x00;
		msg.header.type = 0x01;         //�������ͣ�1


		int begin=m_tags->getPointBegin();
		int idx=0;


		for (int no = 0; no < tempno; no++)
		{
			//Q_CommUtility::NU_ZERO(&siq,sizeof(siq));


			int nTempid32 = temp[no];
			if(dbmg_sdb->GetTabDigitByID( nTempid32,curDigitBC))
			{
				idx=curDigitBC->iNoInDPU;
			}
			else
			{
				continue;
			}



			//if (channeltype == 3)//��̬ͨ��
			//{
			//	idx=nTempid32+m_cfgSt->m_YxBeginIndex-1;
			//}

			//			idx=cur[temp[no]].nIndex + cmd->yxbegin-begin-1;

			siqsets.addr0=idx & 0xFF;
			siqsets.addr1=idx >>8 & 0xFF;
			siqsets.addr2=idx >>16 & 0xFF;
			memset(&siqsets.siq,0,sizeof(siqsets.siq));
			//			siq.siq.spi = cur[temp[no]].bValue;
			siqsets.siq.spi = curDigitBC->iVal==1?1:0;

			memcpy(msg.data + no*sizeof(siqsets),&siqsets,sizeof(siqsets));

			//			cur[temp[no]].changeflag = false;
			//��¼SOE����
			m_vecVarSOE[nTempid32-startno]->value = siqsets.siq.spi==1?true:false;	
			GetLocalTimeMs(m_vecVarSOE[nTempid32-startno]->timeSOE);		
			m_vecVarSOE[nTempid32-startno]->changed = true;

			if (no == 59)
			{
				send_I_Msg((BYTE*)&msg, 4*no+6);	//��֯���ķ���
				memcpy(temp, &temp[60], tempno - 60);
				tempno -= 60;
				TRACE("delete curDigitBC");
				return 0;
			}
		}
		send_I_Msg((BYTE*)&msg, 4*tempno + 6);	//��֯���ķ���	
	}

	memset(temp, 0, sizeof(temp));	//����仯��������
	tempno = 0;

	//TRACE("delete curDigitBC");
	return 0;
}

int Q_protocolIEC104Slave::handleCommmd_BC_SOE(Q_cmdIEC104 *v_cmd)
{

	if (bZongZhaoComplete == 0)
	{
		return 0;
	}

	if(m_vecVarSOE.size()<=0)
		return 0;

	if (m_vecVarSOE[0] == NULL)
	{
		return 0;
	}

	Q_cmdIEC104_AC *cmd = (Q_cmdIEC104_AC *)v_cmd;
	if(!(cmd->m_cSendSOE))
	{
		return 0;
	}
	time_t tt;
	tt= time(NULL);
	////TRACE("handleCommmd_BC_SOE");
	if (cmd->m_cOnOff == false || tt - cmd->timeflag < cmd->m_nTime)
	{
		return 0;
	}

	cmd->timeflag = time(NULL);
	Q_IEC104_ASDU msg;

	int startno = m_tags->getPointBegin();
	int endno = m_tags->getPointEnd();


	if (SOEsendno >= endno)
	{
		SOEsendno = startno;
	}
	if(SOEsendno<=0 || SOEsendno<startno)
	{
		SOEsendno=startno;
	}
	int nTempid32=-1;
	for(;SOEsendno<endno; SOEsendno++)
	{
		if(m_vecVarSOE[SOEsendno-startno]->changed)
		{
			nTempid32=m_vecVarSOE[SOEsendno-startno]->id;
			m_vecVarSOE[SOEsendno-startno]->changed=false;
			break;
		}
	}
	if(nTempid32>0)
	{
		CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
		CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
		SCADA_Digit *curDigitBC=NULL;
		int idx = -1;
		if(dbmg_sdb->GetTabDigitByID( nTempid32,curDigitBC))
		{
			idx=curDigitBC->iNoInDPU;				
		}
		//Q_SP104 siq;	
		if (idx >= 0)
		{
			Q_SP104 siqsets;
			msg.header.commom_asdu_1 = cmd->asdu_addr;//������ַ
			msg.header.commom_asdu_2 = 0x00;
			msg.header.qual = 0x01;         //�ɱ�ṹ�޶���---��֯���ĺ����
			msg.header.tx_cause_1 = 0x03;   //����ԭ��ͻ��---�̶�
			msg.header.tx_cause_2 = 0x00;
			msg.header.type = 0x1e;         //�������ͣ�1

			siqsets.addr0=idx & 0xFF;
			siqsets.addr1=idx >>8 & 0xFF;
			siqsets.addr2=idx >>16 & 0xFF;
			memset(&siqsets.siq,0,sizeof(siqsets.siq));
			siqsets.siq.spi = m_vecVarSOE[SOEsendno-startno]->value==true?1:0;
			memcpy(msg.data, &siqsets,sizeof(siqsets));			

			int num = 4;
			SYSTEMTIME tSOE = m_vecVarSOE[SOEsendno-startno]->timeSOE;
			WORD ms = tSOE.wMilliseconds + tSOE.wSecond*1000;			
			msg.data[num++] =  (ms&0x00ff);
			msg.data[num++] =  (ms&0xff00) >> 8;
			msg.data[num++] =  tSOE.wMinute;
			msg.data[num++] =  tSOE.wHour;
			msg.data[num++] =  tSOE.wDay | (((tSOE.wDayOfWeek) << 5) & 0x00E0);
			msg.data[num++] =  tSOE.wMonth;
			msg.data[num++] =  tSOE.wYear - 2000;
			send_I_Msg((BYTE*)&msg, 4 + 6 + 7);	//��֯���ķ���	

		}
	}

	//TRACE("delete curDigitBC");
	return 0;
}

int Q_protocolIEC104Slave::SendInterrogateData(Q_cmdIEC104 *v_cmd)
{
	//����ң��
	int i=0;
	Q_IEC104_ASDU msg;
	msg.header.qual=0x00; //˳��������
	msg.header.tx_cause_1=0x14 & 0xFF;
	msg.header.tx_cause_2=0x00 & 0xFF;
	//	msg.header.commom_asdu_1=v_cmd->m_nComAsdu & 0xFF;
	//	msg.header.commom_asdu_2=(v_cmd->m_nComAsdu>>8) & 0xFF;

	msg.header.commom_asdu_1=m_cfgBase->m_nAsduAddr & 0xFF;
	msg.header.commom_asdu_2=(m_cfgBase->m_nAsduAddr>>8) & 0xFF;
	//fs20150822��վ���ٻ���ַ�б������þ���

	Q_cmdIEC104_IC *cmd=(Q_cmdIEC104_IC *)v_cmd;
	if(cmd!=NULL)
	{
		//����������Ϣ ��������˳�� ң�š�ң�⡢ң��
		//ң���������͡���װ����������
		//ң���������͡���װ����������
		//ң���������͡���װ����������
		//
		//���㴦��ң��
		//
		int len=0;
		if(!cmd->m_bflgSendDigital)
		{
			msg.header.type=0x01 & 0xFF;
			//���� ȫ��Ĭ������ѭַ ������ʼ��ַ��
			if(m_tags!=NULL)
				len=m_tags->getPointCount();

			if(len>cmd->m_nDigitalPos+m_cfgSt->n_YxNumPerFrame)
			{
				SendIC_SP_Res(&msg,cmd->m_nDigitalPos,m_cfgSt->n_YxNumPerFrame);
				cmd->m_nDigitalPos+=m_cfgSt->n_YxNumPerFrame;
			}
			else
			{
				SendIC_SP_Res(&msg,cmd->m_nDigitalPos,len-cmd->m_nDigitalPos);
				cmd->m_nDigitalPos+=len-cmd->m_nDigitalPos;
				cmd->m_bflgSendDigital=true;
			}
		}
		else
		{
			//������ң��Ȼ����ң����Ϣ
			if(!cmd->m_bflgSendAnlog)
			{
				//���� ȫ��Ĭ������ѭַ ������ʼ��ַ��
				if(m_tags!=NULL)
					len=m_tags->getAnalogCount();

				printf("Interrogate SendIC_ME_Res count=%d\n",len);
				if(len>cmd->m_nAnalogPos+m_cfgSt->n_YcNumPerFrame)
				{
					SendIC_ME_Res(&msg,cmd->m_nAnalogPos,m_cfgSt->n_YcNumPerFrame);
					cmd->m_nAnalogPos+=m_cfgSt->n_YcNumPerFrame;
				}
				else
				{
					SendIC_ME_Res(&msg,cmd->m_nAnalogPos,len-cmd->m_nAnalogPos);
					cmd->m_nAnalogPos+=len-cmd->m_nAnalogPos;
					cmd->m_bflgSendAnlog=true;
					cmd->m_eState=eCmdStateIntSendDataFin;// ���ٷ����������
				}
			}
		}
	}
	return Q_104SUC;
}

int Q_protocolIEC104Slave::SendIC_SP_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount)//fs20150727
{

	//2016-11-8 tjs begin
	if(m_tags->getPointCount()<=1)
	{
		printf("SendIC_SP_Res this channel data is empty in digit table");
		return 0;
	}
	//2016-11-8 tjs begin

	int channeltype = m_ProSetting->getChannelType();

	msg->header.qual=nCount & 0x7F;//��˳���


	CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	int begin = m_cfgSt->m_YxBeginIndex;;
	int t_nIdx= nIdx+m_cfgSt->m_YxBeginIndex;//�ϴ������ݵ�ַ

	int nDigitBeginid32 = m_tags->getPointBegin();

	SCADA_Digit BeginDigit;
	dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID,nDigitBeginid32,&BeginDigit);

	//int nFirstinfoaddr = BeginDigit.iNoInDPU;
	begin =  nDigitBeginid32;

	t_nIdx= nIdx+BeginDigit.iNoInDPU;


	SCADA_Digit *curDigitBC=NULL;
	SCADA_Digit *zfDigitBC=NULL;

	//��˳����
	int pi=0;
	//Q_SP104  siq;
	Q_SP104  siqsets;
	for (int idx=t_nIdx,ii=nIdx;idx<t_nIdx+nCount;idx++,ii++)
	{
		//		Q_CommUtility::NU_ZERO(&siq,sizeof(siq));

		//		int nTempid32 = ii+begin;
		//		dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID, nTempid32,curDigit);

		int ninfoaddr = ii+begin;

		//Q_RECORDDATATAG2 dataArray;
		//dataArray.val.eDataType=eDataTypeBit;
		//dataArray.val.channelID=m_ProSetting->getChannelID();
		//dataArray.val.device=msg->header.commom_asdu_1;
		//dataArray.val.no = ninfoaddr;
		//dataArray.val.fValue = 0;

		//if (channeltype == 3)//��̬ͨ��
		//{
		//	m_pMgrIEC104SlaveProtocolSetting->evalDataValue(&dataArray,1,3,ninfoaddr-begin,2);
		//}	
		//else
		//{
		//	m_pMgrIEC104SlaveProtocolSetting->evalDataValue(&dataArray,1);
		//}

		if(dbmg_sdb->GetTabDigitByID(ninfoaddr, curDigitBC))
		{
			if(curDigitBC->iZfID>0)
			{
				if(dbmg_sdb->GetTabDigitByID(curDigitBC->iZfID,zfDigitBC))
				{
					curDigitBC->iVal=zfDigitBC->iVal;
					SYSTEMTIME systime;
					GetLocalTime(systime);
					memcpy(&curDigitBC->tSnapTime,&systime,sizeof(curDigitBC->tSnapTime));
				}

			}
		}

		int niRaw = curDigitBC->iVal;

		siqsets.addr0=(idx) & 0xFF;
		siqsets.addr1=(idx) >>8 & 0xFF;
		siqsets.addr2=(idx) >>16 & 0xFF;

		memset(&siqsets.siq,0,sizeof(siqsets.siq));
		if (niRaw == 1)
		{
			siqsets.siq.spi=1;
		}
		else
		{
			siqsets.siq.spi=0;
		}
		memcpy(&msg->data[pi],&siqsets,sizeof(siqsets));

		pi+=sizeof(Q_SP104);
	}
	//p->LeaveMutex();

	send_I_Msg((BYTE*)msg, nCount*sizeof(Q_SP104)+6);


	//����仯��������tjs 2014-7-23	
	SCADA_Digit curDigit;
	int nbegin=m_tags->getPointBegin();
	t_nIdx=nIdx+m_cfgSt->m_YxBeginIndex;

	for (int idx=t_nIdx,ii=nIdx;idx<t_nIdx+nCount;idx++,ii++)
	{
		int nTempid32 = ii+nbegin;
		dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID, nTempid32,&curDigit);	
		m_vecVarYX[ii]->value=curDigit.iVal==1?true:false;
	}
	return Q_104SUC;
}

/*
int Q_protocolIEC104Slave::SendIC_DP_Res(Q_IEC104_ASDU *msg,vector<Q_ST_104PT *> &vTable,const int &nIdx,const int &nCount)
{
UINT len=(UINT)vTable.size();
msg->header.qual=msg->header.qual | len & 0xFF;

int pi=0;
msg->data[pi++]=vTable[0]->ioa & 0xFF;
msg->data[pi++]=vTable[0]->ioa>>8 & 0xFF;
msg->data[pi++]=vTable[0]->ioa>>16 & 0xFF;

Q_DIQ104 diq;
Q_dataManager* p = _GlobalData.getDataManager();

//��Ϣ��Ӧ�ĵ���Ѿ���vTable�д����ˣ����Ǵ�
Q_RECORDDATATAG qData;
qData.eDataType=eDataTypeInt;

for (UINT i=0;i<len;i++)
{
Q_CommUtility::NU_ZERO(&diq,sizeof(diq));

qData.nIndex=vTable[nIdx]->rid;
p->evalData(&qData);
diq.dpi=qData.nValue;

msg->data[pi]=*((BYTE *)&diq);
pi+=sizeof(Q_DIQ104);
}
send_I_Msg((BYTE*)msg, len+6);
return Q_104SUC;
}
*/

int Q_protocolIEC104Slave::SendIC_ME_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount)//fs20150727
{
	msg->header.qual=0x7f&nCount;
	//2016-11-8 tjs begin

	if(m_tags->getAnalogCount()<=1)
	{
		printf("SendIC_ME_Res this channel data is empty in analog table\n");
		return 0;
	}

	//2016-11-8 tjs begin
	int channeltype = m_ProSetting->getChannelType();
	//	Q_dataManager* p = _GlobalData.getDataManager();
	//  p->EnterMutex();

	CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	int nAnalogBeginid32 = m_tags->getAnalogBegin();

	SCADA_Analog BeginAnalog;
	dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,nAnalogBeginid32,&BeginAnalog);

	int nFirstinfoaddr = BeginAnalog.iNoInDPU;//16385

	int t_nIdx= nIdx+nFirstinfoaddr;

	printf("SendIC_ME_Res count=%d",nCount);

	//if (channeltype == 3)//��̬ͨ��
	//{
	//	t_nIdx=nIdx+m_cfgSt->m_YcBeginIndex;
	//}
	//else
	//{
	//	t_nIdx= nIdx+nFirstinfoaddr;	
	//}
	//

	SCADA_Analog *curAnalogAc=NULL;
	SCADA_Analog *zfAnalogAc=NULL;
	if(m_cfgSt->m_AsdnYcType==eAsduYcType13)
	{
		msg->header.type=0x0D & 0xFF;
		//��˳����
		int pi=0;
		Q_MF104 meq;
		BYTE tmp[4];

		float fval=0.0;
		for (int idx=t_nIdx,ii=nIdx;idx<t_nIdx+nCount;idx++,ii++)
		{

			//			Q_CommUtility::NU_ZERO(&meq,sizeof(meq));
			//			int nTempid32 = ii+begin;
			//			dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID, nTempid32,curAnalog);

			int nVID = ii+nAnalogBeginid32;

			/*	Q_RECORDDATATAG2 dataArray;
			dataArray.val.eDataType=eDataTypeFloat;
			dataArray.val.channelID=m_ProSetting->getChannelID();
			dataArray.val.device=msg->header.commom_asdu_1;
			dataArray.val.no = ninfoaddr;
			dataArray.val.fValue = 0;*/

			if(dbmg_sdb->GetTabAnalogByID(nVID,curAnalogAc))
			{
				if(curAnalogAc==NULL)
					continue;

				fval=curAnalogAc->fVal;

				if(curAnalogAc->iZfID>0)
				{
					if(dbmg_sdb->GetTabAnalogByID(curAnalogAc->iZfID,zfAnalogAc))
					{
						fval=zfAnalogAc->fVal;
						curAnalogAc->fVal=fval;
					}
				}
			}

			SYSTEMTIME systime;
			GetLocalTime(systime);
			memcpy(&curAnalogAc->tSnapTime,&systime,sizeof(curAnalogAc->tSnapTime));
			//if (channeltype == 3)//��̬ͨ��
			//{
			//	m_pMgrIEC104SlaveProtocolSetting->evalDataValue(&dataArray,1,3,ii+1,1);
			//}	
			//else
			//{
			//	m_pMgrIEC104SlaveProtocolSetting->evalDataValue(&dataArray,1);
			//}

			meq.addr0=(idx) & 0xFF;
			meq.addr1=(idx) >>8 & 0xFF;
			meq.addr2=(idx) >>16 & 0xFF;

			memset(tmp,0,sizeof(tmp));
			fval=fval*curAnalogAc->fCoeff;
			memcpy(tmp,&fval,sizeof(fval));

			if(m_cfgBase->m_eFloatSeq==eFloatSeqLittle)
			{
				meq.seqv.val[0]=tmp[3];
				meq.seqv.val[1]=tmp[2];
				meq.seqv.val[2]=tmp[1];
				meq.seqv.val[3]=tmp[0];
			}
			else
			{
				meq.seqv.val[0]=tmp[0];
				meq.seqv.val[1]=tmp[1];
				meq.seqv.val[2]=tmp[2];
				meq.seqv.val[3]=tmp[3];

			}

			memcpy(&msg->data[pi],&meq,sizeof(meq));

			pi+=sizeof(Q_MF104);


		}
		send_I_Msg((BYTE*)msg, nCount*sizeof(Q_MF104)+6);
	}
	else if(m_cfgSt->m_AsdnYcType==eAsduYcType21)//21 fun code
	{
		msg->header.type=0x15 & 0xFF;
		//��˳����
		int pi=0;
		Q_MF104_21 meq;

		//int begin=nFirstinfoaddr;

		//if (channeltype == 3)//��̬ͨ��
		//{			
		//	begin=m_cfgSt->m_YcBeginIndex;
		//}
		float fval=0.0;
		for (int idx=t_nIdx,ii=nIdx;idx<t_nIdx+nCount;idx++,ii++)
		{
			int nVID = ii+nAnalogBeginid32;

			/*Q_RECORDDATATAG2 dataArray;
			dataArray.val.eDataType=eDataTypeFloat;
			dataArray.val.channelID=m_ProSetting->getChannelID();
			dataArray.val.device=msg->header.commom_asdu_1;
			dataArray.val.no = ninfoaddr;
			dataArray.val.fValue = 0;*/

			//if (channeltype == 3)//��̬ͨ��
			//{
			//	m_pMgrIEC104SlaveProtocolSetting->evalDataValue(&dataArray,1,3,ii+1,1);
			//}	
			//else
			//{
			//	m_pMgrIEC104SlaveProtocolSetting->evalDataValue(&dataArray,1);
			//}

			if(dbmg_sdb->GetTabAnalogByID(nVID,curAnalogAc))
			{
				if(curAnalogAc==NULL)
					continue;

				fval=curAnalogAc->fVal;

				if(curAnalogAc->iZfID>0)
				{
					if(dbmg_sdb->GetTabAnalogByID(curAnalogAc->iZfID,zfAnalogAc))
					{
						fval=zfAnalogAc->fVal;
						curAnalogAc->fVal=fval;
					}
				}
			}

			SYSTEMTIME systime;
			GetLocalTime(systime);
			memcpy(&curAnalogAc->tSnapTime,&systime,sizeof(curAnalogAc->tSnapTime));

			meq.addr0=(idx) & 0xFF;
			meq.addr1=(idx) >>8 & 0xFF;
			meq.addr2=(idx) >>16 & 0xFF;

			//			if(cur[ii+begin].factor!=0)

			SHORT nValue = (int)(fval*curAnalogAc->fCoeff);
			if(m_cfgBase->m_eIntSeq==eIntSeqLittle)
			{
				meq.seqv.val[0]=nValue&0xff;
				meq.seqv.val[1]=nValue>>8;
			}
			else if(m_cfgBase->m_eIntSeq==eIntSeqBig)
			{
				meq.seqv.val[0]=nValue>>8;
				meq.seqv.val[1]=nValue&0xff;
			}


			memcpy(&msg->data[pi],&meq,sizeof(meq));

			pi+=sizeof(Q_MF104_21);

		}
		send_I_Msg((BYTE*)msg, nCount*sizeof(Q_MF104_21)+6);

	}
	else if(m_cfgSt->m_AsdnYcType==eAsduYcType9)//9
	{
		msg->header.type=0x09 & 0xFF;
		//��˳����
		int pi=0;
		Q_MS104 meq;
		//int begin=nFirstinfoaddr;

		//if (channeltype == 3)//��̬ͨ��
		//{			
		//	begin=m_cfgSt->m_YcBeginIndex;
		//}
		float fval=0.0;
		for (int idx=t_nIdx,ii=nIdx;idx<t_nIdx+nCount;idx++,ii++)
		{

			//			Q_CommUtility::NU_ZERO(&meq,sizeof(meq));
			int nVID = ii+nAnalogBeginid32;
			/*Q_RECORDDATATAG2 dataArray;
			dataArray.val.eDataType=eDataTypeFloat;
			dataArray.val.channelID=m_ProSetting->getChannelID();
			dataArray.val.device=msg->header.commom_asdu_1;
			dataArray.val.no = ninfoaddr;
			dataArray.val.fValue = 0;*/

			//if (channeltype == 3)//��̬ͨ��
			//{
			//	m_pMgrIEC104SlaveProtocolSetting->evalDataValue(&dataArray,1,3,ii+1,1);
			//}	
			//else
			//{
			//	m_pMgrIEC104SlaveProtocolSetting->evalDataValue(&dataArray,1);
			//}
			if(dbmg_sdb->GetTabAnalogByID(nVID,curAnalogAc))
			{
				if(curAnalogAc==NULL)
					continue;

				fval=curAnalogAc->fVal;

				if(curAnalogAc->iZfID>0)
				{
					if(dbmg_sdb->GetTabAnalogByID(curAnalogAc->iZfID,zfAnalogAc))
					{
						fval=zfAnalogAc->fVal;
						curAnalogAc->fVal=fval;
					}
				}
			}
			meq.addr0=(idx) & 0xFF;
			meq.addr1=(idx) >>8 & 0xFF;
			meq.addr2=(idx) >>16 & 0xFF;

			SYSTEMTIME systime;
			GetLocalTime(systime);
			memcpy(&curAnalogAc->tSnapTime,&systime,sizeof(curAnalogAc->tSnapTime));

			SHORT nValue = (int)(fval*curAnalogAc->fCoeff);
			if(m_cfgBase->m_eIntSeq==eIntSeqLittle)
			{
				meq.seqv.val[0]=nValue&0xff;
				meq.seqv.val[1]=nValue>>8;
			}
			else if(m_cfgBase->m_eIntSeq==eIntSeqBig)
			{
				meq.seqv.val[0]=nValue>>8;
				meq.seqv.val[1]=nValue&0xff;
			}


			memcpy(&msg->data[pi],&meq,sizeof(meq));

			pi+=sizeof(Q_MS104);

		}
		send_I_Msg((BYTE*)msg, nCount*sizeof(Q_MS104)+6);
	}

	//����仯��������tjs 2014-7-23
	SCADA_Analog curAnalog;
	int nbegin=m_tags->getAnalogBegin();
	t_nIdx=nIdx+m_cfgSt->m_YcBeginIndex;
	for (int idx=t_nIdx,ii=nIdx;idx<t_nIdx+nCount;idx++,ii++)
	{
		int nTempid32 = ii+nbegin;
		dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID, nTempid32,&curAnalog);
		m_vecVarYC[ii]->value=curAnalog.fVal;
	}

	return Q_104SUC;
}

int Q_protocolIEC104Slave::sendYtCommand(const int &asdu,const int &cause,Q_IEC104_ST_C_SE *st)
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

int Q_protocolIEC104Slave::sendYtCommand3(const int &asdu,const int &cause,Q_IEC104_ST_C_SE3 *st)
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

void Q_protocolIEC104Slave::SetSigExecCmd(int MsgID)
{
	m_nExecCmdMsgID=MsgID;
}

void Q_protocolIEC104Slave::SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen)
{
	//TRACE("Q_protocolIEC104Slave::SendMsgPrint,m_pMgrIEC104SlaveProtocolSetting=%08x\n",m_pMgrIEC104SlaveProtocolSetting);

	g_rtdbInfIEC104 = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
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
	else if (nmsgtype == eMsgTypeMsg || nmsgtype == eMsgTypeError|| nmsgtype == eMsgTypeCh)//��ʾ��Ϣ����Ϣֵ��������Ϣ�����ݽ��͡���Լ����
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

				//��Ϊ���־�Ȼ�����������ͣ������ǲ���ȷ�� tjs 2016-10-28

				//������RtServer������Ϣ

				//TRACE("Q_protocolIEC104Slave::send message2 process id=%d\n",pProcess->iProcessID);
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

			//TRACE("Q_protocolIEC104Slave::send message3 process id=%d",pProcess->iProcessID);
			m_rtdbInf->getMainDb()->SendMessageTo(pProcess->ID32,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
			/*char strMsg[200] = {0};
			sprintf_s(strMsg,sizeof(strMsg),"MsgPrint:stid = %d msgtype id = %d message = %s",MsgPrint.ichannelid,MsgPrint.msgtype,MsgPrint.msg);*/
			//printf("%s\n",strMsg);
			//TRACE("MsgPrint:stid = %d msgtype id = %d message = %s",MsgPrint.ichannelid,MsgPrint.msgtype,MsgPrint.msg);
		}
	}

}
