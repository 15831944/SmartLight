//*
//*
//* Q_runningUnit.h
//*
//*
#if !defined(__Q_RESERVE_MANAGER_H__INCLUDED__)
#define __Q_RESERVE_MANAGER_H__INCLUDED__

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
#include "Q_reservethread.h"

#include "Q_tcpServerTask.h"
#include "Q_tcpServerListenThread.h"





/** 
 * @interface 
 * @brief Q_taskRunningUnit class declare
 * @author yangxi
 * @date 2012-1-2
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_reserveManager
{
public:
	Q_reserveManager(void);
	virtual ~Q_reserveManager(void);

public:
	virtual bool							initialize(void);
	virtual bool							startWork(void);
	virtual bool							stopWork(void);
	virtual bool							switchWork(void);

	virtual bool							terminateUnit(void);
	virtual bool                            EndApp(void);

	inline	void							setDriverMode(Q_CommonDriveMode eMode) { m_eDriverMode = eMode;}
	bool									setTask(Q_taskBase* pTask);
	Q_taskBase*                             getTask(void);

	bool									setThread(Q_reserveThread* pThread);
	//更新本机状态
	bool uptNodeStatus(int v_status);
	bool uptNodeStatus(int v_status,BYTE nMaster);
	bool uptOtherNodeStatus(int v_status,BYTE nMaster);
	bool checkCurrentNodeState(BYTE nodetype,BYTE runstate);
	Q_reserveThread* getpThread(){return m_Thread;}



protected:
	Q_CommonDriveMode						m_eDriverMode;
	Q_taskBase*								m_Task;
	Q_reserveThread*							m_Thread;
};






#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif

#endif // !defined(__Q_TASKMANAGER_H__INCLUDED__)