/////////////////////////////////////////////////////////////////////////////
// CmdProcess.h: 定义一个负责处理各种命令的类CCmdProcess
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

	// ... 各种命令处理函数
private:
	//询问实时库当前时间 comment tjs
	void	NetCmdAskRtdbFinish(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//收到对方实时库请求返回，时间命令，对时 comment tjs
	void	NetCmdAnswerRtdbFinish(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//请求实时库数据内容 comment tjs
	void	NetCmdAskRtdbData(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//收到实时库请求返回，同步实时库数据 comment tjs
	void	NetCmdAnswerRtdbData(char * pBuffer,int nBufferSize,CSocketEx * pSocket);

	void	NetCmdAnswerRtdbDataEnd(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//重新装库，同时返回数据
	void	NetCmdReloadDataBase(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	void	NetCmdReloadTable(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//请求数据库信息发送消息为CDbAttr大小的数据
	void	NetCmdAskDataBase(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//请求表信息,发送消息内容为CTableAttr的结构大小数据
	void	NetCmdAskTable(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//请求所有库ID,int类型的数组
	void	NetCmdAskAllDataBaseID(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//给TCP接收者socket赋两个ip信息及名称id
	void	NetCmdTcpLinkInfo(char * pBuffer,int nBufferSize,CSocketEx * pSocket);
	//回应心跳 内容已注释掉
	void	NetCmdTcpHeart(char * pBuffer,int nBufferSize,CSocketEx * pSocket);

	//本机之间发送消息
	void	NetCmdSendMessage( CHAR * pBuffer, INT iBufferSize, CSocketEx *pSocket );
//	void	NetCmdSendJavaMessage( CHAR * pBuffer, INT iBufferSize );
	void	NetCmdMessageProcessType( CHAR * pBuffer, INT iBufferSize, CSocketEx * pSocket );
	//更新记录
	void	NetCmdUpdateRecords( CHAR * pBuffer, INT iBufferSize, BOOL bInsert = FALSE );
	//删除记录
	void	NetCmdDeleteRecords( CHAR * pBuffer, INT iBufferSize );
	//删除所有记录
	void	NetCmdDeleteAllRecords( CHAR * pBuffer, INT iBufferSize );
	//更新某个记录的某个字段值
	void	NetCmdUpdateField( CHAR * pBuffer, INT iBufferSize );
	//更新某个记录的某个字段值组
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
	//================文件处理
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
	//文件处理end
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_CMD_PROCESS_H_2004_03_08__
