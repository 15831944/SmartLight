/****************************************************************************
 * ScadaAPI.h : SCADA�������ʽӿ��ඨ��									*
 ****************************************************************************/
#ifndef __AFX_SCADA_API_H__
#define __AFX_SCADA_API_H__
/////////////////////////////////////////////////////////////////////////////
#include "ScadaDefine.h"
#include "ThreadMutex.h"
#include "Task.h"
/////////////////////////////////////////////////////////////////////////////
struct CbOpenAnalyse
{
	SCADA_Digit		*pDigit;
	int				iOpenClose;
	int				iTimeout;
	int				iLossTime;
	UINT			uFaultID;
	INT				iFaultIndex;
	bool			iUsing;
	SYSTEMTIME		SysTime;
};
//2015-06-26
//ң����ң�ء�ң�⡢ң��ͨ�õĲ�ѯ���¼�ؼ���
typedef struct _st_devKey
{
  UINT stid;//�ɼ���ԪID
  UINT device;//�豸��(lDPUID)
  UINT no;//վ�����(iNoInDPU)
  UINT iZf;//�Ƿ�Ϊת��
  _st_devKey()
  {
	  iZf=0;//0δת��1ת��
  }
}stdevKey;

#define MAX_CB_OPENANALYSE_NUM 200

class  AFX_EXT_CLASS  CScadaDataBase : public CDataBase
{
public:
	CScadaDataBase();
	~CScadaDataBase();
	
public:
	/*
	����Ϊ��׼�Ķ�д���м�¼�ĺ������ɿ����Ƶ�CDataBase�С�
	���º���(��GetNextElement��GetElementByIndex����)�����ع����ڴ��ָ�룬ע�����еĶ�����ʹ��ǰ��Ҫ��ʼ��,����ΪNULL��
	���Ҫֱ�ӷ��ʹ����ڴ棬ֱ��ʹ��CDBTable�ĺ�����
	*/
	//�õ�ĳ����ļ�¼��
	INT	    GetElementCount( INT iTableID );
	//�õ�ĳ�����ͷ��¼λ��
	INT		GetFirstElementPosition( INT iTableID );
	//�õ�ĳ�������һ����¼
	BOOL	GetNextElement( INT iTableID, INT& iIndex, void *pElement );
	//ͨ��ID�õ�ĳ�����ĳ����¼��INDEX
	BOOL	GetElementIndexByID(INT iTableID, UINT uRecordID, INT& iIndex);
	//ͨ��INDEX�õ�ĳ�����ĳ����¼
	BOOL	GetElementByIndex( INT iTableID, INT iIndex, void *pElement );
	//ͨ��INDEX����ĳ�����������¼
	BOOL	SetElementByIndex( INT iTableID, INT iIndex, void *pElement, BOOL bBroadcast = TRUE );
	//ͨ��IDȡ��ĳ�����ĳ����¼
	BOOL	GetElementByID( INT iTableID, UINT uRecordID, void *pElement );
	//ͨ��ID����ĳ�����������¼
	BOOL	SetElementByID( INT iTableID, UINT uRecordID, void *pElement, BOOL bBroadcast = TRUE );
	//����ĳ�����һ����¼����¼��ID32=0Ϊ������¼��
	BOOL	AddElement( INT iTableID, void *pElement , BOOL bBroadcast = TRUE );
	//ͨ��INDEXɾ��ĳ�����һ����¼
	BOOL	DeleteElementByIndex( INT iTableID, INT iIndex, BOOL bBroadcast = TRUE );
	//ͨ��IDɾ��ĳ�����һ����¼
	BOOL	DeleteElementByID( INT iTableID, UINT uRecordID, BOOL bBroadcast = TRUE );
	//ͨ����¼INDEX�õ�ĳ�����ĳ����¼��ĳ���ֶε�ֵ
	BOOL	GetElementFieldByIndex( INT iTableID, INT iRecordIndex, INT iFieldIndex, void * pFieldData );
	//ͨ����¼INDEX����ĳ�����ĳ����¼��ĳ���ֶε�ֵ
	BOOL	SetElementFieldByIndex( INT iTableID, INT iRecordIndex, INT iFieldIndex, void * pFieldData, BOOL bBroadcast );
	//ͨ����¼ID�õ�ĳ�����ĳ����¼��ĳ���ֶε�ֵ
	BOOL	GetElementFieldByID( INT iTableID, UINT uRecordID, INT iFieldIndex, void * pFieldData );
	//ͨ����¼ID����ĳ�����ĳ����¼��ĳ���ֶε�ֵ
	BOOL	SetElementFieldByID( INT iTableID, UINT uRecordID, INT iFieldIndex, void * pFieldData, BOOL bBroadcast );

public:
	//���º�����SCADA�����еĹ���,��Ҫ��ǰ�û��ȵ���.
	void	ProcessAnalogPacket( AnalogPacket APacket );
	BOOL	CheckAnalog( SCADA_Analog * pAnalog,void * pOrigin,BOOL bFormula = FALSE);
	BOOL	CheckAnalog(INT iIndex, SCADA_Analog * pAnalog,void * pOrigin,BOOL bFormula = FALSE);
	void	AddAnalogEvent( INT iAlarmType, SCADA_Analog* pAnalog, SYSTEMTIME & time, INT iType=0, INT iAlarmClass=0 );
	
