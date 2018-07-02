/////////////////////////////////////////////////////////////////////////////
// Rtdb_Struct.h : ����ʹ�ô˶�̬�������ͷ�ļ�������ԭ��
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_RTDB_STRUCT_H__
#define __AFX_RTDB_STRUCT_H__

#include "gSystemDef.h"
/////////Ʒ��////////////////////////////////
#define Quality_Good 0
#define Quality_Bad 1

/////////////////////////////////////////////////////////////////////////////
struct MDB_MEM_OFFSET
{
	// ��ʾm_pDataBase ��������ڹ����ڴ���ƫ����
	UINT	DbInfoOffset;
	// ��ʾ���ݿ�������������BOOL�����ڹ����ڴ���ƫ����
	UINT	DbInfoIndexOffset;
	// ��Ϣ�����ڹ����ڴ���ƫ����
	UINT	TabMessageOffset;
	// �����̶���Ϣ�����λ���ڹ����ڴ���ƫ����
	UINT	ReadMessagePosOffset;
	// ��Ϣ���Ӷ����ڹ����ڴ���ƫ����
};
/////////////////////////////////////////////////////////////////////////////
#define MESBODY_MAX_LENGTH	256
struct tabMessage
{
	UINT	m_uMsgID;			// ���ʹ��������Ϣ��ȫ��Ψһ����ϢID
	UINT	m_uTargetID32;		// ������Ϣ�Ľ��̵�ID32(0��ʾ�㲥��ָ�����͵ĳ���)
	UINT	m_uSourceID32;		// ������Ϣ�Ľ��̵�ID32
	INT		m_iProcessType;		// ������Ϣ�Ľ�������(�����Ƕ�����̵����)
	INT		m_iMsgType;			// ��Ϣ����
	INT		m_iMsgSize;			// ��Ϣ�峤��
	BOOL	m_bBodyNeedChange;	// ��Ϣ���Ƿ���ҪӦ�ý����ֽ�˳��ת��
	// ��Ϣ��(����Ϣ��Ӧ��������m_bBodyNeedChange�ж��Ƿ�Ҫ����Ϣ������ֽ�˳��ת��)
	BYTE	m_bMes[MESBODY_MAX_LENGTH];
};
struct ProcessMessageInfo
{
	UINT	iWritePos;
	UINT	iReadPos;
};
/////////////////////////////////////////////////////////////////////////////
// �����ǽڵ�����(SYSTEM_NetNode->iType)�ĺ궨��:
#define nt_MainServer			(1<<0)	//��������
#define nt_BackServer			(1<<1)	//��������
#define nt_MainFEP				(1<<2)	//��ǰ�û�
#define nt_BackFEP				(1<<3)	//��ǰ�û�
#define nt_Client				(1<<4)	//����վ

#define nt_AutoLogout			(1<<10)	//�����Զ�ע��

//����������������Ϣ��ȫ�����ֻ��һ���ڵ㷢��
#define nt_JavaUpdateMsg		(1<<11)	//�򱾻�Java���͸�����Ϣ
#define nt_WebServer			(1<<12)	//Web����
#define nt_AlarmSound			(1<<21)	//����������
#define nt_AlarmPrint			(1<<22)	//��������ӡ
#define nt_AlarmAuto			(1<<23)	//�������Զ�ȷ��
#define nt_AlarmGraph			(1<<24)	//�������ƻ���
#define nt_AlarmPut				(1<<25) //�������ƴ���
#define nt_AlarmBell			(1<<26) //�������
#define nt_AlarmFlute			(1<<27) //������


/////////////////////////////////////////////////////////////////////////////
// �����ǽ�������(tabProcess->p_iType)�ĺ궨��:
// ��ע: Ϊ�˱���ͬʱ��������̷�����Ϣ,����Ľ������Ͳ��ñ�־λ
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define pt_RTSERVER				(1 << 0)	//ϵͳ����
#define pt_HISTORY				(1 << 1)	//��ʷת��
#define pt_CHANNELOP			(1 << 2)	//ǰ��ͨѶ
#define pt_ScadaServer			(1 << 3)	//SCADA����
#define pt_GRAPH				(1 << 4)	//ͼ�ν��� //src =7
#define pt_ALARM				(1 << 5)	//����
#define pt_AGC			(1 << 6)	//AGC�㷨
#define pt_AVC			(1 << 7)	//AVC�㷨
#define pt_PSCS			(1 << 8)	//�ȶ������㷨
#define pt_filetran		(1 << 9)	//�ļ�����
#define pt_DBGSVR			    (1 << 10)	//���ݴ��������dbgsvr����Լ��ӡ����

