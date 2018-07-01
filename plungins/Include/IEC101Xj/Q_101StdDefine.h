
#if !defined(__Q_104STANDARD_DEFINE_H__INCLUDED__)
#define __Q_104STANDARD_DEFINE_H__INCLUDED__

#include "Q_frameInclude.h"

#ifdef WINDOWS_ENV
#pragma warning(push)
#pragma warning(disable: 4251) 
#endif

/** @brief Add include file. */
#include "Q_IEC101Types.h"
#include <list>
#include <time.h>
using namespace std;


#define Q_104SUC 0
#define Q_104FAIL 1

// define commands
const BYTE  CMD_STARTDT		 =0x04;           //启动链路
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
	eCmdStateYkConfirm    =15,  //遥控确认
	eCmdStateYkExecCon = 16 //执行已下发

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
typedef Q_Protocol_VarType Q_IEC104_VarType;
typedef Q_Protocol_ExecCmd Q_IEC104_ExecCmd;

//功能码定义:
enum FUNC_CODE_PRM_DEF
{
	FUNC_PRM_RSTLINK = 0,
	FUNC_PRM_RSTPROCESS = 1,
	FUNC_PRM_TESTLINK = 2,
	FUNC_PRM_SENDDATA_WC = 3,
	FUNC_PRM_SENDDATA_NC = 4,
	FUNC_PRM_REQ_ACD = 8,
	FUNC_PRM_REQ_LINKSTAT = 9, //请求远方链路状态
	FUNC_PRM_REQ_CLASS1 = 10,
	FUNC_PRM_REQ_CLASS2 = 11,
};

enum FUNC_CODE_SUB_DEF
{
	FUNC_SUB_CONF = 0,
	FUNC_SUB_CONF_LINKBUSY = 1,
	FUNC_SUB_ACK = 8,
	FUNC_SUB_NACK = 9,
	FUNC_SUB_LINKOK = 11,
	FUNC_SUB_LINK_NOTWORK = 14,
	FUNC_SUB_LINK_NOTFINISH = 15
};
enum FUNC_CODE_CTRLBYTE_DEF
{
	CTRLBYTE_DIR = 0x80,//dir =0 m->s, =1 s->m
	CTRLBYTE_PRM = 0x40,
	CTRLBYTE_FCB_OR_ACD = 0x20,
	//CTRLBYTE_ACD = 0x20
	CTRLBYTE_FCV_OR_DFC = 0x10,
	//CTRLBYTE_DFC = 0x10
	CTRLBYTE_FUNC = 0x0f,
};
//帧类型定义
enum FRM_TYPE_DEF
{
	FRMTYPE_SEND_NC = 1,
	FRMTYPE_SEND_WC = 2,
	FRMTYPE_REQ_RESP = 3,
};
//定义链路状态, PRM: 转发—>接受站; SUB:原站—>转发.
//原方状态:
enum IEC104_LINK_PRM_STATE_DEF
{
	IEC104_LINK_PRM_STATE_NULL = 0,
	IEC104_LINK_PRM_STATE_REQSTATUS = 1,    //请求远方连路状态
	IEC104_LINK_PRM_STATE_SENDRST = 2,      //请求复位链路
	IEC104_LINK_PRM_STATE_LINKOK= 3,	//建立完成了
	IEC104_LINK_PRM_STATE_WAITRSCONF = 4,	//WAIT ACK/NACK FOR RESET
	IEC104_LINK_PRM_STATE_WAITSTATUS = 5,	//WAIT FOR LINKSTATUS

};
//副方状态:
enum IEC104_LINK_SUB_STATE_DEF
{
	IEC104_LINK_SUB_STATE_NULL = 0,
	IEC104_LINK_SUB_STATE_STATUSCONF = 1, //回复请求链路状态
	IEC104_LINK_SUB_STATE_RSTCONF = 2,     //回复了重置
	IEC104_LINK_SUB_STATE_INITFIN = 3      //初使化完成
};
//定义应用层状态
enum IEC104_APP_STATE_DEF
{
	IEC104_APP_STATE_IDLE = 0,
	IEC104_APP_STATE_WAITALLDATACONF = 1,
	IEC104_APP_STATE_WAITTIMECONF = 2,
	IEC104_APP_STATE_WAITALLKWHCONF = 3,
	IEC104_APP_STATE_WAITALLDATA = 4,
	IEC104_APP_STATE_WAITALLKWH = 5,
	IEC104_APP_STATE_WAITGRPDATA = 6,
	IEC104_APP_STATE_WAITGRPKWH = 7,
	IEC104_APP_STATE_WAITYKCONF = 8,
	IEC104_APP_STATE_WAITYKFINISH = 9,
	IEC104_APP_STATE_ACK_CLASS1 = 10,
	IEC104_APP_STATE_ACK_CLASS2 = 11,
	IEC104_APP_STATE_ACK = 12,
};

