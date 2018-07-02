/****************************************************************************
 * DBTable.h : �����������ݿ��еı�ṹ�����ݼ�¼.							*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*
 * version : 1.8; created : 2002-08-17; last modified: 2004-08-28			*
 ****************************************************************************/
#ifndef __AFX_DATABASE_TABLE_H_2002_08_17__
#define __AFX_DATABASE_TABLE_H_2002_08_17__

#include "DBField.h"
#include "DataValue.h"

#define DB_TABLE_FlAG_PRIVATE 8   //�ǿ���˽�б���Ҫ����
class CDataBase;
/////////////////////////////////////////////////////////////////////////////
typedef struct _table_attr
{
	CHAR		m_cTableName[24];	// ��ļ���
	CHAR		m_cTableCName[40];	// ���������
	UINT		m_uDBID;			// ���������ݿ��ID
	UINT		m_uTableID;			// ���ID
	INT			m_iIndexInDB;		// �����������ݿ��е����
	INT			m_iTableSize;		// ���¼�Ĵ�С
	INT			m_iRecordMaxCount;	// ���¼�����������
	INT			m_iFieldNumber;		// ���а������ֶεĸ���
	INT			m_iFieldOffset;		// �����ڴ��д�ű��ֶε�λ��
	INT			m_iDataOffset;		// �����ڴ��д�ű����ݵ�λ��
	INT			m_iRecordCount;		// ���е�ʵ�ʼ�¼��
	UINT		m_uIDExclusive;		// ��������¼�¼��ID32
	INT        m_uFlag;            //��־ 2016-09-08 tjs ����
} CTableAttr;
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CDBTable : protected CTableAttr
{
	friend class CDataBase;
public:
	CDBTable();
	~CDBTable();

public:
	UINT	GetDataBaseID( );		// ����,�õ����ݿ�����
	CHAR *	GetTableName( );		// ����,�õ��������
	CHAR *	GetTableCName( );		// ����,�õ������������
	UINT	GetTableID( );			// ����,�õ���ı�־ID
	INT		GetIndexInDB( );		// ����,�õ������������ݿ�������
	INT		GetTableSize( );		// ����,�õ����¼�Ĵ�С
	INT		GetRecordMaxCount( );	// ����,�õ����м�¼��������
	INT		GetRecordCount( );		// ����,�õ����м�¼��ʵ�ʸ���
	INT		GetTableFieldNum( );	// ����,�õ������ֶθ���
	UINT	GetIDExclusive( );		// ����,�õ����¼Ψһ��ʶ

	INT	GetTableFlag();		// ����,�õ���ı�־λ 2016-09-08 ����


	void	SetTableName(CHAR * cTableName);	// ����,���ñ������
	void	SetTableCName(CHAR * cTableCName);	// ����,���ñ����������
	void	SetDataBaseID(UINT uDataBaseID);	// �ڲ�,�������ݿ�����
	void	SetTableID(UINT uTableID);			// �ڲ�,���ñ��ID
	void	SetIndexInDB(INT iIndex);			// �ڲ�,���ñ����������ݿ�������
	void	SetTableSize(INT iSize);			// �ڲ�,���ñ��¼�Ĵ�С
	void	SetRecordMaxCount(INT iMaxNum);		// �ڲ�,���ñ��м�¼��������
	void	SetRecordCount( INT iCount );		// �ڲ�,���ñ��м�¼��ʵ�ʸ���
	void	SetTableFieldNum(INT iFieldNum);	// �ڲ�,���ñ����ֶθ���
	void	SetIDExclusive(UINT uiID);			// ����,���ñ��¼Ψһ��ʶ

	void SetTableFlag(INT uflg);               //���ţ����ñ�ı�־λ
// �Ա��ֶεĴ���
public:
	// �����ֶ��ڱ��������õ��ֶζ���
    BOOL	GetFieldFromIndex( CDBField& DBField, INT iFieldIndex );
	// �����ֶε����ֵõ��ֶζ���
    BOOL	GetFieldFromName( CDBField& DBField, CHAR * cFieldName );
	// �����ֶε��������ֵõ��ֶζ���
    BOOL	GetFieldFromCName( CDBField& DBField, CHAR * cFieldCName );
	// �����ֶε�ID�õ��ֶζ���
    BOOL	GetFieldFromID( CDBField& DBField, UINT uiFieldID );
	// �����ֶε����Ʋ������ڱ��е����
	int		FindFieldFromName(char * cFieldName,bool bCName);

	CFieldAttr * GetFieldFromIndex(INT iFieldIndex);
	CFieldAttr * GetFieldFromID(UINT uFieldID);
	CFieldAttr * GetFieldFromName(CHAR * pName);
	CFieldAttr * GetFieldFromCName(CHAR * pCName);

	// �жϱ�������Ƿ����ֶζ���
	bool	IfHasField( );
	// �õ�������е�һ���ֶζ���λ��
	int		GetFirstFieldPostion( );
	// �õ����������һ���ֶζ���λ��
	bool	GetNextField( CDBField & DBField, int & iIndex );
	// ���±��ָ����ŵ��ֶζ���
	bool	SetField( CDBField DBField );

public:
	// ���� "=" ��������ڸ���CDBTable����
	CDBTable &	operator=(CDBTable & Val);
	CDBTable &	operator=(CTableAttr & Val);
	// �õ������ڴ����ֶζ������ʼ��ַ
	CFieldAttr*	GetFieldAddress( );
	// ȡ�ù����ڴ��б����ݵ���ʼ��ַ
	LPSTR		GetDataAddress( );

// �Ա��¼�Ĵ���
public:
	// ����ID32���ұ��еļ�¼������,���鲻Ҫʹ��,�Ժ���֧��
	BOOL	FindIDExclusive( UINT uRecordID, INT & iIndex );
	// ����ID32���ұ��еļ�¼������,���ظ�����ʾû���ҵ�
	INT		FindRecordIndex( UINT uRecordID );
	// ����ID32���ұ��еļ�¼
	CHAR *	FindIDExclusive( UINT uRecordID );
	BOOL	FindIDExclusive( UINT uRecordID, void * pVoid );
	// ����һ����¼,���ز����¼��λ�����(����-2��ʾ��¼�Ѵ���,����-1��ʾ��������)
	INT		Insert( void * pVoid, BOOL bBroadcast = TRUE, BOOL bSave = FALSE );
	// �ڱ�������һ�¼�¼,����ָ��ID32,��Ϊ0�����������,��ָ���Ƿ�Ҫ�Զ�����Ĭ��ֵ
	INT		Insert( UINT nID32, BOOL bDefault, BOOL bBroadcast = TRUE );
	void	InitRecordDefault(char * pRecord);
	// ����һ����¼,��ѡ��iIndex���ڿ��ٶ�λ,��Index-ID32У�����ͺ���Index
	INT		Update( void * pVoid, INT iIndex = -1, BOOL bBroadcast = TRUE, BOOL bSave = FALSE );
	// ɾ��һ����¼(uRecordIDΪ0����iIndexΪ׼;������uRecordIDΪ׼,��ʱiIndex����Ϊ-1)
	BOOL	Delete( UINT uRecordID, INT iIndex, BOOL bBroadcast = TRUE, BOOL bSave = FALSE );
	// ɾ�����е����м�¼
	BOOL	DeleteAllRecord( BOOL bBroadcast = TRUE );

	// ��ĳ�����ָ���ֶ���ŵ�һ���������ͬһ��ֵ(iRecordIndex2Ϊ-1����β)
	BOOL	SetColumnValueAt(INT iFieldIndex,void * pData,INT iRecordIndex1,INT iRecordIndex2,BOOL bBroadcast=TRUE);

	//���º���������������޸ĺ���ã����޸ĵ����ݸ�֪��������
	// ������ĳ�����ָ���ֶ���ŵ�һ�����ݹ㲥��ȥ
	BOOL	UpdateColumnAt(INT iFieldIndex,INT iRecordIndex1,INT iRecordIndex2=-1, BOOL bOnlyWeb=FALSE);
	// ������ĳ�����ָ���ֶ�ID��һ����ĳЩ��¼�����ݹ㲥��ȥ
	BOOL	UpdateColumnAt(INT iFieldIndex,INT iFieldCount,INT * pRecordIndex);
	// ������ĳ�����ָ��ID��һ����¼�㲥��ȥ
	BOOL	UpdateRecord(UINT uRecordID);
	// ������ĳ�����ָ����ŵ�һ����¼�㲥��ȥ
	BOOL	UpdateRecordAt(INT iRecordIndex);
	// ������ĳ�����ָ��ID��һ����¼�Ķ���ֶ�ֵ�㲥��ȥ CMD_UPDATE_RECORD_FIELDS
	BOOL	UpdateRecord(UINT uRecordID,INT nFieldCount,INT * pFieldIndex);
	// �����������ȫ���㲥 CMD_BROADCAST_DATA
	BOOL	Broadcast();

	// Java�������ڴ���׷��һ����¼��֪ͨC/C++
	BOOL	JavaAppend(BOOL bBroadcast = TRUE);
	// Java�������C/C++�Ľӿ�ɾ��һ����¼
	BOOL	JavaDelete(UINT uRecordID, INT iIndex, BOOL bBroadcast = TRUE);

	// ��ָ����¼��nFieldIndex1��nFieldIndex2�ֶν���ǰ��,Ȼ��pNewItemֵ����nFieldIndex2�ֶ�
	BOOL	PushFields(UINT uRecordID,INT nFieldIndex1,INT nFieldIndex2,CHAR * pNewItem,BOOL bBroadcast = TRUE);

	//ʵ�������¼�����ֶεĸ��� CMD_UPDATE_FIELDS
	//bSendNowһ������ΪFALSE,��������1024bit���Զ�����,����Ҫ��ʱ����ʱ��bSendNow����ΪTRUE
	BOOL	UpdateFields(UINT uRecordID, INT nRecordIndex, INT nFieldIndex, BOOL bSendNow = FALSE);
	//��buffer�����������ͣ��ڵ������������UpdateFields��������Ҫ���ô˺���
	//Ҳ������������UpdateFields����ʱ������bSendNowΪTRUE
	BOOL	UpdateFields();

public:
	// �����Ƿ��м�¼
	bool	IsEmpty( );
	// ȡ�ñ��е�һ����¼��λ��(�޼�¼�򷵻�-1)
	int		GetFirstPosition( );
	// ȡ��ָ��λ�ô��ı��¼,ͬʱ��λ�ò���ָ����һ����¼,ע�⵽��¼���յ㽫����NULL
	void *	GetNextElement( int & iIndex );
	// ȡ��ָ��λ�ô��ļ�¼(����д�ͷ���NULL)
	CHAR *	GetAt( INT iIndex );
	// ȡ��ָ��ID32��/��Index�ļ�¼��ͷ��ַ
	CHAR *	GetRecordAddr(UINT uRecordID,INT iIndex);

public:
	// �õ�����ָ����ŵļ�¼��ָ����ŵ��ֶε�ֵ
	BOOL	GetFieldDataAt( INT iRecordIndex, INT iFieldIndex, CDataValue& DataValue );
	// ���ñ���ָ����ŵļ�¼��ָ����ŵ��ֶε�ֵ
	BOOL	SetFieldDataAt( INT iRecordIndex, INT iFieldIndex, CDataValue DataValue, BOOL bBroadcast = TRUE );

	// �õ�����ָ����ŵļ�¼��ָ����ŵ��ֶε�ֵ
	BOOL	GetFieldDataAt( INT iRecordIndex, INT iFieldIndex, void * pVoid );
	// ���ñ���ָ����ŵļ�¼��ָ����ŵ��ֶε�ֵ
	BOOL	SetFieldDataAt( INT iRecordIndex, INT iFieldIndex, void * pVoid, BOOL bBroadcast = TRUE );

	// �õ�����ָ��ID32�ļ�¼��ָ����ŵ��ֶε�ֵ
	BOOL	GetFieldData(UINT uRecordID, INT iFieldIndex, void * pVoid);
	// ���ñ���ָ��ID32�ļ�¼��ָ����ŵ��ֶε�ֵ
	BOOL	SetFieldData(UINT uRecordID, INT iFieldIndex, const void * pVoid, BOOL bBroadcast = TRUE);

	// ���ñ������ָ��λ�õļ�¼�����ָ��λ�õ��ֶ�ֵ, ��������Ǹ��ֶ�ֵ�ĸ�ʽ���ַ���, �����Ҫ����ʽ��
	BOOL	SetFieldByText( INT nRecordIndex, INT nFieldIndex, CHAR * pText, BOOL bBroadcast = TRUE );

public:
	// �Ա����ĳ�Ա�������ֽ�˳��ת��
	void	ByteOrderChange_Member( );
	// �Ա������ָ��λ�õļ�¼�������ֶ����ݽ����ֽ�ת��
	void	ByteOrderChange_Record(int iRecordIndex,bool bChangeID32=true);
	// ��ָ��buffer���ձ��ֶ����ݽ����ֽ�ת��
	void	ByteOrderChange_Record(char *pRecordBuffer);
	// ������CCmdProcess��ֱ���޸�ʵʱ������
	void	SendDataToWebServer(UINT nCmd, char *Buffer, int iSize);
	void	MapUpdate( UINT* uRecordID, INT *iFieldIndex, INT iCount );
public:
	// �������ݱ��浽����Դ��(�Զ�ʶ������Դ���ļ��������ݿ�)
	bool	SaveData(bool bBroadcast);

	CTableAttr *	GetTableInfo( );

private:
	void	SetDataBase(CDataBase * pDataBase);
	bool	IsInShareMemory( );

	BOOL	CreateFieldMemory();
	BOOL	ReleaseFieldMemory();

	INT		InitAddField( CDBField& DBField );

	// ���ù����ڴ��б�����ƫ����
	void	SetDataOffset(INT iOffset);
	// ���ù����ڴ����ֶζ���������ƫ����
	void	SetFieldOffset(INT iOffset);

	void	MapChangeNotify(UINT uType,UINT uDbID,UINT uTabID,UINT uRecordID,BOOL bBroadcast);

	//���º�����ʵʱ������ָ����¼���浽���ý�����
	bool	InsertRecordTo( UINT nID );
	bool	UpdateRecordTo( UINT nID );
	bool	DeleteRecordTo( UINT nID );
	bool	InsertProcessForNode(UINT uNodeID);
	bool	DeleteProcessForNode(UINT uNodeID);
	bool	HasActiveProcessForNode(UINT uNodeID);

private:
	CDBField *	m_pField;		// ˽����ʱ����
	CDataBase *	m_pDataBase;	// �������ݿ��ָ��

	//UpdateFields����ʹ��
	CHAR		m_UpdateFieldsBuffer[CMD_MAX_LENGTH];
	INT			m_iTotalLen;

};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_DATABASE_TABLE_H_2002_08_17__
