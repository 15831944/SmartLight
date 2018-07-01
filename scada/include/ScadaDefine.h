/****************************************************************************
 * ScadaAPI.h : SCADA各种数据类型定义									*
 ****************************************************************************/
#ifndef __AFX_SCADA_DEFINE_H__
#define __AFX_SCADA_DEFINE_H__

#include "AlarmDefine.h"
#include "SCADA.h"
#include "MultiDataBase.h"
#include "NetStructDef.h"

/////////////////////////////////////////////////////////////////////////////
//RControl.iTaskType的定义
typedef struct _tagRControl
{
	INT			iStation;		// Station index
	INT			iYKPoint;		// 遥控点号
	INT			iCommandType;	// 遥控命令码
	INT			iVal;			// 遥控目标值
	INT         iDevice;        //设备号 2015-06-30 tjs 
	INT         iZf;//是否为转发
	INT         iYxID;//关联遥信ID 2015-12-9 tjs 
	_tagRControl()
	{
		iZf=0;//0非转发,1转发
		iYxID=0;

	}
} RControl;//遥控信息结构体

//2015-06-29
typedef struct _tagRSetpoint
{
	INT			iStation;		// Station index
	INT			iPoint;		// 遥调点号
	INT			iCommandType;	// 遥调命令码
	float		fVal;			// 遥调目标值
	INT         iDevice;
	INT         iZf;//是否为转发
	INT         iYcID;//关联遥测ID 2015-12-9 tjs 
	_tagRSetpoint()
	{
		iZf=0;//0非转发,1转发
	}
} RSetpoint;//遥调信息结构体


//*****20150907樊硕添加用于规约打印
typedef struct _tagRMsgPrint
{
	INT			iChannelID;	   // Station index
	INT			iMsgType;	   // 打印类型
	int         nlen;        //长度
	BYTE	    msg[200];      // 内容

}RMsgPrint;//规约打印结构体



/////////////////////////////////////////////////////////////////////////////
typedef struct _tagYXChange
{
        UINT			uElePointID;	// 遥信变位点
        BOOL			bOpen;		// 分合
        BOOL			bFault;	        // 故障跳闸
} YXChange;//遥信变位消息体，供ScadaAPI和ScadaServer使用，用于跳闸统计
/////////////////////////////////////////////////////////////////////////////
// 通道类型定义(2003-04-02):
#define CHANNEL_TYPE_ASYNCOMM			1 //异步串口
#define	CHANNEL_TYPE_SYNCOMM			2 //同步串口
#define	CHANNEL_TYPE_BUS422				3 //总线422
#define	CHANNEL_TYPE_BUS485				4 //总线485
#define	CHANNEL_TYPE_LONWORK			5 //Lonwork网
#define	CHANNEL_TYPE_PROFIBUS			6 //PROFI总线
#define	CHANNEL_TYPE_TCPIPNET			7 //TCP/IP
/////////////////////////////////////////////////////////////////////////////
// 规约类型定义(2003-04-02):
#define PROTOCOL_TYPE_PUNKNOW			-1
#define PROTOCOL_TYPE_IEC101			0
#define PROTOCOL_TYPE_IEC102			1
#define PROTOCOL_TYPE_IEC103			2
#define PROTOCOL_TYPE_IEC104			3
#define PROTOCOL_TYPE_DNP				4
#define PROTOCOL_TYPE_SCI1801			5
#define PROTOCOL_TYPE_CDT				6
#define PROTOCOL_TYPE_FM				7
#define PROTOCOL_TYPE_GPS				8
#define PROTOCOL_TYPE_PMODBUS			9
#define PROTOCOL_TYPE_PSV				10
#define PROTOCOL_TYPE_POLLING			11
#define PROTOCOL_TYPE_CSE				12
#define PROTOCOL_TYPE_VEC				13

