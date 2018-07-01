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

#define RC_UNDEFINE			0		//无效

#define RC_ONE_SELECT		1		//顺控:其中一个开关进行遥控选择
#define RC_ONE_SELECTOK		2		//顺控:其中一个开关进行遥控选择成功
#define RC_ONE_SELECTFAIL	3		//顺控:其中一个开关进行遥控选择失败，结束当前开关遥控
#define RC_ONE_EXECUTE		4		//顺控:其中一个开关进行遥控执行
#define RC_ONE_EXECUTEOK	5		//顺控:其中一个开关进行遥控执行成功
#define RC_ONE_EXECUTEFAIL	6		//顺控:其中一个开关进行遥控执行失败，结束当前开关遥控
#define RC_ONE_WAITOK		7		//顺控:其中一个开关遥信变位成功，结束当前开关遥控
#define RC_ONE_WAITFAIL		8		//顺控:其中一个开关遥信变位失败，结束当前开关遥控

#define RC_ALL_ENDFAIL		9		//顺控失败
#define RC_ALL_END			10		//顺控成功完成

#define WM_RC_ENDFAIL ( WM_USER + 0x1115 )	//失败消息
#define WM_RC_END	  ( WM_USER + 0x1116 )	//成功消息

typedef BOOL (CALLBACK *lpfnRCFailProcess)( BOOL bRCSingle, INT iRet, UINT ulYKID, CString strError );

class CJudgeRControl	//遥控判断类
{
public:
	CJudgeRControl( CMultiDataBase *pMDB, CScadaDataBase * pScadaDB, SCADA_Relay tabRelay, int iTargetVal );
	CJudgeRControl( CMultiDataBase *pMDB, CScadaDataBase * pScadaDB, SCADA_Relay tabRelay, int iTargetVal, bool bSingleRC, int iKv );
	virtual ~CJudgeRControl();
public:
	//能否遥控判断，调用下面函数并在最后判公式
	bool CanRControl();

	int  GetAtresiaFlag();
	int GetDigitValue(UINT ulYKID32);
	//遥控等待遥信变位
	bool WaitDigitValue();

	void SendAlarmMessage( int iSubType, CString strError );
	void SendRCingToFM( INT bRcing );

	TabCbDigitInfo		m_RControlCbInfo;
	lpfnRCFailProcess	m_lpfnRC;
	CDataBase *			m_pNetDB;
	bool				m_bTestRc;
	CWriteLog*			m_plog;
	bool				m_bBreakerRC;			//遥控开关
private:
	CMultiDataBase *	m_pMDB;
	CScadaDataBase *	m_pScadaDB;
	SCADA_Relay			m_tabRelay;
	
	bool				m_bSingleRC;			//单控标志
	int					m_iTargetVal;			//目标位置
	int					m_iAtresiaFlag;			//遥控条件标志, 图形设置按钮状态使用
	int					m_iKv;					//电压等级(750, 10)
};

class AFX_EXT_CLASS CRControl
{
public:
	CRControl(CMultiDataBase *pMDB, CScadaDataBase * pScadaDB, CDataBase *	pNetDB, CWnd* pWnd );
	CRControl(CMultiDataBase *pMDB, CScadaDataBase * pScadaDB, lpfnRCFailProcess lpfnRC );
	CRControl(CMultiDataBase *pMDB, CScadaDataBase * pScadaDB );
	virtual ~CRControl();
//遥控条件判断,图形设置按钮状态使用
	int RCContidionJudge( UINT ulRelayID, int iTargetVal, UINT& ulFGRCID );

// 单步遥控接口
public:
	void SingleRC(int nRCID,int nProcessID,bool bModal,int nScreen, int iRCKv, int iTargetVal);
	void ProcessMsg(tabMessage * pMsg);
// 序列遥控接口
public:
	void SerialRC(UINT nRCSerialID);
	//停电预处理
	bool AnalyseRcstrSerialForStop();
	//获取手车位置, 0 分位 1 合位 -1 未知 -2 配置错误
	int GetHandcartState( UINT ulRelayID );
	//执行序列控制
	bool SerialRCExec();
	//停止序列控制
	bool SerialRCStop();
	//获取遥控信息，在执行完后调用。false 不打印或出错
	bool IfPrintOpNote();
	bool GetRCHead( tabRCHead &RCHead );
	bool GetRCContent( CtabRCContentArray &tabRCContentArray );
	//单控进行中
	bool IfSingleRCRunning();
	//结束遥控
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
