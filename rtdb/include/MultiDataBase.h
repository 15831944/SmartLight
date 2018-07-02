/****************************************************************************
 * MultiDataBase.h : �������ڹ��������ݿ����CMultiDataBase.				*
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

#define RTDB_NULL		0 // ʵʱ��δ��ʼ��
#define RTDB_STRUCTURE	1 // ʵʱ��ṹ�ѳ�ʼ��
#define RTDB_DATAINIT	2 // ʵʱ�������ѳ�ʼ��

#define EVENT_NOTIFY	1 // ��Ϣ����:�¼�����
//#define TCP_NOTIFY		2 // ��Ϣ����:����TCP����

/////////////////////////////////////////////////////////////////////////////
typedef struct _rtdb_attr
{
//	CHAR		m_pszSysIniFile[96];	// ϵͳ�����ļ���ȫ·��
//	CHAR		m_pszDataPath[96];		// ϵͳ�����ļ����·��

	NODE_INFO	m_Server[MAX_SERVER_NUM];	// 4����������Ϣ
	CHAR		m_pszHostName[24];	// �����ڵ�����
	CHAR		m_strIP1[16];		// ���ڵ�1��IP��ַ
	CHAR		m_strIP2[16];		// ���ڵ�2��IP��ַ
	CHAR		m_strNet1IP[16];	// ���ڵ�1������
	CHAR		m_strNet2IP[16];	// ���ڵ�2������
	CHAR		m_strNet1Mask[16];	// ���ڵ�1��IP����
	CHAR		m_strNet2Mask[16];	// ���ڵ�2��IP����

	INT			m_iDBMaxNum;		// ������������ݿ����
	INT			m_iRtdbStatus;		// ʵʱ��״̬
//	UINT		m_uCurMsgID;		// ��ǰ��Ϣ��ID��
	UINT		m_nThisNodeID;		// ���ڵ��ڽڵ���е�ID
	INT			m_iCurrentNetUse;	// ��ǰʹ�õ�����
	bool		m_bNet1Active;		// 1��״̬
	bool		m_bNet2Active;		// 2��״̬
	SYSTEMTIME	m_Net1GetTime;		// 1�����¼�������ʱ��
	SYSTEMTIME	m_Net2GetTime;		// 2�����¼�������ʱ��

	UINT		m_nSysUserID;		// ϵͳ��ǰ�û�ID
	CHAR		m_cSysUserName[16];	// ϵͳ��ǰ�û�����
	INT			m_nRightsFlag[4];	// ϵͳ��ǰ�û���Ȩ��
	SYSTEMTIME	m_tLoginUse;		// ϵͳ�û���¼ʹ��ʱ��
} CRtdbAttr;

class AFX_EXT_CLASS CMultiDataBase : protected CRtdbAttr
{
	friend class CCmdProcess;
public:
	CMultiDataBase();
	~CMultiDataBase();

/////////////////////////////////////////////////////////////////////////////
public:
	// ��RtServer����,��������������ٴ�������ʵʱ���ݿ�ϵͳ(���������Ƿ�����Ҳ�����ǿͻ���)
	bool	RtdbManagerInit(bool bLoadFromMapFile = false);
	// ���ɷ�RtServer����,��ʼ������������ͨѶģ��(���������Ƿ�����Ҳ�����ǿͻ���)
	bool	SocketInit(bool bRtServer = false);
	bool	SocketExit();
	// ��ȡ����������Ϣ
	void	GetNetLinkInfo(char* strLink);

	BOOL	AttachShareMemory( );
	void	DetachShareMemory( );
	void	DestroyMemory( );

	// ����ע��
	bool	Register(int nType,int iWnd,bool bFakeDel = true, int nLocalMax = 1);
	// ����ע��
	bool	UnRegister();

	// ���½��̱��б�������Ϣ
	bool	RefreshThisProcess( );

	bool	LocalNodeManager();
	bool	TotalNodeManager();
	void	DoOnStandby();
	int		GetSystemAlarmClass();
	int     GetNetHeartPeriod();	
	int     GetNodeHeartPeriod();	
	int     GetProcessHeartPeriod();	
	// ��������ʵʱ���ͷ�ļ�
	bool	BuildIncludeFile( LPSTR szPasPath );

	BOOL	GetFirstDataBasePostion( INT& Pos );
	BOOL	GetNext( CDataBase& DataBase, INT& Pos );

	BOOL	GetDataBaseFromName( CDataBase& DataBase, CHAR * pstrName );
	BOOL	GetDataBaseFromCName( CDataBase& DataBase, CHAR * pstrCName );
	BOOL	GetDataBaseFromID( CDataBase& DataBase, UINT uID );

	// �������ݿ�
	BOOL	DuplicateDataBase( UINT UlIDSource, UINT UlIDTarget, char* strDBName=NULL, BOOL bOverWrite = TRUE );
	// ����������ݿ����MDB��(�������߸���һ�������ݿ��)
	BOOL	AddDataBaseOnLine( CDataBase* pDataBase );
	
	BOOL DeleteDataBase( UINT UlIDTarget, char* strDBName );

	CDataBase* FindDataBase( UINT ulDBID );

public:
	INT			GetDataBaseMaxNum( );
	INT			GetRtdbStatus( );

	// ������������,���û���޸��򷵻�false,����޸����򷵻�true
	NODE_INFO * GetServerInfo( );
	int 		GetServerIndex( char *pNodeName=NULL );		//ȡ���������, -1��ʾ���Ƿ�����

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

	// ����û��Ƿ�ӵ��ָ����Ȩ��
	// uUserID=0, ָ��ǰ�û�
	bool	CheckRights(UINT nRight, UINT uUserID=0);
	// ��ȡĳ���ڵ�ĵ�ǰ�û�ID,ʧ�ܷ���0
	UINT	GetNodeUserID(UINT uNodeID);
	// �û���¼
	void	UserLogin(UINT uUserID);
	// ע����ǰ�û�
	void	UserLogout();

	// ���縴���ļ�
	BOOL	NetCopyFile(UINT uNodeID,char * pszSourceFile,char * pszTargetFile,bool bBak=false);
	//��������ʹ����� tjs 2015-12-19
	void updateResourceUsage();
	//���½ڵ��ʱ����� fs 2016-2-3
	void updateNodeTabPara();
	//���½��̱���� fs 2016-2-4
	void updateProcessTabPara(int nProcessType, BOOL bFirstStart);
	void cleanProcessStatus(int nProcessType);
	void cleanNodeTabPara();
	//���ӻ�ȡʵʱ��״̬�ӿ� wenjp 2016.5.19
	int GetMasterStatus();
	//����������� wenjp
	bool SaveDumpData();


public:
	BOOL	AddDataBase( CDataBase * pDataBase );
	BOOL	AddTable( CDBTable & DBTable );
	BOOL	AddField( CDBField & DBField );

	// ע���������������Ǹ�Ӧ��ʹ�õ�
	BOOL	UpdateDBInfo(CHAR * pDBName, CDbAttr * pDbInfo);
	BOOL	UpdateDBInfo(UINT uDBID, CDbAttr * pDbInfo);

public:
	void	SetRtdbStatus(INT nStatus);
	void	SetNet1Active(bool bActive);
	void	SetNet2Active(bool bActive);
	void	SetCurrentNetUse(INT iCuttentNetUse);

public:
	// ���ݱ�(Ӣ��)����ȡ��ϵͳ�����
	BOOL	GetSystemTable(CDBTable & table,CHAR * pTabName);
	// ���ݱ��IDȡ��ϵͳ�����
	BOOL	GetSystemTable(CDBTable & table,UINT uTableID);

	// ȡ��ϵͳ���̱��еĽ��̸���
	int		GetProcessCount( );
	// ȡ��ϵͳ���̱���׼�¼��ַ
	SYSTEM_Process * GetTabProcessAddr();
	int		GetProcessType(INT nProcessType);

	// �ڽ��̱����ָ������ID�Ľ���,Ĭ�ϲ��������ұ�����
	int		FindProcess(int nProcessID = -1,UINT uNodeID = 0);
	// �ڽ��̱����ָ���������͵Ľ���, Ĭ�ϲ��������ұ�����
	SYSTEM_Process * FindProcessByProType(int iProcessType, UINT uNodeID=0);
	SYSTEM_Process * GetProcess(int nProcessID = -1,UINT uNodeID = 0);
	//�������ڷ������ϵ�RtServer
	void FindProcessOnServer( SYSTEM_Process* pProcess, int iProcessType=pt_RTSERVER );

	//����Ϊ��������ж�,����trueӦΪ������ falseӦΪ������
	//����˵��:bOnServer�����ڷ������ϵĽ����ж�,����ȫ�������ж�
	//������̶�ʱ���øú���,���ݷ���ֵ����������Ϊ
	bool	CheckProcessMain(bool bOnServer=true);
	//�ֶ��л�����,���Ϊ���л�Ϊ��.���Ϊ���л�Ϊ��
	//����˵��:bOnServer�������л�,����ȫ�������л�
	void	SwitchService(char *pHostName=NULL, int iProcessType=pt_RTSERVER);
	void	SwitchService(UINT uNetNodeID, int iProcessType);
	void    ResetTcpChannel();

/////////////////////////////////////////////////////////////////////////////
// �ڵ���һЩ�ӿں���
public:
	// �жϱ��ڵ��Ƿ��Ƿ������ڵ�.����1:��������������2:��������������0:�Ƿ�����
	int		IsServerNode(char * pszNodeName=NULL);
	// ���ڵ�����Ƿ���ָ�����ƵĽڵ�
	bool	IsNetNodeExist(char * pszNodeName);
	// �ڽڵ���и��ݽڵ���������ҽڵ�(������ұ��ڵ�,����ȡpszNodeNameΪ��)
	bool	GetNetNodeByName(char * pszNodeName,SYSTEM_NetNode & node);
	bool	GetNetNodeByName(char * pszNodeName,NODE_INFO & node);
	// �ڽڵ���и��ݽڵ�ID���ҽڵ�(������ұ��ڵ�,����ȡuNodeIDΪ0)
	SYSTEM_NetNode * GetNetNodeByID(UINT uNodeID);
	void	SetLoginUserIDForThisNode();

/////////////////////////////////////////////////////////////////////////////
// ��Ϣ���һЩ�ӿں���
public:
	// ��ȫ��ָ�����͵Ľ��̷���һ����Ϣ
	void	SendMessageNet(INT iProcessType,INT iMsgType,INT iMsgSize=0,CHAR * pBuffer=NULL);
	void	SendMessage(INT iProcessType,INT iMsgType,INT iMsgSize=0,CHAR * pBuffer=NULL);
	void	SendMessage(INT iSourceProcessID, INT iTargetProcessID, INT iProcessType,INT iMsgType,INT iMsgSize=0,CHAR * pBuffer=NULL);
	// ��ָ����¼��(ID32)�Ľ��̷���һ����Ϣ
	void	SendMessageTo(UINT uProcessID32,int iMsgType,int iMsgSize=0,char * pBuffer=NULL);
	// ȡ��ָ����ŵĽ��̵����ȼ���ߵ�һ����Ϣ
	tabMessage* GetPriorityMessage(int iProcessIndex, int iHandleIndex);
	// ���ͱ�����Ϣ
	void	SendLocalMsg(tabMessage * pTabMessage,bool bCheckLoop = false);
private:
	void NotifyProcessGetMessage(int iEventIndex,tabMessage * pTabMessage);
/////////////////////////////////////////////////////////////////////////////
// ��ʽ���㷵��true��false�ĺ���
public:
	bool	CheckCondition(UINT uFormulaID);

/////////////////////////////////////////////////////////////////////////////
//** ʵʱ��������Դ(��ϵ���ݿ�/�ļ�)֮��Ľ����ӿ�
public:
	// ����ʵʱ������(���в�����ʹ��ȱʡֵ������ֵ)
	bool	SaveData(bool bBroadcast);

	bool	SaveSnapshot(const char * pszFilePath);
	bool	LoadSnapshot(const char * pszFilePath,bool bBroadCast = true);

	// ����ʵʱ��ṹ���������(�ļ�/��ϵ���ݿ�)
	BOOL	LoadRtdbStructure();
	BOOL	LoadRtdbData();

	BOOL	SaveDataTo(bool bSaveToFile);

	//�����ÿ��д���ʵʱ���(ʹ��ǰ��:�����ļ���ʽ����ʵʱ��)
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
	// ɱ������ĳ���ڵ�����н���
	void	KillProcessOfNode(UINT uNodeID);
	// Ϊ���нڵ㴴������ռλ��¼
	void	CreateFakeProcess();
	// �ӷ�����ȡʵʱ������
	bool	GetDataFromServer();

private:
	CDataBase * m_pDataBase[DATABASE_MAX_NUM];
	UINT		m_uProcessID32;
	CDataBase	m_SystemDB;
	CMutexEx	m_MutexEx;
	CProcessEvent m_ProcessEvent[pt_Total];
	CMutexEx	m_MsgMutexEx[pt_Total];
	
	CShareMemory m_ShareMemory;	// �����ڴ�
	
	CRtdbDefFileXml m_RtdbDefFielXml;//xml�����ļ�
	CComputerWatch m_ComputerWatch;
	CLogUtil *m_Util_MultiDB;
	char m_LastError[300];//��ӡ����ʹ��
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_MULTIPLE_DATABASE_MANAGER_H_2002_10_24__
