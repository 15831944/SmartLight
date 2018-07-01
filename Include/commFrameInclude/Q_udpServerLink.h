//*
//*
//* Q_tcpClientLink.h
//*
//*
#if !defined(__Q_UDPSERVERLINK_H__INCLUDED__)
#define __Q_UDPSERVERLINK_H__INCLUDED__

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
class __Q_COMMFRAME_EXPORT Q_udpServerLinkSetting : public Q_linkSettingBase
{
public:
	Q_udpServerLinkSetting(void);
	virtual ~Q_udpServerLinkSetting(void);



public:
	inline void			setServerIp(char* chIp) { sprintf_s(m_chServerIp,Q_FRAME_CONST32,"%s",chIp);}
	inline char*		getServerIp() { return m_chServerIp;}
	inline void			setPort(int nPort) { m_nPort = nPort;}
	inline int			getPort() { return m_nPort;}
	inline void			setAnyIp(bool bVal) { m_bAnyIp = bVal;}
	inline bool			getAnyIp() { return m_bAnyIp;}

	inline void			setClientOneIp(char* chIp) { sprintf_s(m_chClientOneIp,Q_FRAME_CONST32,"%s",chIp);}
	inline char*		getClientOneIp() { return m_chClientOneIp;}
	inline void			setClientOnePort(int nPort) { m_nClientOnePort = nPort;}
	inline int			getClientOnePort() { return m_nPort;}

protected:
	char				m_chServerIp[Q_FRAME_CONST32];
	int					m_nPort;
	bool				m_bAnyIp;
	char				m_chClientOneIp[Q_FRAME_CONST32];
	int					m_nClientOnePort;
	
};

/** 
 * @interface 
 * @brief Q_tcpClientLink class declare
 * @author yangxi
 * @date 2012-1-16
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_udpServerLink : public Q_linkBase
{
public:
	Q_udpServerLink (void);
	virtual ~Q_udpServerLink (void);




public:
	virtual bool			initializeLink();
	virtual bool			terminateLink();
	virtual bool			connect() ;
	virtual bool			disconnect();
	virtual bool			readData();
	virtual bool			writeData();


protected:

	SOCKET					m_socket;

	fd_set					m_readSet;

	DWORD					m_dwLastErr;
    //需要维护一个对端的地址表，如果该地址不存在就断开此连接
	SOCKADDR_IN				m_address;
	SOCKADDR_IN				m_addressClient;//对端地址
};


#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif

#endif // !defined(__Q_TCPCLIENTLINK_H__INCLUDED__)

