/****************************************************************************
 * MsgDealer.h : �������ڴ���ʵʱ�ⷢ��������Ϣ����CMsgDealer				*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*
 * author : ; last modified : 2004-08-03; version 1.5						*
 ****************************************************************************/
#ifndef __AFX_MESSAGE_DEALER__2004_08_03__
#define __AFX_MESSAGE_DEALER__2004_08_03__

#include "Rtdb_Struct.h"
//#include "ThreadMutex.h"
//#include "ThreadEvent.h"
#include "ProcessEvent.h"
#include "MutexEx.h"

//#define MAX_MESSAGE_SIZE 1000

/////////////////////////////////////////////////////////////////////////////
// Ӧ�ó����Զ�����Ϣ����������,ע��˺�����Ҫdelete�������Ϣ��
typedef void (* MsgDealFuncType)(tabMessage * pMsg);
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CMsgDealer
{
public:
	CMsgDealer();
	virtual ~CMsgDealer();

public:
	// ����������ʼ��(����Ψһ��һ��������õĺ���)
	bool	Init(CMultiDataBase * m_pRtdb,int nProcessType,MsgDealFuncType pFunc);
	// �������(һ����Բ���)
	void	Exit( );

private:
	// ��Ϣ�����߳�
	static  ThreadFuncRet ReceiveThread(LPVOID pData);
	bool	m_bReceiveRun;

private:
	bool	m_bExit;
	int		m_nProcessType;

	CMutexEx		m_ProcessMutex;
	CProcessEvent	m_ProcessEvent;
	int				m_iProcessIndex;
	int				m_iHandleIndex;
	CMultiDataBase*	m_pRtdb;

	// ��Ӧ����Ҫ�Լ�ʵ�ֵ���Ϣ������
	MsgDealFuncType	m_pDealFunc;

};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_MESSAGE_THREAD__2004_08_03__
