//*
//*
//* Q_frameDefine.h
//*
//*
#if !defined(__Q_FRAMEDEFINE_H__INCLUDED__)
#define __Q_FRAMEDEFINE_H__INCLUDED__

#include "gSystemDef.h"
#include "Q_CommFrameDef.h"
#include "MsgDefine.h"
/** @brief Define debug flag macro. */
//#ifdef _DEBUG 
//#define   __Q_DEBUG_TASK__						1
//#define  __Q_DEBUG_CHANNEL__						2
//#define  __Q_DEBUG_LINK__						3
//#define  __Q_DEBUG_TCPCLIENTLINK__				4
//#define  __Q_DEBUG_TCPSERVERLINK__				5
//#define __Q_DEBUG_SERIALLINK__					6
//#define  __Q_DEBUG_CDTPROTOCOL__					7
//#define  __Q_DEBUG_104PROTOCOL__					8
//#define __Q_DEBUG_THREAD__						9
//#define  __Q_DEBUG_EXEC_CMD__						12
//#endif



/** @brief Define link log status. */
#define Q_LINK_LOG_NONE						0
#define Q_LINK_LOG_ALL						1
#define Q_LINK_LOG_ERROR					2


/** @brief Define const value in frame. */
#define Q_FRAME_CONST2							2
#define Q_FRAME_CONST8							8
#define Q_FRAME_CONST16							16
#define Q_FRAME_CONST25                         25
#define Q_FRAME_CONST32							32
#define Q_FRAME_CONST64							64
#define Q_FRAME_CONST128						128
#define Q_FRAME_CONST256						256
#define Q_FRAME_CONST512						512
#define Q_FRAME_CONST1024						1024
#define Q_FRAME_CONST2048						2048
#define Q_FRAME_CONST4096						4096
#define Q_FRAME_CONST8192						8192
#define Q_FRAME_CONST20480                      20480
#define Q_FRAME_CONST40960                      40960
#define Q_FRAME_CONST409600                     409600


#define Q_FRAME_DEFAULT_PATHLENGTH				Q_FRAME_CONST256
#define Q_FRAME_DEFAULT_RECEIVEBUFFERSIZE		Q_FRAME_CONST4096 //原来是1024 2015-11-3前是4096
#define Q_FRAME_DEFAULT_SENDBUFFERSIZE			Q_FRAME_CONST4096  //原来是2048  2015-11-3前
#define Q_FRAME_DEFAULT_THREADPERIOD			Q_FRAME_CONST1024


#define Q_MSGTRAN_MAXMSGLEN                       170


enum Q_AVCMSGTYPE
{
	//1、错误信息（闭锁、电压越限、通道断开等），2、提示信息（目标值、当前值），3、风机信息，4、计算开始、结束信息
	eAVCDEBUG_Err=1,
	eAVCDEBUG_Info=2,
	eAVCDEBUG_Wind=3,
	eAVCDEBUG_BeginEnd=4
};


/** @brief Define common status type. */
typedef enum _Q_CommonStatusType
{
	eCommonStatusNull			= 0,
	eCommonStatusNormal			= 1,
	eCommonStatusError			= 2
}Q_CommonStatusType;

/** @brief Define common drive mode. */
typedef enum _Q_CommonDriveMode
{
	eCommonModeNull				= 0,
	eCommonModeSingle			= 1,
	eCommonModeEach				= 2,
	eCommonModeReadWriteExplain = 3,
	eCommonModeReadWrite		= 4
}Q_CommonDriveMode;

/** @brief Define common drive mode. */
typedef enum _Q_CommonBackupMode
{
	eCommonBackupNull			= 0,
	eCommonBackupHot			= 1,
	eCommonBackupCold			= 2
}Q_CommonBackupMode;
typedef enum _Q_LogType
{
	eComLogTypeTotal			= 0,
	eComLogTypeExplain			= 1,
	eComLogTypePrompt			= 2,
	eComLogTypeError           = 3
}Q_LogType;

