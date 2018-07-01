/****************************************************************************
 * DataValue.h : 定义类CDataValue对各种类型的数据进行封装.					*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*
 * 备注: 还可利用模板简化代码,进行大小比较时由调用处先判断类型是否一致较合理*
 ****************************************************************************/
#ifndef __AFX_ALL_TYPE_DATA_VALUE_DEFINE_H_2002_08_16__
#define __AFX_ALL_TYPE_DATA_VALUE_DEFINE_H_2002_08_16__

#include "Rtdb_Struct.h"
/////////////////////////////////////////////////////////////////////////////
//数据类型定义
#define DATA_TYPE_NULL			0
#define DATA_TYPE_CHAR			1
#define DATA_TYPE_UCHAR			2
//16位整数
#define DATA_TYPE_SHORT			3
#define DATA_TYPE_USHORT		4
//32位整数
#define DATA_TYPE_INT			5
#define DATA_TYPE_UINT			6
//其它
#define DATA_TYPE_LONG			9
#define DATA_TYPE_ULONG			10
//浮点类型
#define DATA_TYPE_FLOAT			11
#define DATA_TYPE_DOUBLE		12
//字符串类型
#define DATA_TYPE_STRING		13
//时间类型
#define DATA_TYPE_TIME			14
/////////////////////////////////////////////////////////////////////////////
// 字段最大长度(字符串)
#define FIELD_MAX_LENGTH		600
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CDataValue
{
public:
	union DataValueUnion 
	{
		CHAR		cValue;
		UCHAR		ucValue;
		SHORT		siValue;
		USHORT		usiValue;
		INT         iValue;
		UINT        uiValue;
		LONG        lValue;
		ULONG       ulValue;
		FLOAT		fValue;
		DOUBLE      dValue;
		CHAR		strValue[FIELD_MAX_LENGTH];
		SYSTEMTIME	tValue;
	};
// 构造函数和析构函数
public:
	CDataValue();
	CDataValue(CHAR	  cVal);
	CDataValue(UCHAR  ucValue);
	CDataValue(SHORT  siVal);
	CDataValue(USHORT usiVal);
	CDataValue(INT	  iVal);
	CDataValue(UINT	  uiVal);
	CDataValue(LONG	  lVal);
	CDataValue(ULONG  ulVal);
	CDataValue(FLOAT  fVal);
	CDataValue(DOUBLE dVal);
	CDataValue(CHAR * szVal);
	CDataValue(SYSTEMTIME tVal);
	CDataValue(CDataValue & Val);
    ~CDataValue();

// 成员变量
protected:
	DataValueUnion	m_Value;	//数据单元
	WORD			m_wType;	//数据类型

// 重载 "=" 运算符
public:
	CDataValue & operator=(CHAR	  cVal);
	CDataValue & operator=(UCHAR  ucVal);
	CDataValue & operator=(SHORT  siVal);
	CDataValue & operator=(USHORT usiVal);
	CDataValue & operator=(INT	  iVal);
	CDataValue & operator=(UINT	  uiVal);
	CDataValue & operator=(LONG	  lVal);
	CDataValue & operator=(ULONG  ulVal);
	CDataValue & operator=(FLOAT  fVal);
	CDataValue & operator=(DOUBLE dVal);
	CDataValue & operator=(CHAR * szVal);
	CDataValue & operator=(SYSTEMTIME tVal);
	CDataValue & operator=(CDataValue & Val);

// 类型转换
public:
	CHAR	GetChar( );
	UCHAR	GetUChar( );
	SHORT	GetShort( );
	USHORT	GetUShort( );
	INT		GetInt( );
	UINT	GetUInt( );
	LONG	GetLong( );
	ULONG	GetULong( );
	FLOAT	GetFloat( );
	DOUBLE	GetDouble( );
	CHAR *	GetString( );
	SYSTEMTIME	GetTime( );

// 重载 "==" 运算符
public:
	BOOL operator==(CHAR   cVal);
	BOOL operator==(UCHAR  ucVal);
	BOOL operator==(SHORT  siVal);
	BOOL operator==(USHORT usiVal);
	BOOL operator==(INT	   iVal);	
	BOOL operator==(UINT   uiVal);	
	BOOL operator==(LONG   lVal);
	BOOL operator==(ULONG  ulVal);
	BOOL operator==(FLOAT  rVal);
	BOOL operator==(DOUBLE dVal);
	BOOL operator==(CHAR * szVal);
	BOOL operator==(SYSTEMTIME tVal);
	BOOL operator==(CDataValue & Val);

// 重载 "<=" 运算符
public:
	BOOL operator<=(CHAR   cVal);
	BOOL operator<=(UCHAR  ucVal);
	BOOL operator<=(SHORT  siVal);
	BOOL operator<=(USHORT usiVal);
	BOOL operator<=(INT	   iVal);	
	BOOL operator<=(UINT   uiVal);	
	BOOL operator<=(LONG   lVal);
	BOOL operator<=(ULONG  ulVal);
	BOOL operator<=(FLOAT  rVal);
	BOOL operator<=(DOUBLE dVal);
	BOOL operator<=(CHAR * szVal);
	BOOL operator<=(SYSTEMTIME tVal);
	BOOL operator<=(CDataValue & Val);

// 重载 ">=" 运算符
public:
	BOOL operator>=(CHAR   cVal);
	BOOL operator>=(UCHAR  ucVal);
	BOOL operator>=(SHORT  siVal);
	BOOL operator>=(USHORT usiVal);
	BOOL operator>=(INT	   iVal);	
	BOOL operator>=(UINT   uiVal);	
	BOOL operator>=(LONG   lVal);
	BOOL operator>=(ULONG  ulVal);
	BOOL operator>=(FLOAT  rVal);
	BOOL operator>=(DOUBLE dVal);
	BOOL operator>=(CHAR * szVal);
	BOOL operator>=(SYSTEMTIME tVal);
	BOOL operator>=(CDataValue & Val);

// 重载 ">" 运算符
public:
	BOOL operator>(CHAR   cVal);
	BOOL operator>(UCHAR  ucVal);
	BOOL operator>(SHORT  siVal);
	BOOL operator>(USHORT usiVal);
	BOOL operator>(INT	   iVal);	
	BOOL operator>(UINT   uiVal);	
	BOOL operator>(LONG   lVal);
	BOOL operator>(ULONG  ulVal);
	BOOL operator>(FLOAT  rVal);
	BOOL operator>(DOUBLE dVal);
	BOOL operator>(CHAR * szVal);
	BOOL operator>(SYSTEMTIME tVal);
	BOOL operator>(CDataValue & Val);

// 重载 "<" 运算符
public:
	BOOL operator<(CHAR   cVal);
	BOOL operator<(UCHAR  ucVal);
	BOOL operator<(SHORT  siVal);
	BOOL operator<(USHORT usiVal);
	BOOL operator<(INT	   iVal);	
	BOOL operator<(UINT   uiVal);	
	BOOL operator<(LONG   lVal);
	BOOL operator<(ULONG  ulVal);
	BOOL operator<(FLOAT  rVal);
	BOOL operator<(DOUBLE dVal);
	BOOL operator<(CHAR * szVal);
	BOOL operator<(SYSTEMTIME tVal);
	BOOL operator<(CDataValue & Val);

// 重载 "!=" 运x算符
public:
	BOOL operator!=(CHAR   cVal);
	BOOL operator!=(UCHAR  ucVal);
	BOOL operator!=(SHORT  siVal);
	BOOL operator!=(USHORT usiVal);
	BOOL operator!=(INT	   iVal);	
	BOOL operator!=(UINT   uiVal);	
	BOOL operator!=(LONG   lVal);
	BOOL operator!=(ULONG  ulVal);
	BOOL operator!=(FLOAT  rVal);
	BOOL operator!=(DOUBLE dVal);
	BOOL operator!=(CHAR * szVal);
	BOOL operator!=(SYSTEMTIME   tVal);
	BOOL operator!=(CDataValue & Val);

public:
	// 重载 "()" 运算符,只对字符串类型有效
	CHAR & operator()(INT iPosition);

	DataValueUnion& GetValue()	{ return m_Value; };
    WORD  GetType()				{ return m_wType; };

	void  GetDisplayText(CHAR * cText);
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_ALL_TYPE_DATA_VALUE_DEFINE_H_2002_08_16__
