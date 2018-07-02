/////////////////////////////////////////////////////////////////////////////
// TcpWorker.h: ����һ��������ĳ��socket�϶�д���ݵ���CTcpWorker
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
	//����
	int				m_nBufferEnd;
	char			m_pszBuffer[NET_BUFFER_SIZE];
	
	//����
    CThreadMutex	m_SendMutex;
	char			m_pszSendBuffer[TCP_FRAME_LENGTH];
	int				m_iSendBufferSize;

public:
	CCmdProcess *	m_pCmdProcesser;
	bool			m_bClient;
	NODE_INFO		m_TcpLinkInfo;		//�Զ�IP��Ϣ
	int				m_iPort;			//�������˿���Ϣ
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
