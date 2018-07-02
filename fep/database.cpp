#include "gSystemDef.h"
#include "database.h"
#include "tinyxmlinclude.h"
#include "GlobalDefine.h"
database::database()
{
	m_nMaxAnalogCount = 0;//����ң�����
	m_nMaxDigitalCount = 0;//����ң�Ÿ���
	m_nMaxYKCount = 0;//����ң�ظ���
	m_nMaxYTCount = 0;//����ң������
	m_nChanCount = 0;//ͨ������
	setLogPath();
	m_log=NULL;
}
database::~database()
{
}

void database::setDataPath(char *v_path)
{
  // strcpy_s(m_Path,sizeof(m_Path),v_path);
	sprintf_s(m_Path,sizeof(m_Path),"%s",v_path);

   m_pMgr = _GlobalData.getDataManager();
}

bool database::loadYC(vector<ST_GeCtrlChl> &vecChan)
{	
	char strYCpath[MAX_PATH];
	sprintf_s(strYCpath,MAX_PATH,"%s%s",m_Path,"yc.xml");

	TiXmlDocument doc(strYCpath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
		if(m_setting->getDataSaveFlg())
		{
			if(!copyFile("yc"))
			{
				Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ʽ����\n��������%s\n����λ�ã���%d ,��%d",strYCpath,doc.ErrorDesc(),doc.ErrorRow(),doc.ErrorCol());
		
				return false;
			}
			else
			{
				doc.LoadFile();
				if(doc.ErrorId() > 0)  
				{
					Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ʽ����\n��������%s\n����λ�ã���%d ,��%d",strYCpath,doc.ErrorDesc(),doc.ErrorRow(),doc.ErrorCol());
				
					return false;

				}
			}
		}
		else
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ʽ����\n��������%s\n����λ�ã���%d ,��%d",strYCpath,doc.ErrorDesc(),doc.ErrorRow(),doc.ErrorCol());
	
			return false;
		}

	}

	TiXmlNode* node = 0;
	TiXmlNode* root = 0;
	TiXmlNode* nodeItem = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	root = doc.FirstChild( "Setting" );
	todoElement = root->ToElement();

	nodeItem = doc.FirstChild( "Setting" );
	if (nodeItem==NULL)
	{
		return false;
	}
	nodeItem  = nodeItem->FirstChild("Field");
	if (nodeItem==NULL)
	{
		return false;
	}
	
	int nYCCount = 0;
	//read yccount

	for ( node = todoElement->FirstChild( "Field" );
		node;
		node = node->NextSibling( "Field" ) )
	{
		nYCCount++;
	}

	const char* pAttr = NULL;

	if (!m_pMgr->AllocateYCMem(nYCCount))
	{
		Q_commonUtility::outError(m_log->getFile(),"AllocateYCMemʧ�ܣ�");

		return false;
	}
	m_nMaxAnalogCount = nYCCount;//���ң�����

	nYCCount = 0;
	int nPrevChanid = 0;
	LPQ_ANALOGUEDATATAG p_dataAnalog= m_pMgr->getAnalogData();//ң��ֵ

	char unit[20];
	for ( node = todoElement->FirstChild( "Field" );
		node;
		node = node->NextSibling( "Field" ) )
	{
		itemElement=node->ToElement();
		pAttr = itemElement->Attribute("id");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������id�����⣬����ϸ���",strYCpath);
		
			return false;
		}
		p_dataAnalog[nYCCount].nIndex=atoi(pAttr);
		if (nYCCount+1!=p_dataAnalog[nYCCount].nIndex)
		{
		Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������id = %d����������������ϸ���",strYCpath,p_dataAnalog[nYCCount].nIndex);

			return false;
		}
		pAttr = itemElement->Attribute("descr");
		if (pAttr==NULL)
		{
		Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������descr�����⣬����ϸ���",strYCpath);

			return false;
		}
		//strcpy_s(p_dataAnalog[nYCCount].chName,sizeof(p_dataAnalog[nYCCount].chName),pAttr);
		sprintf_s(p_dataAnalog[nYCCount].chName,sizeof(p_dataAnalog[nYCCount].chName),"%s",pAttr);
		pAttr = itemElement->Attribute("datasource");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������datasource�����⣬����ϸ���",strYCpath);

			return false;
		}
		p_dataAnalog[nYCCount].datasource = atoi(pAttr);
		//if (p_dataAnalog[nYCCount].datasource==2)
		//{//���
		//	goto NEXTYC;
		//}

		pAttr = itemElement->Attribute("channel");
		if (pAttr==NULL)
		{
		Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������channel�����⣬����ϸ���",strYCpath);

			return false;
		}
		p_dataAnalog[nYCCount].channelid = atoi(pAttr);
		nPrevChanid = p_dataAnalog[nYCCount].channelid;
		bool bifmatch =false;//���channel�Ƿ�ƥ��channel.xml�е�id���Ƿ���ڸ�ͨ��
		for (vector<ST_GeCtrlChl>::iterator it=vecChan.begin();it!=vecChan.end();it++)
		{
			if (it->id==p_dataAnalog[nYCCount].channelid)
			{
				bifmatch =true;
				if (it->YCbegin==0)//firsttime
				{
					it->YCbegin = p_dataAnalog[nYCCount].nIndex;
					it->YCend =  p_dataAnalog[nYCCount].nIndex;
				}
				else
				{
					it->YCend = p_dataAnalog[nYCCount].nIndex;
				}
				break;
			}
		}
		if (!bifmatch)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң��id=%d��channel��ͨ�����id��ƥ�䣬����ϸ���",strYCpath,p_dataAnalog[nYCCount].nIndex);

			return false;
		}
		pAttr = itemElement->Attribute("device");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������device�����⣬����ϸ���",strYCpath);

			return false;
		}
		p_dataAnalog[nYCCount].device=atoi(pAttr);
		pAttr = itemElement->Attribute("no");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������no�����⣬����ϸ���",strYCpath);
		
			return false;
		}
		p_dataAnalog[nYCCount].no=atoi(pAttr);
		pAttr = itemElement->Attribute("factor");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������factor�����⣬����ϸ���",strYCpath);

			return false;
		}
		p_dataAnalog[nYCCount].factor=(float)atof(pAttr);
		pAttr = itemElement->Attribute("type");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������type�����⣬����ϸ���",strYCpath);

			return false;
		}
		const char *type=pAttr;
		if(type!=NULL)
		{
 			if (strcmp(type,"INT")==0)
 			{
 				p_dataAnalog[nYCCount].eDataType=eDataTypeInt;
 				pAttr = itemElement->Attribute("value");
 				if (pAttr==NULL)
 				{
 					Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������value�����⣬����ϸ���",strYCpath);
 			
 					return false;
 				}
 				p_dataAnalog[nYCCount].nValue=atoi(pAttr);
 			}
 			else if (strcmp(type,"FLOAT")==0)
 			{
				p_dataAnalog[nYCCount].eDataType=eDataTypeFloat;
				pAttr = itemElement->Attribute("value");
				if (pAttr==NULL)
				{
				Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������value�����⣬����ϸ���",strYCpath);
	
					return false;
				}
				//sscanf_s(pAttr, "%f", &(p_dataAnalog[nYCCount].fValue));
				p_dataAnalog[nYCCount].fValue=(float)atof(pAttr);
			}
		}

		pAttr = itemElement->Attribute("unit");
		if (pAttr!=NULL)
		{
			sprintf_s(p_dataAnalog[nYCCount].unit,sizeof(p_dataAnalog[nYCCount].unit),"%s",pAttr);
			
			const char *tmp=itemElement->Attribute("unit");
			if(tmp!=NULL)
			{
				//strcpy_s(p_dataAnalog[nYCCount].unit,sizeof(p_dataAnalog[nYCCount].unit),tmp);
				sprintf_s(p_dataAnalog[nYCCount].unit,sizeof(p_dataAnalog[nYCCount].unit),"%s",tmp);
				
				int length = strlen(tmp); 
				memset(unit,NULL,sizeof(unit));
				
				for (int i=0; i<length; i++) 
				{ 
					unit[i] = tolower(p_dataAnalog[nYCCount].unit[i]); 
				} 
				if(strcmp(unit,"mw")==0)
				{
					p_dataAnalog[nYCCount].eUnitType=eUnitTypeMW;
				}
				else if(strcmp(unit,"kw")==0)
				{
					p_dataAnalog[nYCCount].eUnitType=eUnitTypekW;
				}
				else if(strcmp(unit,"kvar")==0)
				{
					p_dataAnalog[nYCCount].eUnitType=eUnitTypekVar;
				}
				else if(strcmp(unit,"mvar")==0)
				{
					p_dataAnalog[nYCCount].eUnitType=eUnitTypeMVar;
				}
				else
				{
					p_dataAnalog[nYCCount].eUnitType=eUnitTypeNull;
				}

			}
		}

		nYCCount++;
		
	}

	return true;
}

