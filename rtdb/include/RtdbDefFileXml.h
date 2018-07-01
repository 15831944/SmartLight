#ifndef __AFX_RTDB_DEF_FILE_XML_DEAL_H_WENPO_2015_06_19
#define __AFX_RTDB_DEF_FILE_XML_DEAL_H_WENPO_2015_06_19
/****************************************************************************
 * RtdbDefFileXml.h: 处理实时库结构定义信息与DBTblStructure.xml文件的交互操作(读存)	*
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
	 bool	ReadRtdbDefXml(char * pRtdbDefFile, CMultiDataBase * pMDB);//Xml总函数
	 bool	ReadFileXml(char * pRtdbDefFile);//读取xml初始化m_vecDB m_vecTable m_vecField
	 bool	SetDate(CMultiDataBase * pMDB);//将vector中的数据设置到数据库
	 bool	ReadFreeXml();//释放vector
	 CDataBase *	ReadDataBaseXml(XmlDBStruct* pXmlDBStruct);//将DataBase添加到数据库
	 bool	ReadTableXml(XmlTable* pXmlTable, CDBTable * pTable);//将Table添加到数据库
	 bool	ReadFieldXml(XmlField* pXmlField, CDBField * pField);//将Field添加到数据库

	 vector<XmlDBStruct*> m_vecDB;  
	 vector<XmlTable*> m_vecTable;  
	 vector<XmlField*> m_vecField;  
};
#endif // __AFX_RTDB_DEF_FILE_XML_DEAL_H_WENPO_2015_06_19
