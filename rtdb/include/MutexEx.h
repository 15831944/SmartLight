/////////////////////////////////////////////////////////////////////////////
// MutexEx.h: �����ƽ̨�Ļ���ͬ��������.
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_MUTEX_OBJECT_EXTEND_H_2004_08_16__
#define __AFX_MUTEX_OBJECT_EXTEND_H_2004_08_16__

#include "gSystemDef.h"
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CMutexEx
{
public:
	// ����ڹ��캯���в�ָ������������Լ�����Create�����������������
	CMutexEx(char * psName = NULL);
	//��ʵ���õ�
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
	//bSingleon�Ƿ����ڽ��̵�ʵ������
	bool	Create(char * psName,bool bSingleon=false);

	int     Wait(int nMilliSeconds = INFINITE);
	bool    Release();

public:
//	static void DeleteMutex();
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_MUTEX_OBJECT_EXTEND_H_2004_08_16__
