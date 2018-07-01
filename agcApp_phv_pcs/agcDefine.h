#ifndef __AGC_DEFINE_DEFINED_H_
#define __AGC_DEFINE_DEFINED_H_
#include "AGC.h"
#include "gSystemDef.h"
static const char *g_ParamKey[ 30 ] =
{
	"",
	"CalcInterval",   //1 调节计算时间间隔(毫秒)
	"SendCmdInterval",// 下发命令时间间隔(毫秒)
	"ReSendFlg",//5 执行命令不成功重复发送
	"LogFlg", // 日志开关
	"LockCheckFlg",// 闭环检查开启
	"SafeCheckFlg", //14 安全约束开启
	"DebugFlg",
	"AdMethod",         //调节方式调节策略(1按开机容量2按裕度)
	"OneMinLimitFlg",				    //启用1分钟变化率限制
	"CmdTimeOut",  //判断命令执行不到位时间默认秒
	"AGCDataLog",
	"AGCDataLogPeriod"
};
typedef enum _enmAgcParamKey
{
	eAgcParamNull=0,
	eAgcParamCalcInterval=1,
	eAgcParamSendCmdInterval=2,
	eAgcParamReSendFlg=3,
	eAgcParamLogFlg=4,
	eAgcParamLockCheckFlg=5,
	eAgcParamSafeCheckFlg=6,
	eAgcParamDebugFlg=7,
	eAgcParamAdMethod=8,
	eAgcParamOneMinLimitFlg=9,
	eAgcParamCmdTimeOut=10,
	eAgcParamAGCDataLog=11,
	eAgcParamAGCDataLogPeriod=12

}enmAgcParamKey;

static const char *g_AgcDigitKey[ 20 ] =
{
	"",
	"AgcOptionOn",// AGC投退信号
	"AgcCycleOn", // AGC投入闭环控制
	"TotalBlock",
	"PhvChannel"
};
typedef enum _enmAgcDigitKey
{
	eAgcDigitNull=0,
	eAgcDigitOptionOn=1,
	eAgcDigitCycleOn=2,
	eAgcDigitTotalBlock=3,
	eAgcDigitPhvChannel=4

}enmAgcDigitKey;

static const char *g_AgcAnalogKey[ 30 ] =
{
	"",
	"NetUa",   //1 并网点有功
	"NetUb",// 并网点无功
	"NetUc", //并网点电压
	"NetUMax",// 并网点cos
	"NetUMin",//并网点频率
	"ULimit", // 辐照强度
	"PhvSumPower",//逆变器有功总和
	"PhvOpenCount",// 逆变器开机台数
	"PhvOpenCap",// 逆变器开机容量
	"SocSumPower",//AGC远方计划值 10
	"Capicity", //总装机容量(kW)
	"SocChargeMax",
	"SocDischargeMin",
	"SocChargePMax",
	"SocDischargePMax",
	"FtPowerU",
	"SocChargekW",//储能稳定时充电功率(kW)
	"SocDischargeKw", //储能稳定时放电功率(kW)
	"AdVoltModify",
	"ChargeBeginHour",
	"ChargeBeginMin",
	"ChargeEndHour",
	"ChargeEndMin",
	"DischargeBeginHour",
	"DischargeBeginMin",
	"DischargeEndHour",
	"DischargeEndMin",
	"SocChPhvFactor"



};
typedef enum _enmAgcAnalogKey
{
	eAgcAnalogNull=0,
	eAgcAnalogNetUa=1,
	eAgcAnalogNetUb=2,
	eAgcAnalogNetUc=3,
	eAgcAnalogNetUMax=4,
	eAgcAnalogNetUMin=5,
	eAgcAnalogULimit=6,
	eAgcAnalogPhvSumPower=7,
	eAgcAnalogPhvOpenCount=8,
	eAgcAnalogPhvOpenCap=9,
	eAgcAnalogSocSumPower=10,
	eAgcAnalogCapicity=11,
	eAgcAnalogSocChargeMax=12,
	eAgcAnalogSocDischargeMin=13,
	eAgcAnalogSocChargePMax,
	eAgcAnalogSocDischargePMax,
	eAgcAnalogFtPowerU,
	eAgcAnalogSocChargekW,
	eAgcAnalogSocDischargeKw,
	eAgcAnalogAdVoltModify,
	eAgcAnalogChargeBeginHour,
	eAgcAnalogChargeBeginMin,
	eAgcAnalogChargeEndHour,
	eAgcAnalogChargeEndMin,
	eAgcAnalogDischargeBeginHour,
	eAgcAnalogDischargeBeginMin,
	eAgcAnalogDischargeEndHour,
	eAgcAnalogDischargeEndMin,
	eAgcAnalogSocChPhvFactor



}enmAgcAnalogKey;


