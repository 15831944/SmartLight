/////////////////////////////////////////////////////////////////////////////
// RtdbSql.h : ʵ��ʵʱ�����ϵ��֮��Ķ�д����
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_RTDB_SQL_RW_H_2005_02_22__
#define __AFX_RTDB_SQL_RW_H_2005_02_22__

#include "gSystemDef.h"

#ifdef UNIX_ENV
    #include "BaseSql_u.h"
#else
  #include "BaseSql.h"
#endif
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CRtdbSql : public CBaseSql
{
public:
	CRtdbSql(CMultiDataBase * pMultiDB);
	virtual ~CRtdbSql();

public:
	// �����������ݿ�����б������(�Զ���ɾ��������)
	bool	SaveAllData(CMultiDataBase * pMDB);
	// ����ĳ�����ݿ�����б������(�Զ���ɾ��������)
	bool	SaveDbData(CDataBase& DataBase);
	// ����ĳ���������(�Զ���ɾ��������)
	bool	SaveTableData(CDataBase& db, CDBTable& tab);
	// ����ĳ����ͼ������(�Զ���ɾ������ͼ������)
	bool	SaveViewData(CDataBase & db, CDBTable& tab,char* pszFieldCName,int nFieldVal);

	// ������������¼����ĺ���
	bool	InsertRecord(CDataBase& db, CDBTable& tab, UINT nID);
	bool	DeleteRecord(CDataBase& db, CDBTable& tab, UINT nID);
	bool	UpdateRecord(CDataBase& db, CDBTable& tab, UINT nID);

	// �����������ݿ�����б������
	bool	LoadAllData(CMultiDataBase * pMDB);
	// ����ĳ�����ݿ�����б������
	bool	LoadDbData(CDataBase& DataBase);
	// ����ĳ���������
	bool	LoadTableData(CDataBase& db, CDBTable& tab);

	// �����ÿ��д���ʵʱ���(ʹ��ǰ��:�����ļ���ʽ����ʵʱ��)
	bool	CreateAllTable(CMultiDataBase * pMDB);
	
	CString FormSQLStr( CDataBase& DataBase, CDBTable&	DBTable );
	bool	AlterTable( CString strDbName, CString strTableName, CString strSQL1 );
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_RTDB_SQL_RW_H_2005_02_22__
