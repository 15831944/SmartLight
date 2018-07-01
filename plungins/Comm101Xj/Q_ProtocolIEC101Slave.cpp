#include "stdafx.h"
#include "Q_protocolIEC101Slave.h"
#include "Q_IEC101CommFun.h"
#include "Q_101CP56Time2a.h"
#include "tinyxmlinclude.h"

Q_protocolIEC104Slave::Q_protocolIEC104Slave(void)
{
	bStartFlg = false;
	memset(LastError, 0, sizeof(LastError));
	//setSendUTestFlag(true);
	m_IC_State=eCmdStateNull;
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


/*樊硕重新编写201500801*/
/*初始化遥测遥信数据容器，用于变化上送*/
void Q_protocolIEC104Slave::initVaribleData()
{
    m_vecVarYC.clear();
	m_vecVarYX.clear();

	bZongZhaoComplete = false;

	int nAnalogRecord = m_tags->getAnalogCount();
	CScadaDataBase *dbmg_sda=NULL;

	for(int i=0;i<nAnalogRecord;i++)
	{
		IEC104_VARIBLE_YC *yc=new IEC104_VARIBLE_YC;
		yc->id=m_tags->getAnalogBegin()+i;
		yc->value=0;
		m_vecVarYC.push_back(yc);
	}

	int nDigitRecord = m_tags->getPointCount();
	for(int i=0;i<nDigitRecord;i++)
	{

		IEC104_VARIBLE_YX *yx=new IEC104_VARIBLE_YX;
		yx->id=m_tags->getPointBegin()+i;
		yx->value=false;

		m_vecVarYX.push_back(yx);

		IEC104_VARIBLE_SOE *soe=new IEC104_VARIBLE_SOE;
		soe->id=m_tags->getPointBegin()+i;
		soe->value=false;
		soe->changed=false;
		m_vecVarSOE.push_back(soe);

	}
}


/*清空测遥信容器*/
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
	int temp[Q_FRAME_CONST25] = {};//存储实时库索引
	int tempno = 0;
	Q_IEC104_ASDU msg;
	msg.header.commom_asdu_1=m_cfgBase->m_nAsduAddr & 0xFF;
	msg.header.commom_asdu_2=(m_cfgBase->m_nAsduAddr>>8) & 0xFF;
	msg.header.tx_cause_1 = 0x03;   //传送原因：突发---固定
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
	CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
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

			val= m_vecVarYC[ACsendno-startno]->value - vdiff;//原来是fRaw 2015-12-10 tjs

			if(val>curAnalogAc->fDeadSec||val<-curAnalogAc->fDeadSec)//变化上送死区判断
			{
				m_vecVarYC[ACsendno-startno]->value = vdiff;//更新更改后的值，不管是否被上传tjs

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
	   return true ;//没有数据
	}

	int begin=m_tags->getAnalogBegin();
	int idx=0;

	if(m_cfgSt->m_AsdnYcType==eAsduYcType13)
	{
		Q_MF101 meq;
		float fval=0;
		int n_idx=0;
		while (tempno >= m_cfgSt->n_YcNumPerFrame)
		{
			msg.header.qual =m_cfgSt->n_YcNumPerFrame&0xFF;      //可变结构限定词---组织报文后计算
			msg.header.type = 0x0D;                              //数据类型：13：短浮点---暂时固定

			for (int j = 0; j < m_cfgSt->n_YcNumPerFrame; j++)
			{
				int nTempid32 = temp[j+n_idx];
				dbmg_sdb->GetTabAnalogByID( nTempid32,curAnalogAc);
			
				idx=curAnalogAc->iNoInDPU;
				meq.addr0=idx & 0xFF;
				meq.addr1=idx >>8 & 0xFF;

				memset(tmp,0,sizeof(tmp));
				fval=curAnalogAc->fVal*curAnalogAc->fCoeff;//以前少加了系数了
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

				memcpy(msg.data + j * sizeof(Q_MF101), &meq, sizeof(Q_MF101));
			}

			send_I_Msg((BYTE*)&msg, sizeof(meq)*m_cfgSt->n_YcNumPerFrame+6);	//组织报文发送 
			tempno -= m_cfgSt->n_YcNumPerFrame;
			n_idx+=m_cfgSt->n_YcNumPerFrame;
		}

		if (tempno > 0 && tempno < m_cfgSt->n_YcNumPerFrame)
		{
			msg.header.qual = tempno;       //可变结构限定词---组织报文后计算
			msg.header.type = 0x0D;         //数据类型：13：短浮点---暂时固定

			for (int no = 0; no < tempno; no++)
			{
				int nTempid32 = temp[no+n_idx];
				dbmg_sdb->GetTabAnalogByID(nTempid32,curAnalogAc);
			
				idx=curAnalogAc->iNoInDPU;

				meq.addr0=idx & 0xFF;
				meq.addr1=idx >>8 & 0xFF;
			

				memset(tmp,0,sizeof(tmp));
		
				fval=curAnalogAc->fVal*curAnalogAc->fCoeff;
				memcpy(tmp,&fval, sizeof(fval));

				if(m_cfgBase->m_eFloatSeq==eFloatSeqLittle)
				{
					meq.seqv.val[0]=tmp[3];//从低到高
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

				memcpy(msg.data + no*sizeof(Q_MF101),&meq,sizeof(Q_MF101));
			}

			send_I_Msg((BYTE*)&msg, sizeof(meq)*tempno+6);	//组织报文发送
		    memset(temp, 0, sizeof(temp));	//处理变化数据数组
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
		//非顺序发送
		SHORT nValue =0;
		int n_idx = 0;

		//非顺序发送
		int pi=0;
		Q_MS101 meq;
		memset(&meq,0,sizeof(meq));

		//一般设置走不进来 tjs 2014-3-14
		while (tempno >= m_cfgSt->n_YcNumPerFrame)
		{	
			msg.header.qual = m_cfgSt->n_YcNumPerFrame&0xFF;         //可变结构限定词---组织报文后计算

			for (int j = 0; j < m_cfgSt->n_YcNumPerFrame; j++)
			{
				int nTempid32 = temp[j+n_idx];
				dbmg_sdb->GetTabAnalogByID(  nTempid32,curAnalogAc);
				idx=curAnalogAc->iNoInDPU;
			
				meq.addr0=idx & 0xFF;
				meq.addr1=idx >>8 & 0xFF;
			

				memset(tmp,0,sizeof(tmp));

				nValue = (int)(curAnalogAc->fVal*curAnalogAc->fCoeff);//原来是用fRaw

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

			send_I_Msg((BYTE*)&msg, sizeof(meq)*m_cfgSt->n_YcNumPerFrame+6);	//组织报文发送 
			memset(temp, 0, sizeof(temp));	//处理变化数据数组
			tempno -= m_cfgSt->n_YcNumPerFrame;
			n_idx+=m_cfgSt->n_YcNumPerFrame;
		}

		if (tempno > 0 && tempno < m_cfgSt->n_YcNumPerFrame)
		{
			msg.header.qual = tempno;         //可变结构限定词---组织报文后计算

			for (int no = 0; no < tempno; no++)
			{
				int nTempid32 = temp[no+n_idx];
				dbmg_sdb->GetTabAnalogByID( nTempid32,curAnalogAc);

				meq.addr0=idx & 0xFF;
				meq.addr1=idx >>8 & 0xFF;
			

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

			send_I_Msg((BYTE*)&msg, sizeof(meq)*tempno+6);	//组织报文发送

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
	//if (bZongZhaoComplete == 0)
	//{
	//	return 0;
	//}

	//if(m_vecVarYX.size()<=0)
	//	return 0;

	//if (m_vecVarYX[0] == NULL)
	//{
	//	return 0;
	//}

	//int channeltype = m_ProSetting->getChannelType();

	//Q_cmdIEC104_AC *cmd = (Q_cmdIEC104_AC *)v_cmd;
	//time_t tt;
	//tt= time(NULL);
	////TRACE("handleCommmd_BC");
	//
	//CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
	//CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();


	//if (cmd->m_cOnOff == false || tt - cmd->timeflag < cmd->m_nTime)
	//{
	//	return 0;
	//}

	//cmd->timeflag = time(NULL);
	//int temp[Q_FRAME_CONST32] = {};
	//int tempno = 0;
	//Q_IEC104_ASDU msg;
	//

	////判定是否有变化
	////判定是否有变化
	//int startno = m_tags->getPointBegin();
	//int endno = m_tags->getPointEnd();


	//if (BCsendno >= endno)
	//{
	//	BCsendno = startno;
	//}
	//if(BCsendno<=0)
	//	BCsendno=startno;
	//SCADA_Digit *curDigitBC=NULL;
	//SCADA_Digit *zfDigitBC=NULL;
	//for (int i = BCsendno; i <= endno; i++)
	//{

	//	if (BCsendno < 0)
	//	{
	//		return 0;
	//	}
	//	
	//	int nId32 = BCsendno;
	//	if(dbmg_sdb->GetTabDigitByID(nId32, curDigitBC))
	//	{
	//		curDigitBC->iVal==1?true:false;
	//		if(curDigitBC->iZfID>0)
	//		{
	//			if(dbmg_sdb->GetTabDigitByID(curDigitBC->iZfID,zfDigitBC))
	//			{
	//				curDigitBC->iVal=zfDigitBC->iVal;
	//				SYSTEMTIME systime;
	//				GetLocalTime(systime);
	//				memcpy(&curDigitBC->tSnapTime,&systime,sizeof(curDigitBC->tSnapTime));
	//			}
	//			
	//		}
	//	}
	//	

	//	bool btemp = curDigitBC->iVal==1?true:false;
	//	if(m_vecVarYX[BCsendno-startno]->value != btemp)
	//	{
	//		m_vecVarYX[BCsendno-startno]->value = curDigitBC->iVal==1?true:false;
	//		temp[tempno] = i;//
	//		tempno++;
	//		if(tempno >= Q_FRAME_CONST32)
	//		{
	//			BCsendno= i + 1;
	//			break;
	//		}
	//	}
	//	BCsendno++;
	//}

	////Q_SP104 siq;
	//Q_SP104 siqsets;
	//if (tempno > 0)
	//{
	//	msg.header.commom_asdu_1 = cmd->asdu_addr;//公共地址
	//	msg.header.commom_asdu_2 = 0x00;
	//	msg.header.qual = tempno;         //可变结构限定词---组织报文后计算
	//	msg.header.tx_cause_1 = 0x03;   //传送原因：突发---固定
	//	msg.header.tx_cause_2 = 0x00;
	//	msg.header.type = 0x01;         //数据类型：1


	//	int begin=m_tags->getPointBegin();
	//	int idx=0;


	//	for (int no = 0; no < tempno; no++)
	//	{
	//		//Q_CommUtility::NU_ZERO(&siq,sizeof(siq));


	//		int nTempid32 = temp[no];
	//		if(dbmg_sdb->GetTabDigitByID( nTempid32,curDigitBC))
	//		{
	//			idx=curDigitBC->iNoInDPU;
	//		}
	//		else
	//		{
	//			continue;
	//		}
	//		
	//		

	//		//if (channeltype == 3)//组态通道
	//		//{
	//		//	idx=nTempid32+m_cfgSt->m_YxBeginIndex-1;
	//		//}

	//		//			idx=cur[temp[no]].nIndex + cmd->yxbegin-begin-1;

	//		siqsets.addr0=idx & 0xFF;
	//		siqsets.addr1=idx >>8 & 0xFF;
	//		siqsets.addr2=idx >>16 & 0xFF;
	//		memset(&siqsets.siq,0,sizeof(siqsets.siq));
	//		//			siq.siq.spi = cur[temp[no]].bValue;
	//		siqsets.siq.spi = curDigitBC->iVal==1?1:0;

	//		memcpy(msg.data + no*sizeof(siqsets),&siqsets,sizeof(siqsets));

	//		//			cur[temp[no]].changeflag = false;
	//					//记录SOE数据
	//		m_vecVarSOE[nTempid32-startno]->value = siqsets.siq.spi==1?true:false;	
	//		GetLocalTimeMs(m_vecVarSOE[nTempid32-startno]->timeSOE);		
	//		m_vecVarSOE[nTempid32-startno]->changed = true;

	//		if (no == 59)
	//		{
	//			send_I_Msg((BYTE*)&msg, 4*no+6);	//组织报文发送
	//			memcpy(temp, &temp[60], tempno - 60);
	//			tempno -= 60;
	//			TRACE("delete curDigitBC");
	//			return 0;
	//		}
	//	}
	//	send_I_Msg((BYTE*)&msg, 4*tempno + 6);	//组织报文发送	
	//}

	//memset(temp, 0, sizeof(temp));	//处理变化数据数组
	//tempno = 0;

	//TRACE("delete curDigitBC");
	return 0;
}

int Q_protocolIEC104Slave::handleCommmd_BC_SOE(Q_cmdIEC104 *v_cmd)
{
	
	//if (bZongZhaoComplete == 0)
	//{
	//	return 0;
	//}

	//if(m_vecVarSOE.size()<=0)
	//	return 0;

	//if (m_vecVarSOE[0] == NULL)
	//{
	//	return 0;
	//}

	//Q_cmdIEC104_AC *cmd = (Q_cmdIEC104_AC *)v_cmd;
	//if(!(cmd->m_cSendSOE))
	//{
	//	return 0;
	//}
	//time_t tt;
	//tt= time(NULL);
	//////TRACE("handleCommmd_BC_SOE");
	//if (cmd->m_cOnOff == false || tt - cmd->timeflag < cmd->m_nTime)
	//{
	//	return 0;
	//}

	//cmd->timeflag = time(NULL);
	//Q_IEC104_ASDU msg;

	//int startno = m_tags->getPointBegin();
	//int endno = m_tags->getPointEnd();


	//if (SOEsendno >= endno)
	//{
	//	SOEsendno = startno;
	//}
	//if(SOEsendno<=0 || SOEsendno<startno)
	//{
	//	SOEsendno=startno;
	//}
	//int nTempid32=-1;
	//for(;SOEsendno<endno; SOEsendno++)
	//{
	//	if(m_vecVarSOE[SOEsendno-startno]->changed)
	//	{
	//		nTempid32=m_vecVarSOE[SOEsendno-startno]->id;
	//		m_vecVarSOE[SOEsendno-startno]->changed=false;
	//		break;
	//	}
	//}
	//if(nTempid32>0)
	//{
	//	CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
	//	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
	//	SCADA_Digit *curDigitBC=NULL;
	//	int idx = -1;
	//	if(dbmg_sdb->GetTabDigitByID( nTempid32,curDigitBC))
	//	{
	//		idx=curDigitBC->iNoInDPU;				
	//	}
	//	//Q_SP104 siq;	
	//	if (idx >= 0)
	//	{
	//		Q_SP104 siqsets;
	//		msg.header.commom_asdu_1 = cmd->asdu_addr;//公共地址
	//		msg.header.commom_asdu_2 = 0x00;
	//		msg.header.qual = 0x01;         //可变结构限定词---组织报文后计算
	//		msg.header.tx_cause_1 = 0x03;   //传送原因：突发---固定
	//		msg.header.tx_cause_2 = 0x00;
	//		msg.header.type = 0x1e;         //数据类型：1

	//		siqsets.addr0=idx & 0xFF;
	//		siqsets.addr1=idx >>8 & 0xFF;
	//		siqsets.addr2=idx >>16 & 0xFF;
	//		memset(&siqsets.siq,0,sizeof(siqsets.siq));
	//		siqsets.siq.spi = m_vecVarSOE[SOEsendno-startno]->value==true?1:0;
	//		memcpy(msg.data, &siqsets,sizeof(siqsets));			
	//	
	//		int num = 4;
	//		SYSTEMTIME tSOE = m_vecVarSOE[SOEsendno-startno]->timeSOE;
	//		WORD ms = tSOE.wMilliseconds + tSOE.wSecond*1000;			
	//		msg.data[num++] =  (ms&0x00ff);
	//		msg.data[num++] =  (ms&0xff00) >> 8;
	//		msg.data[num++] =  tSOE.wMinute;
	//		msg.data[num++] =  tSOE.wHour;
	//		msg.data[num++] =  tSOE.wDay | (((tSOE.wDayOfWeek) << 5) & 0x00E0);
	//		msg.data[num++] =  tSOE.wMonth;
	//		msg.data[num++] =  tSOE.wYear - 2000;
	//		send_I_Msg((BYTE*)&msg, 4 + 6 + 7);	//组织报文发送	
	//		
	//	}
	//}

	//TRACE("delete curDigitBC");
	return 0;
}

int Q_protocolIEC104Slave::SendInterrogateData(Q_cmdIEC104 *v_cmd)
{
	//处理遥信
	int i=0;
	Q_IEC104_ASDU msg;
	msg.header.qual=0x00; //顺序发送内容
	msg.header.tx_cause_1=0x14 & 0xFF;
	msg.header.tx_cause_2=0x00 & 0xFF;
//	msg.header.commom_asdu_1=v_cmd->m_nComAsdu & 0xFF;
//	msg.header.commom_asdu_2=(v_cmd->m_nComAsdu>>8) & 0xFF;

	msg.header.commom_asdu_1=m_cfgBase->m_nAsduAddr & 0xFF;
	msg.header.commom_asdu_2=(m_cfgBase->m_nAsduAddr>>8) & 0xFF;
	//fs20150822子站总召唤地址有本地配置决定

	Q_cmdIEC104_IC *cmd=(Q_cmdIEC104_IC *)v_cmd;
	if(cmd!=NULL)
	{
		//根据配置信息 决定总召顺序 遥信、遥测、遥步
		//遥测数据类型、封装的数据数量
		//遥步数据类型、封装的数据数量
		//遥信数据类型、封装的数据数量
		//
		//单点处理遥信
		//
		int len=0;
		if(!cmd->m_bflgSendDigital)
		{
		
			//单点 全部默认连续循址 设置起始地址。
			if(m_tags!=NULL)
			len=m_tags->getPointCount();

			printf("SendInterrogateData digit len=%d,m_nDigitalPos=%d,n_YxNumPerFrame=%d\n",len,cmd->m_nDigitalPos,m_cfgSt->n_YxNumPerFrame);
			if(len>cmd->m_nDigitalPos+m_cfgSt->n_YxNumPerFrame)
			{
				SendIC_DP_Res(&msg,cmd->m_nDigitalPos,m_cfgSt->n_YxNumPerFrame);
				cmd->m_nDigitalPos+=m_cfgSt->n_YxNumPerFrame;
			}
			else
			{
				SendIC_DP_Res(&msg,cmd->m_nDigitalPos,len-cmd->m_nDigitalPos);
				cmd->m_nDigitalPos+=len-cmd->m_nDigitalPos;
				cmd->m_bflgSendDigital=true;
			}
		}
		else
		{
        //发送完遥信然后发送遥测信息
			if(!cmd->m_bflgSendAnlog)
			{
				//单点 全部默认连续循址 设置起始地址。
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
					cmd->m_eState=eCmdStateIntSendDataFin;// 总召发送数据完成
				}
			}
		}
	}
	return Q_104SUC;
}

int Q_protocolIEC104Slave::SendIC_SP_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount)//fs20150727
{

	
	
	return Q_104SUC;
}
//双点
int Q_protocolIEC104Slave::SendIC_DP_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount)
{

	if(m_tags->getPointCount()<=1)
	{
		printf("SendIC_DP_Res this channel data is empty in digit table");
		return 0;
	}
	//2016-11-8 tjs begin

	int channeltype = m_ProSetting->getChannelType();
	msg->header.type=0x03 ;
	msg->header.qual=nCount & 0x8F;//顺序的


	CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

	//	int nAnalogBeginid32 = m_tags->getAnalogBegin();

	int begin = m_tags->getPointBegin();;
	int t_nIdx= nIdx+m_tags->getPointBegin();//上传的数据地址

	int nDigitBeginid32 = m_tags->getPointBegin();


	printf("SendIC_DP_Res 1\n");
	SCADA_Digit BeginDigit;
	dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID,nDigitBeginid32,&BeginDigit);

	begin =  nDigitBeginid32;

	t_nIdx= nIdx+BeginDigit.iNoInDPU;


	SCADA_Digit *curDigitBC=NULL;
	SCADA_Digit *zfDigitBC=NULL;

	//非顺序发送
	int pi=0;
	//Q_SP104  siq;


	msg->data[pi]=(t_nIdx) & 0xFF;
	pi++;
	msg->data[pi]=(t_nIdx) >>8 & 0xFF;
	pi++;
	for (int idx=t_nIdx,ii=nIdx;idx<t_nIdx+nCount;idx++,ii++)
	{
		int ninfoaddr = ii+begin;
		printf("SendIC_DP_Res idx=%d,id=%d\n",idx,ninfoaddr);
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
	    Q_DIQ101	diq;
		if (niRaw == 1)
		{
			diq.dpi=2;
		}
		else
		{
			diq.dpi=1;
		}
		memcpy(&msg->data[pi],&diq,sizeof(Q_DIQ101));

		pi+=sizeof(Q_DIQ101);
	}
	//p->LeaveMutex();
	printf("SendIC_DP_Res send data\n");
	send_I_Msg((BYTE*)msg, nCount*sizeof(Q_DIQ101)+8);


	//处理变化上送数据tjs 2014-7-23	
	SCADA_Digit curDigit;
	int nbegin=m_tags->getPointBegin();
	t_nIdx=nIdx+m_cfgSt->m_YxBeginIndex;

	for (int idx=t_nIdx,ii=nIdx;idx<t_nIdx+nCount;idx++,ii++)
	{
		int nTempid32 = ii+nbegin;
		dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID, nTempid32,&curDigit);	
		m_vecVarYX[ii]->value=curDigit.iVal==1?true:false;
	}
	printf("SendIC_DP_Res send data fin\n");
	return Q_104SUC;
}


int Q_protocolIEC104Slave::SendIC_ME_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount)//fs20150727
{
	
	msg->header.qual=0x7F&nCount;
	//2016-11-8 tjs begin
	
	msg->header.qual=msg->header.qual|0x80;
	if(m_tags->getAnalogCount()<=1)
	{
		printf("SendIC_ME_Res this channel data is empty in analog table\n");
		return 0;
	}

	int channeltype = m_ProSetting->getChannelType();


	CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
	CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
	
	int nAnalogBeginid32 = m_tags->getAnalogBegin();
	
	SCADA_Analog BeginAnalog;
	dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,nAnalogBeginid32,&BeginAnalog);
	
	int nFirstinfoaddr = BeginAnalog.iNoInDPU;//16385
	
	int t_nIdx= nIdx+nFirstinfoaddr;

	printf("SendIC_ME_Res count=%d",nCount);
	SCADA_Analog *curAnalogAc=NULL;
	SCADA_Analog *zfAnalogAc=NULL;
    if(m_cfgSt->m_AsdnYcType==eAsduYcType13)
	{
		msg->header.type=0x0D & 0xFF;
		//顺序发送
		int pi=0;
	
		BYTE tmp[4];
		float fval=0.0;
		for (int idx=t_nIdx,ii=nIdx;idx<t_nIdx+nCount;idx++,ii++)
		{
			int nVID = ii+nAnalogBeginid32;
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

			if(idx==t_nIdx)
			{
				Q_MF101 meq;
				meq.addr0=(idx) & 0xFF;
				meq.addr1=(idx) >>8 & 0xFF;
				//	meq.addr2=(idx) >>16 & 0xFF;

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

				pi+=sizeof(Q_MF101);
			}
			else
			{
				Q_MF101Seq meq;
				memset(tmp,0,sizeof(tmp));
				fval=fval*curAnalogAc->fCoeff;
				memcpy(tmp,&fval,sizeof(fval));

				if(m_cfgBase->m_eFloatSeq==eFloatSeqLittle)
				{
					meq.val[0]=tmp[3];
					meq.val[1]=tmp[2];
					meq.val[2]=tmp[1];
					meq.val[3]=tmp[0];
				}
				else
				{
					meq.val[0]=tmp[0];
					meq.val[1]=tmp[1];
					meq.val[2]=tmp[2];
					meq.val[3]=tmp[3];

				}
				meq.qds.iv=0;
				memcpy(&msg->data[pi],&meq,sizeof(meq));
				pi+=sizeof(Q_MF101Seq);
			}
		}
	    send_I_Msg((BYTE*)msg, (nCount-1)*sizeof(Q_MF101Seq)+sizeof(Q_MF101)+6);
    }
	else if(m_cfgSt->m_AsdnYcType==eAsduYcType9)//9
	{
		msg->header.type=0x09 & 0xFF;
		//非顺序发送
		int pi=0;
	
	
		float fval=0.0;
		for (int idx=t_nIdx,ii=nIdx;idx<t_nIdx+nCount;idx++,ii++)
		{

			int nVID = ii+nAnalogBeginid32;
		
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
			if(idx==t_nIdx)
			{
				Q_MS101 meq;
				meq.addr0=(idx) & 0xFF;
				meq.addr1=(idx) >>8 & 0xFF;
				//	meq.addr2=(idx) >>16 & 0xFF;

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
				  pi+=sizeof(Q_MS101);
			}
			else
			{

				Q_MS101Seq meq;
			
				SYSTEMTIME systime;
				GetLocalTime(systime);
				memcpy(&curAnalogAc->tSnapTime,&systime,sizeof(curAnalogAc->tSnapTime));

				SHORT nValue = (int)(fval*curAnalogAc->fCoeff);
				if(m_cfgBase->m_eIntSeq==eIntSeqLittle)
				{
					meq.val[0]=nValue&0xff;
					meq.val[1]=nValue>>8;
				}
				else if(m_cfgBase->m_eIntSeq==eIntSeqBig)
				{
					meq.val[0]=nValue>>8;
					meq.val[1]=nValue&0xff;
				}
				meq.qds.iv=0;

				memcpy(&msg->data[pi],&meq,sizeof(meq));
				pi+=sizeof(Q_MS101Seq);
			}

		}
		send_I_Msg((BYTE*)msg, sizeof(Q_MS101)+(nCount-1)*sizeof(Q_MS101Seq)+6);
	}

    //处理变化上送数据tjs 2014-7-23
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


