#include "Q_protocolIEC103.h"
#include "Q_channelBase.h"
#include "tinyxmlinclude.h"
#include "gSystemDef.h"
#include <sys/types.h>

Q_protocolIEC103::Q_protocolIEC103()
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
Q_protocolIEC103::~Q_protocolIEC103()
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

	//Q_IEC103CommandManager *m_listCmd;//�����б� //����Э�����ͷ�
	//Q_103StandardTags* m_tags;//���ݱ����
}

bool Q_protocolIEC103::startProcess()
{
	if(m_ProSetting->getPrintSetting(eComLogTypePrompt))
	{
		sprintf_s(LastError,sizeof(LastError),"��ʹ����ʼ...");
		OnShowMsg(LastError,sizeof(LastError));
	}


	m_eProtocolStatus = eCommonStatusNormal;
	if(sendStartDT()==0)
	{
		Q_IEC103StandardCommand *pCommand=new Q_IEC103StandardCommand();
		pCommand->setCommonType(eCommandTypeCycle);
		pCommand->setCommand103Type(e103CmdTypeTestFR);
		m_listCmd->addCommand(pCommand);

		if(m_cfgBase->m_bSendIC==true)
		{
			//���ٻ�
			Q_cmdIEC103 *pCommandIC=new Q_cmdIEC103();
			pCommandIC->setCommonType(eCommandTypeCycle);
			pCommandIC->setCommand103Type(e103_CALL_ALL_DATA);
			pCommandIC->m_eState=e103CmdStateIntRequest;
			pCommandIC->setMaxTestNum(50);//tjs 2013-4-7
			pCommandIC->m_nComAsdu=m_cfgBase->m_nAsduAddr;
			pCommandIC->m_bExec=false;
			m_listCmd->addCommand(pCommandIC);
		}

		return true;
	}
	else
		return false;
	return true;

}
bool Q_protocolIEC103::stopProcess()
{
	m_eProtocolStatus = eCommonStatusNull;
	//m_listCmd->clearCommand(); //2013-02-24 ���buffer
	//bStartFlg=false;

	if(m_ProSetting->getPrintSetting(eComLogTypePrompt))
	{
		sprintf_s(LastError,sizeof(LastError),"�Ͽ�����...");
		OnShowMsg(LastError,sizeof(LastError));
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


bool Q_protocolIEC103::explainData()
{
	bool bRet=true;
	Q_commonBuffer *recvTool=NULL;

	if (m_pProcotolConfig!=NULL)
	{
		recvTool=m_pProcotolConfig->getReceiveBuffer();
		if (recvTool!=NULL)
		{
			int curLen=recvTool->getDataLength();//��ǰ������������

			while (curLen>MAX_103_PACKET_NUM)
			{
				//����������
				bRet=handleAnalyseData(recvTool);
				if (false==bRet)
					break;
				curLen=recvTool->getDataLength();
			}

			if (curLen>=MIN_103_FRAME_NUM)
			{
				bRet=handleAnalyseData(recvTool);
			}
			if (curLen <= 0)
			{
				Q_103NoDataTimes++;
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
bool Q_protocolIEC103::handleAnalyseData(Q_commonBuffer *recvTool)
{
	bool bRet=true;

	char msgBuf[Q_FRAME_CONST2048];
	//Q_CommUtility::NU_ZERO(msgBuf,sizeof(msgBuf));
	memset(msgBuf,0,sizeof(msgBuf));
	int res=0;
	res=recvTool->readData(msgBuf,sizeof(msgBuf),false);

	//onRxMsg((BYTE*)&msgBuf[0], res);
	if(res<0)
	{
		if(m_ProSetting->getPrintSetting(eComLogTypeTotal)==true)
		{
			sprintf_s(LastError,sizeof(LastError),"Read data exception,buffer size too small,Please add receive buffer size.close link\n");
			OnShowMsg(LastError,sizeof(LastError));
		}
		recvTool->clearBuffer();//�������
		//closeLink();
		return false;

	}

	Q_103NoDataTimes = 0;//�յ����ݣ���ʶ����

	int j   = 0;
	int len = 0;//14�������������ֽڼ�0x68����Ҫ��2
	while (j < res)
	{

		if (msgBuf[j] == 0x10 && msgBuf[j+4] == 0x16)
		{
			onRxMsg((BYTE*)&msgBuf[j], 5);
			BYTE byte = msgBuf[j + 1];
			decodeUmsg(byte, 5);
			//rec_short_msg(byte,5);
			//decodeUmsg((BYTE*)&msgBuf[j], 5);

			recvTool->removeData(5);
			j+=5;
		}

		else if (msgBuf[j] == 0x68 && msgBuf[j+3] == 0x68)
		{
			//֡У��
			len = msgBuf[j+1]&0xFF; //����ԭ������&0xFF���ֳ���127��ò�����ȷ������
			if (len+6 > res-j)
			{
				recvTool->removeData(res - j - 1);
				j = res;
				break;//�������ݲ��㣬������
			}			
			else if(len>0)
			{
				if (((msgBuf[j+4]>>5)&0x01) == 1)
				{
					//ACD = 1 �ٻ�һ������
					next_command_flag = e103_CALL_ONE_DATA;
				}
				Q_IEC103vfyRet ret=verifyFrame((BYTE*)&msgBuf[j],len+6);

				if (j+len+6 <= res) //���������ܹ�����һ��֡��С
				{
					// call callback
					m_bSendTestFR=false;

					onRxMsg((BYTE*)&msgBuf[j], len+6);

					if (e103FrameType_I==ret.m_eFrameType)
					{
						decodeImsg((BYTE*)&msgBuf[j+6], len);
					}

					else
					{
						//������쳣����
						//I��ʽ���������ر�����
						bRet=false;
						//���֡�������ݴ�С���󡢷�����š�������ŵȴ���ر���·

						m_errorNo=e103ErrNo_FrameVerify;
						OnError(m_errorNo,__LINE__);
						//closeLink();
						break;
					}
				}
				else if(len==0)
				{
					TRACE("-------find packet len is zero\n-----");
				}
				recvTool->removeData(len+6);//ɾ����������
				j = j + len + 2;
			}
		}

		else
		{
			j++;
		}
	}
	return bRet;
}
bool Q_protocolIEC103::initializeProtocol()
{

	m_ProSetting = (Q_IEC103Setting *)(m_pProcotolConfig->getProtocolSetting());

	if (NULL==m_cfgBase)
	{
		m_cfgBase=new Q_103BaseCfg;

		if (NULL != m_ProSetting)
		{
			//m_ProSetting->getBaseSetting(m_cfgBase
			m_ProSetting->getBaseSetting(m_cfgBase);

			if (NULL==m_cfgSt)
				m_cfgSt=new Q_103StCfg;

			m_ProSetting->getStationSetting(m_cfgSt);

			if (NULL==m_cfgControl)
				m_cfgControl=new Q_103CtrlCfg;

			m_ProSetting->getControlSetting(m_cfgControl);


			if (NULL==m_cfgSyncClock)
				m_cfgSyncClock=new Q_103SyncClockCfg;

			m_ProSetting->getSyncClockSetting(m_cfgSyncClock);
		}
	}
	if (NULL==m_st)
	{
		m_st=new Q_IEC103_Struct;
		m_st->nW=m_cfgBase->m_nW;
		m_st->nK=m_cfgBase->m_nK;

		if (m_pProcotolConfig->getProtocolCommandManager()!=NULL)
		{
			m_listCmd = (Q_IEC103CommandManager *)(m_pProcotolConfig->getProtocolCommandManager());
			if (m_listCmd==NULL)
			{
				m_errorNo=e103ErrNo_CmdListNull;
				OnError(m_errorNo,__LINE__);

				return false;
			}
		}
		else
		{
			m_errorNo=e103ErrNo_CmdListNull;
			OnError(m_errorNo,__LINE__);

			return false;
		}

	}
	m_cpuEndian=eBigEndian;
	/*if (Q_CommUtility::isBigEndian())
	{
	m_cpuEndian=eBigEndian;
	}
	else
	{
	m_cpuEndian=eLittleEndian;
	}*/
	m_tags=(Q_103StandardTags*)m_ProSetting->getTagsManager();


	return true;
}
Q_IEC103vfyRet Q_protocolIEC103::verifyFrame(BYTE* msgbuf,UINT len)
{
	Q_IEC103vfyRet ret;
	ret.m_bSuc=true;

	BYTE ucTemp_Length;
	if ( len> (UINT)m_cfgBase->m_nMaxFrameLen || len<MIN_103_FRAME_NUM)
	{
		ret.m_bSuc=false;
		ret.m_nErrorCode=e103ErrNo_FrameNum;
		return ret;
	}

	ucTemp_Length = len & 0xff;

	Q_IEC103_APCI *p = (Q_IEC103_APCI*) &msgbuf[0];
	if (p->start1 == p->start2 && p->start1 == 0x68)  //FORMAT_I  ��֡
	{

		ret.m_eFrameType=e103FrameType_I;

		if (ucTemp_Length < 12) //ASDU I ֡Ӧ�ôﵽһ����ʼ����
		{
			ret.m_bSuc=false;
			ret.m_nErrorCode=e103ErrNo_FrameNum;
			return ret;
		}

		//vfy asdu
		Q_IEC103_ASDU_header *header=(Q_IEC103_ASDU_header*)msgbuf;

		int asdu =0;

		asdu=header->commom_asdu_1;


		if (asdu!=0xFF && asdu!=0xFFFF)
		{
			if (m_cfgBase->m_nAsduAddr!=asdu)
			{
				ret.m_bSuc=false;
				ret.m_nErrorCode=e103ErrNo_MisAsdu;
				return ret;
			}
		}

		//vfy asdu end
		ret.m_bSuc=true;
		return  ret;
	}

	return ret;
}

void Q_protocolIEC103::onTxMsg(BYTE *msgbuf, unsigned int len)
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
			//::SendMessage(m_ProSetting->getWndHwnd(),m_ProSetting->getWndMsgID(),(WPARAM)&log,(LPARAM)sizeof(log));
		}
	}

}
void Q_protocolIEC103::onRxMsg(BYTE *msgbuf, unsigned int len)
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
			
			//::SendMessage(m_ProSetting->getWndHwnd(),m_ProSetting->getWndMsgID(),(WPARAM)&log,(LPARAM)sizeof(log));
		}
	}

}
void Q_protocolIEC103::OnErrorMsg(char *msg,unsigned int len)
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
			//::SendMessage(m_ProSetting->getWndHwnd(),m_ProSetting->getWndMsgID(),(WPARAM)&log,(LPARAM)sizeof(log));
		}
	}
}
void Q_protocolIEC103::OnShowMsg(char *msg,unsigned int len)
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
			//::SendMessage(m_ProSetting->getWndHwnd(),m_ProSetting->getWndMsgID(),(WPARAM)&log,(LPARAM)sizeof(log));
		}
	}
}
void Q_protocolIEC103::RestartCh()
{	
	time(&m_nTimerConfirm);
	sleepflag = 20;
// 	m_clientChannel = new Q_channelBase();
// 	m_clientChannel->checkStatus();
	sprintf_s(LastError,sizeof(LastError),"Read data exception.close link channel: %d\n",m_cfgBase->m_nAsduAddr);
	OnShowMsg(LastError,sizeof(LastError));
	return;
}
// bool Q_protocolIEC103::getSockConnectStatus()
// {
// 	return m_eProtocolStatus==eCommonStatusNormal?true:false;
// }
void Q_protocolIEC103::setDefaultValue(void)
{
	time(&m_nTimerConnect);//m_nTimerConnect=0;
	time(&m_nTimerConfirm);//=0;
	time(&m_nTimer_S_Ack);//=0;
	m_bTimer_S_Ack=0;
	time(&m_nTimer_U_Test);//=0;
	this_command_flag = 0;
	next_command_flag = e103_INIT_CHANNEL;
	m_bSendTestFR=false;
	m_bStopSend=false; //Ĭ�ϲ��ܷ������� //����Ϊ Ĭ�Ϸ������� 2012-05-02
	FCB = false;
	FCV = true;
	Q_103NoDataTimes = 0;
	Q_103TWODataTimes = 0;
	sleepflag = 0;
}
bool Q_protocolIEC103::makeFrame()
{
	time(&now);
	if (now - m_nTimerConfirm < sleepflag)
	{
		return true;
	}

	if (next_command_flag == e103_CALL_TWO_DATA || next_command_flag == e103_INIT_CHANNEL)
	{
		m_nTimer_S_Ack = now - m_nTimerConnect;
		if (m_nTimer_S_Ack > 120)
		{
			next_command_flag = e103_CALL_ALL_DATA;
			m_nTimerConnect = now;
		}
	}
	if (Q_103NoDataTimes > 10)
	{
		RestartCh();
		Q_103NoDataTimes = 0;
		return true;
	}
	else if (Q_103NoDataTimes > 6 && next_command_flag == e103_CALL_TWO_DATA)
	{
		next_command_flag = e103_INIT_CHANNEL;
	}

	if(Q_103TWODataTimes > 10 && next_command_flag == e103_CALL_TWO_DATA)
	{
		next_command_flag = e103_CALL_ONE_DATA;
	}

	switch (next_command_flag)
	{
	case e103_INIT_CHANNEL:
		//��ʼ��
		send_S_Msg();
		Q_103NoDataTimes++;
		this_command_flag = e103_INIT_CHANNEL;
		break;
	case e103_CALL_ALL_DATA:
		//���ٻ�
		send_I_Msg();
		time(&now);
		m_nTimerConnect = now;
		this_command_flag = e103_CALL_ALL_DATA;
		break;
	case e103_CALL_ONE_DATA:
		//һ������
		send_U_Msg(10);
		Q_103TWODataTimes = 0;
		this_command_flag = e103_CALL_ONE_DATA;
		break;
	case e103CmdTypeYT:
		send_YT_preset(yt_device, yt_dotno, yt_value);
		this_command_flag = e103CmdTypeYT;
		break;
	case e103CmdTypeC_RC:
		send_YK_preset(yk_device, yk_dotno, yk_value);
		this_command_flag = e103CmdTypeC_RC;
		break;
	case e103CmdTypePlanCurve:
		send_YT_exe(yt_device);
		this_command_flag = e103CmdTypePlanCurve;
		break;
	case e103CmdTypeC_EI:
		send_YK_exe(yk_device, yk_dotno, yk_value);
		this_command_flag = e103CmdTypeC_EI;
		break;
	default:
		//��������
		send_U_Msg(11);
		Q_103TWODataTimes++;
		Q_103NoDataTimes++;
		this_command_flag = e103_CALL_TWO_DATA;
		break;
	}
	//sleep(10);//2016-12-14 yy ��ԭ���� 50 ��Ϊ 10   2016-12-21 ��sleep������ɶϿ��ͻ�������ʱǰ�ó�����������˴�ʹ��sleepҲû�е�������ע�� yy
	return true;
}

