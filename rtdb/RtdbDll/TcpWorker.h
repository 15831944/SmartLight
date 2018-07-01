/////////////////////////////////////////////////////////////////////////////
// TcpWorker.h: 定义一个用于在某个socket上读写数据的类CTcpWorker
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_TCP_WORKER_H_2004_03_02__
#define __AFX_TCP_WORKER_H_2004_03_02__

#include "CmdProcess.h"
#include "ThreadEvent.h"
#include "ThreadMutex.h"
/////////////////////////////////////////////////////////////////////////////
class CTcpWorker : public CSocketEx
{
public:
	CTcpWorker(CCmdProcess * pProcesser);
	virtual ~CTcpWorker();

protected:
    CThreadMutex	m_ThreadMutex;
    CThreadEvent	m_ProcessEvent;
	//接收
	int				m_nBufferEnd;
	char			m_pszBuffer[NET_BUFFER_SIZE];
	
	//发送
    CThreadMutex	m_SendMutex;
	char			m_pszSendBuffer[TCP_FRAME_LENGTH];
	int				m_iSendBufferSize;

public:
	CCmdProcess *	m_pCmdProcesser;
	bool			m_bClient;
	NODE_INFO		m_TcpLinkInfo;		//对端IP信息
	int				m_iPort;			//服务器端口信息
	bool			m_bReceiveRun;
	bool			m_bProcessRun;
public:
	bool	Init(SOCKET hSocket, bool bCreateThread=true);
	bool	ConnectInit(int nPort,char * pSocketAddress);
	bool	FindHead();

public:
	static  ThreadFuncRet ReceiveThread(LPVOID pTcpWorker);
	static  ThreadFuncRet ProcessThread(LPVOID pTcpWorker);

public:
	virtual	bool	TcpCmdProcess(char * pBuffer, int nBufferSize);

	virtual bool	SendCommand(int nCommand,char * pBuffer,int nBufferSize, bool bSendNow=true);
	virtual bool	SendCommand();
	virtual void    KeepLive();

public:
	bool	SendData(char * pBuffer,int nBufferSize, bool bSendNow=true);
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_TCP_WORKER_H_2004_03_02__
