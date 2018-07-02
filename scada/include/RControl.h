/****************************************************************************
 * RControl.h: interface file for the CRControl class.						*
 ****************************************************************************/
#ifndef __AFX_RCONTROL_H_2003_08_28__
#define __AFX_RCONTROL_H_2003_08_28__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include "SCADAAPI.h"
#include "NET.h"
#include "RightDefine.h"
#include "WriteLog.h"
#include "RC_Const.h"
#include "MsgDefine.h"

class CSingleRC;
class CSerialRC;
/////////////////////////////////////////////////////////////////////////////
#define RC_750			750
#define RC_10			10

#define RC_UNDEFINE			0		//��Ч

#define RC_ONE_SELECT		1		//˳��:����һ�����ؽ���ң��ѡ��
#define RC_ONE_SELECTOK		2		//˳��:����һ�����ؽ���ң��ѡ��ɹ�
#define RC_ONE_SELECTFAIL	3		//˳��:����һ�����ؽ���ң��ѡ��ʧ�ܣ�������ǰ����ң��
#define RC_ONE_EXECUTE		4		//˳��:����һ�����ؽ���ң��ִ��
#define RC_ONE_EXECUTEOK	5		//˳��:����һ�����ؽ���ң��ִ�гɹ�
#define RC_ONE_EXECUTEFAIL	6		//˳��:����һ�����ؽ���ң��ִ��ʧ�ܣ�������ǰ����ң��
#define RC_ONE_WAITOK		7		//˳��:����һ������ң�ű�λ�ɹ���������ǰ����ң��
#define RC_ONE_WAITFAIL		8		//˳��:����һ������ң�ű�λʧ�ܣ�������ǰ����ң��

#define RC_ALL_ENDFAIL		9		//˳��ʧ��
#define RC_ALL_END			10		//˳�سɹ����

#define WM_RC_ENDFAIL ( WM_USER + 0x1115 )	//ʧ����Ϣ
#define WM_RC_END	  ( WM_USER + 0x1116 )	//�ɹ���Ϣ

typedef BOOL (CALLBACK *lpfnRCFailProcess)( BOOL bRCSingle, INT iRet, UINT ulYKID, CString strError );

class CJudgeRControl	//ң���ж���
{
public:
	CJudgeRControl( CMultiDataBase *pMDB, CScadaDataBase * pScadaDB, SCADA_Relay tabRelay, int iTargetVal );
	CJudgeRControl( CMultiDataBase *pMDB, CScadaDataBase * pScadaDB, SCADA_Relay tabRelay, int iTargetVal, bool bSingleRC, int iKv );
	virtual ~CJudgeRControl();
public:
	//�ܷ�ң���жϣ��������溯����������й�ʽ
	bool CanRControl();

	int  GetAtresiaFlag();
	int GetDigitValue(UINT ulYKID32);
	//ң�صȴ�ң�ű�λ
	bool WaitDigitValue();

	void SendAlarmMessage( int iSubType, CString strError );
	void SendRCingToFM( INT bRcing );

	TabCbDigitInfo		m_RControlCbInfo;
	lpfnRCFailProcess	m_lpfnRC;
	CDataBase *			m_pNetDB;
	bool				m_bTestRc;
	CWriteLog*			m_plog;
	bool				m_bBreakerRC;			//ң�ؿ���
private:
	CMultiDataBase *	m_pMDB;
	CScadaDataBase *	m_pScadaDB;
	SCADA_Relay			m_tabRelay;
	
	bool				m_bSingleRC;			//���ر�־
	int					m_iTargetVal;			//Ŀ��λ��
	int					m_iAtresiaFlag;			//ң��������־, ͼ�����ð�ť״̬ʹ��
	int					m_iKv;					//��ѹ�ȼ�(750, 10)
};

class AFX_EXT_CLASS CRControl
{
public:
	CRControl(CMultiDataBase *pMDB, CScadaDataBase * pScadaDB, CDataBase *	pNetDB, CWnd* pWnd );
	CRControl(CMultiDataBase *pMDB, CScadaDataBase * pScadaDB, lpfnRCFailProcess lpfnRC );
	CRControl(CMultiDataBase *pMDB, CScadaDataBase * pScadaDB );
	virtual ~CRControl();
//ң�������ж�,ͼ�����ð�ť״̬ʹ��
	int RCContidionJudge( UINT ulRelayID, int iTargetVal, UINT& ulFGRCID );

// ����ң�ؽӿ�
public:
	void SingleRC(int nRCID,int nProcessID,bool bModal,int nScreen, int iRCKv, int iTargetVal);
	void ProcessMsg(tabMessage * pMsg);
// ����ң�ؽӿ�
public:
	void SerialRC(UINT nRCSerialID);
	//ͣ��Ԥ����
	bool AnalyseRcstrSerialForStop();
	//��ȡ�ֳ�λ��, 0 ��λ 1 ��λ -1 δ֪ -2 ���ô���
	int GetHandcartState( UINT ulRelayID );
	//ִ�����п���
	bool SerialRCExec();
	//ֹͣ���п���
	bool SerialRCStop();
	//��ȡң����Ϣ����ִ�������á�false ����ӡ�����
	bool IfPrintOpNote();
	bool GetRCHead( tabRCHead &RCHead );
	bool GetRCContent( CtabRCContentArray &tabRCContentArray );
	//���ؽ�����
	bool IfSingleRCRunning();
	//����ң��
	void End();
private:
	bool				m_bIsRegOk;
	CMultiDataBase *	m_pMDB;
	CScadaDataBase *	m_pScadaDB;
	CDataBase *			m_pNetDB;
	CSingleRC *			m_pSingleRCDlg;
	CSerialRC *			m_pSerialRC;
	lpfnRCFailProcess	m_lpfnRC;
	CWnd *				m_pWnd;
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_RCONTROL_H_2003_08_28__
