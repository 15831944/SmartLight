#include "gSystemDef.h"
#include "tinyxmlinclude.h"
#include "Q_ProtocolIEC103Slave.h"
#include "Q_IEC103CommFun.h"
#include "Q_103CP56Time2a.h"

Q_dataManager *m_pMgrIEC103Slave; /*所有IEC104Slave的rtdb全局变量*/
bool Q_protocolIEC103Slave::findBeginByte(e103FrameType &eType,Q_commonBuffer *recvTool)
{
	if (recvTool==NULL)
	{
		return false;
	}
	int nLen = recvTool->getDataLength();
	TRACE("findBeginByte RecvBuff len=%d\n",nLen);
	if (nLen == 0)
	{
		return false;
	}
	//BYTE *buff = new BYTE[nLen];
	
	if(nLen > MAX_RECIVE_PACKET_NUM)
		nLen = MAX_RECIVE_PACKET_NUM;
	BYTE *buff = m_ReciveBuffer;
	memset(buff,0,nLen);
	if (recvTool->readData((char*)buff,nLen,false) < 1)
	{
		//delete []buff;
		//buff = NULL;
		return false;
	}

	for (int i=0;i<nLen-2;i++)
	{
		if (buff[i]==0x0A && buff[i+1]==0x81)
		{//通用分类数据
			if (buff[i+2]==1 || buff[i+2]==2 || buff[i+2]==9 || buff[i+2]==10)
			{//第三个字节为COT传送原因
				//1自发2循环9总查询10总查询终止
				if (i>0)
				{
					onRxMsg(buff,i);
					// 					sprintf_s(LastError,sizeof(LastError),"********未知的报文*******\n");
					// 					OnShowMsg(LastError,sizeof(LastError));
				}
				eType = e103ASDU0AH_Data;
				recvTool->removeData(i);
				
				//delete []buff;
				//buff = NULL;
				return true;
			}
			else if (buff[i+2]==20 || buff[i+2]==21 || buff[i+2]==40 || buff[i+2]==41 || buff[i+2]==42 || buff[i+2]==43 ||buff[i+2]==44)
			{//20一般命令或肯定认可21命令的否认认可
				//40通用分类写命令的命令的肯定认可
				//41通用分类写命令的命令否定认可
				if (i>0)
				{
					onRxMsg(buff,i);
					// 					sprintf_s(LastError,sizeof(LastError),"********未知的报文*******\n");
					// 					OnShowMsg(LastError,sizeof(LastError));
				}
				eType = e103ASDU0AH_Command;
				recvTool->removeData(i);
				//delete []buff;
				//buff = NULL;
				return true;
			}
			else
			{//0A 81 传送原因没有找到

			}
		}
		else if (buff[i]==0x17 /*&& buff[i+1]==0x81*/ && (buff[i+2]==9 || buff[i+2]==31))
		{
			//ASDU23被记录的扰动表
			if (i>0)
			{
				onRxMsg(buff,i);
				// 				sprintf_s(LastError,sizeof(LastError),"********未知的报文*******\n");
				// 				OnShowMsg(LastError,sizeof(LastError));
			}
			eType = e103ASDU17H;
			recvTool->removeData(i);
			//delete []buff;
			//buff = NULL;
			return true;
		}
		else if (buff[i]==0x1A && buff[i+1]==0x81 && buff[i+2]==0x1F)
		{
			//ASDU26扰动数据传输准备就绪
			if (i>0)
			{
				onRxMsg(buff,i);
				// 				sprintf_s(LastError,sizeof(LastError),"********未知的报文*******\n");
				// 				OnShowMsg(LastError,sizeof(LastError));
			}
			eType = e103ASDU1AH;
			recvTool->removeData(i);
			//delete []buff;
			//buff = NULL;
			return true;
		}
		else if (buff[i]==0x1B && buff[i+1]==0x81 && buff[i+2]==0x1F)
		{
			//ASDU27被记录的通道传输准备就绪
			if (i>0)
			{
				onRxMsg(buff,i);
				// 				sprintf_s(LastError,sizeof(LastError),"********未知的报文*******\n");
				// 				OnShowMsg(LastError,sizeof(LastError));
			}
			eType = e103ASDU1BH;
			recvTool->removeData(i);
			//delete []buff;
			//buff = NULL;
			return true;
		}
		else if (buff[i]==0x1C && buff[i+1]==0x81 && buff[i+2]==0x1F)
		{
			//ASDU28带标志的状态变位数据传输准备就绪
			if (i>0)
			{
				onRxMsg(buff,i);
				// 				sprintf_s(LastError,sizeof(LastError),"********未知的报文*******\n");
				// 				OnShowMsg(LastError,sizeof(LastError));
			}
			eType = e103ASDU1CH;
			recvTool->removeData(i);
			//delete []buff;
			//buff = NULL;
			return true;
		}
		else if (buff[i]==0x1D && buff[i+1]==0x81 && buff[i+2]==0x1F)
		{
			//ASDU29传送带标志的状态变位
			if (i>0)
			{
				onRxMsg(buff,i);
				// 				sprintf_s(LastError,sizeof(LastError),"********未知的报文*******\n");
				// 				OnShowMsg(LastError,sizeof(LastError));
			}
			eType = e103ASDU1DH;
			recvTool->removeData(i);
			//delete []buff;
			//buff = NULL;
			return true;
		}
		else if (buff[i]==0x1E && buff[i+1]==0x81 && buff[i+2]==0x1F)
		{
			//ASDU30传输扰动值
			if (i>0)
			{
				onRxMsg(buff,i);
				// 				sprintf_s(LastError,sizeof(LastError),"********未知的报文*******\n");
				// 				OnShowMsg(LastError,sizeof(LastError));
			}
			eType = e103ASDU1EH;
			recvTool->removeData(i);
			//delete []buff;
			//buff = NULL;
			return true;
		}
		else if (buff[i]==0x1F && buff[i+1]==0x81 && buff[i+2]==0x1F)
		{
			//ASDU31传送结束
			if (i>0)
			{
				onRxMsg(buff,i);
				// 				sprintf_s(LastError,sizeof(LastError),"********未知的报文*******\n");
				// 				OnShowMsg(LastError,sizeof(LastError));
			}
			eType = e103ASDU1FH;
			recvTool->removeData(i);
			//delete []buff;
			//buff = NULL;
			return true;
		}
	}
	onRxMsg(buff,nLen);
	// 	sprintf_s(LastError,sizeof(LastError),"********未知的报文*******\n");
	// 	OnShowMsg(LastError,sizeof(LastError));
	recvTool->removeData(nLen);
	eType = e103ASDUNull;
	//delete []buff;
	//buff = NULL;
	return false;
}

