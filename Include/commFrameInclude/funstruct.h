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
    int     bucket;                              /* hash bucket hashͰ*/
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
	INT         	iCallP;                            //�е��������
};
typedef struct _StationArgs    StationArgs;

/*
 * ͨ����·�Ĳ���
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
    //time_t      begin;                  /* record begin time  �޸�Ϊ�����ٻ�ʱ��*/
    time_t      groupTime;                    /* record end time �޸�Ϊ�����ٻ�ʱ��*/

};
typedef struct _CommLinkArgs    CommLinkArgs;

#define COMMLINK_PRIMARY    1
#define COMMLINK_SLAVE      2

//commlink flag
#define COMMLINK_STOP       1<<2
#define COMMLINK_DEBUG       1<<3

/*
 * ����ͨ������
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
 * ������·����
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
	char       		strTu[20];                         //�ɼ���Ԫ����
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
	char       		strTu[20];                         //�ɼ���Ԫ����
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
	char       		strTu[20];                         //�ɼ���Ԫ����
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
	INT         	iType;      //����
	INT         	ulYXID32;   //��ң�ű��е�ID
	char       		strTu[20];                         //�ɼ���Ԫ����
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
    int         num;//�����ݰ�ģ��������
    UINT        sid[MAXANALOG];             /* վIDid32 ����һ����Ե���ͨ��ID����Ϊһ��վһ��ͨ��*/
    int         index[MAXANALOG];           /* point in station ��� */
    FLOAT       raw[MAXANALOG];             /* raw  ԭʼ���� ,ԭ����int���͵�*/ 

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
//	int         device[MAXANALOG]; /*2015-06-24����ͨѶʱʹ�ô���վ��ַ*/
	//int         no[MAXANALOG];/*վ���*/
};
typedef struct _DigitPackets    DigitPackets;    

/*
 * ң�����ݺ�ң�������ݽṹ
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
const int   MES_TYPE_NONE       = 0;            // ��ʾû���¼�
const int   MES_TYPE_RELAY      = 1;            // ң���¼�
const int   MES_TYPE_DIGIT      = 2;            // ң�ű�λ�¼�
const int   MES_TYPE_SOE        = 3;            // SOE�¼�
const int   MES_TYPE_FAULT      = 4;            // �����¼�
const int   MES_TYPE_PROTECT    = 5;            // ���������¼�
const int   MES_TYPE_SPOINT     = 6;            //���ģʽ
const int   MES_TYPE_MsgPrint   = 11;           //��Լ��ӡ 
const int   MES_TYPE_MsgExpand  = 12;           //��չ��Ϣ

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
    int        value;//ֵ
    int        state;
	int        device;//2015-06-30 �豸��
};
typedef struct _MesRelayBody    MesRelayBody;


/*********************************************************************
 * ң�ű�λ�¼��Ľṹ
 *    point    : ���    
 *    value    : ״̬
*********************************************************************/
struct _MesDigitBody
{
    MesCommBody    comm;
    int        point;
    int        value;
};
typedef struct _MesDigitBody    MesDigitBody;


/*********************************************************************
 * SOE�¼��Ľṹ
 *    point    : SOE�¼���ң�ŵ�
 *    value    : ״̬
 *    time    : ʱ��
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
 * �����¼��Ľṹ������Ͷ���
 *    device    : �����豸
 *    fault    : ��������
 *    time    : ����ʱ��
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
 * �����¼��Ľṹ�����ض���
 *    device    : �����������豸վ���豸��
 *    type    : �������� 
 *    time    : ����ʱ�� 
*********************************************************************/
const int    MES_BH_UNDEFINE  = 0;    // δ����
const int    MES_BH_GL        = 1;    // �����ٶϱ�������
const int    MES_BH_SY        = 2;    // ʧѹ��������
const int    MES_BH_FDBZT     = 3;    // �ֶα���Ͷ
const int    MES_BH_LLBZT     = 4;    // ���籸��Ͷ
const int    MES_BH_BZT       = 5;    // ��ͨ����Ͷ

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
	int             point;            /* ��� */
	float           val;//ֵ
	int             device;//�豸��
};
typedef struct _MesSetPointBody        MesSetPoinBody;


//20150907��˶��ӣ���Ϣ��Լ��ӡ����ʹ�ã�
struct _MesMsgPrint
{
	MesCommBody     comm;        //ͨ���¼�
	int             ichannelid;  //ͨ��ID
	int             msgtype;     //��ӡ����
	int             nlen;        //����
	BYTE	        msg[200];    //����
};
typedef struct _MesMsgPrint   MesMsgPrint;


//20160301��˶���
struct _ExpandMsg
{
	MesCommBody  comm;              //ͨ���¼�
	int   nChannelid;               //ͨ��id
	int   nDevice;                  //�豸id
	int   nProperty1;               //�Զ�������1-������Oracel��ԼӦ���У�����Ϊ��ID�������������ѡ��ͬ�ı�ṹ
	int   nProperty2;               //�Զ�������2-Ҳ�����ǵ������
	BYTE  m_bMes[200];              //��Ϣ��(�Զ���ṹ�壬���Ȳ��ó���200�ֽڣ���ָ��ת��)
};
typedef struct _ExpandMsg  ExpandMsg;

/*********************************************************************
 * ��Ϣ������ݽṹ,ʹ������
*********************************************************************/
union _MesBody
{
    MesCommBody     comm;               // ͨ���¼�
    MesRelayBody    relay;              // ң���¼�
    MesSOEBody      soe;                // soe�¼� 
    MesDigitBody    digit;              // ң�ű�λ�¼�
    MesFaultBody    fault;              // �����¼�
    MesProtectBody  protect;            // �����¼�
	MesSetPoinBody  spoint;             // ����ң��
	ExpandMsg       mesmsgexpand;       // ��չ��Ϣ-20160229��˶���
	MesMsgPrint     mesmsgprint;	    // ��Լ��ӡ�¼�-20150907��˶���
};
typedef union    _MesBody    MesBody;        


/*********************************************************************
 * ��Ϣ�����ݽṹ
 * flag    : 0 �����꣬�ȴ���У; 1 δ����
 * tt    : ��ʾ���¼��ύ��ʱ��,���ڳ�ʱ����
 * mes    : ���¼��Ľṹ��
 * next : ��һ���¼�
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
 * �������Ͷ���,����ڷ־�
*********************************************************************/
const int    PROTECT_NONE    = 0;        // δ��������    
const int    PROTECT_TYPE_1    = 1;        // ����һ�γ���
const int    PROTECT_TYPE_2    = 2;        // �������γ���
const int    PROTECT_TYPE_3    = 3;        // �������γ���
const int    PROTECT_TYPE_4    = 4;        // �غ�բ

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

	struct UtilHash      setpoint;//���Ҳ��ң�� 2015-6-25

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

//2015-06-24�½�tjs
//���ڽ���hash��ʹ�ã��Ա���Ը���վ��ַ��վ��Ų�ѯ�ڲ�ID
struct _stDEVICE_NO
{
	UINT device;//վ��ַ
	UINT no;//վ�����
};
typedef _stDEVICE_NO stDeviceNo;
#endif //_FEP_STRUCT_H_
