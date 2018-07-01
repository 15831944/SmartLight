#ifndef _DATA_BASE_H_
#define _DATA_BASE_H_
#include "Q_GSetting.h"
#include "Q_frameInclude.h"
#include "Log.h"
class database
{
public:
	database();
	virtual ~database();
	void setDataPath(char *v_path);
	bool loadData();
	bool loadYC(vector<ST_GeCtrlChl> &vecChan);
	bool loadYX(vector<ST_GeCtrlChl> &vecChan);
	bool loadYK(vector<ST_GeCtrlChl> &vecChan);
	bool loadYT(vector<ST_GeCtrlChl> &vecChan);
	void setSetting(Q_GSetting *v_setting);
	void setLog(Log *v_log);

private:
	//当遥测遥信打不开的时候，需要拷备旧文件保证程序正常可读。
	bool copyFile(char *ftype);
	void setLogPath();
	void initLogPath();
private:
	char m_Path[MAX_PATH];
	int m_nMaxAnalogCount;//最大的遥测个数
	int m_nMaxDigitalCount;//最大的遥信个数
	int m_nMaxYKCount;//最大的遥控个数
	int m_nMaxYTCount;//最大的遥调个数
	int m_nChanCount;//通道个数
	Q_GSetting *m_setting;
	char chInfo[Q_FRAME_CONST256];//信息内容
	char m_logPath[MAX_PATH];
	Q_dataManager* m_pMgr;
	Log *m_log;
};
#endif