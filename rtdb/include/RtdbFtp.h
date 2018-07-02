/////////////////////////////////////////////////////////////////////////////
// RtdbFile.h : ʵʱ��֧�ֵ��ļ�����,Ŀǰ���ļ����ͶԴ��ļ�������Ч�ʵ͵�����
// ftp�ĸ�Ŀ¼��CSC2000, �����ļ�������CSC2000��
// ��:CSC2000=/export/home/app/csc2000, �����ļ�/export/home/app/csc2000/project/1.txt
// �������� Put("/project/1.txt");
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
	
	CShareMemory	m_ShareMemory;	// �����ڴ�
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
	
	//����Ftp�ļ�����״̬
	bool	SetState(char *strFileName, int iState);
	//��ȡFtp�ļ�����״̬
	int		GetState(char *strFileName);

	bool    IsActiveFtpServer();

	bool    Put(char *strFileName, bool bAllNode);

	//�ڱ��ش���ָ��Ŀ¼
	bool    MkLocalDir(char *strDir);
	
public:
	bool    SetFtpServer(UINT uNetNodeID);

	//��ȡ�ļ�
	bool	Get(char *strFileName);
	//�ϴ��ļ�
	bool	Put(char *strFileName);

	//��ָ���ļ����͵�ϵͳ�ڵ����л�ڵ�
	bool	Sync(char *strFileName);

	//ɾ���ļ�
	bool	Delete(char *strFileName);

	//�������ļ�
	bool	Rename(char *strFileName, char *strNewFileName);

	//����ĳ��Ŀ¼�µ������ļ��б�
	bool	ListFile(char *strDir);
	
	//�ڱ��غ�Ftp�������ϴ���ָ��Ŀ¼
	bool    MkDir(char *strDir);
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_RTDB_FTP_RW_H_2006_05_15__
