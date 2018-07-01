#include "Q_GSetting.h"
#include "tinyxmlinclude.h"
#include "Log.h"

Q_GSetting::Q_GSetting()
{
   memset(m_Path,sizeof(m_Path),sizeof(m_Path));
   m_AlgCount=0;
   m_PointCount=0;
   m_nChannelCount=0;
   m_nServerUnitCount=0;
   m_nChannelCountRes=0;
   m_CurMasterYCID = -1;
   m_log=NULL;
   
}
Q_GSetting::~Q_GSetting()
{
	
}
void Q_GSetting::setXmlPath(char *vPath)
{
 sprintf_s(m_Path,sizeof(m_Path),"%s",vPath);
}
bool Q_GSetting::getAutoRun()
{
	bool flg=false;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(1,1);
		if(strcmp(tmp,"false")==0)
		{
			flg=false;
		}
		else
		{
           flg=true;
		}
	}
	return flg;
}
bool Q_GSetting::getAgcalgorithm()
{
	bool flg=false;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(2,1);
		if(strcmp(tmp,"false")==0)
		{
			flg=false;
		}
		else
		{
           flg=true;
		}
	}
	return flg;
}
bool Q_GSetting::getAvcalgorithm()
{
	bool flg=false;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(3,1);
		if(strcmp(tmp,"false")==0)
		{
			flg=false;
		}
		else
		{
           flg=true;
		}
	}
	return flg;
}
bool Q_GSetting::getDataHandle()
{
	bool flg=false;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(4,1);
		if(tmp!=NULL)
		if(strcmp(tmp,"false")==0)
		{
			flg=false;
		}
		else
		{
           flg=true;
		}
	}
	return flg;
}
bool Q_GSetting::getDataRecordFlg()
{
	bool flg=false;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(5,1);
		if(tmp!=NULL)
		{
			if(strcmp(tmp,"false")==0)
			{
				flg=false;
			}
			else
			{
			   flg=true;
			}
		}
		
	}
	return flg;
}
bool Q_GSetting::getSampleGenerator()
{
	bool flg=false;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(100,1);
		if(tmp!=NULL)
		{
			if(strcmp(tmp,"false")==0)
			{
				flg=false;
			}
			else
			{
				flg=true;
			}
		}

	}
	return flg;
}

bool Q_GSetting::getFCCounting()
{
	bool flg=false;
	if(m_vContent.size()>0)
	{
		char *tmp = getContentID(200,1);
		if(tmp!=NULL)
		{
			if(strcmp(tmp,"false")==0)
			{
				flg=false;
			}
			else
			{
				flg=true;
			}
		}

	}
	return flg;
}

bool Q_GSetting::getSwitchWhenDiaoCrash()
{
	bool flg=false;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(12,1);
		if(tmp!=NULL)
		{
			if(strcmp(tmp,"false")==0)
			{
				flg=false;
			}
			else
			{
				flg=true;
			}
		}

	}
	return flg;
}

int Q_GSetting::getDiaoduChannelID()
{
	int nChannelID=-1;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(13,1);
		if(tmp!=NULL)
		{
			nChannelID = atoi(tmp);
		}
	}
	return nChannelID;
}

int Q_GSetting::getSwithSecond()
{
	int nSwithSecond=-1;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(14,1);
		if(tmp!=NULL)
		{
			nSwithSecond = atoi(tmp);
		}
	}
	return nSwithSecond;
}
int Q_GSetting::getSwitchYXID()
{
	int nYXID=0;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(15,1);
		if(tmp!=NULL)
		{
			nYXID = atoi(tmp);
		}
	}
	return nYXID;
}


bool Q_GSetting::getDataSaveFlg()
{
	bool flg=false;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(6,1);
		if(tmp!=NULL)
		{
			if(strcmp(tmp,"false")==0)
			{
				flg=false;
			}
			else
			{
				flg=true;
			}
		}

	}
	return flg;
}

int Q_GSetting::getDataSaveTime()
{
	int nTime=60;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(7,1);
		if(tmp!=NULL)
		{
			nTime = atoi(tmp);
		}
		if (nTime<1)
		{
			nTime = 60;
		}
	}
	return nTime;
}

bool Q_GSetting::init()
{
	
	bool flg1=true,flg2=true,flg3=true;
	/*flg1=initBase();
	if(flg1!=true)
	{
       printf("Q_GSetting::init initBase failed\n");
	}*/

	flg2=initChannelFromRtdb();
	if(flg2!=true)
	{
       printf("Q_GSetting::init initChannelFromRtdbfailed\n");
	}
	/*flg3=initNode();
	if(flg3!=true)
	{
        printf("Q_GSetting::init initNode failed\n");
	}*/

	return flg1&flg2&flg3;


}

