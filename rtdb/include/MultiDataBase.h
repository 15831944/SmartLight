/****************************************************************************
 * MultiDataBase.h : 定义用于管理多个数据库的类CMultiDataBase.				*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*
 * version : 1.8; created : 2002-10-24; last modified: 2004-05-17			*
 ****************************************************************************/
#ifndef __AFX_MULTIPLE_DATABASE_MANAGER_H_2002_10_24__
#define __AFX_MULTIPLE_DATABASE_MANAGER_H_2002_10_24__

#include "SYSTEM.h"
#include "DataBase.h"
#include "SocketEx.h"
#include "ShareMemory.h"
#include "ProcessEvent.h"
#include "AlarmDefine.h"
#include "RtdbDefFileXml.h"//add by Wenpo
#include "ComputerWatch.h"
#include "CLogUtil.h"

#define RTDB_NULL		0 // 实时库未初始化
#define RTDB_STRUCTURE	1 // 实时库结构已初始化
#define RTDB_DATAINIT	2 // 实时库数据已初始化

#define EVENT_NOTIFY	1 // 消息机制:事件触发
//#define TCP_NOTIFY		2 // 消息机制:本机TCP触发

/////////////////////////////////////////////////////////////////////////////
typedef struct _rtdb_attr
{
//	CHAR		m_pszSysIniFile[96];	// 系统配置文件的全路径
//	CHAR		m_pszDataPath[96];		// 系统数据文件存放路径

	NODE_INFO	m_Server[MAX_SERVER_NUM];	// 4个服务器信息
	CHAR		m_pszHostName[24];	// 本机节点名称
	CHAR		m_strIP1[16];		// 本节点1网IP地址
	CHAR		m_strIP2[16];		// 本节点2网IP地址
	CHAR		m_strNet1IP[16];	// 本节点1网网段
	CHAR		m_strNet2IP[16];	// 本节点2网网段
	CHAR		m_strNet1Mask[16];	// 本节点1网IP掩码
	CHAR		m_strNet2Mask[16];	// 本节点2网IP掩码

	INT			m_iDBMaxNum;		// 包含的最大数据库个数
	INT			m_iRtdbStatus;		// 实时库状态
//	UINT		m_uCurMsgID;		// 当前消息的ID数
	UINT		m_nThisNodeID;		// 本节点在节点表中的ID
	INT			m_iCurrentNetUse;	// 当前使用的网络
	bool		m_bNet1Active;		// 1网状态
	bool		m_bNet2Active;		// 2网状态
	SYSTEMTIME	m_Net1GetTime;		// 1网最新检测包到达时间
	SYSTEMTIME	m_Net2GetTime;		// 2网最新检测包到达时间

	UINT		m_nSysUserID;		// 系统当前用户ID
	CHAR		m_cSysUserName[16];	// 系统当前用户名称
	INT			m_nRightsFlag[4];	// 系统当前用户的权限
	SYSTEMTIME	m_tLoginUse;		// 系统用户登录使用时间
} CRtdbAttr;

class AFX_EXT_CLASS CMultiDataBase : protected CRtdbAttr
{
	friend class CCmdProcess;
public:
	CMultiDataBase();
	~CMultiDataBase();

/////////////////////////////////////////////////////////////////////////////
public:
	// 由RtServer调用,先启动网络服务再创建本机实时数据库系统(本机可能是服务器也可能是客户端)
	bool	RtdbManagerInit(bool bLoadFromMapFile = false);
	// 仅由非RtServer调用,初始化本机的网络通讯模块(本机可能是服务器也可能是客户端)
	bool	SocketInit(bool bRtServer = false);
	bool	SocketExit();
	// 获取网络连接信息
	void	GetNetLinkInfo(char* strLink);

	BOOL	AttachShareMemory( );
	void	DetachShareMemory( );
	void	DestroyMemory( );

	// 进程注册
	bool	Register(int nType,int iWnd,bool bFakeDel = true, int nLocalMax = 1);
	// 进程注销
	bool	UnRegister();

	// 更新进程表中本进程信息
	bool	RefreshThisProcess( );

	bool	LocalNodeManager();
	bool	TotalNodeManager();
	void	DoOnStandby();
	int		GetSystemAlarmClass();
	int     GetNetHeartPeriod();	
	int     GetNodeHeartPeriod();	
	int     GetProcessHeartPeriod();	
	// 创建整个实时库的头文件
	bool	BuildIncludeFile( LPSTR szPasPath );

	BOOL	GetFirstDataBasePostion( INT& Pos );
	BOOL	GetNext( CDataBase& DataBase, INT& Pos );

	BOOL	GetDataBaseFromName( CDataBase& DataBase, CHAR * pstrName );
	BOOL	GetDataBaseFromCName( CDataBase& DataBase, CHAR * pstrCName );
	BOOL	GetDataBaseFromID( CDataBase& DataBase, UINT uID );

	// 复制数据库
	BOOL	DuplicateDataBase( UINT UlIDSource, UINT UlIDTarget, char* strDBName=NULL, BOOL bOverWrite = TRUE );
	// 在线添加数据库对象到MDB中(用于在线复制一个新数据库等)
	BOOL	AddDataBaseOnLine( CDataBase* pDataBase );
	
