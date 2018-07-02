
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
const BYTE  CMD_STARTDT		 =0x04;           //������·
const BYTE  CMD_STARTDT_CON  =0x08;          //�������ݴ���ȷ��
const BYTE  CMD_STOPDT		 =0x10;
const BYTE  CMD_STOPDT_CON   =0x20;          //ֹͣ���ݴ���ȷ��
const BYTE  CMD_TESTFR		 =0x40;
const BYTE  CMD_TESTFR_CON	 =0x80;           //����ȷ��

const BYTE  CMD_ACT             =0x06;           //����
const BYTE  CMD_ACT_CON         =0x07;           //����ȷ��
const BYTE  CMD_DEACT           =0x08;           //��ֹ����
const BYTE  CMD_DEACT_CON       =0x09;         //��ֹ����ȷ��
const BYTE  CMD_DEACT_TERM      =0x0a;       //������ֹ  
const BYTE  CMD_COMMON_ADDR_TMP =0x01; 
const BYTE  CMD_KNOWN_INFO_ADDR =0x6f;//δ֪����Ϣ�����ַ6f����2f?
const UINT MIN_FRAME_NUM=6;//��С֡��С
//const UINT MAX_REMOTE_NUM=8;//���ĶԶ˸��� //ԭ����10��
const UINT MAX_REMOTE_NUM=32;//���ĶԶ˸��� //ԭ����8��

const UINT MAX_104_PACKET_NUM=255;//����104���Ĵ�С


typedef enum _Q_IEC104_ErrorCode
{

	e104ErrNo_Suc=0,//�ɹ�
	e104ErrNo_WriteSend=1, //д�������������
	e104ErrNo_Link=2,//��·�Ͽ������
	e104ErrNo_P_UnExist=3, //ң�ص�ַ��Ӧ�ĵ������
	e104ErrNo_4=4,
	e104ErrNo_FrameNum=5, //֡�����Ի��ߴ������ֵ����С����Сֵ6
    e104ErrNo_RxOverTx=6, //�������к�Ӧ��С�ڽ������к�
	e104ErrNo_FrameSeq    =7,  //֡����������
	e104ErrNo_CmdListNull =8, //�����б�û�г�ʹ��
	e104ErrNo_CmdNoSel    =9,   //����û��ѡ���豸
	e104Errno_CmdSend     =10,    //��������
	e104Errno_UptDb       =11,//����ʵʱ��
	e104ErrNo_MisAsdu     =12, //�������ݷ���Ԫ��Ų���
	e104ErrNo_FrameVerify    =13,  //֡����ʧ��
	e104ErrNo_BeyondK    =14  //֡����ʧ��

}Q_IEC104_ErrorCode;

typedef enum _Q_IEC104CmdType
{
	eCmdTypeC_Null   =0, //��
	eCmdTypeC_IC     =1, //�ٻ�
	eCmdTypeC_SC     =2, //����ң��		
	eCmdTypeC_DC     =3, //˫��ң��
	eCmdTypeC_SE     =4, // �趨����
	eCmdTypeC_CS     =5, //ʱ��ͬ��
    eCmdTypeC_EI     =6, //��ʼ������
	eCmdTypeC_RC     =7, //��������
	eCmdTypeTestFR   =8,   //������·
	eCmdTypePlanCurve   =9,    //�ƻ�����
	eCmdTypeParameter=10,   //�����趨
	eCmdTypeAnalogchange=11 //�仯ң���ϴ�

}Q_IEC104CmdType;
typedef enum _Q_IEC104CmdState
{
	eCmdStateNull         =0, //׼��
	eCmdStateEnd          =1,//�������
	eCmdStateYkSelect     =2,//ң��ѡ��
    eCmdStateYkSelectCon    =3, //ң��ѡ����
	eCmdStateYkExec       =4, //ִ��
	eCmdStateYkExecAct    =5,//ִ�����ȷ��
	eCmdStateYkCancel     =6,//����
	eCmdStateYkCancelAct  =7, //����ȷ��
	eCmdStateIntRequest   =8,//�ٻ�����
	eCmdStateIntRequestAct=9,//�ٻ���Ӧ
	eCmdStateIntSendData  =10,//�ٻ�����������
	eCmdStateIntEnd       =11, //�ٻ����� 
	eCmdStateIntSendDataFin =12, //���ٷ����������
	eCmdStateSync         =13,//ͬ������
	eCmdStateSyncAct      =14, //ͬ��ȷ��
	eCmdStateYkConfirm    =15,  //ң��ȷ��
	eCmdStateYkExecCon = 16 //ִ�����·�

}Q_IEC104CmdState;
typedef enum _Q_IEC104_ST_Type
{
	eStation_Unknown =0,   //δ֪վ��
	eStation_Master  =1,  //��վ
	eStation_Slave   =2    //��վ
}Q_IEC104_ST_Type;


