/////////////////////////////////////////////////////////////////////////////
// Rtdb_Struct.h : 包含使用此动态库所需的头文件及函数原型
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_RTDB_STRUCT_H__
#define __AFX_RTDB_STRUCT_H__

#include "gSystemDef.h"
/////////品质////////////////////////////////
#define Quality_Good 0
#define Quality_Bad 1

/////////////////////////////////////////////////////////////////////////////
struct MDB_MEM_OFFSET
{
	// 表示m_pDataBase 数组对象在共享内存中偏移量
	UINT	DbInfoOffset;
	// 表示数据库对象利用情况的BOOL数组在共享内存中偏移量
	UINT	DbInfoIndexOffset;
	// 消息对象在共享内存中偏移量
	UINT	TabMessageOffset;
	// 各进程读消息对象的位置在共享内存中偏移量
	UINT	ReadMessagePosOffset;
	// 消息连接对象在共享内存中偏移量
};
/////////////////////////////////////////////////////////////////////////////
#define MESBODY_MAX_LENGTH	256
struct tabMessage
{
	UINT	m_uMsgID;			// 发送处分配的消息的全网唯一的消息ID
	UINT	m_uTargetID32;		// 接收消息的进程的ID32(0表示广播给指定类型的程序)
	UINT	m_uSourceID32;		// 发送消息的进程的ID32
	INT		m_iProcessType;		// 接收消息的进程类型(可以是多个进程的组合)
	INT		m_iMsgType;			// 消息类型
	INT		m_iMsgSize;			// 消息体长度
	BOOL	m_bBodyNeedChange;	// 消息体是否需要应用进行字节顺序转换
	// 消息体(收消息的应用须先由m_bBodyNeedChange判断是否要对消息体进行字节顺序转换)
	BYTE	m_bMes[MESBODY_MAX_LENGTH];
};
struct ProcessMessageInfo
{
	UINT	iWritePos;
	UINT	iReadPos;
};
/////////////////////////////////////////////////////////////////////////////
// 下面是节点类型(SYSTEM_NetNode->iType)的宏定义:
#define nt_MainServer			(1<<0)	//主服务器
#define nt_BackServer			(1<<1)	//备服务器
#define nt_MainFEP				(1<<2)	//主前置机
#define nt_BackFEP				(1<<3)	//备前置机
#define nt_Client				(1<<4)	//工作站

#define nt_AutoLogout			(1<<10)	//启动自动注销

//服务器不发更新消息，全网最好只有一个节点发送
#define nt_JavaUpdateMsg		(1<<11)	//向本机Java发送更新消息
#define nt_WebServer			(1<<12)	//Web服务
#define nt_AlarmSound			(1<<21)	//允许报警发声
#define nt_AlarmPrint			(1<<22)	//允许报警打印
#define nt_AlarmAuto			(1<<23)	//允许报警自动确认
#define nt_AlarmGraph			(1<<24)	//允许报警推画面
#define nt_AlarmPut				(1<<25) //允许报警推窗口
#define nt_AlarmBell			(1<<26) //允许电铃
#define nt_AlarmFlute			(1<<27) //允许电笛


/////////////////////////////////////////////////////////////////////////////
// 下面是进程类型(tabProcess->p_iType)的宏定义:
// 备注: 为了便于同时给多个进程发送消息,这里的进程类型采用标志位
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define pt_RTSERVER				(1 << 0)	//系统管理
#define pt_HISTORY				(1 << 1)	//历史转储
#define pt_CHANNELOP			(1 << 2)	//前置通讯
#define pt_ScadaServer			(1 << 3)	//SCADA服务
#define pt_GRAPH				(1 << 4)	//图形界面 //src =7
#define pt_ALARM				(1 << 5)	//报警
#define pt_AGC			(1 << 6)	//AGC算法
#define pt_AVC			(1 << 7)	//AVC算法
#define pt_PSCS			(1 << 8)	//稳定控制算法
#define pt_filetran		(1 << 9)	//文件传输
#define pt_DBGSVR			    (1 << 10)	//数据处理服务器dbgsvr，规约打印功能

