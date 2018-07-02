#ifndef __AGC_CALC_DEFINED_H_
#define __AGC_CALC_DEFINED_H_
#include "gSystemDef.h"
#include "MsgDealer.h"
#include "Q_frameInclude.h"
#include "agcFuncRtdb.h"
#include "agcDefine.h"
#include "agcStructure.h"
#include "RtdbTabSave.h"
#include "DeleteLog.h"

class agcCalc
{
public:
	agcCalc(void);
	virtual ~agcCalc(void);
	//��ʹ��
	bool Init();
	//��ʼ����
	bool Start();
	//ֹͣ����
	bool Stop();
	//ˢ������
	void freshdata();
	//�õ�����
	int getCommand();
	//������� ����true ������������
	bool isLocked();
	//��ȫԼ����� ����true��ȫԼ����ͨ��
	bool isSafeConstraint();
	//����
	void distribute();
	//ִ�н��
	void execResults();

	//nType �̱߳�ʶ
	//�ȴ�ʱ��
	//����ֵtrue�ȴ���ʱ false �˳��߳�
	bool waitForTimeout(int nType,int nTimespan);
    //����ң��
	void handleYk(AGC_Digit *rec);
	//����ң��
	void handleYt(AGC_Analog *rec);

    agcFuncRtdb *getFuncAgcRtdb();
	//�Ƿ�����
	bool isLaunched();

	//�Ƿ�Զ������,����ֵtrue�ջ����У�false��������
	bool isExec();
	Q_ThreadStatusType getRunStatus();
	//����Զ��ң������
	void resetRemoteYtAlarm();
	//��������Ƿ�Ϊ���� trueΪ����,falseΪ����
	bool checkIsMaster();
private:
	//�̺߳���
	// �����߳�
	static  ThreadFuncRet CalcThreadProc(LPVOID lpParameter);
	//���㸨��
	static  ThreadFuncRet FreshThreadProcAux(LPVOID lpParameter);
	static ThreadFuncRet  UpdataProcessPara(LPVOID lpParameter);
	static void ProcessMsg(tabMessage * pMsg);
	//����ֵtrue�ɹ� falseʧ��
	//nRes�õ�������Ľ��
	bool getCmdSetPointValue(bool bRemote,float &nRes);
	//������
	//output nRes���صõ�������
	//flg�Ƿ񵥶�ȡԶ���򱾵�Ŀ��,falseĬ��Ϊ���ݵ�ǰ���з�ʽȡ�õ�ǰ����������remoteflgȡ������
	//remoteflag��Զ���򱾵�ȡ������ true��ʾȡ�õ�ǰԶ������,false��ʾȡ��ǰ��������
	int readCurrentDateTarget(float& nRes,bool flg,bool remoteflg);

	//�жϵ���Ŀ���Ƿ���Ч
	bool checkAdjustObject(time_t currentTime,GeneratorUnit *p);
	//ֱ��ִ��,������ã�һ�����ÿһ������䶼��ʱ����
	void execDirect();
	//д��־����ʵʱ���ݿ��м�¼��
	void writeBuffer(char* buffer);
	float modifyCurrentTarget(float dTarget,float dMaxOnePower,float curPower);
	//�ж�ң�����Ƿ���ȷ����
	bool checkYtExists();
	void distribute_phv();
	//�ȶ����Ʋ���
	int disNormalMethod();
	//���ܿ��Ʋ���
	//dTarget ����Ŀ�꣬Ӧ����һ����ֵ���ɡ� 
	//>0�ŵ� <0��� 0����
	int pcsControlMethod(float dTarget);
	//��ѹ����
	int UpVoltage();
	//��ѹ����
	int DownVoltage();
	//���ܳ�����
   int pcsChargeMethod(float dTarget);
	//���ܷŵ����
	int pcsDischargeMethod(float dTarget);
	//���ܴ�������
	int pcsWaitMethod();
	//���ܿ�������
	int pcsStartMethod();
	//ͨ�ñ������
	int commonVaribleCheck();
	//�����繦�ʲ�ֵ
	float calcChargePower();
	//����ŵ繦�ʲ�ֵ
	float calcDischargePower();
	//�����ִ���㷨
	void execPhv();
	//����ִ���㷨
	void execPcs();
	//��鷢�͵������Ƿ�Ϸ�
	bool checkAdjustObjectPcs(time_t currentTime,GeSocUnit *p);
	//ͨ�ñ������
	int cheakCommonVarible();
	//����ֵ0���� 1��� 2 ���
	int getCurrentChargetOrDis();
	//�õ���̬�³�繦��
	float getNormalChargePower();
private:
	CThreadEvent m_CalcEvent;
	CThreadEvent m_FreshEventAux;
	CThreadMutex m_lockFreshData;
	agcFuncRtdb m_funcRtdb;

	Q_AgcParameter m_adParameter;//���ڲ���
	float m_fTotalInstCap;//Ĭ��װ�����������մ�װ����������Ĭ�ϵļƻ�����
	float m_getTarget;//�õ���Ŀ��ֵ
	int m_nGenCount;       //��Ⱥ����
	int m_nSocCount;       //��Ⱥ����
	GeneratorUnit** m_pGenInfo;  //���л�Ⱥ��Ϣ
	GeSocUnit* m_pSocInfo;  //���д�����Ϣ
	float m_fnoCanAdjust;//���ɵ��豸����
	Q_ThreadStatusType m_eCalcThreadStatus;//�����߳�״̬
	volatile bool m_bThreadRunning;//�߳����б�־
	volatile bool m_bNeedStopProcess;//�Ƿ�ֹͣ����
	char m_LastError[300];//��ӡ����ʹ��

	time_t m_timFirstStart;//��һ������ʱ��
	bool m_bFistStartFlg;//�����һ������
	time_t m_timDiscon;//����վͨ�ŶϿ�ʱ���

	float m_1minrectar;
	time_t m_t1minstmp;		//1���ӱ仯�ʣ�1����ʱ���
    float c_AdMaxfactor;//����ԣ���ۿ�ϵ��
	CRtdbTabSave *m_pAgcRtdbSaveAnalog; //���ڼ�¼������Ϣ����ָ�� ��ʱָ������û�и�ֵ
	DeleteLog *m_pDeleteLog; //���ڼ�¼������Ϣ����ָ��
	float m_targetOld;
};

#endif