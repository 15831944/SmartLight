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
	//刷新机组
	BOOL freshPhvUnit();
    //刷新遥信
	BOOL freshDigit();
	//刷新遥测
	BOOL freshAnlog();
	//刷新机组
	BOOL freshSocUnit();
	CMultiDataBase *getMainDb();
	CScadaDataBase *getScadaDb();
	CAgcDataBase *getAgcDb();
	//设置消息处理函数
	void setMsgHandler(MsgDealFuncType pFunc);
	int getCommAdParam(enmAgcParamKey key);
	int setAgcWarn(enmAgcAlarmKey key, int iVal);
	int getAgcAlarm(enmAgcAlarmKey key);
	//返回值true为表示找到了记录
	//返回值false表示没有找到此遥控
	BOOL findYk(RControl *rc,AGC_Digit * &rec);
	//返回值true为表示找到了记录
	//返回值false表示没有找到此遥控
	BOOL findYt(RSetpoint *rs,AGC_Analog * &rec);
	//检查遥调点是否存在
	BOOL checkYtExists(int iStation,int iDevice,int iPoint);
	//不存在返回-1
	int getAgcDigit(enmAgcDigitKey key);
	float getAgcAnalog(enmAgcAnalogKey key);
	//得同时更新scada表与AGC_Analog的记录表
	bool uptAlgRecordBykW(enmAgcAnalogKey key,float val);
	//得同时更新scada表与AGC_Digit的记录表
	bool uptDigRecord(enmAgcDigitKey key, int val);
    bool uptDigRecord(int  nID,int val);
	//返回值true表示成功,false失败
	//chid通道号
	//device 设备号
	//no 遥调点号
	//val是值
	bool sendDeviceExecYt(int chid,int device,int no,float val);
	//发送遥控
	bool sendDeviceExecYk(int chid,int device,int no,int val);
	//清空AGC日志
	void clearAgcLog();
	//添加AGC日志
	void pushAgcLog(char *msg);
	//判断float类型是否相同
	bool equalDouble(float num1,float num2);
	//得同时更新scada表与AVC_Analog的记录表
	//checkUnit是否检查符号，只有有功的单位才有意义
	BOOL uptAlgRecordByID(int nid,float val,bool checkUnit=false);
	//得到当前机器主动状态
	//1主机 0备机
	ServerBkType getServerBkType();
	int initAgcAlarmValue();
private:
	CMultiDataBase *m_dbmg_mdb;
	CScadaDataBase *m_dbmg_sdb;
	CAgcDataBase *m_dbmg_agc;
	CMsgDealer * _msg_dealer;
	int nLogCurIndex;//当前写入记录位置
	CFuncUtil *m_Util;
	char m_LastError[300];
	BOOL m_bPhvUnitTableOK;//是否初使化过机组
	
};

typedef struct _adjust_param
{
	float PMaxSum ; //可调节上限
	float PMinSum; //可调节下限
	float PRealSum ;  //实际总出力
	float PRealRatingSum; //实际可调节开机容量总和
	float pRatingSum;//额定装机容量总和
	bool bIsLocked;//此次是否闭锁
	float fLastTarget;//上一个指令
	float fConnectPower;//并网点有功
	float fCurTargetP;//当前目标
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