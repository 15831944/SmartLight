/****************************************************************************
 * ScadaAPI.h : SCADA�����������Ͷ���									*
 ****************************************************************************/
#ifndef __AFX_SCADA_DEFINE_H__
#define __AFX_SCADA_DEFINE_H__

#include "AlarmDefine.h"
#include "SCADA.h"
#include "MultiDataBase.h"
#include "NetStructDef.h"

/////////////////////////////////////////////////////////////////////////////
//RControl.iTaskType�Ķ���
typedef struct _tagRControl
{
	INT			iStation;		// Station index
	INT			iYKPoint;		// ң�ص��
	INT			iCommandType;	// ң��������
	INT			iVal;			// ң��Ŀ��ֵ
	INT         iDevice;        //�豸�� 2015-06-30 tjs 
	INT         iZf;//�Ƿ�Ϊת��
	INT         iYxID;//����ң��ID 2015-12-9 tjs 
	_tagRControl()
	{
		iZf=0;//0��ת��,1ת��
		iYxID=0;

	}
} RControl;//ң����Ϣ�ṹ��

//2015-06-29
typedef struct _tagRSetpoint
{
	INT			iStation;		// Station index
	INT			iPoint;		// ң�����
	INT			iCommandType;	// ң��������
	float		fVal;			// ң��Ŀ��ֵ
	INT         iDevice;
	INT         iZf;//�Ƿ�Ϊת��
	INT         iYcID;//����ң��ID 2015-12-9 tjs 
	_tagRSetpoint()
	{
		iZf=0;//0��ת��,1ת��
	}
} RSetpoint;//ң����Ϣ�ṹ��


//*****20150907��˶������ڹ�Լ��ӡ
typedef struct _tagRMsgPrint
{
	INT			iChannelID;	   // Station index
	INT			iMsgType;	   // ��ӡ����
	int         nlen;        //����
	BYTE	    msg[200];      // ����

}RMsgPrint;//��Լ��ӡ�ṹ��



/////////////////////////////////////////////////////////////////////////////
typedef struct _tagYXChange
{
        UINT			uElePointID;	// ң�ű�λ��
        BOOL			bOpen;		// �ֺ�
        BOOL			bFault;	        // ������բ
} YXChange;//ң�ű�λ��Ϣ�壬��ScadaAPI��ScadaServerʹ�ã�������բͳ��
/////////////////////////////////////////////////////////////////////////////
// ͨ�����Ͷ���(2003-04-02):
#define CHANNEL_TYPE_ASYNCOMM			1 //�첽����
#define	CHANNEL_TYPE_SYNCOMM			2 //ͬ������
#define	CHANNEL_TYPE_BUS422				3 //����422
#define	CHANNEL_TYPE_BUS485				4 //����485
#define	CHANNEL_TYPE_LONWORK			5 //Lonwork��
#define	CHANNEL_TYPE_PROFIBUS			6 //PROFI����
#define	CHANNEL_TYPE_TCPIPNET			7 //TCP/IP
/////////////////////////////////////////////////////////////////////////////
// ��Լ���Ͷ���(2003-04-02):
#define PROTOCOL_TYPE_PUNKNOW			-1
#define PROTOCOL_TYPE_IEC101			0
#define PROTOCOL_TYPE_IEC102			1
#define PROTOCOL_TYPE_IEC103			2
#define PROTOCOL_TYPE_IEC104			3
#define PROTOCOL_TYPE_DNP				4
#define PROTOCOL_TYPE_SCI1801			5
#define PROTOCOL_TYPE_CDT				6
#define PROTOCOL_TYPE_FM				7
#define PROTOCOL_TYPE_GPS				8
#define PROTOCOL_TYPE_PMODBUS			9
#define PROTOCOL_TYPE_PSV				10
#define PROTOCOL_TYPE_POLLING			11
#define PROTOCOL_TYPE_CSE				12
#define PROTOCOL_TYPE_VEC				13

