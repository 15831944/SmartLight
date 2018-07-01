/****************************************************************************
 * DBTable.h : 用于描述数据库中的表结构和数据记录.							*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*
 * version : 1.8; created : 2002-08-17; last modified: 2004-08-28			*
 ****************************************************************************/
#ifndef __AFX_DATABASE_TABLE_H_2002_08_17__
#define __AFX_DATABASE_TABLE_H_2002_08_17__

#include "DBField.h"
#include "DataValue.h"

#define DB_TABLE_FlAG_PRIVATE 8   //是库中私有表不需要传送
class CDataBase;
/////////////////////////////////////////////////////////////////////////////
typedef struct _table_attr
{
	CHAR		m_cTableName[24];	// 表的简名
	CHAR		m_cTableCName[40];	// 表的中文名
	UINT		m_uDBID;			// 表所属数据库的ID
	UINT		m_uTableID;			// 表的ID
	INT			m_iIndexInDB;		// 表在所属数据库中的序号
	INT			m_iTableSize;		// 表记录的大小
	INT			m_iRecordMaxCount;	// 表记录的最大允许数
	INT			m_iFieldNumber;		// 表中包含的字段的个数
	INT			m_iFieldOffset;		// 共享内存中存放表字段的位置
	INT			m_iDataOffset;		// 共享内存中存放表数据的位置
	INT			m_iRecordCount;		// 表中的实际记录数
	UINT		m_uIDExclusive;		// 待分配给新记录的ID32
	INT        m_uFlag;            //标志 2016-09-08 tjs 增加
} CTableAttr;
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CDBTable : protected CTableAttr
{
	friend class CDataBase;
public:
	CDBTable();
	~CDBTable();

public:
	UINT	GetDataBaseID( );		// 开放,得到数据库类型
	CHAR *	GetTableName( );		// 开放,得到表的名字
	CHAR *	GetTableCName( );		// 开放,得到表的中文名字
	UINT	GetTableID( );			// 开放,得到表的标志ID
	INT		GetIndexInDB( );		// 开放,得到表在所属数据库中索引
	INT		GetTableSize( );		// 开放,得到表记录的大小
	INT		GetRecordMaxCount( );	// 开放,得到表中记录的最大个数
	INT		GetRecordCount( );		// 开放,得到表中记录的实际个数
	INT		GetTableFieldNum( );	// 开放,得到表中字段个数
	UINT	GetIDExclusive( );		// 开放,得到表记录唯一标识

	INT	GetTableFlag();		// 开放,得到表的标志位 2016-09-08 增加


	void	SetTableName(CHAR * cTableName);	// 开放,设置表的名字
	void	SetTableCName(CHAR * cTableCName);	// 开放,设置表的中文名字
	void	SetDataBaseID(UINT uDataBaseID);	// 内部,设置数据库类型
	void	SetTableID(UINT uTableID);			// 内部,设置表的ID
	void	SetIndexInDB(INT iIndex);			// 内部,设置表在所属数据库中索引
	void	SetTableSize(INT iSize);			// 内部,设置表记录的大小
	void	SetRecordMaxCount(INT iMaxNum);		// 内部,设置表中记录的最大个数
	void	SetRecordCount( INT iCount );		// 内部,设置表中记录的实际个数
	void	SetTableFieldNum(INT iFieldNum);	// 内部,设置表中字段个数
	void	SetIDExclusive(UINT uiID);			// 开放,设置表记录唯一标识

	void SetTableFlag(INT uflg);               //开放，设置表的标志位
// 对表字段的处理
public:
	// 根据字段在表中索引得到字段对象
    BOOL	GetFieldFromIndex( CDBField& DBField, INT iFieldIndex );
	// 根据字段的名字得到字段对象
    BOOL	GetFieldFromName( CDBField& DBField, CHAR * cFieldName );
	// 根据字段的中文名字得到字段对象
    BOOL	GetFieldFromCName( CDBField& DBField, CHAR * cFieldCName );
	// 根据字段的ID得到字段对象
    BOOL	GetFieldFromID( CDBField& DBField, UINT uiFieldID );
	// 根据字段的名称查找它在表中的序号
	int		FindFieldFromName(char * cFieldName,bool bCName);

	CFieldAttr * GetFieldFromIndex(INT iFieldIndex);
	CFieldAttr * GetFieldFromID(UINT uFieldID);
	CFieldAttr * GetFieldFromName(CHAR * pName);
	CFieldAttr * GetFieldFromCName(CHAR * pCName);

	// 判断表对象中是否有字段对象
	bool	IfHasField( );
	// 得到表对象中第一个字段对象位置
	int		GetFirstFieldPostion( );
	// 得到表对象中下一个字段对象位置
	bool	GetNextField( CDBField & DBField, int & iIndex );
	// 更新表的指定序号的字段对象
	bool	SetField( CDBField DBField );

public:
	// 重载 "=" 运算符用于复制CDBTable对象
	CDBTable &	operator=(CDBTable & Val);
	CDBTable &	operator=(CTableAttr & Val);
	// 得到共享内存中字段对象的起始地址
	CFieldAttr*	GetFieldAddress( );
	// 取得共享内存中表数据的起始地址
	LPSTR		GetDataAddress( );

// 对表记录的处理
public:
	// 根据ID32查找表中的记录的索引,建议不要使用,以后不再支持
	BOOL	FindIDExclusive( UINT uRecordID, INT & iIndex );
	// 根据ID32查找表中的记录的索引,返回负数表示没有找到
	INT		FindRecordIndex( UINT uRecordID );
	// 根据ID32查找表中的记录
	CHAR *	FindIDExclusive( UINT uRecordID );
	BOOL	FindIDExclusive( UINT uRecordID, void * pVoid );
	// 插入一条记录,返回插入记录的位置序号(返回-2表示记录已存在,返回-1表示其它错误)
	INT		Insert( void * pVoid, BOOL bBroadcast = TRUE, BOOL bSave = FALSE );
	// 在表中增加一新记录,可以指定ID32,若为0则由这里分配,可指定是否要自动分配默认值
	INT		Insert( UINT nID32, BOOL bDefault, BOOL bBroadcast = TRUE );
	void	InitRecordDefault(char * pRecord);
	// 更新一条记录,可选的iIndex用于快速定位,如Index-ID32校验错误就忽略Index
	INT		Update( void * pVoid, INT iIndex = -1, BOOL bBroadcast = TRUE, BOOL bSave = FALSE );
	// 删除一条记录(uRecordID为0则以iIndex为准;否则以uRecordID为准,此时iIndex可填为-1)
	BOOL	Delete( UINT uRecordID, INT iIndex, BOOL bBroadcast = TRUE, BOOL bSave = FALSE );
	// 删除表中的所有记录
	BOOL	DeleteAllRecord( BOOL bBroadcast = TRUE );

	// 将某个表的指定字段序号的一列数据设成同一个值(iRecordIndex2为-1则到列尾)
	BOOL	SetColumnValueAt(INT iFieldIndex,void * pData,INT iRecordIndex1,INT iRecordIndex2,BOOL bBroadcast=TRUE);

	//以下函数是在完成数据修改后调用，将修改的内容告知其它机器
	// 将本地某个表的指定字段序号的一列数据广播出去
	BOOL	UpdateColumnAt(INT iFieldIndex,INT iRecordIndex1,INT iRecordIndex2=-1, BOOL bOnlyWeb=FALSE);
	// 将本地某个表的指定字段ID的一列中某些记录的数据广播出去
	BOOL	UpdateColumnAt(INT iFieldIndex,INT iFieldCount,INT * pRecordIndex);
	// 将本地某个表的指定ID的一条记录广播出去
	BOOL	UpdateRecord(UINT uRecordID);
	// 将本地某个表的指定序号的一条记录广播出去
	BOOL	UpdateRecordAt(INT iRecordIndex);
	// 将本地某个表的指定ID的一条记录的多个字段值广播出去 CMD_UPDATE_RECORD_FIELDS
	BOOL	UpdateRecord(UINT uRecordID,INT nFieldCount,INT * pFieldIndex);
	// 将本表的数据全网广播 CMD_BROADCAST_DATA
	BOOL	Broadcast();

	// Java程序向内存中追加一条记录后通知C/C++
	BOOL	JavaAppend(BOOL bBroadcast = TRUE);
	// Java程序调用C/C++的接口删除一条记录
	BOOL	JavaDelete(UINT uRecordID, INT iIndex, BOOL bBroadcast = TRUE);

	// 将指定记录的nFieldIndex1至nFieldIndex2字段进行前推,然后将pNewItem值赋给nFieldIndex2字段
	BOOL	PushFields(UINT uRecordID,INT nFieldIndex1,INT nFieldIndex2,CHAR * pNewItem,BOOL bBroadcast = TRUE);

	//实现任意记录任意字段的更新 CMD_UPDATE_FIELDS
	//bSendNow一般设置为FALSE,当数据有1024bit后自动发送,在需要及时更新时将bSendNow设置为TRUE
	BOOL	UpdateFields(UINT uRecordID, INT nRecordIndex, INT nFieldIndex, BOOL bSendNow = FALSE);
	//将buffer数据主动发送，在调用完带参数的UpdateFields函数后，需要调用此函数
	//也可以在最后调用UpdateFields函数时，设置bSendNow为TRUE
	BOOL	UpdateFields();

public:
	// 表中是否有记录
	bool	IsEmpty( );
	// 取得表中第一条记录的位置(无记录则返回-1)
	int		GetFirstPosition( );
	// 取得指定位置处的表记录,同时将位置参数指向下一条记录,注意到记录集终点将返回NULL
	void *	GetNextElement( int & iIndex );
	// 取得指定位置处的记录(如果有错就返回NULL)
	CHAR *	GetAt( INT iIndex );
	// 取得指定ID32和/或Index的记录的头地址
	CHAR *	GetRecordAddr(UINT uRecordID,INT iIndex);

public:
	// 得到表中指定序号的记录的指定序号的字段的值
	BOOL	GetFieldDataAt( INT iRecordIndex, INT iFieldIndex, CDataValue& DataValue );
	// 设置表中指定序号的记录的指定序号的字段的值
	BOOL	SetFieldDataAt( INT iRecordIndex, INT iFieldIndex, CDataValue DataValue, BOOL bBroadcast = TRUE );

	// 得到表中指定序号的记录的指定序号的字段的值
	BOOL	GetFieldDataAt( INT iRecordIndex, INT iFieldIndex, void * pVoid );
	// 设置表中指定序号的记录的指定序号的字段的值
	BOOL	SetFieldDataAt( INT iRecordIndex, INT iFieldIndex, void * pVoid, BOOL bBroadcast = TRUE );

	// 得到表中指定ID32的记录的指定序号的字段的值
	BOOL	GetFieldData(UINT uRecordID, INT iFieldIndex, void * pVoid);
	// 设置表中指定ID32的记录的指定序号的字段的值
	BOOL	SetFieldData(UINT uRecordID, INT iFieldIndex, const void * pVoid, BOOL bBroadcast = TRUE);

	// 设置表对象中指定位置的记录对象的指定位置的字段值, 但输入的是该字段值的格式化字符串, 因此需要反格式化
	BOOL	SetFieldByText( INT nRecordIndex, INT nFieldIndex, CHAR * pText, BOOL bBroadcast = TRUE );

public:
	// 对表对象的成员变量做字节顺序转换
	void	ByteOrderChange_Member( );
	// 对表对象中指定位置的记录的所有字段数据进行字节转换
	void	ByteOrderChange_Record(int iRecordIndex,bool bChangeID32=true);
	// 对指定buffer按照表字段数据进行字节转换
	void	ByteOrderChange_Record(char *pRecordBuffer);
	// 处理在CCmdProcess中直接修改实时库的情况
	void	SendDataToWebServer(UINT nCmd, char *Buffer, int iSize);
	void	MapUpdate( UINT* uRecordID, INT *iFieldIndex, INT iCount );
public:
	// 将表数据保存到数据源中(自动识别数据源是文件还是数据库)
	bool	SaveData(bool bBroadcast);

	CTableAttr *	GetTableInfo( );

private:
	void	SetDataBase(CDataBase * pDataBase);
	bool	IsInShareMemory( );

	BOOL	CreateFieldMemory();
	BOOL	ReleaseFieldMemory();

	INT		InitAddField( CDBField& DBField );

	// 设置共享内存中表对象的偏移量
	void	SetDataOffset(INT iOffset);
	// 设置共享内存中字段对象索引的偏移量
	void	SetFieldOffset(INT iOffset);

	void	MapChangeNotify(UINT uType,UINT uDbID,UINT uTabID,UINT uRecordID,BOOL bBroadcast);

	//以下函数将实时库数据指定记录保存到永久介质中
	bool	InsertRecordTo( UINT nID );
	bool	UpdateRecordTo( UINT nID );
	bool	DeleteRecordTo( UINT nID );
	bool	InsertProcessForNode(UINT uNodeID);
	bool	DeleteProcessForNode(UINT uNodeID);
	bool	HasActiveProcessForNode(UINT uNodeID);

private:
	CDBField *	m_pField;		// 私有临时变量
	CDataBase *	m_pDataBase;	// 所属数据库的指针

	//UpdateFields函数使用
	CHAR		m_UpdateFieldsBuffer[CMD_MAX_LENGTH];
	INT			m_iTotalLen;

};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_DATABASE_TABLE_H_2002_08_17__