#define PROTOCOL_TYPE_FDK_DCF5			601	//青州扩展CDT-东方电子
#define PROTOCOL_TYPE_OLD_DCF5			602	//青州扩展CDT-东方电子
#define	PROTOCOL_TYPE_QINGZHOU_CDT		603	//青州扩展CDT-青州
#define PROTOCOL_TYPE_HAINYANG_CDT		604	//青州扩展CDT-海洋
/////////////////////////////////////////////////////////////////////////////
// 采集单元类型定义(2003-04-02):
#define STATION_TYPE_ACTUAL				1	//实际单元
#define	STATION_TYPE_CALCULATE			2	//计算单元
#define	STATION_TYPE_TRANSMIT			3	//转发单元
#define	STATION_TYPE_MIMIC				4	//模拟单元
#define	STATION_TYPE_RTU				5	//远动单元
#define	STATION_TYPE_FTU				6	//配电单元
#define	STATION_TYPE_PROTECT			7	//保护装置
/////////////////////////////////////////////////////////////////////////////
// 遥测量类型定义(2003-04-02):
#define	Analog_Type_I						1			//	电流
#define	Analog_Type_U						2			//	电压
#define	Analog_Type_I_phase					3			// 相电流
#define	Analog_Type_P						4			//	有功
#define	Analog_Type_I_line					5			// 线电流
#define	Analog_Type_I_zero					6			// 零序电流
#define	Analog_Type_U_phase					7			// 相电压
#define	Analog_Type_Q						8			//	无功
#define	Analog_Type_U_zero					9		// 零序电压
#define Analog_Type_3U0						27		//	3U0电压
#define Analog_Type_DWH						28		//	高压档位
#define Analog_Type_DWM						29		//	中压档位
#define	Analog_Type_F						16			//	周波
#define	Analog_Type_T						32			//	温度
#define	Analog_Type_C						64			//	功率因数
#define	Analog_Type_O						128		//	其它

#define	Analog_Type_IA						(1 << 8)		//	电流A相
#define	Analog_Type_IB						(1 << 9)		//	电流B相
#define	Analog_Type_IC						(1 << 10)		//	电流C相
#define	Analog_Type_UAB						(1 << 11)		//	电压AB相
#define	Analog_Type_UBC						(1 << 12)		//	电压BC相
#define	Analog_Type_UCA						(1 << 13)		//	电压CA相
#define	Analog_Type_UA						(1 << 14)		//	电压A相
#define	Analog_Type_UB						(1 << 15)		//	电压B相
#define	Analog_Type_UC						(1 << 16)		//	电压C相

#define	Analog_Type_U_line					(1 << 17)	// 线电压
#define	Analog_Type_Ux  			        (1 << 17)	//   线电压Ux
#define	Analog_Type_Iha     				(1 << 18)		//
#define	Analog_Type_Ima        				(1 << 19)	//
#define	Analog_Type_Ila      				(1 << 20)	//
#define	Analog_Type_Ph       				(1 << 21)	//
#define	Analog_Type_Pm    					(1 << 22)	//
#define	Analog_Type_Pl    				    (1 << 23)	//
#define	Analog_Type_Qh    				    (1 << 24)	//
#define	Analog_Type_Qm       				(1 << 25)	//
#define	Analog_Type_Ql      				(1 << 26)	//
#define	Analog_Type_T1      				(1 << 27)	//
#define	Analog_Type_T2       				(1 << 28)	//
#define	Analog_Type_T3       				(1 << 29)	//
#define	Analog_Type_T4      				(1 << 30)	//

/////////////////////////////////////////////////////////////////////////////
// 遥信量单位类型定义(2003-04-02):
#define	Analog_Unit_NONE					0		// 没有单位
#define	Analog_Unit_A						1		// 安培,A
#define	Analog_Unit_KA						2		// 千安,kA
#define	Analog_Unit_V						11		// 伏特,V
#define	Analog_Unit_KV						12		// 千伏,kV
#define	Analog_Unit_W						21		// 瓦特,W
#define	Analog_Unit_KW						22		// 千瓦,kW
#define	Analog_Unit_MW						23		// 兆瓦,MW
#define	Analog_Unit_Var						31		// 乏,Var
#define	Analog_Unit_KVar					32		// 千乏,kVar
#define	Analog_Unit_MVar					33		// 兆乏,MVar
#define	Analog_Unit_Hz						41		// 赫兹,Hz
#define	Analog_Unit_O						51		// 度,o
/////////////////////////////////////////////////////////////////////////////
// 遥信量类型定义(2003-04-02):
//1-99  用于位置信息

