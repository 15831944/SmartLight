//*
//*
//* Q_tcpServerTask.h
//*
//*
#if !defined(__Q_TCPSERVERLISTENTHREAD_H__INCLUDED__)
#define __Q_TCPSERVERLISTENTHREAD_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif



#include "Q_tcpServerTask.h"
#include "Q_taskBase.h"
#include "Q_tcpServerUnitInfo.h"
#include "gSystemDef.h"





/** 
 * @interface 
 * @brief Q_tcpServerWorkThread class declare
 * @author yangxi
 * @date 2012-5-23
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_tcpServerWorkThread
{
public:
	Q_tcpServerWorkThread(void);
	virtual ~Q_tcpServerWorkThread(void);


public:
	virtual bool					startWork(void);
	virtual bool					stopWork(void);
	virtual int						runThread(void) = 0;

	inline Q_ThreadStatusType		getThreadStatus(void) { return m_eThreadStatus;}
	inline void						setThreadPeriod(int nThreadPeriod) { m_nThreadPeriod = nThreadPeriod;}
	inline int						getThreadPeriod(void) { return m_nThreadPeriod;}
	inline void						setServerUnitInfo(Q_tcpServerUnitInfo* p) { m_serverUnitInfo = p;}
	inline Q_tcpServerUnitInfo*		getServerUnitInfo(void) { return m_serverUnitInfo;}
    void waitForTimeout();
private:
	static  ThreadFuncRet _Q_Commfrme_TcpServerWorkThreadProc(LPVOID lpParameter);

protected:
	volatile Q_ThreadStatusType				m_eThreadStatus;
	int								m_nThreadPeriod;
	Q_tcpServerUnitInfo*			m_serverUnitInfo;
};



//DWORD WINAPI						_Q_Commfrme_TcpServerWorkThreadProc(LPVOID lpParameter);





/** 
 * @interface 
 * @brief Q_tcpServerListenThread class declare
 * @author yangxi
 * @date 2012-5-23
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_tcpServerListenThread : public Q_tcpServerWorkThread
{
public:
	Q_tcpServerListenThread(void);
	virtual ~Q_tcpServerListenThread(void);


public:	
	virtual int						runThread();

};





/** 
 * @interface 
 * @brief Q_tcpServerCheckThread class declare
 * @author yangxi
 * @date 2012-5-23
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_tcpServerCheckThread : public Q_tcpServerWorkThread
{
public:
	Q_tcpServerCheckThread(void);
	virtual ~Q_tcpServerCheckThread(void);


public:	
	virtual int						runThread();
	


};



#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif


#endif // !defined(__Q_TCPSERVERLISTENTHREAD_H__INCLUDED__)
