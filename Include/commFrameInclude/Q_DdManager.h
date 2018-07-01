#if !defined(__Q_DDMANAGER_H__INCLUDED__)
#define __Q_DDMANAGER_H__INCLUDED__


//#pragma once

#include "Q_runningUnit.h"

class __Q_COMMFRAME_EXPORT Q_DdManager
{
public:
	Q_DdManager(void);
	virtual ~Q_DdManager(void);

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

#endif // !defined(__Q_TASKMANAGER_H__INCLUDED__)
