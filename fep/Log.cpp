#include "Log.h"
Log::Log()
{
 m_logfile=NULL;
}
Log::~Log()
{
	if(m_logfile!=NULL)
		fclose(m_logfile);
}
FILE *Log::getFile()
{
 //如果为今天的不需要新建,如果为不是今天的需要新建一下
	//TODO:
	if(m_logfile==NULL)
	{
		initLogPath();
	}
	return m_logfile;

}

void Log::initLogPath()
{
	char * pFilePath = getenv( "CCSOSD" );

	char szIniFile[MAX_PATH];
	if( pFilePath == NULL )
		pFilePath = getcwd(szIniFile, MAX_PATH);

	time_t t = time(NULL);
	tm* aTm = localtime(&t);

	
	sprintf_s(szIniFile,sizeof(szIniFile),"%s/log/%.4d-%.2d-%.2d.log",pFilePath,aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday);

	m_logfile= fopen(szIniFile,"a+");
	if(m_logfile==NULL)
	{
		TRACE("open log file failed!\n");
	}
}

