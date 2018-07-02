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
#define Q_FRAME_DEFAULT_RECEIVEBUFFERSIZE		Q_FRAME_CONST4096 //ԭ����1024 2015-11-3ǰ��4096
#define Q_FRAME_DEFAULT_SENDBUFFERSIZE			Q_FRAME_CONST4096  //ԭ����2048  2015-11-3ǰ
#define Q_FRAME_DEFAULT_THREADPERIOD			Q_FRAME_CONST1024


#define Q_MSGTRAN_MAXMSGLEN                       170


enum Q_AVCMSGTYPE
{
	//1��������Ϣ����������ѹԽ�ޡ�ͨ���Ͽ��ȣ���2����ʾ��Ϣ��Ŀ��ֵ����ǰֵ����3�������Ϣ��4�����㿪ʼ��������Ϣ
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
	eMsgTypeTx=1,//������Ϣ
	eMsgTypeRx=2,//���յ�����Ϣ
	eMsgTypeCh=3,    //���ݽ��͡���Լ����	
	eMsgTypeMsg=4,  //��ʾ��Ϣ����Ϣֵ
	eMsgTypeError=5, //�������Ϣ
	eMsgTypeMsgAgc=6,  //��Ϣֵ������AGC��Ϣ
	eMsgTypeMsgAvc=7,  //��Ϣֵ������AVC��Ϣ
	eMsgTypeControl=8, //������Ϣ
}Q_MsgType;
enum Q_COMM_PROTOCOL_TYPE
{
  eCOMM_IEC104=1,
  eCOMM_MODBUS=2,
  eCOMM_CDT=3  
};
typedef struct _Q_COM_LOG_INFO
{
	int chid;//ͨ��id
	Q_MsgType type;//��Ϣ����
	char *msg;//��Ϣ����
	int len;//��Ϣ����
	Q_AVCMSGTYPE nAVCType;//AVC���ͣ�1��������Ϣ����������ѹԽ�ޡ�ͨ���Ͽ��ȣ���2����ʾ��Ϣ��Ŀ��ֵ����ǰֵ����3��SVG���������������ȣ�4�����㿪ʼ��������Ϣ
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
//��Ϊ��������103��Լ��Ҫ50��ͨ�������Կ���128������չ��
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
	eCommandTypeExtend			= 4 //��չ
}Q_CommandType;

typedef enum _Q_CommandTypeExt
{
	eCommandManual 			    = 0,//�ֶ��·�
	eCommandAgc					= 1,//AGC����
	eCommandAvc					= 2,//AVC����
}Q_CommandTypeExt;
#define Q_COMMANDLIST_MAX					Q_FRAME_CONST128  //ԭ����32,������Դ�Ƚ�����,���Ը�Ϊ128��




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
	eSerialModelNull			= 0,		///* ��У��
	eSerialModelMark			= 1,		///* ���У��
	eSerialModelOdd				= 2,		///* ��У��
	eSerialModelSpace			= 3,		///* ��żУ��
	eSerialModelEven			= 4			///* żУ��
}Q_SerialModel;


/** @brief Define serial link parameter. */





//----------------------------------------------------------------------------
//*
//*
//* Q_tcpClientLink macro
//*
//*

#define	Q_TCPCLIENT_SELECTTIME					300000	//300ms //һ��ͻ���socketʹ��
#define	Q_TCPCLIENT_SELECTTIME_SEC				1	//1�� //һ��ͻ���socketʹ��
#define	Q_TCPSERVER_CLIENT_SELECTTIME			100000	
#define	Q_UDP_SELECTTIME			500000	//�ͻ���socket��ʱ���ʱ��
#define Q_TCPCLIENT_RECEIVE_BUFFERSIZE			Q_FRAME_DEFAULT_RECEIVEBUFFERSIZE
#define Q_TCPCLIENT_SEND_BUFFERSIZE				Q_FRAME_DEFAULT_SENDBUFFERSIZE //��ʱ����tjs
//socket Ĭ�ϻ�������С���ݻ�������С�����𣬴���ϵͳ��socket�� 2015-11-11 add 
#define Q_SOCKET_RECV_BUFFERSIZE			   Q_FRAME_CONST1024*10
#define Q_SOCKET_SEND_BUFFERSIZE			   Q_FRAME_CONST1024*10





