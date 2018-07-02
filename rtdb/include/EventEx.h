/////////////////////////////////////////////////////////////////////////////
// EventEx.h: �����ƽ̨���¼�������.
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_EVENT_OBJECT_EXTEND_H_2004_03_23__
#define __AFX_EVENT_OBJECT_EXTEND_H_2004_03_23__

#include "gSystemDef.h"
/////////////////////////////////////////////////////////////////////////////
// �¼�����
class AFX_EXT_CLASS CEventEx
{
public:
	CEventEx();
	CEventEx(char * psName, int iSemNo, int iTotalNum);
	virtual ~CEventEx();

private:
#ifdef WINDOWS_ENV
	HANDLE	m_hEvent;
#else
	int		m_hEvent;
#endif
	char	m_strSemName[40];
	int		m_iTotalNum;
	int		m_iSemNo;
	int		m_iInitValue;

public:
	//����ֵ���ͬһ�����ƵĻ������Ѿ�����Ҳ����true
	bool	Create(char * psName, int iSemNo, int iTotalNum, int iInitValue=0);
	bool	SetEvent( bool bAdd=true );
	bool	Wait(int dwMilliSeconds = INFINITE);
	void	DeleteEvent();
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_EVENT_OBJECT_EXTEND_H_2004_03_23__
