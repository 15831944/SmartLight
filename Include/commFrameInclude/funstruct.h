#ifndef _FEP_STRUCT_H_
#define _FEP_STRUCT_H_

#include "portdefines.h"
#include "gSystemDef.h"
//#include "defines.h"

typedef timeval		TIME;
typedef timeval	*	PTIME;
typedef timeval	*	LPTIME;

#define OBJ_CHAR        1
#define OBJ_INT         2
#define OBJ_LLONG       3
#define OBJ_FLOAT       4
#define OBJ_TIME        5
#define OBJ_STRING      6
#define OBJ_BINARY      7

struct UtilHashv {
    int     key;
    void *  item;

    struct UtilHashv * left;
    struct UtilHashv * right;
    struct UtilHashv * next;
};

struct UtilHash
{
    int     maxkey;                              /* max key */
    int     bucket;                              /* hash bucket hash桶*/
    int     count;                               /* hash count */

    struct UtilHashv *  seqx_left;                  /* seq */
    struct UtilHashv *  seqx_right;                 /* seqx right */
    struct UtilHashv ** table;                      /* hash table */
};

struct UtilTimer
{
    ULONG64    base;
    ULONG64    pass;
};

struct UtilStatis
{
    int *   array;
    int     count;    
    int     sec_count;
    int     current;
};

struct UtilBinStrus {
    int     size;
    int     len;
    void *  bin;
};

struct UtilStrucs {
    void *  struc;
    int *   stype;
    int *   offset;
    int     count;
};

const int    SRS_NO_HAPPEN   = 0;            /* no protect event */
const int    SRS_HAPPEN      = 1;            /* protect happen */

const int    TIMER_SRS      = 10000;        
const int    TIMER_STATE    = 1000;        
const int    TIMER_RELOAD   = 5000;       
const int    TIMER_SYNC     = 400000;    

const int    MAXANALOG   = 32;
const int    MAXDIGIT    = 32;
const int    MAXPULSE    = 32;

struct _StationArgs
{
    UINT        sid;                    /* station id32 */
    int             type;                   /* station type */
    int             record;                 /* record flag */
    int             state;                  /* station state */
    char            name[64];               /* station name */
    int             digit;
    int             analog;
    int             pulse;
    int             relay;
    time_t          begin;                  /* record begin */
    time_t          end;                    /* record end */ 
	INT         	iCallP;                            //招电度量周期
};
typedef struct _StationArgs    StationArgs;

/*
 * 通信链路的参数
 */
struct _CommLinkArgs
{
    UINT        cid;	    	    /* commlink id */
    char            name[32];		    /* commlink name */
    int             channelid;		    /* channel id */
    int             mode;       	    /* commlink mode(0, 1, 3, 4) */
    int             channelmode;        /* channel mode */
    int             address;		    /* commlink address */

    int             switcht;	  	    /* commlink switch time */
    int             offline;		    /* offline time */  
    int             callTime;		    /* CallTime time */  

    int             level;	    	    /* commlink level(0,1) */
    int             relate;	    	    /* relating commlink *///
    int             flag;               /* commlink flag */

    int             protocoltype;	    /* protocol type */
    char            protocolargs[128];  /* protocol args */
    char            linktable[128];	    /* sub station address */

    /* statis */
    int         state;			        /* commlink state */ 
    UINT    sendbytes;		        /* sendbytes */ 
    UINT    sendright;              /* send rights */
    UINT    recvbytes;    
    UINT    recvright;   
    float       ratio;      

    /* record */
    int         record;                 /* commlink record type */
    //time_t      begin;                  /* record begin time  修改为分组召唤时间*/
    time_t      groupTime;                    /* record end time 修改为分组召唤时间*/

};
typedef struct _CommLinkArgs    CommLinkArgs;

#define COMMLINK_PRIMARY    1
#define COMMLINK_SLAVE      2

//commlink flag
#define COMMLINK_STOP       1<<2
#define COMMLINK_DEBUG       1<<3

/*
 * 物理通道参数
 */
struct ChannelArgs
{
    UINT     cid;	                /* channel id */ 
    int          type;			        /* channel type */
    char         name[32];		        /* channel name */
    char         args[128];		        /* channel args */
	INT          fepID;                 // channel fepid
	INT          fepGroupID;            // channel fepgroupid
};
typedef struct ChannelArgs    ChannelArgs;

const int MAX_RELATED_NUM    = 32;           /* max station per link */ 
/*
 * 关联链路参数
 */
