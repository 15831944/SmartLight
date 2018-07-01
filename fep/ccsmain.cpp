#include "stdafx.h"
#include "Q_frameInclude.h"
#include "debugdef.h"
#include "signal.h"
#include "GlobalDefine.h"
#include "string.h"

#undef  CREATE_DUMP_FLAG

#ifdef WINDOWS_ENV
#include <DbgHelp.h> 
#endif
#define	AUTHORIZING_ONLYONE	
#define	AUTHORIZING_MAC_ADDR	"00:90:e8:14:2e:e9"
#include "CRunManager.h"
static void breakBySignal(int);
/*
 * process signal
 */

CRunManager *gRunner=NULL;
bool m_bRuningFlg=true;
void breakBySignal(int)
{
    printf("banner bye-bye");
	m_bRuningFlg=false;
	if (gRunner != NULL)
	{
		gRunner->stopComm();
	}
    exit(0);
}
//详细的更新记录
void showChangeLog();
void showDynamicLibChangeLog(char *libName);

const UINT g_fep_major_version = 3;
const UINT g_fep_minor_version = 0;
UINT g_bStopFlag=0;
int g_debug_level=0;
const char* g_usage = 
"-v, -version    check the version of fep\n" 
"-q, -quit       stop fep under backgroud\n" 
"-l[n], -L[n]    control output debug messages.eg:-l2\n"
"-p[n],-P[n]     print protocol library change log.eg:-pCommIEC104\n"
"-h, -? -help    print this screen\n";
#ifdef CREATE_DUMP_FLAG
#ifdef WINDOWS_ENV 
void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException);
LONG CrashHandler(EXCEPTION_POINTERS *pException);
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException);
#endif
#endif
int main(int argc, char* argv[])
{
	printf("fep starting......main(*,*)\n");//append.


	int ret = 1;
	int n;
	for(n = 1; n < argc; n ++)
	{
		char* szRet = strtok(argv[n], "-");
		if(szRet)
		{
			if(strcmp(szRet, "q") == 0 || strcmp(szRet, "quit") == 0)
			{
				g_bStopFlag = 1;
			}
			else if((szRet[0] == 'l' || szRet[0] == 'L') && 
				strlen(szRet) > 1 &&
				isdigit(szRet[1]))
			{
				g_debug_level = atoi(szRet+1);
				if(g_debug_level > dfl_sensive)
				{
					printf("fep start with debug level %d ...!!!\n", g_debug_level );
				}
			}
			else if (strcmp(szRet, "v") == 0 || strcmp(szRet, "version") == 0)
			{
				printf("fep Version %d.%d\n", 
					g_fep_major_version,
					g_fep_minor_version);

				showChangeLog();

				ret = 0;
				break;
			}
			else if((szRet[0] == 'p' || szRet[0] == 'P') && 
				strlen(szRet) > 1 )
			{
				char tmp[50];
				memset(tmp,0,sizeof(tmp));
				strcpy(tmp,szRet+1);
#ifdef UNIX_ENV
				printf("print protocol dynamic library change log for %s.so !!!\n", tmp);
#else
				printf("print protocol dynamic library change log for %s.dll !!!\n", tmp);
#endif
				showDynamicLibChangeLog(tmp);
				ret=-1;
			}
			else if (strcmp(szRet, "h") == 0 || 
				strcmp(szRet, "?") == 0 ||
				strcmp(szRet, "help"))
			{
				printf("fep Version %d.%d\n", 
					g_fep_major_version,
					g_fep_minor_version);

				printf("%s", g_usage);
				ret = -1;
				break;
			}
			else
			{
				printf("fep Version %d.%d\n", 
					g_fep_major_version,
					g_fep_minor_version);

				printf("%s", g_usage);
				//bad argument, print usage
				ret = -1;
				break;
			}
		}
	}

#ifdef AUTHORIZING_ONLYONE
#ifdef UNIX_ENV
	/*printf("This is OS_UNIX environment!!!\n");//append.
	char str[20];
	int fd;
	struct ifreq ifr;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
	ioctl(fd, SIOCGIFHWADDR, &ifr);
	close(fd);
	sprintf(str,"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
		(unsigned char)ifr.ifr_hwaddr.sa_data[0],
		(unsigned char)ifr.ifr_hwaddr.sa_data[1],
		(unsigned char)ifr.ifr_hwaddr.sa_data[2],
		(unsigned char)ifr.ifr_hwaddr.sa_data[3],
		(unsigned char)ifr.ifr_hwaddr.sa_data[4],
		(unsigned char)ifr.ifr_hwaddr.sa_data[5]);
	if(strcmp(str,AUTHORIZING_MAC_ADDR)!=0)
	{
		printf("\n?该协调控制装置未授权使用(This ccsosd is not been authorized!)\n");
		return 0;
	}
	*/
#endif
#endif


		if(ret <=0)
		{
			return 0;
		}
//	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler); 

	gRunner =new CRunManager();
	//char *tmp=NULL;
	//strcpy(tmp,"123");
	if(0==gRunner->init())
	{
		printf("Init Success\n");
		gRunner->startComm();
	}
	else
	{
        printf("Init Failed\n");
		breakBySignal(1);
	}

#ifdef UNIX_ENV
	signal(SIGALRM, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, breakBySignal);
	signal(SIGINT, breakBySignal);
	signal(SIGQUIT, breakBySignal);
	signal(SIGTERM,breakBySignal);
	signal(SIGSEGV, breakBySignal);
#else
	signal(SIGTERM, breakBySignal);
	signal(SIGINT, breakBySignal);
	signal(SIGBREAK, breakBySignal);
#endif

	if(g_bStopFlag)
	{

	}

   while(m_bRuningFlg)
   {
	   gRunner->chkMsg();
	   MilliSleep(500);
   }

   gRunner->stopComm();
   printf("quiting fep...\n");

   return ret;
}
void showChangeLog()
{
printf("=== 2016-07-21====\n");
printf("1、增加打印输入版本功能\n");
printf("=== 2016-07-28====\n");
printf("1、修正打印版本信息时，fep coredump错误\n");

printf("=== 2016-09-01====\n");
printf("1、更改Ubuntu下打印时，文件保存到文件夹名称为'bin\\data\\message\\'的情况，更改为根据不同的编译选项决定选择反斜杠或者是正斜杠\n");

printf("=== 2016-09-10====\n");
printf("1、因为增加了子站上传后配置时，配置了IEC104子站上传后下面的通道就没有初使化了\n");
printf("=== 2016-09-11====\n");
printf("1、libCommFrame中serialLink去掉打印收到的内容，因为发现崩溃。\n");

printf("=== 2016-10-28====\n");
printf("1、更改通道状态，使主备通道状态位独立更新。\n");
printf("=== 2016-11-18====\n");
printf("1、更改通道检查,支持IEC104子站规约多通道上送时，多个通道中的数据可以不同\n");
printf("=== 2016-11-19====\n");
printf("1、支持当前工程路径可配置\n");
printf("=== 2016-11-19====\n");
printf("1、更改dataManager中更新通道状态的方法，判断通道大于等于1时时标，客户端状态为1\n");
printf("2、更改tcpServerUnitInfo通道状态，因为发现多个子站时通道状态不正确\n");
printf("=== 2016-12-28====\n");
printf("1、修改modbusTcp子站上送数据不乘以系数的缺陷；\n");
printf("1、modbusRtu主站超过30S收不到数据，把通道运行状态设置为0；\n");
printf("1、modbusRtu主站如果连续10帧报文的CRC校验错误，把通道运行状态设置为0，需要修改配置文件来启用该功能；\n");
printf("=== 2017-1-9====\n");
printf("1、更改一个Tcp Server默认最大的线程及任务数最大为128,原来32个，主要是为了支持南自103规约用\n");
printf("2、前置程序更改支持TCP server做为主站,模拟发送命令的方法处理逻辑\n");
printf("3、前置程序更改退出算法功能\n");
printf("4、修改增加特殊消息处理机制支持IEC103TcpServer做主站命令\n");
printf("=== 2017-1-9====\n");
printf("1、ScadaAPI中GetAnalogIndexByKey函数更改，改成索引为ID-1,主要是因为发现厂站内序号一段时间是从1开始的，其实应该是从0开始配置\n");
printf("=== 2017-1-13====\n");
printf("1、更改Q_dataManager::evalDataValue方法，以便当为转发点时正常更新目标值\n");
printf("=== 2017-2-6====\n");
printf("延长tcp客户端select超时时间，原来是300毫米，现在改为1300毫秒\n");
printf("=== 2017-2-20====\n");
printf("修改Q_simpleThread::stopWork()方法，等待线程退出后该方法才退出，如果线程超过5秒还没有退出，该方法退出\n");
printf("=== 2017-3-3====\n");
printf("消除实时库主备切换后通道状态不正确的缺陷，修改Q_taskBase::CheckMasterRun(int type),关闭通道时更新通道表里对应通道的状态\n");
printf("=== 2017-3-10====\n");
printf("1在Commframe修改，如果对方的IP不在过滤的IP范围内的话，原来是不处理，会造成大量的CLOSE_WAIT阻塞了Tcp访问的问题\n");
printf("2增加了主动关闭功能，但会造成出现大量的TIME_WAIT状态\n");
printf("=== 2017-3-14====\n");
printf("1、Commframe修改主动关闭时如果用\n");
printf("=== 2017-3-23====\n");
printf("1、修正Commframe中Q_tcpServerLink中linux下打印的日志路径名不正确。\n");
}
#ifdef WINDOWS_ENV
void showDynamicLibChangeLog(char *libName)
{
	gRunner =new CRunManager();
	gRunner->printProtocolChangeLog(libName);
	
}
#ifdef CREATE_DUMP_FLAG
#ifdef WINDOWS_ENV 
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)  
{     
	// 这里弹出一个错误对话框并退出程序  
	//  
	CrashHandler(pException);

	//	FatalAppExit(-1,  _T("*** Unhandled Exception! ***"));  
	exit(-1);

	return EXCEPTION_EXECUTE_HANDLER;  
}
// 创建Dump文件
// 
void CreateDumpFile(LPCSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
	// 创建Dump文件
	//
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Dump信息
	//
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// 写入Dump文件内容
	//
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

	CloseHandle(hDumpFile);
}

// 作为except块中表达式的函数
//
LONG CrashHandler(EXCEPTION_POINTERS *pException)
{	
	// 在这里添加处理程序崩溃情况的代码
	//

	// 这里以弹出一个对话框为例子
	//
	//MessageBox(NULL, _T("Message from Catch handler"), _T("Test"), MB_OK);

	// 创建Dump文件
	//
	SYSTEMTIME stime;
	GetLocalTime(&stime);

	char tt[30];
	char path[MAX_PATH];
	sprintf_s(tt, sizeof(tt),"%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d_%3.3d", stime.wYear,stime.wMonth,stime.wDay,
		stime.wHour, stime.wMinute,
		stime.wSecond, stime.wMilliseconds);
	memset(path,NULL,sizeof(path));

	sprintf_s(path,sizeof(path),"Commapp(%s).dmp",tt);


	CreateDumpFile(_T(path), pException);

	return EXCEPTION_EXECUTE_HANDLER;
}
#endif
#endif
#endif