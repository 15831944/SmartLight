// SerialRC.h: interface for the CSerialRC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALRC_H__4C553E7D_63E6_4B9D_BDB0_7D53FF70B898__INCLUDED_)
#define AFX_SERIALRC_H__4C553E7D_63E6_4B9D_BDB0_7D53FF70B898__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#include "SCADAAPI.h"
#include "WriteLog.h"
#include "RControl.h"
#include "OperateRecord.h"

typedef CArray< RControl, RControl& > CRControlArray ;
class CSerialRC
{
public:
	CSerialRC(CMultiDataBase *pMDB, CScadaDataBase * pScadaDB, UINT nRCSerialID);
	virtual ~CSerialRC();

	//ͣ��Ԥ����
	bool FormOneStationStopSerial();
	//ִ�����п���
	bool Exec();
	//��Ϣ����
	void ProcessMsg(tabMessage *pMsg);

	bool ExecSingleYK(int iIndex);
	//��ȡң��λ��
	int GetDigitValue(UINT ulYKID32);
	//���������Ϣ
	bool AnalyseRcstrSerial();
	bool AddRControl( UINT RelayID32, int iTargetVal );
	bool InsertRControl( UINT RelayID32, int iTargetVal, int iIndex );
	bool GetRcSerial();

	HANDLE	m_hSerialRCThread;
	static	DWORD WINAPI SerialRCThread (LPVOID pSerialRCThread);	// �����߳�
	
	//Ϊ�˷���˳��ʧ�ܶ��ӣ���̬
	void	SendAlarmMessage( CString strError );

	UINT	m_ulRCSerialID;				//��ص��
	bool	m_bFGType;					//���Ϊ���� 2004-12-14
	bool    m_bStopSerial;
	int     m_iKV;
	bool	m_bTempSerialRC;
	bool	m_UseBP;
	CRControlArray	m_RControlArray;	
	CUIntArray		m_YKID32Array;		//ң�ص��
	CUIntArray		m_SpanTimeArray;
	CUIntArray		m_bNoContinueArray;		//ʧ�ܲ�����ң�ر�־

	int		m_iCurrentRc;			//��ǰ��غ�
	int		m_iSpanTime;			//���(��)

	int		m_IfSelected;			//0 ��ʼ�� 1 ѡ��ɹ� -1 ѡ��ʧ��
	int 	m_IfExecOk;				//0 ��ʼ�� 1 ִ�гɹ� -1 ִ��ʧ��

	int		m_iRcFailNum;			//���ʧ�ܴ���
	
	//ʵʱ��ӿ�
	CWriteLog	m_log;
	CMultiDataBase	*m_pMultiDataBase;
	CScadaDataBase  *m_pScada;
	CDataBase		*m_pNetDB;
	CDBTable		m_DBTable;
	SCADA_RcSerial	m_RcSerial;
	bool			m_bRtdbInit;
	lpfnRCFailProcess	m_lpfnRC;
	CWnd *				m_pWnd;

	//����Ʊ���ݽӿ�
	COperateRecord m_OperateRecord;
};

#endif // !defined(AFX_SERIALRC_H__4C553E7D_63E6_4B9D_BDB0_7D53FF70B898__INCLUDED_)