	void	ProcessDigitPacket( DigitPacket APacket );
	BOOL	CheckDigit( SCADA_Digit * pDigit,INT iOrigin,BOOL bFormula = FALSE);
	BOOL	CheckDigit(INT iIndex, SCADA_Digit * pDigit,INT iOrigin,BOOL bFormula = FALSE);
	void	AddDigitEvent( INT iAlarmType, SCADA_Digit* pDigit, SYSTEMTIME & time, INT iType=0, INT iAlarmClass=0 );

	CbOpenAnalyse			m_FaultAnalyseArray[MAX_CB_OPENANALYSE_NUM]; 
	static  ThreadFuncRet	CbOpenAnnlyseThread(LPVOID pScadaDataBase);
	CThreadMutex			m_cs;
	//��ʱ�ƴα���
	void					AlarmForRunTimeNum();
	void					DigitStatInit();

	void	ProcessPulsePacket( PulsePacket PPacket );
//	BOOL	CheckPulse( SCADA_Pulse * pPulse, void * pOrigin , BOOL bFormula = FALSE);
//	BOOL	CheckPulse( INT iIndex, SCADA_Pulse * pPulse, void * pOrigin , BOOL bFormula = FALSE);
	//ȡ����·����ȼ�¼:����һΪǰ�����͵Ĳɼ��㣬������Ϊ����·�ĵ�ȵ�
	//BOOL	GetPLDPulse(SCADA_Pulse * &pSPulse, SCADA_Pulse * &pTPulse);
	
	//����ͳ����Ϣ��ϵͳ������ʱ���㲥��ͳ��ʱ�ο�ʼʱ�㲥��
//	void    ClearStatistic(SYSTEMTIME SysTime,BOOL bBroadcast = TRUE);

	//����ң��\ң�Ź���ֵ����������ã����ɱ��������ͳ�Ʋ��㲥��
	BOOL	SetAnalog(UINT iID,FLOAT value);
	BOOL	SetDigit(UINT iID,INT value = 1);

	BOOL	JudgeFault( SCADA_Digit *pDigit,int type,int gap);
	BOOL	JudgeFault( SCADA_Digit *pDigit, UINT uFaultID32, INT iFaultIndex, int gap );
	//ȡ�¹���
	BOOL	FindFault( SCADA_Digit *pDigit, UINT& uFaultID32, INT& iFaultIndex );
	//ȡ�¹��ܵı����ȼ�
	INT		GetFaultClass(SCADA_Digit* pDigit);
	//ȡ���ޱ����ȼ�
	INT		GetRepairClass(INT iClass);
public:
	//������������ǰ�û����ã��򿪺͹ر���ʱ�������������¹���բ�жϡ�
	void	Open_Msg();
	void	Close_Msg();

public:
	//������һЩSCADA��ʱ����,�����Ӧ�������Ҫ��ʱ���񣬿�ֱ�ӵ���CTaskScheduler.
//	void	SetCalculateFormulaTask(int cycle/*in second*/);
//	void	SetClearStatisticTask();

public:
	//BOOL	GetPLDAnalog(SCADA_Analog * &pSAnalog, SCADA_Analog * &pTAnalog);
	//��ȡң��Ĺ���ֵ �ú�����������·��·����� ��ʷ�����߿���ʹ�øú���
	BOOL	GetAnalogValue(UINT uAnalogID, float &fVal, bool bPLD=TRUE);
		