bool Q_protocolIEC103Slave::explainData()
{
	bool bRet=true;
	Q_commonBuffer *recvTool=NULL;

	if (m_pProcotolConfig!=NULL)
	{
		recvTool=m_pProcotolConfig->getReceiveBuffer();
		if (recvTool!=NULL)
		{
			e103FrameType e103type = e103ASDUNull;
			while (findBeginByte(e103type,recvTool))
			{//能找到起始字节
				//返复读内容

				int nReadLen = 0;
				bRet=handleAnalyseData(recvTool,nReadLen,e103type);
				if (false==bRet)//数据不足,break
				{
					recvTool->clearBuffer();//清理掉
					break;
				}
				else
				{
					recvTool->removeData(nReadLen);//删除已解析的数据
				}
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


bool Q_protocolIEC103Slave::handleAnalyseData(Q_commonBuffer *recvTool,int &nReadLen,e103FrameType e103type)
{
	bool bRet=true;

	char msgBuf[Q_FRAME_CONST8192];
	//Q_CommUtility::NU_ZERO(msgBuf,sizeof(msgBuf));

	int res=0;
	res=recvTool->readData(msgBuf,sizeof(msgBuf),false);

	TRACE("readData len = %d \n",res);
	if(res<0)
	{
		
			sprintf_s(LastError,sizeof(LastError),"Read data exception,buffer size too small,Please add receive buffer size.close link\n");
			//OnShowMsg(LastError,sizeof(LastError));
			OnShowMsg(LastError,strlen(LastError)+1);
		
		
		recvTool->clearBuffer();//清空内容
		return false;
	}
	//nReadLen，表示已经读了多少个字节，返回false表示长度不够
	if (!decodeImsg((BYTE*)&msgBuf[0],res,nReadLen,e103type))
	{
		bRet = false;
	}
	else
	{
		Q_103NoDataTimes = 0;//收到数据，标识清零
	}
	return bRet;
}

bool Q_protocolIEC103Slave::makeFrame()
{
	char chNextflag[32];
	memset(chNextflag,0,sizeof(chNextflag));
	switch(next_command_flag)
	{
	case e103_CALL_ALL_DATA:
		sprintf_s(chNextflag,sizeof(chNextflag),"e103_CALL_ALL_DATA");
		break;
	case e103_CALL_ONE_DATA:
		sprintf_s(chNextflag,sizeof(chNextflag),"e103_CALL_ONE_DATA");
		break;
	case e103_CALL_TWO_DATA:
		sprintf_s(chNextflag,sizeof(chNextflag),"e103_CALL_TWO_DATA");
		break;
	case e103_INIT_CHANNEL:
		sprintf_s(chNextflag,sizeof(chNextflag),"e103_INIT_CHANNEL");
		break;
	default:
		sprintf_s(chNextflag,sizeof(chNextflag),"未知命令");
		break;
	}
	//TRACE("********next_command_flag = %d,%s ********\n",next_command_flag,chNextflag);

	time(&now);
	if (now - m_nTimerConfirm < sleepflag)
	{
		return true;
	}

	//判断
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
		//RestartCh();
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

	list<Q_IEC103StandardCommand *> v_listCmd;
	int len=m_listCmd->getCommandByStatus(v_listCmd);//查无错误的命令列表
	if (len>0)
	{
		for (list<Q_IEC103StandardCommand *>::iterator it=v_listCmd.begin();it!=v_listCmd.end();it++)
		{
			//查询当前未处理的，解状态值也为未处理的命令
			Q_cmdIEC103 *cur_cmd=(Q_cmdIEC103 *)*it;
			if (cur_cmd->m_eState!=e103Cmd_Null)
			{
				switch(cur_cmd->getCommand103Type())
				{
				case e103CmdTypeYT:
					handleCommmd_YT(cur_cmd);
					break;
				case e103CmdTypeYK:
					handleCommmd_YK(cur_cmd);
					break;
				}
				//如果命令列表里有命令，先处理命令，return
				return true;
			}
		}
	}

	switch (next_command_flag)
	{
	case e103_INIT_CHANNEL:
		//初始化
		// 		sprintf_s(LastError,sizeof(LastError),"1、初始化开始\n");
		// 		OnShowMsg(LastError,sizeof(LastError));
		send_S_Msg();
		Q_103NoDataTimes++;
		this_command_flag = e103_INIT_CHANNEL;
		break;
	case e103_CALL_ALL_DATA:
		//总召唤
		// 		sprintf_s(LastError,sizeof(LastError),"2、总召唤\n");
		// 		OnShowMsg(LastError,sizeof(LastError));
		send_I_Msg();
		time(&now);
		m_nTimerConnect = now;
		this_command_flag = e103_CALL_ALL_DATA;
		break;
	case e103_CALL_ONE_DATA:
		//一级数据
		// 		sprintf_s(LastError,sizeof(LastError),"3、召唤一级数据\n");
		// 		OnShowMsg(LastError,sizeof(LastError));
		send_U_Msg(10);
		Q_103TWODataTimes = 0;
		this_command_flag = e103_CALL_ONE_DATA;
		break;


		/*case e103CmdTypeYT:
		send_YT_preset(yt_device, yt_dotno, yt_value);
		this_command_flag = e103CmdTypeYT;
		break;
		case e103CmdTypeC_RC:
		send_YK_preset(yk_device, yk_dotno, yk_value);
		this_command_flag = e103CmdTypeC_RC;
		break;*/

		// 	case e103CmdTypePlanCurve:
		// 		send_YT_exe(yt_device);
		// 		this_command_flag = e103CmdTypePlanCurve;
		// 		break;
		// 	case e103CmdTypeC_EI:
		// 		send_YK_exe(yk_device, yk_dotno, yk_value);
		// 		this_command_flag = e103CmdTypeC_EI;
		// 		break;
	default:
		//二级数据
		// 		sprintf_s(LastError,sizeof(LastError),"4、召唤2级数据\n");
		// 		OnShowMsg(LastError,sizeof(LastError));
		send_U_Msg(11);
		Q_103TWODataTimes++;
		Q_103NoDataTimes++;
		this_command_flag = e103_CALL_TWO_DATA;
		break;
	}
	//sleep(10);//2016-12-14 yy 将原来的 50 改为 10  2016-12-21 该sleep方法造成断开客户端连接时前置程序崩溃。而此处使用sleep也没有道理所以注释 yy
	return true;
}

int Q_protocolIEC103Slave::send_I_Msg()
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

	//
	for (vector<int>::iterator iter=m_cfgBase->m_vecStationAddr.begin();iter!=m_cfgBase->m_vecStationAddr.end();iter++)
	{
		m_transceiveBuf[6] = 0x07;//TYPE 总召唤
		m_transceiveBuf[7] = 0x81;//VSQ 1个
		m_transceiveBuf[8] = 0x09;//传送原因 总召唤
		m_transceiveBuf[9] = *iter;//BYTE(addr);
		//m_transceiveBuf[10] = 0xFF;// FUN
       //2017-02-09 tjs
		//看南自的应该是FE
		m_transceiveBuf[10] = 0xFE;
		m_transceiveBuf[11] = 0x00;// INF
		//m_transceiveBuf[12] = 0x01;
		  //2017-02-09 tjs
		//看南自这么改的
		m_transceiveBuf[12] = 0x14;

		Q_commonBuffer *pSendTool=NULL;
		if (m_pProcotolConfig!=NULL)
			pSendTool=m_pProcotolConfig->getSendBuffer();

		if (pSendTool!=NULL)
		{
			bool ret=pSendTool->writeData((char*)m_transceiveBuf+6,7);
			if(false==ret)
			{
				TRACE("-----send data failed -----\n");
				return ret;
			}
			onTxMsg(&m_transceiveBuf[6],7);
			next_command_flag = e103_CALL_TWO_DATA;

		}
	}
	next_command_flag = e103_CALL_TWO_DATA;
	return 0;
}

int Q_protocolIEC103Slave::send_U_Msg(int cmd)
{
	next_command_flag = e103_CALL_TWO_DATA;
	return 0;
}

int Q_protocolIEC103Slave::send_S_Msg(void)
{
	next_command_flag = e103_CALL_ALL_DATA;
	return 0;
}


bool Q_protocolIEC103Slave::decodeImsg(BYTE *msgbuf,int nLen,int &readlen,e103FrameType e103type)
{
	Q_IEC103_ASDU_header *header = NULL;
	header = (Q_IEC103_ASDU_header *)msgbuf;
	bool bRet = true;//返回false代表数据不足

	switch(e103type)
	{
	case e103ASDU0AH_Data:
		{
			//总查询回复的
			bRet = Rec_TCP_Data(msgbuf,nLen,readlen);
			break;
		}	
	case e103ASDU0AH_Command:
		{
			// 			sprintf_s(LastError,sizeof(LastError),"收到 %02x %02x %02x %02x %02x %02x  帧 \n",header->type,header->qual,header->tx_cause,header->commom_asdu_1,*(msgbuf+4),*(msgbuf+5));
			// 			OnShowMsg(LastError,sizeof(LastError));
			if (header->tx_cause == 40 || header->tx_cause == 41 || header->tx_cause == 44)
			{
				BYTE *p = msgbuf+8;
				if (*p == 0x0D)
				{//遥调
					// 					sprintf_s(LastError,sizeof(LastError),"收到%02x %02x %02x帧，遥调\n",header->type,header->qual,header->tx_cause);
					// 					OnShowMsg(LastError,sizeof(LastError));
					bRet = explainYT(msgbuf,nLen,readlen);
					break;
				}
				else if (*p == 0x0B)
				{//遥控
					// 					sprintf_s(LastError,sizeof(LastError),"收到%02x %02x %02x帧，遥控\n",header->type,header->qual,header->tx_cause);
					// 					OnShowMsg(LastError,sizeof(LastError));
					bRet = explainYK(msgbuf,nLen,readlen);
					break;
				}
			}
			else
			{
				// 				sprintf_s(LastError,sizeof(LastError),"收到%02x %02x %02x帧，但找不到解析部分，向下找3个字节\n",header->type,header->qual,header->tx_cause);
				// 				OnShowMsg(LastError,sizeof(LastError));
				readlen = 3;
				onRxMsg(msgbuf,3);
				break;
			}
		}
	default:
		{
			sprintf_s(LastError,sizeof(LastError),"收到%02x %02x %02x帧，但找不到解析部分，向下找3个字节\n",header->type,header->qual,header->tx_cause);
			//OnShowMsg(LastError,sizeof(LastError));
			OnShowMsg(LastError,strlen(LastError)+1);
			readlen = 3;
			onRxMsg(msgbuf,3);
			break;
		}
	}
	return bRet;
}

// bool Q_protocolIEC103Slave::Rec_TCP_Data(BYTE *msgbuf,int nLen,int &readlen)
// {
// 	int group_no = 0;
// 	int seq_no = 0;
// 	int data_type = 0;
// 	int data_num = 0;
// 	int data_width = 0;
// 	int tempbval = 0;
// 	int device = 0;
// 	int adr = 0;
// 
// 	group_no = *(msgbuf + 8/*15*/);
// 	seq_no = *(msgbuf + 9/*16*/);
// 	data_type = *(msgbuf + 11/*18*/);
// 	data_num = *(msgbuf + 7/*14*/);//数据数量
// 	data_width = *(msgbuf + 12) + 6;//组号、条目号、KOD、GDD3字节
// 	device = *(msgbuf + 3);
// 
// 	if (data_num*data_width + 8 > nLen)//判断数据是否够
// 	{
// 		return false;
// 	}
// 	else
// 	{
// 		readlen = data_num * data_width + 8;
// 		onRxMsg(msgbuf,readlen);
// 	}
// 
// 
// 	if (device!=m_cfgBase->m_nStationAddr1 && device!=m_cfgBase->m_nStationAddr2)
// 	{
// 		//sprintf_s(LastError,sizeof(LastError),"收到一般数据，站地址=%d，配置中地址为%d、%d，不解析。\n",device,m_cfgBase->m_nStationAddr1,m_cfgBase->m_nStationAddr2);
// 		//OnShowMsg(LastError,sizeof(LastError));
// 		return true;
// 	}
// 
// 	for (int i = 0; i< data_num/* - 1*/; i++)
// 	{
// 		seq_no = *(msgbuf + 9/*16*/ + i * data_width);
// 		if (data_type == 7)//遥测
// 		{
// 			adr =/* group_no * 256 +*/ seq_no;
// 			// Q_MF103Seq *pf=(Q_MF103Seq *)(msgbuf + 21 + i * data_width);
// 			float *val = (float *)(msgbuf + 14 + i*data_width)  ;
// 			Q_RECORDDATATAG dataArray;
// 			dataArray.eDataType=eDataTypeFloat;
// 			if (m_tags!=NULL)
// 			{
// 				if (m_tags->fillTagInformation(&dataArray,adr,device)==true)
// 				{
// 					Q_dataManager* p =m_pMgrIEC103Slave;
// 					if (dataArray.nIndex >=0  && dataArray.nIndex < p->getAnalogCount())
// 					{
// 						float factor=p->getAnalogData()[dataArray.nIndex].factor;
// 
// 						dataArray.fValue= (*val)*factor;	
// 						p->recordDataValue(&dataArray,1);
// 
// 					}	
// 				}
// 				else
// 				{
// 					//sprintf_s(LastError,sizeof(LastError),"收到遥测数据，公共地址=%d，点号=%d，但在遥测表中无法找到该点。",device,adr);
// 					//OnShowMsg(LastError,sizeof(LastError));
// 				}
// 			}
// 		}
// 		else if (data_type == 9)//遥信
// 		{
// 			// 			sprintf_s(LastError,sizeof(LastError),"收到遥信数据，公共地址=%d。\n",device);
// 			// 			OnShowMsg(LastError,sizeof(LastError));
// 			adr = /*group_no * 256 +*/ seq_no;
// 			tempbval = *(msgbuf + 14/*21*/ + i*data_width);
// 			Q_RECORDDATATAG dataArray;
// 			dataArray.eDataType=eDataTypeBit;
// 			if (m_tags!=NULL)
// 			{
// 				if (m_tags->fillTagInformation(&dataArray,adr,device)==true)
// 				{
// 					Q_dataManager* p =m_pMgrIEC103Slave;
// 					if (dataArray.nIndex >=0 && dataArray.nIndex < p->getDigitalCount())
// 					{
// 						dataArray.bValue = (tempbval & 0x0001) == 0?true:false;
// 						p->recordDataValue(&dataArray,1);
// 					}
// 				}
// 				else
// 				{
// 					//sprintf_s(LastError,sizeof(LastError),"收到遥信数据，公共地址=%d，点号=%d，但在遥测表中无法找到该点。",device,adr);
// 					//OnShowMsg(LastError,sizeof(LastError));
// 				}
// 			}
// 		}
// 		else
// 		{
// 			sprintf_s(LastError,sizeof(LastError),"收到其他数据，数据类型=%d，宽度=%d，公共地址=%d。\n",data_type,data_width-6,device);
// 			OnShowMsg(LastError,sizeof(LastError));
// 		}
// 	}
// 	return true;
// }

bool Q_protocolIEC103Slave::Rec_TCP_Data(BYTE *msgbuf,int nLen,int &readlen)
{
	int group_no = 0;
	int seq_no = 0;
	int data_type = 0;
	int data_num = 0;
	int data_width = 0;
	int tempbval = 0;
	int device = 0;
	int adr = 0;
	BYTE tempv = 0;
	BYTE mvalhi = 0;
	BYTE mvallo = 0;
	int tempnvalue = 0;
	int templen = 0;

	//	group_no = *(msgbuf + 8/*15*/);
	//	seq_no = *(msgbuf + 9/*16*/);
	//	data_type = *(msgbuf + 11/*18*/);
	data_num = *(msgbuf + 7/*14*/);//数据数量
	//	data_width = *(msgbuf + 12) + 6;//组号、条目号、KOD、GDD3字节
	device = *(msgbuf + 3);

	// 	if (data_num*data_width + 8 > nLen)//判断数据是否够
	// 	{
	// 		return false;
	// 	}
	// 	else
	// 	{
	// 		readlen = data_num * data_width + 8;
	// 		onRxMsg(msgbuf,readlen);
	// 	}

	templen = 8;//解析位置
	for (int i = 0; i< data_num; i++)
	{
		data_width = *(msgbuf + templen + 4);
		if (templen <= nLen - 6)
		{
			templen = templen + data_width + 6;
		}
		else
		{
			sprintf_s(LastError,sizeof(LastError),"解析数据不够，缓冲区字节=%d，报文头数据个数=%d，当前解析字节=%d，此数据长度=%d\n",nLen,data_num,templen,data_width);
			//OnShowMsg(LastError,sizeof(LastError));
			OnShowMsg(LastError,strlen(LastError)+1);
			onRxMsg(msgbuf,templen);
			return false;
		}
	}
	onRxMsg(msgbuf,templen);
	
	/*if(device==3)
	{
	TRACE("通道过滤总个数为%d\r\n",m_cfgBase->m_vecStationAddr.size());
	for (vector<int>::iterator iter = m_cfgBase->m_vecStationAddr.begin();iter!=m_cfgBase->m_vecStationAddr.end();iter++)
	{
	TRACE("通道过滤设备地址:%d\r\n",*iter);
	}
	}*/

	bool bStationExist = false;
	for (vector<int>::iterator iter = m_cfgBase->m_vecStationAddr.begin();iter!=m_cfgBase->m_vecStationAddr.end();iter++)
	{
		if (device==*iter)
		{
			bStationExist = true;
			break;
		}
		
	}
	
	if (!bStationExist/*device!=m_cfgBase->m_nStationAddr1 && device!=m_cfgBase->m_nStationAddr2*/)
	{
		//sprintf_s(LastError,sizeof(LastError),"收到一般数据，站地址=%d，配置中地址为%d、%d，不解析。\n",device,m_cfgBase->m_nStationAddr1,m_cfgBase->m_nStationAddr2);
		//OnShowMsg(LastError,sizeof(LastError));
// 		readlen = templen;
// 		return true;
		return false;
	}

	templen = 8;//解析位置
	for (int i = 0; i< data_num/* - 1*/; i++)
	{
		group_no = *(msgbuf + templen);
		seq_no = *(msgbuf + templen + 1);
		data_type = *(msgbuf + templen + 3);
		data_width = *(msgbuf + templen + 4);

		if (data_type == 7)//遥测
		{
			adr = /*group_no *256 +*/ seq_no;
//			Q_MF103Seq *pf=(Q_MF103Seq *)(msgbuf + 21 + i * data_width);
			//Q_MF103Seq *pf=(Q_MF103Seq *)(msgbuf + templen + i * data_width);
			//float *val = (float *)(msgbuf + templen + 6);

			short val=0;

			//默认低字节在前
			if(m_cfgBase->m_eIntSeq==e103FloatSeqLittle)
			{
				val = msgbuf[templen + 4 +1]<<8 | msgbuf[templen + 4 +2];
				//val=(pf->val[1]<<8) | pf->val[0];
			}
			else
			{
				val = msgbuf[templen + 4 +2]<<8 | msgbuf[templen + 4 +1];
				//val=(pf->val[0]<<8) | pf->val[1];
			}


			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeFloat;

			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=device;
			dataArray.no=adr;
			dataArray.fValue=val;
			

			if(m_pMgrIEC103Slave == NULL)
				m_pMgrIEC103Slave =m_pMgrIEC103Slave;

			if (m_pMgrIEC103Slave != NULL)
			{
				if(!m_pMgrIEC103Slave->recordDataValue(&dataArray,1))
					TRACE("修改遥测数据失败。 \n");
				else
					TRACE("修改遥测数据成功。 \n");
				
			}
			
			/*
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeFloat;
			if (m_tags!=NULL)
			{
				if (m_tags->fillTagInformation(&dataArray,adr,device)==true)
				{
					Q_dataManager* p =m_pMgrIEC103Slave;
					if (dataArray.nIndex >=0  && dataArray.nIndex < p->getAnalogCount())
					{
						float factor=p->getAnalogData()[dataArray.nIndex].factor;

						dataArray.fValue= (*val)*factor;	
						p->recordDataValue(&dataArray,1);

					}	
				}
				else
				{
					//sprintf_s(LastError,sizeof(LastError),"收到遥测数据，公共地址=%d，点号=%d，但在遥测表中无法找到该点。",device,adr);
					//OnShowMsg(LastError,sizeof(LastError));
				}
			}*/
		}
		else if (data_type == 9)//遥信
		{
			adr = /*group_no *256 +*/ seq_no;
			tempbval = *(msgbuf + templen + 6);

			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeBit;
			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=device;
			dataArray.no=adr;

			if (tempbval == 2)
			{
				dataArray.bValue = true;
			} 
			else
			{
				dataArray.bValue = false;
			}

			if(m_pMgrIEC103Slave == NULL)
				m_pMgrIEC103Slave =m_pMgrIEC103Slave;
			if (m_pMgrIEC103Slave != NULL)
			{
				//m_pMgrIEC103Slave->recordDataValue(&dataArray,1);

				if(!m_pMgrIEC103Slave->recordDataValue(&dataArray,1))
					TRACE("修改遥信数据失败。 \n");
				else
					TRACE("修改遥信数据成功。 \n");

			}
			else
			{
				sprintf_s(LastError,sizeof(LastError),"收到遥信数据，公共地址=%d，点号=%d，但在遥测表中无法找到该点。",device,adr);
				//OnShowMsg(LastError,sizeof(LastError));
			}
			/*if(!m_pMgr->recordDataValue(&dataArray,1))
			{
			}*/
			/*
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeBit;
			if (m_tags!=NULL)
			{
				if (m_tags->fillTagInformation(&dataArray,adr,device)==true)
				{
					Q_dataManager* p =m_pMgrIEC103Slave;
					if (dataArray.nIndex >=0 && dataArray.nIndex < p->getDigitalCount())
					{
						//dataArray.bValue = (tempbval & 0x0001) == 0?true:false;
						if (tempbval == 2)
						{
							dataArray.bValue = true;
						} 
						else
						{
							dataArray.bValue = false;
						}
						p->recordDataValue(&dataArray,1);
					}
				}
				else
				{
					//sprintf_s(LastError,sizeof(LastError),"收到遥信数据，公共地址=%d，点号=%d，但在遥测表中无法找到该点。",device,adr);
					//OnShowMsg(LastError,sizeof(LastError));
				}
			}
			*/
		}
		else if (data_type == 12)
		{
			if (i==data_num-1)
			{
				int aaa=1;
			}
			tempv = *(msgbuf + templen + 7);
			tempv = (tempv>>5)&0x07;//00000111
			if (tempv > 0)
			{
				if (templen <= nLen - 6)
				{
					templen = templen + data_width + 6;
				}
				continue;
			}
			adr = /*group_no *256 +*/ seq_no;
			mvalhi = *(msgbuf + templen + 7);
			mvallo = *(msgbuf + templen + 6);
			tempnvalue = (mvalhi&0x0F)*255 + (mvallo&0xFF);
			if (((mvalhi>>4)&0x01) == 1)
			{
				tempnvalue = tempnvalue * (-1);
			}
			//tjs 2017-01-08 begin
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType=eDataTypeFloat;
			/*
			if (m_tags!=NULL)
			{
				if (m_tags->fillTagInformation(&dataArray,adr,device)==true)
				{
					Q_dataManager* p =m_pMgrIEC103Slave;
					if (dataArray.nIndex >=0  && dataArray.nIndex < p->getAnalogCount())
					{
						float factor=p->getAnalogData()[dataArray.nIndex].factor;

						dataArray.fValue= tempnvalue*factor;	
						p->recordDataValue(&dataArray,1);
					}	
				}
				else
				{
					//sprintf_s(LastError,sizeof(LastError),"收到遥测数据，公共地址=%d，点号=%d，但在遥测表中无法找到该点。",device,adr);
					//OnShowMsg(LastError,sizeof(LastError));
				}
			}*/

			dataArray.channelID=m_ProSetting->getChannelID();
			dataArray.device=device;
			dataArray.no=adr;
			dataArray.fValue=tempnvalue;
			m_pMgrIEC103Slave->recordDataValue(&dataArray,1);

			//end 
		}
		else
		{
			sprintf_s(LastError,sizeof(LastError),"收到其他数据，数据类型=%d，宽度=%d，公共地址=%d。\n",data_type,data_width-6,device);
			//OnShowMsg(LastError,sizeof(LastError));
			OnShowMsg(LastError,strlen(LastError)+1);
		}
		if (templen <= nLen - 6)
		{
			templen = templen + data_width + 6;
		}
		else
			break;

	}
	readlen = templen;
	//onRxMsg(msgbuf,readlen);

	return true;
}

void Q_protocolIEC103Slave::send_YT_preset(int device, int dotno, float value)
{
	memset(LastError,0,sizeof(LastError));
	sprintf_s(LastError,sizeof(LastError),"Q_protocolIEC103Slave::send_YT_preset\n");
	OnShowMsg(LastError,strlen(LastError)+1);

	BYTE m_transceiveBuf[26];
	short nValue = (short)value;

	BYTE *buf=0;
	buf = (BYTE *)(&nValue);

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

	m_transceiveBuf[6] = 0x0A;//通用写条目/////////////开始
	m_transceiveBuf[7] = 0x81;//VSQ 1个
	m_transceiveBuf[8] = 0x28;//传送原因 通用分类写条目
	m_transceiveBuf[9] = BYTE(device);
	m_transceiveBuf[10] = 0xFE;// FUN
	m_transceiveBuf[11] = 0xF9;// INF   (F9)带确认的写条目，(FA)不带确认的写条目（带执行的写条目）
	m_transceiveBuf[12] = 0x01;//RII返回信息标识符（0-255）
	m_transceiveBuf[13] = 0x01;//NGD通用分类集数目
	m_transceiveBuf[14] = 0x0D/*(BYTE)(dotno/256)*/;//组号固定13
	m_transceiveBuf[15] = (BYTE)(dotno&0xFF);//条目号
	m_transceiveBuf[16] = 0x01;//描述类别KOD，实际值
	//通用分类数据描述GDD，CP24{数据类型8，数据宽度8，数目7，后续状态1}
	//m_transceiveBuf[17] = 0x07;//3--整形；7--短浮点型；8--长浮点型，6浮点数IEE754
	m_transceiveBuf[17] = 0x03;
	m_transceiveBuf[18] = 0x02;//数据宽度	
	m_transceiveBuf[19] = 0x01;//数目1，后续状态0
	//通用分类标识数据GID,CP8*数据宽度*数目
	m_transceiveBuf[20] = *(buf);
	m_transceiveBuf[21] = *(buf + 1);
	//	m_transceiveBuf[22] = *(buf + 2);
	//	m_transceiveBuf[23] = *(buf + 3);
	// 	BYTE byte = CalculateCheck(&m_transceiveBuf[4],20);
	// 	m_transceiveBuf[24] = byte;
	// 	m_transceiveBuf[25] = 0x16;

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		bool ret=pSendTool->writeData((char*)(m_transceiveBuf+6),16);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
		}
		else
		{
			onTxMsg(m_transceiveBuf+6,16);
		}
		return;
	}
	next_command_flag = e103_CALL_TWO_DATA;
	return;
}

void Q_protocolIEC103Slave::send_YT_exe(int device, int dotno, float value)
{

	memset(LastError,0,sizeof(LastError));
	sprintf_s(LastError,sizeof(LastError),"Q_protocolIEC103Slave::send_YT_exe\n");
	OnShowMsg(LastError,strlen(LastError)+1);

	BYTE m_transceiveBuf[26];
	short nValue = (short)value;
	BYTE *buf=0;
	buf = (BYTE *)(&nValue);

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
	//
	m_transceiveBuf[6] = 0x0A;//通用写条目
	m_transceiveBuf[7] = 0x81;//VSQ 1个
	m_transceiveBuf[8] = 0x28;//传送原因 通用分类写条目
	m_transceiveBuf[9] = BYTE(device);
	m_transceiveBuf[10] = 0xFE;// FUN
	m_transceiveBuf[11] = 0xFA;// INF   (F9)带确认的写条目，(FA)不带确认的写条目
	m_transceiveBuf[12] = 0x01;//RII
	//m_transceiveBuf[13] = 0x40;//NGD

	m_transceiveBuf[13] = 0x01;//NGD通用分类集数目
	m_transceiveBuf[14] = 0x0D/*(BYTE)(dotno/256)*/;//组号固定13
	m_transceiveBuf[15] = (BYTE)(dotno&0xFF);//条目号
	m_transceiveBuf[16] = 0x01;//描述类别KOD，实际值
	//通用分类数据描述GDD，CP24{数据类型8，数据宽度8，数目7，后续状态1}
	//m_transceiveBuf[17] = 0x07;//3--整形；7--短浮点型；8--长浮点型，6浮点数IEE754
	m_transceiveBuf[17] = 0x03;
	m_transceiveBuf[18] = 0x02;//数据宽度
	m_transceiveBuf[19] = 0x01;//数目1，后续状态0
	//通用分类标识数据GID,CP8*数据宽度*数目
	m_transceiveBuf[20] = *(buf);
	m_transceiveBuf[21] = *(buf + 1);
	// 	m_transceiveBuf[22] = *(buf + 2);
	// 	m_transceiveBuf[23] = *(buf + 3);
// 	BYTE byte = CalculateCheck(&m_transceiveBuf[4],20);
// 	m_transceiveBuf[24] = byte;
// 	m_transceiveBuf[25] = 0x16;

	// 	BYTE byte = CalculateCheck(&m_transceiveBuf[4],10);
	// 	m_transceiveBuf[14] = byte;
	// 	m_transceiveBuf[15] = 0x16;

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		bool ret=pSendTool->writeData((char*)m_transceiveBuf+6,16);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
		}
		else
		{
			onTxMsg(m_transceiveBuf+6,16);
		}
		return;
	}
	next_command_flag = e103_CALL_TWO_DATA;
	return;
}