struct CommulinkRelate
{
	int			m_iRelateState[MAX_RELATED_NUM];
	int			m_iRelateNum;
	int			m_pRelateID[MAX_RELATED_NUM];
};
typedef struct CommulinkRelate  CommulinkRelate;

struct _DigitTable
{
    UINT    sid;
    int         index;                      /* point in station */
    UINT        ID32;        /* id32 */            
	char       		strTu[20];                         //采集单元名称
    char        name[64];                   /* digit name */
    int         raw;                        /* raw value */
    int         value;                      /* real value */
    int         reverse;                    /* is reverse */
    int         refresh;                    /* is refresh */
    SYSTEMTIME    time;             /* last receive time */
};
typedef struct _DigitTable    DigitTable;

struct _AnalogTable
{
    UINT        sid;
    int             index;
    UINT        ID32;        /* id32 */            
	char       		strTu[20];                         //采集单元名称
    char            name[64];
    FLOAT             raw;
    float           value;
    int             refresh;                /* is refresh */
    float           coef;  
    float           offset;
    SYSTEMTIME      time;
};
typedef struct _AnalogTable    AnalogTable;

/*
 * pulse data
 */
struct _PulseTable
{
    UINT        sid;
    int             index;
    UINT        ID32;        /* id32 */            
	char       		strTu[20];                         //采集单元名称
    char            name[64];
    int             raw;
    float           value;
    int             refresh;
    float           coef;
    float           offset;
    SYSTEMTIME      time;    
};
typedef struct _PulseTable    PulseTable;

struct _RelayTable
{
    UINT        sid;        /* station id32 */            
    int             index;      /* point in station */
    UINT        ID32;        /* id32 */            
	INT         	iType;      //类型
	INT         	ulYXID32;   //在遥信表中的ID
	char       		strTu[20];                         //采集单元名称
    char            name[64];   /* name */
};
typedef struct _RelayTable    RelayTable;

/*
 * analog package
 */
struct _AnalogData
{
    UINT   sid;             /* station id32 */
    int        index;           /* point in station */
    int        raw;             /* raw */
};
typedef struct _AnalogData    AnalogData;

struct _AnalogPackets
{
    int         num;//此数据包模拟量个数
    UINT        sid[MAXANALOG];             /* 站IDid32 这里一般可以当成通道ID，因为一个站一个通道*/
    int         index[MAXANALOG];           /* point in station 点号 */
    FLOAT       raw[MAXANALOG];             /* raw  原始数据 ,原来是int类型的*/ 

};
typedef struct _AnalogPackets    AnalogPackets;

/*
 * digit data from sub station
 */
struct _DigitPackets
{
    int         num;
    UINT        sid[MAXDIGIT];
    int         index[MAXDIGIT];
    int         raw[MAXDIGIT];
//	int         device[MAXANALOG]; /*2015-06-24增加通讯时使用代表站地址*/
	//int         no[MAXANALOG];/*站编号*/
};
typedef struct _DigitPackets    DigitPackets;    

/*
 * 遥脉数据和遥脉包数据结构
 */
struct _PulsePackets
{
    int         num;
    UINT    sid[MAXPULSE];
    int         index[MAXPULSE];
    int         raw[MAXPULSE];
};
typedef struct _PulsePackets    PulsePackets;

/*********************************************************************
 * common event define
 *    destination : event destination station (defaut 0)    
 *    source      : event source station id (default 0)
 *    type        : event type
*********************************************************************/
const int   MES_TYPE_NONE       = 0;            // 表示没有事件
const int   MES_TYPE_RELAY      = 1;            // 遥控事件
const int   MES_TYPE_DIGIT      = 2;            // 遥信变位事件
const int   MES_TYPE_SOE        = 3;            // SOE事件
const int   MES_TYPE_FAULT      = 4;            // 故障事件
const int   MES_TYPE_PROTECT    = 5;            // 保护动作事件
const int   MES_TYPE_SPOINT     = 6;            //设点模式
const int   MES_TYPE_MsgPrint   = 11;           //规约打印 
const int   MES_TYPE_MsgExpand  = 12;           //扩展消息

const int   MES_CHECK_YES       = 1;            // check event
const int   MES_CHECK_NO        = 0;            // not check event

const int   CHECK_SUCCESS       = 1;            // check success
const int   CHECK_FAIL          = 0;            // check fail

const int   MES_DIGIT_OPEN      = 0;            // open state
const int   MES_DIGIT_CLOSE     = 1;            // close state