#define PROTOCOL_TYPE_FDK_DCF5			601	//������չCDT-��������
#define PROTOCOL_TYPE_OLD_DCF5			602	//������չCDT-��������
#define	PROTOCOL_TYPE_QINGZHOU_CDT		603	//������չCDT-����
#define PROTOCOL_TYPE_HAINYANG_CDT		604	//������չCDT-����
/////////////////////////////////////////////////////////////////////////////
// �ɼ���Ԫ���Ͷ���(2003-04-02):
#define STATION_TYPE_ACTUAL				1	//ʵ�ʵ�Ԫ
#define	STATION_TYPE_CALCULATE			2	//���㵥Ԫ
#define	STATION_TYPE_TRANSMIT			3	//ת����Ԫ
#define	STATION_TYPE_MIMIC				4	//ģ�ⵥԪ
#define	STATION_TYPE_RTU				5	//Զ����Ԫ
#define	STATION_TYPE_FTU				6	//��絥Ԫ
#define	STATION_TYPE_PROTECT			7	//����װ��
/////////////////////////////////////////////////////////////////////////////
// ң�������Ͷ���(2003-04-02):
#define	Analog_Type_I						1			//	����
#define	Analog_Type_U						2			//	��ѹ
#define	Analog_Type_I_phase					3			// �����
#define	Analog_Type_P						4			//	�й�
#define	Analog_Type_I_line					5			// �ߵ���
#define	Analog_Type_I_zero					6			// �������
#define	Analog_Type_U_phase					7			// ���ѹ
#define	Analog_Type_Q						8			//	�޹�
#define	Analog_Type_U_zero					9		// �����ѹ
#define Analog_Type_3U0						27		//	3U0��ѹ
#define Analog_Type_DWH						28		//	��ѹ��λ
#define Analog_Type_DWM						29		//	��ѹ��λ
#define	Analog_Type_F						16			//	�ܲ�
#define	Analog_Type_T						32			//	�¶�
#define	Analog_Type_C						64			//	��������
#define	Analog_Type_O						128		//	����

#define	Analog_Type_IA						(1 << 8)		//	����A��
#define	Analog_Type_IB						(1 << 9)		//	����B��
#define	Analog_Type_IC						(1 << 10)		//	����C��
#define	Analog_Type_UAB						(1 << 11)		//	��ѹAB��
#define	Analog_Type_UBC						(1 << 12)		//	��ѹBC��
#define	Analog_Type_UCA						(1 << 13)		//	��ѹCA��
#define	Analog_Type_UA						(1 << 14)		//	��ѹA��
#define	Analog_Type_UB						(1 << 15)		//	��ѹB��
#define	Analog_Type_UC						(1 << 16)		//	��ѹC��

#define	Analog_Type_U_line					(1 << 17)	// �ߵ�ѹ
#define	Analog_Type_Ux  			        (1 << 17)	//   �ߵ�ѹUx
#define	Analog_Type_Iha     				(1 << 18)		//
#define	Analog_Type_Ima        				(1 << 19)	//
#define	Analog_Type_Ila      				(1 << 20)	//
#define	Analog_Type_Ph       				(1 << 21)	//
#define	Analog_Type_Pm    					(1 << 22)	//
#define	Analog_Type_Pl    				    (1 << 23)	//
#define	Analog_Type_Qh    				    (1 << 24)	//
#define	Analog_Type_Qm       				(1 << 25)	//
#define	Analog_Type_Ql      				(1 << 26)	//
#define	Analog_Type_T1      				(1 << 27)	//
#define	Analog_Type_T2       				(1 << 28)	//
#define	Analog_Type_T3       				(1 << 29)	//
#define	Analog_Type_T4      				(1 << 30)	//