#define	Digit_Type_DLQ_1					1		// 断路器合位
#define	Digit_Type_DLQ_0					2		// 断路器分位
#define	Digit_Type_DZ_1						3		// 刀  闸合位
#define	Digit_Type_DZ_0						4		// 刀  闸分位
#define	Digit_Type_SC_1						5		// 手  车工作位
#define	Digit_Type_SC_0						6		// 手  车实验位
#define	Digit_Type_JDDZ_1					7		// 接地刀闸合位
#define	Digit_Type_JDDZ_0					8		// 接地刀闸分位
#define	Digit_Type_YaBan					21		// 压板状态		//1代表投入
#define	Digit_Type_LOCAL					31		// 就地			//0代表远方

//100-299 用于保护信息
#define	Digit_Type_RELAY					100		// 一般保护信号（主类）
#define	Digit_Type_Fault					101		// 事故总
#define	Digit_Type_Current1					111		// 过流一段
#define	Digit_Type_Current2					112		// 过流二段
#define	Digit_Type_Current3					113		// 过流三段
#define	Digit_Type_Current4					114		// 过流四段
#define	Digit_Type_Current5					115		// 反时限过电流保护
#define	Digit_Type_Current01				121		// 零序一段
#define	Digit_Type_Current02				122		// 零序二段
#define	Digit_Type_Current03				123		// 零序三段
#define	Digit_Type_Current04				124		// 零序四段
#define	Digit_Type_Current05				125		// 反时限零序保护
#define	Digit_Type_DISTANCE1				131		// 距离保护一段
#define	Digit_Type_DISTANCE2				132		// 距离保护二段
#define	Digit_Type_DISTANCE3				133		// 距离保护三段
#define	Digit_Type_DISTANCE4				133		// 距离保护四段
#define	Digit_Type_DISTANCEN1				141		// 接地距离保护一段
#define	Digit_Type_DISTANCEN2				142		// 接地距离保护二段
#define	Digit_Type_DISTANCEN3				143		// 接地距离保护三段
#define	Digit_Type_DISTANCEN4				143		// 接地距离保护四段
#define Digit_Type_LGas						151		// 轻瓦斯保护
#define Digit_Type_HGas						152		// 重瓦斯保护
#define Digit_Type_HHLoad					153		// 高压过负荷保护
#define Digit_Type_MHLoad					154		// 中压过负荷保护
#define Digit_Type_LHLoad					155		// 低压过负荷保护
#define Digit_Type_Loil						156		// 低油位保护
#define Digit_Type_HDiff					161		// 纵差保护
#define Digit_Type_VDiff					162		// 横差保护
#define Digit_Type_BusDiff					163		// 母线差动
#define Digit_Type_Direct					164		// 方向高频
#define Digit_Type_FxDirect					165		// 高频闭锁负序方向
#define Digit_Type_PhaseDiff				166		// 相差高频
#define Digit_Type_ILock_LOWU				181		// 电流闭锁低电压
#define Digit_Type_ULock_I                  182		// 电压闭锁电流
#define Digit_Type_LowVolf					183		// 低电压保护
#define Digit_Type_Loss						191		// 失灵保护
#define Digit_Type_JDXX						199		// 小电流接地选线


//大于250的为重合闸和自动装置
#define Digit_Type_SPhaseZCH				251		// 单相重合闸
#define Digit_Type_TPhaseZCH				252		// 三相重合闸
#define Digit_Type_MPhaseZCH				253		// 综合重合闸
#define Digit_Type_BZT0						261		// 备自投1
#define Digit_Type_BZT1						262		// 备自投2
#define Digit_Type_VoltOpSET				271		// 低压减载
#define Digit_Type_FreqOpSET				272		// 频率动作
#define Digit_Type_FlowOpSET				273		// 切负荷

#define Digit_Type_AllFault					280		// 全站事故总

//300-499 用于告警
#define	Digit_Type_Foretell					300		//  预告信号（主类）
#define	Digit_Type_PT						301		//  PT断线			//1断线
#define	Digit_Type_SPRING					302		//  弹簧未储能		//1未储能
#define	Digit_Type_Comm						303		//  装置通讯状态	//1中断
#define	Digit_Type_NodeWork					331		//  计算机节点运行状态	//1运行
#define	Digit_Type_Net1						332		//  计算机节点网络1运行状态	//1运行
#define	Digit_Type_Net2						333		//  计算机节点网络1运行状态	//1运行
#define	Digit_Type_Channel					334		//  通道运行状态	//1运行

//500-599 其它
#define	Digit_Type_Other					500		// 其它

