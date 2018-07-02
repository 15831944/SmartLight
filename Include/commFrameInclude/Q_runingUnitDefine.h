#ifndef __Q_RUNNING_UNIT_DEFINE_H_
#define __Q_RUNNING_UNIT_DEFINE_H_
#include "gSystemDef.h"
#include "Q_frameDefine.h"
#include "Q_runningUnit.h"
#include "Q_dataManager.h"

//ͨ�������
typedef struct ST_GeCtrlChl
{
	int id;//ͨ����
	char name[100]; //ֵ
	bool enabled;
	int type;
	char cfgfile[128];
	int YCbegin;//ң��������ʼ
	int YCend;//ң����������
	int YXbegin;//ң��������ʼ
	int YXend;//ң����������
	int YKbegin;
	int YKend;
	int YTbegin;
	int YTend;
	char protocol[50];
	char classname[50];//��˶�޸�20150723�������ƣ�����IEC104��վ
	int  nchanneltype; //��˶�޸�20160229��ͨ�����ͣ�����IEC104��Լ�ж���վ��վ
	char link[50];
	//int grpid;
	bool iscmd;//�Ƿ�������ͨ��
	int chStatusid; //jwk add to change channel status in db
	int protocolSubType;//��Լ������
	ST_GeCtrlChl(void)
	{
		id=0;
		memset(name,0,sizeof(name));
		enabled=false;
		type=0;
		memset(cfgfile,0,sizeof(cfgfile));
		//dataindex=0;
		YCbegin = 0;
		YXbegin = 0;
		YKbegin = 0;
		YTbegin = 0;
		YCend = 0;
		YXend = 0;
		YKend = 0;
		YTend = 0;
		memset(protocol,0,sizeof(protocol));
		memset(link,0,sizeof(link));
		memset(classname,0,sizeof(classname));//��˶�޸�20150723
		nchanneltype = 0;
		//grpid=0;
		iscmd=false;
		protocolSubType=0;
	}
}ST_GeCtrlChl;//ͨ��

//��Լ�ɼ�����
typedef struct _chDownCommPara
{
	char strFilePath[MAX_PATH];//��Լ�ļ�·��
	ST_GeCtrlChl *cfg;//ͨ������
	Q_dataManager    *m_pMgr;//ʵʱ��ӿ�

}Q_chDownCommPara;
//�ɼ���Լ���ز���
typedef struct _chDownReturn 
{
	Q_taskRunningUnit* unit;
	Q_tcpServerUnitInfo* taskServer;//tcp server 20161215
	char protocol[50];
	Q_taskBase* task;
	UINT channelID;
	bool enabled;
	bool bTcpServer;//�Ƿ���tcp server
	_chDownReturn()
	{
		taskServer=NULL;
		unit=NULL;
		task=NULL;
		channelID=0;
		enabled=true;
		bTcpServer=false;
	}
}Q_chDownReturn;

//global instance

#ifdef WINDOWS_ENV
#	define OUTPUTAPI __stdcall
#endif // WINDOWS_ENV
#ifdef UNIX_ENV
#	define OUTPUTAPI
#endif // UNIX_ENV

#ifdef WINDOWS_ENV
#define HR_LIB_EXT ".dll"
#endif

#ifdef UNIX_ENV
#define HR_LIB_EXT ".so"
#endif

#define HR_MOD_INIT "mod_init"
#define HR_MOD_EXIT "mod_exit"
#define HR_PROTOCOL_CREATE "create_protocol_object"
#define HR_PROTOCOL_FREE   "free_protocol_object"
#define HR_PROTOCOL_ADD    "add_command"
#define HR_PROTOCOL_ADD_EXPAND    "add_command_expand"//��չ����

#define HR_MOD_CREATE "create_mod_object"
#define HR_MOD_FREE   "free_mod_object"
#define HR_PRO_CHANGELOG "print_change_log"


typedef int (OUTPUTAPI *MOD_INIT)(int dbg_level);
typedef Q_chDownReturn*(OUTPUTAPI *CREATE_PROTOCOL_OBJECT)(Q_chDownCommPara*);
typedef int (OUTPUTAPI *ADD_ROTOCOL_COMMAND)(Q_taskBase *task,Q_RECORDDATATAG*);//�������-ң��ң��
typedef int (OUTPUTAPI *ADD_ROTOCOL_COMMAND_EXPAND)(Q_taskBase *task,Q_RECORDDATATAG*,ExpandMsg*);//�������-��չ��Ϣ

typedef void (OUTPUTAPI *FREE_PROTOCOL_OBJECT)(Q_chDownReturn*);
typedef void (OUTPUTAPI *MOD_EXIT)();
//��ӡ���¼�¼
typedef int (OUTPUTAPI *PRINT_CHANGE_LOG)();





#endif