/////////////////////////////////////////////////////////////////////////////
// DBField.h : 用于描述数据库中表的字段的定义属性和取值.					*
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*
// version : 1.8; created : 2002-08-17; last modified: 2004-05-29			*
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_DATABASE_TABLE_FIELD_H_2002_08_17__
#define __AFX_DATABASE_TABLE_FIELD_H_2002_08_17__

#include "Rtdb_Struct.h"
#define  FIELD_FLAG_STATIC 0 //静态的数据,例如描述信息 配置信息等
#define  FIELD_FLAG_DYNAMIC 1 //动态数据 ,实时变化的数据,例如标识值等
/////////////////////////////////////////////////////////////////////////////
typedef struct _field_attr
{
	CHAR	m_cFieldName[24];	// 字段简名
	CHAR	m_cFieldCName[40];	// 字段中文名(描述)
	UINT	m_uDBID;			// 所属数据库ID
	UINT	m_uTableID;			// 所属表ID
	UINT	m_uFieldID;			// 字段标识
	INT		m_iIndexInTable;	// 字段在所属表中索引
	INT		m_iFieldType;		// 字段类型
	INT		m_iFieldTypeSize;	// 字段类型大小
	INT		m_iFieldSize;		// 字段大小(为[字段类型大小]的整数倍,对于基本数据类型则相等)
	INT		m_iOffsetInTable;	// 字段距表头的偏移量
	CHAR	m_cDefaultValue[32];// 字段默认值
//	INT    m_iFlag;//标识 0静态数据 1动态数据,用以区分实时传送数据,1的数据请求后立即返回,0的数据,全数据才返回 tjs 2015-11-7
} CFieldAttr;
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CDBField : protected CFieldAttr
{
public:
	CDBField();
	~CDBField();

public:
	UINT		GetDataBaseID();		// 开放,得到数据库类型
	UINT		GetTableID();			// 开放,得到字段所属表的ID
	CHAR *		GetFieldName();			// 开放,得到字段的名字
	CHAR *		GetFieldCName();		// 开放,得到字段的中文名字
	UINT		GetFieldID();			// 开放,得到字段的ID
	INT			GetIndexInTable();		// 开放,得到字段在所属表中的内部索引
	INT			GetFieldType();			// 开放,得到字段的数据类型
	INT			GetFieldTypeSize();		// 开放,得到字段的数据类型字节大小
	INT			GetFieldSize();			// 开放,得到字段的字节大小
	INT			GetFieldOffset();		// 开放,得到字段在所属结构中的偏移量
	CHAR *		GetTypeStr();			// 开放,得到字段的数据类型描述
	CHAR *		GetDefaultValue();		// 开放,得到字段的默认值
//	INT         GetFlag();              //开放,得到标识 2015-11-7谭井胜添加

	void		SetDataBaseID(UINT uDataBaseID);	// 内部,设置数据库类型
	void		SetTableID(UINT uTableID);			// 内部,设置字段所属表的ID
	void		SetFieldName(CHAR * cFieldName);	// 内部,设置字段的名字
	void		SetFieldCName(CHAR * cFieldCName);	// 内部,设置字段的中文名字
	void		SetFieldID(UINT uFieldID);			// 内部,设置字段的ID
	void		SetIndexInTable(INT iDBTableIndex);	// 内部,设置字段在所属表中的内部索引
	void		SetFieldType(INT iFieldType);		// 内部,设置字段的数据类型
	void		SetFieldTypeSize(INT iSize);		// 内部,设置字段的数据类型字节大小
	void		SetFieldSize(INT iFieldSize);		// 内部,设置字段的字节大小
	void		SetFieldOffset(INT iFieldOffset);	// 内部,设置字段在所属结构中的偏移量
	void		SetDefaultValue(CHAR * cValue);		// 内部,设置字段的默认值
  //  void        SetFlag(INT iFlag);                  //内部,设置字段的标识 2015-11-7谭井胜添加
	CDBField &  operator=(CDBField & Val);		// 开放,重载 "=" 运算符用于复制字段对象
	CDBField &  operator=(CFieldAttr& Val);		// 开放,重载 "=" 运算符用于复制字段对象

	void		ByteOrderChange_Member( );	// 开放,对字段对象中内部属性数据进行字节转换
	void		ByteOrderChange_Field(char * pFieldData);

	// 根据字段类型得到所给字段数据的字符串
	static void	GetDisplayText(int iType,CHAR * cField,CHAR * cDisplay);
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_DATABASE_TABLE_FIELD_H_2002_08_17__