typedef enum _Q_MsgType
{
	eMsgTypeTx=1,//发送消息
	eMsgTypeRx=2,//接收到的消息
	eMsgTypeCh=3,    //数据解释、规约解析	
	eMsgTypeMsg=4,  //提示信息、信息值
	eMsgTypeError=5, //错误的消息
	eMsgTypeMsgAgc=6,  //信息值并且是AGC信息
	eMsgTypeMsgAvc=7,  //信息值并且是AVC信息
	eMsgTypeControl=8, //控制信息
}Q_MsgType;
enum Q_COMM_PROTOCOL_TYPE
{
  eCOMM_IEC104=1,
  eCOMM_MODBUS=2,
  eCOMM_CDT=3  
};
typedef struct _Q_COM_LOG_INFO
{
	int chid;//通道id
	Q_MsgType type;//消息类型
	char *msg;//信息内容
	int len;//消息长度
	Q_AVCMSGTYPE nAVCType;//AVC类型，1、错误信息（闭锁、电压越限、通道断开等），2、提示信息（目标值、当前值），3、SVG、逆变器分配情况等，4、计算开始、结束信息
}Q_COM_LOG_INFO;


/** @brief Define common config file path. */
#define Q_COMMONCONFIGFILEPATH					"\\config\\"





//----------------------------------------------------------------------------
//*
//*
//* Q_taskBase macro
//*
//*
#define Q_TASK_MAXCHANNELNUMBER					Q_FRAME_CONST2




//----------------------------------------------------------------------------
//*
//*
//* Q_threadBase macro
//*
//*
/** @brief Define thread status. */
typedef enum _Q_ThreadStatusType
{
	eThreadInitialize			= 0,
	eThreadRun					= 1,
	eThreadSuspend				= 2,
	eThreadStop					= 3,
	eThreadQuit					= 4,
	eThreadError				= 5
}Q_ThreadStatusType;


//----------------------------------------------------------------------------
//*
//*
//* Q_taskManager macro
//*
//*
//#define Q_TASKMANAGER_MAXTASKNUMBER				Q_FRAME_CONST32
//#define Q_TASKMANAGER_MAXTHREADNUMBER			Q_FRAME_CONST32
//因为陕西南自103规约需要50个通道，所以开辟128以外扩展用
#define Q_TASKMANAGER_MAXTASKNUMBER				Q_FRAME_CONST128
#define Q_TASKMANAGER_MAXTHREADNUMBER			Q_FRAME_CONST128



//----------------------------------------------------------------------------
//*
//*
//* Q_taskRunningUnit macro
//*
//*
#define Q_TASKUNIT_MAXTASKNUMBER				Q_FRAME_CONST8




//----------------------------------------------------------------------------
//*
//*
//* Q_protocolBase macro
//*
//*
typedef enum _Q_CommandType
{
	eCommandTypeNull			= 0,
	eCommandTypeCycle			= 1,
	eCommandTypeDialog			= 2,
	eCommandTypeOnce			= 3,
	eCommandTypeExtend			= 4 //扩展
}Q_CommandType;

typedef enum _Q_CommandTypeExt
{
	eCommandManual 			    = 0,//手动下发
	eCommandAgc					= 1,//AGC命令
	eCommandAvc					= 2,//AVC命令
}Q_CommandTypeExt;
#define Q_COMMANDLIST_MAX					Q_FRAME_CONST128  //原来是32,因有龙源比较特殊,所以改为128条




//----------------------------------------------------------------------------
//*
//*
//* Q_linkBase macro
//*
//*
#define Q_LINK_MAXNODATATIMES_DEFAULT			Q_FRAME_CONST64



//----------------------------------------------------------------------------
//*
//*
//* Q_serialLink macro
//*
//*

#define Q_SERIAL_SENDBUFFERSIZE					Q_FRAME_CONST2048
#define Q_SERIAL_RECEIVEBUFFERSIZE				Q_FRAME_CONST2048



/** @brief Define serial stop bit type. */
typedef enum _Q_SerialStopBit
{
	eSerialStopBit10			= 0,
	eSerialStopBit15			= 1,
	eSerialStopBit20			= 2
}Q_SerialStopBit;

/** @brief Define serial check mode type. */
typedef enum _Q_SerialModel
{
	eSerialModelNull			= 0,		///* 无校验
	eSerialModelMark			= 1,		///* 标记校验
	eSerialModelOdd				= 2,		///* 奇校验
	eSerialModelSpace			= 3,		///* 奇偶校验
	eSerialModelEven			= 4			///* 偶校验
}Q_SerialModel;


/** @brief Define serial link parameter. */





//----------------------------------------------------------------------------
//*
//*
//* Q_tcpClientLink macro
//*
//*

