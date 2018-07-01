//*
//*
//* Q_taskManager.h
//*
//*
#if !defined(__Q_TASKMANAGER_H__INCLUDED__)
#define __Q_TASKMANAGER_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif



#include "Q_runningUnit.h"






/** 
 * @interface 
 * @brief Q_taskManager class declare
 * @author yangxi
 * @date 2012-1-2
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_taskManager
{
public:
	Q_taskManager(void);
	virtual ~Q_taskManager(void);



public:
	virtual bool					initializeManager(void);
	virtual bool					terminateManager(void);
	virtual bool					startWork(void);
	virtual bool					stopWork(void);
	void                            clear(void);

	bool							addUnit(Q_taskRunningUnit* pUnit);



protected:
	int								m_nUnitCount;
	Q_taskRunningUnit*				m_arrayTaskUnit[Q_TASKMANAGER_MAXTASKNUMBER];
};


#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif


#endif // !defined(__Q_TASKMANAGER_H__INCLUDED__)

