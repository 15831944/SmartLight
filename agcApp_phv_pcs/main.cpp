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
	//system("banner bye-bye"); //��Ϊ�����˳��Ῠס�һ������������
	printf("banner bye-bye");
	exit(0);
}
//��ϸ�ĸ��¼�¼
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

	//��ʹ��ʵʱ���ݿ�
	//��ʹ��scada��
	//��ʹ��AVC��
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
	printf("1�����Ӵ�ӡ����汾����\n");
	printf("=== 2016-09-01====\n");
	printf("1������Ubuntu�´�ӡʱ���ļ����浽�ļ�������Ϊ'bin\\data\\message\\agc'�����������Ϊ���ݲ�ͬ�ı���ѡ�����ѡ��б�ܻ�������б��\n");
	printf("=== 2016-09-12====\n");
	printf("1�����������־���㷨����\n");
	printf("=== 2016-09-20====\n");
	printf("1�����������־���ܣ�ԭ����ֻ�����˵�һ����¼\n");
	printf("=== 2016-10-12====\n");
	printf("1��CAgcDataBase���ʹ����������CDataBase()\n");
	printf("=== 2016-11-23====\n");
	printf("1������֧�ָ���ң�⡢ң��ʱȫ��ͬ������\n");
	printf("2�����ı����㷨�����У�ֻ��ˢ������\n");
	printf("=== 2016-11-26====\n");
	printf("1��Ϊ��ȫ��ͬ������m_dbmg_mdb->SocketInit()\n");
	printf("=== 2016-11-28====\n");
	printf("1����������ʱ����m_dbmg_sdb->SetElementByID,��˷��������ʵ�����\n");
	printf("2������һ���ӱ仯���㷨\n");
	printf("=== 2016-12-12====\n");
	printf("1������AGC����ģʽȡ������Դ��ȱ��,ԭ��ȡ����Զ������\n");
	printf("=== 2017-01-06====\n");
	printf("1������AGCָ����ͬ���ٴ����㷨\n");
	printf("2��1���ӱ仯�ʼ��ʧ�ܣ���ȫԼ����ⲻͨ��\n");
	printf("=== 2017-1-16====\n");
	printf("1��AGC�Ļ����������Ϊ��������������������\n");
}