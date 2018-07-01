//*
//*
//* Q_tcpClientLink.h
//*
//*
#if !defined(__Q_UDPCLIENTLINK_H__INCLUDED__)
#define __Q_UDPCLIENTLINK_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif

#include "Q_linkBase.h"





/** 
 * @interface 
 * @brief Q_tcpClientLinkSetting class declare
 * @author yangxi
 * @date 2012-1-16
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_udpClientLinkSetting : public Q_linkSettingBase
{
public:
	Q_udpClientLinkSetting(void);
	virtual ~Q_udpClientLinkSetting(void);



public:
	inline void			setServerIp(char* chIp) { sprintf_s(m_chServerIp,Q_FRAME_CONST32,"%s",chIp);}
	inline char*		getServerIp() { return m_chServerIp;}
	inline void			setPort(int nPort) { m_nPort = nPort;}
	inline int			getPort() { return m_nPort;}


	inline void			setClientIp(char* chIp) { sprintf_s(m_chClientIp,Q_FRAME_CONST32,"%s",chIp);}
	inline char*		getClientIp() { return m_chClientIp;}
	inline void			setClientPort(int nPort) { m_nClientPort = nPort;}
	inline int			getClientPort() { return m_nClientPort;}



protected:
	char				m_chServerIp[Q_FRAME_CONST32];
	int					m_nPort;

	char				m_chClientIp[Q_FRAME_CONST32];
	int					m_nClientPort;

	
};

/** 
 * @interface 
 * @brief Q_tcpClientLink class declare
 * @author yangxi
 * @date 2012-1-16
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_udpClientLink : public Q_linkBase
{
public:
	Q_udpClientLink (void);
	virtual ~Q_udpClientLink (void);




public:
	virtual bool			initializeLink();
	virtual bool			terminateLink();
	virtual bool			connect() ;
	virtual bool			disconnect();
	virtual bool			readData();
	virtual bool			writeData();
	virtual bool			clearData();
	virtual bool			clearBufferData();

protected:

	SOCKET					m_socket;

	fd_set					m_readSet;

	DWORD					m_dwLastErr;

	SOCKADDR_IN				m_address;
};


#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif

#endif // !defined(__Q_TCPCLIENTLINK_H__INCLUDED__)

