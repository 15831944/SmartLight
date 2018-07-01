/////////////////////////////////////////////////////////////////////////////
// MsgDefine.h : head file of rtdb msg type define
// 说明: 1到10为系统保留区间,其它应用禁止使用
// 注意: message.def中定义的消息从200开始,为了配合message.def,这里定义的消息不要大于200。
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_MESSAGE_DEFINE_H__2005_03_10__
#define __AFX_MESSAGE_DEFINE_H__2005_03_10__

#define SYSTEM_MAX_MESSAGE			200

/////////////////////////////////////////////////////////////////////////////
// 系统保留定义,这里可能仅仅是副本,真实定义可能写死到程序中了
#define mes_Map_Change				1  //共享内存已经修改,Java的RtServer应响应(消息体:nType,nDbID,nTabID,nRecordIndex)

/////////////////////////////////////////////////////////////////////////////
#define mes_User_Login				11  //用户登录
#define mes_User_Logout				12  //用户注销

#define mes_Start_Application		21  //远程启动应用程序
#define mes_End_Application			22  //远程关闭应用程序,各应用程序应该响应并立即关闭
#define mes_Sys_Exit				23  //远程关闭本地系统,MCI程序响应后关闭本地所有应用
#define mes_Local_Exit				24  //关闭本机系统,由MCI发出,该消息不广播

#define mes_Alarm_Acknowleged		31
#define mes_Alarm_DeleteItem		32
#define mes_Alarm_ClearAlarm        33

#define mes_Graph_YX_Change			41
#define mes_Change_RunConfig		42
#define mes_Change_PowerFlag		43
#define mes_ResetAllProtect			44//保护总复归
#define mes_TestBell				45//电铃测试
#define mes_TestFlute				46//电笛测试

#define mes_RCSelect				51
#define mes_RCExecute				52
#define mes_RCSelectOk				53
#define mes_RCSelectFail			54
#define mes_RCExecuteOk				55
#define mes_RCExecuteFail			56
#define mes_RCCancelSelect          57
#define mes_RCCancelSelectOk        58
#define mes_RCCancelSelectFail      59
#define mes_RC_ASK_FIRST			60
#define mes_RC_FIRST_ACK			61
#define mes_RCOperatorName			62
#define mes_RCMonitorName			63
#define mes_RCPasswordOK			64

#define mes_AddSysEle_Process		71
#define mes_DelSysEle_Process		72
#define mes_AddSysEle_Event			73

#define mes_Transmit_Change			81

#define mes_PAS_Net2Pas 			100	//同步Net库消息(校验Net库,copy到Pas库)
#define mes_PAS_Part_Net2Pas 		101	//仅仅同步Net库参数数据消息(校验并同步部分参数到Pas库)
#define mes_PAS_StartCal			102 //启周期计算消息
#define mes_PAS_StopCal				103 //停周期计算消息
#define mes_PAS_CalSE			 	104	//触发se计算
#define mes_PAS_CalPF			 	105	//触发PF计算
#define mes_PAS_CalCA			 	106	//触发CA计算
#define mes_PAS_CalSCC			 	107	//触发SCC计算

#define mes_Fep_Reload              111
#define mes_ScadaServer_YXChange    120    //遥信变位

#define mes_ToFMRawPacket			150		//应用程序已组织好的下发报文,前置将其下发即可
#define mes_FromFMRawPacket			151		//前置收到不直接处理的报文,转发应用程序,由相关程序自己处理
#define mes_MsgPrint                152 //打印通讯规约类的消息使用
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_MESSAGE_DEFINE_H__2005_03_10__