int Q_protocolIEC103::send_I_Msg()
{
	BYTE m_transceiveBuf[15];
	int addr = 0;
	addr = m_cfgBase->m_nAsduAddr;
	BYTE CODE = 0x00;
	if (FCB)
	{
		CODE = 0x40 + 0x10 + 0x03;
		FCB = false;
	}
	else
	{
		CODE = 0x40 + 0x10 + 0x20 + 0x03;
		FCB = true;
	}
	m_transceiveBuf[0] = 0x68;
	m_transceiveBuf[1] = 0x09;
	m_transceiveBuf[2] = 0x09;
	m_transceiveBuf[3] = 0x68;
	m_transceiveBuf[4] = CODE;//CODE
	m_transceiveBuf[5] = 0x00;//BYTE(addr);
	m_transceiveBuf[6] = 0x07;//TYPE ���ٻ�
	m_transceiveBuf[7] = 0x81;//VSQ 1��
	m_transceiveBuf[8] = 0x09;//����ԭ�� ���ٻ�
	m_transceiveBuf[9] = 0x00;//BYTE(addr);
	m_transceiveBuf[10] = 0xFF;// FUN
	m_transceiveBuf[11] = 0x00;// INF
	m_transceiveBuf[12] = 0x01;
	BYTE byte = CalculateCheck(&m_transceiveBuf[4],9);
	m_transceiveBuf[13] = byte;
	m_transceiveBuf[14] = 0x16;

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		bool ret=pSendTool->writeData((char*)m_transceiveBuf,15);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
			return ret;
		}
		onTxMsg(&m_transceiveBuf[0],15);
		next_command_flag = e103_CALL_TWO_DATA;

	}
	next_command_flag = e103_CALL_TWO_DATA;
	return 0;
}

