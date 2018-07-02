/////////////////////////////////////////////////////////////////////////////
// DBField.h : �����������ݿ��б���ֶεĶ������Ժ�ȡֵ.					*
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*
// version : 1.8; created : 2002-08-17; last modified: 2004-05-29			*
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_DATABASE_TABLE_FIELD_H_2002_08_17__
#define __AFX_DATABASE_TABLE_FIELD_H_2002_08_17__

#include "Rtdb_Struct.h"
#define  FIELD_FLAG_STATIC 0 //��̬������,����������Ϣ ������Ϣ��
#define  FIELD_FLAG_DYNAMIC 1 //��̬���� ,ʵʱ�仯������,�����ʶֵ��
/////////////////////////////////////////////////////////////////////////////
typedef struct _field_attr
{
	CHAR	m_cFieldName[24];	// �ֶμ���
	CHAR	m_cFieldCName[40];	// �ֶ�������(����)
	UINT	m_uDBID;			// �������ݿ�ID
	UINT	m_uTableID;			// ������ID
	UINT	m_uFieldID;			// �ֶα�ʶ
	INT		m_iIndexInTable;	// �ֶ���������������
	INT		m_iFieldType;		// �ֶ�����
	INT		m_iFieldTypeSize;	// �ֶ����ʹ�С
	INT		m_iFieldSize;		// �ֶδ�С(Ϊ[�ֶ����ʹ�С]��������,���ڻ����������������)
	INT		m_iOffsetInTable;	// �ֶξ��ͷ��ƫ����
	CHAR	m_cDefaultValue[32];// �ֶ�Ĭ��ֵ
//	INT    m_iFlag;//��ʶ 0��̬���� 1��̬����,��������ʵʱ��������,1�������������������,0������,ȫ���ݲŷ��� tjs 2015-11-7
} CFieldAttr;
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CDBField : protected CFieldAttr
{
public:
	CDBField();
	~CDBField();

public:
	UINT		GetDataBaseID();		// ����,�õ����ݿ�����
	UINT		GetTableID();			// ����,�õ��ֶ��������ID
	CHAR *		GetFieldName();			// ����,�õ��ֶε�����
	CHAR *		GetFieldCName();		// ����,�õ��ֶε���������
	UINT		GetFieldID();			// ����,�õ��ֶε�ID
	INT			GetIndexInTable();		// ����,�õ��ֶ����������е��ڲ�����
	INT			GetFieldType();			// ����,�õ��ֶε���������
	INT			GetFieldTypeSize();		// ����,�õ��ֶε����������ֽڴ�С
	INT			GetFieldSize();			// ����,�õ��ֶε��ֽڴ�С
	INT			GetFieldOffset();		// ����,�õ��ֶ��������ṹ�е�ƫ����
	CHAR *		GetTypeStr();			// ����,�õ��ֶε�������������
	CHAR *		GetDefaultValue();		// ����,�õ��ֶε�Ĭ��ֵ
//	INT         GetFlag();              //����,�õ���ʶ 2015-11-7̷��ʤ���

	void		SetDataBaseID(UINT uDataBaseID);	// �ڲ�,�������ݿ�����
	void		SetTableID(UINT uTableID);			// �ڲ�,�����ֶ��������ID
	void		SetFieldName(CHAR * cFieldName);	// �ڲ�,�����ֶε�����
	void		SetFieldCName(CHAR * cFieldCName);	// �ڲ�,�����ֶε���������
	void		SetFieldID(UINT uFieldID);			// �ڲ�,�����ֶε�ID
	void		SetIndexInTable(INT iDBTableIndex);	// �ڲ�,�����ֶ����������е��ڲ�����
	void		SetFieldType(INT iFieldType);		// �ڲ�,�����ֶε���������
	void		SetFieldTypeSize(INT iSize);		// �ڲ�,�����ֶε����������ֽڴ�С
	void		SetFieldSize(INT iFieldSize);		// �ڲ�,�����ֶε��ֽڴ�С
	void		SetFieldOffset(INT iFieldOffset);	// �ڲ�,�����ֶ��������ṹ�е�ƫ����
	void		SetDefaultValue(CHAR * cValue);		// �ڲ�,�����ֶε�Ĭ��ֵ
  //  void        SetFlag(INT iFlag);                  //�ڲ�,�����ֶεı�ʶ 2015-11-7̷��ʤ���
	CDBField &  operator=(CDBField & Val);		// ����,���� "=" ��������ڸ����ֶζ���
	CDBField &  operator=(CFieldAttr& Val);		// ����,���� "=" ��������ڸ����ֶζ���

	void		ByteOrderChange_Member( );	// ����,���ֶζ������ڲ��������ݽ����ֽ�ת��
	void		ByteOrderChange_Field(char * pFieldData);

	// �����ֶ����͵õ������ֶ����ݵ��ַ���
	static void	GetDisplayText(int iType,CHAR * cField,CHAR * cDisplay);
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_DATABASE_TABLE_FIELD_H_2002_08_17__