bool Q_GSetting::initBase()
{
	//initial invert info from config.xml

	char m_PathBase[MAX_PATH];
	sprintf_s(m_PathBase,sizeof(m_PathBase),"%sbase.xml",m_Path);

	TiXmlDocument doc(m_PathBase);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
		sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件格式有误\n错误描述%s\n错误位置：行%d ,列%d",m_PathBase,doc.ErrorDesc(),doc.ErrorRow(),doc.ErrorCol());
		Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
		return false;
	}

	TiXmlNode* node = 0;
	TiXmlNode* root = 0;
//	TiXmlNode* nodeItem = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	root = doc.FirstChild( "Setting" );
	todoElement = root->ToElement();

//	const char* pAttr = NULL;
	for ( node = todoElement->FirstChild( "Field" );
		node;
		node = node->NextSibling( "Field" ) )
	{
		itemElement=node->ToElement();
		ST_GeCfgT content;
		content.grpid=1;
		content.id=atoi(itemElement->Attribute("id"));
		sprintf_s(content.name,sizeof(content.name),"%s",itemElement->Attribute("name"));
		sprintf_s(content.value,sizeof(content.value),"%s",itemElement->Attribute("value"));
		m_vContent.push_back(content);
	}
	return true;

	//初始化结点
	//初始化channel
}

bool Q_GSetting::initChannel()
{
	//initial invert info from config.xml
//	CString strErr;

	char m_PathBase[MAX_PATH];
	sprintf_s(m_PathBase,sizeof(m_PathBase),"%schannel.xml",m_Path);

	TiXmlDocument doc(m_PathBase);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
		sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件格式有误\n错误描述%s\n错误位置：行%d ,列%d",m_PathBase,doc.ErrorDesc(),doc.ErrorRow(),doc.ErrorCol());
		Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
		return false;
	}

	TiXmlNode* node = 0;
	TiXmlNode* root = 0;
	TiXmlNode* nodeItem = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	root = doc.FirstChild( "Setting" );
	todoElement = root->ToElement();

	const char* pAttr = NULL;
	for ( node = todoElement->FirstChild( "Field" );
		node;
		node = node->NextSibling( "Field" ) )
	{
		itemElement=node->ToElement();
		
		ST_GeCtrlChl ch;
		//ch.grpid=grpid;
		pAttr =itemElement->Attribute("id");
		if (NULL==pAttr)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中通道配置id有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		ch.id=atoi(pAttr);
		
		pAttr = itemElement->Attribute("name");
		if (NULL==pAttr)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中通道配置name有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		sprintf_s(ch.name,sizeof(ch.name),"%s",pAttr);
		pAttr = itemElement->Attribute("enabled");
		if (NULL==pAttr)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中通道配置enabled有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		if(strcmp(pAttr,"true")==0)
		{
			ch.enabled=true;
		}
		else
		{
			ch.enabled=false;
		}
		pAttr = itemElement->Attribute("type");
		if (NULL==pAttr)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中通道配置type有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		ch.type=atoi(pAttr);
		pAttr = itemElement->Attribute("cfgfile");
		if (NULL==pAttr)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中通道配置cfgfile有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		sprintf_s(ch.cfgfile,sizeof(ch.cfgfile),"%s",pAttr);
	

		pAttr = itemElement->Attribute("protocol");
		if (NULL==pAttr)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中通道配置protocol有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		sprintf_s(ch.protocol,sizeof(ch.protocol),"%s",pAttr);
		pAttr = itemElement->Attribute("link");
		if (NULL==pAttr)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中通道配置link有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		sprintf_s(ch.link,sizeof(ch.link),"%s",pAttr);
		pAttr =itemElement->Attribute("statusid");
		if (NULL==pAttr)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中通道配置statusid有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		ch.chStatusid = atoi(itemElement->Attribute("statusid"));
		
		m_vChannel.push_back(ch);
		
	}
	return true;
}

