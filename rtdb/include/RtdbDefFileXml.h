#ifndef __AFX_RTDB_DEF_FILE_XML_DEAL_H_WENPO_2015_06_19
#define __AFX_RTDB_DEF_FILE_XML_DEAL_H_WENPO_2015_06_19
/****************************************************************************
 * RtdbDefFileXml.h: ����ʵʱ��ṹ������Ϣ��DBTblStructure.xml�ļ��Ľ�������(����)	*
 ****************************************************************************/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "gSystemDef.h"
#include "DBTable.h"
//#include "MultiDataBase.h"
#include <vector>
using namespace std;
#include "XmlHead.h"
class CMultiDataBase;
class CRtdbDefFileXml
{
public:
	CRtdbDefFileXml(void);
	~CRtdbDefFileXml(void);
public:
	 bool	ReadRtdbDefXml(char * pRtdbDefFile, CMultiDataBase * pMDB);//Xml�ܺ���
	 bool	ReadFileXml(char * pRtdbDefFile);//��ȡxml��ʼ��m_vecDB m_vecTable m_vecField
	 bool	SetDate(CMultiDataBase * pMDB);//��vector�е��������õ����ݿ�
	 bool	ReadFreeXml();//�ͷ�vector
	 CDataBase *	ReadDataBaseXml(XmlDBStruct* pXmlDBStruct);//��DataBase��ӵ����ݿ�
	 bool	ReadTableXml(XmlTable* pXmlTable, CDBTable * pTable);//��Table��ӵ����ݿ�
	 bool	ReadFieldXml(XmlField* pXmlField, CDBField * pField);//��Field��ӵ����ݿ�

	 vector<XmlDBStruct*> m_vecDB;  
	 vector<XmlTable*> m_vecTable;  
	 vector<XmlField*> m_vecField;  
};
#endif // __AFX_RTDB_DEF_FILE_XML_DEAL_H_WENPO_2015_06_19
