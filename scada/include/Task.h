//////////////////////////////////////////////////////////////////////////
#ifndef	_TASK_SCHEDULER_H__
#define	_TASK_SCHEDULER_H__

#include "gSystemDef.h"
#include <time.h>

#ifdef UNIX_ENV
	#include <unistd.h>
	#include "pthread.h"
#endif

#ifdef WINDOWS_ENV
#include	<mmsystem.h>
#pragma	comment(lib, "Winmm.lib")
#endif

#define	MAX_TASKS	32

namespace wiener_task
{
	typedef	struct _SECONDLY {
	  WORD SecondsInterval;
	} SECONDLY;

	typedef	struct _MINUTELY {
	  WORD MinutesInterval;
	} MINUTELY;

	typedef	struct _HOURLY {
	  WORD HoursInterval;
	} HOURLY;

	typedef	struct _DAILY {
	  WORD DaysInterval;
	} DAILY;

	typedef	struct _MONTHLYDATE {
	  DWORD	rgfDays;
	  WORD rgfMonths;
	} MONTHLYDATE;

	typedef	union _TRIGGER_TYPE_UNION {
	  SECONDLY Secondly;
	  MINUTELY Minutely;
	  HOURLY Hourly;
	  DAILY	Daily;
	  MONTHLYDATE MonthlyDate;
	} TRIGGER_TYPE_UNION;

	typedef	enum _TASK_TRIGGER_TYPE	{
	  TASK_TIME_TRIGGER_INVALID			= 0,
	  TASK_TIME_TRIGGER_ONCE	    = 1,
	  TASK_TIME_TRIGGER_SECONDLY	    = 2,
	  TASK_TIME_TRIGGER_MINUTELY	    = 3,
	  TASK_TIME_TRIGGER_HOURLY	    = 4,
	  TASK_TIME_TRIGGER_DAILY	    = 5,
	  TASK_TIME_TRIGGER_MONTHLYDATE	    = 6,
	} TASK_TRIGGER_TYPE, *PTASK_TRIGGER_TYPE;

	typedef	struct _TASK_TRIGGER {
	  WORD wStartYear;
	  WORD wStartMonth;
	  WORD wStartDay;
	  WORD wStartHour;
	  WORD wStartMinute;
	  WORD wStartSecond;
	  TASK_TRIGGER_TYPE TriggerType;
	  TRIGGER_TYPE_UNION Type;
	} TASK_TRIGGER,	* PTASK_TRIGGER;

	#ifdef WINDOWS_ENV
		#define	THREAD_HANDLE	HANDLE
	#endif
	#ifdef UNIX_ENV
		#define	THREAD_HANDLE	pthread_t
	#endif
	
	typedef	void (*ThreadFunc)(void	*);
	
	class Task
	{
	public:
		TASK_TRIGGER	m_trigger;
		Task(LPCTSTR id,TASK_TRIGGER trigger,ThreadFunc	pNewFunc,void *pPara = NULL);
		~Task( );
		void Execute( );
		char* GetTaskID() { return m_strTaskID;	}
		bool IsTaskAlive() { return bTaskStatus; }
	
	private:
		static void * run(void *arg);
		bool bTaskStatus;
	
		ThreadFunc	pFunc;
		void *		pParameter;
		THREAD_HANDLE	  ThreadHandle;
		char		  m_strTaskID[32];
	};
	
	class AFX_EXT_CLASS CTaskScheduler
	{
	public:
		CTaskScheduler();
		~CTaskScheduler();
	
		void	StartTimer();
		void	StopTimer();
	
		bool RegisterTask(LPCTSTR id,TASK_TRIGGER trigger, ThreadFunc pNewFunc,	void *pPara = NULL);
		void RemoveTask(LPCTSTR	id);
	
	//protected:
		bool CheckTrigger(TASK_TRIGGER trigger,	time_t now);
		Task*		m_pTasks[MAX_TASKS];
		bool		m_bFlags[MAX_TASKS];
	
	#ifdef WINDOWS_ENV
		UINT  timerRes;
		void TimerProc(int handle = 0);
	#endif
	
	#ifdef UNIX_ENV
	    struct sigaction old_sig_action;
		struct itimerval old_timer;
		static void TimerProc(int handle = 0);
	#endif
		UINT	TimerId;
		int		m_nTasks;
		void	ExecuateFunc();
	};
}
using namespace	wiener_task;

#endif //_TASK_SCHEDULER_H__
