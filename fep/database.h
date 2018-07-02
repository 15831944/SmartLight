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
	//��ң��ң�Ŵ򲻿���ʱ����Ҫ�������ļ���֤���������ɶ���
	bool copyFile(char *ftype);
	void setLogPath();
	void initLogPath();
private:
	char m_Path[MAX_PATH];
	int m_nMaxAnalogCount;//����ң�����
	int m_nMaxDigitalCount;//����ң�Ÿ���
	int m_nMaxYKCount;//����ң�ظ���
	int m_nMaxYTCount;//����ң������
	int m_nChanCount;//ͨ������
	Q_GSetting *m_setting;
	char chInfo[Q_FRAME_CONST256];//��Ϣ����
	char m_logPath[MAX_PATH];
	Q_dataManager* m_pMgr;
	Log *m_log;
};
#endif