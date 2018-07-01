/****************************************************************************
 * ScadaAPI.h : SCADA处理及访问接口类定义									*
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
//遥调、遥控、遥测、遥信通用的查询表记录关键字
typedef struct _st_devKey
{
  UINT stid;//采集单元ID
  UINT device;//设备号(lDPUID)
  UINT no;//站内序号(iNoInDPU)
  UINT iZf;//是否为转发
  _st_devKey()
  {
	  iZf=0;//0未转发1转发
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
	以下为标准的读写库中记录的函数，可考虑移到CDataBase中。
	以下函数(除GetNextElement，GetElementByIndex以外)不返回共享内存的指针，注意所有的对象在使用前需要初始化,不能为NULL。
	如果要直接访问共享内存，直接使用CDBTable的函数。
	*/
	//得到某个表的记录数
	INT	    GetElementCount( INT iTableID );
	//得到某个表的头记录位置
	INT		GetFirstElementPosition( INT iTableID );
	//得到某个表的下一个记录
	BOOL	GetNextElement( INT iTableID, INT& iIndex, void *pElement );
	//通过ID得到某个表的某条记录的INDEX
	BOOL	GetElementIndexByID(INT iTableID, UINT uRecordID, INT& iIndex);
	//通过INDEX得到某个表的某条记录
	BOOL	GetElementByIndex( INT iTableID, INT iIndex, void *pElement );
	//通过INDEX设置某个表的整条记录
	BOOL	SetElementByIndex( INT iTableID, INT iIndex, void *pElement, BOOL bBroadcast = TRUE );
	//通过ID取得某个表的某条记录
	BOOL	GetElementByID( INT iTableID, UINT uRecordID, void *pElement );
	//通过ID设置某个表的整条记录
	BOOL	SetElementByID( INT iTableID, UINT uRecordID, void *pElement, BOOL bBroadcast = TRUE );
	//增加某个表的一条记录，记录的ID32=0为新增记录。
	BOOL	AddElement( INT iTableID, void *pElement , BOOL bBroadcast = TRUE );
	//通过INDEX删除某个表的一条记录
	BOOL	DeleteElementByIndex( INT iTableID, INT iIndex, BOOL bBroadcast = TRUE );
	//通过ID删除某个表的一条记录
	BOOL	DeleteElementByID( INT iTableID, UINT uRecordID, BOOL bBroadcast = TRUE );
	//通过记录INDEX得到某个表的某条记录的某个字段的值
	BOOL	GetElementFieldByIndex( INT iTableID, INT iRecordIndex, INT iFieldIndex, void * pFieldData );
	//通过记录INDEX设置某个表的某条记录的某个字段的值
	BOOL	SetElementFieldByIndex( INT iTableID, INT iRecordIndex, INT iFieldIndex, void * pFieldData, BOOL bBroadcast );
	//通过记录ID得到某个表的某条记录的某个字段的值
	BOOL	GetElementFieldByID( INT iTableID, UINT uRecordID, INT iFieldIndex, void * pFieldData );
	//通过记录ID设置某个表的某条记录的某个字段的值
	BOOL	SetElementFieldByID( INT iTableID, UINT uRecordID, INT iFieldIndex, void * pFieldData, BOOL bBroadcast );

public:
	//以下函数是SCADA库特有的功能,主要由前置机等调用.
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
	//计时计次报警
	void					AlarmForRunTimeNum();
	void					DigitStatInit();

	void	ProcessPulsePacket( PulsePacket PPacket );
//	BOOL	CheckPulse( SCADA_Pulse * pPulse, void * pOrigin , BOOL bFormula = FALSE);
//	BOOL	CheckPulse( INT iIndex, SCADA_Pulse * pPulse, void * pOrigin , BOOL bFormula = FALSE);
	//取被旁路代电度记录:参数一为前置上送的采集点，参数二为被带路的电度点
	//BOOL	GetPLDPulse(SCADA_Pulse * &pSPulse, SCADA_Pulse * &pTPulse);
	
	//更新统计信息，系统初启动时不广播，统计时段开始时广播。
