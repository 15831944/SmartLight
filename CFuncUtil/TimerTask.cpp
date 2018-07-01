#include "StdAfx.h"
#include"TimerTask.h"

TimerTask::~TimerTask()
{
	stop();
}
void TimerTask::setInterval(int span)
{
	m_timeSpan = span;
}
bool TimerTask::start()
{
	m_bRun=true;
	bool bRet = CreateThreadEx(runTaskThread, (void *)this);
	return bRet;
}

bool TimerTask::stop()
{
	m_bRun = false;
	while(m_bThreadStateRun)
	{
		MilliSleep(100);
	}
	printf("TimerTaskExcit\r\n");
	return true;

}

ThreadFuncRet TimerTask::runTaskThread(LPVOID pData)
{
	TimerTask * pThis = (TimerTask *)pData;
	pThis->m_bThreadStateRun=true;
	while(pThis->m_bRun)
	{		
		pThis->task(); //Ö´ÐÐÈÎÎñ
		//Ë¯Ãß
        int count = pThis->m_timeSpan*0.01;
        for (int i=0; i<count; i++)
        {
            if (!pThis->m_bRun)
			{
				break;
			}
            MilliSleep(100);
        }
	}
	pThis->m_bThreadStateRun=false;
	return 0;
}
