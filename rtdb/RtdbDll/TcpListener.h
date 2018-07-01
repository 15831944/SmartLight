/////////////////////////////////////////////////////////////////////////////
// TcpListener.h: 定义一个负责在服务器上监听客户端连接请求类CTcpWorker
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_TCP_LISTENER_H_2004_03_02__
#define __AFX_TCP_LISTENER_H_2004_03_02__

class CTcpReceiver;

#include "MultiDataBase.h"

#define MAX_ACCEPT_COUNT	20
/////////////////////////////////////////////////////////////////////////////
class CTcpListener : public CSocketEx
{
public:
	CTcpListener(CMultiDataBase * pMDB);
	virtual ~CTcpListener();

	CMultiDataBase*	m_pMultiDataBase;

	int			m_nPort;

public:
	CTcpReceiver*	m_pTcpReceiver[MAX_ACCEPT_COUNT];
	bool			m_bTcpReceiverOk[MAX_ACCEPT_COUNT];

public:
	bool	Init(int nPort,char * pSocketAddress = NULL);
	virtual bool	SendCommand(int nCommand,char * pBuffer,int nBufferSize, bool bSendNow=true);
	virtual bool	SendCommand();
	virtual void    KeepLive();

private:
	bool	AcceptInfo(SOCKET sAccept, char *strIP, int& iUseNet);
	static  ThreadFuncRet ListenThread(LPVOID pListener);
	bool		m_bListenRun;
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_TCP_LISTENER_H_2004_03_02__