int Q_protocolIEC103::send_U_Msg(int cmd)
{
	BYTE m_transceiveBuf[5];
	BYTE CODE = 0x00;
	int addr = 0;
	addr = m_cfgBase->m_nAsduAddr;
	if (FCB)
	{
		CODE = 0x40 + 0x10;
		FCB = false;
	}
	else
	{
		CODE = 0x40 + 0x10 + 0x20;
		FCB = true;
	}

	CODE = CODE + cmd;
	m_transceiveBuf[0] = 0x10;
	m_transceiveBuf[1] = CODE;
	m_transceiveBuf[2] = 0x00;//(BYTE)addr;
	BYTE byte=CalculateCheck(&m_transceiveBuf[1],2);
	m_transceiveBuf[3] = byte;
	m_transceiveBuf[4] = 0x16;

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		bool ret=pSendTool->writeData((char*)m_transceiveBuf,5);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
			return ret;
		}
		next_command_flag = e103_CALL_TWO_DATA;
		onTxMsg(&m_transceiveBuf[0],5);

	}
	next_command_flag = e103_CALL_TWO_DATA;
	return 0;
}

int Q_protocolIEC103::send_S_Msg(void)
{
	BYTE m_transceiveBuf[5];
	int addr = 0;
	addr = m_cfgBase->m_nAsduAddr;
	m_transceiveBuf[0] = 0x10;
	m_transceiveBuf[1] = 0x40;
	m_transceiveBuf[2] = 0x00;//BYTE(addr);
	BYTE byte = CalculateCheck(&m_transceiveBuf[1],2);
	m_transceiveBuf[3] = byte;
	m_transceiveBuf[4] = 0x16;

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		bool ret=pSendTool->writeData((char*)m_transceiveBuf,5);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
			return ret;
		}

	}
	onTxMsg(&m_transceiveBuf[0],5);
	next_command_flag = e103_CALL_ALL_DATA;
	return 0;
}
//
//	START Command
//
int Q_protocolIEC103::sendStartDT(void)
{
	return 0;
}

