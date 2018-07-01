//*
//*
//* Q_tcpServerRunningUnit.h
//*
//*
#if !defined(__Q_TCPSERVERRUNNINGUNIT_H__INCLUDED__)
#define __Q_TCPSERVERRUNNINGUNIT_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif


#include "Q_runningUnit.h"
#include "Q_tcpServerUnitInfo.h"



/** 
 * @interface 
 * @brief Q_tcpServerTaskRunningUnit class declare
 * @author yangxi
 * @date 2012-5-23
 * @see 
 * @bug No known bug so far.
 */
class __Q_COMMFRAME_EXPORT Q_tcpServerTaskRunningUnit : public Q_taskRunningUnit
{
public:
	Q_tcpServerTaskRunningUnit(void);
	virtual ~Q_tcpServerTaskRunningUnit(void);




public:
	virtual bool							initializeUnit(void);
	virtual bool							startWork(void);
	virtual bool							stopWork(void);
	virtual bool							terminateUnit(void);


	inline void								setCheckThread(Q_tcpServerCheckThread* p) { m_checkThread = p;}
	inline Q_tcpServerCheckThread*			getCheckThread(void) { return m_checkThread;}
	inline void								setListenThread(Q_tcpServerListenThread* p) { m_listenThread = p;}
	inline Q_tcpServerListenThread*			getListenThread(void) { return m_listenThread;}
	inline void								setServerUnitInfo(Q_tcpServerUnitInfo* p) { m_serverUnitInfo = p;}
	inline Q_tcpServerUnitInfo*				getServerUnitInfo(void) { return m_serverUnitInfo;}


	inline void								setDdCheckThread(Q_tcpServDdCheckThread* p) { m_DdcheckThread = p;}
	inline Q_tcpServDdCheckThread*			getDdCheckThread(void) { return m_DdcheckThread;}
	inline void								setDdListenThread(Q_tcpServDdListenThread* p) { m_DdListenThread = p;}
	inline Q_tcpServDdListenThread*			getDdListenThread(void) { return m_DdListenThread;}

protected:
	Q_tcpServerCheckThread*					m_checkThread;

	Q_tcpServerListenThread*				m_listenThread;

	Q_tcpServerUnitInfo*					m_serverUnitInfo;

	Q_tcpServDdCheckThread*                 m_DdcheckThread;  //调度线程

	Q_tcpServDdListenThread*                m_DdListenThread;  //调度线程
	

	
};

#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif


#endif // !defined(__Q_TCPSERVERRUNNINGUNIT_H__INCLUDED__)