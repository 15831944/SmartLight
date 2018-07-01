/////////////////////////////////////////////////////////////////////////////
// TcpReceiver.h: 定义一个用于服务器上某个socket读写数据的类CTcpReceiver
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_TCP_RECEIVER_H_WW_2004_03_03__
#define __AFX_TCP_RECEIVER_H_WW_2004_03_03__

#include "TcpWorker.h"
#include "TcpListener.h"
/////////////////////////////////////////////////////////////////////////////
class CTcpReceiver : public CTcpWorker
{
public:
	CTcpReceiver(CCmdProcess * pProcesser);
	virtual ~CTcpReceiver();

public:
	CTcpListener *	m_pTcpListener;
public:
	virtual	bool  TcpCmdProcess(char * pBuffer, int nBufferSize);
	virtual void    KeepLive();
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_TCP_RECEIVER_H_WW_2004_03_03__