//
//#define Q_TCPSERVER_DEFPRECONNECT				Q_FRAME_CONST8 
//#define Q_TCPSERVER_DEFCLIENTCOUNT				Q_FRAME_CONST8

#define Q_TCPSERVER_DEFPRECONNECT				Q_FRAME_CONST16 //2015-04-02
//#define Q_TCPSERVER_DEFCLIENTCOUNT				Q_FRAME_CONST16
#define Q_TCPSERVER_DEFCLIENTCOUNT				Q_FRAME_CONST64 //Ϊ��֧��ɽ���żҲ���糡һ���˿�����12��IEC104��վ

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
	eDataTypeNull				= 0,		///* ������
	eDataTypeFloat				= 1,		///* ����
	eDataTypeInt				= 2,		///* ����
	eDataTypeBit				= 3			///* ��λ
}Q_DataType;

typedef enum _Q_DataQuality
{
	eCreate = 0,//���������
	eNomal = 1,//��������
	eBad = 2//ͨ���쳣
}Q_DataQuality;//������
typedef enum _Q_UnitType
{
	eUnitTypeNull=0,      //������
	eUnitTypekW=1,        //kW
    eUnitTypeMW=2,         //MW
	eUnitTypekVar=3,       //kVar
	eUnitTypeMVar=4         //MVar
}Q_UnitType;
/** @brief Define analogue data struct. */
typedef struct _Q_ANALOGUEDATATAG
{//yc
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];//����
	char            unit[20];//��λ
	BYTE            quality; //������0����1����2ֹͣ
	BYTE			datasource;//������Դ(�ɼ�������㡢���)0/1/2
	int             nIndex;//id��
	int             device;//վ���
	int             no; //վ�����
	int				nValue;
	int             lastnValue;//��һ��intֵ
	int             channelid;//ͨ��id
	float           rawvalue;//������
	float           factor;//ϵ��
	float			fValue;
	float           lastfValue;//��һ��floatֵ
	float           deadzone;//����
    Q_UnitType      eUnitType;
	Q_DataType		eDataType;//����
	time_t            timestatmp;//��ǰʱ��
	time_t            lastimestamp;//��һ��ֵ��ʱ��
	bool             changeflag;
}Q_ANALOGUEDATATAG,*LPQ_ANALOGUEDATATAG;



/** @brief Define digital data struct. */
typedef struct _Q_DIGITALEDATATAG
{//yx
	bool			bValue;
	bool			lastbValue;
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];
	BYTE			datasource;//������Դ(�ɼ�������㡢���)0/1/2
	BYTE            quality; //������
	int             nIndex;//���
	int             device;//վ���
	int             no; //վ�����
	int             channelid;//ͨ��id
	time_t            timestatmp;//��ǰʱ��
	time_t			lasttimestamp;
	bool             changeflag;
}Q_DIGITALEDATATAG,*LPQ_DIGITALEDATATAG;





/** ң�� */
typedef struct _Q_YKDATATAG
{
	bool			bValue;
	BYTE            controltype;//��������(0�޷�У,1�з�У)
	BYTE			execresult;//1�ɹ�2��ʱ3ʧ��
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];
	int             nIndex;//id��
	int             channelid;//ͨ��id
	int             device;//վ���
	int             no; //վ�����
	int             yxindex;//ң������
	time_t            selecttime;//ѡ��ʱ��
	time_t			  selectreplytime;//�ظ�ʱ��
	time_t            exectime;//ִ��ʱ��
	time_t            finishtime;//���ʱ��
	bool			bzf;//�Ƿ�Ϊת����
	int             zfchannelid;//ת��ͨ��id
	int             zfdevice;//ת��վ���
	int             zfno; //ת��վ�����
}Q_YKDATATAG,*LPQ_YKEDATATAG;

