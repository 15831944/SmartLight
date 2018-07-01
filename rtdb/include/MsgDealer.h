/****************************************************************************
 * MsgDealer.h : 定义用于处理实时库发送来的消息的类CMsgDealer				*
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
// 应用程序自定义消息处理函数类型,注意此函数不要delete传入的消息体
typedef void (* MsgDealFuncType)(tabMessage * pMsg);
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CMsgDealer
{
public:
	CMsgDealer();
	virtual ~CMsgDealer();

public:
	// 工作参数初始化(这是唯一的一个必须调用的函数)
	bool	Init(CMultiDataBase * m_pRtdb,int nProcessType,MsgDealFuncType pFunc);
	// 清除工作(一般可以不调)
	void	Exit( );

private:
	// 消息接收线程
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

	// 各应用需要自己实现的消息处理函数
	MsgDealFuncType	m_pDealFunc;

};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_MESSAGE_THREAD__2004_08_03__
