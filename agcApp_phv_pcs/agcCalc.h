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
	//初使化
	bool Init();
	//开始运行
	bool Start();
	//停止运行
	bool Stop();
	//刷新数据
	void freshdata();
	//得到命令
	int getCommand();
	//闭锁检查 返回true 闭锁条件成立
	bool isLocked();
	//安全约束检查 返回true安全约束不通过
	bool isSafeConstraint();
	//分配
	void distribute();
	//执行结果
	void execResults();

	//nType 线程标识
	//等待时间
	//返回值true等待超时 false 退出线程
	bool waitForTimeout(int nType,int nTimespan);
    //处理遥控
	void handleYk(AGC_Digit *rec);
	//处理遥调
	void handleYt(AGC_Analog *rec);

    agcFuncRtdb *getFuncAgcRtdb();
	//是否运行
	bool isLaunched();

	//是否远方运行,返回值true闭环运行，false开环运行
	bool isExec();
	Q_ThreadStatusType getRunStatus();
	//复归远方遥调报警
	void resetRemoteYtAlarm();
	//如果主机是否为主机 true为主机,false为备机
	bool checkIsMaster();
private:
	//线程函数
	// 计算线程
	static  ThreadFuncRet CalcThreadProc(LPVOID lpParameter);
	//计算辅助
	static  ThreadFuncRet FreshThreadProcAux(LPVOID lpParameter);
	static ThreadFuncRet  UpdataProcessPara(LPVOID lpParameter);
	static void ProcessMsg(tabMessage * pMsg);
	//返回值true成功 false失败
	//nRes得到的命令的结果
	bool getCmdSetPointValue(bool bRemote,float &nRes);
	//读命令
	//output nRes返回得到的命令
	//flg是否单独取远方或本地目标,false默认为根据当前运行方式取得当前命令，否则根据remoteflg取得命令
	//remoteflag按远方或本地取得命令 true表示取得当前远方命令,false表示取当前本地命令
	int readCurrentDateTarget(float& nRes,bool flg,bool remoteflg);

	//判断调节目标是否有效
	bool checkAdjustObject(time_t currentTime,GeneratorUnit *p);
	//直接执行,这个常用，一般就是每一条命令间都有时间间隔
	void execDirect();
	//写日志到到实时数据库中记录表
	void writeBuffer(char* buffer);
	float modifyCurrentTarget(float dTarget,float dMaxOnePower,float curPower);
	//判断遥调点是否正确配置
	bool checkYtExists();
	void distribute_phv();
	//稳定控制策略
	int disNormalMethod();
	//储能控制策略
	//dTarget 调节目标，应该是一个差值即可。 
	//>0放电 <0充电 0待机
	int pcsControlMethod(float dTarget);
	//升压策略
	int UpVoltage();
	//降压策略
	int DownVoltage();
	//储能充电策略
   int pcsChargeMethod(float dTarget);
	//储能放电策略
	int pcsDischargeMethod(float dTarget);
	//储能待机策略
	int pcsWaitMethod();
	//储能开机策略
	int pcsStartMethod();
	//通用变量检查
	int commonVaribleCheck();
	//计算充电功率差值
	float calcChargePower();
	//计算放电功率差值
	float calcDischargePower();
	//逆变器执行算法
	void execPhv();
	//储能执行算法
	void execPcs();
	//检查发送的命令是否合法
	bool checkAdjustObjectPcs(time_t currentTime,GeSocUnit *p);
	//通用变量检查
	int cheakCommonVarible();
	//返回值0待机 1充电 2 充电
	int getCurrentChargetOrDis();
	//得到稳态下充电功率
	float getNormalChargePower();
private:
	CThreadEvent m_CalcEvent;
	CThreadEvent m_FreshEventAux;
	CThreadMutex m_lockFreshData;
	agcFuncRtdb m_funcRtdb;

	Q_AgcParameter m_adParameter;//调节参数
	float m_fTotalInstCap;//默认装机容量，按照此装机容量生成默认的计划曲线
	float m_getTarget;//得到的目标值
	int m_nGenCount;       //机群总数
	int m_nSocCount;       //机群总数
	GeneratorUnit** m_pGenInfo;  //所有机群信息
	GeSocUnit* m_pSocInfo;  //所有储能信息
	float m_fnoCanAdjust;//不可调设备容量
	Q_ThreadStatusType m_eCalcThreadStatus;//计算线程状态
	volatile bool m_bThreadRunning;//线程运行标志
	volatile bool m_bNeedStopProcess;//是否停止处理
	char m_LastError[300];//打印错误使用

	time_t m_timFirstStart;//第一次启动时间
	bool m_bFistStartFlg;//程序第一次启动
	time_t m_timDiscon;//与主站通信断开时间点

	float m_1minrectar;
	time_t m_t1minstmp;		//1分钟变化率，1分钟时间戳
    float c_AdMaxfactor;//可增裕度折扣系数
	CRtdbTabSave *m_pAgcRtdbSaveAnalog; //定期记录运行信息对象指针 暂时指针内容没有赋值
	DeleteLog *m_pDeleteLog; //定期记录运行信息对象指针
	float m_targetOld;
};

#endif