void Q_protocolIEC103Slave::send_YK_preset_DP(int device, int dotno, BYTE value)
{
	BYTE m_transceiveBuf[26];
	BYTE nValue = 1;
	if (value==0)
	{
		nValue = 1;
	}
	else
	{
		nValue = 2;
	}
	//BYTE *buf = &value;

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

	m_transceiveBuf[6] = 0x0A;//通用写条目/////////////开始
	m_transceiveBuf[7] = 0x81;//
	m_transceiveBuf[8] = 0x28;//传送原因 通用分类写条目
	m_transceiveBuf[9] = BYTE(device);
	m_transceiveBuf[10] = 0xFE;// FUN
	m_transceiveBuf[11] = 0xF9;// INF   (F9)带确认的写条目，(FA)不带确认的写条目（带执行的写条目）
	m_transceiveBuf[12] = 0x02;//RII返回信息标识符（0-255）
	m_transceiveBuf[13] = 0x01;//NGD通用分类集数目
	m_transceiveBuf[14] = 0x0B/*(BYTE)(dotno/256)*/;//遥控组号固定0b
	m_transceiveBuf[15] = (BYTE)(dotno&0xFF);//条目号
	m_transceiveBuf[16] = 0x01;//描述类别KOD，实际值
	//通用分类数据描述GDD，CP24{数据类型8，数据宽度8，数目7，后续状态1}
	//m_transceiveBuf[17] = 0x07;//3--整形；7--短浮点型；8--长浮点型，6浮点数IEE754
	m_transceiveBuf[17] = 10;//单点信息
	m_transceiveBuf[18] = 0x01;//数据宽度	
	m_transceiveBuf[19] = 0x01;//数目1，后续状态0
	//通用分类标识数据GID,CP8*数据宽度*数目
	m_transceiveBuf[20] = nValue;
	//m_transceiveBuf[21] = *(buf + 1);
	//	m_transceiveBuf[22] = *(buf + 2);
	//	m_transceiveBuf[23] = *(buf + 3);
	// 	BYTE byte = CalculateCheck(&m_transceiveBuf[4],20);
	// 	m_transceiveBuf[24] = byte;
	// 	m_transceiveBuf[25] = 0x16;

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		bool ret=pSendTool->writeData((char*)m_transceiveBuf+6,15);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
		}
		else
		{
			onTxMsg(&m_transceiveBuf[6],15);
		}
		return;
	}
	next_command_flag = e103_CALL_TWO_DATA;
	return;
}