void Q_protocolIEC104Slave::SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen)
{
  //TRACE("Q_protocolIEC104Slave::SendMsgPrint,m_pMgrIEC104SlaveProtocolSetting=%08x\n",m_pMgrIEC104SlaveProtocolSetting);


	CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();

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
	else if (nmsgtype == eMsgTypeMsg || nmsgtype == eMsgTypeError|| nmsgtype == eMsgTypeCh)//提示信息、信息值、错误消息、数据解释、规约解析
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

				//因为发现居然给主备都发送，这样是不正确的 tjs 2016-10-28

				//给本机RtServer发送消息
			
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
			//2016-10-28 tjs 因为不应该给备都发送消息

			//TRACE("Q_protocolIEC104Slave::send message3 process id=%d",pProcess->iProcessID);
			m_rtdbInf->getMainDb()->SendMessageTo(pProcess->ID32,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
			/*char strMsg[200] = {0};
			sprintf_s(strMsg,sizeof(strMsg),"MsgPrint:stid = %d msgtype id = %d message = %s",MsgPrint.ichannelid,MsgPrint.msgtype,MsgPrint.msg);*/
			//printf("%s\n",strMsg);
			//TRACE("MsgPrint:stid = %d msgtype id = %d message = %s",MsgPrint.ichannelid,MsgPrint.msgtype,MsgPrint.msg);
		}
	}

}