static const char *g_AgcAlarmKey[ 20 ] =
{
	"",
	"OffNet",           //1 AGC自动切本地运行
	"PcsFault",         // AGC自动切远方运行
	"PcsFaultOverLoad", // AGC与主站通道断开
	"BusVoltHigh",      // AGC与主站通道恢复
	"OverLoad",         // AGC与主站通道恢复
	"SocLowOverLoad",   // AGC与主站通道恢复
	"PcsFaultStartNoExec",// AGC与主站通道恢复
	"PhvChBroken",      // AGC与主站通道恢复
	"PcsFaultChargeNoExec",   
	"PcsFaultDischargeNoExec",   
	"PcsFaultWaitNoExec"
};
typedef enum _enmAlarmKey
{
	eAgcAlarmNull=0,
	eAgcAlarmOffNet=1,
	eAgcAlarmPcsFault=2,
	eAgcAlarmPcsFaultOverLoad=3,
	eAgcAlarmBusVoltHigh=4,
	eAgcAlarmOverLoad=5,
	eAgcAlarmSocLowOverLoad=6,
	eAgcAlarmPcsFaultStartNoExec=7,
	eAgcAlarmPhvChBroken=8,
	eAgcAlarmPcsFaultChargeNoExec=9,   
	eAgcAlarmPcsFaultDischargeNoExec=10,   
	eAgcAlarmPcsFaultWaitNoExec=11
}enmAgcAlarmKey;




#define CALC_TH_KeyID 1 //计算线程标识
#define  PLAN_TH_KeyID 2 //计划线程标识
#define CALC_TH_AuxKeyID 3
#define AGCCALC_TIMESPAN 50 //AGC等待时间最小间隔

/*AGC本地控制方式*/
typedef enum _enmCMD_MODE
{
eCMD_MODE_INVALID=0,  //无效
eCMD_MODE_SETPOINT=2, //设点模式


}enmCMD_MODE;


//错误标识
#define _ERROR_GET_REAL_VAL_FAIL -5    //读取实时目标值时发生错误


const double AGC_CMD_DIFF_VAL_UGEXEC=2000;    //当有功命令差值超过此值后，如果当前逆变器在执行中，重新分配命令
const int    AGC_RE_SEND_MAX_TIMES=3;//最多重发次数

const char * const AGC_PATH_LOGFILE= "agclogfile.log";
const char * const AGC_LOG_PATH = "agc";

#ifdef WINDOWS_ENV
//#define COMM_PLAN_CURVE_DIR							"\\data\\" //AGC远程曲线
#define COMM_PLAN_CURVE_DIR							"\\data\\remote_agc\\" //AGC远程曲线
#define COMM_PLAN_LOCAL_CURVE_DIR					"\\data\\local_agc\\" //kc add AGC本地曲线路径
const char * const agcDataLogPath="\\data\\message\\agc\\";
#else
#define COMM_PLAN_CURVE_DIR							"/data/remote_agc/" //AGC远程曲线
#define COMM_PLAN_LOCAL_CURVE_DIR					"/data/local_agc/" //kc add AGC本地曲线路径

const char * const agcDataLogPath="/data/message/agc/";
#endif

