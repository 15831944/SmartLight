/////////////////////////////////////////////////////////////////////////////
// SocketEx.h: ����һ����ƽ̨��SOCKET������
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
	BYTE	bS_1; //comment by tjs eg��ID��ʼ
	BYTE	bF_1;
	BYTE	bJ_1;
	BYTE	bB_1;//eg��ID��ʼ end UINT����
	BYTE	bS_2;//eg��ID��ʼ
	BYTE	bF_2;
	BYTE	bJ_2;
	BYTE	bB_2;//eg��ID����
	BYTE	bLen1_L;//eg��֡��¼������ʼ
	BYTE	bLen1_H;
	BYTE	bLen2_L;
	BYTE	bLen2_H;//eg��֡��¼������ʼ
	INT		iHostByte;
	INT		bBodyNeedChange;// ��֡���ݰ������ݻ���Ҫ�����ֽ�˳��ת��
	INT		iFrameLength;	// ֡����(�������˰�ͷ�ĳ���)
	INT		iCommand;		// ������
    UINT	uSendHostID;	// ���ͽڵ��ID
	INT		iTargetNet;		// Ŀ������
	INT		iSendNet;		// ��������
};
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CSocketEx
{
public:
	CSocketEx();
	CSocketEx(int nDomain, int nType, int nProtocol = 0);
	virtual ~CSocketEx();
public:
	// socket�����
	SOCKET	m_hSocket;
	// socket��ʵ������(SOCK_STREAM / SOCK_DGRAM)
	int		m_nType;
	// �ಥ���͵ı�־
	bool    m_bMUDP;
    bool	m_bWillExit;	//�˳���־
	bool    m_bConnectOk;	//����������־
	int		m_nNetUse;

public:
	// �жϱ������Ƿ��Ѿ���ʼ��һ�����õ��׽���
	bool	IsValid();
	// ����socket�����
	SOCKET	Socket(int nDomain, int nType, int nProtocol = 0);
	// ����socket�����(���洴�������ļ�:�Զ�ȡnDomain=AF_INET,nProtocol = 0)
	SOCKET	Socket(int nType);

	// �󶨵�ĳ��[IP��ַ:�˿�]
	int		Bind(int nPort, char * pIPAddress = NULL);
	// ����TCP����״̬
	int		Listen(int nBackLog);
	// ���ӷ�������(����nLimitSecond���������ӵ�ʱ��,���Ϊ0��û������)
	int		Connect(char * pIPAddress, int nPort, int nMiliSecond = 600);
	bool	ConnectTest(char * pIPAddress, int nPort, int nMiliSecond = 600);
	// ����TCP��������
	SOCKET	Accept(struct sockaddr * pSockAddress = NULL);
	// ��������(�������������Զ�����recv��recvfrom)
	int		Recv(char * pBuffer, int nLength, int nSecond = 0,struct sockaddr * pFrom = NULL);
	// ��������(�������������Զ�����send��sendto)
	int		Send(const char * pBuffer, int nLength, int nSecond = 0, struct sockaddr * pTo = NULL);
	// TCP��������ʽ��������
	int		SendNonBlock(const char * pBuffer, int nLength, int nLimitSecond);
	// ������˳�����ģʽ
	bool	SetBlock(bool bBlock);

public:
	// ��������Ƿ���Է�������
	bool	IsAllowSend(int nSecond);
	// ��������Ƿ���Զ�ȡ����
	bool	IsAllowRecv(int nSecond);

public:
	// ����socketѡ��
	int		SetSockOpt(int level, int optname, const char * optval, int optlen);
	// ���ù㲥ѡ��
	bool	SetBroadCast(bool bBroadCast);
	// ����socket�󶨵Ķ˿��Ƿ���Ա�����(��������Linux)
	bool	SetReuseAddress(bool bReuse);
	// ����I/O����ģʽ
	bool	SetBlockMode(bool bBlock);

	// ���ý��ջ�������С
	bool	SetRecvBufSize(int nBufSize);
	// ���÷��ͻ�������С
	bool	SetSendBufSize(int nBufSize);

// ����ҪŲ��CBaseNet���еļ�������
public:
	static void		HeadByteOrderChange(NET_TRANS_HEAD * pNetTransHead);
public:
	// ��Զ˷����������ݰ�
	virtual bool	SendCommand(int nCommand,char * pBuffer,int nBufferSize, bool bSendNow=true);
	virtual bool	SendCommand();

	virtual void	KeepLive();

};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_SOCKETEX_H_2004_03_04__
