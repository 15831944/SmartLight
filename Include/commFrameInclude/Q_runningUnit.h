//*
//*
//* Q_runningUnit.h
//*
//*
#if !defined(__Q_RUNNINGUNIT_H__INCLUDED__)
#define __Q_RUNNINGUNIT_H__INCLUDED__


#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif



#include "Q_frameDefine.h"
#include "Q_frameAssistant.h"
#include "Q_taskBase.h"
#include "Q_threadBase.h"

#include "Q_tcpServerTask.h"
#include "Q_tcpServerListenThread.h"
#include "Q_tcpServDdThread.h"





/** 
 * @interface 
 * @brief Q_taskRunningUnit class declare
 * @author yangxi
 * @date 2012-1-2
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_taskRunningUnit
{
public:
	Q_taskRunningUnit(void);
	virtual ~Q_taskRunningUnit(void);



public:
	virtual bool							initializeUnit(void);
	virtual bool							startWork(void);
	virtual bool							stopWork(void);
	//检查任务是否退出
	virtual bool							checkWorkQuit(void);
	virtual bool							terminateUnit(void);

	inline	void							setDriverMode(Q_CommonDriveMode eMode) { m_eDriverMode = eMode;}
	bool									addTask(Q_taskBase* pTask);
	bool									addThread(Q_threadBase* pThread);
	//2016.5.19 增加检测是否主机运行代码 wenjp
	bool									CheckMasterRun();



protected:
	Q_CommonDriveMode						m_eDriverMode;
	int										m_nTaskCount;
	Q_taskBase*								m_arrayTask[Q_TASKMANAGER_MAXTASKNUMBER];
	int										m_nThreadCount;
	Q_threadBase*							m_arrayThread[Q_TASKMANAGER_MAXTHREADNUMBER];
};






#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif


#endif // !defined(__Q_TASKMANAGER_H__INCLUDED__)