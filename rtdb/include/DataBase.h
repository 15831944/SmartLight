/////////////////////////////////////////////////////////////////////////////
// DataBase.h : 用于描述数据库系统中的每一个数据库.							*
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
	CHAR	m_cDBName[24];	// 数据库名字
	CHAR	m_cDBCName[40];	// 数据库中文名字
	UINT	m_uDBID;		// 数据库标志
	UINT	m_uDBType;		// 数据库类型
	INT		m_iTableNumber;	// 数据库中表的个数
	INT		m_iFieldNumber;	// 数据库中字段的个数
	INT		m_iDBSize;		// 数据库大小(共享内存的总大小)
	INT		m_iOffsetTable;	// 所有表信息存放的起始偏移量
	INT		m_iOffsetField;	// 所有字段信息存放的起始偏移量
	INT		m_iOffsetData;	// 所有数据记录存放的起始偏移量
} CDbAttr;
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CDataBase : protected CDbAttr
{
	friend class CMultiDataBase;
public:
	CDataBase();
	~CDataBase();

private:
	BOOL			m_bShareMemory;		// 是否存在共享内存
	CMultiDataBase*	m_pMultiDataBase;	// 指示管理多个数据库的指针
	CShareMemory	m_ShareMemory;		// 共享内存起始地址
	CDBTable *		m_pTable;
	CMutexEx 		m_MutexEx;			//数据库互斥量

public:
	CHAR *		GetDBName();		// 得到数据库名字
	CHAR *		GetDBCName();		// 得到数据库中文名字
	UINT		GetDBID();			// 得到数据库ID
	UINT		GetDBType();		// 得到数据库类型
    INT			GetDBTableNum();	// 得到数据库中表的个数
	INT			GetDBFieldNum();	// 得到数据库中字段的个数
    INT			GetDBSize();		// 得到数据库占用内存大小
    CMutexEx* 	GetMutexEx();		// 获取互斥量

	LPSTR		GetStartAddress( );	// 得到共享内存的起始地址
	CTableAttr*	GetTableAddress( );	// 得到共享内存中表对象的起始地址
	CFieldAttr*	GetFieldAddress( );	// 得到共享内存中字段对象的起始地址

	void		SetDBName(CHAR * pstrDBName);	// 设置数据库名字	
	void		SetDBCName(CHAR * pstrDBCName);	// 设置数据库中文名字
	void		SetDBID(UINT uDBID);			// 设置数据库ID
	void		SetDBType(UINT uDBType);		// 设置数据库类型
    void		SetDBTableNum(INT iDBTableNum);	// 设置数据库中表的个数
	void		SetDBFieldNum(INT iDBFieldNum);	// 设置数据库中字段的个数
    void		SetDBSize(INT iSize);			// 设置数据库占用内存大小

	//重载 "=" 运算符
	CDataBase&  operator=(CDataBase& Val);
	CDataBase&  operator=(CDbAttr& Val);

public:
	CMultiDataBase * GetMultiDataBase()	{ return m_pMultiDataBase; }
	BOOL	AttachShareMemory( );
	BOOL	DetachShareMemory( );
	BOOL	IsInShareMemory( )			{ return m_bShareMemory; }

	// 根据表的名字得到一个表对象
    BOOL	GetDBTableFromName( CDBTable& DBTable, CHAR * pstrTableName );
	// 根据表的中文名字得到一个表对象
    BOOL	GetDBTableFromCName( CDBTable& DBTable, CHAR * pstrTableCName );
	// 根据表的ID得到一个表对象
    BOOL	GetDBTableFromID( CDBTable& DBTable, UINT uTableID );
	// 根据表在共享内存的索引得到一个表对象
    BOOL	GetDBTableFromIndex( CDBTable& DBTable, INT iTableIndex );

	// 判断数据库中是否有表对象
	BOOL	IfHasTable( );
	// 得到数据库中第一个表对象位置
	int		GetFirstTablePostion( );
	// 得到数据库中下一个表对象位置
	bool	GetNextTable( CDBTable & DBTable, int & iIndex );

	// 保存实时库数据
	bool	SaveData(bool bBroadcast);

	bool	SaveSnapshot(const char * pszFilePath);
	bool	LoadSnapshot(const char * pszFilePath,bool bBroadCast = true);

	// 对数据库对象的成员变量做字节顺序转换
	void	ByteOrderChange_Member( );
	// 对数据库中指定表的数据做字节顺序转换
	void	ByteOrderChange_TableData( UINT uTableID );

	BOOL	BuildIncludeFile(LPSTR szPasPath);	// 创建数据库描述文件
	BOOL	CreateTableMemory( );				// 分配表空间
	BOOL	ReleaseTableMemory( );				// 释放已分配表空间

public:
	// 直接获取指定的表的数据起始地址
	CHAR *	GetTableDataAddress(UINT uTableID);
	CHAR *	GetTableDataAddressAt(INT nTableIndex);
	CHAR *	GetTableDataAddress(char * pszTableName);
	// 直接获取表的记录数
	INT		GetTableRecordCount(UINT uTableID);
	// 存取指定ID的表的指定记录ID32指定序号的字段的值
	BOOL	GetTableFieldData(UINT uTableID, INT iFieldIndex, UINT uRecordID, CHAR * pData);
	BOOL	SetTableFieldData(UINT uTableID, INT iFieldIndex, UINT uRecordID, const CHAR * pData, BOOL bBroadcast=TRUE);
	// 直接存取指定ID的表的记录数据
	CHAR *	GetTableRecordAt(UINT uTableID, INT iRecordIndex);
	CHAR *	GetTableRecord(UINT uTableID, UINT uRecordID);
	BOOL	SetTableRecord(UINT uTableID, CHAR * pRecord, BOOL bBroadcast = TRUE);

private:
	BOOL	InitAddTable( INT iIndex, CDBTable& DBTable );
	BOOL	InitAddField( CDBField& DBField );

	// 创建共享内存
	BOOL	CreateMem( );
	BOOL	CreateEmptyMem( INT iSize );
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_DATABASE_H_2002_08_25__
