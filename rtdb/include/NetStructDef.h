/////////////////////////////////////////////////////////////////////////////
// NetStructDef.h : ���ڶ���һЩ����������ʹ�õĽṹ��
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_NET_CMD_STRUCT_DEFINE_H_2003_11_12__
#define __AFX_NET_CMD_STRUCT_DEFINE_H_2003_11_12__
/////////////////////////////////////////////////////////////////////////////
// ��ӻ���±����������¼
struct UPDATE_RECORDS_HEAD
{
	UINT	uDBID;			// ���ݿ�ID
	UINT	uTableID;		// ��ID
	int		iRecordSize;	// ���¼��С
	int		iRecordNum;		// ���������µı��¼����
};
/////////////////////////////////////////////////////////////////////////////
// ɾ�������������¼
struct DELETE_RECORDS_HEAD
{
	UINT	uDBID;			// ���ݿ�ID
	UINT	uTableID;		// ��ID
	int		iRecordNum;		// ���������µı��¼����
};
/////////////////////////////////////////////////////////////////////////////
// ���ı��еĶ�����¼��ͬһ���ֶε�ֵ
struct UPDATE_FIELD_HEAD
{
	UINT	uDBID;			// ���ݿ�ID
	UINT	uTableID;		// ���ID
	UINT	uFieldID;		// �ֶ�ID
	INT		iFieldSize;		// �ֶδ�С
	INT		iFieldCount;	// �ֶθ���
};
/////////////////////////////////////////////////////////////////////////////
// ���ı��еĶ�����¼�Ķ���ֶε�ֵ(����ͷ)
struct UPDATE_FIELDS_HEAD
{
	UINT	uDBID;			// ���ݿ�ID
	UINT	uTableID;		// ���ID
	INT		iFieldCount;	// �ֶθ���
};
/////////////////////////////////////////////////////////////////////////////
// ���ı��еĶ���������¼��ͬһ���ֶε�ֵ
struct UPDATE_FIELD_AREA_HEAD
{
	UINT	uDBID;			// ���ݿ�ID
	UINT	uTableID;		// ���ID
	UINT	uFieldID;		// �ֶ�ID
	INT		iFieldSize;		// �ֶδ�С
	INT		iRecordIndex1;	// ��ʼ��¼���
	INT		iRecordIndex2;	// ��ʼ��¼���
};
/////////////////////////////////////////////////////////////////////////////
// �޸ı��е�һ����¼�Ķ���ֵ
struct UPDATE_RECORD_FIELDS_HEAD
{
	UINT	uDBID;			// ���ݿ�ID
	UINT	uTableID;		// ���ID
	UINT	uRecordID;		// ��¼��ID
	INT		iFieldCount;	// �ֶεĸ���
};
/////////////////////////////////////////////////////////////////////////////
// �޸ı�����һ�е�ֵ
struct SET_COLUMN_VALUE_HEAD
{
	UINT	uDBID;			// ���ݿ�ID
	UINT	uTableID;		// ���ID
	INT		iRecordIndex1;	// ��¼���1
	INT		iRecordIndex2;	// ��¼���2
	INT		iFieldIndex;	// �ֶ����
};
/////////////////////////////////////////////////////////////////////////////
// ѭ���������ֵ
struct PUSH_FIELDS_HEAD
{
	UINT	uDBID;			// ���ݿ�ID
	UINT	uTableID;		// ���ID
	UINT	uRecordID;		// ��¼��ID
	INT		iFieldIndex1;	// �ֶ����1
	INT		iFieldIndex2;	// �ֶ����2
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_NET_CMD_STRUCT_DEFINE_H_2003_11_12__