bool Q_protocolIEC104Slave::startProcess()
{
	//printf("Q_protocolIEC104Slave::startProcess() m_st=%08x m_pMgr=%08x m_ProSetting=%08x\n",m_st,m_pMgr,m_ProSetting);
	printf("Q_protocolIEC104Slave::startProcess()\n");
	m_st->setDefault();
	setDefaultValue();

	m_LinkData.ACD_Sub = 0;
	m_LinkData.FCB_Main = 0;
	m_LinkData.setMasterState(IEC104_LINK_PRM_STATE_NULL);
	m_LinkData.nAckSubCount = 0;

	sprintf_s(LastError, sizeof(LastError), "初使化开始...");

	SendMsgPrint(m_ProSetting->getChannelID(), eMsgTypeError, (BYTE*)LastError, strlen(LastError) + 1);

	m_eProtocolStatus = eCommonStatusNormal;
	

	/*	Q_IEC104StandardCommand *pCommand = new Q_IEC104StandardCommand();
		pCommand->setCommonType(eCommandTypeCycle);
		pCommand->setCommand104Type(eCmdTypeTestFR);
		m_listCmd->addCommand(pCommand);*/

		
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




	return true;
}


bool Q_protocolIEC104Slave::stopProcess()
{
	m_eProtocolStatus = eCommonStatusNull;
	//m_listCmd->clearCommand(); //2013-02-24 清空buffer
	bStartFlg = false;


	//	sprintf_s(LastError,sizeof(LastError),"断开链接...");

	//SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeMsg,(BYTE*)LastError,strlen(LastError)+1);




	/*if (g_rtdbInfIEC104 != NULL)
	{
	g_rtdbInfIEC104->UpdateAnalogQulity(m_ProSetting->getChannelID(),Quality_Bad);

	g_rtdbInfIEC104->UpdateDigitQulity(m_ProSetting->getChannelID(),Quality_Bad);
	}*/


	return true;
}



