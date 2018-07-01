//*
//*
//* Q_simpleThread.h
//*
//*
#if !defined(__Q_RESERVETHREAD_H__INCLUDED__)
#define __Q_RESERVETHREAD_H__INCLUDED__

#ifdef WINDOWS_ENV

#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif

#include "Q_frameDefine.h"
#include "Q_frameAssistant.h"
#include "Q_threadBase.h"
#include "ThreadEvent.h"



/** 
 * @interface 
 * @brief Q_simpleThread class declare
 * @author yangxi
 * @date 2012-1-12
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_reserveThread :	public Q_threadBase
{
public:
	Q_reserveThread(void);
	virtual ~Q_reserveThread(void);



public:
	virtual int				runThread(void);
	virtual int             runDdThread(void);
	bool getEvent();
	void setSwitch();

private:
    CThreadEvent m_hSwitchToSlave;
	DWORD m_dwSwitchTime;
	void clearBuffer();//清除接收发送缓冲区数据
};


#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif
#endif // !defined(__Q_SIMPLETHREAD_H__INCLUDED__)
