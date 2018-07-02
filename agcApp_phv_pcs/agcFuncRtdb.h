#ifndef __AGC_RTDB_DEFINED_H
#define __AGC_RTDB_DEFINED_H
#include "gSystemDef.h"
#include "MultiDataBase.h"
#include "ScadaAPI.h"
#include "AGC.h"
#include "AgcDataBase.h"
#include "MsgDealer.h"
#include "agcDefine.h"
#include "CFuncUtil.h"
//const char * const HOME_NAME = "CCSOSD";
class agcFuncRtdb
{
public :
	agcFuncRtdb();
	~agcFuncRtdb();
public:

public:
	bool rtdb_init();
	bool dbmg_init();
	int rtdb_login();
	int rtdb_logout();
	//ˢ�»���
	BOOL freshPhvUnit();
    //ˢ��ң��
	BOOL freshDigit();
	//ˢ��ң��
	BOOL freshAnlog();
	//ˢ�»���
	BOOL freshSocUnit();
	CMultiDataBase *getMainDb();
	CScadaDataBase *getScadaDb();
	CAgcDataBase *getAgcDb();
	//������Ϣ������
	void setMsgHandler(MsgDealFuncType pFunc);
	int getCommAdParam(enmAgcParamKey key);
	int setAgcWarn(enmAgcAlarmKey key, int iVal);
	int getAgcAlarm(enmAgcAlarmKey key);
	//����ֵtrueΪ��ʾ�ҵ��˼�¼
	//����ֵfalse��ʾû���ҵ���ң��
	BOOL findYk(RControl *rc,AGC_Digit * &rec);
	//����ֵtrueΪ��ʾ�ҵ��˼�¼
	//����ֵfalse��ʾû���ҵ���ң��
	BOOL findYt(RSetpoint *rs,AGC_Analog * &rec);
	//���ң�����Ƿ����
	BOOL checkYtExists(int iStation,int iDevice,int iPoint);
	//�����ڷ���-1
	int getAgcDigit(enmAgcDigitKey key);
	float getAgcAnalog(enmAgcAnalogKey key);
	//��ͬʱ����scada����AGC_Analog�ļ�¼��
	bool uptAlgRecordBykW(enmAgcAnalogKey key,float val);
	//��ͬʱ����scada����AGC_Digit�ļ�¼��
	bool uptDigRecord(enmAgcDigitKey key, int val);
    bool uptDigRecord(int  nID,int val);
	//����ֵtrue��ʾ�ɹ�,falseʧ��
	//chidͨ����
	//device �豸��
	//no ң�����
	//val��ֵ
	bool sendDeviceExecYt(int chid,int device,int no,float val);
	//����ң��
	bool sendDeviceExecYk(int chid,int device,int no,int val);
	//���AGC��־
	void clearAgcLog();
	//���AGC��־
	void pushAgcLog(char *msg);
	//�ж�float�����Ƿ���ͬ
	bool equalDouble(float num1,float num2);
	//��ͬʱ����scada����AVC_Analog�ļ�¼��
	//checkUnit�Ƿ�����ţ�ֻ���й��ĵ�λ��������
	BOOL uptAlgRecordByID(int nid,float val,bool checkUnit=false);
	//�õ���ǰ��������״̬
	//1���� 0����
	ServerBkType getServerBkType();
	int initAgcAlarmValue();
private:
	CMultiDataBase *m_dbmg_mdb;
	CScadaDataBase *m_dbmg_sdb;
	CAgcDataBase *m_dbmg_agc;
	CMsgDealer * _msg_dealer;
	int nLogCurIndex;//��ǰд���¼λ��
	CFuncUtil *m_Util;
	char m_LastError[300];
	BOOL m_bPhvUnitTableOK;//�Ƿ��ʹ��������
	
};

typedef struct _adjust_param
{
	float PMaxSum ; //�ɵ�������
	float PMinSum; //�ɵ�������
	float PRealSum ;  //ʵ���ܳ���
	float PRealRatingSum; //ʵ�ʿɵ��ڿ��������ܺ�
	float pRatingSum;//�װ�������ܺ�
	bool bIsLocked;//�˴��Ƿ����
	float fLastTarget;//��һ��ָ��
	float fConnectPower;//�������й�
	float fCurTargetP;//��ǰĿ��
	_adjust_param()
	{
		PMaxSum=0;
		PMinSum=0;
		PRealSum=0; 
		PRealRatingSum=0; 
		pRatingSum=0;
		bIsLocked=false;
		fLastTarget=0;
		 fConnectPower=0;
		 fCurTargetP=0;
	}
}adjust_param;

#endif