bool Q_GSetting::initNode()
{
	//initial invert info from config.xml
	//CString strErr;

	char m_PathBase[MAX_PATH];
	sprintf_s(m_PathBase,sizeof(m_PathBase),"%snode.xml",m_Path);

	TiXmlDocument doc(m_PathBase);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{

		sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件格式有误\n错误描述%s\n错误位置：行%d ,列%d",m_PathBase,doc.ErrorDesc(),doc.ErrorRow(),doc.ErrorCol());
		Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
		return false;
	}

	TiXmlNode* node = 0;
	TiXmlNode* root = 0;
	TiXmlNode* nodeItem = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	root = doc.FirstChild( "Setting" );
	todoElement = root->ToElement();

	const char* pAttr = NULL;
	for ( node = todoElement->FirstChild( "Field" );
		node;
		node = node->NextSibling( "Field" ) )
	{
		itemElement=node->ToElement();
		ST_GeNodeInfo ch;
		pAttr = itemElement->Attribute("id");
		if (NULL==pAttr)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中节点配置id有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		ch.id=atoi(pAttr);
		pAttr = itemElement->Attribute("name");
		if (NULL==pAttr)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中节点配置name有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		sprintf_s(ch.name,sizeof(ch.name),"%s",pAttr);
		pAttr = itemElement->Attribute("runmethod");
		if (NULL==pAttr)
		{

			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中节点配置runmethod有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		ch.runmethod=atoi(pAttr);
		pAttr = itemElement->Attribute("type");
		if (NULL==pAttr)
		{

			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中节点配置type有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		ch.type=atoi(pAttr);
		pAttr = itemElement->Attribute("defaultmaster");
		if (NULL==pAttr)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中节点配置defaultmaster有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		ch.defaultmaster=atoi(pAttr);
		pAttr = itemElement->Attribute("currentnode");
		if (NULL==pAttr)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中节点配置currentnode有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		ch.currentnode=atoi(pAttr);
		pAttr = itemElement->Attribute("status");
		if (NULL==pAttr)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中节点配置status有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		ch.status=atoi(pAttr);
		pAttr = itemElement->Attribute("error");
		if (NULL==pAttr)
		{

			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中节点配置error有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		ch.error=atoi(pAttr);
		pAttr = itemElement->Attribute("timestamp");
		if (NULL==pAttr)
		{
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"%s文件中节点配置timestamp有问题，请仔细检查",m_PathBase);
			Q_commonUtility::outError(m_log->getFile(),m_LastMsg);
			return false;
		}
		ch.timestamp=atol(pAttr);
		m_vNodeInfo.push_back(ch);
	}
	return true;
}
char *Q_GSetting::getContentID(const int &v_idx,const int &v_grp)
{
	for (UINT i=0;i<m_vContent.size();i++)
	{
		if (m_vContent[i].id==v_idx&&m_vContent[i].grpid==v_grp)
		{
			return m_vContent[i].value;
		}
	}
	return NULL;
}
bool Q_GSetting::addChannel(Q_channelBase* pChannel)
{
	if(m_nChannelCount >= Q_FRAME_CONST32)
		return false;
	m_arrayChannel[m_nChannelCount] = pChannel;
	m_nChannelCount += 1;
	return true;
}
Q_channelBase* Q_GSetting::getChannel(int i)
{
	return m_arrayChannel[i]; 
}
int Q_GSetting::getChannelCount()
{
	return m_nChannelCount;
}
bool Q_GSetting::addServerUnit(Q_tcpServerUnitInfo* pUnit)
{
	if(m_nServerUnitCount >= Q_FRAME_CONST8)
		return false;
	m_arrayServerUnit[m_nServerUnitCount] = pUnit;
	m_nServerUnitCount += 1;
	return true;
}
int  Q_GSetting::getServerUnitCount()
{
	return m_nServerUnitCount;
}
Q_tcpServerUnitInfo*   Q_GSetting::getServerUnit(int i)
{
   return m_arrayServerUnit[i]; 
}
void Q_GSetting::setServerUnitCount(int v_count)
{
  m_nServerUnitCount=v_count;
}
void Q_GSetting::setChannelCount(int v_count)
{
	m_nChannelCount=v_count;
}
bool Q_GSetting::addResChannel(Q_channelBase* pCh)
{
	if(m_nChannelCountRes >= Q_FRAME_CONST2)
		return false;
	m_arrayChannelRes[m_nChannelCountRes] = pCh;
	m_nChannelCountRes += 1;
	return true;

}
int Q_GSetting::getChannelCountRes()
{
	return m_nChannelCountRes;

}
Q_channelBase*  Q_GSetting::getResChannel(int i)
{
 return m_arrayChannelRes[i]; 
}
void Q_GSetting::setChannelCountRes(int v_count)
{
 m_nChannelCountRes=v_count;
}
int Q_GSetting::getCurMasterYCID()
{
	int YCID=-1;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(11,1);
		if(tmp!=NULL)
		{
			YCID = atoi(tmp);
		}
	}
	return YCID;
}
bool Q_GSetting::getLogManager()
{
	bool flg=false;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(9,1);
		if(tmp!=NULL)
		{
			if(strcmp(tmp,"true")==0)
			{
				flg=true;
			}
			else
			{
				flg=false;
			}
		}

	}
	return flg;
}
bool Q_GSetting::getDebugSerivce()
{
	bool flg=false;
	if(m_vContent.size()>0)
	{
		char *tmp=getContentID(9,1);
		if(tmp!=NULL)
		{
			if(strcmp(tmp,"true")==0)
			{
				flg=true;
			}
			else
			{
				flg=false;
			}
		}

	}
	return flg;
}
void Q_GSetting::setLog(Log *v_log)
{
	m_log=v_log;
}
void Q_GSetting::setDataMgr(Q_dataManager *mgr)
{
     m_pMgr=mgr;
}
void Q_GSetting::GetCommLinkTable(CDBTable &v_CommLink)
{
	if(m_pMgr!=NULL)
	{

		if(m_pMgr->getRtdbInf()!=NULL)
		{
			CScadaDataBase *scada=m_pMgr->getRtdbInf()->getScadaDb();
			if(scada!=NULL)
			scada->GetDBTableFromID(v_CommLink,SCADA_TAB_Channel_ID);
		}
		
	}

}

