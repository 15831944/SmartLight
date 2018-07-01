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

	//停电预处理
	bool FormOneStationStopSerial();
	//执行序列控制
	bool Exec();
	//消息处理
	void ProcessMsg(tabMessage *pMsg);

	bool ExecSingleYK(int iIndex);
	//获取遥信位置
	int GetDigitValue(UINT ulYKID32);
	//分析序控信息
	bool AnalyseRcstrSerial();
	bool AddRControl( UINT RelayID32, int iTargetVal );
	bool InsertRControl( UINT RelayID32, int iTargetVal, int iIndex );
	bool GetRcSerial();

	HANDLE	m_hSerialRCThread;
	static	DWORD WINAPI SerialRCThread (LPVOID pSerialRCThread);	// 序列线程
	
	//为了发送顺控失败而加，变态
	void	SendAlarmMessage( CString strError );

	UINT	m_ulRCSerialID;				//序控点号
	bool	m_bFGType;					//序控为复归 2004-12-14
	bool    m_bStopSerial;
	int     m_iKV;
	bool	m_bTempSerialRC;
	bool	m_UseBP;
	CRControlArray	m_RControlArray;	
	CUIntArray		m_YKID32Array;		//遥控点号
	CUIntArray		m_SpanTimeArray;
	CUIntArray		m_bNoContinueArray;		//失败不继续遥控标志

	int		m_iCurrentRc;			//当前序控号
	int		m_iSpanTime;			//间隔(秒)

	int		m_IfSelected;			//0 初始化 1 选择成功 -1 选择失败
	int 	m_IfExecOk;				//0 初始化 1 执行成功 -1 执行失败

	int		m_iRcFailNum;			//序控失败次数
	
	//实时库接口
	CWriteLog	m_log;
	CMultiDataBase	*m_pMultiDataBase;
	CScadaDataBase  *m_pScada;
	CDataBase		*m_pNetDB;
	CDBTable		m_DBTable;
	SCADA_RcSerial	m_RcSerial;
	bool			m_bRtdbInit;
	lpfnRCFailProcess	m_lpfnRC;
	CWnd *				m_pWnd;

	//操作票数据接口
	COperateRecord m_OperateRecord;
};

#endif // !defined(AFX_SERIALRC_H__4C553E7D_63E6_4B9D_BDB0_7D53FF70B898__INCLUDED_)
