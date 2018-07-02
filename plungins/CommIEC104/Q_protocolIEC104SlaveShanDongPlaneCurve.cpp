#include "stdafx.h"
#include "Q_protocolIEC104SlaveShanDongPlaneCurve.h"
#include "Q_IEC104CommFun.h"
#include "Q_104CP56Time2a.h"
#include "tinyxmlinclude.h"
#include "tinyxmlinclude.h"

Q_protocolIEC104SlaveShanDongPlaneCurve::Q_protocolIEC104SlaveShanDongPlaneCurve()
{
	m_localAgcPlaneCurveYcIdStart=-1;
	m_localAgcPlaneCurveYcIdEnd=-1;

	m_RemoteAgcPlaneCurveYcIdStart=-1;
	m_RemoteAgcPlaneCurveYcIdEnd=-1;

	m_localAvcPlaneCurveYcIdStart=-1;
	m_localAvcPlaneCurveYcIdEnd=-1;

	m_RemoteAvcPlaneCurveYcIdStart=-1;
	m_RemoteAvcPlaneCurveYcIdEnd=-1;
}
int Q_protocolIEC104SlaveShanDongPlaneCurve::handleCommmd_SE(Q_cmdIEC104 *v_cmd)//ɽ���汾ң�������
{
	int ycId = -1;
	bool bExist = false;
	if(v_cmd->m_eState == eCmdStateYkExecAct)
	{
		Q_cmdIEC104_SE *cmd = (Q_cmdIEC104_SE *)v_cmd;

		if(cmd!=NULL)
		{
			if(cmd->m_nExecFlg>0)
			{
				//��˶�޸�20150728
				//�ж�ң�����Ƿ����
				int nPos = 0;				
				int YTtoYCID32 = -1;
				int	YTID32 = -1;	

				Q_cmdIEC104_SE *cmd = (Q_cmdIEC104_SE *)v_cmd;
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
						ycId = curSetPoint.ulYCID32;
						break;
					}
				}
			}
		}
	}
	int res = Q_protocolIEC104Slave::handleCommmd_SE(v_cmd);
	Q_cmdIEC104_SE *cmd = (Q_cmdIEC104_SE *)v_cmd;
	if(bExist) //ң��ִ��
	{
		SCADA_Analog *curAnalogAc=NULL;
		CFEPFuncRtdb *m_rtdbInf = m_pMgrIEC104SlaveProtocolSetting->getRtdbInf();
		CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
		if(dbmg_sdb->GetTabAnalogByID(ycId, curAnalogAc))
		{
			if(ycId>=m_cfgBase->m_lAgcPlCYCStart && ycId<=m_cfgBase->m_lAgcPlCYCEnd) //ң��AGC���ؼƻ�ֵ
			{						
				writeFile(m_cfgBase->m_lAgcPath, curAnalogAc->fVal, ycId - m_cfgBase->m_lAgcPlCYCStart + 1);			
			}
			else if(ycId>=m_cfgBase->m_lAvcPlCYCStart && ycId<=m_cfgBase->m_lAvcPlCYCEnd)//ң��AVC���ؼƻ�ֵ�ı�
			{
				writeFile(m_cfgBase->m_lAvcPath, curAnalogAc->fVal, ycId - m_cfgBase->m_lAvcPlCYCStart + 1);
			}
		}
	}
	return res;
}


bool Q_protocolIEC104SlaveShanDongPlaneCurve::initializeProtocol()
{
	Q_protocolIEC104Slave::initializeProtocol();
	initPlaneCurveDate();
	return true;
}

