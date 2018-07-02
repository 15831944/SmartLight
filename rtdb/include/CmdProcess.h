/////////////////////////////////////////////////////////////////////////////
// CmdProcess.h: ����һ������������������CCmdProcess
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_CMD_PROCESS_H_2004_03_08__
#define __AFX_CMD_PROCESS_H_2004_03_08__

#include "MultiDataBase.h"
/////////////////////////////////////////////////////////////////////////////
#define CP_FINISH			0 
#define CP_ERROR			1 
#define CP_CONTINUE			2 
/////////////////////////////////////////////////////////////////////////////
#define  MsgStructMaxSize	250
#define  MsgFieldMaxSize	150

struct Msg_DataStruct
{
	int iIndex;
	int iType;
	int iSize;
};

struct Msg_HeadDef
{
	char m_cName[40];
	int m_iKey;
	int bGroup;
	Msg_DataStruct m_dataList[MsgFieldMaxSize];
	int iFieldNum;
};

class CMessageDef
{
public:
	static Msg_HeadDef* GetMsgDef( );
	static void ReadMsgLine( char* cLine, int& iIndex);
	static void ReadMsgDefFile(  );
	
	static bool m_bInit;
	static Msg_HeadDef m_MsgMap[MsgStructMaxSize];
};



class CCmdProcess
{
public:
	CCmdProcess(CMultiDataBase * pMDB);
	virtual ~CCmdProcess();

	CMultiDataBase*	m_pMultiDataBase;
public:
	bool	CmdProcess(char * pBuffer,int nBufferSize,CSocketEx * pSocket = NULL);
	bool	IsContinue(int nCommand);

	// ... �����������
private:
	//ѯ��ʵʱ�⵱ǰʱ�� comment tjs
	void	NetCmdAskRtdbFinish(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//�յ��Է�ʵʱ�����󷵻أ�ʱ�������ʱ comment tjs
	void	NetCmdAnswerRtdbFinish(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//����ʵʱ���������� comment tjs
	void	NetCmdAskRtdbData(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//�յ�ʵʱ�����󷵻أ�ͬ��ʵʱ������ comment tjs
	void	NetCmdAnswerRtdbData(char * pBuffer,int nBufferSize,CSocketEx * pSocket);

	void	NetCmdAnswerRtdbDataEnd(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//����װ�⣬ͬʱ��������
	void	NetCmdReloadDataBase(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	void	NetCmdReloadTable(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//�������ݿ���Ϣ������ϢΪCDbAttr��С������
	void	NetCmdAskDataBase(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//�������Ϣ,������Ϣ����ΪCTableAttr�Ľṹ��С����
	void	NetCmdAskTable(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//�������п�ID,int���͵�����
	void	NetCmdAskAllDataBaseID(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//��TCP������socket������ip��Ϣ������id
	void	NetCmdTcpLinkInfo(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//��Ӧ���� ������ע�͵�
	void	NetCmdTcpHeart(char * pBuffer,int nBufferSize,CSocketEx * pSocket);

	//����֮�䷢����Ϣ
	void	NetCmdSendMessage( CHAR * pBuffer, INT iBufferSize, CSocketEx *pSocket );
//	void	NetCmdSendJavaMessage( CHAR * pBuffer, INT iBufferSize );
	void	NetCmdMessageProcessType( CHAR * pBuffer, INT iBufferSize, CSocketEx * pSocket );
	//���¼�¼
	void	NetCmdUpdateRecords( CHAR * pBuffer, INT iBufferSize, BOOL bInsert = FALSE );
	//ɾ����¼
	void	NetCmdDeleteRecords( CHAR * pBuffer, INT iBufferSize );
	//ɾ�����м�¼
	void	NetCmdDeleteAllRecords( CHAR * pBuffer, INT iBufferSize );
	//����ĳ����¼��ĳ���ֶ�ֵ
	void	NetCmdUpdateField( CHAR * pBuffer, INT iBufferSize );
	//����ĳ����¼��ĳ���ֶ�ֵ��
	void	NetCmdUpdateFields( CHAR * pBuffer, INT iBufferSize );
	void	NetCmdUpdateFieldArea( CHAR * pBuffer, INT iBufferSize );
	void	NetCmdSetColumnValue( CHAR * pBuffer, INT iBufferSize );
	void	NetCmdUpdateRecordFields( CHAR * pBuffer, INT iBufferSize );
	void	NetCmdSaveData( CHAR * pBuffer, INT iBufferSize );
	void	NetCmdPushFields( CHAR * pBuffer, INT iBufferSize );

	void	NetCmdDetectNet( CHAR * pBuffer, INT iBufferSize, CSocketEx * pSocket );
	void	NetCmdTime( CHAR * pBuffer, INT iBufferSize );
	void	NetCmdLocalNodeManager( CHAR * pBuffer, INT iBufferSize );
	void	NetCmdTotalNodeManager( CHAR * pBuffer, INT iBufferSize );
	void	NetCmdMainServerThis( CHAR * pBuffer, INT iBufferSize );
	void	NetCmdCloseSocket(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//================�ļ�����
	void	NetCmdPutFileStart( CHAR * pBuffer, INT iBufferSize, CSocketEx *pSocket );
	void	NetCmdPutFileContinue( CHAR * pBuffer, INT iBufferSize, CSocketEx *pSocket );
	void	NetCmdPutFileFail( CHAR * pBuffer, INT iBufferSize, CSocketEx *pSocket );
	void	NetCmdPutFileEnd( CHAR * pBuffer, INT iBufferSize, CSocketEx *pSocket );

	void	NetCmdGetFile( CHAR * pBuffer, INT iBufferSize, CSocketEx *pSocket );
	void	NetCmdGetFileStart( CHAR * pBuffer, INT iBufferSize, CSocketEx *pSocket );
	void	NetCmdGetFileContinue( CHAR * pBuffer, INT iBufferSize, CSocketEx *pSocket );
	void	NetCmdGetFileFail( CHAR * pBuffer, INT iBufferSize, CSocketEx *pSocket );
	void	NetCmdGetFileEnd( CHAR * pBuffer, INT iBufferSize, CSocketEx *pSocket );

	void	NetCmdDeleteFile( CHAR * pBuffer, INT iBufferSize, CSocketEx *pSocket );
	void	NetCmdRenameFile( CHAR * pBuffer, INT iBufferSize, CSocketEx *pSocket );

	void	NetCmdMkDir( CHAR * pBuffer, INT iBufferSize, CSocketEx *pSocket );
	//�ļ�����end
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_CMD_PROCESS_H_2004_03_08__
