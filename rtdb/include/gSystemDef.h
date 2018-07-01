/////////////////////////////////////////////////////////////////////////////
// gSystemDef.h : include file for standard system include files
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_SYSTEM_DEFINE_OF_RTDBDLL_H_WW_2003_11_14__
#define __AFX_SYSTEM_DEFINE_OF_RTDBDLL_H_WW_2003_11_14__

/////////////////////////////////////////////////////////////////////////////
// 本机是Window类系统还是Unix类系统
//#define WINDOWS_ENV
//#define UNIX_ENV
/////////////////////////////////////////////////////////////////////////////
// 系统中是否有ALPHA64机器
// #define ALPHA_64_PLATFORM
/////////////////////////////////////////////////////////////////////////////
#define CMD_MAX_LENGTH			1400
/////////////////////////////////////////////////////////////////////////////
// 包含一些系统头文件
#ifdef WINDOWS_ENV
	#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

	#include <sys/types.h>
	#include <sys/stat.h>
	#include "stdio.h"
	#include "stdlib.h"
	#include "time.h"
	#include "memory.h"
	#include "string.h"
	#include "AfxMT.h"
	#include <winsock2.h>
	#include <direct.h>
	#include <string>
#endif
/////////////////////////////////////////////////////////////////////////////
#ifdef UNIX_ENV
	#define AFX_EXT_CLASS
	#define WINAPI

	#include <sys/types.h>
	#include <sys/stat.h>
	#include "stdio.h"
	#include "stdlib.h"
	#include <unistd.h>
	#include <fcntl.h>
	#include "string.h"
	#include <string>
	#include <signal.h>
	#include "memory.h"
	#include <sched.h>
	#include "pthread.h"
	#include <errno.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <sys/shm.h>
	#include <semaphore.h>
	#include <sys/mman.h>
	#include <sys/ipc.h>
	#include <sys/sem.h>
    #include <sys/time.h>
    #include <signal.h>

    #include <syslog.h>
    #include <errno.h>
    #include <sys/stat.h>

	#define MAX_PATH				255



#endif

/* * include for use oci * *
#ifdef UNIX_ENV
	#include "oratypes.h"
	#include "ociapr.h"
	#include "ocidem.h"
	#include "OciSql.h"
#endif
 * * * * * * * * * * * * * */
/////////////////////////////////////////////////////////////////////////////
// 统一定义所需的数据类型
#ifndef FALSE
	#define FALSE					0
#endif

#ifndef TRUE
	#define TRUE					1
#endif
/////////////////////////////////////////////////////////////////////////////
#undef	INT16
#undef	UINT16
#undef	SHORT
#undef	USHORT
#undef	INT
#undef	INT32
#undef	UINT
#undef	UINT32

#undef	WORD
#undef	DWORD
#undef	LONG
#undef	LONG32
#undef	ULONG
#undef	UINT
#undef	LONG64
#undef	ULONG64

#undef	BOOL

#undef	CHAR
#undef	UCHAR
#undef	BYTE

#undef	FLOAT
#undef	FLOAT32
#undef	REAL
#undef	DOUBLE
#undef	FLOAT64

typedef short  int				INT16;
typedef unsigned	short int	UINT16;
typedef short					SHORT;
typedef unsigned	short		USHORT;
typedef int						INT;
typedef int						INT32;
typedef unsigned	int			UINT;
typedef unsigned	int			UINT32;



typedef timeval		TIME;
typedef timeval	*	PTIME;
typedef timeval	*	LPTIME;

typedef unsigned short			WORD;
typedef unsigned long			DWORD;
typedef long					LONG;
typedef unsigned	long		ULONG;

typedef unsigned long ulYear;
typedef unsigned long ulMonth;
typedef unsigned long ulDay;
#ifdef WINDOWS_ENV
typedef	_int64					LONG64;
typedef unsigned	_int64		ULONG64;
#endif

#ifdef UNIX_ENV
typedef long					LONG32;
typedef unsigned	long		ULONG32;
typedef	long	long			LONG64;
typedef unsigned long	long	ULONG64;
#endif

typedef int						BOOL;

typedef char					CHAR;
typedef unsigned char			UCHAR;
typedef unsigned char			BYTE;

typedef float					FLOAT;
typedef float					FLOAT32;
typedef double					REAL;
typedef double					DOUBLE;
typedef double					FLOAT64;
/////////////////////////////////////////////////////////////////////////////
//typedef short					SHORT;
//typedef unsigned short			USHORT;
//typedef unsigned short			WORD;
//typedef int						INT;
//typedef unsigned int			UINT;
/////////////////////////////////////////////////////////////////////////////
// 统一一些名称
#ifdef WINDOWS_ENV
	#define MilliSleep		Sleep
	typedef int				socklen_t;