void Q_protocolIEC104Slave::CP56Time2aToTime(unsigned char *data, SYSTEMTIME *st)
{/*
	unsigned int mili = data[0] | data[1] << 8;
	st->wSecond = mili / 1000;
	st->wMilliseconds = mili - st->wSecond * 1000;

	st->wMinute = data[2] & 0x3F;;
	st->wHour = data[3] & 0x1F;

	st->wDay = data[4] & 0x1F;
	st->wDayOfWeek = (data[4] & 0xE0) >> 5;
	if (st->wDayOfWeek)	// if zero day of week not used.
		st->wDayOfWeek = (st->wDayOfWeek + 1) % 7;
	st->wMonth = data[5] & 0x0F;
	st->wYear = 2000 + (data[6] & 0x7F);*/
}


bool Q_protocolIEC104Slave::terminateProtocol()
{
	m_ProSetting = NULL;
	return true;
}


int Q_protocolIEC104Slave::decodeMsg_U(BYTE *msgbuf, unsigned int len)
{
	/*
	一连上之后会收到这样的报文
	10 80 01 00 81 16 
	10 8B 01 00 8C 16 
	10 C9 01 00 CA 16 
	10 C9 01 00 CA 16 
	10 C9 01 00 CA 16 
	10 C9 01 00 CA 16 
	10 C9 01 00 CA 16 10 C9 01 00 CA 16 10 C9 01 00 CA 16 10 C9 01 00 CA 16 10 C9 01 00 CA 16 10 C9 01 00 CA 16
	规定当没有子站建立起来后，子站的报文要过滤着用
	*/

	//在这里可以连续处理多帧

	if (msgbuf[5] == 0x16)	//frame end char
	{

		if (MakeCheck(msgbuf + 1, 3) != msgbuf[4])	//checksum error
		{
			//m_pChannel->CalFrameCnt(i,IOSCAN_RX_FRAME_CHECKERR);
			return 0;
		}

		//int btFuncCode = msgbuf[1] & CTRLBYTE_FUNC;//前四个字节
		Q_CtrlZone *pi = (Q_CtrlZone *)((BYTE *)(msgbuf)+1);

		int btFuncCode = pi->fun;

		printf("m_LinkData.getMaserState()=%d  fun = %d, prm = %d\n", m_LinkData.getMaserState(), btFuncCode, pi->prm);

		switch (btFuncCode)
		{
			case FUNC_PRM_REQ_LINKSTAT://09
			{
				//当主站未建立完成不处理C9的请求
				if (m_LinkData.getMasterInited()==false)
				{
					//如果主站的状态为建立完成后方回复
					
						linkAckSubLinkStatus();//回复linkstatus
						m_LinkData.setSlaveState(IEC104_LINK_SUB_STATE_STATUSCONF);
					
				}
				else
			    {
					//sendStartDT();
			    }
											
			}
		break;
		case FUNC_PRM_RSTLINK://0
		{
			//子站也可以是复位远方
			
				if (IEC104_LINK_SUB_STATE_STATUSCONF== m_LinkData.getSlaveStatus())
				{
					m_LinkData.setSlaveState(IEC104_LINK_SUB_STATE_RSTCONF);
					linkAckSubReset();
					m_LinkData.setSlaveState(IEC104_LINK_SUB_STATE_INITFIN);
					sendStartDT();//发送启动帧

					m_LinkData.setMasterState(IEC104_LINK_PRM_STATE_REQSTATUS);
					m_LinkData.setSlaveState(IEC104_LINK_SUB_STATE_INITFIN);
					
					
				}
				else if(IEC104_LINK_PRM_STATE_SENDRST== m_LinkData.getMaserState())
				{
					m_LinkData.setMasterState(IEC104_LINK_PRM_STATE_LINKOK);
					//发送一个初使化结束的
					send_InitFinished(1, 0);
				}

		}
		break;
		case FUNC_PRM_REQ_CLASS2://11
		{
			//IEC104_LINK_PRM_STATE_UNRESET 
			if (m_LinkData.getMaserState() == IEC104_LINK_PRM_STATE_REQSTATUS||
				m_LinkData.getMaserState() == IEC104_LINK_PRM_STATE_NULL)
			{			
				//已请求完成
				//发送复位远方状态
				
				linkSendLinkReset();
								
				m_LinkData.setMasterState(IEC104_LINK_PRM_STATE_SENDRST);
			}
			printf("m_LinkData.getMaserState()=%d\n", m_LinkData.getMaserState());
			

		}
		break;
		default:
			break;
		}

	}



	return 0;
}

