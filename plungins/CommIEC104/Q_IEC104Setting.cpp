#include "stdafx.h"
#include "Q_IEC104Setting.h"

Q_104SyncClockCfg::Q_104SyncClockCfg()
{
	m_bExecCS=false;
}
Q_IEC104Setting::Q_IEC104Setting(void)
{
	memset(LastError,0,sizeof(LastError));
	m_104TagManager=NULL;
	m_wnd=NULL;
	m_nMsgID=0;
	m_nDebugToolMsgID=0;
}
Q_IEC104Setting::Q_IEC104Setting(Q_IEC104Setting *other)
{
	//------by lx on 2015-6-9----------------
	//strcpy(m_sFilePath,other->m_sFilePath);
	//-------原始----------------------------
	sprintf_s(m_sFilePath,sizeof(m_sFilePath),other->m_sFilePath);
	//-------end lx--------------------------
	memset(LastError,0,sizeof(LastError));
	m_vContent.clear();
	m_vGroup.clear();

	for(vector<ST_104CfgT>::iterator ia=other->m_vContent.begin();ia!=other->m_vContent.end();ia++)
	{
		m_vContent.push_back(*ia);
	}

	for(vector<ST_104CfgT>::iterator ia=other->m_vGroup.begin();ia!=other->m_vGroup.end();ia++)
	{
		m_vGroup.push_back(*ia);
	}
	m_104TagManager=new Q_104StandardTags(other->m_104TagManager);
	m_wnd=other->m_wnd;
	m_nMsgID=other->m_nMsgID;
	m_nDebugToolMsgID = other->m_nDebugToolMsgID;
	m_channel_id=other->m_channel_id;
	m_channel_type= other->m_channel_type;
	//fs20150820addupchannel
}
void Q_IEC104Setting::EvalData(Q_IEC104Setting *other)
{
	//------by lx on 2015-6-10----------------
	//strcpy(other->m_sFilePath,m_sFilePath);
	//-------原始----------------------------
	sprintf_s(other->m_sFilePath,sizeof(other->m_sFilePath),m_sFilePath);
	//-------end lx--------------------------
	memset(other->LastError,0,sizeof(other->LastError));

	for(vector<ST_104CfgT>::iterator ia=this->m_vContent.begin();ia!=this->m_vContent.end();ia++)
	{
		other->m_vContent.push_back(*ia);
	}

	for(vector<ST_104CfgT>::iterator ia=this->m_vGroup.begin();ia!=this->m_vGroup.end();ia++)
	{
		other->m_vGroup.push_back(*ia);
	}
	other->m_104TagManager=new Q_104StandardTags(this->m_104TagManager);
	other->m_wnd=this->m_wnd;
	other->m_nMsgID=this->m_nMsgID;
	other->m_nCmdSignalID=this->m_nCmdSignalID;
	other->m_channel_id=this->m_channel_id;

}
Q_IEC104Setting::~Q_IEC104Setting(void)
{
	m_vGroup.clear();
	m_vContent.clear();
	if(m_104TagManager!=NULL)
	{
		delete m_104TagManager;
		m_104TagManager=NULL;
	}

}
void Q_IEC104Setting::setFilePath(char *v_path)
{
	//------by lx on 2015-6-10----------------
	//strcpy(m_sFilePath,v_path);
	//-------原始----------------------------
	sprintf_s(m_sFilePath,sizeof(m_sFilePath),v_path);
	//-------end lx--------------------------
}
void Q_IEC104Setting::SetWndInfo(HWND wnd,int vMsgID)
{
	m_wnd = wnd;
	m_nMsgID=vMsgID;
}
bool Q_IEC104Setting::initSetting()
{
	return init();
  
}
bool Q_IEC104Setting::reloadLog()
{
	TiXmlDocument doc(m_sFilePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
#ifdef _DEBUG
		sprintf_s(LastError,sizeof(LastError),"解析配置文件出错,当前行号:%d\n",__LINE__);
		TRACE(LastError);
#endif
		return false;
	}

	TiXmlNode* node = 0;
	TiXmlNode* root = 0;
	TiXmlNode* nodeItem = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	root = doc.FirstChild( "Setting" );
	todoElement = root->ToElement();

	assert( todoElement  );

	for ( node = todoElement->FirstChild( "Record" );
		node;
		node = node->NextSibling( "Record" ) )
	{
		itemElement=node->ToElement();


		int groupid=atoi(itemElement->Attribute("id"));
		if(groupid==10)
		{

			for (nodeItem=node->FirstChild("Field");nodeItem;
				nodeItem=nodeItem->NextSibling("Field"))
			{
				itemElement=nodeItem->ToElement();


				int id=atoi(itemElement->Attribute("id"));
				
				for (UINT i=0;i<m_vContent.size();i++)
				{
					if (m_vContent[i].group==groupid &&m_vContent[i].id==id)
					{
						//------by lx on 2015-6-10----------------
						//sprintf(m_vContent[i].value,"%s",itemElement->Attribute("value"));
						//-------原始----------------------------
						sprintf_s(m_vContent[i].value,sizeof(m_vContent[i].value),"%s",itemElement->Attribute("value"));
						//-------end lx--------------------------
					}
				}

				
			}
		}
	}

	return true;
}
bool Q_IEC104Setting::init()
{
	TiXmlDocument doc(m_sFilePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
#ifdef _DEBUG
		sprintf_s(LastError,sizeof(LastError), "解析配置文件出错,当前行号:%d\n",__LINE__);
		TRACE(LastError);
#endif
		return false;
	}
    
	TiXmlNode* node = 0;
	TiXmlNode* root = 0;
	TiXmlNode* nodeItem = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	root = doc.FirstChild( "Setting" );
	todoElement = root->ToElement();

	assert( todoElement  );

	for ( node = todoElement->FirstChild( "Record" );
		node;
		node = node->NextSibling( "Record" ) )
	{
		itemElement=node->ToElement();

		ST_104CfgT group;

		group.id=atoi(itemElement->Attribute("id"));
		//------by lx on 2015-6-10----------------
		//sprintf(group.name,sizeof(group.name),"%s",itemElement->Attribute("name"));
		//-------原始----------------------------
		sprintf_s(group.name,sizeof(group.name),"%s",itemElement->Attribute("name"));
		//-------end lx--------------------------
		
		group.type=eXmlDataTypeString;

		m_vGroup.push_back(group);

		for (nodeItem=node->FirstChild("Field");nodeItem;
			nodeItem=nodeItem->NextSibling("Field"))
		{
			itemElement=nodeItem->ToElement();
			ST_104CfgT content;
			content.group=group.id;

			content.id=atoi(itemElement->Attribute("id"));
			//------by lx on 2015-6-10----------------
			sprintf_s(content.name,sizeof(content.name),"%s",itemElement->Attribute("name"));
			sprintf_s(content.value,sizeof(content.value),"%s",itemElement->Attribute("value"));
			//------原始------------------------------
			//sprintf_s(content.name,"%s",itemElement->Attribute("name"));
			//sprintf_s(content.value,"%s",itemElement->Attribute("value"));
			//------end lx-----------------------------

			const char *type=itemElement->Attribute("type");
			if (strcmp(type,"STRING")==0)
			{
				content.type=eXmlDataTypeString;

			}
			else if (strcmp(type,"ENUM")==0)
			{
				content.type=eXmlDataTypeEnum;
			}
			else if (strcmp(type,"INT")==0)
			{
				content.type=eXmlDataTypeInt;
			}
			else if (strcmp(type,"FLOAT")==0)
			{
				content.type=eXmlDataTypeFloat;
			}
			m_vContent.push_back(content);

		}
	}

	return true;
}

