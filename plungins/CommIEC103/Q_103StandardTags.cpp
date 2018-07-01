#include "gSystemDef.h"
#include "Q_frameInclude.h"
#include "Q_103StandardTags.h"
#include "tinyxmlinclude.h"
Q_103StandardTags::Q_103StandardTags()
{
	memset(m_Path,NULL,sizeof(m_Path));
	m_alg_count=0;
	m_alg_begin=0;
	m_alg_end=0;
	m_pnt_count=0;
	m_pnt_begin=0;
	m_pnt_end=0;
	m_ctrl_count=0;
	m_ctrl_begin=0;
	m_ctrl_end=0;
	m_adjust_count=0;
	m_adjust_begin=0;
	m_adjust_end=0;
}
Q_103StandardTags::Q_103StandardTags(Q_103StandardTags *other)
{
	m_alg_count=other->m_alg_count;
	m_alg_begin=other->m_alg_begin;
	m_alg_end=other->m_alg_end;
	m_pnt_count=other->m_pnt_count;
	m_pnt_begin=other->m_pnt_begin;
	m_pnt_end=other->m_pnt_end;

	m_ctrl_count=other->m_ctrl_count;
	m_ctrl_begin=other->m_ctrl_begin;
	m_ctrl_end=other->m_ctrl_end;
	m_adjust_count=other->m_adjust_count;
	m_adjust_begin=other->m_adjust_begin;
	m_adjust_end=other->m_adjust_end;
	//strcpy_s(m_Path,sizeof(m_Path),other->m_Path);
	sprintf_s(m_Path,sizeof(m_Path),other->m_Path);
}
Q_103StandardTags::~Q_103StandardTags()
{

}
void Q_103StandardTags::setTagsPath(char *vPath)
{
	//strcpy_s(m_Path,sizeof(m_Path),vPath);
	sprintf_s(m_Path,sizeof(m_Path),vPath);
}
bool Q_103StandardTags::initializeTags()
{
	//loadData();

	return true;
}
bool  Q_103StandardTags::loadData()
{
	TiXmlDocument doc(m_Path);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
		TRACE("--prase file failed in cdtChntCtrlTags\n");
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

	int nAnlogCur=0;
	int nDigitalCur=0;

	Q_dataManager* p = _GlobalData.getDataManager();

	LPQ_ANALOGUEDATATAG p_dataAnalog=p->getAnalogData();//遥测值

	LPQ_DIGITALEDATATAG p_dataDigital=p->getDigitalData();//遥信值

	for ( node = todoElement->FirstChild( "Record" );node;node = node->NextSibling( "Record" ) )
	{
		itemElement=node->ToElement();

		if(strcmp(itemElement->Attribute("name"),"data")==0)
		{
			for (nodeItem=node->FirstChild("Field");nodeItem;
				nodeItem=nodeItem->NextSibling("Field"))
			{
				itemElement=nodeItem->ToElement();
				int nID=atoi(itemElement->Attribute("id"));
				int n_val=atoi(itemElement->Attribute("value"));
				switch(nID)
				{
				case 1:
					m_alg_count=n_val;
					break;
				case 2:
					m_alg_begin=n_val;
					break;
				case 3:
					m_alg_end=n_val;
					break;
				case 4:
					m_pnt_count=n_val;
					break;
				case 5:
					m_pnt_begin=n_val;
					break;
				case 6:
					m_pnt_end=n_val;
					break;
				}
			}
		}
		else if(strcmp(itemElement->Attribute("name"),"analog")==0)
		{
			if(p->getAnalogCount()>0)
			{

                nAnlogCur+=m_alg_begin;
				for (nodeItem=node->FirstChild("Field");nodeItem;
					nodeItem=nodeItem->NextSibling("Field"))
				{
					itemElement=nodeItem->ToElement();

					p_dataAnalog[nAnlogCur].nIndex=atoi(itemElement->Attribute("id"));
					p_dataAnalog[nAnlogCur].device=atoi(itemElement->Attribute("device"));
					p_dataAnalog[nAnlogCur].no=atoi(itemElement->Attribute("no"));
					//=atof(itemElement->Attribute("factor"));
					sscanf(itemElement->Attribute("factor"),"%f", &(p_dataAnalog[nAnlogCur].factor));
					const char *type=itemElement->Attribute("type");
					if (strcmp(type,"INT")==0)
					{
						p_dataAnalog[nAnlogCur].eDataType=eDataTypeInt;
						p_dataAnalog[nAnlogCur].nValue=atoi(itemElement->Attribute("value"));
					}
					else if (strcmp(type,"FLOAT")==0)
					{
						p_dataAnalog[nAnlogCur].eDataType=eDataTypeFloat;
						//p_dataAnalog[nAnlogCur].fValue=atof(itemElement->Attribute("value"));
						sscanf(itemElement->Attribute("value"), "%f", &(p_dataAnalog[nAnlogCur].fValue));
					}
					//strcpy_s(p_dataAnalog[nAnlogCur].chName,sizeof(p_dataAnalog[nAnlogCur].chName),itemElement->Attribute("descr"));
					sprintf_s(p_dataAnalog[nAnlogCur].chName,sizeof(p_dataAnalog[nAnlogCur].chName),itemElement->Attribute("descr"));

					//strcpy_s(p_dataAnalog[nAnlogCur].unit,sizeof(p_dataAnalog[nAnlogCur].unit),itemElement->Attribute("unit"));
					sprintf_s(p_dataAnalog[nAnlogCur].unit,sizeof(p_dataAnalog[nAnlogCur].unit),itemElement->Attribute("unit"));
					//p_dataAnalog[nAnlogCur].testflag=atoi(itemElement->Attribute("testflag"));
					//p_dataAnalog[nAnlogCur].testvalue=atoi(itemElement->Attribute("testvalue"));
					nAnlogCur++;
				}
			}
			else continue;
		}
		else if(strcmp(itemElement->Attribute("name"),"point")==0)
		{
			if(p->getDigitalCount()>0)
			{

				nDigitalCur+=m_pnt_begin;
				for (nodeItem=node->FirstChild("Field");nodeItem;
					nodeItem=nodeItem->NextSibling("Field"))
				{
					itemElement=nodeItem->ToElement();

					strcpy(p_dataDigital[nDigitalCur].chName,itemElement->Attribute("descr"));
					p_dataDigital[nDigitalCur].nIndex=atoi(itemElement->Attribute("id"));
					p_dataDigital[nDigitalCur].device=atoi(itemElement->Attribute("device"));
					p_dataDigital[nDigitalCur].no=atoi(itemElement->Attribute("no"));
					int n=atoi(itemElement->Attribute("value"));
					p_dataDigital[nDigitalCur].bValue=(n==1?true:false);

					//p_dataDigital[nDigitalCur].testflag=atoi(itemElement->Attribute("testflag"));
					//p_dataDigital[nDigitalCur].testvalue=atoi(itemElement->Attribute("testvalue"));

					nDigitalCur++;
				}
			}
			else 
				continue;
		}

	}
	return true;
}
void   Q_103StandardTags::setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_alg_count=nVaildCount;
	m_alg_begin=nAddrBegin;
	m_alg_end=nAddrEnd;
}