int Q_protocolIEC104Slave::decodeMsg_I(BYTE *msgbuf, unsigned int len)
{
	//printf("--------Q_protocolIEC104Slave::decodeMsg_I-----\n");
	Q_IEC104_ASDU_header *header = NULL;
	Q_104ApduEx apdu_ex;
	apdu_ex.len = len;
	apdu_ex.msgbuf = msgbuf;
	apdu_ex.seq = false;

	header = (Q_IEC104_ASDU_header*)msgbuf;

	apdu_ex.num = header->qual & 0x7F;
	if (header->qual & 0x80)
		apdu_ex.seq = true;

	apdu_ex.cause = header->tx_cause_1&0x3F;	// 传送原因

	apdu_ex.asdu = header->commom_asdu_2 << 8;
	apdu_ex.asdu |= header->commom_asdu_1;

	apdu_ex.type = header->type;

	//TRACE("=====Q_protocolIEC104Slave::decodeMsg_I len = %d  m_ProSetting = %08x=====\n",len,m_ProSetting);


	sprintf_s(LastError, sizeof(LastError), "ASDU Address=%d\tTypeID=%d\tCount=%d\tCause=%d\n",
		apdu_ex.asdu, header->type, apdu_ex.num, apdu_ex.cause);
	TRACE(LastError);
#ifdef _DEBUG
	//OnErrorMsg(m_errorNo, LastError);
	//新规约打印
	SendMsgPrint(m_ProSetting->getChannelID(), eMsgTypeError, (BYTE*)LastError, strlen(LastError) + 1);
#endif
	bool scaled = false;
	int ret = Q_104SUC;

	

	
	send_S_Msg();
	
	//临时注掉为了测试报警方便
	if (false==m_LinkData.getMasterInited()||false==m_LinkData.getSlaveInited())
	{
		if (70 != header->type)
		{
			return ret;
		}
	}
	time(&this->m_nTimer_U_Test);//记录一下上次收到数的时间;
	// type identification
	switch (header->type)
	{
	case 1: //Single-point information
		func_M_SP_NA_1(&apdu_ex);
		break;
	case 3:	//Double -point information
		func_M_DP_NA_1(&apdu_ex);//原来就没有将数据写入只sprnitf
		break;
	case 9:  //Measured Normalized 正常值
	case 11: //Measured Scaled 测量值、标度化值
		scaled = (header->type == 11) ? true : false;
		func_M_ME_NA_NB_1(&apdu_ex, scaled);//fs20150703
		break;
	case 13: //Measure value, short floating point value
		func_M_ME_NC_1(&apdu_ex);
		break;

	case 30: //Single-point information with time tag CP56Time2a
		func_M_SP_TB_1(&apdu_ex);//fs20150713
		break;
	case 31://双点带时间格式的
		func_M_DP_TB_1(&apdu_ex);
			break;

	case 21:
		func_M_ME_ND_1(&apdu_ex);//规一化值、无时标、无品质
		break;

	case 70:
		//if (m_LinkData.getSlaveStatus() == IEC104_LINK_SUB_STATE_RSTCONF)
		//{//发现子站初使化链路时，有时不发送复位链路,就直接上送完了请求链路后，就直接上送初使化结束
			m_LinkData.setSlaveState(IEC104_LINK_SUB_STATE_INITFIN);
		//}
			bStartFlg = true;
		TRACE("End of Initialization\n");
		break;
	case 100:
		ret = func_Interrogate(&apdu_ex);
		break;
	case 104:
		//解析测试帧
		func_TestFR(&apdu_ex);
		break;
	default:
		decodeMsg_I_Ex(&apdu_ex);
		break;
	}
	bool bSendSFrame = false;

	if (m_cfgBase != NULL)
	{
		bSendSFrame = m_cfgBase->m_bSendSFrame;
	}

	
	return ret;
}

//
//	Interrogation Command confirm
//
int Q_protocolIEC104Slave::send_IC_Response(const int &asdu, const int &cause)
{
	/*TRACE("--------------------Q_protocolIEC104Slave::send_IC_Response------------------------\n");*/
	Q_IEC104_ASDU msg;
	msg.header.type = 0x64;					// interrogation command   总命令
	msg.header.qual = 0x01;					// number of elements
	msg.header.tx_cause_1 = cause;			// 0x07 act confirm  0x0A 停止发送
	msg.header.tx_cause_2 = 0x00;
	msg.header.commom_asdu_1 = asdu & 0xFF;
	msg.header.commom_asdu_2 = (asdu >> 8) & 0xFF;
	// group information
	msg.data[0] = 0x00;
	msg.data[1] = 0x00;
	msg.data[2] = 0x14;//总召 104是两个字节 2017-10-27
	m_LinkData.FCB_Main = 0;//总召一定1

	m_IC_State=eCmdStateIntRequestAct;
	return send_I_MsgInit((BYTE*)&msg, 9);
}






