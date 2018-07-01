#ifndef __CRUN_MANGER_H_
#define  __CRUN_MANGER_H_
#include "gSystemDef.h"
#include "Q_frameInclude.h"
#include "GlobalDefine.h"
#include "Q_GSetting.h"
#include "database.h"
#include "CLibLoader.h"
#include "linearhash.h"
#include "Log.h"
#include <vector>
using namespace std;

class CRunManager
{
public:
	CRunManager(void);
	virtual ~CRunManager(void);
	int init();//初始化
	bool startComm(void);
	bool stopComm(void);
	char *getRunPath();
	char *getConfigPath();
	//检查消息 //true成功false不成功
	bool chkMsg();
	//true运行
	Q_ThreadStatusType getRunStatus();
	//打印规约更新日志
	int printProtocolChangeLog(char *libName);
private:
	int initBase();
	int initRtdb(Q_GSetting *pGSetting);
	bool initComm(void);
	CLibLoader* refProtocolLib(string& libname);
	void unrefProtocolLib(string& libname);
	//初使化采集规约
	Q_chDownReturn* createScadaProtocol(ST_GeCtrlChl *cfg);
	void freeScadaProtocol(Q_chDownReturn* pProtocol);
	//添加主站命令
	bool addMasterCmd(TaskCfgList &task,LPQ_RECORDDATATAG rec);
	//添加扩展消息命令
	bool addMasterCmdExpandMsg(TaskCfgList &task,LPQ_RECORDDATATAG rec,ExpandMsg* MsgExpand);
	Q_chDownReturn* createZtProtocol(ST_GeCtrlChl *cfg);
	static ThreadFuncRet UpdataProcessPara(LPVOID lpParameter);
	CMultiDataBase *getMaindb();
	int getTaskMainChannelID(int chid);
	int handleSpecialMsg();
private:
	char szIniFile[MAX_PATH];
	Q_GSetting m_GSettting;//初始化基本表
	database m_db;//实时数据库初使化使用
	//DataIndex m_dbIndex;//数据索引区
	Q_dataManager *m_pMgr;
	int m_RunMethod;
	Q_taskManager		m_taskManager;//任务管理
	vector<TaskCfgList> m_taskList;

	CLinearHash<string, CLibLoader*> m_protocol_lib;
	CLinearHash<string, CLibLoader*> m_mod_lib;
	char m_chInfo[Q_FRAME_CONST256];//信息内容
	Log *m_log;//日志打印使用
	Q_ThreadStatusType m_eCalcThreadStatus;//运行状态
};
#endif