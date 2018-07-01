//*
//*
//* Q_tcpClientLink.h
//*
//*
#if !defined(__Q_TCPCLIENTLINK_H__INCLUDED__)
#define __Q_TCPCLIENTLINK_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif

#include "gSystemDef.h"
#include "Q_linkBase.h"






/** 
 * @interface 
 * @brief Q_tcpClientLinkSetting class declare
 * @author yangxi
 * @date 2012-1-16
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_tcpClientLinkSetting : public Q_linkSettingBase
{
public:
	Q_tcpClientLinkSetting(void);
	virtual ~Q_tcpClientLinkSetting(void);



public:
	inline void			setServerIp(char* chIp) { sprintf_s(m_chServerIp,Q_FRAME_CONST32,"%s",chIp);}
	inline char*		getServerIp(void) { return m_chServerIp;}
	inline void			setPort(int nPort) { m_nPort = nPort;}
	inline int			getPort(void) { return m_nPort;}


protected:
	char				m_chServerIp[Q_FRAME_CONST32];

	int					m_nPort;

	
};

/** 
 * @interface 
 * @brief Q_tcpClientLink class declare
 * @author yangxi
 * @date 2012-1-16
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_tcpClientLink : public Q_linkBase
{
public:
	Q_tcpClientLink(void);
	virtual ~Q_tcpClientLink(void);




public:
	virtual bool			initializeLink(void);
	virtual bool			terminateLink(void);
	virtual bool			connect(void);
	virtual bool			disconnect(void);
	virtual bool			readData(void);
	virtual bool			writeData(void);
private:
	bool SetBlock(bool bBlock);//ÉèÖÃ·Ç×èÈûor×èÈû true·Ç×èÈû falseÎª×èÈû
    
protected:

	SOCKET					m_socket;

//	fd_set					m_readSet;

	DWORD					m_dwLastErr;

	SOCKADDR_IN				m_address;
	char m_Lastmsg[200];
	char m_pFilePath[512];
	
};


#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif
#endif // !defined(__Q_TCPCLIENTLINK_H__INCLUDED__)

