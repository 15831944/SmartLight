/////////////////////////////////////////////////////////////////////////////
// BaseSql_u.h : oracle���ʽӿڻ����ඨ��
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_BASE_RDB_ORACLE_H__
#define __AFX_BASE_RDB_ORACLE_H__

#define	OTL_ORA9I // Compile OTL 4.0/OCI9i
#include <otlv4.h>
#include <iostream.h>
#include <CString.h>
#include "gSystemDef.h"
/////////////////////////////////////////////////////////////////////////////
#define _com_error otl_exception
extern void dump_com_error(_com_error & e);
/////////////////////////////////////////////////////////////////////////////
#define DST_NULL		-1// �Ƿ�����Դ
#define DST_SQL_SERVER	0 // SQL-Server
#define DST_ACCESS		1 // access
#define DST_ORACLE		2 // oracle
#define DST_SYBASE		3 // sybase
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CBaseSql
{
public:
	CBaseSql();
	virtual ~CBaseSql();

public:
	void	SetIniFile(const char * cIniFile);
	void	SetDBSet(const char * cDBSet);
	int		InitDB(bool bOnlyOne = false);
	int		GetInitState();
	bool	CloseDB();
	int		GetActiveDBType();

	int		DoSQL(LPCTSTR strSQL,int nTarget = 3);
	int		IsTableExist(const char * cTableName);

protected:
	char	m_strIniFile[200];
	char	m_strDBSet[40];
	int		m_nInitState;

	int		m_nDBType[2];		// ����Դ����(ȡֵΪRDB_XXX)
	char	m_strServer[2][64];	// ����Դ������
	char	m_strUser[2][24];	// ����Դ�û�����
	char	m_strPasswd[2][24];	// ����Դ�û�����

public:
	otl_connect ora_connection[2];
	int		m_nActiveDB;
private:
	void	ReadConfig();
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_BASE_RDB_ORACLE_H__