//	void    ClearStatistic(SYSTEMTIME SysTime,BOOL bBroadcast = TRUE);

	//设置遥测\遥信工程值，供仿真调用，生成报警事项，作统计并广播。
	BOOL	SetAnalog(UINT iID,FLOAT value);
	BOOL	SetDigit(UINT iID,INT value = 1);

	BOOL	JudgeFault( SCADA_Digit *pDigit,int type,int gap);
	BOOL	JudgeFault( SCADA_Digit *pDigit, UINT uFaultID32, INT iFaultIndex, int gap );
	//取事故总
	BOOL	FindFault( SCADA_Digit *pDigit, UINT& uFaultID32, INT& iFaultIndex );
	//取事故总的报警等级
	INT		GetFaultClass(SCADA_Digit* pDigit);
	//取检修报警等级
	INT		GetRepairClass(INT iClass);
public:
	//这两个函数由前置机调用，打开和关闭临时缓冲区，用于事故跳闸判断。
	void	Open_Msg();
	void	Close_Msg();

public:
	//可设置一些SCADA定时服务,其余各应用如果需要定时服务，可直接调用CTaskScheduler.
//	void	SetCalculateFormulaTask(int cycle/*in second*/);
//	void	SetClearStatisticTask();

public:
	//BOOL	GetPLDAnalog(SCADA_Analog * &pSAnalog, SCADA_Analog * &pTAnalog);
	//获取遥测的工程值 该函数考虑了旁路带路的情况 历史、曲线可以使用该函数
	BOOL	GetAnalogValue(UINT uAnalogID, float &fVal, bool bPLD=TRUE);
		
	//直接访问共享内存.
	//根据表的厂站中序号(索引)，得到记录
	// iIndex 在厂站中的序号 
	BOOL	GetTabAnalogByIndex( INT iIndex, SCADA_Analog* & pAnalog );
	BOOL	GetTabDigitByIndex( INT iIndex, SCADA_Digit* & pDigit );
	//BOOL	GetTabPulseByIndex( INT iIndex, SCADA_Pulse* & pPulse );
	
	BOOL	GetTabAnalogByID( UINT iID, SCADA_Analog* & pAnalog );
	BOOL	GetTabDigitByID( UINT iID, SCADA_Digit* & pDigit );
//	BOOL	GetTabPulseByID( UINT iID, SCADA_Pulse* & pPulse );

	//2015-06-26 add by tjs  begin
	//遥控 
	BOOL	GetTabRelayByIndex( INT iIndex, SCADA_Relay* & pRelay );
	//设点 2015-06-26 add by tjs 
	BOOL	GetTabSetPointByIndex( INT iIndex, SCADA_SetPoint* & pSetpoint );
	//根据ID32内部ID查找记录

	//遥控 2015-06-26 add by tjs 
	BOOL	GetTabRelayByID( UINT iID, SCADA_Relay* & pRelay);
	//设点 2015-06-26 add by tjs 
	BOOL	GetTabSetPointByID( UINT iID, SCADA_SetPoint* & pSetpoint);
   
	//根据通讯使用的关键字得到记录
	//遥控
		//2016-2-25 增加bTransFlg 用来识别是否是来自转发数据,默认为非转发
	BOOL	GetTabRelayByKey( const stdevKey &vkey, SCADA_Relay* & pRelay,bool bTransFlg=false );
	//遥调、设点
		//2016-2-25 增加bTransFlg 用来识别是否是来自转发数据,默认为非转发
	BOOL	GetTabSetPointByKey(const stdevKey &vkey, SCADA_SetPoint* & pSetpoint,bool bTransFlg=false );
	//2015-07-16 add主要原因是
	//因为现在引引通过channelid +设备号+设备地址来唯一识别一个写入的记录
	//也即uTuID+lDPUID+iNoInDPU 
	//成功返回TRUE 错误返回FALSE同时置o_index为-1

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
	//更新遥控、遥调表 1遥控,2遥调
	BOOL    uptTableYkOrYt(int ntype);
	//更新遥测表的单位对应枚举类型
	//其它程序调用的话，如果多次调用重复写，只是影响效率而矣
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
	BOOL            m_bInitAnalogUnitEnumOK;//2015-11-18 谭井胜添加初使化遥测的单位妹举类型使用
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_SCADA_API_H__
  