//֡����
typedef enum _Q_IEC104FrameType
{
	eFrameType_Null=0,//��Ч
	eFrameType_I=1,
	eFrameType_U=2,
	eFrameType_S=3
}Q_IEC104FrameType;
//У�鷵��ֵ

class   Q_IEC104vfyRet
{
public:
	 Q_IEC104FrameType m_eFrameType;
	bool m_bSuc;//�Ƿ�ɹ�
	WORD m_nErrorCode;//�����롣 
	Q_IEC104vfyRet(void)
	{
      m_eFrameType=eFrameType_Null;
	  m_bSuc=true;
      m_nErrorCode=0;
	}
};
typedef Q_Protocol_VarType Q_IEC104_VarType;
typedef Q_Protocol_ExecCmd Q_IEC104_ExecCmd;

//�����붨��:
enum FUNC_CODE_PRM_DEF
{
	FUNC_PRM_RSTLINK = 0,
	FUNC_PRM_RSTPROCESS = 1,
	FUNC_PRM_TESTLINK = 2,
	FUNC_PRM_SENDDATA_WC = 3,
	FUNC_PRM_SENDDATA_NC = 4,
	FUNC_PRM_REQ_ACD = 8,
	FUNC_PRM_REQ_LINKSTAT = 9, //����Զ����·״̬
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
//֡���Ͷ���
enum FRM_TYPE_DEF
{
	FRMTYPE_SEND_NC = 1,
	FRMTYPE_SEND_WC = 2,
	FRMTYPE_REQ_RESP = 3,
};
//������·״̬, PRM: ת����>����վ; SUB:ԭվ��>ת��.
//ԭ��״̬:
enum IEC104_LINK_PRM_STATE_DEF
{
	IEC104_LINK_PRM_STATE_NULL = 0,
	IEC104_LINK_PRM_STATE_REQSTATUS = 1,    //����Զ����·״̬
	IEC104_LINK_PRM_STATE_SENDRST = 2,      //����λ��·
	IEC104_LINK_PRM_STATE_LINKOK= 3,	//���������
	IEC104_LINK_PRM_STATE_WAITRSCONF = 4,	//WAIT ACK/NACK FOR RESET
	IEC104_LINK_PRM_STATE_WAITSTATUS = 5,	//WAIT FOR LINKSTATUS

};
//����״̬:
enum IEC104_LINK_SUB_STATE_DEF
{
	IEC104_LINK_SUB_STATE_NULL = 0,
	IEC104_LINK_SUB_STATE_STATUSCONF = 1, //�ظ�������·״̬
	IEC104_LINK_SUB_STATE_RSTCONF = 2,     //�ظ�������
	IEC104_LINK_SUB_STATE_INITFIN = 3      //��ʹ�����
};
//����Ӧ�ò�״̬
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

//Ӧ�ò��������Ͷ���
enum APP_DATA_TYPE_DEF
{
	APPTYPE_SP_NT = 1,
	APPTYPE_SP_WT = 2,
	APPTYPE_DP_NT = 3,
	APPTYPE_DP_WT = 4,
	APPTYPE_ME_NT = 9,
	APPTYPE_ME_WT = 10,
	APPTYPE_M_ME_NC = 13, ////������ 
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
/////����ԭ��
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
//��Ӧ��ʽ����
enum ACK_TYPE_DEF
{
	ACK_UNSOLICITED = 0,	//��������Ӧ
	ACK_REQUESTED = 1	//��������Ӧ
};
//����ÿ����������
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