//#define pt_RTDBEDIT				(1 << 4)	//ʵʱ��ά��

//#define pt_PDR					(1 << 6)	//�¹ʼ�¼

//#define pt_MCI					(1 << 8)	//���ؽ���
//#define pt_GRAPH_EDIT			(1 << 9)	//ͼ�α༭
//#define pt_DTS_Console			(1 << 10)	//DTS����̨
#define pt_Java_Server			(1 << 11)	//JAVAϵͳ����
//#define pt_PAS_Server			(1 << 12)	//PAS����̨
//#define pt_Alarm_Mobile			(1 << 13)	//����ֵ��
//#define pt_DBGSVR			    (1 << 14)	//���ݴ��������dbgsvr����Լ��ӡ����

#define pt_Total				16	//�ܽ�����
/////////////////////////////////////////////////////////////////////////////
// �����ǽ�������״̬(tabProcess->p_iStatus)�ĺ궨��:
#define ps_Run		( 1 << 1 )
#define ps_Main		( 1 << 2 )
#define ps_FakeDel	( 1 << 3 )
#define ps_Exit		( 1 << 4 )
#define ps_Switch	( 1 << 5 )
//#define ps_Tcp		( 1 << 6 )
/////////////////////////////////////////////////////////////////////////////
// ����ڵ���Ϣ�洢��Ԫ
typedef struct _tagNodeInfo
{
	CHAR		m_strID[24];
	CHAR		m_strIP1[16];
	CHAR		m_strIP2[16];
} NODE_INFO;
/////////////////////////////////////////////////////////////////////////////
#define MESSAGE_MAX_NUM			3000
//#define PROCESS_MAX_NUM			400//ע������Ķ���һ��Ҫ�ͽ��̱�������С������ͬ
#define DATABASE_MAX_NUM		10
/////////////////////////////////////////////////////////////////////////////
#define NET_1		1 // һ��
#define NET_2		2 // ����
#define NET_ALL		3 // ˫��
/////////////////////////////////////////////////////////////////////////////
class CMultiDataBase;
/////////////////////////////////////////////////////////////////////////////
// define transmit type

#define MAX_TRANSMIT_TYPE	6		//ϵͳ֧�ֵ�tcpͨ���� //��ʱ����cpuռ���ʹ��ߵ�����
//#define MAX_SERVER_NUM		4		//ϵͳ֧�ֵķ���������,�������以Ϊ����
#define MAX_SERVER_NUM		2		//ϵͳ֧�ֵķ���������,�������以Ϊ����
#define TT_SYS_UDP		0 // ϵͳUDPͨ��(����ϵͳ����״̬������;)
#define TT_TRANSMIT		1 // ���д���ͨ��(��ʼ��������ʱ��ʵʱ������λ�Ĵ���)
#define TT_MESSAGE		2 // ϵͳ��Ϣ����
#define TT_UPDATE		3 // ���ݸ���ͨ��(���ڰ���ϵͳ�����ڵ����б����ݵĸ���)
#define TT_TCPNOTIFY	4 // ������Ϣ(����TCP�������¼���ʽ֪ͨ����)
#define TT_EXTEND_1		5 // ���ݴ���ĵ�2��Ԥ������

#define TT_WEB			20 // web����

extern char pszChannelName[6][15];
/////////////////////////////////////////////////////////////////////////////
#ifdef WINDOWS_ENV
	extern __declspec( dllexport ) bool SendCommand(int nCommand, char * pBuffer, int nBufferSize, int nSendType = TT_UPDATE);
	extern __declspec( dllexport ) bool SendWebCommand(int nCommand, char * pBuffer, int nBufferSize);
	extern __declspec( dllexport ) void	ForceBroadcast(int nCommand,char * pBuffer,int nBufferSize);
	extern __declspec( dllexport ) void SetDefaultField(int nDataType,char * pFieldData,char * pszDefault);
#endif
#ifdef UNIX_ENV
	extern "C" bool SendCommand(int nCommand, char * pBuffer, int nBufferSize, int nSendType = TT_UPDATE);
	extern "C" bool SendWebCommand(int nCommand, char * pBuffer, int nBufferSize );
	extern "C" void ForceBroadcast(int nCommand,char * pBuffer,int nBufferSize);
	extern "C" void SetDefaultField(int nDataType,char * pFieldData,char * pszDefault);
#endif
/////////////////////////////////////////////////////////////////////////////

#endif // __AFX_RTDB_STRUCT_H__
