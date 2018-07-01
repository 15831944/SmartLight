//*
//*
//* Q_simpleThread.h
//*
//*
#if !defined(__Q_SIMPLETHREAD_H__INCLUDED__)
#define __Q_SIMPLETHREAD_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif



#include "Q_frameDefine.h"
#include "Q_frameAssistant.h"
#include "Q_threadBase.h"




/** 
 * @interface 
 * @brief Q_simpleThread class declare
 * @author yangxi
 * @date 2012-1-12
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_simpleThread :	public Q_threadBase
{
public:
	Q_simpleThread(void);
	virtual ~Q_simpleThread(void);



public:
	virtual int				runThread(void);
	virtual int             runDdThread(void);
	virtual bool			stopWork(void);
private:
	int m_nCheckMaserType;//检查主机还得备机
	volatile bool m_bThreadStateRun;
};


#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif

#endif // !defined(__Q_SIMPLETHREAD_H__INCLUDED__)