//应用层数据类型定义
enum APP_DATA_TYPE_DEF
{
	APPTYPE_SP_NT = 1,
	APPTYPE_SP_WT = 2,
	APPTYPE_DP_NT = 3,
	APPTYPE_DP_WT = 4,
	APPTYPE_ME_NT = 9,
	APPTYPE_ME_WT = 10,
	APPTYPE_M_ME_NC = 13, ////浮点数 
	APPTYPE_CO_NT = 15,
	APPTYPE_CO_WT = 16,
	APPTYPE_PS_STS_NT = 20,
	APPTYPE_ME_ND = 21,
	APPTYPE_YK = 46,
	APPTYPE_YT = 47,
	APPTYPE_SETPOINT = 48,
	APPTYPE_CALLDATA = 100,
	APPTYPE_CALLKWH = 101,
	APPTYPE_READDATA = 102,
	APPTYPE_TIMESYNC = 103,
	APPTYPE_TEST = 104,
	APPTYPE_RSTPROC = 105,
};
/////传输原因
enum APP_COT_TYPE_DEF
{
	APP_COT_PER_CYC = 1,
	APP_COT_BACKSCAN = 2,
	APP_COT_SPONT = 3,
	APP_COT_INIT = 4,
	APP_COT_REQ = 5,
	APP_COT_ACT = 6,
	APP_COT_ACT_CON = 7,
	APP_COT_DEACT = 8,
	APP_COT_DEACT_CON = 9,
	APP_COT_ACT_TERM = 10,

	APP_COT_RESP_CALLALL = 20,
	APP_COT_RESP_GRP1 = 21,
	APP_COT_RESP_GRP2 = 22,
	APP_COT_RESP_GRP3 = 23,
	APP_COT_RESP_GRP4 = 24,
	APP_COT_RESP_GRP5 = 25,
	APP_COT_RESP_GRP6 = 26,
	APP_COT_RESP_GRP7 = 27,
	APP_COT_RESP_GRP8 = 28,
	APP_COT_RESP_GRP9 = 29,
	APP_COT_RESP_GRP10 = 30,
	APP_COT_RESP_GRP11 = 31,
	APP_COT_RESP_GRP12 = 32,
	APP_COT_RESP_GRP13 = 33,
	APP_COT_RESP_GRP14 = 34,
	APP_COT_RESP_GRP15 = 35,
	APP_COT_RESP_GRP16 = 36,

	APP_COT_RESP_CO = 37,
	APP_COT_RESP_CO_GRP1 = 38,
	APP_COT_RESP_CO_GRP2 = 39,
	APP_COT_RESP_CO_GRP3 = 40,
	APP_COT_RESP_CO_GRP4 = 41,
};
//响应方式定义
enum ACK_TYPE_DEF
{
	ACK_UNSOLICITED = 0,	//非请求响应
	ACK_REQUESTED = 1	//被请求响应
};
//定义每组数据容量
enum DATA_NUM_PERGROUP_DEF
{
	IEC104_YXNUM_PERGROUP = 127,
	IEC104_YCNUM_PERGROUP = 123,
	IEC104_KWHNUM_PERGROUP = 32,
};
// define timeout 
enum IEC104_TIMEOUT_DEF
{
	IEC104_TIMEOUT = 10,
	IEC104_RESEND_TIMES = 3,
};
enum IEC104_FUNC_FRAME_ADDR
{
	IEC104_YC_FORWARD_BEGIN = 0x701,
	//IEC104_YC_FORWARD_END = 0x7f,
	IEC104_YM_FORWARD_BEGIN = 0xc01,
	//IEC104_YM_FORWARD_END = 0xDF,
	IEC104_YX_FORWARD_BEGIN = 0x001,
	//IEC104_YX_FORWARD_END = 0xFF,
};
enum IEC104_FORWARD_STATUS
{
	IEC104_RTU_FORWARD_READY = 0,
	IEC104_RTU_FORWARD_BEINGING,
	IEC104_RTU_FORWARD_END,
};
enum ONE_OF_TYPES_FORWARD_STATE
{
	ONE_OF_TYPES_FORWARD_CONTINUE,
	ONE_OF_TYPES_FORWARD_END,
};

enum DATA_CONFIG_STATE
{
	DATA_START_CONFIG = 0,
	DATA_PROCESSING,
	DATA_END_CONFIG,
};
enum CMD_TYPE
{
	CMD_NONE = 0,
	SEND_LINK_STATE,
	SEND_LINK_RESET,
	PROC_CLASS1_CMD,
	PROC_CLASS2_CMD,
	PROC_ALLDATA_CMD,
	SYNC_TIMEFLAG_CMD,
	PROC_AllKWH_CMD,
	PROC_YK_CMD,
	YK_OK_CONFIG_CMD
};
enum DATA_SEND_NUM_PER_FRAME
{
	MAX_YCNUM_PERFRAME_SEQ = 10,//50,
	MAX_YXNUM_PERFRAME_SEQ = 20,//100,
	MAX_YMNUM_PERFRAME_SEQ = 8,//25,
	MAX_YCNUM_PERFRAME_UNSEQ = 5,//26,
	MAX_YXNUM_PERFRAME_UNSEQ = 10,// 35,
	MAX_YMNUM_PERFRAME_UNSEQ = 4,//15,
};
#endif