struct _MesCommBody
{
    UINT    destination;                    /* destination station */
    UINT    source;                         /* source station */
    int         type;                           /* event type */
};
typedef struct _MesCommBody    MesCommBody;     /* common message body */

/*********************************************************************
 * relay event
 *    cmd       : type
 *    check     : is check
 *    point     : point in station
 *    value     : dest value
 *    state     : check sucess/fail
*********************************************************************/
const int    RELAY_SELECT    = 0; 
const int    RELAY_EXECUTE   = 1;
const int    RELAY_CANCEL    = 2;
const int    RELAY_NONE    = -1;

struct _MesRelayBody
{
    MesCommBody    comm; //destion is channel id
    int        cmd;     
    int        check;  
    int        point;  //no 
    int        value;//值
    int        state;
	int        device;//2015-06-30 设备号
};
typedef struct _MesRelayBody    MesRelayBody;


/*********************************************************************
 * 遥信变位事件的结构
 *    point    : 点号    
 *    value    : 状态
*********************************************************************/
struct _MesDigitBody
{
    MesCommBody    comm;
    int        point;
    int        value;
};
typedef struct _MesDigitBody    MesDigitBody;


/*********************************************************************
 * SOE事件的结构
 *    point    : SOE事件的遥信点
 *    value    : 状态
 *    time    : 时间
*********************************************************************/
struct _MesSOEBody
{
    MesCommBody     comm;        
    int             point;
    int             value;
    SYSTEMTIME    time;
};
typedef struct _MesSOEBody    MesSOEBody;        


/*********************************************************************
 * 故障事件的结构体和类型定义
 *    device    : 故障设备
 *    fault    : 故障类型
 *    time    : 故障时间
*********************************************************************/
struct _MesFaultBody
{
    MesCommBody    comm;
    int            device;
    int            fault;

    SYSTEMTIME      time;
};
typedef struct _MesFaultBody        MesFaultBody;


/*********************************************************************
 * 保护事件的结构体和相关定义
 *    device    : 发生保护的设备站内设备号
 *    type    : 保护类型 
 *    time    : 发生时间 
*********************************************************************/
const int    MES_BH_UNDEFINE  = 0;    // 未定义
const int    MES_BH_GL        = 1;    // 过流速断保护动作
const int    MES_BH_SY        = 2;    // 失压保护动作
const int    MES_BH_FDBZT     = 3;    // 分段备自投
const int    MES_BH_LLBZT     = 4;    // 联络备自投
const int    MES_BH_BZT       = 5;    // 普通备自投

struct _MesProtectBody
{
    MesCommBody     comm;
    int             device;             /* device in station */ 
    int             protect;            /* protect type */
    SYSTEMTIME      time;       /* receive time */
};
typedef struct _MesProtectBody        MesProtectBody;

//2015-06-29
struct _MesSetPointBody
{
	MesCommBody     comm;
	int             point;            /* 点号 */
	float           val;//值
	int             device;//设备号
};
typedef struct _MesSetPointBody        MesSetPoinBody;


//20150907樊硕添加，消息规约打印功能使用！
struct _MesMsgPrint
{
	MesCommBody     comm;        //通用事件
	int             ichannelid;  //通道ID
	int             msgtype;     //打印类型
	int             nlen;        //长度
	BYTE	        msg[200];    //内容
};
typedef struct _MesMsgPrint   MesMsgPrint;


//20160301樊硕添加
struct _ExpandMsg
{
	MesCommBody  comm;              //通用事件
	int   nChannelid;               //通道id
	int   nDevice;                  //设备id
	int   nProperty1;               //自定义属性1-例如在Oracel规约应用中，定义为表ID，根据这个属性选择不同的表结构
	int   nProperty2;               //自定义属性2-也可以是点号属性
	BYTE  m_bMes[200];              //消息体(自定义结构体，长度不得超过200字节，用指针转换)
};
typedef struct _ExpandMsg  ExpandMsg;

/*********************************************************************
 * 消息体的数据结构,使用联合
*********************************************************************/
union _MesBody
{
    MesCommBody     comm;               // 通用事件
    MesRelayBody    relay;              // 遥控事件
    MesSOEBody      soe;                // soe事件 
    MesDigitBody    digit;              // 遥信变位事件
    MesFaultBody    fault;              // 故障事件
    MesProtectBody  protect;            // 保护事件
	MesSetPoinBody  spoint;             // 设点或遥调
	ExpandMsg       mesmsgexpand;       // 扩展消息-20160229樊硕添加
	MesMsgPrint     mesmsgprint;	    // 规约打印事件-20150907樊硕添加
};
typedef union    _MesBody    MesBody;        


