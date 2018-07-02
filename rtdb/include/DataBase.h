/////////////////////////////////////////////////////////////////////////////
// DataBase.h : �����������ݿ�ϵͳ�е�ÿһ�����ݿ�.							*
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*
// version : 2.0; created : 2002-08-25;	last modified : 2004-11-03			*
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_DATABASE_H_2002_08_25__
#define __AFX_DATABASE_H_2002_08_25__

#include "DBTable.h"
#include "ShareMemory.h"
#include "MutexEx.h"

/////////////////////////////////////////////////////////////////////////////
typedef struct _db_attr
{
	CHAR	m_cDBName[24];	// ���ݿ�����
	CHAR	m_cDBCName[40];	// ���ݿ���������
	UINT	m_uDBID;		// ���ݿ��־
	UINT	m_uDBType;		// ���ݿ�����
	INT		m_iTableNumber;	// ���ݿ��б�ĸ���
	INT		m_iFieldNumber;	// ���ݿ����ֶεĸ���
	INT		m_iDBSize;		// ���ݿ��С(�����ڴ���ܴ�С)
	INT		m_iOffsetTable;	// ���б���Ϣ��ŵ���ʼƫ����
	INT		m_iOffsetField;	// �����ֶ���Ϣ��ŵ���ʼƫ����
	INT		m_iOffsetData;	// �������ݼ�¼��ŵ���ʼƫ����
} CDbAttr;
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CDataBase : protected CDbAttr
{
	friend class CMultiDataBase;
public:
	CDataBase();
	~CDataBase();

private:
	BOOL			m_bShareMemory;		// �Ƿ���ڹ����ڴ�
	CMultiDataBase*	m_pMultiDataBase;	// ָʾ���������ݿ��ָ��
	CShareMemory	m_ShareMemory;		// �����ڴ���ʼ��ַ
	CDBTable *		m_pTable;
	CMutexEx 		m_MutexEx;			//���ݿ⻥����

public:
	CHAR *		GetDBName();		// �õ����ݿ�����
	CHAR *		GetDBCName();		// �õ����ݿ���������
	UINT		GetDBID();			// �õ����ݿ�ID
	UINT		GetDBType();		// �õ����ݿ�����
    INT			GetDBTableNum();	// �õ����ݿ��б�ĸ���
	INT			GetDBFieldNum();	// �õ����ݿ����ֶεĸ���
    INT			GetDBSize();		// �õ����ݿ�ռ���ڴ��С
    CMutexEx* 	GetMutexEx();		// ��ȡ������

	LPSTR		GetStartAddress( );	// �õ������ڴ����ʼ��ַ
	CTableAttr*	GetTableAddress( );	// �õ������ڴ��б�������ʼ��ַ
	CFieldAttr*	GetFieldAddress( );	// �õ������ڴ����ֶζ������ʼ��ַ

	void		SetDBName(CHAR * pstrDBName);	// �������ݿ�����	
	void		SetDBCName(CHAR * pstrDBCName);	// �������ݿ���������
	void		SetDBID(UINT uDBID);			// �������ݿ�ID
	void		SetDBType(UINT uDBType);		// �������ݿ�����
    void		SetDBTableNum(INT iDBTableNum);	// �������ݿ��б�ĸ���
	void		SetDBFieldNum(INT iDBFieldNum);	// �������ݿ����ֶεĸ���
    void		SetDBSize(INT iSize);			// �������ݿ�ռ���ڴ��С

	//���� "=" �����
	CDataBase&  operator=(CDataBase& Val);
	CDataBase&  operator=(CDbAttr& Val);

public:
	CMultiDataBase * GetMultiDataBase()	{ return m_pMultiDataBase; }
	BOOL	AttachShareMemory( );
	BOOL	DetachShareMemory( );
	BOOL	IsInShareMemory( )			{ return m_bShareMemory; }

	// ���ݱ�����ֵõ�һ�������
    BOOL	GetDBTableFromName( CDBTable& DBTable, CHAR * pstrTableName );
	// ���ݱ���������ֵõ�һ�������
    BOOL	GetDBTableFromCName( CDBTable& DBTable, CHAR * pstrTableCName );
	// ���ݱ��ID�õ�һ�������
    BOOL	GetDBTableFromID( CDBTable& DBTable, UINT uTableID );
	// ���ݱ��ڹ����ڴ�������õ�һ�������
    BOOL	GetDBTableFromIndex( CDBTable& DBTable, INT iTableIndex );

	// �ж����ݿ����Ƿ��б����
	BOOL	IfHasTable( );
	// �õ����ݿ��е�һ�������λ��
	int		GetFirstTablePostion( );
	// �õ����ݿ�����һ�������λ��
	bool	GetNextTable( CDBTable & DBTable, int & iIndex );

	// ����ʵʱ������
	bool	SaveData(bool bBroadcast);

	bool	SaveSnapshot(const char * pszFilePath);
	bool	LoadSnapshot(const char * pszFilePath,bool bBroadCast = true);

	// �����ݿ����ĳ�Ա�������ֽ�˳��ת��
	void	ByteOrderChange_Member( );
	// �����ݿ���ָ������������ֽ�˳��ת��
	void	ByteOrderChange_TableData( UINT uTableID );

	BOOL	BuildIncludeFile(LPSTR szPasPath);	// �������ݿ������ļ�
	BOOL	CreateTableMemory( );				// �����ռ�
	BOOL	ReleaseTableMemory( );				// �ͷ��ѷ����ռ�

public:
	// ֱ�ӻ�ȡָ���ı��������ʼ��ַ
	CHAR *	GetTableDataAddress(UINT uTableID);
	CHAR *	GetTableDataAddressAt(INT nTableIndex);
	CHAR *	GetTableDataAddress(char * pszTableName);
	// ֱ�ӻ�ȡ��ļ�¼��
	INT		GetTableRecordCount(UINT uTableID);
	// ��ȡָ��ID�ı��ָ����¼ID32ָ����ŵ��ֶε�ֵ
	BOOL	GetTableFieldData(UINT uTableID, INT iFieldIndex, UINT uRecordID, CHAR * pData);
	BOOL	SetTableFieldData(UINT uTableID, INT iFieldIndex, UINT uRecordID, const CHAR * pData, BOOL bBroadcast=TRUE);
	// ֱ�Ӵ�ȡָ��ID�ı�ļ�¼����
	CHAR *	GetTableRecordAt(UINT uTableID, INT iRecordIndex);
	CHAR *	GetTableRecord(UINT uTableID, UINT uRecordID);
	BOOL	SetTableRecord(UINT uTableID, CHAR * pRecord, BOOL bBroadcast = TRUE);

private:
	BOOL	InitAddTable( INT iIndex, CDBTable& DBTable );
	BOOL	InitAddField( CDBField& DBField );

	// ���������ڴ�
	BOOL	CreateMem( );
	BOOL	CreateEmptyMem( INT iSize );
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_DATABASE_H_2002_08_25__