int Q_protocolIEC104Slave::func_Interrogate(Q_104ApduEx *msg)
{
	
	int ret=Q_104SUC;
	if (msg->cause == CMD_ACT) //激活
	{
		Q_cmdIEC104_IC *cmd=new Q_cmdIEC104_IC;

		cmd->setCommand104Type(eCmdTypeC_IC);
		cmd->m_eState=eCmdStateIntRequest;
		cmd->m_nTxCause=msg->cause;
		cmd->m_nComAsdu=msg->asdu;
		cmd->m_bExec=false;
		cmd->m_cQuality=msg->msgbuf[9]; //第10字节就是

	
		cmd->m_bflgSendAnlog=false;
		cmd->m_bflgSendDigital=false;

		m_listCmd->addCommand(cmd);//把命令压栈

	}
	else if (msg->cause == CMD_DEACT) //停止激活
	{
		//查询命令把状态值更新了
		send_IC_Response(msg->asdu,CMD_DEACT_TERM); //停止激活确认
	}


	return ret;
}
//单点遥信上传
void Q_protocolIEC104Slave::func_M_SP_NA_1(Q_104ApduEx *msg)
{
	/*
	UINT pos = 0;
	DWORD adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);

	int i = 0;
	RecordSets dataRec;
	dataRec.eDataType = eDataTypeBit;

	if (msg->seq)
	{
		Q_SP104 *ps = (Q_SP104*)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);


		adr =  ps->addr1 << 8 | ps->addr0;

		pos +=2;//地址域
		Q_SIQ104	*pi;

		while (pos < msg->len)
		{
			pi = (Q_SIQ104*)&(msg->msgbuf[pos]);

			dataRec.rec[i].channelID = m_ProSetting->getChannelID();
			dataRec.rec[i].device = msg->asdu;
			dataRec.rec[i].no = adr;


			dataRec.rec[i].nValue = pi->spi;

			dataRec.num = i + 1;

			if (dataRec.num >= MAX_RECORD_NUM)
			{
				m_pMgr->recordDataValue(dataRec);

				dataRec.num = 0;
				i = -1;
			}

			//fs20150713
			i++;
			pos += sizeof(Q_SIQ104);
			adr++;
		}
	}
	else
	{

		while (pos < msg->len)
		{
			Q_SP104 *ps = (Q_SP104*)&(msg->msgbuf[pos]);

			adr =  ps->addr1 << 8 | ps->addr0;



			dataRec.rec[i].channelID = m_ProSetting->getChannelID();
			dataRec.rec[i].device = msg->asdu;
			dataRec.rec[i].no = adr;


			dataRec.rec[i].nValue = ps->siq.spi;

			dataRec.num = i + 1;

			if (dataRec.num >= MAX_RECORD_NUM)
			{
				m_pMgr->recordDataValue(dataRec);

				dataRec.num = 0;
				i = -1;
			}
			i++;
			pos += sizeof(Q_SP104);
		}
	}

	if (dataRec.num > 0)
	{
		m_pMgr->recordDataValue(dataRec);
	}
}

//双点遥信变位
void Q_protocolIEC104Slave::func_M_DP_NA_1(Q_104ApduEx *msg)
{
	unsigned int pos = sizeof(Q_IEC104_ASDU_header);
	unsigned long adr;//对象地址

	int i = 0;
	RecordSets dataRec;
	dataRec.eDataType = eDataTypeBit;

	if (msg->seq)
	{
		//注:好像有问题:2017-09-19 因为顺序的应该与非顺序的格式不一样的才对呢 tjs
		Q_DP104 *ps = (Q_DP104*)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

		adr =  ps->addr1 << 8 | ps->addr0;
		pos += 2;
		Q_DIQ104	*pi;
		while (pos < msg->len)
		{

			pi = (Q_DIQ104*)&(msg->msgbuf[pos]);


			dataRec.rec[i].channelID = m_ProSetting->getChannelID();
			dataRec.rec[i].device = msg->asdu;
			dataRec.rec[i].no = adr;


			dataRec.rec[i].nValue = pi->dpi;



			dataRec.num = i + 1;

			if (dataRec.num >= MAX_RECORD_NUM)
			{
				m_pMgr->recordDataValue(dataRec);

				dataRec.num = 0;
				i = -1;
			}
			i++;
			adr++;
			pos++;
		}
	}
	else
	{
		;
		while (pos < msg->len)
		{
			Q_DP104 *ps = (Q_DP104*)&(msg->msgbuf[pos]);
			adr = ps->addr1 << 8 | ps->addr0;


			dataRec.rec[i].channelID = m_ProSetting->getChannelID();
			dataRec.rec[i].device = msg->asdu;
			dataRec.rec[i].no = adr;

			dataRec.rec[i].nValue = ps->diq.dpi;


			dataRec.num = i + 1;

			if (dataRec.num >= MAX_RECORD_NUM)
			{
				m_pMgr->recordDataValue(dataRec);

				dataRec.num = 0;
				i = -1;
			}
			i++;
			pos += sizeof(Q_DP104);
		}
	}
	if (dataRec.num > 0)
	{
		m_pMgr->recordDataValue(dataRec);
	}
	*/
}
//带品质描述的规一化值与标度化值
void Q_protocolIEC104Slave::func_M_ME_NA_NB_1(Q_104ApduEx *msg, const bool &scaled)
{
	/*
	UINT pos;
	DWORD adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);
	int i = 0;
	RecordSets dataRec;
	dataRec.eDataType = eDataTypeFloat;

	if (msg->seq) //有序
	{
		//个数是几个

		adr = msg->msgbuf[pos + 1] << 8 | msg->msgbuf[pos];


		pos += 2;


		while (pos < msg->len)
		{
			Q_MS104Seq *pf = (Q_MS104Seq *)&(msg->msgbuf[pos]);
			short val = 0;

			//默认低字节在前
			if (m_cfgBase->m_eIntSeq == eFloatSeqLittle)
			{
				val = (pf->val[1] << 8) | pf->val[0];
			}
			else
			{
				val = (pf->val[0] << 8) | pf->val[1];
			}



			dataRec.rec[i].channelID = m_ProSetting->getChannelID();
			dataRec.rec[i].device = msg->asdu;
			dataRec.rec[i].no = adr;
			dataRec.rec[i].fValue = val;

			dataRec.num = i + 1;

			if (dataRec.num >= MAX_RECORD_NUM)
			{
				m_pMgr->recordDataValue(dataRec);

				dataRec.num = 0;
				i = -1;
			}

			adr++;
			i++;
			pos += sizeof(Q_MS104Seq);
		}

	}
	else
	{


		//非有序
		while (pos < msg->len)
		{
			Q_MS104 *pf = (Q_MS104*)&(msg->msgbuf[pos]);

			adr = pf->addr1 << 8 | pf->addr0;

			short val = 0;
			if (m_cfgBase->m_eIntSeq == eFloatSeqLittle)
			{
				val = (pf->seqv.val[1] << 8) | pf->seqv.val[0];
			}
			else
			{
				val = (pf->seqv.val[0] << 8) | pf->seqv.val[1];
			}
#ifdef _DEBUG
			sprintf_s(LastError,
				"SFP_%d: %3.1f\tOFlow: %d\tValid: %d\tBlocked: %d\n",
				adr, val, pf->seqv.qds.ov, pf->seqv.qds.iv, pf->seqv.qds.bl);
			//OnErrorMsg(m_errorNo, LastError);n
#endif
			// call callback //TODO:
			dataRec.rec[i].channelID = m_ProSetting->getChannelID();
			dataRec.rec[i].device = msg->asdu;
			dataRec.rec[i].no = adr;
			dataRec.rec[i].fValue = val;

			dataRec.num = i + 1;

			if (dataRec.num >= MAX_RECORD_NUM)
			{
				m_pMgr->recordDataValue(dataRec);

				dataRec.num = 0;
				i = -1;
			}
			i++;
			pos += sizeof(Q_MS104);
		}


	}
	if (dataRec.num > 0)
	{
		m_pMgr->recordDataValue(dataRec);
	}
	*/
}
//短浮点数
void Q_protocolIEC104Slave::func_M_ME_NC_1(Q_104ApduEx *msg)
{
	/*
	printf("func_M_ME_NC_1 m_cfgBase->m_eFloatSeq =%d\n ", m_cfgBase->m_eFloatSeq);
	int suc_count = 0;
	UINT pos;
	DWORD adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);
	int i = 0;
	RecordSets dataRec;
	dataRec.eDataType = eDataTypeFloat;

	if (msg->seq) //顺序的
	{

		adr =  msg->msgbuf[pos + 1] << 8 | msg->msgbuf[pos];

		pos += 2;

		while (pos < msg->len)
		{
			Q_MF104Seq *pf = (Q_MF104Seq *)&(msg->msgbuf[pos]);

			BYTE tmp[4];
			float val = 0;
			if (m_cfgBase->m_eFloatSeq == eFloatSeqBig)
			{

				tmp[0] = pf->val[3];//从高到低
				tmp[1] = pf->val[2];
				tmp[2] = pf->val[1];
				tmp[3] = pf->val[0];
			}
			else
			{
				tmp[0] = pf->val[0];
				tmp[1] = pf->val[1];
				tmp[2] = pf->val[2];
				tmp[3] = pf->val[3];
			}
			

			memcpy(&val, tmp, sizeof(val));

			printf("func_M_ME_NC_1 vsq dev=%d addr=%d  val=%.2f\n", msg->asdu, adr, val);

			for (int yy = 0; yy < 4; yy++)
			{
				printf("val[%d]=%.2X ", yy, pf->val[yy]);
			}
		

			dataRec.rec[i].channelID = m_ProSetting->getChannelID();
			dataRec.rec[i].device = msg->asdu;
			dataRec.rec[i].no = adr;
			dataRec.rec[i].fValue = val;

			dataRec.num = i + 1;

			if (dataRec.num >= MAX_RECORD_NUM)
			{
				m_pMgr->recordDataValue(dataRec);

				printf("m_pMgr->recordDataValue count=%d\n",dataRec.num);
				dataRec.num = 0;
				i = -1;
			}
			i++;
			
			adr++;
			pos += sizeof(Q_MF104Seq);
		}



	}
	else
	{


		while (pos < msg->len)
		{
			Q_MF104 *pf = (Q_MF104*)&(msg->msgbuf[pos]);


			adr =  pf->addr1 << 8 | pf->addr0;


			BYTE tmp[4];
			float val = 0;
			if (m_cfgBase->m_eFloatSeq == eFloatSeqBig)
			{

				tmp[0] = pf->seqv.val[3];
				tmp[1] = pf->seqv.val[2];
				tmp[2] = pf->seqv.val[1];
				tmp[3] = pf->seqv.val[0];
			}
			else
			{
				tmp[0] = pf->seqv.val[0];
				tmp[1] = pf->seqv.val[1];
				tmp[2] = pf->seqv.val[2];
				tmp[3] = pf->seqv.val[3];
			}
			
		   

			memcpy(&val, tmp, sizeof(val));
			printf("func_M_ME_NC_1 vsq dev=%d addr=%d  val=%.2f\n", msg->asdu, adr, val);
			for (int yy = 0; yy < 4; yy++)
			{
				printf("val[%d]=%.2X ", yy, pf->seqv.val[yy]);
			}
			dataRec.rec[i].channelID = m_ProSetting->getChannelID();
			dataRec.rec[i].device = msg->asdu;
			dataRec.rec[i].no = adr;
			dataRec.rec[i].fValue = val;

			dataRec.num = i + 1;

			if (dataRec.num >= MAX_RECORD_NUM)
			{
				m_pMgr->recordDataValue(dataRec);

				dataRec.num = 0;
				i = -1;
			}
			i++;


			pos += sizeof(Q_MF104);
		}

	}
	if (dataRec.num > 0)
	{
		printf("m_pMgr->recordDataValue count=%d\n", dataRec.num);
		m_pMgr->recordDataValue(dataRec);
	}
	*/
}