/*********************************************************************
 * 消息的数据结构
 * flag    : 0 处理完，等待反校; 1 未处理
 * tt    : 表示此事件提交的时间,用于超时处理
 * mes    : 此事件的结构体
 * next : 下一个事件
*********************************************************************/
struct _MesStruct
{
    int         flag;
    MesBody     mes;  

    UtilTimer    tt;
    struct _MesStruct *    next;
};
typedef struct _MesStruct       MesStruct;

/*********************************************************************
 * 保护类型定义,大武口分局
*********************************************************************/
const int    PROTECT_NONE    = 0;        // 未定义类型    
const int    PROTECT_TYPE_1    = 1;        // 过流一段出口
const int    PROTECT_TYPE_2    = 2;        // 过流二段出口
const int    PROTECT_TYPE_3    = 3;        // 过流三段出口
const int    PROTECT_TYPE_4    = 4;        // 重合闸

#define TRANSMIT_YES        1
#define TRANSMIT_NO         0
#define COMM_ALIVE_TIMER    20000

const int MES_ESP               = 20000;     

const int MAX_TRANSMIT          = 10;
const int MAX_FILENAME          = 256;   
const int STATION_HASH_BUCKET   = 64;
const int POINT_HASH_BUCKET     = 1024;  

struct _scomm
{
    UINT         commid;
    int              level;
    int              state;
    int				 relateid;

    struct UtilTimer  timer;
    struct _scomm *  next;
};

struct _smap
{
    UINT            id;            
    int                 type;           

    MesStruct *         mes;   
    struct _scomm *     comm; 

    struct UtilHash      analog;
    struct UtilHash      digit;
    struct UtilHash      pulse;
    struct UtilHash      relay;

	struct UtilHash      setpoint;//设点也叫遥调 2015-6-25

    struct UtilHash      analog_index;
    struct UtilHash      digit_index;
    struct UtilHash      pulse_index;
    struct UtilHash      relay_index;
	struct UtilHash      setpoint_index;

    int                 iAnalogNum;           
    int                 iDigitNum;           
    int                 iPulseNum;           
    int                 iRelayNum;  
	int                 iSetPointNum;
    pthread_mutex_t     mutex;          
    struct _smap *      next; 
};

struct _smap_set
{
    struct UtilHash  hash;               
    UINT        cache[MAX_TRANSMIT];  
    int             count;                
}; //*_old_set = NULL;

/*
 * keep transmit file
 */
struct _file_table
{
    int     id;
    char    filename[MAX_FILENAME];
    struct _file_table    * next;
};

#define MAX_COMMULINK_NUM 100

#define COMMULINK_TIMEROUT		5

#define COMMULINK_OTHER			0
#define COMMULINK_WORKING		1
#define COMMULINK_STOPWORK		2
#define COMMULINK_TRY1WORK		3
#define COMMULINK_TRY2WORK		4
#define COMMULINK_TRY3WORK		5
#define COMMULINK_TRY4WORK		6
#define COMMULINK_TRY5WORK		7

struct obj_commlink_change
{
    int     iFepID;
    int     iCommuID;
    int     iStatus;
};

struct obj_commlink_status
{
    int     cid;
    int     status;
    int     isSrc;
};

#define MIN_LEN_STATUS 16
struct obj_fepnode_status
{
    int     cid;
    int     cgroupid;
    int     commnum;
};

#define MAX_MEMBER_NUM	16
struct obj_group_member
{
	int		cid;
	char	strip1[20];
	char	strip2[20];
};
/*#ifdef OPEN_MAX
static int openmax = OPEN_MAX;
#else
static int openmax = 0;
#endif
*/
const int OPEN_MAX_GUESS =    256;
static const char *TIMERCOMMAND = "sfdate";

#define HH_BCD2HEX(x) ((x >> 4) * 10 + (x & 0x0F))
#define HH_HEX2BCD(x) ((x << 4) / 10 + data % 10)
#define OFFSET(s, a)    (int)(&(((s *)0)->a))

//Relay_iType_TRFM
#define Relay_TRFM_UP		1
#define Relay_TRFM_STOP		0
#define Relay_TRFM_DOWN		-1

//2015-06-24新建tjs
//用于建立hash表使用，以便可以根据站地址与站编号查询内部ID
struct _stDEVICE_NO
{
	UINT device;//站地址
	UINT no;//站内序号
};
typedef _stDEVICE_NO stDeviceNo;
#endif //_FEP_STRUCT_H_
