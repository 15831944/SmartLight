#include "stdafx.h"
#include "Q_frameInclude.h"
#include "Q_101StandardTags.h"
#include "tinyxmlinclude.h"

Q_104StandardTags::Q_104StandardTags()
{
	memset(m_Path,0,sizeof(m_Path));
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
Q_104StandardTags::Q_104StandardTags(Q_104StandardTags *other)
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
//	strcpy_s(m_Path,sizeof(m_Path),other->m_Path);
	sprintf_s(m_Path,sizeof(m_Path),other->m_Path);
}
Q_104StandardTags::~Q_104StandardTags()
{

}
void Q_104StandardTags::setTagsPath(char *vPath)
{
//	strcpy_s(m_Path,sizeof(m_Path),vPath);
	sprintf_s(m_Path,sizeof(m_Path),vPath);
}
bool Q_104StandardTags::initializeTags()
{
	//loadData();

	return true;
}

bool  Q_104StandardTags::loadData()
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

					p_dataAnalog[nAnlogCur].factor=(float)atof(itemElement->Attribute("factor"));
					//------by lx on 2015-6-10----------------
					//sscanf(itemElement->Attribute("factor"),"%f", &(p_dataAnalog[nAnlogCur].factor));
					//------原始------------------------------
					//sscanf_s(itemElement->Attribute("factor"),"%f", &(p_dataAnalog[nAnlogCur].factor));
					//------end lx-----------------------------
					const char *type=itemElement->Attribute("type");
					if (strcmp(type,"INT")==0)
					{
						p_dataAnalog[nAnlogCur].eDataType=eDataTypeInt;
						p_dataAnalog[nAnlogCur].nValue=atoi(itemElement->Attribute("value"));
					}
					else if (strcmp(type,"FLOAT")==0)
					{
						p_dataAnalog[nAnlogCur].eDataType=eDataTypeFloat;
						p_dataAnalog[nAnlogCur].fValue=(float)atof(itemElement->Attribute("value"));
						//------by lx on 2015-6-10----------------
						//sscanf(itemElement->Attribute("value"), "%f", &(p_dataAnalog[nAnlogCur].fValue));
						//------原始------------------------------
						//sscanf_s(itemElement->Attribute("value"), "%f", &(p_dataAnalog[nAnlogCur].fValue));
						//------end lx-----------------------------
					}
					//strcpy_s(p_dataAnalog[nAnlogCur].chName,sizeof(p_dataAnalog[nAnlogCur].chName),itemElement->Attribute("descr"));
					//strcpy_s(p_dataAnalog[nAnlogCur].unit,sizeof(p_dataAnalog[nAnlogCur].unit),itemElement->Attribute("unit"));
					sprintf_s(p_dataAnalog[nAnlogCur].chName,sizeof(p_dataAnalog[nAnlogCur].chName),itemElement->Attribute("descr"));
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

					//strcpy_s(p_dataDigital[nDigitalCur].chName,sizeof(p_dataDigital[nDigitalCur].chName),itemElement->Attribute("descr"));
					sprintf_s(p_dataDigital[nDigitalCur].chName,sizeof(p_dataDigital[nDigitalCur].chName),itemElement->Attribute("descr"));
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
void   Q_104StandardTags::setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_alg_count=nVaildCount;
	m_alg_begin=nAddrBegin;
	m_alg_end=nAddrEnd;
}

