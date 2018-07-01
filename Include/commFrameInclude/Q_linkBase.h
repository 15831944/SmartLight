//*
//*
//* Q_linkBase.h
//*
//*
#if !defined(__Q_LINKBASE_H__INCLUDED__)
#define __Q_LINKBASE_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif



#include "Q_frameDefine.h"
#include "Q_frameAssistant.h"


/** 
 * @interface 
 * @brief Q_linkSettingBase class declare
 * @author yangxi
 * @date 2011-12-28
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_linkSettingBase
{
public:
	Q_linkSettingBase(void);
	virtual ~Q_linkSettingBase(void);

};


/** 
 * @interface 
 * @brief Q_linkConfig class declare
 * @author yangxi
 * @date 2011-12-28
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_linkConfig
{
public:
	Q_linkConfig(void);
	virtual ~Q_linkConfig(void);



public:
	inline Q_linkSettingBase*	getLinkSetting() { return m_pLinkSetting;}
	inline void					setLinkSetting(Q_linkSettingBase* pLinkSetting) { m_pLinkSetting = pLinkSetting;}

	inline Q_commonBuffer*		getReceiveBuffer() { return m_pReceiveBuffer;}
	inline void					setReceiveBuffer(Q_commonBuffer* pBuffer) { m_pReceiveBuffer = pBuffer;}
	inline Q_commonBuffer*		getSendBuffer() { return m_pSendBuffer;}
	inline void					setSendBuffer(Q_commonBuffer* pBuffer) { m_pSendBuffer = pBuffer;}

	inline void					setMaxNoDataTimes(int nValue) { m_nMaxNoDataTimes = nValue;}
	inline int					getMaxNoDataTimes() { return m_nMaxNoDataTimes;}
	inline void					setLogPath(char* chPath) { sprintf_s(m_chLogPath,Q_FRAME_CONST256,"%s",chPath);}
	inline char*				getLogPath() { return m_chLogPath;}
	

protected:


	/** receive data buffer. */ 
	Q_commonBuffer*				m_pReceiveBuffer;


	/** send data buffer. */ 
	Q_commonBuffer*				m_pSendBuffer;


	/** setting info. */ 
	Q_linkSettingBase*			m_pLinkSetting;

	/** max no data times. */
	int							m_nMaxNoDataTimes;
	/** log path */
	char						m_chLogPath[Q_FRAME_CONST256];

};


/** 
 * @interface 
 * @brief Q_linkBase class declare
 * @author yangxi
 * @date 2011-12-29
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_linkBase
{
public:
	Q_linkBase(void);
	virtual ~Q_linkBase(void);


public:
	virtual bool				initializeLink() = 0;
	virtual bool				terminateLink() = 0;
	virtual bool				connect() = 0;
	virtual bool				disconnect() = 0;
	virtual bool				readData() = 0;
	virtual bool				writeData() = 0;
	virtual bool				clearData();
	
	inline Q_CommonStatusType	getLinkStatus() { return m_eLinkStatus;}
	void setLinkStatus(Q_CommonStatusType t){m_eLinkStatus = t;}//jwk add
	inline Q_linkConfig*		getLinkConfig() { return m_pLinkConfig;}
	inline void					setLinkConfig(Q_linkConfig* pLinkConfig) { m_pLinkConfig = pLinkConfig;}
	inline void					setLogStatus(int nLog) { m_nLogStatus = nLog;}

protected:
	void					writeLog(int nType,char* chVal);
protected:
	Q_CommonStatusType			m_eLinkStatus;
	int							m_nNoRDataTimes;
	int							m_nNoSDataTimes;
	Q_linkConfig*				m_pLinkConfig;
	int							m_nLogStatus;		//0:no log,1:all log,2:error log
	FILE*						m_fileLog;
	int						   m_nLastLogTime;
	char                       m_chLog[200];

};

#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif


#endif // !defined(__Q_LINKBASE_H__INCLUDED__)
