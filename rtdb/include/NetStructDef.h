/////////////////////////////////////////////////////////////////////////////
// NetStructDef.h : 用于定义一些命令网络所使用的结构体
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_NET_CMD_STRUCT_DEFINE_H_2003_11_12__
#define __AFX_NET_CMD_STRUCT_DEFINE_H_2003_11_12__
/////////////////////////////////////////////////////////////////////////////
// 添加或更新表的若干条记录
struct UPDATE_RECORDS_HEAD
{
	UINT	uDBID;			// 数据库ID
	UINT	uTableID;		// 表ID
	int		iRecordSize;	// 表记录大小
	int		iRecordNum;		// 待插入或更新的表记录个数
};
/////////////////////////////////////////////////////////////////////////////
// 删除表的若干条记录
struct DELETE_RECORDS_HEAD
{
	UINT	uDBID;			// 数据库ID
	UINT	uTableID;		// 表ID
	int		iRecordNum;		// 待插入或更新的表记录个数
};
/////////////////////////////////////////////////////////////////////////////
// 更改表中的多条记录的同一个字段的值
struct UPDATE_FIELD_HEAD
{
	UINT	uDBID;			// 数据库ID
	UINT	uTableID;		// 表的ID
	UINT	uFieldID;		// 字段ID
	INT		iFieldSize;		// 字段大小
	INT		iFieldCount;	// 字段个数
};
/////////////////////////////////////////////////////////////////////////////
// 更改表中的多条记录的多个字段的值(报文头)
struct UPDATE_FIELDS_HEAD
{
	UINT	uDBID;			// 数据库ID
	UINT	uTableID;		// 表的ID
	INT		iFieldCount;	// 字段个数
};
/////////////////////////////////////////////////////////////////////////////
// 更改表中的多条连续记录的同一个字段的值
struct UPDATE_FIELD_AREA_HEAD
{
	UINT	uDBID;			// 数据库ID
	UINT	uTableID;		// 表的ID
	UINT	uFieldID;		// 字段ID
	INT		iFieldSize;		// 字段大小
	INT		iRecordIndex1;	// 起始记录序号
	INT		iRecordIndex2;	// 起始记录序号
};
/////////////////////////////////////////////////////////////////////////////
// 修改表中的一条记录的多列值
struct UPDATE_RECORD_FIELDS_HEAD
{
	UINT	uDBID;			// 数据库ID
	UINT	uTableID;		// 表的ID
	UINT	uRecordID;		// 记录的ID
	INT		iFieldCount;	// 字段的个数
};
/////////////////////////////////////////////////////////////////////////////
// 修改表整整一列的值
struct SET_COLUMN_VALUE_HEAD
{
	UINT	uDBID;			// 数据库ID
	UINT	uTableID;		// 表的ID
	INT		iRecordIndex1;	// 记录序号1
	INT		iRecordIndex2;	// 记录序号2
	INT		iFieldIndex;	// 字段序号
};
/////////////////////////////////////////////////////////////////////////////
// 循环数组添加值
struct PUSH_FIELDS_HEAD
{
	UINT	uDBID;			// 数据库ID
	UINT	uTableID;		// 表的ID
	UINT	uRecordID;		// 记录的ID
	INT		iFieldIndex1;	// 字段序号1
	INT		iFieldIndex2;	// 字段序号2
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_NET_CMD_STRUCT_DEFINE_H_2003_11_12__
