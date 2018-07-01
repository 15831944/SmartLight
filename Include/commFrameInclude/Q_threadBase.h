//*
//*
//* Q_threadBase.h
//*
//*
#if !defined(__Q_THREADBASE_H__INCLUDED__)
#define __Q_THREADBASE_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once

#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif


#include "gSystemDef.h"
#include "Q_frameDefine.h"
#include "Q_frameAssistant.h"
#include "Q_taskBase.h"




/** 
 * @interface 
 * @brief Q_threadBase class declare
 * @author yangxi
 * @date 2012-1-2
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_threadBase
{
public:
	Q_threadBase(void);
	virtual ~Q_threadBase(void);



public:
	virtual bool			startWork(void);
	virtual bool			stopWork(void);
	virtual int				runThread(void) = 0;
	virtual int             runDdThread(void) = 0;
	virtual bool            startDdWork(void);

    bool isClose();
	Q_ThreadStatusType		getThreadStatus(void) { return m_eThreadStatus;}
	void					setThreadPeriod(int nThreadPeriod) { m_nThreadPeriod = nThreadPeriod;}
	int						getThreadPeriod(void) { return m_nThreadPeriod;}
	bool					addTask(Q_taskBase* pTask);
	void waitForTimeout();
private:
	// 消息接收线程,便于被统一的线程初使化函数调用
	static  ThreadFuncRet _Q_Commfrme_ThreadProc(LPVOID lpParameter);
	static ThreadFuncRet  _Q_Commfrme_Dd_ThreadProc(LPVOID lpParameter);


protected:
	volatile Q_ThreadStatusType		m_eThreadStatus;
	int						m_nThreadPeriod;
	int						m_nTaskCount;
	Q_taskBase*				m_arrayTask[Q_TASKMANAGER_MAXTASKNUMBER];

};

//DWORD WINAPI				_Q_Commfrme_ThreadProc(LPVOID lpParameter);
//DWORD WINAPI				_Q_Commfrme_Dd_ThreadProc(LPVOID lpParameter);

#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif


#endif // !defined(__Q_THREADBASE_H__INCLUDED__)