#endif
#ifdef TRU64_OS
	typedef int				socklen_t;
#endif
#ifdef UNIX_ENV
	#undef NULL
	#define NULL			0
	#define _stricmp		strcasecmp
	#define closesocket		close
	#define TRACE			printf
	#define INVALID_SOCKET	(SOCKET)(~0)
	#define SOCKET_ERROR	(-1)
	#define SOCKET			int
	typedef int				HANDLE;
	typedef int				HWND;
	#define SOCKADDR_IN		sockaddr_in
	typedef const char *	LPCTSTR;
	typedef const char *	LPCSTR;
	typedef char *			LPSTR;
	typedef void *			LPVOID;
	struct SYSTEMTIME 
	{
		WORD wYear;
		WORD wMonth;
		WORD wDayOfWeek;
		WORD wDay;
		WORD wHour;
		WORD wMinute;
		WORD wSecond;
		WORD wMilliseconds;
	};
	union semun {
		int val; // used for SETVAL only
		struct semid_ds * buf; // used for IPC_SET and IPC_STAT
		unsigned short * array; // used for GETALL and SETALL
	};
#endif
/*
#ifdef WINDOWS_ENV

typedef long					LONG;
typedef unsigned long			ULONG;
typedef unsigned long			DWORD;

#else

#ifndef ALPHA_64_PLATFORM
typedef long					LONG;
typedef unsigned long			ULONG;
typedef unsigned long			DWORD;
#else
typedef int						LONG;
typedef unsigned int			ULONG;
typedef unsigned int			DWORD;
#endif

#endif
/////////////////////////////////////////////////////////////////////////////
#ifndef MOTIF_MODE
typedef int						BOOL;
#endif

typedef char					CHAR;
typedef unsigned char			UCHAR;
typedef unsigned char			BYTE;

typedef float					FLOAT;
typedef double					DOUBLE;
*/

#define UINT32_INVALID			0xFFFFFFFF
/////////////////////////////////////////////////////////////////////////////
#ifdef WINDOWS_ENV
	// 线程函数返回值(在函数里必须返回0以适应跨平台需求)
	typedef DWORD			ThreadFuncRet;
	// 线程对象句柄
	typedef HANDLE			HTHREAD;
#endif
#ifdef UNIX_ENV
	#define WAIT_FAILED		-1
	#define WAIT_OBJECT_0	0
	#define INFINITE        0xFFFFFFFF	//无超时限制

	// 线程函数返回值(在函数里必须返回0以适应跨平台需求)
	typedef void *			ThreadFuncRet;
	// 线程对象句柄
	typedef pthread_t		HTHREAD;

#endif
	typedef ThreadFuncRet (* ThreadFuncType)(LPVOID pParam);
/////////////////////////////////////////////////////////////////////////////
// 提供一些重要的跨平台API
#ifdef WINDOWS_ENV
	extern __declspec( dllexport ) void GetLocalTime(SYSTEMTIME & SysTime);
	extern __declspec( dllexport ) void GetLocalTimeMs(SYSTEMTIME & SysTime);
	extern __declspec( dllexport ) int  GetTimeSpan(SYSTEMTIME & SysTime);
	extern __declspec( dllexport ) void ReadTime(LPSTR pszTime,SYSTEMTIME * pTime);
	extern __declspec( dllexport ) void ByteOrderChange(void * pData,int iDataSize);
	extern __declspec( dllexport ) void	ByteOrderChange_SYSTEMTIME(SYSTEMTIME * pSystemTime);
	extern __declspec( dllexport ) time_t GetSecond(const SYSTEMTIME & SysTime);
	extern __declspec( dllexport ) void KillProcess(int nProcessID);