#define	Q_TCPCLIENT_SELECTTIME					300000	//300ms //一般客户端socket使用
#define	Q_TCPCLIENT_SELECTTIME_SEC				1	//1秒 //一般客户端socket使用
#define	Q_TCPSERVER_CLIENT_SELECTTIME			100000	
#define	Q_UDP_SELECTTIME			500000	//客户端socket超时检查时间
#define Q_TCPCLIENT_RECEIVE_BUFFERSIZE			Q_FRAME_DEFAULT_RECEIVEBUFFERSIZE
#define Q_TCPCLIENT_SEND_BUFFERSIZE				Q_FRAME_DEFAULT_SENDBUFFERSIZE //临时增加tjs
//socket 默认缓冲区大小数据缓冲区大小有区别，此是系统的socket的 2015-11-11 add 
#define Q_SOCKET_RECV_BUFFERSIZE			   Q_FRAME_CONST1024*10
#define Q_SOCKET_SEND_BUFFERSIZE			   Q_FRAME_CONST1024*10





//
//#define Q_TCPSERVER_DEFPRECONNECT				Q_FRAME_CONST8 
//#define Q_TCPSERVER_DEFCLIENTCOUNT				Q_FRAME_CONST8

#define Q_TCPSERVER_DEFPRECONNECT				Q_FRAME_CONST16 //2015-04-02
//#define Q_TCPSERVER_DEFCLIENTCOUNT				Q_FRAME_CONST16
#define Q_TCPSERVER_DEFCLIENTCOUNT				Q_FRAME_CONST64 //为了支持山东张家产风电场一个端口连接12个IEC104主站

//----------------------------------------------------------------------------
//*
//*
//* Q_dataManager macro
//*
//*

#define Q_DATAMANAGER_TAGNAMELENGTH				Q_FRAME_CONST64	


/** @brief Define serial check mode type. */
typedef enum _Q_DataType
{
	eDataTypeNull				= 0,		///* 空类型
	eDataTypeFloat				= 1,		///* 浮点
	eDataTypeInt				= 2,		///* 整形
	eDataTypeBit				= 3			///* 按位
}Q_DataType;

typedef enum _Q_DataQuality
{
	eCreate = 0,//程序刚启动
	eNomal = 1,//正常接收
	eBad = 2//通道异常
}Q_DataQuality;//质量码
typedef enum _Q_UnitType
{
	eUnitTypeNull=0,      //空类型
	eUnitTypekW=1,        //kW
    eUnitTypeMW=2,         //MW
	eUnitTypekVar=3,       //kVar
	eUnitTypeMVar=4         //MVar
}Q_UnitType;
/** @brief Define analogue data struct. */
typedef struct _Q_ANALOGUEDATATAG
{//yc
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];//点名
	char            unit[20];//单位
	BYTE            quality; //质量码0创建1正常2停止
	BYTE			datasource;//数据来源(采集、计算点、虚点)0/1/2
	int             nIndex;//id号
	int             device;//站编号
	int             no; //站内序号
	int				nValue;
	int             lastnValue;//上一个int值
	int             channelid;//通道id
	float           rawvalue;//生数据
	float           factor;//系数
	float			fValue;
	float           lastfValue;//上一个float值
	float           deadzone;//死区
    Q_UnitType      eUnitType;
	Q_DataType		eDataType;//类型
	time_t            timestatmp;//当前时间
	time_t            lastimestamp;//上一个值的时间
	bool             changeflag;
}Q_ANALOGUEDATATAG,*LPQ_ANALOGUEDATATAG;



/** @brief Define digital data struct. */
typedef struct _Q_DIGITALEDATATAG
{//yx
	bool			bValue;
	bool			lastbValue;
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];
	BYTE			datasource;//数据来源(采集、计算点、虚点)0/1/2
	BYTE            quality; //质量码
	int             nIndex;//序号
	int             device;//站编号
	int             no; //站内序号
	int             channelid;//通道id
	time_t            timestatmp;//当前时间
	time_t			lasttimestamp;
	bool             changeflag;
}Q_DIGITALEDATATAG,*LPQ_DIGITALEDATATAG;





/** 遥控 */
typedef struct _Q_YKDATATAG
{
	bool			bValue;
	BYTE            controltype;//控制类型(0无反校,1有返校)
	BYTE			execresult;//1成功2超时3失败
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];
	int             nIndex;//id号
	int             channelid;//通道id
	int             device;//站编号
	int             no; //站内序号
	int             yxindex;//遥信索引
	time_t            selecttime;//选择时间
	time_t			  selectreplytime;//回复时间
	time_t            exectime;//执行时间
	time_t            finishtime;//完成时间
	bool			bzf;//是否为转发点
	int             zfchannelid;//转发通道id
	int             zfdevice;//转发站编号
	int             zfno; //转发站内序号
}Q_YKDATATAG,*LPQ_YKEDATATAG;

