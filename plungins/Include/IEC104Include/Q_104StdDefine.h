
#if !defined(__Q_104STANDARD_DEFINE_H__INCLUDED__)
#define __Q_104STANDARD_DEFINE_H__INCLUDED__
/** 
 * @interface 
 * @brief IEC-60870-104 protocol define
 * @author 谭井胜 TanJingSheng
 * @date 2012-2-07
 * @see 
 * @bug No know bugs so far.
 */
#include "Q_frameInclude.h"

#ifdef WINDOWS_ENV
#pragma warning(push)
#pragma warning(disable: 4251) 
#endif

/** @brief Add include file. */
#include "Q_IEC104Types.h"
#include <list>
#include <time.h>
using namespace std;


#define Q_104SUC 0
#define Q_104FAIL 1

// define commands
const BYTE  CMD_STARTDT		 =0x04;
const BYTE  CMD_STARTDT_CON  =0x08;          //启动数据传输确认
const BYTE  CMD_STOPDT		 =0x10;
const BYTE  CMD_STOPDT_CON   =0x20;          //停止数据传输确认
const BYTE  CMD_TESTFR		 =0x40;
const BYTE  CMD_TESTFR_CON	 =0x80;           //测试确认

const BYTE  CMD_ACT             =0x06;           //激活
const BYTE  CMD_ACT_CON         =0x07;           //激活确认
const BYTE  CMD_DEACT           =0x08;           //仃止激活
const BYTE  CMD_DEACT_CON       =0x09;         //仃止激活确认
const BYTE  CMD_DEACT_TERM      =0x0a;       //激活终止  
const BYTE  CMD_COMMON_ADDR_TMP =0x01; 
const BYTE  CMD_KNOWN_INFO_ADDR =0x6f;//未知的信息对象地址6f还是2f?
const UINT MIN_FRAME_NUM=6;//最小帧大小
//const UINT MAX_REMOTE_NUM=8;//最大的对端个数 //原来是10个
const UINT MAX_REMOTE_NUM=32;//最大的对端个数 //原来是8个

const UINT MAX_104_PACKET_NUM=255;//最大的104报文大小


typedef enum _Q_IEC104_ErrorCode
{

	e104ErrNo_Suc=0,//成功
	e104ErrNo_WriteSend=1, //写输出缓冲区错误
	e104ErrNo_Link=2,//链路断开或错误
	e104ErrNo_P_UnExist=3, //遥控地址对应的点表不存在
	e104ErrNo_4=4,
	e104ErrNo_FrameNum=5, //帧长不对或者大于最大值或者小于最小值6
    e104ErrNo_RxOverTx=6, //发送序列号应该小于接收序列号
	e104ErrNo_FrameSeq    =7,  //帧序序有问题
	e104ErrNo_CmdListNull =8, //命令列表没有初使化
	e104ErrNo_CmdNoSel    =9,   //可能没有选择设备
	e104Errno_CmdSend     =10,    //发送命令
	e104Errno_UptDb       =11,//更新实时库
	e104ErrNo_MisAsdu     =12, //公共数据服务单元编号不对
	e104ErrNo_FrameVerify    =13,  //帧检验失败
	e104ErrNo_BeyondK    =14  //帧检验失败

}Q_IEC104_ErrorCode;

typedef enum _Q_IEC104CmdType
{
	eCmdTypeC_Null   =0, //空
	eCmdTypeC_IC     =1, //召唤
	eCmdTypeC_SC     =2, //单点遥控		
	eCmdTypeC_DC     =3, //双点遥控
	eCmdTypeC_SE     =4, // 设定命令
	eCmdTypeC_CS     =5, //时钟同步
    eCmdTypeC_EI     =6, //初始化结束
	eCmdTypeC_RC     =7, //升降命令
	eCmdTypeTestFR   =8,   //测试链路
	eCmdTypePlanCurve   =9,    //计划曲线
	eCmdTypeParameter=10,   //参数设定
	eCmdTypeAnalogchange=11 //变化遥测上传

}Q_IEC104CmdType;
typedef enum _Q_IEC104CmdState
{
	eCmdStateNull         =0, //准备
	eCmdStateEnd          =1,//命令结束
	eCmdStateYkSelect     =2,//遥控选择
    eCmdStateYkSelectCon    =3, //遥控选择发送
	eCmdStateYkExec       =4, //执行
	eCmdStateYkExecAct    =5,//执行完成确认
	eCmdStateYkCancel     =6,//撤消
	eCmdStateYkCancelAct  =7, //撤消确认
	eCmdStateIntRequest   =8,//召唤请求
	eCmdStateIntRequestAct=9,//召唤响应
	eCmdStateIntSendData  =10,//召唤发送数据中
	eCmdStateIntEnd       =11, //召唤结束 
	eCmdStateIntSendDataFin =12, //总召发送数据完成
	eCmdStateSync         =13,//同步命令
	eCmdStateSyncAct      =14, //同步确认
	eCmdStateYkConfirm    =15  //遥控确认

}Q_IEC104CmdState;
typedef enum _Q_IEC104_ST_Type
{
	eStation_Unknown =0,   //未知站别
	eStation_Master  =1,  //主站
	eStation_Slave   =2    //从站
}Q_IEC104_ST_Type;


//帧类型
typedef enum _Q_IEC104FrameType
{
	eFrameType_Null=0,//无效
	eFrameType_I=1,
	eFrameType_U=2,
	eFrameType_S=3
}Q_IEC104FrameType;
//校验返回值

class   Q_IEC104vfyRet
{
public:
	 Q_IEC104FrameType m_eFrameType;
	bool m_bSuc;//是否成功
	WORD m_nErrorCode;//错误码。 
	Q_IEC104vfyRet(void)
	{
      m_eFrameType=eFrameType_Null;
	  m_bSuc=true;
      m_nErrorCode=0;
	}
};
//del ty tjs 2015-04-15 ,得去掉m_Block;
//typedef enum Q_IEC104_VarType
//{
//	eVarTypeNull=0,//未知类型
//	eVarTypeYc=1,//遥测变化
//	eVarTypeYx=2,//遥信变化
//	eVarTypeEx=3 //其它的值变化
//}Q_IEC104_VarType;
////用于触发外部执行命令用
//typedef struct Q_IEC104_ExecCmd
//{
//	int funcode;// 功能码
//	Q_IEC104_VarType varType;//变化的数据类型
//	int m_vindex;//变化的索引,默认是从零开始的
//	Q_RECORDDATATAG m_tag;//更新数据
//	bool m_bBlock;
//	Q_IEC104_ExecCmd()
//	{
//		funcode=0;
//		varType=eVarTypeNull;
//		m_vindex=0;
//		m_bBlock=false;
//	}
//}Q_IEC104_ExecCmd;

typedef Q_Protocol_VarType Q_IEC104_VarType;
typedef Q_Protocol_ExecCmd Q_IEC104_ExecCmd;


#endif