bool Q_GSetting::initChannelFromRtdb()
{
	//把通讯链路表、规约表等数据组合成按m_vChannel用到的数据
	CDBTable tblChannel;
	GetCommLinkTable(tblChannel);

	int beg_id=0,end_id=0;//遥测用到的
	int beg_yx_id=0,end_yx_id=0;
	for(int i=0;i<tblChannel.GetRecordCount();i++)
	{
		beg_id=0;
		beg_yx_id=0;
		SCADA_Channel item;
		m_pMgr->getRtdbInf()->getScadaDb()->GetElementByIndex(SCADA_TAB_Channel_ID,i,&item);			
		
		ST_GeCtrlChl ch;
		ch.id=item.ID32;
		sprintf_s(ch.name,sizeof(ch.name),"%s",item.strName);

		ch.enabled=item.iFlag==0?false:true;
		

		sprintf_s(ch.cfgfile,sizeof(ch.cfgfile),"%s",item.strProtocolPara);
		//去规约表查询
		

		SCADA_Protocol o_item;
		memset(&o_item,0,sizeof(SCADA_Protocol));

		getProtoInfoByTypeID(item.uProtocolID,o_item);
		sprintf_s(ch.protocol,sizeof(ch.protocol),"%s",o_item.strDLLName);
		sprintf_s(ch.classname,sizeof(ch.classname),"%s",o_item.strClassName);//樊硕修改20150723
		ch.nchanneltype = o_item.iProtocolType;
		ch.protocolSubType=o_item.iProtocolSubType;//规约子类型
		//ch.type=o_item.iProtocolType;
		ch.type = item.iType;

		sprintf_s(ch.link,sizeof(ch.link),"%s",item.strLinkParam);
		ch.chStatusid = item.iStatusID;
        

		//樊硕修改20150727
		SCADA_Analog curAnalog;
		SCADA_Digit curDigit;
		SCADA_Relay curRelay;
		SCADA_SetPoint curSetPoint;

		CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
		CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
		
		int nAnalogRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_Analog_ID );
		int last_uTuID=0;
		for (int j=0; j<nAnalogRecordCount; j++)
		{
			int nTempid32 = j+1;
			dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID, nTempid32,&curAnalog);

		//	printf(" id =%d,uTuID=%d,ch_id=%d,curcount=%d\n",nTempid32,curAnalog.uTuID,ch.id,nAnalogRecordCount);
			if (ch.id==curAnalog.uTuID)
			{
				if(beg_id==0)
				{
                   beg_id=curAnalog.ID32;
				   ch.YCbegin=beg_id;
				   ch.YCend=beg_id;
				   last_uTuID=curAnalog.uTuID;
				}
				else
				{
                  end_id=curAnalog.ID32;
				}	
			}
			if(beg_id!=0)
			{
				if((last_uTuID!=0&&last_uTuID!=curAnalog.uTuID)||
					curAnalog.ID32==nAnalogRecordCount)
				{
					ch.YCend=end_id;
					printf("ch[%d] YcBegin=%d,ycEnd=%d---------\n",ch.id,beg_id,end_id);
					break;
				}
			}
			
		}


		last_uTuID=0;
		int nDigitRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_Digit_ID );
		for (int j=0; j<nDigitRecordCount; j++)
		{
			int nTempid32 = j+1;
			dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID, nTempid32,&curDigit);

			if (ch.id==curDigit.uTuID)
			{
				if(beg_yx_id==0)
				{
					beg_yx_id=curDigit.ID32;
					ch.YXbegin=beg_yx_id;
					ch.YXend=beg_yx_id;
					last_uTuID=curDigit.uTuID;
				}
				else
				{
					end_yx_id=curDigit.ID32;
				}	
			}
			if(beg_yx_id!=0)
			{			
				if((last_uTuID!=0&&last_uTuID!=curDigit.uTuID)||
					curDigit.ID32==nDigitRecordCount)
				{
					ch.YXend=end_yx_id;
					printf("ch[%d] YxBegin=%d,yxEnd=%d---------\n",ch.id,beg_yx_id,end_yx_id);
					break;
				}
			}		
		}

		int beg_yk_id=0,end_yk_id=0;
		last_uTuID=0;
		int nRelayRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_Relay_ID );
		for (int j=0; j<nRelayRecordCount; j++)
		{
			int nTempid32 = j+1;
			dbmg_sdb->GetElementByID(SCADA_TAB_Relay_ID, nTempid32,&curRelay);

			if (ch.id==curRelay.uTuID)
			{
				if(beg_yk_id==0)
				{
					beg_yk_id=curRelay.ID32;
					ch.YKbegin=beg_yk_id;
					ch.YKend=beg_yk_id;
					last_uTuID=curRelay.uTuID;
				}
				else
				{
					end_yk_id=curRelay.ID32;
				}
			}

			if(beg_yk_id!=0)
			{			
				if((last_uTuID!=0&&last_uTuID!=curRelay.uTuID)||
					curRelay.ID32==nRelayRecordCount)
				{
					ch.YKend=end_yk_id;
					printf("ch[%d] YkBegin=%d,ykEnd=%d---------\n",ch.id,beg_yk_id,end_yk_id);
					break;
				}
			}
		}


		int beg_yt_id=0,end_yt_id=0;
		last_uTuID=0;
		int nSetPointRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_SetPoint_ID );
		for (int j=0; j<nSetPointRecordCount; j++)
		{
			int nTempid32 = j+1;
			dbmg_sdb->GetElementByID(SCADA_TAB_SetPoint_ID, nTempid32,&curSetPoint);

			if (ch.id==curSetPoint.uTuID)
			{
				if (beg_yt_id==0)//firsttime
				{
					beg_yt_id=curSetPoint.ID32;
					ch.YTbegin=beg_yt_id;
					ch.YTend=beg_yt_id;
					last_uTuID=curSetPoint.uTuID;
				}
				else
				{
					end_yt_id=curSetPoint.ID32;
				}

				if(beg_yt_id!=0)
				{			
					if((last_uTuID!=0&&last_uTuID!=curSetPoint.uTuID)||
						curSetPoint.ID32==nSetPointRecordCount)
					{
						ch.YTend=end_yt_id;
						printf("ch[%d] YtBegin=%d,ytEnd=%d---------\n",ch.id,beg_yt_id,end_yt_id);
						break;
					}
				}
			}
		}
		m_vChannel.push_back(ch);
	}

	return true;
}
bool Q_GSetting::getProtoInfoByTypeID(int typeID,SCADA_Protocol &o_item)
{
	bool ret=false;
	if(m_pMgr!=NULL)
	{
		if(m_pMgr->getRtdbInf()!=NULL)
		{
			CDBTable v_pro;

			CScadaDataBase *scada=m_pMgr->getRtdbInf()->getScadaDb();
			if(scada!=NULL)
				scada->GetDBTableFromID(v_pro,SCADA_TAB_Protocol_ID);

			if(v_pro.GetRecordCount()>0)
			{
				 for(int i=0;i<v_pro.GetRecordCount();i++)
				 {
                  SCADA_Protocol item;
                  scada->GetElementByIndex(SCADA_TAB_Protocol_ID,i,&item);

				  if(item.ID32==typeID)
				  { 
				   memcpy(&o_item,&item,sizeof(SCADA_Protocol));
				   ret=true;
				   break;
				  }
				
				 }
			}
		}
	}

		
 return ret;
}