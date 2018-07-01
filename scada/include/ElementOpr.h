// ElementOpr.h: interface for the CElementOpr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTOPR_H__70CA61CE_7315_4899_8713_1A14156D3E40__INCLUDED_)
#define AFX_ELEMENTOPR_H__70CA61CE_7315_4899_8713_1A14156D3E40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PasEnv.h"
#include "MultiDataBase.h"
#include "SCADA.h"
#include "NET.h"
#include "TOPO.h"

#ifdef UNIX_ENV
#include "xstring.h"
#define CUIntArray XUIntArray
#endif

//遥控闭锁类型定义，可能同时多个原因闭锁
#define RC_ATRESIA_LOCAL			1		//就地
#define RC_ATRESIA_SAME				(1<<1)	//本身位和目标位相同
#define RC_ATRESIA_CARD				(1<<2)	//开关挂牌
#define RC_ATRESIA_TEST				(1<<3)	//手车不在合位
#define RC_ATRESIA_RCING			(1<<4)	//遥控进行中
#define RC_ATRESIA_ABNORMAL			(1<<5)	//开关位置异常
#define RC_ATRESIA_GRANTED			(1<<6)	//开关已授权
#define RC_ATRESIA_UNGRANTED		(1<<7)	//开关未授权
#define RC_ATRESIA_SCABNORMAL		(1<<8)	//手车位置异常
#define RC_ATRESIA_COMERR			(1<<9)	//装置通讯中断
#define RC_ATRESIA_FAULT			(1<<10)	//保护动作
#define RC_ATRESIA_SPIRING			(1<<11)	//弹簧未储能
#define RC_ATRESIA_GROUNDLINE		(1<<12)	//未拆除接地线
#define RC_ATRESIA_Formula			(1<<13)	//公式闭锁
#define RC_ATRESIA_NORIGHT			(1<<14)	//无遥控权限
#define RC_ATRESIA_NOWATCH			(1<<15)	//监护席未启动监控界面
#define RC_ATRESIA_NOWATCHRIGHT		(1<<16)	//监护席用户无监护权

//以下闭锁是可以解除的
#define RC_ATRESIA_LOOP				(1<<20)	//不能闭环运行
#define RC_ATRESIA_GGEAR			(1<<21)	//地刀不在分位
#define RC_ATRESIA_GEAROPR			(1<<22)	//不能带电合地刀
#define RC_ATRESIA_SWITCHOPR		(1<<23)	//不能带负荷操作刀闸

//装置复归的判断
#define RC_ENABLE_NONEEDRESET		(1<<25)	//无需复归

#define  SECTION_TYPE_NULL		0		//无效间隔
#define  SECTION_TYPE_LINE		1		//线路间隔
#define  SECTION_TYPE_TRFM		2		//变压器间隔
#define  SECTION_TYPE_LD		3		//负荷间隔
#define  SECTION_TYPE_SO		4		//电源间隔

typedef struct 
{
	UINT         		uDigitCloseID;                  //合位
	INT         		uDigitCloseIndex;               
	UINT         		uDigitOpenID;	                //分位
	INT         		uDigitOpenIndex;	   
	
	UINT         		uDigitSCCloseID;				//手车合位
	INT         		uDigitSCCloseIndex;					
	UINT         		uDigitSCOpenID;					//手车分位
	INT         		uDigitSCOpenIndex;	
				
	UINT         		uDigitLocalID;	                //就地位
	INT         		uDigitLocalIndex;	            
	UINT         		uDigitSpringID;	                //弹簧储能状态
	INT         		uDigitSpringIndex;
	
	UINT				uDigitFaultID;					//事故总
	INT					uDigitFaultIndex;	
	UINT				uDigitComErrID;					//通讯中断
	INT					uDigitComErrIndex;	
}TabCbDigitInfo;		//开关本体信息

typedef struct 
{
	UINT         		uElementID;                      //设备ID32
	CUIntArray			uT1EleID;						 //端子1相连所有设备
	CUIntArray			iT1EleIndex;					 //端子1相连所有设备下标
	CUIntArray			uT1Terminal;					 //端子1相连设备的端子
	CUIntArray			uT2EleID;						 //端子2相连所有设备
	CUIntArray			iT2EleIndex;					 //端子2相连所有设备下标
	CUIntArray			uT2Terminal;					 //端子2相连设备的端子
	BOOL				bScan;
}tabElementTopo;		//设备连接关系
class AFX_EXT_CLASS CElementOpr  
{
public:
	CElementOpr(CMultiDataBase *pMultiDataBase);
	CElementOpr();
	~CElementOpr();
 	BOOL RtdbInit(CMultiDataBase *pMultiDataBase);
	
	//分析设备是否带电
	//参数说明：
	//uElePointID	- 发生遥信变位的开关的合位遥信点号
	//bOpen			- 开关打开
	//bFault		- 事故调闸
	//bBroatCast	- 广播结果与否
	BOOL SetElementIfOperate(BOOL bBroatCast=FALSE, BOOL bReArray=FALSE, UINT uElePointID=0, BOOL bOpen=FALSE, BOOL bFault=FALSE);
	//开关量测自动配置
	BOOL  SetScadaMapToCB();