bool database::loadYX(vector<ST_GeCtrlChl> &vecChan)
{
	char strYXpath[MAX_PATH];
	sprintf_s(strYXpath,MAX_PATH,"%s%s",m_Path,"yx.xml");

	TiXmlDocument doc(strYXpath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)
	{
		if(m_setting->getDataSaveFlg())
		{
			if(!copyFile("yx"))
			{
				Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ʽ����\n��������%s\n����λ�ã���%d ,��%d",strYXpath,doc.ErrorDesc(),doc.ErrorRow(),doc.ErrorCol());
		
				return false;
			}
			else
			{
				doc.LoadFile();
				if(doc.ErrorId() > 0)  
				{
					Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ʽ����\n��������%s\n����λ�ã���%d ,��%d",strYXpath,doc.ErrorDesc(),doc.ErrorRow(),doc.ErrorCol());
					
					return false;
				}
			}
		}
		else
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ʽ����\n��������%s\n����λ�ã���%d ,��%d",strYXpath,doc.ErrorDesc(),doc.ErrorRow(),doc.ErrorCol());

			return false;
		}
	}

	TiXmlNode* nodeItem = 0;
	TiXmlElement* itemElement = 0;

	nodeItem = doc.FirstChild( "Setting" );
	if (nodeItem==NULL)
	{
		return false;
	}
	nodeItem  = nodeItem->FirstChild("Field");
	if (nodeItem==NULL)
	{
		return false;
	}
	const char* pAttr = NULL;
	int nYXCount = 0;
	//read yxcount
	while(nodeItem!=NULL)
	{
		nYXCount++;
		nodeItem = nodeItem->NextSibling("Field");
	}
	if (!m_pMgr->AllocateYXMem(nYXCount))
	{
		Q_commonUtility::outError(m_log->getFile(),"AllocateYXMemʧ�ܣ�");

		return false;
	}
	m_nMaxDigitalCount = nYXCount;
	nodeItem = doc.FirstChild("Setting")->FirstChild("Field");
	nYXCount = 0;
	int nPrevChanid = 0;
	LPQ_DIGITALEDATATAG p_dataDigital=m_pMgr->getDigitalData();//ң��ֵ
	while(nodeItem!=NULL)
	{
		itemElement=nodeItem->ToElement();

		pAttr = itemElement->Attribute("id");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������id�����⣬����ϸ���",strYXpath);

			return false;
		}
		p_dataDigital[nYXCount].nIndex=atoi(pAttr);
		if (nYXCount+1!=p_dataDigital[nYXCount].nIndex)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������id = %d����������������ϸ���",strYXpath,p_dataDigital[nYXCount].nIndex);

			return false;
		}
		pAttr = itemElement->Attribute("descr");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������descr�����⣬����ϸ���",strYXpath);

			return false;
		}
	//	strcpy_s(p_dataDigital[nYXCount].chName,sizeof(p_dataDigital[nYXCount].chName),pAttr);
		sprintf_s(p_dataDigital[nYXCount].chName,sizeof(p_dataDigital[nYXCount].chName),"%s",pAttr);

		pAttr = itemElement->Attribute("datasource");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������datasource�����⣬����ϸ���",strYXpath);
		
			return false;
		}
		p_dataDigital[nYXCount].datasource=atoi(pAttr);
		///*if (p_dataDigital[nYXCount].datasource==2)
		//{
		//	goto NEXTYX;
		//}*/

		pAttr = itemElement->Attribute("channel");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������channel�����⣬����ϸ���",strYXpath);
	
			return false;
		}

		p_dataDigital[nYXCount].channelid = atoi(pAttr);
		//if (nPrevChanid>p_dataDigital[nYXCount].channelid)
		//{//
		//	strErr.Format("%s�ļ���ң��id=%d��������channel��˳�����ӣ�����ϸ���",strYXpath,p_dataDigital[nYXCount].nIndex);
		//	//::AfxMessageBox(strErr,MB_OK|MB_ICONERROR);
		//	return false;
		//}
		nPrevChanid = p_dataDigital[nYXCount].channelid;
		bool bifmatch =false;//���channel�Ƿ�ƥ��channel.xml�е�id���Ƿ���ڸ�ͨ��
		for (vector<ST_GeCtrlChl>::iterator it=vecChan.begin();it!=vecChan.end();it++)
		{
			if (it->id==p_dataDigital[nYXCount].channelid)
			{
				bifmatch =true;
				if (it->YXbegin==0)//firsttime
				{
					it->YXbegin = p_dataDigital[nYXCount].nIndex;
					it->YXend =  p_dataDigital[nYXCount].nIndex;
				}
				else
				{
					it->YXend = p_dataDigital[nYXCount].nIndex;
				}
				break;
			}
		}
		if (!bifmatch)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң��id=%d��channel��ͨ�����id��ƥ�䣬����ϸ���",strYXpath,p_dataDigital[nYXCount].nIndex);
	
			return false;
		}
		pAttr = itemElement->Attribute("id");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������id�����⣬����ϸ���",strYXpath);

			return false;
		}
		p_dataDigital[nYXCount].nIndex=atoi(pAttr);

		pAttr = itemElement->Attribute("device");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������id�����⣬����ϸ���",strYXpath);
	
			return false;
		}
		p_dataDigital[nYXCount].device=atoi(pAttr);

		pAttr = itemElement->Attribute("no");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������no�����⣬����ϸ���",strYXpath);

			return false;
		}
		p_dataDigital[nYXCount].no=atoi(pAttr);

		pAttr = itemElement->Attribute("value");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������no�����⣬����ϸ���",strYXpath);

			return false;
		}
		p_dataDigital[nYXCount].bValue=(atoi(pAttr)==1?true:false);
		p_dataDigital[nYXCount].quality = eCreate;

		nYXCount++;
		nodeItem = nodeItem->NextSibling("Field");
	}
	return true;
}
bool database::loadYK(vector<ST_GeCtrlChl> &vecChan)
{
	//Q_dataManager* p = _GlobalData.getDataManager();
	
	char strYKpath[MAX_PATH];
	sprintf_s(strYKpath,MAX_PATH,"%s%s",m_Path,"yk.xml");
	
	TiXmlDocument doc(strYKpath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
		Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ʽ����\n��������%s\n����λ�ã���%d ,��%d",strYKpath,doc.ErrorDesc(),doc.ErrorRow(),doc.ErrorCol());
		
		return false;
	}

	TiXmlNode* nodeItem = 0;
	TiXmlElement* itemElement = 0;

	nodeItem = doc.FirstChild( "Setting" );
	if (nodeItem==NULL)
	{
		return false;
	}
	nodeItem  = nodeItem->FirstChild("Field");
	if (nodeItem==NULL)
	{
		return false;
	}
	const char* pAttr = NULL;
	int nYKCount = 0;
	//read ykcount
	while(nodeItem!=NULL)
	{
		nYKCount++;
		nodeItem = nodeItem->NextSibling("Field");
	}
	if (!m_pMgr->AllocateYKMem(nYKCount))
	{
		Q_commonUtility::outError(m_log->getFile(),"AllocateYKMemʧ�ܣ�");
		
		return false;
	}
	m_nMaxYKCount = nYKCount;
	nodeItem = doc.FirstChild("Setting")->FirstChild("Field");
	nYKCount = 0;
	int nPrevChanid = 0;
	LPQ_YKEDATATAG p_dataYK=m_pMgr->getYKData();//ң��ֵ
	while(nodeItem!=NULL)
	{
		itemElement=nodeItem->ToElement();

		pAttr = itemElement->Attribute("id");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������id�����⣬����ϸ���",strYKpath);

			return false;
		}


		p_dataYK[nYKCount].nIndex=atoi(pAttr);
		if (nYKCount+1!=p_dataYK[nYKCount].nIndex)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������id = %d����������������ϸ���",strYKpath,p_dataYK[nYKCount].nIndex);
					return false;
		}

		pAttr = itemElement->Attribute("channel");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������channel�����⣬����ϸ���",strYKpath);

			return false;
		}

		p_dataYK[nYKCount].channelid = atoi(pAttr);
		if (nPrevChanid>p_dataYK[nYKCount].channelid)
		{//
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң��id=%d��������channel��˳�����ӣ�����ϸ���",strYKpath,p_dataYK[nYKCount].nIndex);

			return false;
		}
		nPrevChanid = p_dataYK[nYKCount].channelid;
		bool bifmatch =false;//���channel�Ƿ�ƥ��channel.xml�е�id���Ƿ���ڸ�ͨ��
		for (vector<ST_GeCtrlChl>::iterator it=vecChan.begin();it!=vecChan.end();it++)
		{
			if (it->id==p_dataYK[nYKCount].channelid)
			{
				bifmatch =true;
				if (it->YKbegin==0)//firsttime
				{
					it->YKbegin = p_dataYK[nYKCount].nIndex;
					it->YKend =  p_dataYK[nYKCount].nIndex;
				}
				else
				{
					it->YKend = p_dataYK[nYKCount].nIndex;
				}
				break;
			}
		}
		if (!bifmatch)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң��id=%d��channel��ͨ�����id��ƥ�䣬����ϸ���",strYKpath,p_dataYK[nYKCount].nIndex);
			return false;
		}


		itemElement=nodeItem->ToElement();

		pAttr = itemElement->Attribute("descr");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������descr�����⣬����ϸ���",strYKpath);

			return false;
		}
		//strcpy_s(p_dataYK[nYKCount].chName,sizeof(p_dataYK[nYKCount].chName),pAttr);
        sprintf_s(p_dataYK[nYKCount].chName,sizeof(p_dataYK[nYKCount].chName),"%s",pAttr);

		pAttr = itemElement->Attribute("id");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������id�����⣬����ϸ���",strYKpath);

			return false;
		}
		p_dataYK[nYKCount].nIndex=atoi(pAttr);

		pAttr = itemElement->Attribute("device");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������device�����⣬����ϸ���",strYKpath);

			return false;
		}
		p_dataYK[nYKCount].device=atoi(pAttr);

		pAttr = itemElement->Attribute("no");
		if (pAttr==NULL)
		{
		Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������no�����⣬����ϸ���",strYKpath);

			return false;
		}
		p_dataYK[nYKCount].no=atoi(pAttr);

		pAttr = itemElement->Attribute("value");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������value�����⣬����ϸ���",strYKpath);

			return false;
		}
		p_dataYK[nYKCount].bValue=(atoi(pAttr)==1?true:false);

		pAttr = itemElement->Attribute("yxindex");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������yxindex�����⣬����ϸ���",strYKpath);

			return false;
		}
		p_dataYK[nYKCount].yxindex = atoi(pAttr);

		pAttr = itemElement->Attribute("type");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������type�����⣬����ϸ���",strYKpath);

			return false;
		}
		p_dataYK[nYKCount].controltype = atoi(pAttr);

		pAttr = itemElement->Attribute("ifZF");
		if (pAttr!=NULL)
		{
			p_dataYK[nYKCount].bzf=atoi(pAttr)==0 ? false : true;
		}

		
			pAttr = itemElement->Attribute("zfchannelid");
			if (pAttr==NULL)
			{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������zfchannelid�����⣬����ϸ���",strYKpath);

				return false;
			}
			p_dataYK[nYKCount].zfchannelid=atoi(pAttr);

			pAttr = itemElement->Attribute("zfdevice");
			if (pAttr==NULL)
			{
		Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������zfdevice�����⣬����ϸ���",strYKpath);
		
				return false;
			}
			p_dataYK[nYKCount].zfdevice=atoi(pAttr);

			pAttr = itemElement->Attribute("zfno");
			if (pAttr==NULL)
			{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������zfno�����⣬����ϸ���",strYKpath);

				return false;
			}
			p_dataYK[nYKCount].zfno=atoi(pAttr);


		nYKCount++;
		nodeItem = nodeItem->NextSibling("Field");
	}
	return true;
}
bool database::loadYT(vector<ST_GeCtrlChl> &vecChan)
{	
	char strYTpath[MAX_PATH];
	sprintf_s(strYTpath,MAX_PATH,"%s%s",m_Path,"yt.xml");


	TiXmlDocument doc(strYTpath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
		Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ʽ����\n��������%s\n����λ�ã���%d ,��%d",strYTpath,doc.ErrorDesc(),doc.ErrorRow(),doc.ErrorCol());

		return false;
	}

	TiXmlNode* nodeItem = 0;
	TiXmlElement* itemElement = 0;

	nodeItem = doc.FirstChild( "Setting" );
	if (nodeItem==NULL)
	{
		return false;
	}
	nodeItem  = nodeItem->FirstChild("Field");
	if (nodeItem==NULL)
	{
		return false;
	}
	const char* pAttr = NULL;
	int nYTCount = 0;
	//read ytcount
	while(nodeItem!=NULL)
	{
		nYTCount++;
		nodeItem = nodeItem->NextSibling("Field");
	}
	if (!m_pMgr->AllocateYTMem(nYTCount))
	{
		Q_commonUtility::outError(m_log->getFile(),"AllocateYKMemʧ�ܣ�");
	
		return false;
	}
	m_nMaxYTCount = nYTCount;
	nodeItem = doc.FirstChild("Setting")->FirstChild("Field");
	nYTCount = 0;
	int nPrevChanid = 0;
	LPQ_YTEDATATAG p_dataYT=m_pMgr->getYTData();//ң��ֵ
	while(nodeItem!=NULL)
	{
		itemElement=nodeItem->ToElement();

		pAttr = itemElement->Attribute("id");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������id�����⣬����ϸ���",strYTpath);

			return false;
		}


		p_dataYT[nYTCount].nIndex=atoi(pAttr);
		if (nYTCount+1!=p_dataYT[nYTCount].nIndex)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������id = %d����������������ϸ���",strYTpath,p_dataYT[nYTCount].nIndex);
			
			return false;
		}

		pAttr = itemElement->Attribute("channel");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������channel�����⣬����ϸ���",strYTpath);
	
			return false;
		}

		p_dataYT[nYTCount].channelid = atoi(pAttr);
		if (nPrevChanid>p_dataYT[nYTCount].channelid)
		{//
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң��id=%d��������channel��˳�����ӣ�����ϸ���",strYTpath,p_dataYT[nYTCount].nIndex);
	
			return false;
		}
		nPrevChanid = p_dataYT[nYTCount].channelid;
		bool bifmatch =false;//���channel�Ƿ�ƥ��channel.xml�е�id���Ƿ���ڸ�ͨ��
		for (vector<ST_GeCtrlChl>::iterator it=vecChan.begin();it!=vecChan.end();it++)
		{
			if (it->id==p_dataYT[nYTCount].channelid)
			{
				bifmatch =true;
				if (it->YTbegin==0)//firsttime
				{
					it->YTbegin = p_dataYT[nYTCount].nIndex;
					it->YTend =  p_dataYT[nYTCount].nIndex;
				}
				else
				{
					it->YTend = p_dataYT[nYTCount].nIndex;
				}
				break;
			}
		}
		if (!bifmatch)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң��id=%d��channel��ͨ�����id��ƥ�䣬����ϸ���",strYTpath,p_dataYT[nYTCount].nIndex);

			return false;
		}

		pAttr = itemElement->Attribute("descr");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������descr�����⣬����ϸ���",strYTpath);
	
			return false;
		}
		//strcpy_s(p_dataYT[nYTCount].chName,sizeof(p_dataYT[nYTCount].chName),pAttr);
		sprintf_s(p_dataYT[nYTCount].chName,sizeof(p_dataYT[nYTCount].chName),"%s",pAttr);

		pAttr = itemElement->Attribute("id");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������id�����⣬����ϸ���",strYTpath);

			return false;
		}
		p_dataYT[nYTCount].nIndex=atoi(pAttr);

		pAttr = itemElement->Attribute("device");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������device�����⣬����ϸ���",strYTpath);

			return false;
		}
		p_dataYT[nYTCount].device=atoi(pAttr);

		pAttr = itemElement->Attribute("no");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������no�����⣬����ϸ���",strYTpath);

			return false;
		}
		p_dataYT[nYTCount].no=atoi(pAttr);

		pAttr = itemElement->Attribute("factor");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������factor�����⣬����ϸ���",strYTpath);

			return false;
		}
		p_dataYT[nYTCount].factor=(float)atof(pAttr);

		pAttr = itemElement->Attribute("ycindex");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������ycindex�����⣬����ϸ���",strYTpath);

			return false;
		}
		int nYCindex=atoi(pAttr);
		if (m_nMaxAnalogCount >= nYCindex)
		{
			p_dataYT[nYTCount].ycindex = nYCindex;
		}
		
		pAttr = itemElement->Attribute("value");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������value�����⣬����ϸ���",strYTpath);
			
			return false;
		}
		p_dataYT[nYTCount].fValue=(float)atof(pAttr);

		pAttr = itemElement->Attribute("factor");
		if (pAttr==NULL)
		{
			Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������factor�����⣬����ϸ���",strYTpath);

			return false;
		}
		p_dataYT[nYTCount].factor=(float)atof(pAttr);

		pAttr = itemElement->Attribute("ifZF");
		if (pAttr!=NULL)
		{
			p_dataYT[nYTCount].bzf=atoi(pAttr)==0 ? false : true;
		}

		if (p_dataYT[nYTCount].bzf)
		{
			pAttr = itemElement->Attribute("zfchannelid");
			if (pAttr==NULL)
			{
				Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������zfchannelid�����⣬����ϸ���",strYTpath);

				return false;
			}
			p_dataYT[nYTCount].zfchannelid=atoi(pAttr);

			pAttr = itemElement->Attribute("zfdevice");
			if (pAttr==NULL)
			{
				Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������zfdevice�����⣬����ϸ���",strYTpath);

				return false;
			}
			p_dataYT[nYTCount].zfdevice=atoi(pAttr);

			pAttr = itemElement->Attribute("zfno");
			if (pAttr==NULL)
			{
				Q_commonUtility::outError(m_log->getFile(),"%s�ļ���ң������zfno�����⣬����ϸ���",strYTpath);

				return false;
			}
			p_dataYT[nYTCount].zfno=atoi(pAttr);
		}
		nYTCount++;
		nodeItem = nodeItem->NextSibling("Field");
	}
	return true;
}