/////////////////////////////////////////////////////////////////////////////
// ң������λ���Ͷ���(2003-04-02):
#define	Analog_Unit_NONE					0		// û�е�λ
#define	Analog_Unit_A						1		// ����,A
#define	Analog_Unit_KA						2		// ǧ��,kA
#define	Analog_Unit_V						11		// ����,V
#define	Analog_Unit_KV						12		// ǧ��,kV
#define	Analog_Unit_W						21		// ����,W
#define	Analog_Unit_KW						22		// ǧ��,kW
#define	Analog_Unit_MW						23		// ����,MW
#define	Analog_Unit_Var						31		// ��,Var
#define	Analog_Unit_KVar					32		// ǧ��,kVar
#define	Analog_Unit_MVar					33		// �׷�,MVar
#define	Analog_Unit_Hz						41		// ����,Hz
#define	Analog_Unit_O						51		// ��,o
/////////////////////////////////////////////////////////////////////////////
// ң�������Ͷ���(2003-04-02):
//1-99  ����λ����Ϣ

#define	Digit_Type_DLQ_1					1		// ��·����λ
#define	Digit_Type_DLQ_0					2		// ��·����λ
#define	Digit_Type_DZ_1						3		// ��  բ��λ
#define	Digit_Type_DZ_0						4		// ��  բ��λ
#define	Digit_Type_SC_1						5		// ��  ������λ
#define	Digit_Type_SC_0						6		// ��  ��ʵ��λ
#define	Digit_Type_JDDZ_1					7		// �ӵص�բ��λ
#define	Digit_Type_JDDZ_0					8		// �ӵص�բ��λ
#define	Digit_Type_YaBan					21		// ѹ��״̬		//1����Ͷ��
#define	Digit_Type_LOCAL					31		// �͵�			//0����Զ��

//100-299 ���ڱ�����Ϣ
#define	Digit_Type_RELAY					100		// һ�㱣���źţ����ࣩ
#define	Digit_Type_Fault					101		// �¹���
#define	Digit_Type_Current1					111		// ����һ��
#define	Digit_Type_Current2					112		// ��������
#define	Digit_Type_Current3					113		// ��������
#define	Digit_Type_Current4					114		// �����Ķ�
#define	Digit_Type_Current5					115		// ��ʱ�޹���������
#define	Digit_Type_Current01				121		// ����һ��
#define	Digit_Type_Current02				122		// �������
#define	Digit_Type_Current03				123		// ��������
#define	Digit_Type_Current04				124		// �����Ķ�
#define	Digit_Type_Current05				125		// ��ʱ�����򱣻�
#define	Digit_Type_DISTANCE1				131		// ���뱣��һ��
#define	Digit_Type_DISTANCE2				132		// ���뱣������
#define	Digit_Type_DISTANCE3				133		// ���뱣������
#define	Digit_Type_DISTANCE4				133		// ���뱣���Ķ�
#define	Digit_Type_DISTANCEN1				141		// �ӵؾ��뱣��һ��
#define	Digit_Type_DISTANCEN2				142		// �ӵؾ��뱣������
#define	Digit_Type_DISTANCEN3				143		// �ӵؾ��뱣������
#define	Digit_Type_DISTANCEN4				143		// �ӵؾ��뱣���Ķ�
#define Digit_Type_LGas						151		// ����˹����
#define Digit_Type_HGas						152		// ����˹����
#define Digit_Type_HHLoad					153		// ��ѹ�����ɱ���
#define Digit_Type_MHLoad					154		// ��ѹ�����ɱ���
#define Digit_Type_LHLoad					155		// ��ѹ�����ɱ���
#define Digit_Type_Loil						156		// ����λ����
#define Digit_Type_HDiff					161		// �ݲ��
#define Digit_Type_VDiff					162		// ����
#define Digit_Type_BusDiff					163		// ĸ�߲
#define Digit_Type_Direct					164		// �����Ƶ
#define Digit_Type_FxDirect					165		// ��Ƶ����������
#define Digit_Type_PhaseDiff				166		// ����Ƶ
#define Digit_Type_ILock_LOWU				181		// ���������͵�ѹ
#define Digit_Type_ULock_I                  182		// ��ѹ��������
#define Digit_Type_LowVolf					183		// �͵�ѹ����
#define Digit_Type_Loss						191		// ʧ�鱣��
#define Digit_Type_JDXX						199		// С�����ӵ�ѡ��


//����250��Ϊ�غ�բ���Զ�װ��
#define Digit_Type_SPhaseZCH				251		// �����غ�բ
#define Digit_Type_TPhaseZCH				252		// �����غ�բ
#define Digit_Type_MPhaseZCH				253		// �ۺ��غ�բ
#define Digit_Type_BZT0						261		// ����Ͷ1
#define Digit_Type_BZT1						262		// ����Ͷ2
#define Digit_Type_VoltOpSET				271		// ��ѹ����
#define Digit_Type_FreqOpSET				272		// Ƶ�ʶ���
#define Digit_Type_FlowOpSET				273		// �и���

#define Digit_Type_AllFault					280		// ȫվ�¹���

//300-499 ���ڸ澯
#define	Digit_Type_Foretell					300		//  Ԥ���źţ����ࣩ
#define	Digit_Type_PT						301		//  PT����			//1����
#define	Digit_Type_SPRING					302		//  ����δ����		//1δ����
#define	Digit_Type_Comm						303		//  װ��ͨѶ״̬	//1�ж�
#define	Digit_Type_NodeWork					331		//  ������ڵ�����״̬	//1����
#define	Digit_Type_Net1						332		//  ������ڵ�����1����״̬	//1����
#define	Digit_Type_Net2						333		//  ������ڵ�����1����״̬	//1����
#define	Digit_Type_Channel					334		//  ͨ������״̬	//1����

//500-599 ����
#define	Digit_Type_Other					500		// ����

/////////////////////////////////////////////////////////////////////////////
// ���������Ͷ���(2003-04-02):
#define	Pulse_Type_P						1		// �й����
#define	Pulse_Type_Q						(1 << 1)// �޹����
/////////////////////////////////////////////////////////////////////////////
// ���ƽʱ�����Ͷ���(2003-04-02):
#define Period_Type_SharpPeak				1		// ���ʱ��
#define Period_Type_Peak					(1 << 1)// ��ʱ��
#define Period_Type_Vale					(1 << 2)// ��ʱ��
#define Period_Type_Flat					(1 << 3)// ƽʱ��
/////////////////////////////////////////////////////////////////////////////
// ң������̬��־����(2003-04-02):
#define Analog_Scan_Enable					1			//	ɨ��ʹ��
#define Analog_Alarm_Enable					(1 << 1)	//	����ʹ��
#define Analog_Statistic_Enable				(1 << 2)	//	ң��ͳ��ʹ��
#define Analog_Absolute_Enable				(1 << 3)	//	����ֵʹ��
#define Analog_Calculate_Enable				(1 << 4)	//	����ʹ��
#define Analog_OtherSide_Enable				(1 << 5)	//	�Զ�ʹ��
#define Analog_Replace_Enable				(1 << 6)	//	��·ʹ��
#define Analog_RecordChange_Enable			(1 << 7)	//	�仯������¼ʹ��
#define Analog_Curve_Enable					(1 << 8)	//	����ʹ��
#define Analog_Reverse_Enable				(1 << 9)	//	ȡ��ʹ��
#define Analog_ManSet						(1 << 10)	//	�˹�����
#define Analog_Accident_Flag    			(1 << 11)	//	�¹ʶ����
#define Analog_Integral_Enable    			(1 << 12)	//	���ֵ��ʹ��
#define Analog_Dispatch_DisEnable    		(1 << 13)	//	���Ƚ�ֹ����
#define	Analog_Repair_Flag					(1 << 14)	//  ����״̬��־
#define Analog_RUNTIME_Enable				(1 << 15)	//  ��ʱ����ʹ��
#define Analog_RUNNUM_Enable				(1 << 16)	//  �ƴα���ʹ��
#define Analog_OldData_Enable				(1 << 17)	//  �����ݱ���ʹ��
#define Analog_DelayAlarm_Enable			(1 << 18)	//  ��ʱ����ʹ��

#define Analog_DelayAlarm_Run				(1 << 25)	//  ��ʱ��������
/////////////////////////////////////////////////////////////////////////////
// ң������̬��־����(2003-04-02):
#define Analog_Alarm_Flag					1			//	������־
#define Analog_OverHigh_Flag				(1 << 1)	//	Խ���ޱ�־
#define Analog_BelowLow_Flag				(1 << 2)	//	Խ���ޱ�־
#define Analog_OverHighHigh_Flag			(1 << 3)	//	Խ�����ޱ�־
#define Analog_BelowLowLow_Flag				(1 << 4)	//	Խ�����ޱ�־
#define Analog_DeadData_Flag    			(1 << 5)	//	�����ݱ�־
#define Analog_OldData_Flag    				(1 << 6)	//	�����ݱ�־
/////////////////////////////////////////////////////////////////////////////
// ң������־λ����(2003-04-02):
#define	Digit_Scan_Enable					1			//	ɨ��ʹ��
#define	Digit_Alarm_Enable					(1 << 1)	//	����ʹ��
#define	Digit_Control_Enable				(1 << 2)	//	ң��ʹ��
#define	Digit_Reverse_Enable				(1 << 3)	//	ȡ��ʹ��
#define	Digit_Calculate_Enable				(1 << 4)	//	����ʹ��
#define	Digit_OpenSum_Enable				(1 << 5)	//  ������ͳ��ʹ��
#define	Digit_CloseSum_Enable				(1 << 6)	//  �ϴ���ͳ��ʹ��
#define	Digit_LastCheck_Enable				(1 << 7)	//  ȥ��ʹ��
#define	Digit_AccidentJudge_Enable			(1 << 8)	//  �¹��ж�ʹ��
#define	Digit_Repair_Flag					(1 << 9)	//  ����״̬��־
#define Digit_ManSet_Flag					(1 << 10)	//  �˹�����״̬��־
#define Digit_Statistic_Enable				(1 << 11)	//  ����ʱ��ͳ��ʹ��
#define Digit_Dispatch_DisEnable    		(1 << 13)	//	���Ƚ�ֹ����
#define Digit_RUNTIME_Enable				(1 << 12)	//  ��ʱ����ʹ��
#define Digit_RUNNUM_Enable					(1 << 14)	//  �ƴα���ʹ��
#define Digit_0_Enable						(1 << 15)	//  ��0������־
#define Digit_DelayAlarm_Enable				(1 << 16)	//  ��ʱ����ʹ��

#define Digit_WINK_Flag						(1 << 25)	//  ��˸��־
#define Digit_DelayAlarm_Run				(1 << 26)	//  ��ʱ��������

/////////////////////////////////////////////////////////////////////////////
// ��������־λ����(2003-04-02):
#define	Pulse_Scan_Enable					1			//	ɨ��ʹ��
#define	Pulse_Calculate_Enable				(1 << 2)	//	����ʹ��
#define	Pulse_Number_Enable					(1 << 3)	//	һ�βɼ�������ʹ��
#define	Pulse_Energy_Enable					(1 << 4)	//	һ�βɼ�����ʹ��
#define	Pulse_MeterNumber_Enable			(1 << 5)	//	���������ʹ��
#define	Pulse_MeterEnergy_Enable			(1 << 6)	//	��ǵ���ʹ��
#define	Pulse_PVF_Enable					(1 << 7)	//	�շ��ƽ����ͳ��ʹ��
#define Pulse_ManSet						(1 << 8)	//	�˹�����
#define Pulse_OtherSide_Enable				(1 << 9)	//	�Զ˴���ʹ��
#define Pulse_Replace_Enable				(1 << 10)	//	��·����ʹ��
#define Pulse_Huanbiao_Enable				(1 << 11)	//	����ʹ��
/////////////////////////////////////////////////////////////////////////////
// ң�ر�־λ����(2005-01-16):
#define Relay_RCing_Flag					(1<<0)	//ң�ؽ�����
#define Relay_Disable_Flag					(1<<1)	//��ֹң��
#define Relay_Maintenance_Flag				(1<<2)	//ң����Ȩά��
#define Relay_NoSoftJudge_Flag				(1<<3)	//����������
#define Relay_TAPError_Flag					(1<<4)	//�������� 

