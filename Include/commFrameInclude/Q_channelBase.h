//*
//*
//* Q_channelBase.h
//*
//*
#if !defined(__Q_CHANNELBASE_H__INCLUDED__)
#define __Q_CHANNELBASE_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif



#include "Q_frameDefine.h"
#include "Q_frameAssistant.h"
#include "Q_linkBase.h"
#include "Q_protocolBase.h"
#include "Q_dataManager.h"



/** 
 * @interface 
 * @brief Q_channelBase class declare
 * @author yangxi
 * @date 2012-1-1
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_channelBase
{
public:
	Q_channelBase(void);
	virtual ~Q_channelBase(void);
	

public:
	 virtual bool					initializeChannel();
	 virtual bool					openChannel();
	 virtual bool					closeChannel();
	 virtual bool					terminateChannel();

	 bool							receiveData();
	 bool							sendData();
	 bool							explainData();
	 bool							clearData();
	 inline Q_commonBuffer*			getReceiveBuffer() { return m_pReceiveBuffer;}
	 inline void					setReceiveBuffer(Q_commonBuffer* pBuffer) { m_pReceiveBuffer = pBuffer;}
	 inline Q_commonBuffer*			getSendBuffer() { return m_pSendBuffer;}
	 inline void					setSendBuffer(Q_commonBuffer* pBuffer) { m_pSendBuffer = pBuffer;}
	 inline Q_linkConfig*			getLinkConfig() { return m_pLinkConfig;}
	 inline void					setLinkConfig(Q_linkConfig* pLinkConfig) { m_pLinkConfig = pLinkConfig;}
	 inline Q_linkSettingBase*		getLinkSetting() { return m_pLinkSetting;}
	 inline void					setLinkSetting(Q_linkSettingBase* pLinkSetting) { m_pLinkSetting = pLinkSetting;}
	 inline Q_linkBase*				getLink() { return m_pLink;}
	 inline void					setLink(Q_linkBase* pLink) { m_pLink = pLink;}
	 inline Q_protocolConfig*		getProtocolConfig() { return m_pProtocolConfig;}
	 inline void					setProtocolConfig(Q_protocolConfig* pProtocolConfig) { m_pProtocolConfig = pProtocolConfig;}
	 inline Q_protocolSettingBase*	getProtocolSetting() { return m_pProtocolSetting;}
	 inline void					setProtocolSetting(Q_protocolSettingBase* pProtocolSetting) { m_pProtocolSetting = pProtocolSetting;}
	 inline	Q_protocolBase*			getProtocol() { return m_pProtocol;}
	 inline void					setProtocol(Q_protocolBase* pProtocol) { m_pProtocol = pProtocol;}
	 inline bool					getPrimaryFlag() { return m_bPrimary;}
	 inline void					setPrimaryFlag(bool bPrimary) { m_bPrimary = bPrimary;}
	 inline void					setCommandList(Q_protocolCommandManagerBase* pCommandList) { m_pCommandList = pCommandList;}

	 inline Q_protocolCommandManagerBase*
									getCommandList() { return m_pCommandList;}
	  bool addCommand(Q_CommandVal* pVal);

	 Q_CommonStatusType				getChannelStatus();
	 void							checkStatus();
	 void                           setChannelID(int nID){m_nID=nID;}
	 int                            getChannelID(){return m_nID;}
	 inline void					setChannelDes(char* chDes) { sprintf_s(m_chDescribe,Q_FRAME_CONST32,"%s",chDes);}
	 inline char*					getChannelDes() { return m_chDescribe;}



protected:
	/** receive data buffer. */ 
	Q_commonBuffer*					m_pReceiveBuffer;
	/** send data buffer. */ 
	Q_commonBuffer*					m_pSendBuffer;
	/** command list. */ 
	Q_protocolCommandManagerBase*	m_pCommandList;
	/** link setting info. */ 
	Q_linkSettingBase*				m_pLinkSetting;
	/** link config info. */ 
	Q_linkConfig*					m_pLinkConfig;
	/** link object. */ 
	Q_linkBase*						m_pLink;
	/** protocol setting info. */ 
	Q_protocolSettingBase*			m_pProtocolSetting;
	/** protocol setting info. */ 
	Q_protocolConfig*				m_pProtocolConfig;
	/** protocol object. */ 
	Q_protocolBase*					m_pProtocol;
	/** primary channel flag. */ 
	bool							m_bPrimary;
	int m_nID;//Í¨µÀID
	/** channel describe. */ 
	char							m_chDescribe[Q_FRAME_CONST64];
};


#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif


#endif // !defined(__Q_CHANNELBASE_H__INCLUDED__)