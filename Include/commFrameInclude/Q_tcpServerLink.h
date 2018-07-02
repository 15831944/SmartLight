//*
//*
//* Q_tcpServerLink.h
//*
//*
#if !defined(__Q_TCPSERVERLINK_H__INCLUDED__)
#define __Q_TCPSERVERLINK_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif


#include "Q_tcpClientLink.h"
#include "ThreadMutex.h"




/** 
 * @interface 
 * @brief Q_tcpServerLink class declare
 * @author yangxi
 * @date 2012-5-21
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_tcpServerLinkSetting
{
public:
	Q_tcpServerLinkSetting(void);
	virtual ~Q_tcpServerLinkSetting(void);



public:
	inline void			setServerIp(char* chIp) { sprintf_s(m_chServerIp,Q_FRAME_CONST32,"%s",chIp);}
	inline char*		getServerIp() { return m_chServerIp;}
	inline void			setPort(int nPort) { m_nPort = nPort;}
	inline int			getPort(void) { return m_nPort;}

	inline void			setAnyIp(bool bVal) { m_bAnyIp = bVal;}
	inline bool			getAnyIp(void) { return m_bAnyIp;}
	inline void			setPreClientCount(int nVal) { m_nPreClientCount = nVal;}
	inline int			getPreClientCount(void) { return m_nPreClientCount;}
	inline void			setMaxClientCount(int nVal) { m_nMaxClientCount = nVal;}
	inline int			getMaxClientCount(void) { return m_nMaxClientCount;}
	//�õ�socket���ջ�������С
	int  getSockeRecvBufferSize();
	//�õ�socket���ͻ�������С
	int  getSocketSendBufferSize();
	//����socket�Ľ��ջ�������СĬ��ΪQ_SOCKET_RECV_BUFFERSIZE
	void setSocketRecvBufferSize(int nSize);
	//����socket���ͻ�������СQ_SOCKET_SEND_BUFFERSIZE
	void setSocketSendBufferSize(int nSize); 
public:
	Q_LSrvPort          m_FilterCleintIP[Q_TCPSERVER_DEFCLIENTCOUNT];//���˵Ŀͻ���IP 2016-6-22
protected:
	bool				m_bAnyIp;
	char				m_chServerIp[Q_FRAME_CONST32];

	int					m_nPort;

	int					m_nPreClientCount;
	int					m_nMaxClientCount;
	int                 m_nSocketRecvSize;
	int                 m_nSocketSendSize;
};

/** 
 * @interface 
 * @brief Q_tcpServerLink class declare
 * @author yangxi
 * @date 2012-5-21
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_tcpServerLink
{
public:
	Q_tcpServerLink(void);
	virtual ~Q_tcpServerLink(void);




public:
	virtual bool				initializeLink(void);
	virtual bool				terminateLink(void);
	virtual bool				startService(void);
	virtual bool				stopService(void);
	virtual bool				acceptClient(void);

	inline SOCKET				getClientSocket(void) { return m_socketClient;}
	inline SOCKADDR_IN			getClientAddress(void) { return m_addressClient;}
//ԭ��inline����,���������,��֪Ϊɶ2015-10-19
	void					    reduceClient(int nVal);
	inline void					setLinkSetting(Q_tcpServerLinkSetting* p) { m_pTcpServerLinkSetting = p;}
	inline Q_tcpServerLinkSetting*
								getLinkSetting(void) { return m_pTcpServerLinkSetting;}

	inline Q_CommonStatusType   getLinkStatus(void) { return m_eLinkStatus;}
	inline void					setLogStatus(int nLog) { m_nLogStatus = nLog;}
	inline int					getLogStatus(void) { return m_nLogStatus;}

	//�����˵�IP,���Ϊtrue��ͨ��,false��ͨ��
	bool       checkFilterIP();
	//2017-03-09
	bool disconnectClient();
	void writeLog(int nType,char* chVal);
//
protected:

	Q_tcpServerLinkSetting*		m_pTcpServerLinkSetting;

	Q_CommonStatusType			m_eLinkStatus;

	SOCKET						m_socket;
	SOCKADDR_IN					m_address;

	SOCKET						m_socketClient;
	SOCKADDR_IN					m_addressClient;

	timeval						m_timeSpan;
	fd_set						m_readSet;

	DWORD						m_dwLastErr;
	int							m_nLogStatus;		//0:no log,1:all log,2:error log

	int							m_nCurrentClientCount;
	int                         m_nRecvBufSize;
	int                         m_nSendBufSize;
	//FILE*						m_fileLog;
	//char                        m_chLog[200];
	//Q_SocketClientInfo          *m_clietnArray[2];
	CThreadMutex m_csClientCnt;//�Ա����Ļ������m_nCurrentClientCount,��Ϊ�Ƕ��߳� 2015-10-19 tjs
};






/** 
 * @interface 
 * @brief Q_tcpServerClientLinkSetting class declare
 * @author yangxi
 * @date 2012-5-23
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_tcpServerClientLinkSetting : public Q_linkSettingBase
{
public:
	Q_tcpServerClientLinkSetting(void);
	virtual ~Q_tcpServerClientLinkSetting(void);



public:
	inline void				setSocket(SOCKET sock) { m_socket = sock;}
	inline SOCKET			getSocket(void) { return m_socket;}
	inline void				setAddress(SOCKADDR_IN addr) { m_address = addr;}
	inline SOCKADDR_IN		getAddress(void) { return m_address;}
	//�õ�socket���ջ�������С
	int  getSockeRecvBufferSize();
	//�õ�socket���ͻ�������С
	int  getSocketSendBufferSize();
	//����socket�Ľ��ջ�������СĬ��ΪQ_SOCKET_RECV_BUFFERSIZE
	void setSocketRecvBufferSize(int nSize);
	//����socket���ͻ�������СQ_SOCKET_SEND_BUFFERSIZE
	void setSocketSendBufferSize(int nSize); 

protected:
	SOCKET					m_socket;

	SOCKADDR_IN				m_address;
	int                 m_nSocketRecvSize;
	int                 m_nSocketSendSize;

	
};



/** 
 * @interface 
 * @brief Q_tcpServerClientLink class declare
 * @author yangxi
 * @date 2012-5-21
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_tcpServerClientLink : public Q_linkBase
{
public:
	Q_tcpServerClientLink(void);
	virtual ~Q_tcpServerClientLink(void);



public:
	virtual bool			initializeLink(void);
	virtual bool			terminateLink(void);
	virtual bool			connect(void);
	virtual bool			disconnect(void);
	virtual bool			readData(void);
	virtual bool			writeData(void);

	inline void				setSocket(SOCKET sock) { m_socket = sock;}
	inline void				setAddress(SOCKADDR_IN address) { m_address = address;}
	void                    setCheckTimeOut(bool flg){m_bCheckTimeOut=flg;}
	//���ö����ݳ�ʱʱ�䣬Ĭ��ΪQ_TCPCLIENT_SELECTTIME ��λ΢��
	void                    setReadTimeOut(int n_vs);
    int                     getReadTimeOut();


protected:

	SOCKET					m_socket;

	fd_set					m_readSet;

	DWORD					m_dwLastErr;

	SOCKADDR_IN				m_address;
	bool                    m_bCheckTimeOut;
	char *m_pSendBuffer;//���͵�buffer2015-11-6 Ϊ��֧�ִ���ڴ淢��,���ٷ���
	char *m_pRecvBuffer;//���յ�Bufer 2015-11-6 Ϊ��֧�ִ���ڴ淢��,���ٷ���
	int m_nReadTimeOut;//����ʱʱ��
};


#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif

#endif // !defined(__Q_TCPSERVERLINK_H__INCLUDED__)