void   Q_103StandardTags::setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_pnt_count=nVaildCount;
	m_pnt_begin=nAddrBegin;
	m_pnt_end=nAddrEnd;
}
void Q_103StandardTags::setControlScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_ctrl_count=nVaildCount;
	m_ctrl_begin=nAddrBegin;
	m_ctrl_end=nAddrEnd;
}
void Q_103StandardTags::setAdjustScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_adjust_count=nVaildCount;
	m_adjust_begin=nAddrBegin;
	m_adjust_end=nAddrEnd;

}
int  Q_103StandardTags::getAnalogBegin()
{
	return m_alg_begin;
}
int  Q_103StandardTags::getAnalogEnd()
{
	return m_alg_end;
}
int    Q_103StandardTags::getPointBegin()
{ 
	return m_pnt_begin;
}
int   Q_103StandardTags::getPointEnd()
{
	return m_pnt_end;
}
int  Q_103StandardTags::getAnalogCount()
{
	return m_alg_count;
}
int   Q_103StandardTags::getPointCount()
{
	return m_pnt_count;
}
//遥控开始地址
int Q_103StandardTags::getCtrlBegin()
{
     return m_ctrl_begin;
}
int Q_103StandardTags::getCtrlEnd()
{
     return m_ctrl_end;
}
//遥调
int Q_103StandardTags::getAdjustBegin()
{
    return m_adjust_begin;
}
int Q_103StandardTags::getAdjustEnd()
{
    return m_adjust_end;
}
int Q_103StandardTags::getCtrlCount()
{
    return m_ctrl_count;
}
int Q_103StandardTags::getAdjustCount()
{
return m_adjust_count;
}
bool Q_103StandardTags::fillTagInformation(LPQ_RECORDDATATAG pData,int nAdr,int nDevID)
{

	//Q_dataManager* p = _GlobalData.getDataManager();
	Q_dataManager* p = _GlobalData.getDataManager();

	if(pData->eDataType!=eDataTypeBit)
	{
		LPQ_ANALOGUEDATATAG alg=p->findAlgTag(nDevID,nAdr,getAnalogBegin(),getAnalogEnd());
		if(alg != NULL)
		{
			pData->nIndex =alg->nIndex-1;//当索引用
			return true;
		}

	}
	else
	{
		LPQ_DIGITALEDATATAG  dig=p->findDigTag(nDevID,nAdr,getPointBegin(),getPointEnd());
		if(dig!=NULL)
		{
			pData->nIndex = dig->nIndex-1;
			return true;
		}

	}
	return false;
}
bool Q_103StandardTags::fillTagInfo_CtrlAdjust(LPQ_RECORDDATATAG pData,int nAdr,int nDevID)
{

	Q_dataManager* p = _GlobalData.getDataManager();

	if(pData->eDataType!=eDataTypeBit)
	{
		LPQ_ANALOGUEDATATAG alg=p->findAlgTag(nDevID,nAdr,getAdjustBegin(),getAdjustEnd());
		if(alg != NULL)
		{
			pData->nIndex =alg->nIndex-1;//当索引用
			return true;
		}

	}
	else
	{
		LPQ_DIGITALEDATATAG  dig=p->findDigTag(nDevID,nAdr,getCtrlBegin(),getCtrlEnd());
		if(dig!=NULL)
		{
			pData->nIndex = dig->nIndex-1;
			return true;
		}

	}
	return false;
}


bool Q_103StandardTags::fillTagYTtoYC(LPQ_RECORDDATATAG pData,int nAdr,int nDevID)
{
	Q_dataManager* p = _GlobalData.getDataManager();

	if(pData->eDataType!=eDataTypeBit)
	{
		LPQ_YTEDATATAG pYT=p->findYTTag(nDevID,nAdr,getAdjustBegin(),getAdjustEnd());
		if(pYT != NULL)
		{
			pData->nIndex =pYT->ycindex-1;
			return true;
		}

	}
	return false;
}