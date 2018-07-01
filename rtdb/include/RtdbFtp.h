/////////////////////////////////////////////////////////////////////////////
// RtdbFile.h : 实时库支持的文件传送,目前的文件传送对大文件还存在效率低的问题
// ftp的根目录是CSC2000, 所有文件必须在CSC2000下
// 如:CSC2000=/export/home/app/csc2000, 传送文件/export/home/app/csc2000/project/1.txt
// 到服务器 Put("/project/1.txt");
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_RTDB_FTP_RW_H_2006_05_15__
#define __AFX_RTDB_FTP_RW_H_2006_05_15__
/////////////////////////////////////////////////////////////////////////////
#define FTP_STATE_NOUSE		0
#define FTP_STATE_WAIT		1
#define FTP_STATE_SUCCESS	2
#define FTP_STATE_FAIL		3

#define FTP_TYPE_PUT		1
#define FTP_TYPE_GET		2
#define FTP_TYPE_FILELIST	3
#define FTP_TYPE_DELETE		4
#define FTP_TYPE_RENAME		5
#define FTP_TYPE_MKDIR		6

#define FTP_STATE_MAXNUM	100

class AFX_EXT_CLASS CRtdbFtp
{
public:
	CRtdbFtp(CMultiDataBase * pMDB, UINT uNetNodeID=0, bool bRtServer=false);
	virtual ~CRtdbFtp();

	CMultiDataBase* m_pMDB;
	UINT			m_iFtpServer; 
	
	CShareMemory	m_ShareMemory;	// 共享内存
	int				m_nFtpBufKey;
	bool			m_bRtServer;

	typedef struct
	{
		char strFileName[256];
		SYSTEMTIME	BeginTime;
		int  iState;
	} _FtpState;
	char*			m_pDataArea;

	//CMutexEx		m_SendMutex;
	
	//设置Ftp文件传送状态
	bool	SetState(char *strFileName, int iState);
	//获取Ftp文件传送状态
	int		GetState(char *strFileName);

	bool    IsActiveFtpServer();

	bool    Put(char *strFileName, bool bAllNode);

	//在本地创建指定目录
	bool    MkLocalDir(char *strDir);
	
public:
	bool    SetFtpServer(UINT uNetNodeID);

	//获取文件
	bool	Get(char *strFileName);
	//上传文件
	bool	Put(char *strFileName);

	//将指定文件发送到系统内的所有活动节点
	bool	Sync(char *strFileName);

	//删除文件
	bool	Delete(char *strFileName);

	//重命名文件
	bool	Rename(char *strFileName, char *strNewFileName);

	//返回某个目录下的所有文件列表
	bool	ListFile(char *strDir);
	
	//在本地和Ftp服务器上创建指定目录
	bool    MkDir(char *strDir);
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_RTDB_FTP_RW_H_2006_05_15__