Q_104BaseCfg *Q_IEC104Setting::getBaseSetting(Q_104BaseCfg *v_cfg)
{
	
	TRACE("Q_IEC104Setting::getBaseSetting begin\n");
   if (v_cfg!=NULL)
    {
        char *tmp=NULL;

		tmp=getContentID(2);//本端IP
		if(tmp!=NULL)
		//strcpy_s(v_cfg->localServer.ip,sizeof(v_cfg->localServer.ip),tmp);
		sprintf_s(v_cfg->localServer.ip,sizeof(v_cfg->localServer.ip),tmp);
		tmp=getContentID(3);//本端端口号
		if(tmp!=NULL)
		v_cfg->localServer.port=atoi(tmp);

		v_cfg->localServer.valid=true;
		TRACE("Q_IEC104Setting::getBaseSetting 1\n");
        for(int i=0;i<MAX_REMOTE_NUM;i++)
		{
            tmp=getContentID(4+i*2);//本端IP
			if(tmp!=NULL)
			{
				if(strcmp(tmp,"")!=0)
				{
					//strcpy_s(v_cfg->remServer[i].ip,sizeof(v_cfg->localServer.ip),tmp);
					sprintf_s(v_cfg->remServer[i].ip,sizeof(v_cfg->localServer.ip),tmp);
					TRACE("Q_IEC104Setting::getBaseSetting client filter=%s\n",v_cfg->remServer[i].ip);
					v_cfg->remServer[i].valid=true;

					tmp=getContentID(5+i*2);//本端端口号
					if(tmp!=NULL)
					{
						if(strcmp(tmp,"")!=0)
						{
							v_cfg->remServer[i].port=atoi(tmp);
						}
						else
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
			
		}
 TRACE("Q_IEC104Setting::getBaseSetting 2\n");
       tmp=getContentID(1);//站别
        if (tmp!=NULL)
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eCurStType=eStation_Master;
            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eCurStType=eStation_Slave;
            }
            else
            {
                v_cfg->m_eCurStType=eStation_Unknown;

            }
        }

		tmp=getContentID(122);
        if (NULL!=tmp)
        {
            v_cfg->m_nAsduLen=atoi(tmp);
        }

        if (NULL!=(tmp=getContentID(123)))
        {
            v_cfg->m_nAsduAddr=atoi(tmp);
			TRACE("getBaseSetting:得到公共地址为%d\n",v_cfg->m_nAsduAddr);
        }
		if (NULL!=(tmp=getContentID(124)))
        {
            v_cfg->m_nTxCauseLen=atoi(tmp);
        }
        
        if (NULL!=(tmp=getContentID(125)))
        {
            v_cfg->m_nMaxFrameLen=atoi(tmp);
        }

        if (NULL!=(tmp=getContentID(126)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eOpenAppMethord=eOpenAppMode_1;
            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eOpenAppMethord=eOpenAppMode_2;

            }
            else if (strcmp("3",tmp)==0)
            {
                v_cfg->m_eOpenAppMethord=eOpenAppMode_3;
            }
            else if (strcmp("4",tmp)==0)
            {
                v_cfg->m_eOpenAppMethord=eOpenAppMode_4;

            }
        }

		if (NULL!=(tmp=getContentID(127)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eFrameSeqVfy=eFrameSeqVfyBoth;
            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eFrameSeqVfy=eFrameSeqVfyLocTxRemRx;

            }
            else if (strcmp("3",tmp)==0)
            {
                v_cfg->m_eFrameSeqVfy=eFrameSeqVfyBothSend;
            }
            else if (strcmp("4",tmp)==0)
            {
                v_cfg->m_eFrameSeqVfy=eFrameSeqVfyNull;

            }
        }

        if (NULL!=(tmp=getContentID(128)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eAsduVfy=eAsduVfy_Total;

            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eAsduVfy=eAsduVfy_Vsq;
            }
            else if (strcmp("3",tmp)==0)
            {
                v_cfg->m_eAsduVfy=eAsduVfy_Cot;

            }
            else if (strcmp("4",tmp)==0)
            {
                v_cfg->m_eAsduVfy=eAsduVfy_Addr;
            }
            else if (strcmp("5",tmp)==0)
            {
                v_cfg->m_eAsduVfy=eAsduVfy_VsqAddr;
            }
            else if (strcmp("6",tmp)==0)
            {
                v_cfg->m_eAsduVfy=eAsduVfy_CotAddr;
            }
            else if (strcmp("7",tmp)==0)
            {
                v_cfg->m_eAsduVfy=eAsduVfy_Null;
            }
        }
        /*
        Q_104IntSeq    m_eIntSeq;
        Q_104FloatSeq  m_eFloatSeq;
        Q_104T1App     m_eT1App;
        Q_104T2App     m_eT2App;
        Q_104T3App     m_eT3App;
        Q_104IF_SendMode m_eIF_Send;
        Q_104IF_RecvMode m_eIF_Recv;

        */
        if (NULL!=(tmp=getContentID(129)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eIntSeq=eIntSeqLittle;

            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eIntSeq=eIntSeqBig;
            }
        }

        if (NULL!=(tmp=getContentID(130)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eFloatSeq=eFloatSeqLittle;

            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eFloatSeq=eFloatSeqBig;
            }
        }

        if (NULL!=(tmp=getContentID(131)))
        {
            v_cfg->m_nT0=atoi(tmp);

        }

        if (NULL!=(tmp=getContentID(132)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eT2App=eT2AppSendS;
            }
        }


        if (NULL!=(tmp=getContentID(133)))
        {
            v_cfg->m_nT1=atoi(tmp);
        }

        if (NULL!=(tmp=getContentID(134)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eT2App=eT2AppSendS;
            }
        }

        if (NULL!=(tmp=getContentID(135)))
        {
            v_cfg->m_nT2=atoi(tmp);
        }

        if (NULL!=(tmp=getContentID(136)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eT3App=eT3AppSendTest;
            }
        }

        if (NULL!=(tmp=getContentID(137)))
        {
            v_cfg->m_nT3=atoi(tmp);
        }

        if (NULL!=(tmp=getContentID(138)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eIF_Send=eIF_SendIgnoreK;
            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eIF_Send=eIF_SendLimitByK;
            }
        }

        if (NULL!=(tmp=getContentID(139)))
        {
            v_cfg->m_nK=atoi(tmp);
        }

        if (NULL!=(tmp=getContentID(140)))
        {

            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eIF_Recv=eIF_RecvBeforeW_Act;
            }
        }

        if (NULL!=(tmp=getContentID(141)))
        {
            v_cfg->m_nW=atoi(tmp);
        }

        if (NULL!=(tmp=getContentID(142)))
        {
            v_cfg->m_nFrameI_MaxCounter=atoi(tmp);
        }
		if(NULL!=(tmp=getContentID(143)))
		{
			int nFlg=atoi(tmp);
			v_cfg->m_bSendSFrame=(nFlg==0)?false:true;
		}
		if(NULL!=(tmp=getContentID(144)))
		{
			int nFlg=atoi(tmp);
			v_cfg->m_bSendStartDt=(nFlg==0)?true:false;
		}
		if(NULL!=(tmp=getContentID(145)))
		{
			int nFlg=atoi(tmp);
			v_cfg->m_bSendIC=(nFlg==0)?true:false;
		}
		if(NULL!=(tmp=getContentID(146)))
		{
			int nFlg=atoi(tmp);
			v_cfg->m_bSendAC=(nFlg==0)?false:true;
		}
		if(NULL!=(tmp=getContentID(147)))
		{
			int nFlg=atoi(tmp);
			v_cfg->m_acTime=atoi(tmp);
		}
		if(NULL!=(tmp=getContentID(150)))
		{
			int nFlg=atoi(tmp);
			v_cfg->m_bSendSOE=(nFlg==0)?false:true;
		}

		if(NULL!=(tmp=getContentID(3,21)))//山东本地AGC计划曲线数据点在遥测表的起始地址
		{
			int nFlg=atoi(tmp);
			v_cfg->m_lAgcPlCYCStart=atoi(tmp);
		}
		if(NULL!=(tmp=getContentID(2,21)))//山东本地AGC计划曲线数据点个数
		{
			int nFlg=atoi(tmp);
			v_cfg->m_lAgcPlCYCEnd=atoi(tmp) + v_cfg->m_lAgcPlCYCStart - 1;
		}
		if(NULL!=(tmp=getContentID(3,22)))//山东本地AGC计划曲线数据点在遥测表的起始地址
		{
			int nFlg=atoi(tmp);
			v_cfg->m_rAgcPlCYCStart=atoi(tmp);
		}
		if(NULL!=(tmp=getContentID(2,22)))//山东本地AGC计划曲线数据点个数
		{
			int nFlg=atoi(tmp);
			v_cfg->m_rAgcPlCYCEnd=atoi(tmp) + v_cfg->m_rAgcPlCYCStart -1;
		}
		if(NULL!=(tmp=getContentID(3,23)))//山东本地AGC计划曲线数据点在遥测表的起始地址
		{
			int nFlg=atoi(tmp);
			v_cfg->m_lAvcPlCYCStart=atoi(tmp);
		}
		if(NULL!=(tmp=getContentID(2,23)))//山东本地AGC计划曲线数据点个数
		{
			int nFlg=atoi(tmp);
			v_cfg->m_lAvcPlCYCEnd=atoi(tmp) + v_cfg->m_lAvcPlCYCStart -1;
		}
		if(NULL!=(tmp=getContentID(3,24)))//山东本地AGC计划曲线数据点在遥测表的起始地址
		{
			int nFlg=atoi(tmp);
			v_cfg->m_rAvcPlCYCStart=atoi(tmp);
		}
		if(NULL!=(tmp=getContentID(2,24)))//山东本地AGC计划曲线数据点个数
		{
			int nFlg=atoi(tmp);
			v_cfg->m_rAvcPlCYCEnd=atoi(tmp) + v_cfg->m_rAvcPlCYCStart - 1;
		}
		////
		if(NULL!=(tmp=getContentID(1,21)))//山东本地AGC计划曲线文件目录
		{
			memset(v_cfg->m_lAgcPath,0,sizeof(v_cfg->m_rAgcPath));
			strcpy(v_cfg->m_lAgcPath,tmp);
		}
		if(NULL!=(tmp=getContentID(1,22)))//山东远方AGC计划曲线文件目录
		{
			memset(v_cfg->m_rAgcPath,0,sizeof(v_cfg->m_rAgcPath));
			strcpy(v_cfg->m_rAgcPath,tmp);
		}
		if(NULL!=(tmp=getContentID(1,23)))//山东本地AVC计划曲线文件目录
		{
			memset(v_cfg->m_lAvcPath,0,sizeof(v_cfg->m_rAgcPath));
			strcpy(v_cfg->m_lAvcPath,tmp);
		}
		if(NULL!=(tmp=getContentID(1,24)))//山东远方AVC计划曲线文件目录
		{
			memset(v_cfg->m_rAvcPath,0,sizeof(v_cfg->m_rAgcPath));
			strcpy(v_cfg->m_rAvcPath,tmp);
		}
		//printf("lagcs:%dlagcend:%dragcs:%dragcend:%dlavcs:%dlavcend:%dragcs:%ddragcend:%d");

    }
		TRACE("Q_IEC104Setting::getBaseSetting end\n");
    return v_cfg;
}


