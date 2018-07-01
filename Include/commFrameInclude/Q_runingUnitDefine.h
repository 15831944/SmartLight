#ifndef __Q_RUNNING_UNIT_DEFINE_H_
#define __Q_RUNNING_UNIT_DEFINE_H_
#include "gSystemDef.h"
#include "Q_frameDefine.h"
#include "Q_runningUnit.h"
#include "Q_dataManager.h"

//通道表参数
typedef struct ST_GeCtrlChl
{
	int id;//通道号
	char name[100]; //值
	bool enabled;
	int type;
	char cfgfile[128];
	int YCbegin;//遥测索引开始
	int YCend;//遥测索引结束
	int YXbegin;//遥信索引开始
	int YXend;//遥信索引结束
	int YKbegin;
	int YKend;
	int YTbegin;
	int YTend;
	char protocol[50];
	char classname[50];//樊硕修改20150723，类名称，用于IEC104子站
	int  nchanneltype; //樊硕修改20160229，通道类型，用于IEC104规约判断主站子站
	char link[50];
	//int grpid;
	bool iscmd;//是否是命令通道
	int chStatusid; //jwk add to change channel status in db
	int protocolSubType;//规约子类型
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
		memset(classname,0,sizeof(classname));//樊硕修改20150723
		nchanneltype = 0;
		//grpid=0;
		iscmd=false;
		protocolSubType=0;
	}
}ST_GeCtrlChl;//通道

//规约采集参数
typedef struct _chDownCommPara
{
	char strFilePath[MAX_PATH];//规约文件路径
	ST_GeCtrlChl *cfg;//通道参数
	Q_dataManager    *m_pMgr;//实时库接口

}Q_chDownCommPara;
//采集规约返回参数
typedef struct _chDownReturn 
{
	Q_taskRunningUnit* unit;
	Q_tcpServerUnitInfo* taskServer;//tcp server 20161215
	char protocol[50];
	Q_taskBase* task;
	UINT channelID;
	bool enabled;
	bool bTcpServer;//是否是tcp server
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
#define HR_PROTOCOL_ADD_EXPAND    "add_command_expand"//扩展命令

#define HR_MOD_CREATE "create_mod_object"
#define HR_MOD_FREE   "free_mod_object"
#define HR_PRO_CHANGELOG "print_change_log"


typedef int (OUTPUTAPI *MOD_INIT)(int dbg_level);
typedef Q_chDownReturn*(OUTPUTAPI *CREATE_PROTOCOL_OBJECT)(Q_chDownCommPara*);
typedef int (OUTPUTAPI *ADD_ROTOCOL_COMMAND)(Q_taskBase *task,Q_RECORDDATATAG*);//添加命令-遥控遥调
typedef int (OUTPUTAPI *ADD_ROTOCOL_COMMAND_EXPAND)(Q_taskBase *task,Q_RECORDDATATAG*,ExpandMsg*);//添加命令-扩展消息

typedef void (OUTPUTAPI *FREE_PROTOCOL_OBJECT)(Q_chDownReturn*);
typedef void (OUTPUTAPI *MOD_EXIT)();
//打印更新记录
typedef int (OUTPUTAPI *PRINT_CHANGE_LOG)();





#endif