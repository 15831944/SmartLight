#pragma once

#include "Q_tcpServerTask.h"
#include "Q_taskBase.h"
#include "Q_tcpServerUnitInfo.h"
#include "gSystemDef.h"

class __Q_COMMFRAME_EXPORT Q_tcpServDdThread
{
public:
	Q_tcpServDdThread(void);
	virtual ~Q_tcpServDdThread(void);

	virtual bool					startWork(void);
	virtual bool					stopWork(void);
	virtual int						runThread(void) = 0;
	void                            waitForTimeout(void);

	inline Q_ThreadStatusType		getThreadStatus(void) { return m_eThreadStatus;}
	inline void						setThreadPeriod(int nThreadPeriod) { m_nThreadPeriod = nThreadPeriod;}
	inline int						getThreadPeriod(void) { return m_nThreadPeriod;}
	inline void						setServerUnitInfo(Q_tcpServerUnitInfo* p) { m_serverUnitInfo = p;}
	inline Q_tcpServerUnitInfo*		getServerUnitInfo(void) { return m_serverUnitInfo;}
private:
	static  ThreadFuncRet _Q_Commfrme_TcpServDdThreadProc(LPVOID lpParameter);

protected:
	volatile Q_ThreadStatusType				m_eThreadStatus;
	int								m_nThreadPeriod;
	Q_tcpServerUnitInfo*			m_serverUnitInfo;
};


//DWORD WINAPI						_Q_Commfrme_TcpServDdThreadProc(LPVOID lpParameter);


class __Q_COMMFRAME_EXPORT Q_tcpServDdListenThread : public Q_tcpServDdThread
{
public:
	Q_tcpServDdListenThread(void);
	virtual ~Q_tcpServDdListenThread(void);

public:	
	virtual int						runThread();
};


class __Q_COMMFRAME_EXPORT Q_tcpServDdCheckThread : public Q_tcpServDdThread
{
public:
	Q_tcpServDdCheckThread(void);
	virtual ~Q_tcpServDdCheckThread(void);

public:	
	virtual int						runThread();
};