/** 遥调 */
typedef struct _Q_YTDATATAG
{//yt
	BYTE            controltype;//控制类型(无反校0,有返校1)
	BYTE			execresult;//1成功2超时3失败4停止激活
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];
	int             ycindex;//遥测索引
	int             channelid;//通道id
	int             device;//站编号
	int             no; //站内序号
	int             nIndex;//id号
	int				nValue;
	float			fValue;
	float           factor;//系数
	time_t          selecttime;//选择时间
	time_t			selectreplytime;//回复时间
	time_t          exectime;//执行时间
	time_t          finishtime;//完成时间
	bool			bzf;//是否为转发点
	int             zfchannelid;//转发通道id
	int             zfdevice;//转发站编号
	int             zfno; //转发站内序号
}Q_YTDATATAG,*LPQ_YTEDATATAG;


/** @brief Define user data struct. */
typedef struct _Q_RECORDDATATAG
{
	Q_DataType		eDataType;
	int				nIndex;//索引值编号
	bool			bValue;
	float			fValue;
	int				nValue;
	int             channelID;//通道编号新增加 2015-06-24
	int             device;//新增 站地址
	int             no;//新增 站数据排序号
	
}Q_RECORDDATATAG,*LPQ_RECORDDATATAG;


typedef struct _Q_RECORDDATATAG2
{
	Q_RECORDDATATAG val;
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];

}Q_RECORDDATATAG2,*LPQ_RECORDDATATAG2;

//CPU字节对齐
typedef enum _Q_ComCpuEndian
{
	eLittleEndian=1, //小端对齐
	eBigEndian=2     //大端对齐
}Q_ComCpuEndian;

enum Q_COMM_LOG_TYPE
{
	eCommLogTypeTx=1,//发送消息
	eCommLogTypeRx=2,//接收到的消息
	eCommLogTypeError=3, //错误的消息
	eCommLogTypeMsg=4  //信息值
};

//typedef struct Q_COMM_LOG_INFO
//{
//	Q_COMM_LOG_TYPE type;//消息类型
//	char *msg;//信息内容
//	int len;//消息长度
//}Q_COMM_LOG_INFO;

//转发遥测定义
typedef struct _Q_TRANS_ALGDATATAG
{
	int             nIndex;//id号
	Q_DataType		eDataType;//类型
	float			fValue;
	int				nValue;
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];
	int             device;//转发站编号
	int             no; //站内序号
	float           factor;//系数
	int             sindex;//源站点号
   _Q_TRANS_ALGDATATAG(void)
   {
	   nIndex=-1;
	   eDataType=eDataTypeNull;
	   fValue=0.0f;
	   nValue=0;
	   memset(chName,0,sizeof(chName));
	   device=0;
	   no=0;
	   sindex=0;
	   factor=1.0f;
   }

}Q_TRANS_ALGDATATAG,*LPQ_TRANS_ALGDATATAG;
//转发遥信定义
typedef struct _Q_TRANS_DIGDATATAG
{
	int             nIndex;//序号
	bool			bValue;
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];
	int             device;//转发站编号
	int             no; //转发站数据站内点号
	int             sindex;//源点号
	_Q_TRANS_DIGDATATAG(void)
	{
		nIndex=-1;
		bValue=false;
		memset(chName,0,sizeof(chName));
		device=0;
		no=0;
		sindex=0;
	}
}Q_TRANS_DIGDATATAG,*LPQ_TRANS_DIGDATATAG;
//通道状态
typedef struct _Q_CHN_INFO
{
	int             nID;//通道ID
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];//通道名
	int             nStatus;//通道状态 0  关闭 1运行
	int			statusid;//jwk add
	bool        enabled;
	char            cfgFile[Q_FRAME_CONST64];
	_Q_CHN_INFO(void)
	{
		enabled=0;
		nID=0;
		memset(chName,0,sizeof(chName));
		nStatus=0;
		memset(cfgFile,0,sizeof(cfgFile));
	}
}Q_CHN_INFO,*LPQ_CHN_INFO;

