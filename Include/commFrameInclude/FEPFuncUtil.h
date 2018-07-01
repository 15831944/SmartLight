#ifndef _FEP_FUNCTION_UTIL_H_
#define _FEP_FUNCTION_UTIL_H_

#include "funstruct.h"
#include "Q_frameDefine.h"
union FloatType
{
    BYTE	bValue[4]; 
    float   fValue;
};

const char * const HOME_NAME = "CCSOSD";
const char * const HOME_PATH = "/home/work/ldns";
const char * const HOME_PATH_WINDOW = "d:\\ldns";
const char * const FEP_PATH_CONFIG = "/config/";
const char * const FEP_FILE_CONFIG = "fep.conf";
const char * const FEP_PATH_CONFIG_WINDOW = "\\config\\";
const char * const FEP_PATH_LOGFILE = "feplogfile.log";
const char * const FEP_PATH_DIGLOGFILE = "diglogfile.log";

const char * const FEP_PATH_DATA = "data";
const char * const FEP_PATH_MESSAGE = "message";
const char * const FEP_PATH_TERMINAL = "terminal";
const char * const FEP_PATH_CHANNEL = "commlink";
const char * const FEP_FILE_TRANS = "trans.conf";
const char * const FEP_PATH_LOGPATH = "log";
const char * const FEP_PATH_DIGLOGPATH = "diglog";

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T 
#endif
class  __Q_COMMFRAME_EXPORT  CFEPFuncUtil
{
public:
	CFEPFuncUtil();

private:
	int m_iLogDay;
	int m_iDigLogDay;
	BOOL m_bRenewLogFile;
	BOOL m_bRenewDigLogFile;
	char m_strLogFile[200];
	char m_strDigLogFile[200];
	char m_strLogPath[100];
	char m_strDigLogPath[100];
	void StatisClear(struct UtilStatis *statis);
	void StatisCheck(struct UtilStatis *statis);
	void GetLogPathname();
	void GetLogFilename();
	void GetDigLogFilename();

	int FrValue(float x, int *exp);
	float LdValue(int a, int exp);
	int IsBigEndian();
	unsigned short CalCrc16(BYTE *puchMsg, int usDataLen);
	char ByteRotate(char data);
	
	int GetCtime1(SYSTEMTIME *p, char *buf, int len);
	int GetCtime2(SYSTEMTIME *c, char *buffer, int length);
	int GetCtime3(SYSTEMTIME *p, char *buf, int len);
	int GetLocalCtime2(char *buffer, int len);
	int GetLocalCtime3(char *buffer, int size);
	void GetCalendarFromMSec(ULONG64 msec, SYSTEMTIME *p);

	void SystemTimeToTIME( SYSTEMTIME SysTime, TIME& Time );

public:	
	void GetTimeBuffer(BYTE *pTimeBuffer);
	bool TIMEToSystemTime( TIME Time, SYSTEMTIME& SysTime );
	void CheckDataPathExit();
	void CheckPathExit(char* pStrPath);
	int	 GetStructFromStream(struct UtilStrucs *struc, void *buffer, int buf_len);
	int  GetStreamFromStruct(void *buffer, int buf_size, struct UtilStrucs *struc);
	void FreeStrucs(struct UtilStrucs *p);
	struct UtilStrucs* AllocStrucs(int count, int *array_type, int *array_offset);
	BYTE CalSum(BYTE *data, int n);
	BYTE CalCrc8(BYTE *data, int n);
	void SetTimer(SYSTEMTIME *p);
	ULONG64 GetMSec();
	ULONG64 GetMSecFromCalendar(SYSTEMTIME *p);
	int GetLocalDay();
	void GetLocalTime(SYSTEMTIME *lpSystemTime);
	int GetLocalCtime1(char *buffer, int length);
	int	 StatisGet(struct UtilStatis *statis, float *rate_rx, float *rate_tx);
	void StatisAddRxByte(struct UtilStatis *statis, int bytes);
	void StatisAddRxRight(struct UtilStatis *statis, int bytes);
	void StatisAddTxByte(struct UtilStatis *statis, int bytes);
	void StatisAddTxRight(struct UtilStatis *statis, int bytes);
	void StatisInit(struct UtilStatis *statis, int count, int sec);
	void* HashRemove(UtilHash *hash, int key);
	void MsSleep(int msec);
	int IsTimeout(struct UtilTimer *t);
	void SetTimeout(struct UtilTimer *t, int msec);
	int ConnectNB(int sockfd, struct sockaddr *saptr, int salen, int msec);

	int HashInit(UtilHash *hash, int bucket);
	int HashAdd(UtilHash *hash, int key, void *item);
	void* HashGet(UtilHash *hash, int key);
	void* HashGetFirst(UtilHash *hash, int *key);
	void * HashGetNext(UtilHash *hash, int *key);
	int	 HashDestroy(UtilHash *hash);
	int HashMaxkey(UtilHash *hash);
	int HashCount(UtilHash *hash);
	void GetConfigPath(char* strPath);
	~CFEPFuncUtil();
	int IsMatch(const char *string, const char *pattern);
	int Daemon();
	bool IsFatalError();
	void	WriteLogfile(int iLinkID, char* strMsg);
	void	WriteDigLogfile(int iLinkID, char* strMsg);
private:
	float FloatAbs(float fValue);
#ifdef WINDOWS_ENV
	//int IsMatchWin(const char *string, const char *pattern);
#else
	//int IsMatchUnix(const char *string, const char *pattern);
	int hh_daemon();
#endif

};


#endif //_FEP_FUNCTION_UTIL_H_
