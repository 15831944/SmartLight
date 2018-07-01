#include "CFuncUtil.h"
#include "CBrowseDir.h"
#include "FEPFuncUtil.h"
CFuncUtil::CFuncUtil(const char *strAppPathName,const char *strAppFileSubfix)
{
	strcpy(m_strAppPathName,strAppPathName); //例如:agc
	strcpy(m_strAppFileSubfix,strAppFileSubfix);//例如agclogfile.log
	initLog();
}
CFuncUtil::~CFuncUtil()
{

}
void CFuncUtil::GetLogPathname()
{
	memset(m_strLogFile,0,sizeof(m_strLogFile));
	char strPath[10] = {0};
	char *pWorkPath;
	char strMsg[200] = {0};
#ifdef UNIX_ENV
	pWorkPath = getenv( HOME_NAME);

	if( pWorkPath == NULL )
		strcpy( strMsg, HOME_PATH );
	else
		strcpy( strMsg, pWorkPath );
	strcpy(strPath,"/");
#else
	pWorkPath=getenv(HOME_NAME);
	if(pWorkPath==NULL)
	{
		strcpy( strMsg, HOME_PATH_WINDOW );
	}
	strcpy( strMsg, pWorkPath );
	strcpy(strPath,"\\");
#endif
	sprintf(m_strLogPath,"%s%s%s%s%s%s%s%s",strMsg,strPath,
		FEP_PATH_DATA,strPath,FEP_PATH_MESSAGE,
		strPath,m_strAppPathName,strPath);
	CheckPathExit(m_strLogPath);
}
void CFuncUtil::GetLogFilename()
{
	int iDay = 0;
	iDay = GetLocalDay();

	SYSTEMTIME sysTime;
	GetLocalTime(sysTime);

/*
	char hour[20];
	memset(hour,NULL,sizeof(hour));
	switch(sysTime.wHour)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		strcpy_s(hour,sizeof(hour),"00-04");
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		strcpy_s(hour,sizeof(hour),"04-08");
		break;
	case 8:
	case 9:
	case 10:
	case 11:
		strcpy_s(hour,sizeof(hour),"08-12");
		break;
	case 12:
	case 13:
	case 14:
	case 15:
		strcpy_s(hour,sizeof(hour),"12-16");
		break;
	case 16:
	case 17:
	case 18:
	case 19:
		strcpy_s(hour,sizeof(hour),"16-20");
		break;
	case 20:
	case 21:
	case 22:
	case 23:
		strcpy_s(hour,sizeof(hour),"20-24");
		break;

	}
	*/
	//sprintf(m_strLogFile, "%s%04d-%02d-%02d(%s)-%s", m_strLogPath,sysTime.wYear,sysTime.wMonth,sysTime.wDay,hour,m_strAppFileSubfix);
	
	sprintf(m_strLogFile, "%s%04d-%02d-%02d-%s", m_strLogPath,sysTime.wYear,sysTime.wMonth,sysTime.wDay,m_strAppFileSubfix);
	if(iDay != m_iLogDay)
	{
		m_iLogDay = iDay;
		m_bRenewLogFile = true;
		delOldLogFile();//删除文件
	}
}
void CFuncUtil::initLog()
{
	m_iLogDay = GetLocalDay();
	m_bRenewLogFile = false;
	GetLogPathname();
	delOldLogFile();
}
void CFuncUtil::CheckPathExit(char* pStrPath)
{
	int iCD = 0;
	int i = 0;

#ifdef UNIX_ENV
	iCD = chdir(pStrPath);
#else
	int n=strlen(pStrPath);
	for(i = 0;i<n ;i++)
	{
		if(pStrPath[i] == '/')
			pStrPath[i] = '\\';
	}
	iCD = _chdir(pStrPath);
#endif

	if(iCD >= 0)
		return;

#ifdef UNIX_ENV
	iCD = mkdir(pStrPath,S_IRWXU|S_IRWXG|S_IRWXO);
#else
	iCD = _mkdir(pStrPath);
#endif

	if(iCD <0)
	{		
		printf("\n:目录创建失败！");
	}
}
void CFuncUtil::CheckDataPathExit()
{
	char *pWorkPath;
	char strMsg[200] = {0};
	char strPathName[200] = {0};
	char strPath[10] = {0};
#ifdef UNIX_ENV
	pWorkPath = getenv( HOME_NAME);

	if( pWorkPath == NULL )
		strcpy( strMsg, HOME_PATH );
	else
		strcpy( strMsg, pWorkPath );
	strcpy(strPath,"/");
#else
	pWorkPath=getenv(HOME_NAME);
	if(pWorkPath==NULL)
	{
		printf("\n envirnoment variable CSC2100_HOME is not set,please set it first");
		return;
	}
	strcpy( strMsg, pWorkPath );
	strcpy(strPath,"\\");
#endif
	sprintf(strPathName,"%s%s%s",strMsg,strPath,FEP_PATH_DATA);
	CheckPathExit(strPathName);

	sprintf(strPathName,"%s%s%s%s%s",strMsg,strPath,
		FEP_PATH_DATA,strPath,FEP_PATH_MESSAGE);
	CheckPathExit(strPathName);

	sprintf(strPathName,"%s%s%s%s%s%s%s",strMsg,strPath,
		FEP_PATH_DATA,strPath,FEP_PATH_MESSAGE,
		strPath,FEP_PATH_CHANNEL);
	CheckPathExit(strPathName);

	sprintf(strPathName,"%s%s%s%s%s%s%s",strMsg,strPath,
		FEP_PATH_DATA,strPath,FEP_PATH_MESSAGE,
		strPath,FEP_PATH_TERMINAL);
	CheckPathExit(strPathName);
}
void CFuncUtil::WriteLogfile(int iLinkID,char* strMsg)
{
	SYSTEMTIME sysTime;
	GetLocalTime(sysTime);
	char strTime[30];
	/*sprintf_s(strTime,"%04d-%02d-%02d %02d:%02d:%02d.%03d",
		sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);
		*/
	sprintf(strTime,"%02d:%02d:%02d.%03d",sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);
		
	FILE *  logRecord = NULL;        /* message record */
	GetLogFilename();
	if(m_bRenewLogFile)
		logRecord = fopen(m_strLogFile, "w");
	else
		logRecord = fopen(m_strLogFile, "a");
	if(logRecord == NULL)
		return;
	m_bRenewLogFile = false;

	if(iLinkID >= 0)
		fprintf(logRecord,"linkid %d:%s %s\n",iLinkID,strTime,strMsg);
	else
		fprintf(logRecord,"%s %s\n",strTime,strMsg);
	fclose(logRecord);

}
/*
* get local calendar time
*/
int CFuncUtil::GetLocalDay()
{
#ifdef UNIX_ENV
	struct timeval Time;
	if(gettimeofday(&Time, NULL) < 0)
		return FALSE;

	tm * pTime = localtime( &(Time.tv_sec) );
	if( pTime == NULL )
		return FALSE;

	return pTime->tm_mday;
#else
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	return sysTime.wDay;
#endif
}
void CFuncUtil::delOldLogFile()
{
		//构造类对象
	CDelFileInDir statdir;
	//设置要遍历的目录
	if (!statdir.SetInitDir(m_strLogPath))
	{
		puts("目录不存在。");
		return;
	}

	//开始遍历
	statdir.BeginBrowse("*.log");
}