void Q_protocolIEC103Slave::send_YK_preset(int device, int dotno, BYTE value)
{
	BYTE m_transceiveBuf[26];

	BYTE *buf = &value;

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

	m_transceiveBuf[6] = 0x0A;//通用写条目/////////////开始 01
	m_transceiveBuf[7] = 0x81;//
	m_transceiveBuf[8] = 0x28;//传送原因 通用分类写条目
	m_transceiveBuf[9] = BYTE(device);
	m_transceiveBuf[10] = 0xFE;// FUN
	m_transceiveBuf[11] = 0xF9;// INF   (F9)带确认的写条目，(FA)不带确认的写条目（带执行的写条目）
	m_transceiveBuf[12] = 0x02;//RII返回信息标识符（0-255）
	m_transceiveBuf[13] = 0x01;//NGD通用分类集数目
	m_transceiveBuf[14] = 0x0B/*(BYTE)(dotno/256)*/;//遥控组号固定0b
	m_transceiveBuf[15] = (BYTE)(dotno&0xFF);//条目号
	m_transceiveBuf[16] = 0x01;//描述类别KOD，实际值
	//通用分类数据描述GDD，CP24{数据类型8，数据宽度8，数目7，后续状态1}
	//m_transceiveBuf[17] = 0x07;//3--整形；7--短浮点型；8--长浮点型，6浮点数IEE754
	m_transceiveBuf[17] = 9;//双点信息
	m_transceiveBuf[18] = 0x01;//数据宽度	
	m_transceiveBuf[19] = 0x01;//数目1，后续状态0
	//通用分类标识数据GID,CP8*数据宽度*数目
	m_transceiveBuf[20] = *(buf);   //15 16
	//m_transceiveBuf[21] = *(buf + 1);
	//	m_transceiveBuf[22] = *(buf + 2);
	//	m_transceiveBuf[23] = *(buf + 3);
	// 	BYTE byte = CalculateCheck(&m_transceiveBuf[4],20);
	// 	m_transceiveBuf[24] = byte;
	// 	m_transceiveBuf[25] = 0x16;

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		bool ret=pSendTool->writeData((char*)m_transceiveBuf+6,15);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
		}
		else
		{
			onTxMsg(&m_transceiveBuf[6],15);
		}
		return;
	}
	next_command_flag = e103_CALL_TWO_DATA;
	return;
}

void Q_protocolIEC103Slave::send_YK_exe_DP(int device, int dotno, BYTE value)
{
	BYTE m_transceiveBuf[26];
	BYTE nValue = 1;
	if (value==0)
	{
		nValue = 1;
	}
	else
	{
		nValue = 2;
	}
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
	//
	m_transceiveBuf[6] = 0x0A;//通用写条目
	m_transceiveBuf[7] = 0x81;//VSQ 1个
	m_transceiveBuf[8] = 0x28;//传送原因 通用分类写条目
	m_transceiveBuf[9] = BYTE(device);
	m_transceiveBuf[10] = 0xFE;// FUN
	m_transceiveBuf[11] = 0xFA;// INF   (F9)带确认的写条目，(FA)不带确认的写条目
	m_transceiveBuf[12] = 0x02;//RII
	//m_transceiveBuf[13] = 0x40;//NGD

	m_transceiveBuf[13] = 0x01;//NGD通用分类集数目
	m_transceiveBuf[14] = 0x0B/*(BYTE)(dotno/256)*/;//遥控是0b
	m_transceiveBuf[15] = (BYTE)(dotno&0xFF);//条目号
	m_transceiveBuf[16] = 0x01;//描述类别KOD，实际值
	//通用分类数据描述GDD，CP24{数据类型8，数据宽度8，数目7，后续状态1}
	//m_transceiveBuf[17] = 0x07;//3--整形；7--短浮点型；8--长浮点型，6浮点数IEE754
	m_transceiveBuf[17] = 9;//双点
	m_transceiveBuf[18] = 0x01;//数据宽度
	m_transceiveBuf[19] = 0x01;//数目1，后续状态0
	//通用分类标识数据GID,CP8*数据宽度*数目
	m_transceiveBuf[20] = nValue;
	//m_transceiveBuf[21] = *(buf + 1);
	// 	m_transceiveBuf[22] = *(buf + 2);
	// 	m_transceiveBuf[23] = *(buf + 3);
// 	BYTE byte = CalculateCheck(&m_transceiveBuf[4],20);
// 	m_transceiveBuf[24] = byte;
// 	m_transceiveBuf[25] = 0x16;

	// 	BYTE byte = CalculateCheck(&m_transceiveBuf[4],10);
	// 	m_transceiveBuf[14] = byte;
	// 	m_transceiveBuf[15] = 0x16;

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		bool ret=pSendTool->writeData((char*)m_transceiveBuf+6,15);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
		}
		else
		{
			onTxMsg(&m_transceiveBuf[6],15);
		}
		return;
	}
	next_command_flag = e103_CALL_TWO_DATA;
	return;
}