#define Relay_DoublePos_Flag				(1<<10)	//˫λң��

// ң�����Ͷ���
#define Relay_iType_CB						1		//���ص�բ
#define Relay_iType_TRFMUP					2		//��ѹ����
#define Relay_iType_TRFMDOWN				7		//��ѹ����
#define Relay_iType_TRFMSTOP				8		//��ѹ��ͣ
#define Relay_iType_YABAN					3		//ѹ��
#define Relay_iType_RESET					4		//����
#define Relay_iType_DZ						5		//��ֵ
#define Relay_iType_DZZONE					6		//��ֵ��
/////////////////////////////////////////////////////////////////////////////
// ��·��·��־λ����
#define PLD_Use_Flag						(1<<0)	//ʹ��
#define PLD_Topo_Flag						(1<<1)	//����ά�� 
#define PLD_Analog_Flag						(1<<2)	//ң������
#define PLD_Pulse_Flag						(1<<3)	//�������
/////////////////////////////////////////////////////////////////////////////
// ��ʽ���־λ����(2003-04-02):
#define	Formula_Disable						1			//	��ʽ����
#define	Formula_Enable						2			//	��ʽʹ��

#define	PDR_q_Disable						1			//	PDR����
#define	PDR_q_HistorySave					4			//	��ʷ���洦����
#define	PDR_q_Value							8			//  PDR����ֵ	

// ȫ�ֱ��־
#define	Global_q_RepairAlarmDisable			1			//	�����в�����
#define	Global_q_PLDTopoEnable				(1<<1)		//	�Զ���·��

//һ����������¹���բʱ����˸,Ϊ�������е��û��Կ��ر�λ����˸��Ҫ������Ӵ˱�־
#define	Global_q_BreakChangeWink			(1<<2)		//	���ر�λ����˸


/////////////////////////////////////////////////////////////////////////////
#define MAX_ANALOG_PACKET_NUM		32
#define MAX_DIGIT_PACKET_NUM		32
#define MAX_PULSE_PACKET_NUM		32
/////////////////////////////////////////////////////////////////////////////
typedef struct _tagAnalogPacket
{
	INT			iAnalogIndex[MAX_ANALOG_PACKET_NUM];	// ��Ӧ��ң�����е�����
	INT			iAnalogID[MAX_ANALOG_PACKET_NUM];		// ��Ӧ��ң�����е�ID32
	FLOAT		fAnalogRaw[MAX_ANALOG_PACKET_NUM];		// ң��ԭʼֵ //ԭ����int���͵��ָ���Ϊfloat����
	INT			iAnalogNum;								// Ҫ���µ�ң����
} AnalogPacket;
/////////////////////////////////////////////////////////////////////////////
typedef struct _tagDigitPacket
{
	INT			iDigitIndex[MAX_DIGIT_PACKET_NUM];		// ��Ӧ��ң�Ŵ���е�����
	INT			iDigitID[MAX_DIGIT_PACKET_NUM];			// ��Ӧ��ң�Ŵ���е�ID32
	INT			iDigitRaw[MAX_DIGIT_PACKET_NUM];		// ң��ԭʼֵ
	INT			iDigitNum;								// Ҫ���µ�ң����
} DigitPacket;
/////////////////////////////////////////////////////////////////////////////
typedef struct _tagPulsePacket
{
	INT			iPulseIndex[MAX_PULSE_PACKET_NUM];		// ��Ӧ�ڵ�ȴ���е�����
	INT			iPulseID[MAX_PULSE_PACKET_NUM];			// ��Ӧ�ڵ�ȴ���е�ID32
	INT			iPulseRaw[MAX_PULSE_PACKET_NUM];		// ���ԭʼֵ[ֱ�ӵ�ַ����]
	INT			iPulseNum;								// Ҫ���µĵ����
} PulsePacket;
/////////////////////////////////////////////////////////////////////////////
#define	Channel_NoWork						1			//	û�й���
#define	Channel_MainChannelWork				(1 << 1)	//	��ͨ������
#define	Channel_BackChannelWork				(1 << 2)	//	��ͨ������
#define	Channel_MainFMWork					(1 << 3)	//	��ǰ�ù���
#define	Channel_BackFMWork					(1 << 4)	//	��ǰ�ù���
/////////////////////////////////////////////////////////////////////////////
// �¹��о����Ͷ���(2003-04-02):
#define FAULT_TYPE_NULL						0	//�����¹���բ�ж�
#define FAULT_TYPE_TOTAL					1	//ֻ��վ���¹����ź�
#define FAULT_TYPE_PROTECTION				2	//ֻ�й��������ź�
#define FAULT_TYPE_TOTAL_AND_POROTECTION	3	//���¹����ź��뱣���ź�
#define FAULT_TYPE_TOTAL_OR_PROTECTION		4	//���¹����źŻ򱣻��ź�
/////////////////////////////////////////////////////////////////////////////
// ת�������Ͷ���(2003-04-02):
#define TRANSMIT_TYPE_ANALOG				0	//ң����
#define TRANSMIT_TYPE_DIGIT					1	//ң����
#define TRANSMIT_TYPE_PULSE					2	//�����

#define LabelType_q_Repair					1	//������
#define LabelType_q_JD						2	//�ӵ���
#define LabelType_q_Tip						4	//��ʾ��
#define LabelType_q_DisenableRC				8	//��ֹң��

#define RCConfig_q_bWatch					1	//����˫ϯң��
#define RCConfig_q_BHDZ						2	//������������ң��
#define RCConfig_q_SAME						4	//Ŀ��λ�͵�ǰֵһ��Ҳ��ң��
#define RCConfig_q_FGDisplay				8	//������ʾң�ؽ���
#define RCConfig_q_TRFMDisplay				16	//��ѹ��ң����ʾң�ؽ���
#define RCConfig_q_10kVLoop					32	//10kV���ܺϻ�
#define RCConfig_q_35kVLoop					64	//35kV���ܺϻ�
#define RCConfig_q_110kVLoop				128	//110kV���ܺϻ�
#define RCConfig_q_220kVLoop				256	//220kV���ܺϻ�
#define RCConfig_q_330kVLoop				512	//330kV���ܺϻ�
#define RCConfig_q_500kVLoop				1024//500kV���ܺϻ�
#define RCConfig_q_NoAutoClose			2048//�ɹ�ң�غ���治�Զ��˳�
#define RCConfig_q_FGNoFault			4096//û�й���Ҳ�ܸ���װ��
#define RCConfig_q_NoSoftJudge			8192//����������
#define RCConfig_q_DispatchNO			16384//У����ȱ��
/////////////////////////////////////////////////////////////////////////////
#define eAnalogUnitTypeNull 0
#define eAnalogUnitTypekW 1 
#define eAnalogUnitTypeMW 2
#define eAnalogUnitTypekVar 3
#define eAnalogUnitTypeMVar 4
#define eAnalogUnitTypeA   5
#define eAnalogUnitTypekV  6
#define eAnalogUnitTypeHz  7
#define eAnalogUnitTypekVA  11  
#define eAnalogUnitTypeV   12
#define eAnalogUnitTypeW   13
#define eAnalogUnitTypekWh  14  //2016-03-04 
#define eAnalogUnitTypeMWh  15  
#define eAnalogUnitTypekVarh   16
#define eAnalogUnitTypeMVarh   17
#endif // __AFX_SCADA_DEFINE_H__