//带时标的单点遥信
void Q_protocolIEC104Slave::func_M_SP_TB_1(Q_104ApduEx *msg)
{
	/*
	printf("Single-point information with time tag CP56Time2a\n");
	int suc_count = 0;
	UINT pos;
	DWORD adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq)
	{
		Q_SP104 *ps = (Q_SP104*)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);
		adr =  ps->addr1 << 8 | ps->addr0;

		pos += 2;//地址域
		Q_SIQ104  *pi;


		while (pos < msg->len)
		{
			pi = (Q_SIQ104*)&(msg->msgbuf[pos]);

			//fs20150713
			Q_SOEDATATAG dataArray;
			dataArray.channelID = m_ProSetting->getChannelID();
			dataArray.device = msg->asdu;
			dataArray.no = adr;
			dataArray.nValue = pi->spi;

			if (0 == pi->iv)
			{
				dataArray.nQuality = Quality_Good;
			}
			else
			{
				dataArray.nQuality = Quality_Bad;
			}

			Q_CP56Time2a t2(&msg->msgbuf[pos + sizeof(Q_SIQ104)]);
			dataArray.tTime = t2._GetSystemTime();
			char buffer[30];

			t2.GetTimeString(buffer, 30);
			printf("time=%s\n", buffer);

			if (!m_pMgr->recordSOEValue(&dataArray))
			{

				sprintf_s(LastError, sizeof(LastError), "func_M_SP_TB_1 Can't find right YX point in realDB,Channel=%d,device=%d,addr=%d", m_ProSetting->getChannelID(), msg->asdu, adr);

				SendMsgPrint(m_ProSetting->getChannelID(), eMsgTypeCh, (BYTE*)LastError, strlen(LastError) + 1);


			}
			//fs20150713

			pos += (sizeof(Q_SIQ104) + 7);//CP56Time2a时间站7个字节，不解析了
			adr++;
		}
	}
	else
	{
		while (pos < msg->len)
		{
			Q_SP104 *ps = (Q_SP104*)&(msg->msgbuf[pos]);

			adr =  ps->addr1 << 8 | ps->addr0;

			//fs20150713
			Q_SOEDATATAG dataArray;
			dataArray.channelID = m_ProSetting->getChannelID();
			dataArray.device = msg->asdu;
			dataArray.no = adr;
			dataArray.nValue = ps->siq.spi;

			if (0 == ps->siq.spi)
			{
				dataArray.nQuality = Quality_Good;
			}
			else
			{
				dataArray.nQuality = Quality_Bad;
			}

			Q_CP56Time2a t2(&msg->msgbuf[pos + sizeof(Q_SP104)]);
			dataArray.tTime = t2._GetSystemTime();
			char buffer[30];

			t2.GetTimeString(buffer, 30);
			printf("time=%s\n", buffer);

			if (m_pMgr!=NULL)
			{
				if (!m_pMgr->recordSOEValue(&dataArray))
				{
					sprintf_s(LastError, sizeof(LastError), "func_M_SP_TB_1 Can't find right YX point in realDB,Channel=%d,device=%d,addr=%d", m_ProSetting->getChannelID(), msg->asdu, adr);
					SendMsgPrint(m_ProSetting->getChannelID(), eMsgTypeCh, (BYTE*)LastError, strlen(LastError) + 1);
				}
			}
			else
			{
				printf("func_M_SP_TB_1 m_pMgr =null \n");
			}
			//fs20150713

			pos += (sizeof(Q_SP104) + 7);//CP56Time2a时间站7个字节，不解析了
		}
	}
	*/


}
//带时标的短浮点遥测
void Q_protocolIEC104Slave::func_M_ME_TF_1(Q_104ApduEx *msg)
{
	/*
	int suc_count = 0;
	UINT pos;
	DWORD adr;//对象地址
	pos = sizeof(Q_IEC104_ASDU_header);
	if (msg->seq) //顺序的
	{
		adr =  msg->msgbuf[pos + 1] << 8 | msg->msgbuf[pos];

		while (pos < msg->len)
		{
			pos += 2;//fix by yf
			Q_MF104Seq *pf = (Q_MF104Seq *)&(msg->msgbuf[pos]);
			float *val = (float *)&pf->val[0];


			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType = eDataTypeFloat;
			dataArray.channelID = m_ProSetting->getChannelID();
			dataArray.device = msg->asdu;
			dataArray.no = adr;
			dataArray.fValue = *val;

			if (!m_pMgr->recordDataValue(&dataArray))
			{
				if (m_ProSetting != NULL)
				{
					sprintf_s(LastError, sizeof(LastError), "Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d", m_ProSetting->getChannelID(), msg->asdu, adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(), eMsgTypeCh, (BYTE*)LastError, strlen(LastError) + 1);
				}

			}
			else
			{
				suc_count++;
			}

			adr++;
			pos += (sizeof(Q_MF104) + 7);
		}

	}
	else
	{
		while (pos < msg->len)
		{
			Q_MF104 *pf = (Q_MF104*)&(msg->msgbuf[pos]);

			adr =  pf->addr1 << 8 | pf->addr0;
			float *val = (float*)&pf->seqv.val[0];

			//fs20150713
			Q_RECORDDATATAG dataArray;
			dataArray.eDataType = eDataTypeFloat;
			dataArray.channelID = m_ProSetting->getChannelID();
			dataArray.device = msg->asdu;
			dataArray.no = adr;
			dataArray.fValue = *val;

			if (!m_pMgr->recordDataValue(&dataArray))
			{
				if (m_ProSetting != NULL)
				{
					sprintf_s(LastError, sizeof(LastError), "Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d", m_ProSetting->getChannelID(), msg->asdu, adr);
					//OnShowMsg(LastError,sizeof(LastError));
					//新规约打印
					SendMsgPrint(m_ProSetting->getChannelID(), eMsgTypeCh, (BYTE*)LastError, strlen(LastError) + 1);
				}

			}
			else
			{
				suc_count++;
			}

			pos += (sizeof(Q_MF104) + 7);
		}

	}

	sprintf_s(LastError, sizeof(LastError), "程序退出类型标识为0x24数据解析,此帧共有%d个遥测数据，成功入库个数%d\n", msg->num, suc_count);
	SendMsgPrint(m_ProSetting->getChannelID(), eMsgTypeCh, (BYTE*)LastError, strlen(LastError) + 1);
	*/
}



