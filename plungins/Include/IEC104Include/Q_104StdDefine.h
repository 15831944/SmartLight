
#if !defined(__Q_104STANDARD_DEFINE_H__INCLUDED__)
#define __Q_104STANDARD_DEFINE_H__INCLUDED__
/** 
 * @interface 
 * @brief IEC-60870-104 protocol define
 * @author ̷��ʤ TanJingSheng
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
	eCmdStateYkConfirm    =15  //ң��ȷ��

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
//del ty tjs 2015-04-15 ,��ȥ��m_Block;
//typedef enum Q_IEC104_VarType
//{
//	eVarTypeNull=0,//δ֪����
//	eVarTypeYc=1,//ң��仯
//	eVarTypeYx=2,//ң�ű仯
//	eVarTypeEx=3 //������ֵ�仯
//}Q_IEC104_VarType;
////���ڴ����ⲿִ��������
//typedef struct Q_IEC104_ExecCmd
//{
//	int funcode;// ������
//	Q_IEC104_VarType varType;//�仯����������
//	int m_vindex;//�仯������,Ĭ���Ǵ��㿪ʼ��
//	Q_RECORDDATATAG m_tag;//��������
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