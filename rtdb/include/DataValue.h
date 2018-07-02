/****************************************************************************
 * DataValue.h : ������CDataValue�Ը������͵����ݽ��з�װ.					*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*
 * ��ע: ��������ģ��򻯴���,���д�С�Ƚ�ʱ�ɵ��ô����ж������Ƿ�һ�½Ϻ���*
 ****************************************************************************/
#ifndef __AFX_ALL_TYPE_DATA_VALUE_DEFINE_H_2002_08_16__
#define __AFX_ALL_TYPE_DATA_VALUE_DEFINE_H_2002_08_16__

#include "Rtdb_Struct.h"
/////////////////////////////////////////////////////////////////////////////
//�������Ͷ���
#define DATA_TYPE_NULL			0
#define DATA_TYPE_CHAR			1
#define DATA_TYPE_UCHAR			2
//16λ����
#define DATA_TYPE_SHORT			3
#define DATA_TYPE_USHORT		4
//32λ����
#define DATA_TYPE_INT			5
#define DATA_TYPE_UINT			6
//����
#define DATA_TYPE_LONG			9
#define DATA_TYPE_ULONG			10
//��������
#define DATA_TYPE_FLOAT			11
#define DATA_TYPE_DOUBLE		12
//�ַ�������
#define DATA_TYPE_STRING		13
//ʱ������
#define DATA_TYPE_TIME			14
/////////////////////////////////////////////////////////////////////////////
// �ֶ���󳤶�(�ַ���)
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
// ���캯������������
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

// ��Ա����
protected:
	DataValueUnion	m_Value;	//���ݵ�Ԫ
	WORD			m_wType;	//��������

// ���� "=" �����
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

// ����ת��
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

// ���� "==" �����
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

// ���� "<=" �����
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

// ���� ">=" �����
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

// ���� ">" �����
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

// ���� "<" �����
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

// ���� "!=" ��x���
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
	// ���� "()" �����,ֻ���ַ���������Ч
	CHAR & operator()(INT iPosition);

	DataValueUnion& GetValue()	{ return m_Value; };
    WORD  GetType()				{ return m_wType; };

	void  GetDisplayText(CHAR * cText);
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_ALL_TYPE_DATA_VALUE_DEFINE_H_2002_08_16__