	//间隔分析,返回值是开关刀闸的设备号,间隔间用0xFFFFFFFF隔开
	BOOL GetSectionByEleID(UINT uEleID, CUIntArray& uEleArray, CUIntArray& uEleIndexArray );
	//分析间隔的类型
	INT  AnalyseSectionType(CUIntArray& uEleIndexArray);
	//分析间隔的信息:主开关、间隔所控制的设备号(线路、变压器、电源、负荷等)
	BOOL  AnalyseSectionInfo(CUIntArray& uEleIndexArray, int& iBreakerIndex, int& iDeviceIndex);
	//取变压器的高压侧档位情况
	void  GetTrfmHTapInfo(UINT uTrfmID32, NET_tapty &tapty);

	BOOL NetTopoRepair();
	
	//VQC的拓扑接口函数
	//分析变压器的并列情况  并列的变压器放在一起，中间由0xFFFFFFFF隔开
	void  AnalyseTrfmRun(UINT uSTID32, CUIntArray& uTrfmArray);
	//查找和某台变压器并列的所有变压器
	void  AnalyseTrfmRun(NET_trfm *pTrfm, CUIntArray& uTrfmArray, BOOL bFindBYQ=TRUE);
	//分析设备的电压等级
	INT	  GetElementKv(INT iEleIndex); 
	//获取和指定变压器连接在一起的母线
	BOOL  GetTrfmBus( UINT uTrfmID, CUIntArray& BusHArray, CUIntArray& BusMArray, CUIntArray& BusLArray );
	//获取和指定母线连接在一起的电容器
	BOOL  GetBusCp( UINT uBusID, CUIntArray&  CpIDArray );
	//获取和指定电容器间隔的开关 
	BOOL  GetCpCb( UINT uCpID, UINT& uCbID );
	//获取设备带电状态
	BOOL  IsOperate(UINT uDeviceType, UINT DeviceID32);
	//判断变压器是否空载(1.不带电、2.有一侧(双圈)两侧(三圈)开关打开变压器为空载. 但没有判断有功等量测)
	BOOL  IsEmptyRun(UINT uTrfmID);

	//小电流接地的拓扑接口函数
	//分析某电压等级下母线的运行情况  不带电不列出 0xFFFFFFFF隔开
	void  AnalyseBusRun(UINT uKvID32, CUIntArray& uBusArray);
	//查找和某母线在一节点上的所有母线
	void  AnalyseBusRun(NET_bus *pBus, CUIntArray& uBusArray);
	//获取和指定母线连接在一起的负荷
	BOOL  GetBusLd( UINT uBusID, CUIntArray&  LdIDArray );


	//网损统计(可以周期调用)
	BOOL  CalPowerLoss();
	//保护信息关联表的创建(已有信息不删除)
	BOOL  WriteProtectRelation();

	//是否能遥控开关的判断
	//uYKID32		遥控点号  
	//iTargetVal	遥控目标值
	//bSingleRC		单控标志
	//返回值:0能遥控，非0不能遥控 -1配置问题 其它参见定义
	INT  CanRControl(UINT uYXID32, UINT uYKID32, INT iTargetVal, BOOL bSingleRC=TRUE);
	//开关本体信息的获取
	BOOL GetCbDigitInfo(UINT uCBID, TabCbDigitInfo & CbDigitInfo);

	//软件闭锁(针对所有开关)
	BOOL SoftJudge(BOOL bSet);

	//设备检修：开关为分位能挂检修牌
	//bRepair		 - 进入检修还是完成检修
	//bSetGroundGear - 是否自动设置接地刀闸位置
	//进入检修：无地刀就在间隔第一个开关(刀闸)处挂地线
	//完成检修：自动将所有接地线标志去掉
	BOOL DeviceRepair(UINT DeviceEleID, BOOL bRepair, BOOL bSetGroundGear=FALSE);
private:
	CMultiDataBase	*m_pMultiDataBase;
	CDataBase		m_ScadaDB;
	CDataBase		m_NetDB;
	CDataBase		m_TopoDB;
	CDBTable		m_dvlinkTable;
	CDBTable		m_elementTable;
	CDBTable		m_cbTable;
	CDBTable		m_DigtalTable;
	TOPO_element	*m_pElement;
	TOPO_dvlink	    *m_pDvlink;
	NET_global		*m_pGlobal;
	int				lv_element;
	int				lv_dvlink;

	char			m_pszSysIniFile[256];
	bool			m_bNeedInit;

	tabElementTopo	*m_tabElementTopo;
	int				m_iElementLength;
	CUIntArray		m_uSOEleID;						 //电源设备
	CUIntArray		m_iSOEleIndex;					 //电源设备下标
	CUIntArray		m_uCBEleID;						 //开关设备
	CUIntArray		m_iCBEleIndex;					 //开关设备下标
	CUIntArray		m_uCBPointID;					 //开关遥信位置
	CUIntArray		m_uSCPointID;					 //手车开关手车遥信位置

	BOOL FormElementTopo();
	//设置开关分合状态
	BOOL SetCbIfOpen();
	//实时库接口
	BOOL RtdbInit();
	//公式闭锁判断
	BOOL  CheckRCCondition(SCADA_Relay Relay, int iTargetVal);
	
	BOOL AddElement(CUIntArray &eleArray, UINT ulID);
	BOOL FindElement(CUIntArray &eleArray, UINT ulID);
	BOOL IfNeedInit();
};

#endif // !defined(AFX_ELEMENTOPR_H__70CA61CE_7315_4899_8713_1A14156D3E40__INCLUDED_)
