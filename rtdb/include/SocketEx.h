/////////////////////////////////////////////////////////////////////////////
// SocketEx.h: 定义一个跨平台的SOCKET处理类
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_SOCKETEX_H_2004_03_04__
#define __AFX_SOCKETEX_H_2004_03_04__

#include "gSystemDef.h"	
/////////////////////////////////////////////////////////////////////////////
#define NET_BUFFER_SIZE			640000
#define RCV_BUFFER_SIZE			4096
#define TCP_FRAME_LENGTH		7168
#define UDP_FRAME_LENGTH		1400
/////////////////////////////////////////////////////////////////////////////
struct NET_TRANS_HEAD
{
	BYTE	bS_1; //comment by tjs eg库ID开始
	BYTE	bF_1;
	BYTE	bJ_1;
	BYTE	bB_1;//eg库ID开始 end UINT类型
	BYTE	bS_2;//eg表ID开始
	BYTE	bF_2;
	BYTE	bJ_2;
	BYTE	bB_2;//eg表ID结束
	BYTE	bLen1_L;//eg此帧记录个数开始
	BYTE	bLen1_H;
	BYTE	bLen2_L;
	BYTE	bLen2_H;//eg此帧记录个数开始
	INT		iHostByte;
	INT		bBodyNeedChange;// 本帧数据包的内容还需要进行字节顺序转换
	INT		iFrameLength;	// 帧长度(不包含此包头的长度)
	INT		iCommand;		// 命令字
    UINT	uSendHostID;	// 发送节点的ID
	INT		iTargetNet;		// 目标网段
	INT		iSendNet;		// 发送网段
};
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CSocketEx
{
public:
	CSocketEx();
	CSocketEx(int nDomain, int nType, int nProtocol = 0);
	virtual ~CSocketEx();
public:
	// socket软插座
	SOCKET	m_hSocket;
	// socket的实际类型(SOCK_STREAM / SOCK_DGRAM)
	int		m_nType;
	// 多播类型的标志
	bool    m_bMUDP;
    bool	m_bWillExit;	//退出标志
	bool    m_bConnectOk;	//连接正常标志
	int		m_nNetUse;

public:
	// 判断本对象是否已经初始化一个可用的套接字
	bool	IsValid();
	// 创建socket软插座
	SOCKET	Socket(int nDomain, int nType, int nProtocol = 0);
	// 创建socket软插座(上面创建函数的简化:自动取nDomain=AF_INET,nProtocol = 0)
	SOCKET	Socket(int nType);

	// 绑定到某个[IP地址:端口]
	int		Bind(int nPort, char * pIPAddress = NULL);
	// 进入TCP监听状态
	int		Listen(int nBackLog);
	// 连接服务器端(参数nLimitSecond限制了连接的时间,如果为0就没有限制)
	int		Connect(char * pIPAddress, int nPort, int nMiliSecond = 600);
	bool	ConnectTest(char * pIPAddress, int nPort, int nMiliSecond = 600);
	// 接收TCP连接请求
	SOCKET	Accept(struct sockaddr * pSockAddress = NULL);
	// 接收数据(根据连接类型自动调用recv或recvfrom)
	int		Recv(char * pBuffer, int nLength, int nSecond = 0,struct sockaddr * pFrom = NULL);
	// 发送数据(根据连接类型自动调用send或sendto)
	int		Send(const char * pBuffer, int nLength, int nSecond = 0, struct sockaddr * pTo = NULL);
	// TCP无阻塞方式发送数据
	int		SendNonBlock(const char * pBuffer, int nLength, int nLimitSecond);
	// 进入或退出阻塞模式
	bool	SetBlock(bool bBlock);

public:
	// 检查现在是否可以发送数据
	bool	IsAllowSend(int nSecond);
	// 检查现在是否可以读取数据
	bool	IsAllowRecv(int nSecond);

public:
	// 设置socket选项
	int		SetSockOpt(int level, int optname, const char * optval, int optlen);
	// 设置广播选项
	bool	SetBroadCast(bool bBroadCast);
	// 设置socket绑定的端口是否可以被重用(尤其用于Linux)
	bool	SetReuseAddress(bool bReuse);
	// 设置I/O阻塞模式
	bool	SetBlockMode(bool bBlock);

	// 设置接收缓冲区大小
	bool	SetRecvBufSize(int nBufSize);
	// 设置发送缓冲区大小
	bool	SetSendBufSize(int nBufSize);

// 可能要挪到CBaseNet类中的几个函数
public:
	static void		HeadByteOrderChange(NET_TRANS_HEAD * pNetTransHead);
public:
	// 向对端发送命令数据包
	virtual bool	SendCommand(int nCommand,char * pBuffer,int nBufferSize, bool bSendNow=true);
	virtual bool	SendCommand();

	virtual void	KeepLive();

};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_SOCKETEX_H_2004_03_04__