/////////////////////////////////////////////////////////////////////////////
// 脉冲量类型定义(2003-04-02):
#define	Pulse_Type_P						1		// 有功电度
#define	Pulse_Type_Q						(1 << 1)// 无功电度
/////////////////////////////////////////////////////////////////////////////
// 峰谷平时段类型定义(2003-04-02):
#define Period_Type_SharpPeak				1		// 尖峰时刻
#define Period_Type_Peak					(1 << 1)// 峰时刻
#define Period_Type_Vale					(1 << 2)// 谷时刻
#define Period_Type_Flat					(1 << 3)// 平时刻
/////////////////////////////////////////////////////////////////////////////
// 遥测量静态标志定义(2003-04-02):
#define Analog_Scan_Enable					1			//	扫描使能
#define Analog_Alarm_Enable					(1 << 1)	//	报警使能
#define Analog_Statistic_Enable				(1 << 2)	//	遥测统计使能
#define Analog_Absolute_Enable				(1 << 3)	//	绝对值使能
#define Analog_Calculate_Enable				(1 << 4)	//	计算使能
#define Analog_OtherSide_Enable				(1 << 5)	//	对端使能
#define Analog_Replace_Enable				(1 << 6)	//	代路使能
#define Analog_RecordChange_Enable			(1 << 7)	//	变化次数记录使能
#define Analog_Curve_Enable					(1 << 8)	//	曲线使能
#define Analog_Reverse_Enable				(1 << 9)	//	取反使能
#define Analog_ManSet						(1 << 10)	//	人工置数
#define Analog_Accident_Flag    			(1 << 11)	//	事故定义点
#define Analog_Integral_Enable    			(1 << 12)	//	积分电度使能
#define Analog_Dispatch_DisEnable    		(1 << 13)	//	调度禁止报警
#define	Analog_Repair_Flag					(1 << 14)	//  检修状态标志
#define Analog_RUNTIME_Enable				(1 << 15)	//  计时报警使能
#define Analog_RUNNUM_Enable				(1 << 16)	//  计次报警使能
#define Analog_OldData_Enable				(1 << 17)	//  老数据报警使能
#define Analog_DelayAlarm_Enable			(1 << 18)	//  延时报警使能

#define Analog_DelayAlarm_Run				(1 << 25)	//  延时报警启动
/////////////////////////////////////////////////////////////////////////////
// 遥测量动态标志定义(2003-04-02):
#define Analog_Alarm_Flag					1			//	报警标志
#define Analog_OverHigh_Flag				(1 << 1)	//	越上限标志
#define Analog_BelowLow_Flag				(1 << 2)	//	越下限标志
#define Analog_OverHighHigh_Flag			(1 << 3)	//	越上上限标志
#define Analog_BelowLowLow_Flag				(1 << 4)	//	越下下限标志
#define Analog_DeadData_Flag    			(1 << 5)	//	死数据标志
#define Analog_OldData_Flag    				(1 << 6)	//	老数据标志
/////////////////////////////////////////////////////////////////////////////
// 遥信量标志位定义(2003-04-02):
#define	Digit_Scan_Enable					1			//	扫描使能
#define	Digit_Alarm_Enable					(1 << 1)	//	报警使能
#define	Digit_Control_Enable				(1 << 2)	//	遥控使能
#define	Digit_Reverse_Enable				(1 << 3)	//	取反使能
#define	Digit_Calculate_Enable				(1 << 4)	//	计算使能
#define	Digit_OpenSum_Enable				(1 << 5)	//  开次数统计使能
#define	Digit_CloseSum_Enable				(1 << 6)	//  合次数统计使能
#define	Digit_LastCheck_Enable				(1 << 7)	//  去抖使能
#define	Digit_AccidentJudge_Enable			(1 << 8)	//  事故判断使能
#define	Digit_Repair_Flag					(1 << 9)	//  检修状态标志
#define Digit_ManSet_Flag					(1 << 10)	//  人工置数状态标志
#define Digit_Statistic_Enable				(1 << 11)	//  运行时间统计使能
#define Digit_Dispatch_DisEnable    		(1 << 13)	//	调度禁止报警
#define Digit_RUNTIME_Enable				(1 << 12)	//  计时报警使能
#define Digit_RUNNUM_Enable					(1 << 14)	//  计次报警使能
#define Digit_0_Enable						(1 << 15)	//  变0动作标志
#define Digit_DelayAlarm_Enable				(1 << 16)	//  延时报警使能