//#define pt_RTDBEDIT				(1 << 4)	//实时库维护

//#define pt_PDR					(1 << 6)	//事故记录

//#define pt_MCI					(1 << 8)	//主控界面
//#define pt_GRAPH_EDIT			(1 << 9)	//图形编辑
//#define pt_DTS_Console			(1 << 10)	//DTS控制台
#define pt_Java_Server			(1 << 11)	//JAVA系统管理
//#define pt_PAS_Server			(1 << 12)	//PAS控制台
//#define pt_Alarm_Mobile			(1 << 13)	//电子值班
//#define pt_DBGSVR			    (1 << 14)	//数据处理服务器dbgsvr，规约打印功能

#define pt_Total				16	//总进程数
/////////////////////////////////////////////////////////////////////////////
// 下面是进程运行状态(tabProcess->p_iStatus)的宏定义:
#define ps_Run		( 1 << 1 )
#define ps_Main		( 1 << 2 )
#define ps_FakeDel	( 1 << 3 )
#define ps_Exit		( 1 << 4 )
#define ps_Switch	( 1 << 5 )
//#define ps_Tcp		( 1 << 6 )
/////////////////////////////////////////////////////////////////////////////
// 定义节点信息存储单元
typedef struct _tagNodeInfo
{
	CHAR		m_strID[24];
	CHAR		m_strIP1[16];
	CHAR		m_strIP2[16];
} NODE_INFO;
/////////////////////////////////////////////////////////////////////////////
#define MESSAGE_MAX_NUM			3000
//#define PROCESS_MAX_NUM			400//注意这里的定义一定要和进程表容量大小定义相同
#define DATABASE_MAX_NUM		10
/////////////////////////////////////////////////////////////////////////////
#define NET_1		1 // 一网
#define NET_2		2 // 二网
#define NET_ALL		3 // 双网
/////////////////////////////////////////////////////////////////////////////
class CMultiDataBase;
/////////////////////////////////////////////////////////////////////////////
// define transmit type

#define MAX_TRANSMIT_TYPE	6		//系统支持的tcp通道数 //临时测试cpu占用率过高的问题
//#define MAX_SERVER_NUM		4		//系统支持的服务器个数,服务器间互为备用
#define MAX_SERVER_NUM		2		//系统支持的服务器个数,服务器间互为备用
#define TT_SYS_UDP		0 // 系统UDP通道(用于系统网络状态检测等用途)
#define TT_TRANSMIT		1 // 集中传输通道(初始化或运行时的实时库整表单位的传输)
#define TT_MESSAGE		2 // 系统消息传输
#define TT_UPDATE		3 // 数据更新通道(用于包括系统表在内的所有表数据的更新)
#define TT_TCPNOTIFY	4 // 本机消息(采用TCP而不是事件方式通知进程)
#define TT_EXTEND_1		5 // 数据传输的第2个预留类型

#define TT_WEB			20 // web发布

extern char pszChannelName[6][15];
/////////////////////////////////////////////////////////////////////////////
#ifdef WINDOWS_ENV
	extern __declspec( dllexport ) bool SendCommand(int nCommand, char * pBuffer, int nBufferSize, int nSendType = TT_UPDATE);
	extern __declspec( dllexport ) bool SendWebCommand(int nCommand, char * pBuffer, int nBufferSize);
	extern __declspec( dllexport ) void	ForceBroadcast(int nCommand,char * pBuffer,int nBufferSize);
	extern __declspec( dllexport ) void SetDefaultField(int nDataType,char * pFieldData,char * pszDefault);
#endif
#ifdef UNIX_ENV
	extern "C" bool SendCommand(int nCommand, char * pBuffer, int nBufferSize, int nSendType = TT_UPDATE);
	extern "C" bool SendWebCommand(int nCommand, char * pBuffer, int nBufferSize );
	extern "C" void ForceBroadcast(int nCommand,char * pBuffer,int nBufferSize);
	extern "C" void SetDefaultField(int nDataType,char * pFieldData,char * pszDefault);
#endif
/////////////////////////////////////////////////////////////////////////////

#endif // __AFX_RTDB_STRUCT_H__
