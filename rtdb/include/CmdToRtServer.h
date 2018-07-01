/////////////////////////////////////////////////////////////////////////////
// CmdToRtServer.h: headfile of the CCmdToRtServer class
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_CMD_TO_RTSERVER_H_2004_03_11__
#define __AFX_CMD_TO_RTSERVER_H_2004_03_11__

#include "CmdProcess.h"
#include "ThreadMutex.h"
#include "MutexEx.h"
#include "EventEx.h"
#include "RtdbFtp.h"
/////////////////////////////////////////////////////////////////////////////
class CCmdToRtServer
{
public:
	CCmdToRtServer(CMultiDataBase * pMultiDataBase);
	virtual ~CCmdToRtServer();

private:
	int		m_nCmdBufKey;
	int		m_nCmdMaxLength;
	int		m_nCmdMaxCount;
	NODE_INFO *m_pServerInfo;

	typedef struct
	{
		int nCommand;
		int nSendType;
		int nBufferSize;
	} _CmdHead;
public:
	bool			m_bInit;
	bool			m_bExit;
	bool			m_bRtServer;
	bool			m_bInNet;					//ÄÚÍø±êÖ¾
	CMultiDataBase*	m_pMultiDataBase;

	CMutexEx		m_SendMutex;
	CEventEx		m_SendEvent;
	CShareMemory	m_ShareMemory;
    CThreadMutex	m_ThreadMutex;

	int *			m_pPutPos;			
	int *			m_pGetPos;			
	char *			m_pCmdArea;			

	CCmdProcess		m_CmdProcess;
	CRtdbFtp*       m_pRtdbFtp;

public:
	CSocketEx *		m_pSocket[MAX_SERVER_NUM][MAX_TRANSMIT_TYPE];
	bool			m_bSocketOk[MAX_SERVER_NUM][MAX_TRANSMIT_TYPE];
	CSocketEx *		m_pWebSocket;
public:
	bool		Init(bool bRtServer);
	bool		SendCommand(int nCommand, char * pBuffer, int nBufferSize, int nSendType);
	void		ResetTcpChannel();

	bool		SocketInit();
	bool		InitWebChannel();

	static bool DeleteSendEvent();
private:
	void		InitRtdbChannel(char * cType,int nPort, int nIndex=-1);
	bool		SendCommandToNet();

public:
	static ThreadFuncRet SendThread(LPVOID pCmdToRtServer);
	static ThreadFuncRet KeepLiveThread(LPVOID pCmdToRtServer);

	static ThreadFuncRet UpdateToWebThread(LPVOID pCmdToRtServer);
	static ThreadFuncRet AllDataToWebThread(LPVOID pCmdToRtServer);
	bool   m_bSendRun;
	bool   m_bKeepLiveRun;
	bool   m_bUpdateRun;
	bool   m_bAllDataRun;
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_CMD_TO_RTSERVER_H_2004_03_11__
