#include "gSystemDef.h"
#include "Q_IEC103Setting.h"
#include "tinyxmlinclude.h"
Q_IEC103Setting::Q_IEC103Setting(void)
{
	memset(LastError,NULL,sizeof(LastError));
	m_103TagManager=NULL;
	m_wnd=NULL;
	m_nMsgID=0;
	
}
Q_IEC103Setting::Q_IEC103Setting(Q_IEC103Setting *other)
{
	strcpy(m_sFilePath,other->m_sFilePath);
	memset(LastError,NULL,sizeof(LastError));
	m_vContent.clear();
	m_vGroup.clear();

	for(vector<ST_103CfgT>::iterator ia=other->m_vContent.begin();ia!=other->m_vContent.end();ia++)
	{
		m_vContent.push_back(*ia);
	}

	for(vector<ST_103CfgT>::iterator ia=other->m_vGroup.begin();ia!=other->m_vGroup.end();ia++)
	{
		m_vGroup.push_back(*ia);
	}
	m_103TagManager=new Q_103StandardTags(other->m_103TagManager);
	m_wnd=other->m_wnd;
	m_nMsgID=other->m_nMsgID;
	m_channel_id=other->m_channel_id;
}
void Q_IEC103Setting::EvalData(Q_IEC103Setting *other)
{
	strcpy(other->m_sFilePath,m_sFilePath);
	memset(other->LastError,NULL,sizeof(other->LastError));

	for(vector<ST_103CfgT>::iterator ia=this->m_vContent.begin();ia!=this->m_vContent.end();ia++)
	{
		other->m_vContent.push_back(*ia);
	}

	for(vector<ST_103CfgT>::iterator ia=this->m_vGroup.begin();ia!=this->m_vGroup.end();ia++)
	{
		other->m_vGroup.push_back(*ia);
	}
	other->m_103TagManager=new Q_103StandardTags(this->m_103TagManager);
	other->m_wnd=this->m_wnd;
	other->m_nMsgID=this->m_nMsgID;
	other->m_nCmdSignalID=this->m_nCmdSignalID;
	other->m_channel_id=this->m_channel_id;

}
Q_IEC103Setting::~Q_IEC103Setting(void)
{
	m_vGroup.clear();
	m_vContent.clear();
	if(m_103TagManager!=NULL)
	{
		delete m_103TagManager;
		m_103TagManager=NULL;
	}

}
void Q_IEC103Setting::setFilePath(char *v_path)
{
    strcpy(m_sFilePath,v_path);
}
void Q_IEC103Setting::SetWndInfo(HWND wnd,int vMsgID)
{
	m_wnd = wnd;
	m_nMsgID=vMsgID;
}
bool Q_IEC103Setting::initSetting()
{
	return init();
  
}
bool Q_IEC103Setting::reloadLog()
{
	TiXmlDocument doc(m_sFilePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
#ifdef _DEBUG
		sprintf_s(LastError, "解析配置文件出错,当前行号:%d\n",__LINE__);
		printf(LastError);
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
					if (m_vContent[i].group==groupid &&
											 m_vContent[i].id==id)
					{
						//sprintf_s(m_vContent[i].value,"%s",itemElement->Attribute("value"));
						sprintf_s(m_vContent[i].value,sizeof(m_vContent[i].value),"%s",itemElement->Attribute("value"));
					}
				}

				
			}
		}
	}

	return true;
}
bool Q_IEC103Setting::init()
{
	TiXmlDocument doc(m_sFilePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)
	{
#ifdef _DEBUG
		sprintf_s(LastError, "解析配置文件出错,当前行号:%d\n",__LINE__);
		printf(LastError);
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

		ST_103CfgT group;

		group.id=atoi(itemElement->Attribute("id"));
		//sprintf_s(group.name,"%s",itemElement->Attribute("name"));
		sprintf_s(group.name,sizeof(group.name),"%s",itemElement->Attribute("name"));
		group.type=e103XmlDataTypeString;

		m_vGroup.push_back(group);

		for (nodeItem=node->FirstChild("Field");nodeItem;
			nodeItem=nodeItem->NextSibling("Field"))
		{
			itemElement=nodeItem->ToElement();
			ST_103CfgT content;
			content.group=group.id;

			content.id=atoi(itemElement->Attribute("id"));
			//sprintf_s(content.name,"%s",itemElement->Attribute("name"));
			//sprintf_s(content.value,"%s",itemElement->Attribute("value"));
			sprintf_s(content.name,sizeof(content.name),"%s",itemElement->Attribute("name"));
			sprintf_s(content.value,sizeof(content.value),"%s",itemElement->Attribute("value"));

			const char *type=itemElement->Attribute("type");
			if (strcmp(type,"STRING")==0)
			{
				content.type=e103XmlDataTypeString;

			}
			else if (strcmp(type,"ENUM")==0)
			{
				content.type=e103XmlDataTypeEnum;
			}
			else if (strcmp(type,"INT")==0)
			{
				content.type=e103XmlDataTypeInt;
			}
			else if (strcmp(type,"FLOAT")==0)
			{
				content.type=e103XmlDataTypeFloat;
			}
			m_vContent.push_back(content);

		}
	}

	return true;
}

