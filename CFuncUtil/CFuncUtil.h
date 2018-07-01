#ifndef  _FuncUtil_H
#define  _FuncUtil_H
#include "CFuncUtilDef.h"
class __Q_CFUNCUTIL_EXPORT CFuncUtil
{
public:
	CFuncUtil(const char *strAppPathName,const char *strAppFileSubfix);
	virtual ~CFuncUtil();
	void initLog();
	void GetLogPathname();
	void GetLogFilename();
	void CheckDataPathExit();
	void CheckPathExit(char* pStrPath);
	void WriteLogfile(int iLinkID,char* strMsg);
	int GetLocalDay();
	//删除日志文件
	void delOldLogFile();
private:
	char m_LastError[200];//打印错误使用
	char m_strLogFile[200];//用于日志保存
	char m_strLogPath[100];
	int m_iLogDay;
	bool m_bRenewLogFile;
	char m_strAppPathName[24]; //例如:agc
	char m_strAppFileSubfix[48];//例如agclogfile.log
};
#endif