void Q_protocolIEC103Slave::send_YK_exe(int device, int dotno, BYTE value)
{
	BYTE m_transceiveBuf[26];

	BYTE *buf= &value;

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
	//
	m_transceiveBuf[6] = 0x0A;//通用写条目
	m_transceiveBuf[7] = 0x81;//VSQ 1个
	m_transceiveBuf[8] = 0x28;//传送原因 通用分类写条目
	m_transceiveBuf[9] = BYTE(device);
	m_transceiveBuf[10] = 0xFE;// FUN
	m_transceiveBuf[11] = 0xFA;// INF   (F9)带确认的写条目，(FA)不带确认的写条目
	m_transceiveBuf[12] = 0x02;//RII
	//m_transceiveBuf[13] = 0x40;//NGD

	m_transceiveBuf[13] = 0x01;//NGD通用分类集数目
	m_transceiveBuf[14] = 0x0B/*(BYTE)(dotno/256)*/;//遥控是0b
	m_transceiveBuf[15] = (BYTE)(dotno&0xFF);//条目号
	m_transceiveBuf[16] = 0x01;//描述类别KOD，实际值
	//通用分类数据描述GDD，CP24{数据类型8，数据宽度8，数目7，后续状态1}
	//m_transceiveBuf[17] = 0x07;//3--整形；7--短浮点型；8--长浮点型，6浮点数IEE754
	m_transceiveBuf[17] = 10;//单点
	m_transceiveBuf[18] = 0x01;//数据宽度
	m_transceiveBuf[19] = 0x01;//数目1，后续状态0
	//通用分类标识数据GID,CP8*数据宽度*数目
	m_transceiveBuf[20] = *(buf);
	//m_transceiveBuf[21] = *(buf + 1);
	// 	m_transceiveBuf[22] = *(buf + 2);
	// 	m_transceiveBuf[23] = *(buf + 3);
	// 	BYTE byte = CalculateCheck(&m_transceiveBuf[4],20);
	// 	m_transceiveBuf[24] = byte;
	// 	m_transceiveBuf[25] = 0x16;

	// 	BYTE byte = CalculateCheck(&m_transceiveBuf[4],10);
	// 	m_transceiveBuf[14] = byte;
	// 	m_transceiveBuf[15] = 0x16;

	Q_commonBuffer *pSendTool=NULL;
	if (m_pProcotolConfig!=NULL)
		pSendTool=m_pProcotolConfig->getSendBuffer();

	if (pSendTool!=NULL)
	{
		bool ret=pSendTool->writeData((char*)m_transceiveBuf+6,15);
		if(false==ret)
		{
			TRACE("-----send data failed -----\n");
		}
		else
		{
			onTxMsg(&m_transceiveBuf[6],15);
		}
		return;
	}
	next_command_flag = e103_CALL_TWO_DATA;
	return;
}

bool Q_protocolIEC103Slave::explainYT(BYTE *msg,int nLen,int &readlen)
{
	if (nLen < 16)
	{//最短16个字节
		readlen = 0;
		return false;
	}
	int YTtoYCID32=0;
	int YTID32=0;
	bool bExist=false;
	Q_IEC103_ASDU_header *header = NULL;
	header = (Q_IEC103_ASDU_header *)msg;
	Q_IEC103_ASDU_COMMON_WRITE *pCommon = (Q_IEC103_ASDU_COMMON_WRITE *) (msg+4);
	if(m_cfgControl->m_YtExecMode==e103YtExecModeSelect)
	{//选择模式
		if (header->tx_cause==44 ||header->tx_cause==40)
		{//通用写确认（预置确认）
			if (pCommon->fun==0xfe && pCommon->inf==0xf9)
			{
				Q_cmdIEC103_SE *cmd=(Q_cmdIEC103_SE*)m_listCmd->getCommandByStatus(e103CmdTypeYT,e103CmdStateYTSelectCon,pCommon->gin_no);
				if(cmd!=NULL)
				{//选择
					
						memset(LastError,NULL,sizeof(LastError));

						if (cmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值选择确认，信息体地址=%d,值=%f ****\n",pCommon->gin_no,cmd->m_fVal);
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定归一化值选择确认，信息体地址=%d，值=%d  ****\n",pCommon->gin_no,cmd->m_nValue);
						}
						//OnShowMsg(LastError,sizeof(LastError));
						OnShowMsg(LastError,strlen(LastError)+1);
					
					

					cmd->m_eState=e103CmdStateYTExec;//准备去发送
					cmd->m_nCurTryNum=0;


					SCADA_SetPoint curSetPoint;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,cmd);

					if (bExist)
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							GetLocalTime(curSetPoint.exectime);

							curSetPoint.iexecret=6;

							CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC103Slave->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);
						}

					}
				}
				else
				{
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"收到ChannelID=%d，device=%d,no=%d的遥调选择返回确认，但在遥调命令列表中不存在！！！\n",m_ProSetting->getChannelID(),header->commom_asdu_1,pCommon->gin_no);
					//OnShowMsg(LastError,sizeof(LastError));
					OnShowMsg(LastError,strlen(LastError)+1);
				}
			}
			else if (pCommon->fun==0xfe && pCommon->inf==0xfa)
			{
				Q_cmdIEC103 *cmd=m_listCmd->getCommandByStatus(e103CmdTypeYT,e103CmdStateYTExecCon,pCommon->gin_no);
				Q_cmdIEC103_SE *pcmd=NULL;
				if(cmd!=NULL)
				{
					
						memset(LastError,NULL,sizeof(LastError));
						pcmd = (Q_cmdIEC103_SE *)cmd;
						if (pcmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值成功，信息体地址=%d，值=%f  ****\n",pCommon->gin_no,pcmd->m_fVal);
							//OnShowMsg(LastError,sizeof(LastError));
							OnShowMsg(LastError,strlen(LastError)+1);
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定归一化值成功，信息体地址=%d，值=%d  ****\n",pCommon->gin_no,pcmd->m_nValue);
							//OnShowMsg(LastError,sizeof(LastError));
							OnShowMsg(LastError,strlen(LastError)+1);
						}	
					

						SCADA_SetPoint curSetPoint;
						bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,pcmd);

						if (bExist)
						{
							if ((YTID32>0) && (&curSetPoint!=NULL))
							{
								GetLocalTime(curSetPoint.exectime);

								curSetPoint.iexecret=1;

								CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC103Slave->getRtdbInf();
								CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
								dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);
							}

						}
					
					m_listCmd->removeCommand(e103CmdTypeYT,e103CmdStateYTExecCon,pCommon->gin_no);
				}
				else
				{
					sprintf_s(LastError,sizeof(LastError),"收到ChannelID=%d，device=%d,no=%d的遥调执行返回确认，但在遥调命令列表中不存在！！！\n",m_ProSetting->getChannelID(),header->commom_asdu_1,pCommon->gin_no);
					OnShowMsg(LastError,strlen(LastError)+1);
				}
			}
		}
		else if (header->tx_cause==41)
		{//执行否定
			if (pCommon->fun==0xfe && pCommon->inf==0xf9)
			{
				Q_cmdIEC103 *cmd=m_listCmd->getCommandByStatus(e103CmdTypeYT,e103CmdStateYTSelectCon,pCommon->gin_no);
				Q_cmdIEC103_SE *pcmd=NULL;
				if(cmd!=NULL)
				{
					
						memset(LastError,NULL,sizeof(LastError));
						pcmd = (Q_cmdIEC103_SE *)cmd;
						if (pcmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"****  选择短浮点值失败，信息体地址=%d，值=%f  ****\n",pCommon->gin_no,pcmd->m_fVal);
							//OnShowMsg(LastError,sizeof(LastError));
							OnShowMsg(LastError,strlen(LastError)+1);
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  选择归一化值失败，信息体地址=%d，值=%d  ****\n",pCommon->gin_no,pcmd->m_nValue);
							//OnShowMsg(LastError,sizeof(LastError));
							OnShowMsg(LastError,strlen(LastError)+1);
						}	
					
						SCADA_SetPoint curSetPoint;
						bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,pcmd );

						if (bExist)
						{
							if ((YTID32>0) && (&curSetPoint!=NULL))
							{
								GetLocalTime(curSetPoint.exectime);

								curSetPoint.iexecret=7;

								CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC103Slave->getRtdbInf();
								CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
								dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);
							}

						}

					m_listCmd->removeCommand(e103CmdTypeYT,e103CmdStateYTSelectCon,pCommon->gin_no);
				}
				else
				{
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"收到ChannelID=%d，device=%d,no=%d的遥调选择*失败*，但在遥调命令列表中不存在！！！\n",m_ProSetting->getChannelID(),header->commom_asdu_1,pCommon->gin_no);
					//OnShowMsg(LastError,sizeof(LastError));
					OnShowMsg(LastError,strlen(LastError)+1);
				}
			}
			else if (pCommon->fun==0xfe && pCommon->inf==0xfa)
			{
				Q_cmdIEC103 *cmd=m_listCmd->getCommandByStatus(e103CmdTypeYT,e103CmdStateYTExecCon,pCommon->gin_no);
				if(cmd!=NULL)
				{
					
						memset(LastError,NULL,sizeof(LastError));
						Q_cmdIEC103_SE *pcmd = (Q_cmdIEC103_SE *)cmd;
						if (pcmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定执行短浮点值失败，信息体地址=%d，值=%f  ****\n",pCommon->gin_no,pcmd->m_fVal);
							//OnShowMsg(LastError,sizeof(LastError));
							OnShowMsg(LastError,strlen(LastError)+1);
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定执行归一化值失败，信息体地址=%d，值=%d  ****\n",pCommon->gin_no,pcmd->m_nValue);
							//OnShowMsg(LastError,sizeof(LastError));
							OnShowMsg(LastError,strlen(LastError)+1);
						}	
					
					Q_dataManager* p =m_pMgrIEC103Slave;
					LPQ_YTEDATATAG pYTData = p->getYTData();
					for (int i=0;i<p->getYTCount();i++)
					{
						if (m_ProSetting->getChannelID()==pYTData[i].channelid && pYTData[i].device==cmd->m_nComAsdu && pYTData[i].no==cmd->m_nInfoAddr)
						{
							pYTData[i].finishtime = time(NULL);
							pYTData[i].execresult = 3;
							break;
						}
					}
					m_listCmd->removeCommand(e103CmdTypeYT,e103CmdStateYTExecCon,pCommon->gin_no);
				}
				else
				{
					sprintf_s(LastError,sizeof(LastError),"收到ChannelID=%d，device=%d,no=%d的遥调执行*失败*，但在遥调命令列表中不存在！！！\n",m_ProSetting->getChannelID(),header->commom_asdu_1,pCommon->gin_no);
					//OnShowMsg(LastError,sizeof(LastError));
					OnShowMsg(LastError,strlen(LastError)+1);
				}
			}
		}
	}
	else
	{//执行模式
		
		if (header->tx_cause==41)
		{//执行成功
			if (pCommon->fun==0xfe && pCommon->inf==0xf9 )
			{
				Q_cmdIEC103 *cmd=m_listCmd->getCommandByStatus(e103CmdTypeYT,e103CmdStateYTSelectCon,pCommon->gin_no);
				Q_cmdIEC103_SE *pcmd=NULL;
				if(cmd!=NULL)
				{
					memset(LastError,NULL,sizeof(LastError));
					pcmd = (Q_cmdIEC103_SE *)cmd;
					if (pcmd->m_cFuncCode==0x32)
					{
						sprintf_s(LastError,sizeof(LastError),"****  直接执行短浮点值成功，信息体地址=%d，值=%f  ****\n",pCommon->gin_no,pcmd->m_fVal);
						//OnShowMsg(LastError,sizeof(LastError));
						OnShowMsg(LastError,strlen(LastError)+1);
					}
					else
					{
						sprintf_s(LastError,sizeof(LastError),"****  直接执行归一化值成功，信息体地址=%d，值=%d  ****\n",pCommon->gin_no,pcmd->m_nValue);
						//OnShowMsg(LastError,sizeof(LastError));
						OnShowMsg(LastError,strlen(LastError)+1);
					}	

					SCADA_SetPoint curSetPoint;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,pcmd );

					if (bExist)
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							GetLocalTime(curSetPoint.exectime);

							curSetPoint.iexecret=1;

							CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC103Slave->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);
						}

					}

				}
				else
				{
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"收到ChannelID=%d，device=%d,no=%d的遥调选择*失败*，但在遥调命令列表中不存在！！！\n",m_ProSetting->getChannelID(),header->commom_asdu_1,pCommon->gin_no);
					//OnShowMsg(LastError,sizeof(LastError));
					OnShowMsg(LastError,strlen(LastError)+1);
					
				}
			}
			m_listCmd->removeCommand(e103CmdTypeYT,e103CmdStateYTSelectCon,pCommon->gin_no);
		}
    }
	readlen = 16;
	onRxMsg(msg,16);
	return true;
}