int Q_IEC104Setting::getZTChlYCBegin()//fs20150820
{
	int res=0;
	char *tmp=getContentID(10,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}

int Q_IEC104Setting::getZTChlYXBegin()//fs20150820
{
	int res=0;
	char *tmp=getContentID(9,2);
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res;
}


bool Q_IEC104Setting::getIsEnableACSend()
{
	int res=0;
	char *tmp=getContentID(146);//樊硕50,2
	if(tmp!=NULL)
	{
		res=atoi(tmp);
	}
	return res==1?true:false;
}


bool Q_IEC104Setting::getPrintSetting(Q_LogType type)
{
	char *tmp=NULL;

	bool ret=false;

	switch(type)
	{
	case eComLogTypeTotal:
		tmp=getContentID(1,10);//本端IP
		if(tmp!=NULL)
		{
			ret=atoi(tmp)==0?false:true;
		}
		break;
	case eComLogTypeExplain:
		tmp=getContentID(2,10);//本端IP
		if(tmp!=NULL)
		{
			ret=atoi(tmp)==0?false:true;
		}
		break;
	case eComLogTypePrompt:
		tmp=getContentID(3,10);//本端IP
		if(tmp!=NULL)
		{
			ret=atoi(tmp)==0?false:true;
		}

		break;
	case eComLogTypeError:
		tmp=getContentID(4,10);//本端IP
		if(tmp!=NULL)
		{
			ret=atoi(tmp)==0?false:true;
		}
		break;
	}
	return ret;
}

Q_104StCfg   *Q_IEC104Setting::getStationSetting(Q_104StCfg *v_cfg)
{
	if (v_cfg!=NULL)
	{
		char *tmp=NULL;
		int n=0;

		tmp=getContentID(2,2);//总召唤遥信每帧封装遥信个数
		if(tmp!=NULL)
		{
			n=atoi(tmp);
			switch(n)
			{
			case 1:
				v_cfg->m_AsdnYcType=eAsduYcType9;
				break;
			case 2:
				v_cfg->m_AsdnYcType=eAsduYcType11;
				break;
			case 3:
				v_cfg->m_AsdnYcType=eAsduYcType13;
				break;
			case 4:
				v_cfg->m_AsdnYcType=eAsduYcType21;
				break;
			default:
				v_cfg->m_AsdnYcType=eAsduYcType13;
				break;
			}
		}
		

		tmp=getContentID(5,2);//总召唤遥信每帧封装遥信个数
		if(tmp!=NULL)
		{
			v_cfg->n_YxNumPerFrame=atoi(tmp);
		}
		tmp=getContentID(6,2);//总召唤遥测每帧封装遥测个数
		if(tmp!=NULL)
		{
			v_cfg->n_YcNumPerFrame=atoi(tmp);
		}
		tmp=getContentID(7,2);//总召唤遥步每帧封装遥步个数
		if(tmp!=NULL)
		{
			v_cfg->n_YbNumPerFrame=atoi(tmp);
		}
		tmp=getContentID(8,2);//总召唤遥信每帧封装遥测个数
		if(tmp!=NULL)
		{
			v_cfg->n_IcInterval=atoi(tmp);
		}
		tmp=getContentID(9,2);
		if(tmp!=NULL)
		{
			v_cfg->m_YxBeginIndex=atoi(tmp);
		}
		tmp=getContentID(10,2);
		if(tmp!=NULL)
		{
			v_cfg->m_YcBeginIndex=atoi(tmp);
		}
	}
    return v_cfg;
}

Q_104CtrlCfg *Q_IEC104Setting::getControlSetting(Q_104CtrlCfg *v_cfg)
{
	if (v_cfg!=NULL)
	{
		char *tmp=NULL;
		int n=0;

		tmp=getContentID(41,3);//遥调模式
		if(tmp!=NULL)
		{
			n=atoi(tmp);
			if(n==1)
			{
				v_cfg->m_YtExecMode=eYtExecModeSelect;
			}
			else if(n==2)
			{
				v_cfg->m_YtExecMode=eYtExecModeDirect;
			}
			else
			{
				v_cfg->m_YtExecMode=eYtExecModeNull;
			}
		}

		tmp=getContentID(49,3);//遥调模式
		if(tmp!=NULL)
		{
			n=atoi(tmp);
			if(n==1)
			{
				v_cfg->m_YsetAsduType=eYsetAsdu30;
			}
			else if(n==2)
			{
				v_cfg->m_YsetAsduType=eYsetAsdu31;
			}
			else if(n==3)
			{
				v_cfg->m_YsetAsduType=eYsetAsdu32;
			}
			else
			{
				v_cfg->m_YsetAsduType=eYsetAsdu3F;//0x3F
			}
		}
		else
		{
			v_cfg->m_YsetAsduType=eYsetAsdu30;
		}
        tmp=getContentID(64,3);//规一化值量程
		if(tmp!=NULL)
		{
         v_cfg->getAsdu48Mode=atoi(tmp);
		}
		else
		{
          v_cfg->getAsdu48Mode=0;
		}
		
		tmp=getContentID(1,3);//遥控模式
		if(tmp!=NULL)
		{
			n=atoi(tmp);
			if(n==1)
			{
				v_cfg->m_YkExecMode=eYtExecModeSelect;
			}
			else if(n==2)
			{
				v_cfg->m_YkExecMode=eYtExecModeDirect;
			}
			else
			{
				v_cfg->m_YkExecMode=eYtExecModeNull;
			}
		}

		tmp = getContentID(2,3);
		if(tmp!=NULL)
		{
			n=atoi(tmp);
			if(n==1)
			{
				v_cfg->m_YKAsduType=eYKAsdu2D;
			}
			else if(n==2)
			{
				v_cfg->m_YKAsduType=eYKAsdu3A;
			}
		}
		else
		{
			v_cfg->m_YKAsduType=eYKAsdu2D;
		}
		
	}
    return v_cfg;
}
Q_104SyncClockCfg *Q_IEC104Setting::getSyncClockSetting(Q_104SyncClockCfg *v_cfg)
{
    char *tmp=NULL;
    if (v_cfg!=NULL)
    {
        tmp=getContentID(1,4);
        if (tmp!=NULL)
        {
            if (strcmp(tmp,"1")==0)
            {
                v_cfg->m_eActMethord=eCS_Act_Master;
            }
            else if (strcmp(tmp,"2")==0)
            {
                v_cfg->m_eActMethord=eCS_Act_MasterFix;
            }
            else if (strcmp(tmp,"3")==0)
            {
                v_cfg->m_eActMethord=eCS_Act_OwnSysBf;
            }
            else if (strcmp(tmp,"4")==0)
            {
                v_cfg->m_eActMethord=eCS_Act_OwnSysAf;
            }
        }

        tmp=getContentID(2,4);
        if (tmp!=NULL)
            v_cfg->m_nFixUsecs=atoi(tmp);

		//是否执行对时
		tmp=getContentID(3,4);
		if (tmp!=NULL)
		{
			v_cfg->m_bExecCS=atoi(tmp)==1?true:false;
		}

    }
    return v_cfg;
}
void Q_IEC104Setting::setChannelClass(const char *className)
{
	memset(m_className,0,sizeof(m_className));
	strcpy(m_className,className);
}
const char* Q_IEC104Setting::getChannelClass()
{
	return m_className;
}
char *Q_IEC104Setting::getContentID(const int &v_idx,const int &v_grp)
{
    for (UINT i=0;i<m_vContent.size();i++)
    {
		
        if (m_vContent[i].group==v_grp && m_vContent[i].id==v_idx)
        {
            return m_vContent[i].value;
        }
    }
    return NULL;
}
void Q_IEC104Setting::setClientAddr(char *v_addr)
{
	if(v_addr!=NULL)
	memcpy(m_strClientAddr,v_addr,sizeof(m_strClientAddr));
}
char *Q_IEC104Setting::getClientAddr()
{
	return m_strClientAddr;
}