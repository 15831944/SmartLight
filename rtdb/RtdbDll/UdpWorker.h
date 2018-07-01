/////////////////////////////////////////////////////////////////////////////
// UdpWorker.h : 用于描述用于UDP数据传输的类CUdpWorker.
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_UDP_WORKER_H_2004_09_06__
#define __AFX_UDP_WORKER_H_2004_09_06__

#include "NetStructDef.h"
#include "CmdProcess.h"
#include "ThreadEvent.h"
#include "ThreadMutex.h"
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CUdpWorker : public CSocketEx
{
public:
	CUdpWorker(CCmdProcess * pProcesser, bool bSingleUDP=true);
	virtual ~CUdpWorker();

protected:
	CMultiDataBase*	m_pMultiDataBase;
	UINT			m_nPort;
	bool			m_bSingleUDP;	//单播标志

	char			m_pszBuffer[NET_BUFFER_SIZE];
	int				m_nBufferEnd;

	CThreadMutex	m_ThreadMutex;
	CThreadEvent	m_ProcessEvent;

public:
	CCmdProcess *	m_pCmdProcesser;
	UINT			m_HasRecvCmd;

public:
	bool	Init( CMultiDataBase * pMultiDataBase, UINT nPort );

	static  ThreadFuncRet ReceiveThread(LPVOID pUdpWorker);
	static  ThreadFuncRet ProcessThread(LPVOID pUdpWorker);
	bool	m_bReceiveRun;
	bool	m_bProcessRun;

	virtual bool	SendCommand(int nCommand,char * pBuffer,int nBufferSize, bool bSendNow=true);
	virtual void    KeepLive();

	bool	ForceBroadcast(int nCommand,char * pBuffer,int nBufferSize);

public:
	virtual bool UdpCmdProcess(char * pBuffer, int nBufferSize);

};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_UDP_WORKER_H_2004_09_06__
