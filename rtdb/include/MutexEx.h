/////////////////////////////////////////////////////////////////////////////
// MutexEx.h: 定义跨平台的互斥同步对象类.
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_MUTEX_OBJECT_EXTEND_H_2004_08_16__
#define __AFX_MUTEX_OBJECT_EXTEND_H_2004_08_16__

#include "gSystemDef.h"
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CMutexEx
{
public:
	// 如果在构造函数中不指定名称则必须自己调用Create函数来创建互斥对象
	CMutexEx(char * psName = NULL);
	//单实例用的
	virtual ~CMutexEx();

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
	char m_strMutexName[40];
public:
	//bSingleon是否用于进程单实例运行
	bool	Create(char * psName,bool bSingleon=false);

	int     Wait(int nMilliSeconds = INFINITE);
	bool    Release();

public:
//	static void DeleteMutex();
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_MUTEX_OBJECT_EXTEND_H_2004_08_16__
