//*
//*
//* Q_tcpServerTask.h
//*
//*
#if !defined(__Q_TCPSERVERTASK_H__INCLUDED__)
#define __Q_TCPSERVERTASK_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif


#include "Q_frameAssistant.h"
#include "Q_channelBase.h"
#include "Q_tcpServerLink.h"




/** 
 * @interface 
 * @brief Q_tcpServerTask class declare
 * @author yangxi
 * @date 2012-5-21
 * @see 
 * @bug No know bugs so far.
 */

class __Q_COMMFRAME_EXPORT Q_tcpServerTask
{
public:
	Q_tcpServerTask(void);
	virtual ~Q_tcpServerTask(void);

	inline void								setProtocolSetting(Q_protocolSettingBase* p) { m_pProtocolSetting = p;}
	inline Q_protocolSettingBase*			getProtocolSetting(void) { return m_pProtocolSetting;}
	inline void								setTcpServerLink(Q_tcpServerLink* p) { m_pTcpServerLink = p;}
	inline Q_tcpServerLink*					getTcpServerLink(void) { return m_pTcpServerLink;}
	inline Q_channelBase*					getClientChannel(void) { return m_clientChannel;}
	void setTaskID(int nTaskid){m_nTaskID=nTaskid;}
	int getTaskID(){return m_nTaskID;}

	inline void	setDlgHWND(HWND wnd){m_wnd = wnd;}
	inline void	setDlgMessageID(int nMsgID){m_nMsgID = nMsgID;}

	virtual bool							initializeTask(void);
	virtual bool							terminateTask(void);
	virtual bool							checkTask(void);
	virtual bool							startWork(void);
	virtual bool							stopWork(void);
	virtual bool							acceptClient(void) = 0;
	virtual bool							reduceClientCount(int nCount);


protected:
	Q_protocolSettingBase*					m_pProtocolSetting;
	Q_tcpServerLink*						m_pTcpServerLink;

	Q_channelBase*							m_clientChannel;
	int m_nTaskID;// 任务 id
	HWND    m_wnd;//界面hwnd
	int     m_nMsgID;//界面消息ID;
};




#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif


#endif // !defined(__Q_TCPSERVERTASK_H__INCLUDED__)