#pragma once
/////////////////////////////////////////////////////////////////////////////
// RtdbFileXml.h : ʵ��ʵʱ�����ϵ��֮��Ķ�д����
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
	bool	SaveAllDataXml(CMultiDataBase * pMDB);//�������ݿ�
	bool	SaveDbDataXml(CDataBase& DataBase);//�������ݿ��еı�
	bool	SaveTableDataXml(CDataBase& db, CDBTable& tab);//������еļ�¼
	//nDataType 0ȫ������ 1nFlag=1������ 2nFlag��0������
	bool	SaveAsTableDataXml(const char *tableName, const char *pathas,int nDataType);
	bool	AnalyseFileName(const char *pFileName, char *pDBName, char *pTableName);

	bool	InsertRecordXml(CDataBase& db, CDBTable& tab, UINT nID);
	bool	DeleteRecordXml(CDataBase& db, CDBTable& tab, UINT nID);
	bool	UpdateRecordXml(CDataBase& db, CDBTable& tab, UINT nID);

	bool	LoadAllDataXml(CMultiDataBase * pMDB);
	bool	LoadDbDataXml(CDataBase& DataBase);
	bool	LoadTableDataXml(CDataBase& db, CDBTable& tab);
private:
	CMultiDataBase * m_pMultiDB;//ʵʱ��ӿ�
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_RTDB_FILE_RW_H_WENPO_2015_06_29__

