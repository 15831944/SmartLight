#pragma once
#if !defined(__Q_103STANDARD_DEFINE_H__INCLUDED__)
#define __Q_103STANDARD_DEFINE_H__INCLUDED__
/** 
 * @interface 
 * @brief IEC-60870-103 protocol define
 * @author 谭井胜 TanJingSheng
 * @date 2012-2-07
 * @see 
 * @bug No know bugs so far.
 */

#pragma warning(push)
#pragma warning(disable: 4251) 

/** @brief Add include file. */
#include "Q_frameInclude.h"
#include "Q_IEC103Types.h"
#include <list>
#include <time.h>
using namespace std;


#define Q_103SUC 0
#define Q_103FAIL 1

// define commands
// const BYTE  CMD_STARTDT		 =0x04;
// const BYTE  CMD_STARTDT_CON  =0x08;          //启动数据传输确认
// const BYTE  CMD_STOPDT		 =0x10;
// const BYTE  CMD_STOPDT_CON   =0x20;          //停止数据传输确认
// const BYTE  CMD_TESTFR		 =0x40;
// const BYTE  CMD_TESTFR_CON	 =0x80;           //测试确认
// 
// const BYTE  CMD_ACT             =0x06;           //激活
// const BYTE  CMD_ACT_CON         =0x07;           //激活确认
// const BYTE  CMD_DEACT           =0x08;           //仃止激活
// const BYTE  CMD_DEACT_CON       =0x09;         //仃止激活确认
// const BYTE  CMD_DEACT_TERM      =0x0a;       //激活终止  
// const BYTE  CMD_COMMON_ADDR_TMP =0x01; 
// const BYTE  CMD_KNOWN_INFO_ADDR =0x6f;//未知的信息对象地址6f还是2f?
 const UINT MIN_103_FRAME_NUM=6;//最小帧大小
 const UINT MAX_103_REMOTE_NUM=64;//最大的对端个数  原来值为10  
  const UINT MAX_103_REMOTE_NUM_BASE=4;
const UINT MAX_103_PACKET_NUM=255;//最大的103报文大小
#define  MAX_RECIVE_PACKET_NUM 10240


typedef enum Q_IEC103_ErrorCode
{

	e103ErrNo_Suc=0,//成功
	e103ErrNo_WriteSend=1, //写输出缓冲区错误
	e103ErrNo_Link=2,//链路断开或错误
	e103ErrNo_P_UnExist=3, //遥控地址对应的点表不存在
	e103ErrNo_4=4,
	e103ErrNo_FrameNum=5, //帧长不对或者大于最大值或者小于最小值6
    e103ErrNo_RxOverTx=6, //发送序列号应该小于接收序列号
	e103ErrNo_FrameSeq    =7,  //帧序序有问题
	e103ErrNo_CmdListNull =8, //命令列表没有初使化
	e103ErrNo_CmdNoSel    =9,   //可能没有选择设备
	e103Errno_CmdSend     =10,    //发送命令
	e103Errno_UptDb       =11,//更新实时库
	e103ErrNo_MisAsdu     =12, //公共数据服务单元编号不对
	e103ErrNo_FrameVerify    =13,  //帧检验失败
	e103ErrNo_BeyondK    =14  //帧检验失败

};

typedef enum Q_IEC103CmdType
{
	e103Cmd_Null   =0, //空
	e103_CALL_ALL_DATA  =1, //总召唤
	e103_CALL_ONE_DATA     =2, //一级数据		
	e103_CALL_TWO_DATA     =3, //二级数据
	e103_INIT_CHANNEL      =4, //初始化
	e103CmdTypeC_CS     =5, //时钟同步
    e103CmdTypeC_EI     =6, //
	e103CmdTypeC_RC     =7, //遥控
	e103CmdTypeTestFR   =8,   //测试链路
	e103CmdTypePlanCurve   =9,    //计划曲线
	e103CmdTypeYT   =10,   //遥调
	e103CmdTypeAnalogchange=11,//变化遥测上传
	e103CmdTypeYK   =12   //遥控
};
typedef enum Q_IEC103CmdState
{
	e103CmdStateNull         =0, //准备
	e103CmdStateEnd          =1,//命令结束
	e103CmdStateYTSelect     =2,//遥调选择
    e103CmdStateYTSelectCon    =3, //遥调选择发送
	e103CmdStateYTExec       =4, //执行
	e103CmdStateYTExecCon    =5,//执行完成确认
	e103CmdStateYTCancel     =6,//撤消
	e103CmdStateYTCancelAct  =7, //撤消确认
	e103CmdStateIntRequest   =8,//召唤请求
	e103CmdStateIntRequestAct=9,//召唤响应
	e103CmdStateIntSendData  =10,//召唤发送数据中
	e103CmdStateIntEnd       =11, //召唤结束 
	e103CmdStateIntSendDataFin =12, //总召发送数据完成
	e103CmdStateSync         =13,//同步命令
	e103CmdStateSyncAct      =14,//同步确认
	e103CmdStateYKSelect     =15,//遥控选择
	e103CmdStateYKSelectCon  =16,//遥控选择发送
	e103CmdStateYKExec       =17,//执行
	e103CmdStateYKExecCon    =18,//执行已发送
	e103CmdStateYKExecAct    =19,//执行完成确认
	e103CmdStateYKCancel     =20,//撤消
	e103CmdStateYKCancelAct  =21, //撤消确认
};
typedef enum Q_IEC103_ST_Type
{
	e103Station_Unknown =0,   //未知站别
	e103Station_Master  =1,  //主站
	e103Station_Slave   =2    //从站
}Q_IEC103_ST_Type;


//帧类型
typedef enum Q_IEC103FrameType
{
	e103FrameType_Null=0,//无效
	e103FrameType_I=1,
	e103FrameType_U=2,
	e103FrameType_S=3
};
//校验返回值
class   Q_IEC103vfyRet
{
public:
	 Q_IEC103FrameType m_eFrameType;
	bool m_bSuc;//是否成功
	WORD m_nErrorCode;//错误码。 
	Q_IEC103vfyRet(void)
	{
      m_eFrameType=e103FrameType_Null;
	  m_bSuc=true;
      m_nErrorCode=0;
	}
};
typedef enum Q_IEC103_VarType
{
	e103VarTypeNull=0,//未知类型
	e103VarTypeYc=1,//遥测变化
	e103VarTypeYx=2,//遥信变化
	e103VarTypeEx=3 //其它的值变化
}Q_IEC103_VarType;
//用于触发外部执行命令用
typedef struct Q_IEC103_ExecCmd
{
	int funcode;// 功能码
	Q_IEC103_VarType varType;//变化的数据类型
	int m_vindex;//变化的索引,默认是从零开始的
	Q_RECORDDATATAG m_tag;//更新数据
	bool m_bBlock;
	Q_IEC103_ExecCmd()
	{
		funcode=0;
		varType=e103VarTypeNull;
		m_vindex=0;
		m_bBlock=false;
	}
}Q_IEC103_ExecCmd;


#endif