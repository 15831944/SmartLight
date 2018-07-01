#ifndef	WRITE_LOG_H
#define	WRITE_LOG_H

#define	MAX_LOG_SIZE	200

#define	LOG_TYPE_INVALID	0
#define	LOG_TYPE_COMMON		1<<0

#define	LOG_NOT_SAVE_1		1<<30
#define	LOG_NOT_SAVE_2		1<<31

#define	LOG_NOT_SAVE	( LOG_NOT_SAVE_1 | LOG_NOT_SAVE_2 )

#include "LogMutex.h"

typedef	struct _COMMON_LOG
{
	char strUser[12];
	SYSTEMTIME tm;
	int main_type;
	int sub_type;
	int scada_type;
	int scada_id;
	int unit_id;
	char strOperation[200];
	bool bMaintain;
}COMMON_LOG,*PCOMMON_LOG;

typedef	struct _LOG
{
public:
	int log_type;
	COMMON_LOG log;
public:
	_LOG()
	{
		log_type = LOG_TYPE_INVALID;
		memset(&log,0,sizeof(log));
	}
}LOG,*PLOG;

class AFX_EXT_CLASS CWriteLog
{
public:
	CWriteLog();
	virtual	~CWriteLog();

	bool OpenLog();
	bool IsLogEnable() { return m_bEnableLog; }
	void EnableLog(bool bEnable) { m_bEnableLog = bEnable; }

	void WriteLog(int main_type,int	sub_type,char* strUser,char* strOperation,int scada_type=-1,int	scada_id=0,int unit_id=0,bool bMaintain=false);

	LOG GetLog(int pos);
	void ClearLog(int pos);

    int GetCurWritePos();
private:
	void AddLog(LOG	log);
	bool	m_bEnableLog;

private:
	char * log_file;
	CLogMutex mutex;
	bool   map_opened;
#ifdef WINDOWS_ENV
	HANDLE hFile;
	HANDLE hLogMap;
	bool   file_opened;
#endif
};
#endif
