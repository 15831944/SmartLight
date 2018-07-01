/////////////////////////////////////////////////////////////////////////////
// AlarmDefine.h : head file of rtdb alarm type define
// 说明: 该类主要是为ScadaAPI准备,由于RtdbDll中需要网络、节点故障报警，将该报警相关定义放到RtdbDll中
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_ALARM_DEFINE_H__2005_03_10__
#define __AFX_ALARM_DEFINE_H__2005_03_10__
/////////////////////////////////////////////////////////////////////////////
typedef struct _tagAlarmEvent
{
	INT			lDegree;		// 报警等级
	INT			lMainType;		// 报警主类型
	INT			lSubType;		// 报警子类型
	SYSTEMTIME	tEventTime;		// 报警时间
	INT			StationID32;	// 厂站单元ID
	INT			PointID32;		// 测量点的ID
	CHAR		strDisplay[200];// 报警显示文字
} AlarmEvent;//告警事件
/////////////////////////////////////////////////////////////////////////////
//** 警等级定义(2003-04-02):
#define AC_CLASS_SHOW					(1 << 0)//显示报警
#define AC_CLASS_PRINT					(1 << 1)//打印报警
#define AC_CLASS_SAVE					(1 << 2)//存储报警
#define AC_CLASS_VOICE					(1 << 3)//语音报警
#define AC_CLASS_BELL                   (1 << 4)//电铃报警
#define AC_CLASS_FLUTE                  (1 << 5)//电笛报警
#define AC_CLASS_MMI                    (1 << 6)//推画面
#define AC_CLASS_FLASH                  (1 << 7)//闪烁
#define AC_CLASS_AUTO                   (1 << 8)//自动确认
#define AC_CLASS_DELETE                 (1 << 9)//确认后不显示
#define AC_CLASS_MESSAGE                (1 << 10)//发送短信
/////////////////////////////////////////////////////////////////////////////
//** 报警主要类型定义(2003-04-02):
#define AT_MAINTYPE_ANALOG				1		//遥测
#define AT_MAINTYPE_DIGIT				(1 << 1)//遥信
#define AT_MAINTYPE_YK					(1 << 2)//遥控
#define AT_MAINTYPE_YT					(1 << 3)//遥调
#define AT_MAINTYPE_SOE					(1 << 4)//SOE
#define AT_MAINTYPE_RELAY				(1 << 5)//保护
#define AT_MAINTYPE_Foretel				(1 << 6)//告警
#define AT_MAINTYPE_COMMUNITION			(1 << 7)//通讯
#define AT_MAINTYPE_OTHER				(1 << 8)//其它
#define AT_MAINTYPE_FAULT				(1 << 11)//故障
#define AT_MAINTYPE_CBSTATE				(1 << 12)//开关无故变位
//#define AT_MAINTYPE_SWITCHACTION		(1 << 9)//开关动作
/////////////////////////////////////////////////////////////////////////////
// 遥测报警次类型(2003-04-02):
#define AT_ANALOG_OVER_HIGH_LIMIT		11	//遥测越上限
#define AT_ANALOG_OVER_LOW_LIMIT		12	//遥测越下限
#define AT_ANALOG_OVER_HIGH2_LIMIT		13	//遥测越上上限
#define AT_ANALOG_OVER_LOW2_LIMIT		14	//遥测越下下限
#define AT_ANALOG_ALARM_RESTORE			19	//遥测报警恢复
/////////////////////////////////////////////////////////////////////////////
// 遥信报警次类型(2003-04-02):
#define AT_DIGIT_OPEN					41	//遥信开关断开
#define AT_DIGIT_CLOSE					42	//遥信开关闭合
#define AT_DIGIT_OPEN_FAULT				43	//遥信事故跳闸
/////////////////////////////////////////////////////////////////////////////
// 遥控报警次类型(2003-04-02):
#define AT_YK_SELECT					61	//遥控选择
#define AT_YK_SELECT_OK					62	//遥控选择成功
#define AT_YK_SELECT_FAIL				63	//遥控选择失败
#define AT_YK_SELECT_CANCEL				64	//遥控选择撤销
#define AT_YK_CANCEL_OK					65	//遥控撤销成功
#define AT_YK_CANCEL_FAIL				66	//遥控撤销失败
#define AT_YK_EXECUTE					67	//遥控执行
#define AT_YK_EXECUTE_OK				68	//遥控执行成功
#define AT_YK_EXECUTE_FAIL				69	//遥控执行失败
/////////////////////////////////////////////////////////////////////////////
// SOE报警次类型(2003-04-02):
#define AT_SOE_OPEN						81	//SOE事件开关断开
#define AT_SOE_CLOSE					82	//SOE事件开关闭合
/////////////////////////////////////////////////////////////////////////////
//** 通讯报警次类型(2003-04-02):
#define AT_COMM_RESUME					90	//通讯恢复
#define AT_COMM_FAIL					91	//通讯失败
#define AT_COMM_TCP_FAIL				92	//TCP通讯失败
#define AT_COMM_UDP_FAIL				93	//UDP通讯失败
#define AT_COMM_COM_INITIALIZE_FAIL		94	//串口初始化失败
#define AT_COMM_COM_OPERAT_FAIL			95	//串口操作失败
#define AT_COMM_NET_1_FAIL				96	//NET_1网络故障
#define AT_COMM_NET_2_FAIL				97	//NET_2网络故障
/////////////////////////////////////////////////////////////////////////////
//** 保护报警次类型(2003-04-02):
#define AT_RELAY_BREAK_OPEN				101	//保护跳闸
#define AT_RELAY_BREAK_CLOSE			102	//保护合闸
#define AT_RELAY_EVENT					103	//保护事件
#define AT_RELAY_ALARM					104	//保护报警
#define AT_RELAY_GET_SET				105	//得到保护定值
#define AT_RELAY_GET_SET_OK				106	//得到保护定值
#define AT_RELAY_GET_SET_FAIL			107	//得到保护定值
#define AT_RELAY_SET_SET				108	//修改保护定值
#define AT_RELAY_SET_SET_OK				109	//修改保护定值
#define AT_RELAY_SET_SET_FAIL			110	//修改保护定值
#define AT_RELAY_YABAN_OPEN				111	//保护压板切
#define AT_RELAY_YABAN_CLOSE			112	//保护压板投
#define AT_RELAY_START				113	//保护启动

#endif // __AFX_ALARM_DEFINE_H__2005_03_10__