//节点的主备情况
const BYTE eNodeTypeSlave=0;
const BYTE eNodeTypeMaster=1;
const BYTE eNodeStateOffline=0;
const BYTE eNodeStateRunning=1;
const BYTE eNodeStateStarting=2;
const BYTE eNodeError=1;//错误
const BYTE eNodeNormal=0;
//备用方式
const BYTE eRunMethodCold=1;
const BYTE eRunMethodHot=2;
const BYTE eRunMethodStandalone=0;
//信息类型
const BYTE eInfoTypeCommon=1;//对端通用信息
const BYTE eInfoTypeAction=2;// 对端动作信息
//结点配置表
typedef struct _Q_NODE_INFO
{
	UINT             nID;//结点编号
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];//结点名
	BYTE nRunMethod;//运行方式1单机运行2冷备用运行3热备用运行
	BYTE	     nNodeType;//是否是主机 1为主机 0为备机
	UINT         nDefaultMaster;//默认主机结点号
	BYTE         nStatus; //状态0 offline,2启动中 1运行
	long         nTimeStamp;//时标
	UINT         nCurNodeID;//当前结点id
	BYTE         nError;//是否出错
	_Q_NODE_INFO(void)
	{
		nID=0;
		//memset(chName,NULL,sizeof(chName));
		memset(chName,0,sizeof(chName));
		nRunMethod=1;
		nNodeType=eNodeTypeSlave;
		nDefaultMaster=0;
		nStatus=eNodeStateOffline;
		nTimeStamp=0;
		nCurNodeID=0;
		nError=eNodeNormal;
	}
}Q_NODE_INFO,*LPQ_NODE_INFO;

typedef struct _CommandVal
{
	int   DestinationId;
	int   ObjectIndex;
	float fval;
	int nval;
	int trycount;//10
	bool yt;//false为yk
	_CommandVal()
	{
		DestinationId=0;
		ObjectIndex=0;
		fval=0;
		nval=0;
		trycount=10;
		yt=false;
	}
}Q_CommandVal;

//typedef struct _SocketClientInfo
//{
//	SOCKET						m_socketClient;//客户端
//	SOCKADDR_IN					m_addressClient;//客户端地址
//	bool                        m_bPrimary;//是否为主
//	int                         m_errCount;
//}Q_SocketClientInfo;

//2015-04-15 add ty tjs 
typedef enum _Protocol_VarType
{
	eModVarTypeNull=0,//未知类型
	eModVarTypeYc=1,//遥测变化
	eModVarTypeYx=2,//遥信变化
	eModVarTypeEx=3 //其它的值变化
}Q_Protocol_VarType;

//用于触发外部执行命令用
typedef struct _Protoco_ExecCmd
{
	int funcode;// 功能码
	int channelid;//通道编号
	Q_Protocol_VarType varType;//变化的数据类型
	int m_vindex;//变化的索引,默认是从零开始的
	bool bzf;
	int zfchannelid;//转发通道id
	int zfdevice;//转发站编号
	int zfno; //转发站内序号
	Q_RECORDDATATAG m_tag;//更新数据
	_Protoco_ExecCmd(void)
	{
		channelid=0;
		bzf=false;
		zfchannelid=0;
		zfdevice=0;
		zfno=0;
		funcode=0;
		varType=eModVarTypeNull;
		m_vindex=0;
	}
}Q_Protocol_ExecCmd;
     



#define cpt_RTSERVER			(1 << 0)	//系统管理
#define cpt_HISTORY				(1 << 1)	//历史转储
#define cpt_CHANNELOP			(1 << 2)	//前置通讯
#define cpt_Total        1 //最大进程个数


#define mes_PROTCOL_PACKET			11  //规约报文
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

#define mes_SPSelect                74  //设点add by tjs 2015-06-29
#define mes_SPExecute               75

//#define mes_MsgPrint                152  //规约打印
#define mes_MsgExpand               82  //扩展消息传输
#define mes_MsgSwitch               83  //主备切换

#ifdef UNIX_ENV
#  define ATTR_NORETURN __attribute__((noreturn))
#  define ATTR_PRINTF(F,V) __attribute__ ((format (printf, F, V)))
#else
#  define ATTR_NORETURN
#  define ATTR_PRINTF(F,V)
#endif

#define RELOAD_COMMLINK         1
#define RELOAD_STATION          2
#define SAVE_COMMLINK         1
#define SAVE_CHANNEL          2
const int MAIN_COMM    = 0;           /* max station per link */ 
const int BACK_COMM    = 1;           /* max station per link */ 
#define FEP_PRIMARY         0
#define FEP_SLAVE           1

//2016-6-22 tcp server 过滤IP使用
typedef struct Q_LSrvPort
{
	char ip[30];//ip地址
	int nChID; //通道ID号
	bool valid;//是否有效
	Q_LSrvPort()
	{
     valid=false;
	 nChID=0;
	}
}Q_LSrvPort;

#endif // !defined(__Q_FRAMEDEFINE_H_INCLUDED__)





