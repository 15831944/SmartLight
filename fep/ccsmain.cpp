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
//��ϸ�ĸ��¼�¼
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
		printf("\n?��Э������װ��δ��Ȩʹ��(This ccsosd is not been authorized!)\n");
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
printf("1�����Ӵ�ӡ����汾����\n");
printf("=== 2016-07-28====\n");
printf("1��������ӡ�汾��Ϣʱ��fep coredump����\n");

printf("=== 2016-09-01====\n");
printf("1������Ubuntu�´�ӡʱ���ļ����浽�ļ�������Ϊ'bin\\data\\message\\'�����������Ϊ���ݲ�ͬ�ı���ѡ�����ѡ��б�ܻ�������б��\n");

printf("=== 2016-09-10====\n");
printf("1����Ϊ��������վ�ϴ�������ʱ��������IEC104��վ�ϴ��������ͨ����û�г�ʹ����\n");
printf("=== 2016-09-11====\n");
printf("1��libCommFrame��serialLinkȥ����ӡ�յ������ݣ���Ϊ���ֱ�����\n");

printf("=== 2016-10-28====\n");
printf("1������ͨ��״̬��ʹ����ͨ��״̬λ�������¡�\n");
printf("=== 2016-11-18====\n");
printf("1������ͨ�����,֧��IEC104��վ��Լ��ͨ������ʱ�����ͨ���е����ݿ��Բ�ͬ\n");
printf("=== 2016-11-19====\n");
printf("1��֧�ֵ�ǰ����·��������\n");
printf("=== 2016-11-19====\n");
printf("1������dataManager�и���ͨ��״̬�ķ������ж�ͨ�����ڵ���1ʱʱ�꣬�ͻ���״̬Ϊ1\n");
printf("2������tcpServerUnitInfoͨ��״̬����Ϊ���ֶ����վʱͨ��״̬����ȷ\n");
printf("=== 2016-12-28====\n");
printf("1���޸�modbusTcp��վ�������ݲ�����ϵ����ȱ�ݣ�\n");
printf("1��modbusRtu��վ����30S�ղ������ݣ���ͨ������״̬����Ϊ0��\n");
printf("1��modbusRtu��վ�������10֡���ĵ�CRCУ����󣬰�ͨ������״̬����Ϊ0����Ҫ�޸������ļ������øù��ܣ�\n");
printf("=== 2017-1-9====\n");
printf("1������һ��Tcp ServerĬ�������̼߳����������Ϊ128,ԭ��32������Ҫ��Ϊ��֧������103��Լ��\n");
printf("2��ǰ�ó������֧��TCP server��Ϊ��վ,ģ�ⷢ������ķ��������߼�\n");
printf("3��ǰ�ó�������˳��㷨����\n");
printf("4���޸�����������Ϣ�������֧��IEC103TcpServer����վ����\n");
printf("=== 2017-1-9====\n");
printf("1��ScadaAPI��GetAnalogIndexByKey�������ģ��ĳ�����ΪID-1,��Ҫ����Ϊ���ֳ�վ�����һ��ʱ���Ǵ�1��ʼ�ģ���ʵӦ���Ǵ�0��ʼ����\n");
printf("=== 2017-1-13====\n");
printf("1������Q_dataManager::evalDataValue�������Ա㵱Ϊת����ʱ��������Ŀ��ֵ\n");
printf("=== 2017-2-6====\n");
printf("�ӳ�tcp�ͻ���select��ʱʱ�䣬ԭ����300���ף����ڸ�Ϊ1300����\n");
printf("=== 2017-2-20====\n");
printf("�޸�Q_simpleThread::stopWork()�������ȴ��߳��˳���÷������˳�������̳߳���5�뻹û���˳����÷����˳�\n");
printf("=== 2017-3-3====\n");
printf("����ʵʱ�������л���ͨ��״̬����ȷ��ȱ�ݣ��޸�Q_taskBase::CheckMasterRun(int type),�ر�ͨ��ʱ����ͨ�������Ӧͨ����״̬\n");
printf("=== 2017-3-10====\n");
printf("1��Commframe�޸ģ�����Է���IP���ڹ��˵�IP��Χ�ڵĻ���ԭ���ǲ���������ɴ�����CLOSE_WAIT������Tcp���ʵ�����\n");
printf("2�����������رչ��ܣ�������ɳ��ִ�����TIME_WAIT״̬\n");
printf("=== 2017-3-14====\n");
printf("1��Commframe�޸������ر�ʱ�����\n");
printf("=== 2017-3-23====\n");
printf("1������Commframe��Q_tcpServerLink��linux�´�ӡ����־·��������ȷ��\n");
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
	// ���ﵯ��һ������Ի����˳�����  
	//  
	CrashHandler(pException);

	//	FatalAppExit(-1,  _T("*** Unhandled Exception! ***"));  
	exit(-1);

	return EXCEPTION_EXECUTE_HANDLER;  
}
// ����Dump�ļ�
// 
void CreateDumpFile(LPCSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
	// ����Dump�ļ�
	//
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Dump��Ϣ
	//
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// д��Dump�ļ�����
	//
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

	CloseHandle(hDumpFile);
}

// ��Ϊexcept���б��ʽ�ĺ���
//
LONG CrashHandler(EXCEPTION_POINTERS *pException)
{	
	// ��������Ӵ�������������Ĵ���
	//

	// �����Ե���һ���Ի���Ϊ����
	//
	//MessageBox(NULL, _T("Message from Catch handler"), _T("Test"), MB_OK);

	// ����Dump�ļ�
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