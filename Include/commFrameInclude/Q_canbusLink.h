//*
//*
//* Q_canbusLink.h
//*
//*
#if !defined(__Q_CANBUSLINK_H__INCLUDED__)
#define __Q_CANBUSLINK_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif

#include "Q_linkBase.h"

#ifdef UNIX_ENV
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/can.h>
#endif

#ifndef PF_CAN
#define PF_CAN 29
#endif
#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif

/** 
 * @interface 
 * @brief Q_canbusLinkSetting class declare for linux
 * @author fanshuo
 * @date 2015-10-23
 * @see 
 * @bug No know bugs so far.
 */


class __Q_COMMFRAME_EXPORT Q_canbusLinkSetting : public Q_linkSettingBase
{
public:
	Q_canbusLinkSetting(void);
	virtual ~Q_canbusLinkSetting(void);

public:
	inline int			getRemoteCanId() { return m_canid_remotenode;}
	inline void			setRemoteCanId(int nCanId) { m_canid_remotenode = nCanId;}
	inline int			getLocalCanId() { return m_canid_localnode;}
	inline void			setLocalCanId(int nCanId) { m_canid_localnode = nCanId;}

	inline void			setCanDevName(char* chdevname) { sprintf_s(m_candevname,Q_FRAME_CONST32,"%s",chdevname);}
	inline char*		getCanDevName() { return m_candevname;}	

protected:
	char				m_candevname[Q_FRAME_CONST32];
	int                 m_canid_localnode;
	int                 m_canid_remotenode;
};



/** 
 * @interface 
 * @brief Q_canbusLink class declare for linux
 * @author yangxi
 * @date 2015-10-23
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_canbusLink : public Q_linkBase
{
public:
	Q_canbusLink(void);
	virtual ~Q_canbusLink(void);


public:
	virtual bool			initializeLink();
	virtual bool			terminateLink();
	virtual bool			connect() ;
	virtual bool			disconnect();
	virtual bool			readData();
	virtual bool			writeData();

protected:

	int 					m_socketcan;

	#ifdef UNIX_ENV
	struct sockaddr_can     m_socketcanaddr;
	struct ifreq            m_ifr;    
	struct can_frame        m_canframe_send;
	struct can_frame        m_canframe_revc;
	fd_set				 	m_readSet;
	DWORD					m_dwLastErr;
	#endif

};





#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif

#endif // !defined(__Q_TCPCLIENTLINK_H__INCLUDED__)

















