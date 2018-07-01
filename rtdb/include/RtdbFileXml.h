#pragma once
/////////////////////////////////////////////////////////////////////////////
// RtdbFileXml.h : 实现实时库与关系库之间的读写操作
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_RTDB_FILE_RW_H_WENPO_2015_06_29__
#define __AFX_RTDB_FILE_RW_H_WENPO_2015_06_29__
/////////////////////////////////////////////////////////////////////////////
#include "gSystemDef.h"
#include "MultiDataBase.h"
class AFX_EXT_CLASS CRtdbFileXml
{
public:
	CRtdbFileXml(CMultiDataBase * pMultiDB);
	~CRtdbFileXml(void);
public:
	char 	m_pszPath[MAX_PATH];
	bool	m_bCName;
	char	m_cSeparator;
	char	m_szNameLink[10];
	char	m_szFileExt[6];
public:
	bool	SaveAllDataXml(CMultiDataBase * pMDB);//保存数据库
	bool	SaveDbDataXml(CDataBase& DataBase);//保存数据库中的表
	bool	SaveTableDataXml(CDataBase& db, CDBTable& tab);//保存表中的记录
	//nDataType 0全部数据 1nFlag=1的数据 2nFlag等0的数据
	bool	SaveAsTableDataXml(const char *tableName, const char *pathas,int nDataType);
	bool	AnalyseFileName(const char *pFileName, char *pDBName, char *pTableName);

	bool	InsertRecordXml(CDataBase& db, CDBTable& tab, UINT nID);
	bool	DeleteRecordXml(CDataBase& db, CDBTable& tab, UINT nID);
	bool	UpdateRecordXml(CDataBase& db, CDBTable& tab, UINT nID);

	bool	LoadAllDataXml(CMultiDataBase * pMDB);
	bool	LoadDbDataXml(CDataBase& DataBase);
	bool	LoadTableDataXml(CDataBase& db, CDBTable& tab);
private:
	CMultiDataBase * m_pMultiDB;//实时库接口
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_RTDB_FILE_RW_H_WENPO_2015_06_29__