bool database::loadData()
{
	return true;
}

bool database::copyFile(char *ftype)
{
	////�Ȱ�ԭ�����ļ�ɾ����
	////��_tmp.xml����һ�£����Դ��Ƿ�������
	////������ɹ���bkforss.xml������
	////����Բ��ɹ�������db/his�µ����ݿ���

	//SYSTEMTIME stime;
	//GetLocalTime(&stime);

	//char tt[30];
	//sprintf_s(tt, sizeof(tt),"(%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d.%3.3d)", stime.wYear,stime.wMonth,stime.wDay,
	//	stime.wHour, stime.wMinute,
	//	stime.wSecond, stime.wMilliseconds);

	//memset(chInfo,NULL,sizeof(chInfo));
	//sprintf_s(chInfo,"%s:%s.xml��ʧ�ܣ����Կ�������",tt,ftype);
	//Q_commonUtility::writeLog(m_logPath,chInfo);

	//char dst[MAX_PATH];
	//sprintf_s(dst,MAX_PATH,"%s%s.xml",m_Path,ftype);
	//
	//char tmp[MAX_PATH];//��ʱ·��
	//sprintf_s(tmp,MAX_PATH,"%s%s_tmp.xml",m_Path,ftype);

	//CopyFile(tmp,dst,FALSE);

	////�����ܷ��

	//TiXmlDocument m_xmlYCdoc;
	//if (!m_xmlYCdoc.LoadFile(dst))
	//{

	//	memset(tmp,NULL,sizeof(tmp));
	//	sprintf_s(tmp,MAX_PATH,"%s%s_bkforss.xml",m_Path,ftype);
	//	CopyFile(tmp,dst,FALSE);
	//			

	//	if (!m_xmlYCdoc.LoadFile(dst))
	//	{
	//		memset(chInfo,NULL,sizeof(chInfo));
	//		sprintf_s(chInfo,"%s:ʹ��%s����ʧ�ܣ���������",tt,tmp);
	//		Q_commonUtility::writeLog(m_logPath,chInfo);
	//		return false;
	//	}
	//	else
	//	{
	//		memset(chInfo,NULL,sizeof(chInfo));
	//		sprintf_s(chInfo,"%s:ʹ��%s�����ɹ�",tt,tmp);
	//		Q_commonUtility::writeLog(m_logPath,chInfo);
	//	}

	//}
	//else
	//{
	//	memset(chInfo,NULL,sizeof(chInfo));
	//	sprintf_s(chInfo,"%s:ʹ��%s�����ɹ�",tt,tmp);
	//	Q_commonUtility::writeLog(m_logPath,chInfo);
	//}
	//memset(chInfo,NULL,sizeof(chInfo));
	//sprintf_s(chInfo,"%s:%s.xml���Կ������ݳɹ�",tt,ftype);
	//Q_commonUtility::writeLog(m_logPath,chInfo);
	return true;
}
void database::setLogPath()
{
	/*char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);

	CString strFile = CString(buffer);
	strFile = strFile.Left(strFile.ReverseFind('\\'));

	strFile += COMM_LOG_DETAIL;
	CTime timCur=CTime::GetCurrentTime();
	CString strPath;
	strPath.Format("%serror\\%d%.2d%.2d",strFile,timCur.GetYear(),timCur.GetMonth(),timCur.GetDay());

	sprintf_s(m_logPath,"%s���汣����־.txt",strPath.GetString());*/
}

void database::setSetting(Q_GSetting *v_setting)
{
	m_setting=v_setting;
}
void database::setLog(Log *v_log)
{
	m_log=v_log;
}