bool Q_protocolIEC103Slave::explainYK(BYTE *msg,int nLen,int &readlen)
{
	if (nLen < 15)
	{//最短15个字节
		readlen = 0;
		return false;
	}

	Q_IEC103_ASDU_header *header = NULL;
	header = (Q_IEC103_ASDU_header *)msg;
	Q_IEC103_ASDU_COMMON_WRITE *pCommon = (Q_IEC103_ASDU_COMMON_WRITE *) (msg+4);
	if(m_cfgControl->m_YkExecMode==e103YtExecModeSelect)
	{//选择模式
		if (header->tx_cause==44 ||header->tx_cause==40)
		{//通用写确认（预置确认）
			if (pCommon->fun==0xfe && pCommon->inf==0xf9)
			{
				Q_cmdIEC103 *cmd=m_listCmd->getCommandByStatus(e103CmdTypeYK,e103CmdStateYKSelectCon,pCommon->gin_no);
				if(cmd!=NULL)
				{//选择
					
						memset(LastError,NULL,sizeof(LastError));

						Q_cmdIEC103_SP *pcmd = (Q_cmdIEC103_SP *)cmd;
						sprintf_s(LastError,sizeof(LastError),"****  遥控单点值选择确认，信息体地址=%d,值=%d ****\n",pCommon->gin_no,pcmd->m_cOnOff);
						//OnShowMsg(LastError,sizeof(LastError));
						OnShowMsg(LastError,strlen(LastError)+1);
					

					cmd->m_eState=e103CmdStateYKExec;//准备去发送
					cmd->m_nCurTryNum=0;

					Q_dataManager* p =m_pMgrIEC103Slave;
					LPQ_YKEDATATAG pYKData = p->getYKData();
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
				}
				else
				{
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 收到ChannelID=%d，device=%d,no=%d的遥控选择返回确认，但在遥控命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),header->commom_asdu_1,pCommon->gin_no);
					//OnShowMsg(LastError,sizeof(LastError));
					OnShowMsg(LastError,strlen(LastError)+1);
				}
			}
			else if (pCommon->fun==0xfe && pCommon->inf==0xfa)
			{
				Q_cmdIEC103 *cmd=m_listCmd->getCommandByStatus(e103CmdTypeYK,e103CmdStateYKExecCon,pCommon->gin_no);
				if(cmd!=NULL)
				{
					
						memset(LastError,NULL,sizeof(LastError));
						Q_cmdIEC103_SP *pcmd = (Q_cmdIEC103_SP *)cmd;
						sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值成功，信息体地址=%d，值=%d  ****\n",pCommon->gin_no,pcmd->m_cOnOff);
						//OnShowMsg(LastError,sizeof(LastError));
						OnShowMsg(LastError,strlen(LastError)+1);
					
					Q_dataManager* p =m_pMgrIEC103Slave;
					LPQ_YKEDATATAG pYKData = p->getYKData();
					for (int i=0;i<p->getYKCount();i++)
					{
						if (m_ProSetting->getChannelID()==pYKData[i].channelid && pYKData[i].device==cmd->m_nComAsdu && pYKData[i].no==cmd->m_nInfoAddr)
						{
							pYKData[i].finishtime = time(NULL);
							pYKData[i].execresult = 1;
							break;
						}
					}
					m_listCmd->removeCommand(e103CmdTypeYK,e103CmdStateYKExecCon,pCommon->gin_no);
				}
				else
				{
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 收到ChannelID=%d，device=%d,no=%d的遥控执行返回确认，但在遥控命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),header->commom_asdu_1,pCommon->gin_no);
					//OnShowMsg(LastError,sizeof(LastError));
				OnShowMsg(LastError,strlen(LastError)+1);
				}
			}
		}
		else if (header->tx_cause==41)
		{//执行否定
			if (pCommon->fun==0xfe && pCommon->inf==0xf9)
			{
				Q_cmdIEC103 *cmd=m_listCmd->getCommandByStatus(e103CmdTypeYK,e103CmdStateYKSelectCon,pCommon->gin_no);
				if(cmd!=NULL)
				{
					
						memset(LastError,NULL,sizeof(LastError));
						Q_cmdIEC103_SP *pcmd = (Q_cmdIEC103_SP *)cmd;
						sprintf_s(LastError,sizeof(LastError),"****  选择单点遥控值失败，信息体地址=%d，值=%d  ****\n",pCommon->gin_no,pcmd->m_cOnOff);
						//OnShowMsg(LastError,sizeof(LastError));
						OnShowMsg(LastError,strlen(LastError)+1);
					
					Q_dataManager* p =m_pMgrIEC103Slave;
					LPQ_YKEDATATAG pYKData = p->getYKData();
					for (int i=0;i<p->getYKCount();i++)
					{
						if (m_ProSetting->getChannelID()==pYKData[i].channelid && pYKData[i].device==cmd->m_nComAsdu && pYKData[i].no==cmd->m_nInfoAddr)
						{
							pYKData[i].finishtime = time(NULL);
							pYKData[i].execresult = 3;
							break;
						}
					}
					m_listCmd->removeCommand(e103CmdTypeYK,e103CmdStateYKSelectCon,pCommon->gin_no);
				}
				else
				{
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 收到ChannelID=%d，device=%d,no=%d的遥控选择*失败*，但在遥控命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),header->commom_asdu_1,pCommon->gin_no);
					//OnShowMsg(LastError,sizeof(LastError));
					OnShowMsg(LastError,strlen(LastError)+1);
				}
			}
			else if (pCommon->fun==0xfe && pCommon->inf==0xfa)
			{
				Q_cmdIEC103 *cmd=m_listCmd->getCommandByStatus(e103CmdTypeYK,e103CmdStateYKExecCon,pCommon->gin_no);
				if(cmd!=NULL)
				{
					
						memset(LastError,NULL,sizeof(LastError));
						Q_cmdIEC103_SP *pcmd = (Q_cmdIEC103_SP *)cmd;
						sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值失败，信息体地址=%d，值=%d  ****\n",pCommon->gin_no,pcmd->m_cOnOff);
						//OnShowMsg(LastError,sizeof(LastError));	
						OnShowMsg(LastError,strlen(LastError)+1);
					
					Q_dataManager* p =m_pMgrIEC103Slave;
					LPQ_YKEDATATAG pYKData = p->getYKData();
					for (int i=0;i<p->getYKCount();i++)
					{
						if (m_ProSetting->getChannelID()==pYKData[i].channelid && pYKData[i].device==cmd->m_nComAsdu && pYKData[i].no==cmd->m_nInfoAddr)
						{
							pYKData[i].finishtime = time(NULL);
							pYKData[i].execresult = 3;
							break;
						}
					}
					m_listCmd->removeCommand(e103CmdTypeYK,e103CmdStateYKExecCon,pCommon->gin_no);
				}
				else
				{
					SYSTEMTIME st;
					GetLocalTime(st);
					sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 收到ChannelID=%d，device=%d,no=%d的遥控执行*失败*，但在遥控命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
						m_ProSetting->getChannelID(),header->commom_asdu_1,pCommon->gin_no);
					//OnShowMsg(LastError,sizeof(LastError));
					OnShowMsg(LastError,strlen(LastError)+1);
				}
			}
		}
	}
	readlen = 15;
	onRxMsg(msg,15);
	return true;
}


