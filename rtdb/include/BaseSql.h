/////////////////////////////////////////////////////////////////////////////
// BaseSql.h: ����һ����װ�˱�׼SQL���ʹ�ϵ���ݿ�Ļ����ӿڵ���CBaseSql
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_BASE_SQL_EX_H__2004_03_15__
#define __AFX_BASE_SQL_EX_H__2004_03_15__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import	 "msado20.tlb" rename("EOF", "adoEOF") rename("BOF", "adoBOF")
#include <icrsint.h>
/////////////////////////////////////////////////////////////////////////////
#define DST_NULL		-1// �Ƿ�����Դ
#define DST_SQL_SERVER	0 // SQL-Server
#define DST_ACCESS		1 // access
#define DST_ORACLE		2 // oracle
#define DST_SYBASE		3 // sybase
#define DST_PGSQL		4 // PGSql
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CBaseSql
{
public:
	CBaseSql();
	virtual ~CBaseSql();

public:
	void	SetIniFile(const char * cIniFile);
	void	SetDBSet(const char * cDBSet);
	//bCreateDBΪ�������ݿ�ʹ��.
	int		InitDB(bool bOnlyOne = false, bool bCreateDB=false);
	int		GetInitState();
	bool	CloseDB();
	int		GetActiveDBType();

	int		DoSQL(LPCTSTR strSQL,int nTarget = 3);
	int		IsTableExist(const char * cTableName);

public:
	bool		Query(LPCTSTR strSQL);
	void		GotoFirstRecord();
	void		GotoNextRecord();
	int			GetRecordCount();
	bool		IsEndOfRecordset();
	_variant_t	GetItemValue(long nIndex);
	_variant_t	GetItemValue(char * pFieldName);

	bool	CloseRecordSet();
	ADODB::_ConnectionPtr	GetActiveConnectionPtr();
	ADODB::_CommandPtr		GetActiveCommandPtr();
	ADODB::_RecordsetPtr	GetActiveRecordsetPtr();

protected:
	char	m_strIniFile[200];
	char	m_strDBSet[40];
	int		m_nInitState;
	int		m_nActiveDB;

	int		m_nDBType[2];		// ����Դ����(ȡֵΪRDB_XXX)
	char	m_strServer[2][64];	// ����Դ������
	char	m_strUser[2][24];	// ����Դ�û�����
	char	m_strPasswd[2][24];	// ����Դ�û�����
	char	m_strInitDB[2][24];	// ��ʼ�������ݿ������(������SQL-Server��SyBase������)

protected:
	bool	m_bOleInit;
	ADODB::_ConnectionPtr	m_pConnection[2];
	ADODB::_CommandPtr		m_pCommand[2];
	ADODB::_RecordsetPtr	m_pRecordset[2];

private:
	void	ReadConfig();
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_BASE_SQL_EX_H__2004_03_15__
