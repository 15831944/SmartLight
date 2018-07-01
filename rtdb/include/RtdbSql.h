/////////////////////////////////////////////////////////////////////////////
// RtdbSql.h : 实现实时库与关系库之间的读写操作
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
	// 保存所有数据库的所有表的数据(自动先删除表数据)
	bool	SaveAllData(CMultiDataBase * pMDB);
	// 保存某个数据库的所有表的数据(自动先删除表数据)
	bool	SaveDbData(CDataBase& DataBase);
	// 保存某个表的数据(自动先删除表数据)
	bool	SaveTableData(CDataBase& db, CDBTable& tab);
	// 保存某个视图的数据(自动先删除该视图的数据)
	bool	SaveViewData(CDataBase & db, CDBTable& tab,char* pszFieldCName,int nFieldVal);

	// 三个针对逐个记录处理的函数
	bool	InsertRecord(CDataBase& db, CDBTable& tab, UINT nID);
	bool	DeleteRecord(CDataBase& db, CDBTable& tab, UINT nID);
	bool	UpdateRecord(CDataBase& db, CDBTable& tab, UINT nID);

	// 载入所有数据库的所有表的数据
	bool	LoadAllData(CMultiDataBase * pMDB);
	// 载入某个数据库的所有表的数据
	bool	LoadDbData(CDataBase& DataBase);
	// 载入某个表的数据
	bool	LoadTableData(CDataBase& db, CDBTable& tab);

	// 向商用库中创建实时库表(使用前提:先以文件方式启动实时库)
	bool	CreateAllTable(CMultiDataBase * pMDB);
	
	CString FormSQLStr( CDataBase& DataBase, CDBTable&	DBTable );
	bool	AlterTable( CString strDbName, CString strTableName, CString strSQL1 );
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_RTDB_SQL_RW_H_2005_02_22__