Q_103BaseCfg *Q_IEC103Setting::getBaseSettingEx(Q_103BaseCfg *v_cfg)
{
	    if (v_cfg!=NULL)
    {
        char *tmp=NULL;

		tmp=getContentID(2);//本端IP
		if(tmp!=NULL)
		strcpy(v_cfg->localServer.ip,tmp);

		tmp=getContentID(3);//本端端口号
		if(tmp!=NULL)
		v_cfg->localServer.port=atoi(tmp);

		v_cfg->localServer.valid=true;


		tmp=getContentID(4);//对端IP，只有这个IP链接进来才有效
		if(tmp!=NULL)
		{
			if(strcmp(tmp,"")!=0)
			{
				strcpy(v_cfg->remServer[0].ip,tmp);
				v_cfg->remServer[0].valid=true;

				tmp=getContentID(5);//对端端口号，没用

				v_cfg->remServer[0].port=atoi(tmp);
			}
		}
 
        tmp=getContentID(1);//站别
        if (tmp!=NULL)
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eCurStType=e103Station_Master;
            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eCurStType=e103Station_Slave;
            }
            else
            {
                v_cfg->m_eCurStType=e103Station_Unknown;

            }
        }
 		tmp=getContentID(21);//南自103规约地址1
 		if (NULL!=tmp)
 		{
			//v_cfg->m_nStationAddr1=atoi(tmp);
			int n=atoi(tmp);
			if(n>0)
			{
 			v_cfg->m_vecStationAddr.push_back(atoi(tmp));
			}
 		}