	BOOL DeleteDataBase( UINT UlIDTarget, char* strDBName );

	CDataBase* FindDataBase( UINT ulDBID );

public:
	INT			GetDataBaseMaxNum( );
	INT			GetRtdbStatus( );

	// 设置主服务器,如果没有修改则返回false,如果修改了则返回true
	NODE_INFO * GetServerInfo( );
	int 		GetServerIndex( char *pNodeName=NULL );		//取服务器序号, -1表示不是服务器

	char *	 	GetHostName( );
	void	 	GetSysIniFile( char* szIniFile );
	void	 	GetDataPath( char* szDataPath );

	bool	IfNet1Active( );
	bool	IfNet2Active( );
	INT		GetCurrentNetUse( );
	CHAR *	GetNet1IP( );
	CHAR *	GetNet2IP( );
	CHAR *	GetIP1( );
	CHAR *	GetIP2( );

	UINT	GetThisNodeID();
	void	SetThisNodeID(UINT nNodeID);
	UINT	GetMsgID();
	UINT	GetProcessID32();
	int		GetProcessIndex();

	UINT	GetSysUserID();
	void	SetSysUserID(UINT nUserID);
	char *	GetSysUserName();
	void	SetSysUserName(char * cName);
	UINT	GetRights(int iIndex);
	void	SetRights(int iIndex,UINT nRightsFlag);
	void	GetLoginUseTime(SYSTEMTIME & tLoginUse);
	void	SetLoginUseTime(SYSTEMTIME & tLoginUse);

	// 检查用户是否拥有指定的权限
	// uUserID=0, 指当前用户
	bool	CheckRights(UINT nRight, UINT uUserID=0);
	// 获取某个节点的当前用户ID,失败返回0
	UINT	GetNodeUserID(UINT uNodeID);
	// 用户登录
	void	UserLogin(UINT uUserID);
	// 注销当前用户
	void	UserLogout();

	// 网络复制文件
	BOOL	NetCopyFile(UINT uNodeID,char * pszSourceFile,char * pszTargetFile,bool bBak=false);
	//更新资料使用情况 tjs 2015-12-19
	void updateResourceUsage();
	//更新节点表时间参数 fs 2016-2-3
	void updateNodeTabPara();
	//更新进程表参数 fs 2016-2-4
	void updateProcessTabPara(int nProcessType, BOOL bFirstStart);
	void cleanProcessStatus(int nProcessType);
	void cleanNodeTabPara();
	//增加获取实时库状态接口 wenjp 2016.5.19
	int GetMasterStatus();
	//保存断面数据 wenjp
	bool SaveDumpData();


public:
	BOOL	AddDataBase( CDataBase * pDataBase );
	BOOL	AddTable( CDBTable & DBTable );
	BOOL	AddField( CDBField & DBField );

	// 注意这两个函数不是给应用使用的
	BOOL	UpdateDBInfo(CHAR * pDBName, CDbAttr * pDbInfo);
	BOOL	UpdateDBInfo(UINT uDBID, CDbAttr * pDbInfo);

public:
	void	SetRtdbStatus(INT nStatus);
	void	SetNet1Active(bool bActive);
	void	SetNet2Active(bool bActive);
	void	SetCurrentNetUse(INT iCuttentNetUse);

public:
	// 根据表(英文)名称取得系统表对象
	BOOL	GetSystemTable(CDBTable & table,CHAR * pTabName);
	// 根据表的ID取得系统表对象
	BOOL	GetSystemTable(CDBTable & table,UINT uTableID);

	// 取得系统进程表中的进程个数
	int		GetProcessCount( );
	// 取得系统进程表的首记录地址
	SYSTEM_Process * GetTabProcessAddr();
	int		GetProcessType(INT nProcessType);

	// 在进程表查找指定进程ID的进程,默认参数用于找本进程
	int		FindProcess(int nProcessID = -1,UINT uNodeID = 0);
	// 在进程表查找指定进程类型的进程, 默认参数用于找本进程
	SYSTEM_Process * FindProcessByProType(int iProcessType, UINT uNodeID=0);
	SYSTEM_Process * GetProcess(int nProcessID = -1,UINT uNodeID = 0);
	//找运行在服务器上的RtServer
	void FindProcessOnServer( SYSTEM_Process* pProcess, int iProcessType=pt_RTSERVER );

