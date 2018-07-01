/////////////////////////////////////////////////////////////////////////////
// ThreadEvent.h: 封装跨平台的用于同一进程内的各线程之间的事件对象
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_THREAD_EVENT_OBJECT_H__
#define __AFX_THREAD_EVENT_OBJECT_H__

#include "gSystemDef.h"
/////////////////////////////////////////////////////////////////////////////
// 事件对象
class AFX_EXT_CLASS CThreadEvent
{
public:
	CThreadEvent();
	virtual ~CThreadEvent();

private:
#ifdef WINDOWS_ENV
	HANDLE	m_hEvent;
#else
	pthread_mutex_t m_mutex;
	pthread_cond_t m_hEvent;
#endif

public:
	void	Init();
	void	Destroy();
	bool	SetEvent();
	bool	Wait(int dwMilliSeconds = INFINITE);
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_THREAD_EVENT_OBJECT_H__
