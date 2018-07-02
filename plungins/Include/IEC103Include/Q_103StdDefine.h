#pragma once
#if !defined(__Q_103STANDARD_DEFINE_H__INCLUDED__)
#define __Q_103STANDARD_DEFINE_H__INCLUDED__
/** 
 * @interface 
 * @brief IEC-60870-103 protocol define
 * @author ̷��ʤ TanJingSheng
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
// const BYTE  CMD_STARTDT_CON  =0x08;          //�������ݴ���ȷ��
// const BYTE  CMD_STOPDT		 =0x10;
// const BYTE  CMD_STOPDT_CON   =0x20;          //ֹͣ���ݴ���ȷ��
// const BYTE  CMD_TESTFR		 =0x40;
// const BYTE  CMD_TESTFR_CON	 =0x80;           //����ȷ��
// 
// const BYTE  CMD_ACT             =0x06;           //����
// const BYTE  CMD_ACT_CON         =0x07;           //����ȷ��
// const BYTE  CMD_DEACT           =0x08;           //��ֹ����
// const BYTE  CMD_DEACT_CON       =0x09;         //��ֹ����ȷ��
// const BYTE  CMD_DEACT_TERM      =0x0a;       //������ֹ  
// const BYTE  CMD_COMMON_ADDR_TMP =0x01; 
// const BYTE  CMD_KNOWN_INFO_ADDR =0x6f;//δ֪����Ϣ�����ַ6f����2f?
 const UINT MIN_103_FRAME_NUM=6;//��С֡��С
 const UINT MAX_103_REMOTE_NUM=64;//���ĶԶ˸���  ԭ��ֵΪ10  
  const UINT MAX_103_REMOTE_NUM_BASE=4;
const UINT MAX_103_PACKET_NUM=255;//����103���Ĵ�С
#define  MAX_RECIVE_PACKET_NUM 10240


typedef enum Q_IEC103_ErrorCode
{

	e103ErrNo_Suc=0,//�ɹ�
	e103ErrNo_WriteSend=1, //д�������������
	e103ErrNo_Link=2,//��·�Ͽ������
	e103ErrNo_P_UnExist=3, //ң�ص�ַ��Ӧ�ĵ������
	e103ErrNo_4=4,
	e103ErrNo_FrameNum=5, //֡�����Ի��ߴ������ֵ����С����Сֵ6
    e103ErrNo_RxOverTx=6, //�������к�Ӧ��С�ڽ������к�
	e103ErrNo_FrameSeq    =7,  //֡����������
	e103ErrNo_CmdListNull =8, //�����б�û�г�ʹ��
	e103ErrNo_CmdNoSel    =9,   //����û��ѡ���豸
	e103Errno_CmdSend     =10,    //��������
	e103Errno_UptDb       =11,//����ʵʱ��
	e103ErrNo_MisAsdu     =12, //�������ݷ���Ԫ��Ų���
	e103ErrNo_FrameVerify    =13,  //֡����ʧ��
	e103ErrNo_BeyondK    =14  //֡����ʧ��

};

typedef enum Q_IEC103CmdType
{
	e103Cmd_Null   =0, //��
	e103_CALL_ALL_DATA  =1, //���ٻ�
	e103_CALL_ONE_DATA     =2, //һ������		
	e103_CALL_TWO_DATA     =3, //��������
	e103_INIT_CHANNEL      =4, //��ʼ��
	e103CmdTypeC_CS     =5, //ʱ��ͬ��
    e103CmdTypeC_EI     =6, //
	e103CmdTypeC_RC     =7, //ң��
	e103CmdTypeTestFR   =8,   //������·
	e103CmdTypePlanCurve   =9,    //�ƻ�����
	e103CmdTypeYT   =10,   //ң��
	e103CmdTypeAnalogchange=11,//�仯ң���ϴ�
	e103CmdTypeYK   =12   //ң��
};
typedef enum Q_IEC103CmdState
{
	e103CmdStateNull         =0, //׼��
	e103CmdStateEnd          =1,//�������
	e103CmdStateYTSelect     =2,//ң��ѡ��
    e103CmdStateYTSelectCon    =3, //ң��ѡ����
	e103CmdStateYTExec       =4, //ִ��
	e103CmdStateYTExecCon    =5,//ִ�����ȷ��
	e103CmdStateYTCancel     =6,//����
	e103CmdStateYTCancelAct  =7, //����ȷ��
	e103CmdStateIntRequest   =8,//�ٻ�����
	e103CmdStateIntRequestAct=9,//�ٻ���Ӧ
	e103CmdStateIntSendData  =10,//�ٻ�����������
	e103CmdStateIntEnd       =11, //�ٻ����� 
	e103CmdStateIntSendDataFin =12, //���ٷ����������
	e103CmdStateSync         =13,//ͬ������
	e103CmdStateSyncAct      =14,//ͬ��ȷ��
	e103CmdStateYKSelect     =15,//ң��ѡ��
	e103CmdStateYKSelectCon  =16,//ң��ѡ����
	e103CmdStateYKExec       =17,//ִ��
	e103CmdStateYKExecCon    =18,//ִ���ѷ���
	e103CmdStateYKExecAct    =19,//ִ�����ȷ��
	e103CmdStateYKCancel     =20,//����
	e103CmdStateYKCancelAct  =21, //����ȷ��
};
typedef enum Q_IEC103_ST_Type
{
	e103Station_Unknown =0,   //δ֪վ��
	e103Station_Master  =1,  //��վ
	e103Station_Slave   =2    //��վ
}Q_IEC103_ST_Type;


//֡����
typedef enum Q_IEC103FrameType
{
	e103FrameType_Null=0,//��Ч
	e103FrameType_I=1,
	e103FrameType_U=2,
	e103FrameType_S=3
};
//У�鷵��ֵ
class   Q_IEC103vfyRet
{
public:
	 Q_IEC103FrameType m_eFrameType;
	bool m_bSuc;//�Ƿ�ɹ�
	WORD m_nErrorCode;//�����롣 
	Q_IEC103vfyRet(void)
	{
      m_eFrameType=e103FrameType_Null;
	  m_bSuc=true;
      m_nErrorCode=0;
	}
};
typedef enum Q_IEC103_VarType
{
	e103VarTypeNull=0,//δ֪����
	e103VarTypeYc=1,//ң��仯
	e103VarTypeYx=2,//ң�ű仯
	e103VarTypeEx=3 //������ֵ�仯
}Q_IEC103_VarType;
//���ڴ����ⲿִ��������
typedef struct Q_IEC103_ExecCmd
{
	int funcode;// ������
	Q_IEC103_VarType varType;//�仯����������
	int m_vindex;//�仯������,Ĭ���Ǵ��㿪ʼ��
	Q_RECORDDATATAG m_tag;//��������
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