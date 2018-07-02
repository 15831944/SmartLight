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
	int init();//��ʼ��
	bool startComm(void);
	bool stopComm(void);
	char *getRunPath();
	char *getConfigPath();
	//�����Ϣ //true�ɹ�false���ɹ�
	bool chkMsg();
	//true����
	Q_ThreadStatusType getRunStatus();
	//��ӡ��Լ������־
	int printProtocolChangeLog(char *libName);
private:
	int initBase();
	int initRtdb(Q_GSetting *pGSetting);
	bool initComm(void);
	CLibLoader* refProtocolLib(string& libname);
	void unrefProtocolLib(string& libname);
	//��ʹ���ɼ���Լ
	Q_chDownReturn* createScadaProtocol(ST_GeCtrlChl *cfg);
	void freeScadaProtocol(Q_chDownReturn* pProtocol);
	//�����վ����
	bool addMasterCmd(TaskCfgList &task,LPQ_RECORDDATATAG rec);
	//�����չ��Ϣ����
	bool addMasterCmdExpandMsg(TaskCfgList &task,LPQ_RECORDDATATAG rec,ExpandMsg* MsgExpand);
	Q_chDownReturn* createZtProtocol(ST_GeCtrlChl *cfg);
	static ThreadFuncRet UpdataProcessPara(LPVOID lpParameter);
	CMultiDataBase *getMaindb();
	int getTaskMainChannelID(int chid);
	int handleSpecialMsg();
private:
	char szIniFile[MAX_PATH];
	Q_GSetting m_GSettting;//��ʼ��������
	database m_db;//ʵʱ���ݿ��ʹ��ʹ��
	//DataIndex m_dbIndex;//����������
	Q_dataManager *m_pMgr;
	int m_RunMethod;
	Q_taskManager		m_taskManager;//�������
	vector<TaskCfgList> m_taskList;

	CLinearHash<string, CLibLoader*> m_protocol_lib;
	CLinearHash<string, CLibLoader*> m_mod_lib;
	char m_chInfo[Q_FRAME_CONST256];//��Ϣ����
	Log *m_log;//��־��ӡʹ��
	Q_ThreadStatusType m_eCalcThreadStatus;//����״̬
};
#endif