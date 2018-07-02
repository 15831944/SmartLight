//*
//*
//* Q_tcpServerUnitInfo.h
//*
//*
#if !defined(__Q_TCPSERVERUNITINFO_H__INCLUDED__)
#define __Q_TCPSERVERUNITINFO_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif



#include "Q_tcpServerTask.h"
#include "Q_simpleThread.h"
#include "Q_taskBase.h"
#include "Q_dataManager.h"
#include "ThreadMutex.h"

/** 
 * @interface 
 * @brief Q_tcpServerUnitInfo class declare
 * @author yangxi
 * @date 2012-5-23
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_tcpServerUnitInfo
{
public:
	Q_tcpServerUnitInfo(void);
	virtual ~Q_tcpServerUnitInfo(void);



public:
	bool								initializeUnit(void);
	bool								startWork(void);
	bool								stopWork(void);
	bool								terminateUnit(void);


	inline void							setServerTask(Q_tcpServerTask* p) { m_tcpServerTask = p;}
	inline Q_tcpServerTask*				getServerTask(void) { return m_tcpServerTask;}


	
	bool								checkClientStatus(void);
	bool								listenClient(void);
	bool                                listenDdClient(void);
	Q_taskBase*	                        getTaskClient(int i);
	int                                 getTaskClientCount(void);

	//2015-05-12 ���շ�緢�ֳ�����������ᣬû����ȷ�󶨶˿ڡ�
	bool                                checkServerStatus(void);
	//2015-06-23 ʵʱ��ӿ� tjs
	void setRtdbInf(Q_dataManager *mgr);
	//2015-11-11 ���ӿ����趨��д�̴߳���ʱ��
	//nWaitMS����
	void setReadWriteWaitTime(int nWaitMS);

	//2016-12-15
	Q_taskBase*	getTaskByChannelID(int chid);
protected:
	bool								addClientInfo(Q_taskBase* pTask,Q_simpleThread* pThread);


protected:
	Q_tcpServerTask*					m_tcpServerTask;

	int									m_nClientCount;
	Q_taskBase*							m_clientTask[Q_TASKMANAGER_MAXTASKNUMBER];
	Q_simpleThread*						m_clientThread[Q_TASKMANAGER_MAXTASKNUMBER];
	Q_dataManager *m_pMgr;
	CThreadMutex m_csClientCnt;//���ӵ�����ͻ���ʱ��,���߳������ֶ�ȱ�� tjs 2015-11-5
	int m_nWaitMS;//�߳�ѭ���ȴ�ʱ��//Ĭ��Ϊ50

};




#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif


#endif // !defined(__Q_TCPSERVERUNITINFO_H__INCLUDED__)