	//进程为主服务的判断,返回true应为主服务 false应为备服务
	//参数说明:bOnServer运行在服务器上的进程判断,还是全网进程判断
	//服务进程定时调用该函数,根据返回值决定进程行为
	bool	CheckProcessMain(bool bOnServer=true);
	//手动切换服务,如果为主切换为备.如果为备切换为主
	//参数说明:bOnServer服务器切换,还是全网进程切换
	void	SwitchService(char *pHostName=NULL, int iProcessType=pt_RTSERVER);
	void	SwitchService(UINT uNetNodeID, int iProcessType);
	void    ResetTcpChannel();

/////////////////////////////////////////////////////////////////////////////
// 节点表的一些接口函数
public:
	// 判断本节点是否是服务器节点.返回1:主服务器、返回2:备服务器、返回0:非服务器
	int		IsServerNode(char * pszNodeName=NULL);
	// 检查节点表中是否有指定名称的节点
	bool	IsNetNodeExist(char * pszNodeName);
	// 在节点表中根据节点机器名查找节点(如果是找本节点,可以取pszNodeName为空)
	bool	GetNetNodeByName(char * pszNodeName,SYSTEM_NetNode & node);
	bool	GetNetNodeByName(char * pszNodeName,NODE_INFO & node);
	// 在节点表中根据节点ID查找节点(如果是找本节点,可以取uNodeID为0)
	SYSTEM_NetNode * GetNetNodeByID(UINT uNodeID);
	void	SetLoginUserIDForThisNode();

/////////////////////////////////////////////////////////////////////////////
// 消息表的一些接口函数
public:
	// 向全网指定类型的进程发送一条消息
	void	SendMessageNet(INT iProcessType,INT iMsgType,INT iMsgSize=0,CHAR * pBuffer=NULL);
	void	SendMessage(INT iProcessType,INT iMsgType,INT iMsgSize=0,CHAR * pBuffer=NULL);
	void	SendMessage(INT iSourceProcessID, INT iTargetProcessID, INT iProcessType,INT iMsgType,INT iMsgSize=0,CHAR * pBuffer=NULL);
	// 向指定记录号(ID32)的进程发送一条消息
	void	SendMessageTo(UINT uProcessID32,int iMsgType,int iMsgSize=0,char * pBuffer=NULL);
	// 取出指定序号的进程的优先级最高的一条消息
	tabMessage* GetPriorityMessage(int iProcessIndex, int iHandleIndex);
	// 发送本地消息
	void	SendLocalMsg(tabMessage * pTabMessage,bool bCheckLoop = false);
private:
	void NotifyProcessGetMessage(int iEventIndex,tabMessage * pTabMessage);
/////////////////////////////////////////////////////////////////////////////
// 公式计算返回true或false的函数
public:
	bool	CheckCondition(UINT uFormulaID);

/////////////////////////////////////////////////////////////////////////////
//** 实时库与数据源(关系数据库/文件)之间的交互接口
public:
	// 保存实时库数据(所有参数都使用缺省值或配置值)
	bool	SaveData(bool bBroadcast);

	bool	SaveSnapshot(const char * pszFilePath);
	bool	LoadSnapshot(const char * pszFilePath,bool bBroadCast = true);

	// 载入实时库结构定义和数据(文件/关系数据库)
	BOOL	LoadRtdbStructure();
	BOOL	LoadRtdbData();

	BOOL	SaveDataTo(bool bSaveToFile);

	//向商用库中创建实时库表(使用前提:先以文件方式启动实时库)
	bool	CreateRtdb2DB();
private:
	CRtdbAttr *			GetRealRtdbInfo( );
	CDbAttr *			GetDbInfoAddr( );
	BOOL *				GetDbInfoIndexAddr( );
	tabMessage *		GetTabMessageAddress( );
	ProcessMessageInfo*	GetReadMessagePosAddress( );

	BOOL	CreateMem(BOOL bLoadFromMapFile = FALSE);
	BOOL	CreateDBMem( );

	void	SetNet1IP( CHAR * pIP1 );
	void	SetNet2IP( CHAR * pIP2 );
	CHAR *	GetNet1Mask( );
	void	SetNet1Mask( CHAR * pMask1 );
	CHAR *	GetNet2Mask( );
	void	SetNet2Mask( CHAR * pMask2 );

	void	GetNet1Time(SYSTEMTIME & time);
	void	GetNet2Time(SYSTEMTIME & time);
	void	SetNet1Time(SYSTEMTIME & time);
	void	SetNet2Time(SYSTEMTIME & time);
	BOOL	ReadConfigInfo();

	INT		GetWriteMessagePos( );
	void	SetWriteMessagePos( INT iIndex );

	bool	CheckLocalNet();
	bool	NodeStartInit( );
	// 杀死属于某个节点的所有进程
	void	KillProcessOfNode(UINT uNodeID);
	// 为所有节点创建进程占位记录
	void	CreateFakeProcess();
	// 从服务器取实时库数据
	bool	GetDataFromServer();

private:
	CDataBase * m_pDataBase[DATABASE_MAX_NUM];
	UINT		m_uProcessID32;
	CDataBase	m_SystemDB;
	CMutexEx	m_MutexEx;
	CProcessEvent m_ProcessEvent[pt_Total];
	CMutexEx	m_MsgMutexEx[pt_Total];
	
	CShareMemory m_ShareMemory;	// 共享内存
	
	CRtdbDefFileXml m_RtdbDefFielXml;//xml配置文件
	CComputerWatch m_ComputerWatch;
	CLogUtil *m_Util_MultiDB;
	char m_LastError[300];//打印错误使用
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_MULTIPLE_DATABASE_MANAGER_H_2002_10_24__
