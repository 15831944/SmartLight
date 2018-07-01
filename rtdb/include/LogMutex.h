/////////////////////////////////////////////////////////////////////////////
// LogMutex.h identical to MutexEx.h: 定义跨平台的互斥同步对象类.
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_LOGMUTEX_OBJECT_EXTEND_H_2004_08_16__
#define __AFX_LOGMUTEX_OBJECT_EXTEND_H_2004_08_16__

#include "gSystemDef.h"

/////////////////////////////////////////////////////////////////////////////
class CLogMutex
{
public:
	// 如果在构造函数中不指定名称则必须自己调用Create函数来创建互斥对象
	CLogMutex(char * psName = NULL);
	virtual ~CLogMutex();

private:
#ifdef WINDOWS_ENV
	HANDLE	m_pMutex;
#else
	typedef struct
	{
		pthread_mutex_t Mutex;
		char    		MutexName[40];
	}SfMutex;
	pthread_mutex_t * m_pMutex;
	char * m_pMapAddress;
#endif

public:
	bool	Create(char * psName);
	int     Wait(int nMilliSeconds = INFINITE);
	bool    Release();

public:
	static void DeleteMutex();
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_LOGMUTEX_OBJECT_EXTEND_H_2004_08_16__