	//ֱ�ӷ��ʹ����ڴ�.
	//���ݱ�ĳ�վ�����(����)���õ���¼
	// iIndex �ڳ�վ�е���� 
	BOOL	GetTabAnalogByIndex( INT iIndex, SCADA_Analog* & pAnalog );
	BOOL	GetTabDigitByIndex( INT iIndex, SCADA_Digit* & pDigit );
	//BOOL	GetTabPulseByIndex( INT iIndex, SCADA_Pulse* & pPulse );
	
	BOOL	GetTabAnalogByID( UINT iID, SCADA_Analog* & pAnalog );
	BOOL	GetTabDigitByID( UINT iID, SCADA_Digit* & pDigit );
//	BOOL	GetTabPulseByID( UINT iID, SCADA_Pulse* & pPulse );

	//2015-06-26 add by tjs  begin
	//ң�� 
	BOOL	GetTabRelayByIndex( INT iIndex, SCADA_Relay* & pRelay );
	//��� 2015-06-26 add by tjs 
	BOOL	GetTabSetPointByIndex( INT iIndex, SCADA_SetPoint* & pSetpoint );
	//����ID32�ڲ�ID���Ҽ�¼

	//ң�� 2015-06-26 add by tjs 
	BOOL	GetTabRelayByID( UINT iID, SCADA_Relay* & pRelay);
	//��� 2015-06-26 add by tjs 
	BOOL	GetTabSetPointByID( UINT iID, SCADA_SetPoint* & pSetpoint);
   
	//����ͨѶʹ�õĹؼ��ֵõ���¼
	//ң��
		//2016-2-25 ����bTransFlg ����ʶ���Ƿ�������ת������,Ĭ��Ϊ��ת��
	BOOL	GetTabRelayByKey( const stdevKey &vkey, SCADA_Relay* & pRelay,bool bTransFlg=false );
	//ң�������
		//2016-2-25 ����bTransFlg ����ʶ���Ƿ�������ת������,Ĭ��Ϊ��ת��
	BOOL	GetTabSetPointByKey(const stdevKey &vkey, SCADA_SetPoint* & pSetpoint,bool bTransFlg=false );
	//2015-07-16 add��Ҫԭ����
	//��Ϊ��������ͨ��channelid +�豸��+�豸��ַ��Ψһʶ��һ��д��ļ�¼
	//Ҳ��uTuID+lDPUID+iNoInDPU 
	//�ɹ�����TRUE ���󷵻�FALSEͬʱ��o_indexΪ-1

	BOOL     GetAnalogIndexByKey(const stdevKey &vkey,int &o_index);
	BOOL     GetDigitIndexByKey(const stdevKey &vkey,int &o_index);

	
    //tjs end
	BOOL	GetFormulaTable();
	void	DiscardFormulaTable();

	BOOL	GetAnalogTable();
	BOOL	GetDigitTable();
	BOOL	GetPulseTable();
	//add by tjs 2015-06-26
	BOOL	GetRelayTable();
	BOOL	GetSetPointTable();
	//����ң�ء�ң���� 1ң��,2ң��
	BOOL    uptTableYkOrYt(int ntype);
	//����ң���ĵ�λ��Ӧö������
	//����������õĻ��������ε����ظ�д��ֻ��Ӱ��Ч�ʶ���
	//BOOL    uptAnalogUnitToEnum();

public:
	bool			m_bExit;
	CTaskScheduler  m_TaskScheduler;
	CDataBase		m_SystemDB;
	CDBTable		m_TabFormula;

	INT				m_nFormulaCycle;
	BOOL			m_bFormulaTableOK;

	CDBTable		m_TabAnalog;
	CDBTable		m_TabDigit;
	//CDBTable		m_TabPulse;
	//add by tjs begin
	CDBTable		m_TabRelay;
	CDBTable		m_TabSetPoint;
	BOOL			m_bRelayTableOK;
	BOOL			m_bSetPointTableOK;
	//2015-06-26
	SCADA_Global	*m_pGlobal;
	BOOL			m_bAnalogTableOK;
	BOOL			m_bDigitTableOK;
	//BOOL			m_bPulseTableOK;
	BOOL            m_bInitAnalogUnitEnumOK;//2015-11-18 ̷��ʤ��ӳ�ʹ��ң��ĵ�λ�þ�����ʹ��
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_SCADA_API_H__
  