int Q_protocolIEC103Slave::handleCommmd_YT(Q_cmdIEC103 *v_cmd)
{
	TRACE("Q_protocolIEC103Slave::handleCommmd_YT begin\r\n");
	int bRet=Q_103SUC;

	v_cmd->m_bExec=true;
	Q_cmdIEC103_SE *cmd=NULL;
	
	int	YTID32 = -1;
	int YTtoYCID32 = -1;
		bool bExist = false;
	if(m_cfgControl->m_YtExecMode==e103YtExecModeSelect)
	{
		TRACE("Q_protocolIEC103Slave::handleCommmd_YT select mode\r\n");
		switch (v_cmd->m_eState)
		{
		case e103CmdStateYTSelect:
			{
				cmd = (Q_cmdIEC103_SE *)v_cmd;
				if(cmd!=NULL)
				{
					switch(m_cfgControl->m_YsetAsduType)
					{
					case e103YsetAsdu30:
						cmd->m_cFuncCode=0x30;
						break;
					case e103YsetAsdu31:
						cmd->m_cFuncCode=0x31;
						break;
					case e103YsetAsdu32:
						cmd->m_cFuncCode=0x32;
						break;
					default:
						cmd->m_cFuncCode=0x32;
						break;
					}
				
	
					SCADA_SetPoint curSetPoint;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,cmd);

					TRACE("Q_protocolIEC103Slave::handleCommmd_YT yt Exist:%s\r\n",bExist==true?"TRUE":"FALSE");
					if (bExist)
					{
						if ((YTID32>0) && (&curSetPoint!=NULL))
						{
							GetLocalTime(curSetPoint.exectime);

							curSetPoint.iexecret=4;
							
							CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC103Slave->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调执行时间
						}

					}
					else
					{
						SYSTEMTIME st;
						GetLocalTime(st);

						memset(LastError,0,sizeof(LastError));
						sprintf_s(LastError,sizeof(LastError)," 处理ChannelID=%d，device=%d,no=%d的遥调选择时，在遥控调命令列表中不存在！！！\n",m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);
						OnShowMsg(LastError,strlen(LastError)+1);

						m_listCmd->removeCommand(v_cmd);
						return 0;

						
					}

					if (cmd->m_cFuncCode==0x32)
					{//浮点
						send_YT_preset(cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
					}
					else	
					{
						float fValue = cmd->m_cOnOff;
						send_YT_preset(cmd->m_nComAsdu,cmd->m_nInfoAddr,fValue);
					}				
					v_cmd->m_eState=e103CmdStateYTSelectCon;
					
					memset(LastError,NULL,sizeof(LastError));
					if (cmd->m_cFuncCode==0x32)
					{
						sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值，模式：选择，公共地址=%d，信息体地址=%d，值=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
					}
					else
					{
						sprintf_s(LastError,sizeof(LastError),"****  设定归一化值，模式：选择，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
					}
					OnShowMsg(LastError,strlen(LastError)+1);
						
					
				}
			}
			break;
		case e103CmdStateYTExec:
			{

				cmd = (Q_cmdIEC103_SE *)v_cmd;
				if(cmd!=NULL)
				{
				
					SCADA_SetPoint curSetPoint;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,cmd);

					float fResult=0.0f;
					if (bExist)
					{
						if (cmd->m_cFuncCode==0x32)
						{
							fResult= cmd->m_fVal;
						}
						else
						{
							fResult=(float)cmd->m_cOnOff;
						}

						   GetLocalTime(curSetPoint.exectime);
						   curSetPoint.fVal=fResult* curSetPoint.fFactor;
						   curSetPoint.iexecret=5;
							
						   CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC103Slave->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调执行时间

					}
					else
					{
						SYSTEMTIME st;
						GetLocalTime(st);
						sprintf_s(LastError,sizeof(LastError),"处理ChannelID=%d，device=%d,no=%d的遥调选择执行时，在遥调命令列表中不存在！！！\n",m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);
						OnShowMsg(LastError,strlen(LastError)+1);

						m_listCmd->removeCommand(v_cmd);
						return 0;
						
					}

					if (cmd->m_cFuncCode==0x32)
					{
						send_YT_exe(cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
					}
					else
					{
						send_YT_exe(cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
					}
					cmd->m_cQuality=0;
					cmd->m_eState=e103CmdStateYTExecCon;
					cmd->m_nCurTryNum=0;

					
						memset(LastError,NULL,sizeof(LastError));
						if (cmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值，模式：执行，公共地址=%d，信息体地址=%d，值=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定归一化值，模式：执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						OnShowMsg(LastError,strlen(LastError)+1);
						
					
				}

			}
			break;
		case e103CmdStateYTSelectCon:
			v_cmd->m_nCurTryNum++;
			if(v_cmd->m_nCurTryNum>=v_cmd->m_nMaxTryCount)
			{
				v_cmd->m_nCurTryNum=0;

				cmd = (Q_cmdIEC103_SE *)v_cmd;

				memset(LastError,NULL,sizeof(LastError));
				if(cmd!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"遥调超时，信息体地址=%d,值=%d or %f\n",cmd->m_nInfoAddr,cmd->m_cOnOff,cmd->m_fVal);
				}
				else
				{
					sprintf_s(LastError,sizeof(LastError),"遥调超时，删除命令.\n");
				}
				OnShowMsg(LastError,strlen(LastError)+1);


				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,cmd);

				float fResult=0.0f;
				if (bExist)
				{
					if (cmd->m_cFuncCode==0x32)
					{
						fResult= cmd->m_fVal;
					}
					else
					{
						fResult=(float)cmd->m_cOnOff;
					}

					curSetPoint.iexecret=2;
					CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC103Slave->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
					dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调执行时间

				}

				m_listCmd->removeCommand(v_cmd);
			}
			break;
		case e103CmdStateYTExecCon:
			v_cmd->m_nCurTryNum++;
			if(v_cmd->m_nCurTryNum>=v_cmd->m_nMaxTryCount)
			{
				v_cmd->m_nCurTryNum=0;

			
				cmd = (Q_cmdIEC103_SE *)v_cmd;

				memset(LastError,NULL,sizeof(LastError));
				if(cmd!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"遥调超时，信息体地址=%d,值=%d or %f\n",cmd->m_nInfoAddr,cmd->m_cOnOff,cmd->m_fVal);
				}
				else
				{
					sprintf_s(LastError,sizeof(LastError),"遥调超时，删除命令.\n");
				}
				OnShowMsg(LastError,strlen(LastError)+1);
				
				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,cmd);

				if (bExist)
				{
					curSetPoint.iexecret=3;
					CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC103Slave->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

					dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调执行时间

				}

				m_listCmd->removeCommand(v_cmd);
			}
			break;
		}
	}
	else
	{
		TRACE("Q_protocolIEC103Slave::handleCommmd_YT direct exec mode\r\n");
		switch (v_cmd->m_eState)
		{
		case e103CmdStateYTSelect:
			{
				cmd = (Q_cmdIEC103_SE *)v_cmd;
				if(cmd!=NULL)
				{
		
					SCADA_SetPoint curSetPoint;
					bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,cmd);

					float fResult=0.0f;
					if (bExist)
					{
						if (cmd->m_cFuncCode==0x32)
						{
							fResult= cmd->m_fVal;
						}
						else
						{
							fResult=(float)cmd->m_cOnOff;
						}


						   GetLocalTime(curSetPoint.selecttime);
						   GetLocalTime(curSetPoint.exectime);
						   curSetPoint.fVal=fResult* curSetPoint.fFactor;

							CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC103Slave->getRtdbInf();
							CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
							curSetPoint.iexecret = 5;
							dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调执行时间

					}
					else
					{
						SYSTEMTIME st;
						GetLocalTime(st);
						sprintf_s(LastError,sizeof(LastError),"处理ChannelID=%d，device=%d,no=%d的遥调选择执行时，在遥调命令列表中不存在！！！\n",m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);
						OnShowMsg(LastError,strlen(LastError)+1);
						
					}

					if (cmd->m_cFuncCode==0x32)
					{
						send_YT_exe(cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
					}
					else
					{
						send_YT_exe(cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
					}
					cmd->m_cQuality=0;
					cmd->m_eState=e103CmdStateYTExecCon;
					cmd->m_nCurTryNum=0;

					
						memset(LastError,NULL,sizeof(LastError));
						if (cmd->m_cFuncCode==0x32)
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值，模式：执行，公共地址=%d，信息体地址=%d，值=%f  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定归一化值，模式：执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						OnShowMsg(LastError,strlen(LastError)+1);
						
					
				}

			}
			break;
		case e103CmdStateYTExecCon:
			v_cmd->m_nCurTryNum++;
			if(v_cmd->m_nCurTryNum>=v_cmd->m_nMaxTryCount)
			{
				v_cmd->m_nCurTryNum=0;

			
				cmd = (Q_cmdIEC103_SE *)v_cmd;

				memset(LastError,NULL,sizeof(LastError));
				if(cmd!=NULL)
				{
					sprintf_s(LastError,sizeof(LastError),"遥调超时，信息体地址=%d,值=%d or %f\n",cmd->m_nInfoAddr,cmd->m_cOnOff,cmd->m_fVal);
				}
				else
				{
					sprintf_s(LastError,sizeof(LastError),"遥调超时，删除命令.\n");
				}
				OnShowMsg(LastError,strlen(LastError)+1);
				

				SCADA_SetPoint curSetPoint;
				bExist = FindYT(YTtoYCID32,YTID32,curSetPoint,cmd);

				if (bExist)
				{
					curSetPoint.iexecret=3;
					CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC103Slave->getRtdbInf();
					CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

					dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,YTID32,&curSetPoint);//写入遥调执行时间

				}

				m_listCmd->removeCommand(v_cmd);
			}
			break;
		}
    }
	return bRet;
}