void   Q_104StandardTags::setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_pnt_count=nVaildCount;
	m_pnt_begin=nAddrBegin;
	m_pnt_end=nAddrEnd;
}
void Q_104StandardTags::setControlScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_ctrl_count=nVaildCount;
	m_ctrl_begin=nAddrBegin;
	m_ctrl_end=nAddrEnd;
}
void Q_104StandardTags::setAdjustScope(int nAddrBegin,int nAddrEnd,int nVaildCount)
{
	m_adjust_count=nVaildCount;
	m_adjust_begin=nAddrBegin;
	m_adjust_end=nAddrEnd;

}
int  Q_104StandardTags::getAnalogBegin()
{
	return m_alg_begin;
}
int  Q_104StandardTags::getAnalogEnd()
{
	return m_alg_end;
}
int    Q_104StandardTags::getPointBegin()
{ 
	return m_pnt_begin;
}
int   Q_104StandardTags::getPointEnd()
{
	return m_pnt_end;
}
int  Q_104StandardTags::getAnalogCount()
{
	return m_alg_count;
}
int   Q_104StandardTags::getPointCount()
{
	return m_pnt_count;
}
//遥控开始地址
int Q_104StandardTags::getCtrlBegin()
{
     return m_ctrl_begin;
}
int Q_104StandardTags::getCtrlEnd()
{
     return m_ctrl_end;
}
//遥调
int Q_104StandardTags::getAdjustBegin()
{
    return m_adjust_begin;
}
int Q_104StandardTags::getAdjustEnd()
{
    return m_adjust_end;
}
int Q_104StandardTags::getCtrlCount()
{
    return m_ctrl_count;
}
int Q_104StandardTags::getAdjustCount()
{
return m_adjust_count;
}
bool Q_104StandardTags::fillTagInformation(LPQ_RECORDDATATAG pData,int nAdr,int nDevID)
{

	//Q_dataManager* p = _GlobalData.getDataManager();
	Q_dataManager* p = _GlobalData.getDataManager();

	if(pData->eDataType!=eDataTypeBit)
	{
		LPQ_ANALOGUEDATATAG alg=p->findAlgTag(nDevID,nAdr,getAnalogBegin(),getAnalogEnd());
		if(alg != NULL)
		{
			pData->nIndex =alg->nIndex;//当索引用
			return true;
		}

	}
	else
	{
		LPQ_DIGITALEDATATAG  dig=p->findDigTag(nDevID,nAdr,getPointBegin(),getPointEnd());
		if(dig!=NULL)
		{
			pData->nIndex = dig->nIndex;
			return true;
		}

	}
	return false;
}
bool Q_104StandardTags::fillTagInfo_CtrlAdjust(LPQ_RECORDDATATAG pData,int nAdr,int nDevID)
{

	Q_dataManager* p = _GlobalData.getDataManager();

	if(pData->eDataType!=eDataTypeBit)
	{
		LPQ_ANALOGUEDATATAG alg=p->findAlgTag(nDevID,nAdr,getAdjustBegin(),getAdjustEnd());
		if(alg != NULL)
		{
			pData->nIndex =alg->nIndex;
			return true;
		}

	}
	else
	{
		LPQ_DIGITALEDATATAG  dig=p->findDigTag(nDevID,nAdr,getCtrlBegin(),getCtrlEnd());
		if(dig!=NULL)
		{
			pData->nIndex = dig->nIndex;
			return true;
		}

	}

	return false;
}


bool Q_104StandardTags::fillTagYTtoYC(LPQ_RECORDDATATAG pData,int nAdr,int nDevID)
{
	Q_dataManager* p = _GlobalData.getDataManager();

	if(pData->eDataType != eDataTypeBit)
	{
		LPQ_YTEDATATAG pYT=p->findYTTag(nDevID,nAdr,getAdjustBegin(),getAdjustEnd());
		if(pYT != NULL)
		{
			pData->nIndex =pYT->ycindex;
			return true;
		}

	}
	return false;
}

bool Q_104StandardTags::fillTagYKtoYX(LPQ_RECORDDATATAG pData,int nAdr,int nDevID)
{
	Q_dataManager* p = _GlobalData.getDataManager();

	if(pData->eDataType == eDataTypeBit)
	{
		LPQ_YKEDATATAG pYK=p->findYKTag(nDevID,nAdr,getCtrlBegin(),getCtrlEnd());
		if(pYK != NULL)
		{
			pData->nIndex =pYK->yxindex;
			return true;
		}

	}
	return false;
}