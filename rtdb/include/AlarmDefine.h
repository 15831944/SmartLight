/////////////////////////////////////////////////////////////////////////////
// AlarmDefine.h : head file of rtdb alarm type define
// ˵��: ������Ҫ��ΪScadaAPI׼��,����RtdbDll����Ҫ���硢�ڵ���ϱ��������ñ�����ض���ŵ�RtdbDll��
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_ALARM_DEFINE_H__2005_03_10__
#define __AFX_ALARM_DEFINE_H__2005_03_10__
/////////////////////////////////////////////////////////////////////////////
typedef struct _tagAlarmEvent
{
	INT			lDegree;		// �����ȼ�
	INT			lMainType;		// ����������
	INT			lSubType;		// ����������
	SYSTEMTIME	tEventTime;		// ����ʱ��
	INT			StationID32;	// ��վ��ԪID
	INT			PointID32;		// �������ID
	CHAR		strDisplay[200];// ������ʾ����
} AlarmEvent;//�澯�¼�
/////////////////////////////////////////////////////////////////////////////
//** ���ȼ�����(2003-04-02):
#define AC_CLASS_SHOW					(1 << 0)//��ʾ����
#define AC_CLASS_PRINT					(1 << 1)//��ӡ����
#define AC_CLASS_SAVE					(1 << 2)//�洢����
#define AC_CLASS_VOICE					(1 << 3)//��������
#define AC_CLASS_BELL                   (1 << 4)//���屨��
#define AC_CLASS_FLUTE                  (1 << 5)//��ѱ���
#define AC_CLASS_MMI                    (1 << 6)//�ƻ���
#define AC_CLASS_FLASH                  (1 << 7)//��˸
#define AC_CLASS_AUTO                   (1 << 8)//�Զ�ȷ��
#define AC_CLASS_DELETE                 (1 << 9)//ȷ�Ϻ���ʾ
#define AC_CLASS_MESSAGE                (1 << 10)//���Ͷ���
/////////////////////////////////////////////////////////////////////////////
//** ������Ҫ���Ͷ���(2003-04-02):
#define AT_MAINTYPE_ANALOG				1		//ң��
#define AT_MAINTYPE_DIGIT				(1 << 1)//ң��
#define AT_MAINTYPE_YK					(1 << 2)//ң��
#define AT_MAINTYPE_YT					(1 << 3)//ң��
#define AT_MAINTYPE_SOE					(1 << 4)//SOE
#define AT_MAINTYPE_RELAY				(1 << 5)//����
#define AT_MAINTYPE_Foretel				(1 << 6)//�澯
#define AT_MAINTYPE_COMMUNITION			(1 << 7)//ͨѶ
#define AT_MAINTYPE_OTHER				(1 << 8)//����
#define AT_MAINTYPE_FAULT				(1 << 11)//����
#define AT_MAINTYPE_CBSTATE				(1 << 12)//�����޹ʱ�λ
//#define AT_MAINTYPE_SWITCHACTION		(1 << 9)//���ض���
/////////////////////////////////////////////////////////////////////////////
// ң�ⱨ��������(2003-04-02):
#define AT_ANALOG_OVER_HIGH_LIMIT		11	//ң��Խ����
#define AT_ANALOG_OVER_LOW_LIMIT		12	//ң��Խ����
#define AT_ANALOG_OVER_HIGH2_LIMIT		13	//ң��Խ������
#define AT_ANALOG_OVER_LOW2_LIMIT		14	//ң��Խ������
#define AT_ANALOG_ALARM_RESTORE			19	//ң�ⱨ���ָ�
/////////////////////////////////////////////////////////////////////////////
// ң�ű���������(2003-04-02):
#define AT_DIGIT_OPEN					41	//ң�ſ��ضϿ�
#define AT_DIGIT_CLOSE					42	//ң�ſ��رպ�
#define AT_DIGIT_OPEN_FAULT				43	//ң���¹���բ
/////////////////////////////////////////////////////////////////////////////
// ң�ر���������(2003-04-02):
#define AT_YK_SELECT					61	//ң��ѡ��
#define AT_YK_SELECT_OK					62	//ң��ѡ��ɹ�
#define AT_YK_SELECT_FAIL				63	//ң��ѡ��ʧ��
#define AT_YK_SELECT_CANCEL				64	//ң��ѡ����
#define AT_YK_CANCEL_OK					65	//ң�س����ɹ�
#define AT_YK_CANCEL_FAIL				66	//ң�س���ʧ��
#define AT_YK_EXECUTE					67	//ң��ִ��
#define AT_YK_EXECUTE_OK				68	//ң��ִ�гɹ�
#define AT_YK_EXECUTE_FAIL				69	//ң��ִ��ʧ��
/////////////////////////////////////////////////////////////////////////////
// SOE����������(2003-04-02):
#define AT_SOE_OPEN						81	//SOE�¼����ضϿ�
#define AT_SOE_CLOSE					82	//SOE�¼����رպ�
/////////////////////////////////////////////////////////////////////////////
//** ͨѶ����������(2003-04-02):
#define AT_COMM_RESUME					90	//ͨѶ�ָ�
#define AT_COMM_FAIL					91	//ͨѶʧ��
#define AT_COMM_TCP_FAIL				92	//TCPͨѶʧ��
#define AT_COMM_UDP_FAIL				93	//UDPͨѶʧ��
#define AT_COMM_COM_INITIALIZE_FAIL		94	//���ڳ�ʼ��ʧ��
#define AT_COMM_COM_OPERAT_FAIL			95	//���ڲ���ʧ��
#define AT_COMM_NET_1_FAIL				96	//NET_1�������
#define AT_COMM_NET_2_FAIL				97	//NET_2�������
/////////////////////////////////////////////////////////////////////////////
//** ��������������(2003-04-02):
#define AT_RELAY_BREAK_OPEN				101	//������բ
#define AT_RELAY_BREAK_CLOSE			102	//������բ
#define AT_RELAY_EVENT					103	//�����¼�
#define AT_RELAY_ALARM					104	//��������
#define AT_RELAY_GET_SET				105	//�õ�������ֵ
#define AT_RELAY_GET_SET_OK				106	//�õ�������ֵ
#define AT_RELAY_GET_SET_FAIL			107	//�õ�������ֵ
#define AT_RELAY_SET_SET				108	//�޸ı�����ֵ
#define AT_RELAY_SET_SET_OK				109	//�޸ı�����ֵ
#define AT_RELAY_SET_SET_FAIL			110	//�޸ı�����ֵ
#define AT_RELAY_YABAN_OPEN				111	//����ѹ����
#define AT_RELAY_YABAN_CLOSE			112	//����ѹ��Ͷ
#define AT_RELAY_START				113	//��������

#endif // __AFX_ALARM_DEFINE_H__2005_03_10__