#define Digit_WINK_Flag						(1 << 25)	//  闪烁标志
#define Digit_DelayAlarm_Run				(1 << 26)	//  延时报警启动

/////////////////////////////////////////////////////////////////////////////
// 脉冲量标志位定义(2003-04-02):
#define	Pulse_Scan_Enable					1			//	扫描使能
#define	Pulse_Calculate_Enable				(1 << 2)	//	计算使能
#define	Pulse_Number_Enable					(1 << 3)	//	一次采集脉冲数使能
#define	Pulse_Energy_Enable					(1 << 4)	//	一次采集电量使能
#define	Pulse_MeterNumber_Enable			(1 << 5)	//	表记脉冲数使能
#define	Pulse_MeterEnergy_Enable			(1 << 6)	//	表记电量使能
#define	Pulse_PVF_Enable					(1 << 7)	//	日峰谷平电量统计使能
#define Pulse_ManSet						(1 << 8)	//	人工置数
#define Pulse_OtherSide_Enable				(1 << 9)	//	对端处理使能
#define Pulse_Replace_Enable				(1 << 10)	//	代路处理使能
#define Pulse_Huanbiao_Enable				(1 << 11)	//	换表使能
/////////////////////////////////////////////////////////////////////////////
// 遥控标志位定义(2005-01-16):
#define Relay_RCing_Flag					(1<<0)	//遥控进行中
#define Relay_Disable_Flag					(1<<1)	//禁止遥控
#define Relay_Maintenance_Flag				(1<<2)	//遥控授权维护
#define Relay_NoSoftJudge_Flag				(1<<3)	//解除软件闭锁
#define Relay_TAPError_Flag					(1<<4)	//滑档闭锁 

#define Relay_DoublePos_Flag				(1<<10)	//双位遥控

// 遥控类型定义
#define Relay_iType_CB						1		//开关刀闸
#define Relay_iType_TRFMUP					2		//变压器升
#define Relay_iType_TRFMDOWN				7		//变压器降
#define Relay_iType_TRFMSTOP				8		//变压器停
#define Relay_iType_YABAN					3		//压板
#define Relay_iType_RESET					4		//复归
#define Relay_iType_DZ						5		//定值
#define Relay_iType_DZZONE					6		//定值区
/////////////////////////////////////////////////////////////////////////////
// 旁路带路标志位定义
#define PLD_Use_Flag						(1<<0)	//使用
#define PLD_Topo_Flag						(1<<1)	//拓朴维护 
#define PLD_Analog_Flag						(1<<2)	//遥测类型
#define PLD_Pulse_Flag						(1<<3)	//电度类型
/////////////////////////////////////////////////////////////////////////////
// 公式表标志位定义(2003-04-02):
#define	Formula_Disable						1			//	公式禁用
#define	Formula_Enable						2			//	公式使用

#define	PDR_q_Disable						1			//	PDR禁用
#define	PDR_q_HistorySave					4			//	历史保存处理中
#define	PDR_q_Value							8			//  PDR计算值	

// 全局表标志
#define	Global_q_RepairAlarmDisable			1			//	检修中不报警
#define	Global_q_PLDTopoEnable				(1<<1)		//	自动旁路代

//一般情况开关事故跳闸时才闪烁,为了满足有的用户对开关变位就闪烁的要求特意加此标志
#define	Global_q_BreakChangeWink			(1<<2)		//	开关变位就闪烁