bool Q_protocolIEC103Slave::FindYT(int &YTtoYCID32, int &YTID32, SCADA_SetPoint &curSetPoint, Q_cmdIEC103_SE*cmd)
{
	int nPos = 0;
	bool bExist = false;

	CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC103Slave->getRtdbInf();
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

int Q_protocolIEC103Slave::handleCommmd_YK(Q_cmdIEC103 *v_cmd)
{
	int bRet=Q_103SUC;

	v_cmd->m_bExec=true;
	Q_cmdIEC103_SP *cmd=NULL;
	if(m_cfgControl->m_YkExecMode==e103YtExecModeSelect)
	{
		switch (v_cmd->m_eState)
		{
		case e103CmdStateYKSelect:
			{
				cmd = (Q_cmdIEC103_SP *)v_cmd;
				if(cmd!=NULL)
				{
					Q_dataManager* p =m_pMgrIEC103Slave;
					LPQ_YKEDATATAG pYKData = p->getYKData();
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
						pYKData[nPos].controltype = 1;
						pYKData[nPos].bValue = cmd->m_cOnOff!=0 ? true:false; 
						pYKData[nPos].selecttime = time(NULL);
						pYKData[nPos].selectreplytime = 0;
						pYKData[nPos].exectime = 0;
						pYKData[nPos].finishtime = 0;
						pYKData[nPos].execresult = 0;
					}
					else
					{
						SYSTEMTIME st;
						GetLocalTime(st);
						sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 处理ChannelID=%d，device=%d,no=%d的遥控选择时，在遥控命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
							m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);
						OnShowMsg(LastError,strlen(LastError)+1);
						
					}
					if (m_cfgControl!=NULL)
					{
						if (m_cfgControl->m_YkAsduType==e103YkAsdu9)
						{
							send_YK_preset_DP(cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						else
						{
							send_YK_preset(cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);	
						}
					}
					else
					{
						send_YK_preset(cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
					}
					v_cmd->m_eState=e103CmdStateYKSelectCon;
					
						memset(LastError,NULL,sizeof(LastError));
						if (m_cfgControl!=NULL)
						{
							if (m_cfgControl->m_YkAsduType==e103YkAsdu9)
							{
								sprintf_s(LastError,sizeof(LastError),"****  设定双点遥控值，模式：选择，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							}
							else
							{
								sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值，模式：选择，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							}
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值，模式：选择，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						//OnShowMsg(LastError,sizeof(LastError));
						OnShowMsg(LastError,strlen(LastError)+1);
					
				}
			}
			break;
		case e103CmdStateYKExec:
			{
				cmd = (Q_cmdIEC103_SP *)v_cmd;
				if(cmd!=NULL)
				{
					Q_dataManager* p =m_pMgrIEC103Slave;
					LPQ_YKEDATATAG pYKData = p->getYKData();
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
						pYKData[nPos].bValue = cmd->m_cOnOff!=0 ? true:false;
						pYKData[nPos].exectime = time(NULL);
					}
					else
					{
						SYSTEMTIME st;
						GetLocalTime(st);
						sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 处理ChannelID=%d，device=%d,no=%d的遥控选择执行时，在遥控命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
							m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);
						//OnShowMsg(LastError,sizeof(LastError));
						OnShowMsg(LastError,strlen(LastError)+1);
					}
					if (m_cfgControl!=NULL)
					{
						if (m_cfgControl->m_YkAsduType==e103YkAsdu9)
						{
							send_YK_exe_DP(cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						else
						{
							send_YK_exe(cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
					}
					else
					{
						send_YK_exe(cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
					}
					cmd->m_cQuality=0;
					cmd->m_eState=e103CmdStateYKExecCon;
					cmd->m_nCurTryNum=0;

					
						memset(LastError,NULL,sizeof(LastError));
						if (m_cfgControl!=NULL)
						{
							if (m_cfgControl->m_YkAsduType==e103YkAsdu9)
							{
								sprintf_s(LastError,sizeof(LastError),"****  设定双点遥控值，模式：执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							}
							else
							{
								sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值，模式：执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
							}
						}
						else
						{
							sprintf_s(LastError,sizeof(LastError),"****  设定单点遥控值，模式：执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
						}
						//OnShowMsg(LastError,sizeof(LastError));
						OnShowMsg(LastError,strlen(LastError)+1);
					
				}

			}
			break;
		case e103CmdStateYKSelectCon:
		case e103CmdStateYKExecCon:
			v_cmd->m_nCurTryNum++;
			if(v_cmd->m_nCurTryNum>=v_cmd->m_nMaxTryCount)
			{
				v_cmd->m_nCurTryNum=0;

				
					cmd = (Q_cmdIEC103_SP *)v_cmd;

					memset(LastError,NULL,sizeof(LastError));
					if(cmd!=NULL)
					{
						sprintf_s(LastError,sizeof(LastError),"遥控超时，信息体地址=%d,值=%d \n",cmd->m_nInfoAddr,cmd->m_cOnOff);
					}
					else
					{
						sprintf_s(LastError,sizeof(LastError),"遥控超时，删除命令.\n");
					}
					//OnShowMsg(LastError,sizeof(LastError));
					OnShowMsg(LastError,strlen(LastError)+1);
				
				Q_dataManager* p =m_pMgrIEC103Slave;
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
				m_listCmd->removeCommand(v_cmd);
			}
			break;
		}
	}
	// 	////////直接执行模式////////////////////////////////////////
	// 	else if(m_cfgControl->m_YtExecMode==eYtExecModeDirect)
	// 	{
	// 		v_cmd->m_nCurTryNum++;
	// 		if(v_cmd->m_nCurTryNum>=v_cmd->m_nMaxTryCount)
	// 		{
	// 			v_cmd->m_nCurTryNum=0;	
	// 			
	// 				cmd = (Q_cmdIEC103_SE *)v_cmd;
	// 
	// 				memset(LastError,NULL,sizeof(LastError));
	// 				if(cmd!=NULL)
	// 				{
	// 					if(cmd->m_cFuncCode==0x32)
	// 					{
	// 						sprintf_s(LastError,sizeof(LastError),"直接遥调超时，删除命令，地址=%d,值=%f\n",cmd->m_nInfoAddr,cmd->m_fVal);
	// 					}
	// 					else
	// 					{
	// 						sprintf_s(LastError,sizeof(LastError),"直接遥调超时，删除命令，地址=%d,值=%d\n",cmd->m_nInfoAddr,cmd->m_cOnOff);
	// 
	// 					}
	// 				}
	// 				else
	// 				{
	// 					sprintf_s(LastError,sizeof(LastError),"直接遥调超时，删除命令.\n");
	// 				}
	// 				OnShowMsg(LastError,sizeof(LastError));
	// 			
	// 			Q_dataManager* p =m_pMgrIEC103Slave;
	// 			LPQ_YTEDATATAG pYTData=p->getYTData();
	// 			int nCount = p->getYTCount();
	// 			int nChanID = m_ProSetting->getChannelID();
	// 			for (int i=0;i<nCount;i++)
	// 			{
	// 				if (nChanID==pYTData[i].channelid && v_cmd->m_nComAsdu==pYTData[i].device && v_cmd->m_nInfoAddr ==pYTData[i].no)
	// 				{
	// 					pYTData[i].execresult = 2;
	// 					break;
	// 				}
	// 			}
	// 			m_listCmd->removeCommand(v_cmd);
	// 		}
	// 
	// 		switch (v_cmd->m_eState)
	// 		{
	// 		case e103CmdStateYTSelect:
	// 			{
	// 				cmd = (Q_cmdIEC104_SE *)v_cmd;
	// 				if(cmd!=NULL)
	// 				{
	// 					switch(m_cfgControl->m_YsetAsduType)
	// 					{
	// 					case eYsetAsdu30:
	// 						cmd->m_cFuncCode=0x30;
	// 						break;
	// 					case eYsetAsdu31:
	// 						cmd->m_cFuncCode=0x31;
	// 						break;
	// 					case eYsetAsdu32:
	// 						cmd->m_cFuncCode=0x32;
	// 						break;
	// 					default:
	// 						cmd->m_cFuncCode=0x32;
	// 						break;
	// 					}
	// 
	// 					Q_dataManager* p =m_pMgrIEC103Slave;
	// 					LPQ_YTEDATATAG pYTData = p->getYTData();
	// 					int nPos = 0;
	// 					bool bExist = false;
	// 					for (int i=0;i<p->getYTCount();i++)
	// 					{
	// 						if (m_ProSetting->getChannelID()==pYTData[i].channelid && pYTData[i].device==cmd->m_nComAsdu && pYTData[i].no==cmd->m_nInfoAddr)
	// 						{
	// 							bExist = true;
	// 							nPos = i;
	// 							break;
	// 						}
	// 					}
	// 					if (bExist)
	// 					{
	// 						pYTData[nPos].controltype = 0;
	// 						if (cmd->m_cFuncCode==0x32)
	// 						{
	// 							pYTData[nPos].fValue = cmd->m_fVal;
	// 						}
	// 						else
	// 						{
	// 							pYTData[nPos].fValue = cmd->m_cOnOff;
	// 						}
	// 						pYTData[nPos].selecttime = 0;
	// 						pYTData[nPos].selectreplytime = 0;
	// 						pYTData[nPos].execresult = 0;
	// 						pYTData[nPos].finishtime = 0;
	// 						pYTData[nPos].exectime = time(NULL);
	// 					}
	// 					else
	// 					{
	// 						SYSTEMTIME st;
	// 						GetLocalTime(&st);
	// 						sprintf_s(LastError,sizeof(LastError),"%d-%02d-%02d %02d:%02d:%02d 处理ChannelID=%d，device=%d,no=%d的遥调选择执行时，在遥调命令列表中不存在！！！\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,\
	// 							m_ProSetting->getChannelID(),cmd->m_nComAsdu,cmd->m_nInfoAddr);
	// 						OnShowMsg(LastError,sizeof(LastError));
	// 					}
	// 
	// 					if(cmd->m_cFuncCode==0x32)
	// 					{
	// 						send_YT_exe(cmd->m_nComAsdu);
	// 						
	// 							memset(LastError,NULL,sizeof(LastError));
	// 							sprintf_s(LastError,sizeof(LastError),"****  设定短浮点值，模式：直接执行，公共地址=%d,信息体地址=%d,值=%.3f.\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_fVal);
	// 							OnShowMsg(LastError,sizeof(LastError));
	// 						
	// 					}
	// 					else
	// 					{
	// 						send_YT_exe(cmd->m_nComAsdu);
	// 						
	// 							memset(LastError,NULL,sizeof(LastError));
	// 							sprintf_s(LastError,sizeof(LastError),"****  设定归一化值，模式：直接执行，公共地址=%d，信息体地址=%d，值=%d  ****\n",cmd->m_nComAsdu,cmd->m_nInfoAddr,cmd->m_cOnOff);
	// 							OnShowMsg(LastError,sizeof(LastError));
	// 						
	// 					}
	// 				}
	// 			}
	// 			break;
	// 		}
	// 
	// 	}

	return bRet;
}

bool Q_protocolIEC103Slave::startProcess()
{
	  
		sprintf_s(LastError,sizeof(LastError),"初使化开始...");
		//OnShowMsg(LastError,sizeof(LastError));
		OnShowMsg(LastError,strlen(LastError)+1);
	
	m_eProtocolStatus = eCommonStatusNormal;
	return true;

}

bool Q_protocolIEC103Slave::stopProcess()
{
	return true;//20161216 yy

	Q_IEC103Setting *setting=(Q_IEC103Setting *)m_pProcotolConfig->getProtocolSetting();
	if(setting==NULL)
		return false;

	sprintf_s(LastError,sizeof(LastError),"断开链接...通道ID=%d",setting->getChannelID());

	//新规约打印
	OnShowMsg(LastError,strlen(LastError)+1);

	m_listCmd->clearCommand(); //2014-03-14 added

	m_eProtocolStatus = eCommonStatusNull;
	return true;
}

void Q_protocolIEC103Slave::SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen)
{
	//TRACE("Q_protocolIEC104Master::SendMsgPrint,g_rtdbInfIEC104=%08x",m_pMgr->getRtdbInf());

	g_rtdbInfIEC103 = m_pMgrIEC103Slave->getRtdbInf();
	CFEPFuncRtdb *m_rtdbInf = g_rtdbInfIEC103;

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

void Q_protocolIEC103Slave::onTxMsg(BYTE *msgbuf, unsigned int len)
{
	
	SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeTx,msgbuf,len);

}
void Q_protocolIEC103Slave::onRxMsg(BYTE *msgbuf, unsigned int len)
{
   SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeRx,msgbuf,len);
}
void Q_protocolIEC103Slave::OnErrorMsg(char *msg,unsigned int len)
{
	
		SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeError,(BYTE*)msg,len);
	
}
void Q_protocolIEC103Slave::OnShowMsg(char *msg,unsigned int len)
{
	SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)msg,len);
}