//
//	STOP Command
//
int Q_protocolIEC103::sendStopDT(void)
{
	return 0;
}

//
//	TEST Frame Command
//
int Q_protocolIEC103::sendTestFR(void)
{
	m_bSendTestFR=true;
	return 0;

}
void Q_protocolIEC103::writefile(char *log)
{
	/*2016-12-08 yy ����104��Լ ���˷���ע�͵�
	try
	{
		CString strFile;
		char buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, MAX_PATH);
		strFile = CString(buffer);
		strFile = strFile.Left(strFile.ReverseFind('\\'));
		sprintf_s(buffer,MAX_PATH,"%s\\log\\tmp.log",strFile);


		FILE *file = NULL;
		errno_t err = fopen_s(&file,buffer,"a+");
		if(file == NULL)
			return;
		CTime curTime = CTime::GetCurrentTime();

		char text[256];
		sprintf_s(text,"(%d-%.2d-%.2d %.2d:%.2d%.2d)--%s\n",curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond(),log);
		fputs(text,file);

		fclose(file);
	}
	catch (...)
	{
	}*/
}
void Q_protocolIEC103::OnError(int code,int lineNum)
{
	char str[200];
	switch(code)
	{
	case e103ErrNo_CmdListNull:
		sprintf_s(str,sizeof(str),"������Ϊ:%d,��Ϣ:�õ������б�ʧ��,�к�%d\n",code,lineNum);
		break;
	case e103ErrNo_FrameVerify:
		sprintf_s(str,sizeof(str),"������Ϊ:%d,��Ϣ:֡У��ʧ��,�к�%d\n",code,lineNum);
		break;
	case e103ErrNo_BeyondK:
		sprintf_s(str,sizeof(str),"������Ϊ:%d,ֹͣ�������ݣ���ǰ���͵�δȷ�ϵ����ݸ�������K,�к�%d\n",code,lineNum);
		break;

	}

	OnErrorMsg(str,strlen(str));
}
int Q_protocolIEC103::decodeImsg(BYTE *msgbuf, const UINT len)
{
	Q_IEC103_ASDU_header *header = NULL;
	header = (Q_IEC103_ASDU_header *)msgbuf;

	switch (header->type)
	{
	case 10:
		if (len > 25)
		{
			Rec_TCP_Data(msgbuf);
		}
		break;
	case 40:
		Rec_CHange_YX(msgbuf);
		break;
	case 41:
		Rec_SOE(msgbuf);
		break;
	case 44:
		Rec_All_YX(msgbuf);
		break;
	case 50:
		Rec_All_YC(msgbuf);
		break;
	default:
		break;
	}

	return 0;
}
// void Q_protocolIEC103::rec_short_msg(BYTE CODE, const UINT len)
// {
// 	//byte CODE = *(msgbuf+1);
// 	if (((CODE>>5)&0x01) == 1)
// 	{
// 		//ACD = 1 �ٻ�һ������
// 		next_command_flag = e103_CALL_ONE_DATA;
// 	}
// 	BYTE fun_code = CODE & 0x0F;
// 	switch (fun_code)
// 	{
// 	case 0:
// 		next_command_flag = e103_CALL_TWO_DATA;
// 		break;
// 	case 1:
// 
// 		break;
// 	case 8:
// 
// 		break;
// 	case 9:
// 
// 		break;
// 	case 11:
// 
// 		break;
// 	default:
// 		break;
// 	}
// 	return;
// }
int Q_protocolIEC103::decodeUmsg(BYTE CODE, const UINT len)
{
	if (this_command_flag == e103_INIT_CHANNEL)
	{
		next_command_flag = e103_CALL_ALL_DATA;
		return 0;
	}
	//byte CODE = *(msgbuf+1);
	int fun_code = CODE & 0x0F;

	switch (fun_code)
	{
	case 0:
		next_command_flag = e103_CALL_TWO_DATA;
		if (((CODE>>5)&0x01) == 1)
		{
			//ACD = 1 �ٻ�һ������
			next_command_flag = e103_CALL_ONE_DATA;
		}
		else if ((CODE&0x01) == 0)
		{
			//��ʼ���ɹ������ٻ�
			next_command_flag = e103_CALL_ALL_DATA;
		}
		break;
	case 1:
		if (this_command_flag == e103CmdTypeYT)
		{
			next_command_flag = e103CmdTypePlanCurve;
		}
		else if (this_command_flag == e103CmdTypeC_RC)
		{
			next_command_flag = e103CmdTypeC_EI;
		}
		break;
	case 8:

		break;
	case 9:
		next_command_flag = e103_CALL_TWO_DATA;
		break;
	case 11:

		break;
	default:
		break;
	}
	return 0;
}
void Q_protocolIEC103::Rec_All_YC(BYTE *msgbuf)
{
	int pos =1;
	DWORD adr;//�����ַ
	USHORT tempval = 0;
	int val = 0;

	int vsq = (*(msgbuf + 1))&0x80;
	int info_num = (*(msgbuf + 1))&0x7F;

	if (vsq == 0) //����
	{
		adr = *(msgbuf + 5);//ң����ʱֻ��һ��FUN-----������Ŀ

		while (pos <= info_num)
		{
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeFloat;

			if(m_tags!=NULL)
			{
				if(m_tags->fillTagInformation(&dataArray,adr,*(msgbuf + 3))==true)
				{
					Q_dataManager* p = _GlobalData.getDataManager();
					if (dataArray.nIndex >=0  && dataArray.nIndex < p->getAnalogCount())
					{
						float factor=p->getAnalogData()[dataArray.nIndex].factor;
						tempval = *(msgbuf + (2*pos+5)) * 256 + *(msgbuf + (2*pos+4));
						if ((tempval & 0x8000) == 0)
						{
							val = (tempval>>3)&0x0FFF;

						}
						else if ((tempval & 0x8000) == 0x8000)
						{
							val = -1 * (~((tempval>>3)&0x0FFF));
						}
						dataArray.fValue=val*factor;	
						p->recordDataValue(&dataArray,1);

					}		
				}
				else
				{
					sprintf(LastError,"���ݹ�����Ϣ���ַ%d,��ϢԪ�صĵ�ַ%d����,δ�ҵ������ݵ�����",*(msgbuf + 3),adr);
					OnErrorMsg(LastError,sizeof(LastError));
				}
			}
			adr++;
			pos++;
		}

	}
	else //asdu50ȫң��һ����˳���
	{
		return;
	}

	return;
}
void Q_protocolIEC103::Rec_All_YX(BYTE *msgbuf)
{
	int pos = 1;
	int info_num = *(msgbuf + 1)&0x7F;
	USHORT tempval;
	DWORD adr;
	adr = *(msgbuf + 5);//ң����ʱֻ��һ��FUN-----������Ŀ
	Q_RECORDDATATAG dataArray;
	dataArray.eDataType=eDataTypeBit;
	while(pos <= info_num)
	{
		tempval = (*(msgbuf + (pos*5 + 1)) & 0xFF)+(*(msgbuf + (pos*5 + 2)) & 0xFF)*256;
		for (int i = 0; i < 16; i++)
		{
			if (m_tags!=NULL)
			{
				if (m_tags->fillTagInformation(&dataArray,adr+i,*(msgbuf + 3))==true)
				{
					Q_dataManager* p = _GlobalData.getDataManager();
					if (dataArray.nIndex >=0 && dataArray.nIndex < p->getDigitalCount())
					{
						dataArray.bValue=((tempval>>i)&0x0001)==1?true:false;
						p->recordDataValue(&dataArray,1);
					}
				}
			}
		}
		pos++;
		adr+=16;
	}

	return;
}
void Q_protocolIEC103::Rec_CHange_YX(BYTE *msgbuf)
{
	DWORD adr;
	BYTE tempval;
	adr = *(msgbuf + 5);
	tempval = *(msgbuf + 6);
	Q_RECORDDATATAG dataArray;
	dataArray.eDataType=eDataTypeBit;

	if (m_tags!=NULL)
	{
		if (m_tags->fillTagInformation(&dataArray,adr,*(msgbuf + 3))==true)
		{
			Q_dataManager* p = _GlobalData.getDataManager();
			if (dataArray.nIndex >=0 && dataArray.nIndex < p->getDigitalCount())
			{
				dataArray.bValue = (tempval & 0x0001) == 1?true:false;
				p->recordDataValue(&dataArray,1);
			}
		}
	}
	return;
}
void Q_protocolIEC103::Rec_SOE(BYTE *msgbuf)
{
	return;
}
void Q_protocolIEC103::Rec_TCP_Data(BYTE *msgbuf)
{
	int group_no = 0;
	int seq_no = 0;
	int data_type = 0;
	int data_num = 0;
	int data_width = 0;
	int tempbval = 0;
	int adr = 0;

	group_no = *(msgbuf + 15);
	seq_no = *(msgbuf + 16);
	data_type = *(msgbuf + 18);
	data_num = *(msgbuf + 14);//��������
	data_width = *(msgbuf + 19) + 6;

	for (int i = 0; i< data_num - 1; i++)
	{		
		seq_no = *(msgbuf + 16 + i * data_width);
		if (data_type == 7)//ң��
		{
			adr = group_no * 256 + seq_no;
			Q_MF103Seq *pf=(Q_MF103Seq *)(msgbuf + 21 + i * data_width);
			float *val = (float *)&pf->val[0];
			
			
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeFloat;

			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=adr;
			dataArray.no=m_cfgBase->m_nAsduAddr;
			dataArray.fValue=*val;

			//printf("--------Q_protocolIEC104Master::func_M_ME_NC_1-----333 %d,%d,%d,%f-----\n",
			//	dataArray.channelID,dataArray.device,dataArray.no,dataArray.fValue);
			//ϵ��ͳһ��recordDataValue���������� comment by tjs 2016-06-21
			if(!m_pMgr->recordDataValue(&dataArray,1))
			{

			}
				

			if (m_tags!=NULL)
			{
				/*
				//if (m_tags->fillTagInformation(&dataArray,adr,m_cfgBase->m_nAsduAddr)==true)
				//{
					Q_dataManager* p = _GlobalData.getDataManager();
					if (dataArray.nIndex >=0  && dataArray.nIndex < p->getAnalogCount())
					{
						float factor=p->getAnalogData()[dataArray.nIndex].factor;

						dataArray.fValue= (*val)*factor;	
						dataArray.fValue= (*val)*factor;	
						p->recordDataValue(&dataArray,1);

					}	
				//}
				*/
			}
		}
		else if (data_type == 18)//ң��
		{
			adr = group_no * 256 + seq_no;
			tempbval = *(msgbuf + 21 + i*data_width);
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeBit;

			/*
			if (m_tags!=NULL)
			{
				if (m_tags->fillTagInformation(&dataArray,adr,m_cfgBase->m_nAsduAddr)==true)
				{
					Q_dataManager* p = _GlobalData.getDataManager();
					if (dataArray.nIndex >=0 && dataArray.nIndex < p->getDigitalCount())
					{
						dataArray.bValue = (tempbval & 0x0001) == 0?true:false;
						p->recordDataValue(&dataArray,1);
					}
				}
			}
			*/

			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=adr;
			dataArray.no=m_cfgBase->m_nAsduAddr;
			dataArray.bValue = (tempbval & 0x0001) == 0?true:false;
			if(!m_pMgr->recordDataValue(&dataArray,1))
			{

			}
		}
	}

	return;
}
BYTE Q_protocolIEC103::CalculateCheck( BYTE *msgbuf ,int num)
{
	BYTE temp = 0;

	for(int i=0; i <num ; i++)
		temp=temp+(*(msgbuf+i));

	return(temp);
}
void Q_protocolIEC103::setYTvalue(int device, int dotno, float value)
{
	yt_device = device;
	yt_dotno = dotno;
	yt_value = value;
}
void Q_protocolIEC103::setYKstate(int device, int dotno, bool value)
{
	yk_device = device;
	yk_dotno = dotno;
	yk_value = value;
}
void Q_protocolIEC103::send_YT_preset(int device, int dotno, float value)
{
	BYTE m_transceiveBuf[24];

	BYTE *buf=0;
	buf = (BYTE *)(&value);
	BYTE CODE = 0x00;
	if (FCB)
	{
		CODE = 0x40 + 0x10 + 0x03;
		FCB = false;
	}
	else
	{
		CODE = 0x40 + 0x10 + 0x20 + 0x03;
		FCB = true;
	}
	m_transceiveBuf[0] = 0x68;
	m_transceiveBuf[1] = 0x12;
	m_transceiveBuf[2] = 0x12;
	m_transceiveBuf[3] = 0x68;
	m_transceiveBuf[4] = CODE;//CODE
	m_transceiveBuf[5] = BYTE(device);
	m_transceiveBuf[6] = 0x0A;//ͨ��д��Ŀ
	m_transceiveBuf[7] = 0x81;//VSQ 1��
	m_transceiveBuf[8] = 0x28;//����ԭ�� ͨ�÷���д��Ŀ
	m_transceiveBuf[9] = BYTE(device);
	m_transceiveBuf[10] = 0xFE;// FUN
	m_transceiveBuf[11] = 0xF9;// INF   (F9)��ȷ�ϵ�д��Ŀ��(FA)����ȷ�ϵ�д��Ŀ
	m_transceiveBuf[12] = 0x2A;//RII
	m_transceiveBuf[13] = 0x81;//NGD
	m_transceiveBuf[14] = (BYTE)(dotno/256);
	m_transceiveBuf[15] = (BYTE)(dotno%256);
	m_transceiveBuf[16] = 0x01;
	m_transceiveBuf[17] = 0x07;//3--���Σ�7--�̸����ͣ�8--��������
	m_transceiveBuf[18] = *(buf);
	m_transceiveBuf[19] = *(buf + 1);
	m_transceiveBuf[20] = *(buf + 2);
	m_transceiveBuf[21] = *(buf + 3);

	BYTE byte = CalculateCheck(&m_transceiveBuf[4],18);
	m_transceiveBuf[22] = byte;
	m_transceiveBuf[23] = 0x16;

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		bool ret=pSendTool->writeData((char*)m_transceiveBuf,24);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
		}
		return;
	}
	next_command_flag = e103_CALL_TWO_DATA;
	return;
}
void Q_protocolIEC103::send_YK_preset(int device, int dotno, bool value)
{
	BYTE m_transceiveBuf[16];
	BYTE DCC = 0x00;
	if (value)
	{
		DCC = 0x82;
	}
	else
		DCC = 0x81;

	BYTE CODE = 0x00;
	if (FCB)
	{
		CODE = 0x40 + 0x10 + 0x03;
		FCB = false;
	}
	else
	{
		CODE = 0x40 + 0x10 + 0x20 + 0x03;
		FCB = true;
	}
	m_transceiveBuf[0] = 0x68;
	m_transceiveBuf[1] = 0x0A;
	m_transceiveBuf[2] = 0x0A;
	m_transceiveBuf[3] = 0x68;
	m_transceiveBuf[4] = CODE;//CODE
	m_transceiveBuf[5] = BYTE(device);
	m_transceiveBuf[6] = 0x40;//
	m_transceiveBuf[7] = 0x01;//VSQ 1��
	m_transceiveBuf[8] = 0x0C;//����ԭ�� 
	m_transceiveBuf[9] = BYTE(device);
	m_transceiveBuf[10] = 0x01;// FUN
	m_transceiveBuf[11] = BYTE(dotno);// INF   
	m_transceiveBuf[12] = DCC;//DCC//����
	m_transceiveBuf[13] = 0x00;//RII

	BYTE byte = CalculateCheck(&m_transceiveBuf[4],10);
	m_transceiveBuf[14] = byte;
	m_transceiveBuf[15] = 0x16;

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		bool ret=pSendTool->writeData((char*)m_transceiveBuf,16);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
		}
		return;
	}
	next_command_flag = e103_CALL_TWO_DATA;
	return;
}
void Q_protocolIEC103::send_YT_exe(int device)
{
	BYTE m_transceiveBuf[16];

	BYTE CODE = 0x00;
	if (FCB)
	{
		CODE = 0x40 + 0x10 + 0x03;
		FCB = false;
	}
	else
	{
		CODE = 0x40 + 0x10 + 0x20 + 0x03;
		FCB = true;
	}
	m_transceiveBuf[0] = 0x68;
	m_transceiveBuf[1] = 0x0A;
	m_transceiveBuf[2] = 0x0A;
	m_transceiveBuf[3] = 0x68;
	m_transceiveBuf[4] = CODE;//CODE
	m_transceiveBuf[5] = BYTE(device);
	m_transceiveBuf[6] = 0x0A;//ͨ��д��Ŀ
	m_transceiveBuf[7] = 0x81;//VSQ 1��
	m_transceiveBuf[8] = 0x28;//����ԭ�� ͨ�÷���д��Ŀ
	m_transceiveBuf[9] = BYTE(device);
	m_transceiveBuf[10] = 0xFE;// FUN
	m_transceiveBuf[11] = 0xFA;// INF   (F9)��ȷ�ϵ�д��Ŀ��(FA)����ȷ�ϵ�д��Ŀ
	m_transceiveBuf[12] = 0x00;//RII
	m_transceiveBuf[13] = 0x40;//NGD

	BYTE byte = CalculateCheck(&m_transceiveBuf[4],10);
	m_transceiveBuf[14] = byte;
	m_transceiveBuf[15] = 0x16;

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		bool ret=pSendTool->writeData((char*)m_transceiveBuf,16);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
		}
		return;
	}
	next_command_flag = e103_CALL_TWO_DATA;
	return;
}
void Q_protocolIEC103::send_YK_exe(int device, int dotno, bool value)
{
	BYTE m_transceiveBuf[16];
	BYTE DCC = 0x00;
	if (value)
	{
		DCC = 0x02;
	}
	else
		DCC = 0x01;

	BYTE CODE = 0x00;
	if (FCB)
	{
		CODE = 0x40 + 0x10 + 0x03;
		FCB = false;
	}
	else
	{
		CODE = 0x40 + 0x10 + 0x20 + 0x03;
		FCB = true;
	}
	m_transceiveBuf[0] = 0x68;
	m_transceiveBuf[1] = 0x0A;
	m_transceiveBuf[2] = 0x0A;
	m_transceiveBuf[3] = 0x68;
	m_transceiveBuf[4] = CODE;//CODE
	m_transceiveBuf[5] = BYTE(device);
	m_transceiveBuf[6] = 0x40;//
	m_transceiveBuf[7] = 0x01;//VSQ 1��
	m_transceiveBuf[8] = 0x0C;//����ԭ�� 
	m_transceiveBuf[9] = BYTE(device);
	m_transceiveBuf[10] = 0x01;// FUN
	m_transceiveBuf[11] = BYTE(dotno);// INF   
	m_transceiveBuf[12] = DCC;//DCC//����
	m_transceiveBuf[13] = 0x00;//RII

	BYTE byte = CalculateCheck(&m_transceiveBuf[4],10);
	m_transceiveBuf[14] = byte;
	m_transceiveBuf[15] = 0x16;

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		bool ret=pSendTool->writeData((char*)m_transceiveBuf,16);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
		}
		return;
	}
	next_command_flag = e103_CALL_TWO_DATA;
	return;
}