#define SOC_POWER_STATUS_Charge 1  //充电
#define SOC_POWER_STATUS_Discharge 2  //放电
#define SOC_POWER_STATUS_Auto  0 //其它
#define Battery_Status_Null  0  // 未定义
#define Battery_Status_Full  4369   // 电量满
#define Battery_Status_Empty  21845    //电量空
#define Battery_Status_Alarm  21845   //报警
#define Battery_Status_Fault  43690 //故障
#define Battery_Status_Normal  48059 //正常
//定义一个机组
typedef struct _GeneratorUnit
{
 AGC_PhvUnit *p_Unit;
 time_t lastAdjustTime;//上一次调节时间
 float lastObjectValue;//上一次发送命令值
 int m_ReSendTimes;//当前重复发送的次数
 bool adjustFlag;
 bool enabled;
 int ratio;//调节比率一般为0-1000，原来值为0到100
 bool canAdjust; 
 float m_CanAdd;//可增
 float m_CanSub;//可减
 _GeneratorUnit()
 {
	 time(&lastAdjustTime);
	 p_Unit=NULL;
	 lastObjectValue=0;
	 m_ReSendTimes = 0;
	 adjustFlag = false;
	 enabled = true;
//	 ratio=0;
	 canAdjust=true;
	 m_CanAdd=0;
	 m_CanAdd=0;
 }

}GeneratorUnit;

//定义一个机组
typedef struct _GeSocUnit
{
	AGC_SocUnit *p_Unit;
	time_t lastAdjustTime;//上一次调节时间
	float lastObjectValue;//上一次发送命令值
	int m_ReSendTimes;//当前重复发送的次数
	bool adjustFlag;
	bool enabled;
	bool canAdjust; 
	bool m_FlgSendStart;//开机标志
	_GeSocUnit()
	{
		time(&lastAdjustTime);
		p_Unit=NULL;
		lastObjectValue=0;
		m_ReSendTimes = 0;
		adjustFlag = false;
		enabled = true;
		canAdjust=true;

		m_FlgSendStart=false;
		m_ReSendTimes=0;
	}
	//检查是充电或者是放电
	//1充电
	//2 放电
	int checkCFStatus()
	{
		if(p_Unit==NULL)
			return SOC_POWER_STATUS_Auto;
		
	
		if(p_Unit->fValUp>0)
		{
			//	kW > 0: power into the grid
			return SOC_POWER_STATUS_Discharge; 
		}
		else if(p_Unit->fValUp<0)
		{
			//kW < 0: power drawn from the grid
			return SOC_POWER_STATUS_Charge;
		}
		else
		{
			return SOC_POWER_STATUS_Auto;
		}
	}
}GeSocUnit;

#define PHVUNIT_EXEC 1
#define PHVUNIT_NOEXEC 0 

typedef enum _AgcAdMethod
{
  eAdMethodNULL=0,//空
  eAdMethodByOpenCapicity=1,//按开机容量
  eAdMethodByMargin=2, //按裕度调节
  eAdMethodByRequest=3, //按需调节
}AgcAdMethod;

#define Quality_Bad_AGC_P 2   //实时无功坏数据
#define Quality_Bad_AVC_UpCmd 4 //指令上送坏数据
#define Quality_Bad_AVC_Unit 8  //机组状态坏数据
#define Quality_Bad_AVC_Channel 16 //通道状态坏数据
#define Quality_Bad_AVC_Exception 32  // 故障状态坏数据
#define Quality_Bad_AVC_UpLimit 64     //调节上限坏数据
#define Quality_Bad_AVC_DownLimit 128  //调节下限坏数据



#define SOC_PCS_EXP_STATUS_Normal 0  //储能pss故障状态 正常
#define SOC_PCS_EXP_STATUS_Warning 1
#define SOC_PCS_EXP_STATUS_Fault 2


#define SOC_PCS_RUN_STATUS_Stopped 1  //储能运行状态为停机
#define SOC_PCS_RUN_STATUS_Run 2       //储能运行状态为运行
#define SOC_PCS_RUN_STATUS_Undefined 9   //储能运行状态未定义
typedef enum _ServerBkType
{
	eSvrBkTypeNULL=0,//空
	eSvrBkTypeMaster=1,//主机
	eSvrBkTypeSlave=2  //备机
}ServerBkType;

#define AGC_SUC 0
#define AGC_FAIL -1
#endif