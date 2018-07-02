#ifndef __AFX_TIMER_TASK__H
#define __AFX_TIMER_TASK__H
#include "gSystemDef.h"
#include "CFuncUtilDef.h"
class  __Q_CFUNCUTIL_EXPORT TimerTask
{
public: 
	TimerTask():m_bRun(false),m_timeSpan(60000),m_bThreadStateRun(false){};
	virtual ~TimerTask();
	virtual void task(){};
	void setInterval(int span);
	bool start();
	bool stop();

private:
	// ����ִ��task�߳�
	static  ThreadFuncRet runTaskThread(LPVOID pData);
	bool m_bRun;
	unsigned int m_timeSpan;//ÿ�����ٺ�����ִ��һ��
	bool m_bThreadStateRun;
};

#endif //__AFX_TIMER_TASK__H