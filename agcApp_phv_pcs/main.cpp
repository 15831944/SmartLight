#include "stdafx.h"
#include "Q_frameInclude.h"
#include "signal.h"
#include "string.h"
#include "agcCalc.h"

agcCalc *g_m_agcCalc = NULL;
static void breakBySignal(int);
void breakBySignal(int)
{
	g_m_agcCalc->Stop();
	TRACE("breakBySignal\n");
	//system("banner bye-bye"); //因为发现退出会卡住且会启动多个进程
	printf("banner bye-bye");
	exit(0);
}
//详细的更新记录
void showChangeLog();


const UINT g_agc_major_version = 3;
const UINT g_agc_minor_version = 0;
UINT g_bStopFlag=0;
int g_debug_level=0;
const char* g_usage = 
	"-v, -version    check the version of agcApp_phv\n" 
	"-q, -quit       stop agcApp_phv under backgroud\n" 
	"-l[n], -L[n]    control output debug messages\n" 
	"-h, -? -help    print this screen\n";

int main(int argc, char* argv[])
{
#ifdef UNIX_ENV
	signal(SIGALRM, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, breakBySignal);
	signal(SIGINT, breakBySignal);
	signal(SIGQUIT, breakBySignal);
	signal(SIGTERM, breakBySignal);
	signal(SIGSEGV, breakBySignal);
#else
	signal(SIGTERM, breakBySignal);
	signal(SIGINT, breakBySignal);
#endif

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
				if(g_debug_level > 0)
				{
					printf("agcApp_phv start with debug level %d ...!!!\n", g_debug_level );
				}
			}
			else if (strcmp(szRet, "v") == 0 || strcmp(szRet, "version") == 0)
			{
				printf("agcApp_phv Version %d.%d\n", 
					g_agc_major_version,
					g_agc_minor_version);

				showChangeLog();

				ret = 0;
				break;
			}
			else if (strcmp(szRet, "h") == 0 || 
				strcmp(szRet, "?") == 0 ||
				strcmp(szRet, "help"))
			{
				printf("agcApp_phv Version %d.%d\n", 
					g_agc_major_version,
					g_agc_minor_version);

				printf("%s", g_usage);
				ret = -1;
				break;
			}
			else
			{
				printf("agcApp_phv Version %d.%d\n", 
					g_agc_major_version,
					g_agc_minor_version);

				printf("%s", g_usage);
				//bad argument, print usage
				ret = -1;
				break;
			}
		}
	}


	if(ret <=0)
	{
		return 0;
	}

	//初使化实时数据库
	//初使化scada库
	//初使化AVC库
	g_m_agcCalc=new agcCalc();	
	g_m_agcCalc->Init();	
	g_m_agcCalc->Start();
    while(1)
	{
		//agcRtdb->freshPhvUnit();
		MilliSleep(500);
	}

	return 0;
}

void showChangeLog()
{
	printf("=== 2016-07-21====\n");
	printf("1、增加打印输入版本功能\n");
	printf("=== 2016-09-01====\n");
	printf("1、更改Ubuntu下打印时，文件保存到文件夹名称为'bin\\data\\message\\agc'的情况，更改为根据不同的编译选项决定选择反斜杠或者是正斜杠\n");
	printf("=== 2016-09-12====\n");
	printf("1、更改清除日志的算法功能\n");
	printf("=== 2016-09-20====\n");
	printf("1、修正清除日志功能，原功能只计算了第一条记录\n");
	printf("=== 2016-10-12====\n");
	printf("1、CAgcDataBase库初使化少增加了CDataBase()\n");
	printf("=== 2016-11-23====\n");
	printf("1、更新支持更新遥测、遥信时全网同步功能\n");
	printf("2、更改备机算法不运行，只是刷新数据\n");
	printf("=== 2016-11-26====\n");
	printf("1、为了全网同步增加m_dbmg_mdb->SocketInit()\n");
	printf("=== 2016-11-28====\n");
	printf("1、更新数据时不用m_dbmg_sdb->SetElementByID,因此方法会带来实库混乱\n");
	printf("2、增加一分钟变化率算法\n");
	printf("=== 2016-12-12====\n");
	printf("1、修正AGC本地模式取错了来源的缺陷,原来取的是远方的了\n");
	printf("=== 2017-01-06====\n");
	printf("1、调度AGC指令相同不再触发算法\n");
	printf("2、1分钟变化率检查失败，安全约束检测不通过\n");
	printf("=== 2017-1-16====\n");
	printf("1、AGC的机组可以配置为样板逆变器，不参与调节\n");
}