/** ң�� */
typedef struct _Q_YTDATATAG
{//yt
	BYTE            controltype;//��������(�޷�У0,�з�У1)
	BYTE			execresult;//1�ɹ�2��ʱ3ʧ��4ֹͣ����
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];
	int             ycindex;//ң������
	int             channelid;//ͨ��id
	int             device;//վ���
	int             no; //վ�����
	int             nIndex;//id��
	int				nValue;
	float			fValue;
	float           factor;//ϵ��
	time_t          selecttime;//ѡ��ʱ��
	time_t			selectreplytime;//�ظ�ʱ��
	time_t          exectime;//ִ��ʱ��
	time_t          finishtime;//���ʱ��
	bool			bzf;//�Ƿ�Ϊת����
	int             zfchannelid;//ת��ͨ��id
	int             zfdevice;//ת��վ���
	int             zfno; //ת��վ�����
}Q_YTDATATAG,*LPQ_YTEDATATAG;


/** @brief Define user data struct. */
typedef struct _Q_RECORDDATATAG
{
	Q_DataType		eDataType;
	int				nIndex;//����ֵ���
	bool			bValue;
	float			fValue;
	int				nValue;
	int             channelID;//ͨ����������� 2015-06-24
	int             device;//���� վ��ַ
	int             no;//���� վ���������
	
}Q_RECORDDATATAG,*LPQ_RECORDDATATAG;


typedef struct _Q_RECORDDATATAG2
{
	Q_RECORDDATATAG val;
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];

}Q_RECORDDATATAG2,*LPQ_RECORDDATATAG2;

//CPU�ֽڶ���
typedef enum _Q_ComCpuEndian
{
	eLittleEndian=1, //С�˶���
	eBigEndian=2     //��˶���
}Q_ComCpuEndian;

enum Q_COMM_LOG_TYPE
{
	eCommLogTypeTx=1,//������Ϣ
	eCommLogTypeRx=2,//���յ�����Ϣ
	eCommLogTypeError=3, //�������Ϣ
	eCommLogTypeMsg=4  //��Ϣֵ
};

//typedef struct Q_COMM_LOG_INFO
//{
//	Q_COMM_LOG_TYPE type;//��Ϣ����
//	char *msg;//��Ϣ����
//	int len;//��Ϣ����
//}Q_COMM_LOG_INFO;

//ת��ң�ⶨ��
typedef struct _Q_TRANS_ALGDATATAG
{
	int             nIndex;//id��
	Q_DataType		eDataType;//����
	float			fValue;
	int				nValue;
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];
	int             device;//ת��վ���
	int             no; //վ�����
	float           factor;//ϵ��
	int             sindex;//Դվ���
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
//ת��ң�Ŷ���
typedef struct _Q_TRANS_DIGDATATAG
{
	int             nIndex;//���
	bool			bValue;
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];
	int             device;//ת��վ���
	int             no; //ת��վ����վ�ڵ��
	int             sindex;//Դ���
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
//ͨ��״̬
typedef struct _Q_CHN_INFO
{
	int             nID;//ͨ��ID
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];//ͨ����
	int             nStatus;//ͨ��״̬ 0  �ر� 1����
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

