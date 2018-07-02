/////////////////////////////////////////////////////////////////////////////
// LogMutex.h identical to MutexEx.h: �����ƽ̨�Ļ���ͬ��������.
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_LOGMUTEX_OBJECT_EXTEND_H_2004_08_16__
#define __AFX_LOGMUTEX_OBJECT_EXTEND_H_2004_08_16__

#include "gSystemDef.h"

/////////////////////////////////////////////////////////////////////////////
class CLogMutex
{
public:
	// ����ڹ��캯���в�ָ������������Լ�����Create�����������������
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
