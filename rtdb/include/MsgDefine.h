/////////////////////////////////////////////////////////////////////////////
// MsgDefine.h : head file of rtdb msg type define
// ˵��: 1��10Ϊϵͳ��������,����Ӧ�ý�ֹʹ��
// ע��: message.def�ж������Ϣ��200��ʼ,Ϊ�����message.def,���ﶨ�����Ϣ��Ҫ����200��
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_MESSAGE_DEFINE_H__2005_03_10__
#define __AFX_MESSAGE_DEFINE_H__2005_03_10__

#define SYSTEM_MAX_MESSAGE			200

/////////////////////////////////////////////////////////////////////////////
// ϵͳ��������,������ܽ����Ǹ���,��ʵ�������д������������
#define mes_Map_Change				1  //�����ڴ��Ѿ��޸�,Java��RtServerӦ��Ӧ(��Ϣ��:nType,nDbID,nTabID,nRecordIndex)

/////////////////////////////////////////////////////////////////////////////
#define mes_User_Login				11  //�û���¼
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

#define mes_Transmit_Change			81

#define mes_PAS_Net2Pas 			100	//ͬ��Net����Ϣ(У��Net��,copy��Pas��)
#define mes_PAS_Part_Net2Pas 		101	//����ͬ��Net�����������Ϣ(У�鲢ͬ�����ֲ�����Pas��)
#define mes_PAS_StartCal			102 //�����ڼ�����Ϣ
#define mes_PAS_StopCal				103 //ͣ���ڼ�����Ϣ
#define mes_PAS_CalSE			 	104	//����se����
#define mes_PAS_CalPF			 	105	//����PF����
#define mes_PAS_CalCA			 	106	//����CA����
#define mes_PAS_CalSCC			 	107	//����SCC����

#define mes_Fep_Reload              111
#define mes_ScadaServer_YXChange    120    //ң�ű�λ

#define mes_ToFMRawPacket			150		//Ӧ�ó�������֯�õ��·�����,ǰ�ý����·�����
#define mes_FromFMRawPacket			151		//ǰ���յ���ֱ�Ӵ���ı���,ת��Ӧ�ó���,����س����Լ�����
#define mes_MsgPrint                152 //��ӡͨѶ��Լ�����Ϣʹ��
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_MESSAGE_DEFINE_H__2005_03_10__