/////////////////////////////////////////////////////////////////////////////
#define MAX_ANALOG_PACKET_NUM		32
#define MAX_DIGIT_PACKET_NUM		32
#define MAX_PULSE_PACKET_NUM		32
/////////////////////////////////////////////////////////////////////////////
typedef struct _tagAnalogPacket
{
	INT			iAnalogIndex[MAX_ANALOG_PACKET_NUM];	// 对应于遥测大表中的索引
	INT			iAnalogID[MAX_ANALOG_PACKET_NUM];		// 对应于遥测大表中的ID32
	FLOAT		fAnalogRaw[MAX_ANALOG_PACKET_NUM];		// 遥测原始值 //原来是int类型的现更改为float类型
	INT			iAnalogNum;								// 要更新的遥测数
} AnalogPacket;
/////////////////////////////////////////////////////////////////////////////
typedef struct _tagDigitPacket
{
	INT			iDigitIndex[MAX_DIGIT_PACKET_NUM];		// 对应于遥信大表中的索引
	INT			iDigitID[MAX_DIGIT_PACKET_NUM];			// 对应于遥信大表中的ID32
	INT			iDigitRaw[MAX_DIGIT_PACKET_NUM];		// 遥信原始值
	INT			iDigitNum;								// 要更新的遥信数
} DigitPacket;
/////////////////////////////////////////////////////////////////////////////
typedef struct _tagPulsePacket
{
	INT			iPulseIndex[MAX_PULSE_PACKET_NUM];		// 对应于电度大表中的索引
	INT			iPulseID[MAX_PULSE_PACKET_NUM];			// 对应于电度大表中的ID32
	INT			iPulseRaw[MAX_PULSE_PACKET_NUM];		// 电度原始值[直接地址复制]
	INT			iPulseNum;								// 要更新的电度数
} PulsePacket;
/////////////////////////////////////////////////////////////////////////////
#define	Channel_NoWork						1			//	没有工作
#define	Channel_MainChannelWork				(1 << 1)	//	主通道工作
#define	Channel_BackChannelWork				(1 << 2)	//	备通道工作
#define	Channel_MainFMWork					(1 << 3)	//	主前置工作
#define	Channel_BackFMWork					(1 << 4)	//	备前置工作
/////////////////////////////////////////////////////////////////////////////
// 事故判据类型定义(2003-04-02):
#define FAULT_TYPE_NULL						0	//不做事故跳闸判断
#define FAULT_TYPE_TOTAL					1	//只判站内事故总信号
#define FAULT_TYPE_PROTECTION				2	//只判关联保护信号
#define FAULT_TYPE_TOTAL_AND_POROTECTION	3	//判事故总信号与保护信号
#define FAULT_TYPE_TOTAL_OR_PROTECTION		4	//判事故总信号或保护信号
/////////////////////////////////////////////////////////////////////////////
// 转发点类型定义(2003-04-02):
#define TRANSMIT_TYPE_ANALOG				0	//遥测量
#define TRANSMIT_TYPE_DIGIT					1	//遥信量
#define TRANSMIT_TYPE_PULSE					2	//电度量

#define LabelType_q_Repair					1	//检修牌
#define LabelType_q_JD						2	//接地牌
#define LabelType_q_Tip						4	//警示牌
#define LabelType_q_DisenableRC				8	//禁止遥控

#define RCConfig_q_bWatch					1	//启动双席遥控
#define RCConfig_q_BHDZ						2	//保护动作闭锁遥控
#define RCConfig_q_SAME						4	//目标位和当前值一致也能遥控
#define RCConfig_q_FGDisplay				8	//复归显示遥控界面
#define RCConfig_q_TRFMDisplay				16	//变压器遥调显示遥控界面
#define RCConfig_q_10kVLoop					32	//10kV不能合环
#define RCConfig_q_35kVLoop					64	//35kV不能合环
#define RCConfig_q_110kVLoop				128	//110kV不能合环
#define RCConfig_q_220kVLoop				256	//220kV不能合环
#define RCConfig_q_330kVLoop				512	//330kV不能合环
#define RCConfig_q_500kVLoop				1024//500kV不能合环
#define RCConfig_q_NoAutoClose			2048//成功遥控后界面不自动退出
#define RCConfig_q_FGNoFault			4096//没有故障也能复归装置
#define RCConfig_q_NoSoftJudge			8192//解除软件闭锁
#define RCConfig_q_DispatchNO			16384//校验调度编号
/////////////////////////////////////////////////////////////////////////////
#define eAnalogUnitTypeNull 0
#define eAnalogUnitTypekW 1 
#define eAnalogUnitTypeMW 2
#define eAnalogUnitTypekVar 3
#define eAnalogUnitTypeMVar 4
#define eAnalogUnitTypeA   5
#define eAnalogUnitTypekV  6
#define eAnalogUnitTypeHz  7
#define eAnalogUnitTypekVA  11  
#define eAnalogUnitTypeV   12
#define eAnalogUnitTypeW   13
#define eAnalogUnitTypekWh  14  //2016-03-04 
#define eAnalogUnitTypeMWh  15  
#define eAnalogUnitTypekVarh   16
#define eAnalogUnitTypeMVarh   17
#endif // __AFX_SCADA_DEFINE_H__