//�ڵ���������
const BYTE eNodeTypeSlave=0;
const BYTE eNodeTypeMaster=1;
const BYTE eNodeStateOffline=0;
const BYTE eNodeStateRunning=1;
const BYTE eNodeStateStarting=2;
const BYTE eNodeError=1;//����
const BYTE eNodeNormal=0;
//���÷�ʽ
const BYTE eRunMethodCold=1;
const BYTE eRunMethodHot=2;
const BYTE eRunMethodStandalone=0;
//��Ϣ����
const BYTE eInfoTypeCommon=1;//�Զ�ͨ����Ϣ
const BYTE eInfoTypeAction=2;// �Զ˶�����Ϣ
//������ñ�
typedef struct _Q_NODE_INFO
{
	UINT             nID;//�����
	char			chName[Q_DATAMANAGER_TAGNAMELENGTH];//�����
	BYTE nRunMethod;//���з�ʽ1��������2�䱸������3�ȱ�������
	BYTE	     nNodeType;//�Ƿ������� 1Ϊ���� 0Ϊ����
	UINT         nDefaultMaster;//Ĭ����������
	BYTE         nStatus; //״̬0 offline,2������ 1����
	long         nTimeStamp;//ʱ��
	UINT         nCurNodeID;//��ǰ���id
	BYTE         nError;//�Ƿ����
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
	bool yt;//falseΪyk
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
//	SOCKET						m_socketClient;//�ͻ���
//	SOCKADDR_IN					m_addressClient;//�ͻ��˵�ַ
//	bool                        m_bPrimary;//�Ƿ�Ϊ��
//	int                         m_errCount;
//}Q_SocketClientInfo;

//2015-04-15 add ty tjs 
typedef enum _Protocol_VarType
{
	eModVarTypeNull=0,//δ֪����
	eModVarTypeYc=1,//ң��仯
	eModVarTypeYx=2,//ң�ű仯
	eModVarTypeEx=3 //������ֵ�仯
}Q_Protocol_VarType;

//���ڴ����ⲿִ��������
typedef struct _Protoco_ExecCmd
{
	int funcode;// ������
	int channelid;//ͨ�����
	Q_Protocol_VarType varType;//�仯����������
	int m_vindex;//�仯������,Ĭ���Ǵ��㿪ʼ��
	bool bzf;
	int zfchannelid;//ת��ͨ��id
	int zfdevice;//ת��վ���
	int zfno; //ת��վ�����
	Q_RECORDDATATAG m_tag;//��������
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
     



#define cpt_RTSERVER			(1 << 0)	//ϵͳ����
#define cpt_HISTORY				(1 << 1)	//��ʷת��
#define cpt_CHANNELOP			(1 << 2)	//ǰ��ͨѶ
#define cpt_Total        1 //�����̸���


#define mes_PROTCOL_PACKET			11  //��Լ����
#define mes_User_Logout				12  //�û�ע��

#define mes_Start_Application		21  //Զ������Ӧ�ó���
#define mes_End_Application			22  //Զ�̹ر�Ӧ�ó���,��Ӧ�ó���Ӧ����Ӧ�������ر�
#define mes_Sys_Exit				23  //Զ�̹رձ���ϵͳ,MCI������Ӧ��رձ�������Ӧ��
#define mes_Local_Exit				24  //�رձ���ϵͳ,��MCI����,����Ϣ���㲥

#define mes_Alarm_Acknowleged		31
#define mes_Alarm_DeleteItem		32
#define mes_Alarm_ClearAlarm        33

#define mes_Graph_YX_Change			41
#define mes_Change_RunConfig		42
#define mes_Change_PowerFlag		43
#define mes_ResetAllProtect			44//�����ܸ���
#define mes_TestBell				45//�������
#define mes_TestFlute				46//��Ѳ���

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

#define mes_SPSelect                74  //���add by tjs 2015-06-29
#define mes_SPExecute               75

//#define mes_MsgPrint                152  //��Լ��ӡ
#define mes_MsgExpand               82  //��չ��Ϣ����
#define mes_MsgSwitch               83  //�����л�

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

//2016-6-22 tcp server ����IPʹ��
typedef struct Q_LSrvPort
{
	char ip[30];//ip��ַ
	int nChID; //ͨ��ID��
	bool valid;//�Ƿ���Ч
	Q_LSrvPort()
	{
     valid=false;
	 nChID=0;
	}
}Q_LSrvPort;

#endif // !defined(__Q_FRAMEDEFINE_H_INCLUDED__)