// 		tmp=getContentID(22);//南自103规约地址2
// 		if (NULL!=tmp)
// 		{
// 			v_cfg->m_nStationAddr2=atoi(tmp);
// 		}
		//因为一个逆变器房有两台逆变器所以设备地址不一样
		tmp=getContentID(21); 
		if (NULL!=tmp)
		{
			char  strNum[40];
			memset(strNum,0,sizeof(strNum));

			bool bNumber = false;
			for (UINT i=0;i<strlen(tmp);i++)
			{
				if (tmp[i]==',')
				{
					if (bNumber)
					{
						v_cfg->m_vecStationAddr.push_back(atoi(strNum));
						//TRACE("通道过滤%s\r\n",strNum);
						//printf("通道过滤%s\r\n",strNum);
						memset(strNum,0,sizeof(strNum));
						bNumber = false;
					}
				}
				else
				{
					if(tmp[i]>='0' && tmp[i]<='9')
					{
						bNumber = true;
						sprintf(strNum,"%s%c",strNum,tmp[i]);
					}
				}
				//最后一个
				if (i==strlen(tmp)-1)
				{
					if (bNumber)
					{
						v_cfg->m_vecStationAddr.push_back(atoi(strNum));
						//TRACE("通道过滤%s\r\n",strNum);
						//printf("通道过滤%s\r\n",strNum);
						memset(strNum,0,sizeof(strNum));
						bNumber = false;
					}
				}
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
                v_cfg->m_eOpenAppMethord=e103OpenAppMode_1;
            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eOpenAppMethord=e103OpenAppMode_2;

            }
            else if (strcmp("3",tmp)==0)
            {
                v_cfg->m_eOpenAppMethord=e103OpenAppMode_3;
            }
            else if (strcmp("4",tmp)==0)
            {
                v_cfg->m_eOpenAppMethord=e103OpenAppMode_4;

            }
        }

		if (NULL!=(tmp=getContentID(127)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eFrameSeqVfy=e103FrameSeqVfyBoth;
            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eFrameSeqVfy=e103FrameSeqVfyLocTxRemRx;

            }
            else if (strcmp("3",tmp)==0)
            {
                v_cfg->m_eFrameSeqVfy=e103FrameSeqVfyBothSend;
            }
            else if (strcmp("4",tmp)==0)
            {
                v_cfg->m_eFrameSeqVfy=e103FrameSeqVfyNull;

            }
        }

        if (NULL!=(tmp=getContentID(128)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eAsduVfy=e103AsduVfy_Total;

            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eAsduVfy=e103AsduVfy_Vsq;
            }
            else if (strcmp("3",tmp)==0)
            {
                v_cfg->m_eAsduVfy=e103AsduVfy_Cot;

            }
            else if (strcmp("4",tmp)==0)
            {
                v_cfg->m_eAsduVfy=e103AsduVfy_Addr;
            }
            else if (strcmp("5",tmp)==0)
            {
                v_cfg->m_eAsduVfy=e103AsduVfy_VsqAddr;
            }
            else if (strcmp("6",tmp)==0)
            {
                v_cfg->m_eAsduVfy=e103AsduVfy_CotAddr;
            }
            else if (strcmp("7",tmp)==0)
            {
                v_cfg->m_eAsduVfy=e103AsduVfy_Null;
            }
        }
        /*
        Q_103IntSeq    m_eIntSeq;
        Q_103FloatSeq  m_eFloatSeq;
        Q_103T1App     m_eT1App;
        Q_103T2App     m_eT2App;
        Q_103T3App     m_eT3App;
        Q_103IF_SendMode m_eIF_Send;
        Q_103IF_RecvMode m_eIF_Recv;

        */
        if (NULL!=(tmp=getContentID(129)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eIntSeq=e103IntSeqLittle;

            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eIntSeq=e103IntSeqBig;
            }
        }

        if (NULL!=(tmp=getContentID(130)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eFloatSeq=e103FloatSeqLittle;

            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eFloatSeq=e103FloatSeqLittle;
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
                v_cfg->m_eT2App=e103T2AppSendS;
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
                v_cfg->m_eT2App=e103T2AppSendS;
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
                v_cfg->m_eT3App=e103T3AppSendTest;
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
                v_cfg->m_eIF_Send=e103IF_SendIgnoreK;
            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eIF_Send=e103IF_SendLimitByK;
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
                v_cfg->m_eIF_Recv=e103IF_RecvBeforeW_Act;
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
		//tjs 2017-1-8 begin
		for(int i=0;i<MAX_103_REMOTE_NUM-MAX_103_REMOTE_NUM_BASE;i++)
		{
			tmp=getContentID(i*2+1,5);
			if(tmp!=NULL)
			{
				if(strcmp(tmp,"")!=0)
				{
					int idx=i+MAX_103_REMOTE_NUM_BASE;
					if(idx<MAX_103_REMOTE_NUM)
					{
						strcpy(v_cfg->remServer[idx].ip,tmp);
						v_cfg->remServer[idx].valid=true;

						tmp=getContentID(2+i*2,5);//本端端口号

						v_cfg->remServer[idx].port=atoi(tmp);
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
		// 2017-1-8 end

    }
    return v_cfg;
}

Q_103BaseCfg *Q_IEC103Setting::getBaseSetting(Q_103BaseCfg *v_cfg)
{
    if (v_cfg!=NULL)
    {
        char *tmp=NULL;

		tmp=getContentID(2);//本端IP
		if(tmp!=NULL)
		strcpy(v_cfg->localServer.ip,tmp);

		tmp=getContentID(3);//本端端口号
		if(tmp!=NULL)
		v_cfg->localServer.port=atoi(tmp);

		v_cfg->localServer.valid=true;

       // for(int i=0;i<MAX_103_REMOTE_NUM;i++)
		for(int i=0;i<MAX_103_REMOTE_NUM_BASE;i++)
		{
            tmp=getContentID(4+i*2);//本端IP
			if(tmp!=NULL)
			{
				if(strcmp(tmp,"")!=0)
				{
					v_cfg->remServer[i].valid=true;
					strcpy(v_cfg->remServer[i].ip,tmp);
					
					tmp=getContentID(5+i*2);//本端端口号

					v_cfg->remServer[i].port=atoi(tmp);
					//TRACE("Q_IEC103Setting::getBaseSetting client filter=%s\n",v_cfg->remServer[i].ip,v_cfg->remServer[i].port);
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
 
        tmp=getContentID(1);//站别
        if (tmp!=NULL)
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eCurStType=e103Station_Master;
            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eCurStType=e103Station_Slave;
            }
            else
            {
                v_cfg->m_eCurStType=e103Station_Unknown;

            }
        }
 		tmp=getContentID(21);//南自103规约地址1
 		if (NULL!=tmp)
 		{
 			//v_cfg->m_nStationAddr1=atoi(tmp);
			int n=atoi(tmp);
			if(n>0)
			{
			v_cfg->m_vecStationAddr.push_back(atoi(tmp));
			}
 		}
// 		tmp=getContentID(22);//南自103规约地址2
// 		if (NULL!=tmp)
// 		{
// 			v_cfg->m_nStationAddr2=atoi(tmp);
// 		}
		
		//因为一个逆变器房有两台逆变器所以设备地址不一样
		//tjs 2018
		tmp=getContentID(21); 
		if (NULL!=tmp)
		{
			char  strNum[40];
			memset(strNum,0,sizeof(strNum));

			bool bNumber = false;
			for (UINT i=0;i<strlen(tmp);i++)
			{
				if (tmp[i]==',')
				{
					if (bNumber)
					{
						v_cfg->m_vecStationAddr.push_back(atoi(strNum));
						//TRACE("通道过滤%s\r\n",strNum);
						//printf("通道过滤%s\r\n",strNum);
						memset(strNum,0,sizeof(strNum));
						bNumber = false;
					}
				}
				else
				{
					if(tmp[i]>='0' && tmp[i]<='9')
					{
						bNumber = true;
						sprintf(strNum,"%s%c",strNum,tmp[i]);
					}
				}
				//最后一个
				if (i==strlen(tmp)-1)
				{
					if (bNumber)
					{
						v_cfg->m_vecStationAddr.push_back(atoi(strNum));
						//TRACE("通道过滤%s\r\n",strNum);
						//printf("通道过滤%s\r\n",strNum);
						memset(strNum,0,sizeof(strNum));
						bNumber = false;
					}
				}
			}
		}
		//tjs 2017-1-8
		tmp=getContentID(122);
        if (NULL!=tmp)
        {
            v_cfg->m_nAsduLen=atoi(tmp);
        }
        if (NULL!=(tmp=getContentID(123)))
        {
            v_cfg->m_nAsduAddr=atoi(tmp);
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
                v_cfg->m_eOpenAppMethord=e103OpenAppMode_1;
            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eOpenAppMethord=e103OpenAppMode_2;

            }
            else if (strcmp("3",tmp)==0)
            {
                v_cfg->m_eOpenAppMethord=e103OpenAppMode_3;
            }
            else if (strcmp("4",tmp)==0)
            {
                v_cfg->m_eOpenAppMethord=e103OpenAppMode_4;

            }
        }

		if (NULL!=(tmp=getContentID(127)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eFrameSeqVfy=e103FrameSeqVfyBoth;
            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eFrameSeqVfy=e103FrameSeqVfyLocTxRemRx;

            }
            else if (strcmp("3",tmp)==0)
            {
                v_cfg->m_eFrameSeqVfy=e103FrameSeqVfyBothSend;
            }
            else if (strcmp("4",tmp)==0)
            {
                v_cfg->m_eFrameSeqVfy=e103FrameSeqVfyNull;

            }
        }

        if (NULL!=(tmp=getContentID(128)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eAsduVfy=e103AsduVfy_Total;

            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eAsduVfy=e103AsduVfy_Vsq;
            }
            else if (strcmp("3",tmp)==0)
            {
                v_cfg->m_eAsduVfy=e103AsduVfy_Cot;

            }
            else if (strcmp("4",tmp)==0)
            {
                v_cfg->m_eAsduVfy=e103AsduVfy_Addr;
            }
            else if (strcmp("5",tmp)==0)
            {
                v_cfg->m_eAsduVfy=e103AsduVfy_VsqAddr;
            }
            else if (strcmp("6",tmp)==0)
            {
                v_cfg->m_eAsduVfy=e103AsduVfy_CotAddr;
            }
            else if (strcmp("7",tmp)==0)
            {
                v_cfg->m_eAsduVfy=e103AsduVfy_Null;
            }
        }
        /*
        Q_103IntSeq    m_eIntSeq;
        Q_103FloatSeq  m_eFloatSeq;
        Q_103T1App     m_eT1App;
        Q_103T2App     m_eT2App;
        Q_103T3App     m_eT3App;
        Q_103IF_SendMode m_eIF_Send;
        Q_103IF_RecvMode m_eIF_Recv;

        */
        if (NULL!=(tmp=getContentID(129)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eIntSeq=e103IntSeqLittle;

            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eIntSeq=e103IntSeqBig;
            }
        }

        if (NULL!=(tmp=getContentID(130)))
        {
            if (strcmp("1",tmp)==0)
            {
                v_cfg->m_eFloatSeq=e103FloatSeqLittle;

            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eFloatSeq=e103FloatSeqLittle;
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
                v_cfg->m_eT2App=e103T2AppSendS;
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
                v_cfg->m_eT2App=e103T2AppSendS;
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
                v_cfg->m_eT3App=e103T3AppSendTest;
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
                v_cfg->m_eIF_Send=e103IF_SendIgnoreK;
            }
            else if (strcmp("2",tmp)==0)
            {
                v_cfg->m_eIF_Send=e103IF_SendLimitByK;
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
                v_cfg->m_eIF_Recv=e103IF_RecvBeforeW_Act;
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

		for(int i=0;i<MAX_103_REMOTE_NUM-MAX_103_REMOTE_NUM_BASE;i++)
		{
            tmp=getContentID(i*2+1,5);
			if(tmp!=NULL)
			{
				if(strcmp(tmp,"")!=0)
				{
					int idx=i+MAX_103_REMOTE_NUM_BASE;
					if(idx<MAX_103_REMOTE_NUM)
					{
					strcpy(v_cfg->remServer[idx].ip,tmp);
					v_cfg->remServer[idx].valid=true;

					tmp=getContentID(2+i*2,5);//本端端口号

					v_cfg->remServer[idx].port=atoi(tmp);
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

    }
    return v_cfg;
}
bool Q_IEC103Setting::getPrintSetting(Q_LogType type)
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
Q_103StCfg   *Q_IEC103Setting::getStationSetting(Q_103StCfg *v_cfg)
{
	if (v_cfg!=NULL)
	{
		char *tmp=NULL;
		int n=0;

		tmp=getContentID(5,2);//总召唤遥信每帧封装遥信个数
		if(tmp!=NULL)
		{
			n=atoi(tmp);
			switch(n)
			{
			case 1:
				v_cfg->m_103_AsdnYcType=e103AsduYcType9;
				break;
			case 2:
				v_cfg->m_103_AsdnYcType=e103AsduYcType11;
				break;
			case 3:
				v_cfg->m_103_AsdnYcType=e103AsduYcType13;
				break;
			case 4:
				v_cfg->m_103_AsdnYcType=e103AsduYcType21;
				break;
			default:
				v_cfg->m_103_AsdnYcType=e103AsduYcType13;
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
Q_103CtrlCfg *Q_IEC103Setting::getControlSetting(Q_103CtrlCfg *v_cfg)
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
				v_cfg->m_YtExecMode=e103YtExecModeSelect;
			}
			else if(n==2)
			{
				v_cfg->m_YtExecMode=e103YtExecModeDirect;
			}
			else
			{
				v_cfg->m_YtExecMode=e103YtExecModeNull;
			}
		}

		tmp=getContentID(49,3);//遥调模式
		if(tmp!=NULL)
		{
			n=atoi(tmp);
			if(n==1)
			{
				v_cfg->m_YsetAsduType=e103YsetAsdu30;
			}
			else if(n==2)
			{
				v_cfg->m_YsetAsduType=e103YsetAsdu31;
			}
			else
			{
				v_cfg->m_YsetAsduType=e103YsetAsdu32;
			}
		}
		else
		{
			v_cfg->m_YsetAsduType=e103YsetAsdu30;
		}

		tmp=getContentID(2,3);//遥控模式
		if(tmp!=NULL)
		{
			n=atoi(tmp);
			if(n==1)
			{
				v_cfg->m_YkAsduType=e103YkAsdu10;
			}
			else if(n==2)
			{
				v_cfg->m_YkAsduType=e103YkAsdu9;
			}
		}
		else
		{
			v_cfg->m_YkAsduType=e103YkAsdu10;
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
				v_cfg->m_YkExecMode=e103YtExecModeSelect;
			}
			else if(n==2)
			{
				v_cfg->m_YkExecMode=e103YtExecModeDirect;
			}
			else
			{
				v_cfg->m_YkExecMode=e103YtExecModeNull;
			}
		}
		
	}
    return v_cfg;
}
Q_103SyncClockCfg *Q_IEC103Setting::getSyncClockSetting(Q_103SyncClockCfg *v_cfg)
{
    char *tmp=NULL;
    if (v_cfg!=NULL)
    {
        tmp=getContentID(1,4);
        if (tmp!=NULL)
        {
            if (strcmp(tmp,"1")==0)
            {
                v_cfg->m_eActMethord=e103CS_Act_Master;
            }
            else if (strcmp(tmp,"2")==0)
            {
                v_cfg->m_eActMethord=e103CS_Act_MasterFix;
            }
            else if (strcmp(tmp,"3")==0)
            {
                v_cfg->m_eActMethord=e103CS_Act_OwnSysBf;
            }
            else if (strcmp(tmp,"4")==0)
            {
                v_cfg->m_eActMethord=e103CS_Act_OwnSysAf;
            }
        }

        tmp=getContentID(2,4);
        if (tmp!=NULL)
            v_cfg->m_nFixUsecs=atoi(tmp);


    }
    return v_cfg;
}
char *Q_IEC103Setting::getContentID(const int &v_idx,const int &v_grp)
{
    for (UINT i=0;i<m_vContent.size();i++)
    {
        if (m_vContent[i].group==v_grp &&
                                 m_vContent[i].id==v_idx)
        {
            return m_vContent[i].value;
        }
    }
    return NULL;
}