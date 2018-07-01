/////////////////////////////////////////////////////////////////////////////
// MUdpWorker.h : 用于描述用于UDP数据传输的类CMUdpWorker.
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_MUDP_WORKER_H_2004_09_06__
#define __AFX_MUDP_WORKER_H_2004_09_06__

#include "NetStructDef.h"
#include "CmdProcess.h"
#include "ThreadEvent.h"
#include "ThreadMutex.h"
/////////////////////////////////////////////////////////////////////////////
//应该为多播的处理 tjs 
class AFX_EXT_CLASS CMUdpWorker : public CSocketEx
{
public:
	CMUdpWorker(CCmdProcess * pProcesser, int nIndex);
	virtual ~CMUdpWorker();

protected:
	CMultiDataBase*	m_pMultiDataBase;
	UINT			m_nPort;

	char			m_pszBuffer[NET_BUFFER_SIZE];
	int				m_nBufferEnd;

	CThreadMutex	m_ThreadMutex;
	CThreadEvent	m_ProcessEvent;

	SOCKET			m_hReceiveSocketA;			//接收A端口
	SOCKET			m_hReceiveSocketB;			//接收B端口
	bool			m_bOKA;
	bool			m_bOKB;
	char			m_MultiCastIP1[24];
	char			m_MultiCastIP2[24];
	int				m_nIndex;
	char			m_IP1[24];
	char			m_IP2[24];
public:
	CCmdProcess *	m_pCmdProcesser;
	UINT			m_HasRecvCmd;

public:
	bool	GetIPConfig();
	bool	Init( CMultiDataBase * pMultiDataBase, UINT nPort );

	void	InitA();
	void	InitB();

	static  ThreadFuncRet ReceiveThreadA(LPVOID pMUdpWorker);
	static  ThreadFuncRet ReceiveThreadB(LPVOID pMUdpWorker);
	static  ThreadFuncRet ProcessThread(LPVOID pMUdpWorker);
	bool	m_bReceiveARun;
	bool	m_bReceiveBRun;
	bool	m_bProcessRun;

	virtual bool	SendCommand(int nCommand,char * pBuffer,int nBufferSize,bool bSendNow=true);
	virtual void    KeepLive();
	
	bool	ForceBroadcast(int nCommand,char * pBuffer,int nBufferSize);

public:
	virtual bool UdpCmdProcess(char * pBuffer, int nBufferSize);

};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_MUDP_WORKER_H_2004_09_06__