int Q_protocolIEC104SlaveShanDongPlaneCurve::initPlaneCurveDate()//�Ѽƻ������ļ����ݶ�ȡ��д�뵽�ƻ����߶�Ӧ��ң���
{
	float fValue[288] ;
	for(int i=0; i<288; i++)
	{
		fValue[i] = -1;
	}
	int res = readLocalPlaneCurveFile(m_cfgBase->m_lAgcPath, fValue);
	if(res == 0)
	{
		saveDataToYC(m_cfgBase->m_lAgcPlCYCStart, m_cfgBase->m_lAgcPlCYCEnd, fValue);
	}

	for(int i=0; i<288; i++)
	{
		fValue[i] = -1;
	}
	res = readRemoutePlaneCurveFile(m_cfgBase->m_rAgcPath, fValue);
	if(res == 0)
	{
		saveDataToYC(m_cfgBase->m_rAgcPlCYCStart, m_cfgBase->m_rAgcPlCYCEnd, fValue);
	}

	for(int i=0; i<288; i++)
	{
		fValue[i] = -1;
	}
	res = readLocalPlaneCurveFile(m_cfgBase->m_lAvcPath, fValue);
	if(res == 0)
	{
		saveDataToYC(m_cfgBase->m_lAvcPlCYCStart, m_cfgBase->m_lAvcPlCYCEnd, fValue);
	}

	for(int i=0; i<288; i++)
	{
		fValue[i] = -1;
	}
	res = readRemoutePlaneCurveFile(m_cfgBase->m_rAvcPath, fValue);
	if(res == 0)
	{
		saveDataToYC(m_cfgBase->m_rAvcPlCYCStart, m_cfgBase->m_rAvcPlCYCEnd, fValue);
	}
	return 0;

}
int Q_protocolIEC104SlaveShanDongPlaneCurve::readRemoutePlaneCurveFile(const char* shanDongFilePath, float fValue[96])
{
	for(int i=0; i<96; i++)//�����е�����Ԥ��Ϊ-1
	{
		fValue[i] = -1;
	}
	FILE* f = fopen(shanDongFilePath, "rb");
	char linebuf[4096];
	if(f) 
	{  		
		int valeId=0;		
		while(!feof(f)) 
		{			
			while(fgets(linebuf, 4096, f)) 
			{ 
				if(linebuf[0] == '#')//
				{   					
//					double value;
					char id[20]; //һ�����ݵ�id��
					char pro_val[40];//һ�����ݵ�����ֵ
					sscanf(linebuf, "%s%s%f",id,pro_val,&(fValue[valeId]));  
					valeId++;
					if(valeId>96)
					{
						printf("readRemoteAgcPlaneCurveFile,AGC�ƻ������ļ��������ݵ�̫��\r\n");
						return -1;
					}
				}				
			}			
		} 
		return 0; //��ȡ�ɹ�
	}   
	return -2;//��ȡʧ��
}
int Q_protocolIEC104SlaveShanDongPlaneCurve::readLocalPlaneCurveFile(const char* shanDongFilePath, float fValue[288])
{
	for(int i=0; i<288; i++)//�����е�����Ԥ��Ϊ-1
	{
		fValue[i] = -1;
	}
	TiXmlDocument doc(shanDongFilePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
		TRACE("--------read plan xml failed\n");
		return -1;
	}

	TiXmlNode* node = 0;
	TiXmlNode* root = 0;
	TiXmlNode* nodeItem = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	root = doc.FirstChild( "Record" );
	todoElement = root->ToElement();
	assert( todoElement  );
   
	//vec.clear();//2012-5-20 add
	bool ret=false;
	int i=0;
	for (nodeItem=todoElement->FirstChild("Field");nodeItem;
		nodeItem=nodeItem->NextSibling("Field"))
	{
		itemElement=nodeItem->ToElement();

		//AGC_PLAN_CURVE plan;
		/*plan.exec=atoi(itemElement->Attribute("se"))==0?true:false;
		plan.id=atoi(itemElement->Attribute("id"));
		plan.val=atof(itemElement->Attribute("value"));*/
		int id=atoi(itemElement->Attribute("id"));
		if(id == 0)//��Ҫ��һ������
		{
			continue;
		}
		if(i>=288)
		{
			return -2; //���ݵ�̫�࣬�ļ�����
		}
		fValue[i++] = atof(itemElement->Attribute("value"));
        //vec.push_back(plan);
		
	}
	return 0;//��ȡʧ��
}

int Q_protocolIEC104SlaveShanDongPlaneCurve::saveDataToYC(int startId, int endId, float fValue[])
{
	for(int adr=startId; adr<=endId;  adr++)
	{
		Q_RECORDDATATAG dataArray;

		dataArray.eDataType=eDataTypeFloat;
		dataArray.channelID=m_ProSetting->getChannelID();
		//dataArray.device=msg->asdu;
		dataArray.nIndex=adr;
		dataArray.fValue=fValue[adr - startId];

		//printf("--------Q_protocolIEC104Master::func_M_ME_NC_1-----333 %d,%d,%d,%f-----\n",
		//	dataArray.channelID,dataArray.device,dataArray.no,dataArray.fValue);
		//ϵ��ͳһ��recordDataValue���������� comment by tjs 2016-06-21
		char LastError     [Q_FRAME_CONST256];//fs
		if(!m_pMgrIEC104SlaveProtocolSetting->recordDataValue(&dataArray, 1, true)) //����YC��ĵ�Ÿ�������
		{
			if(m_ProSetting!=NULL)
			{
				sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Please check yc.xml ,Channel=%d,ycID=%d",m_ProSetting->getChannelID(),adr);
				//OnShowMsg(LastError,sizeof(LastError));
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
			}
			if (m_bInfoMsg)
			{
				sprintf_s(LastError,sizeof(LastError),"Can't find right YC point in realDB,Please check yc.xml ,Channel=%d,ycID=%d",m_ProSetting->getChannelID(),adr);
				//OnInfoMsg(LastError,strlen(LastError)+1);
				//�¹�Լ��ӡ
				SendMsgPrint(m_ProSetting->getChannelID(),eMsgTypeCh,(BYTE*)LastError,strlen(LastError)+1);
			}
		}

	}

	return 0;
}


int Q_protocolIEC104SlaveShanDongPlaneCurve::writeFile(const char* xmlFileTemplatePath,float value, int id)
{
	TiXmlDocument doc(xmlFileTemplatePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
		TRACE("--------read plan xml failed\n");
		return false;
	}

	TiXmlNode* node = 0;
	TiXmlNode* root = 0;
	TiXmlNode* nodeItem = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	root = doc.FirstChild( "Record" );
	todoElement = root->ToElement();
	assert( todoElement);

	bool ret=false;
	for (nodeItem=todoElement->FirstChild("Field");nodeItem;
		nodeItem=nodeItem->NextSibling("Field"))
	{
		itemElement=nodeItem->ToElement();
		int valueId =atoi(itemElement->Attribute("id"));
		if(valueId == id)
		{			
			itemElement->SetAttribute("se",1);
			itemElement->SetDoubleAttribute("value",value);//ɽ������ÿ��
		}
	}
	bool res = doc.SaveFile(xmlFileTemplatePath);
	return res;
}