//	extern __declspec( dllexport ) bool KillThreadEx(HTHREAD hThread);
//	extern __declspec( dllexport ) HTHREAD CreateThreadEx(ThreadFuncType pFunc, LPVOID pParam);
	extern __declspec( dllexport ) bool CreateThreadEx(ThreadFuncType pFunc, LPVOID pParam);
	extern __declspec( dllexport ) int  GetFileSize(const char * pFilePath);
	extern __declspec( dllexport ) void ChangeFileSize(const char * pFilePath,int iSize);
	extern __declspec( dllexport ) void ClearFile(const char * pFilePath);
	extern __declspec( dllexport ) void	_strtrim( char * pString );
	extern __declspec( dllexport ) bool	WritePrivateProfileString(char * lpAppName,char * lpKeyName,char * lpString,char * lpFileName);
	extern __declspec( dllexport ) int	GetPrivateProfileInt2(char * lpAppName,char * lpKeyName,int nDefault,char * lpFileName);
	extern __declspec( dllexport ) int	GetPrivateProfileString2(char * lpAppName,char * lpKeyName,char * lpDefault,char * lpReturnedString,int nSize,char * lpFileName);
	extern __declspec( dllexport ) void	CreateDirectory(char *szDir);
	extern __declspec( dllexport ) void	DeleteDirectory(char *szDir);
	extern __declspec( dllexport ) void GetLocalTimeStr( char *pOutTime ); 
	extern __declspec( dllexport ) unsigned long asc_long(unsigned char *pucSrc,unsigned int uiLen);
	extern __declspec( dllexport ) unsigned char *long_asc( unsigned char *pucDest,unsigned int uiLen,unsigned long ulSrc );
	//把字符串里边的日期占位符####@@$$替换为日期,SD_AVC_POINT####@@$$.RB,替换后SD_AVC_POINT20161221.RB
	extern __declspec( dllexport ) int replaceStringDate(std::string &str, time_t t);
	// add增加的天数，可以为负值 pcData当前天字符串,pcDateAdd返回的结果字符串格式为20150805
	extern __declspec( dllexport ) int dateAdd(char *pcDate,char *pcDateAdd,int add);
	//2016-11-19 tjs add
	extern __declspec( dllexport ) void GetSysConfigFile(char* szIniFile);
	//2016-11-19 tjs end
#endif
#ifdef UNIX_ENV
	extern "C" void GetLocalTime(SYSTEMTIME & SysTime);
	//** extern "C" void GetLocalTime(SYSTEMTIME * pSysTime);
	extern "C" void GetLocalTimeMs(SYSTEMTIME & SysTime);
	extern "C" int  GetTimeSpan(SYSTEMTIME & SysTime);
	extern "C" time_t GetSecond(const SYSTEMTIME & SysTime);
	extern "C" void ReadTime(LPSTR pszTime,SYSTEMTIME * pTime);
	extern "C" void ByteOrderChange(void * pData,int iDataSize);
	extern "C" void	ByteOrderChange_SYSTEMTIME(SYSTEMTIME * pSystemTime);
	extern "C" void KillProcess(int nProcessID);
//	extern "C" bool KillThreadEx(HTHREAD hThread);
//	extern "C" HTHREAD CreateThreadEx(ThreadFuncType pFunc, LPVOID pParam);
	extern "C" bool CreateThreadEx(ThreadFuncType pFunc, LPVOID pParam);
	extern "C" int  GetFileSize(const char * pFilePath);
	extern "C" void ChangeFileSize(const char * pFilePath,int iSize);
	extern "C" void ClearFile(const char * pFilePath);
	extern "C" void	MilliSleep(UINT uMilliSeconds);
	extern "C" void GetLocalTimeStr( char *pOutTime ); 
	extern "C" unsigned long asc_long(unsigned char *pucSrc,unsigned int uiLen);
	extern "C" unsigned  char *long_asc( unsigned char *pucDest,unsigned int uiLen,unsigned long ulSrc );
	extern "C" int dateAdd(char *pcDate,char *pcDateAdd,int add);
	//2016-11-19 tjs add
	extern "C" void GetSysConfigFile(char* szIniFile);
	extern "C"  int replaceStringDate(std::string &str, time_t t);
	//2016-11-19 tjs end

#endif
/////////////////////////////////////////////////////////////////////////////
// 按照windows系统函数的格式提供所需的下面这几个函数
#ifdef UNIX_ENV
	extern "C" bool	WritePrivateProfileString(char * lpAppName,char * lpKeyName,char * lpString,char * lpFileName);
	extern "C" int	GetPrivateProfileInt2(char * lpAppName,char * lpKeyName,int nDefault,char * lpFileName);
	extern "C" int	GetPrivateProfileString2(char * lpAppName,char * lpKeyName,char * lpDefault,char * lpReturnedString,int nSize,char * lpFileName);
	extern "C" BOOL CopyFile(const char * pExistFile,const char * pNewFile,BOOL bFailIfExists);
	extern "C" BOOL DeleteFile(const char * pExistFile);
	extern "C" BOOL MoveFile(const char * pExistFile, const char * pNewFile);
	extern "C" void	_strupr( char * pString );
	extern "C" void	_strtrim( char * pString );
	extern "C" int	GetCurrentProcessId();
	extern "C" bool SetCurrentDirectory(char *szDir);
	extern "C" void CreateDirectory(char *szDir);
	extern "C" void DeleteDirectory(char *szDir);
	extern "C" int lockfile(int fd);
	extern "C" int already_running(const char *filename);
#endif

#ifdef UNIX_ENV
#define	sprintf_s snprintf



#endif

#endif // __AFX_SYSTEM_DEFINE_OF_RTDBDLL_H_WW_2003_11_14__