bool Q_protocolIEC104Slave::time_to_IC()
{
	bool ret = false;
	time_t now;
	time(&now);
	if ((now - m_tIC) >= m_cfgSt->n_IcInterval)
	{
		ret = true;
	}
	return ret;
}




//双点带时标的
void Q_protocolIEC104Slave::func_M_DP_TB_1(Q_104ApduEx *msg)
{

	
}
int Q_protocolIEC104Slave::handleCommmd_IC(Q_cmdIEC104 *v_cmd)
{

	int ret=Q_104SUC;

	v_cmd->m_bExec=true;
	switch (v_cmd->m_eState)
	{
	case eCmdStateIntRequest:
		{
			//发送请求确认针
			send_IC_Response(v_cmd->m_nComAsdu,CMD_ACT_CON);
			
			v_cmd->m_eState=eCmdStateIntRequestAct;//请求确认完成
		}
		break;
	case eCmdStateIntRequestAct:
		{
			//从站主动发送总召数据
			SendInterrogateData(v_cmd);
			v_cmd->m_eState=eCmdStateIntSendData;
		}
		break;
	case eCmdStateIntSendData:
		{
			//从站主动发送总召数据
			SendInterrogateData(v_cmd);
		}
		break;

	case eCmdStateIntSendDataFin:
		{
			bZongZhaoComplete = true;
			send_IC_Response(v_cmd->m_nComAsdu,CMD_DEACT_TERM);//发送总召结束
			v_cmd->m_eState=eCmdStateIntEnd;
			Q_IEC104CommandManager *cmdList=(Q_IEC104CommandManager *)m_pProcotolConfig->getProtocolCommandManager();
			if(cmdList!=NULL)
			{
				cmdList->removeCommand(v_cmd);
				//移除命令
			}
		}
		break;
	}
	return 0;
}
void Q_protocolIEC104Slave::func_M_ME_ND_1(Q_104ApduEx *msg)
{


}
void Q_protocolIEC104Slave::func_M_DP_NA_1(Q_104ApduEx *msg)
{
	

}
int Q_protocolIEC104Slave::func_TestFR(Q_104ApduEx *msg)
{
	int ret = Q_104SUC;

	Q_IEC104_TEST_FR *ps = (Q_IEC104_TEST_FR *)&(msg->msgbuf[sizeof(Q_IEC104_ASDU_header)]);

	if (msg->cause == CMD_ACT) //激活
	{
		//看一下测
		if (ps->data1 == 0xAA && ps->data2 == 0x55)
		{
			m_bSendTestFR = false;
			time(&m_nTimer_U_Test);

			sprintf_s(LastError, sizeof(LastError), "func_TestFR slave station ack success");
		}
		else
		{
			sprintf_s(LastError, sizeof(LastError), "func_TestFR slave station ack error");
		}

		SendMsgPrint(m_ProSetting->getChannelID(), eMsgTypeCh, (BYTE*)LastError, strlen(LastError) + 1);
	}

	//68 0D 0D 68 C4 01 00 68 01 07 00 01 00 00 00 AA 55 35 16 
	Q_IEC104_ASDU buf;
	buf.header.type = 0x68;					
	buf.header.qual = 0x01;					
	buf.header.tx_cause_1 = CMD_ACT_CON;			
	buf.header.tx_cause_2 = 0x00;
	buf.header.commom_asdu_1 =msg->asdu & 0xFF;
	buf.header.commom_asdu_2 = (msg->asdu >> 8) & 0xFF;
	// group information
	buf.data[0] = 0x00;
	buf.data[1] = 0x00;
	buf.data[2] = 0xAA;
	buf.data[3] = 0x55;
	m_LinkData.FCB_Main = 0;//总召一定1

	return send_I_MsgInit((BYTE*)&buf,10);

}
int Q_protocolIEC104Slave::send_InitFinished(const int &asdu, const int &cause)
{

	Q_IEC104_ASDU msg;
	msg.header.type = 0x46;					// 初使化结束
	msg.header.qual = 0x01;					// 
	msg.header.tx_cause_1 = 0x04;			// 0x07 act confirm  0x0A 停止发送
	msg.header.tx_cause_2 = 0x00;
	msg.header.commom_asdu_1 = asdu & 0xFF;
	msg.header.commom_asdu_2 = (asdu >> 8) & 0xFF;
	// group information
	msg.data[0] = 0x00;
	msg.data[1] = 0x00;
	msg.data[2] = 0x02;//总召 104是两个字节 2017-10-27
	m_LinkData.FCB_Main = 0;//总召一定1
	return send_I_MsgInit((BYTE*)&msg, 9);
}
