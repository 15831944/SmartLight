#ifndef  _CLogUtil_H
#define  _CLogUtil_H
#include "gSystemDef.h"
class AFX_EXT_CLASS CLogUtil
{
public:
	CLogUtil(const char *strAppPathName,const char *strAppFileSubfix);
	virtual ~CLogUtil();
	void initLog();
	void GetLogPathname();
	void GetLogFilename();
	void CheckDataPathExit();
	void CheckPathExit(char* pStrPath);
	void WriteLogfile(int iLinkID,char* strMsg);
	int GetLocalDay();
private:
	char m_LastError[200];//打印错误使用
	char m_strLogFile[200];//用于日志保存
	char m_strLogPath[100];
	int m_iLogDay;
	bool m_bRenewLogFile;
	char m_strAppPathName[24]; //例如:agc
	char m_strAppFileSubfix[48];//例如agclogfile.log

private:
	char HOME_NAME[20];
	char HOME_PATH[50];
	char HOME_PATH_WINDOW[50];
	char FEP_PATH_DATA[20];
	char FEP_PATH_MESSAGE[20];
	char FEP_PATH_TERMINAL[20];

};
#endif
