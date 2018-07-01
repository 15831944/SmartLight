//*
//*
//* Q_protocolModbusPlanCurve.h
//*
//*
#pragma once

#ifndef __Q_protocolModbusPlanCurve_H_INCLUDED__
#define __Q_protocolModbusPlanCurve_H_INCLUDED__
#pragma warning(disable:4251)
#include "Q_frameInclude.h"
#include "Q_ModbusPlanCurveDef.h"
#include "Q_ModbusDefine.h"

#include <vector>
using namespace std;


/** 
* @interface Q_protocolModbusPlanCurveCommand 
* @brief modbus tcp server 通讯规约命令
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class __Q_COMMMODBUSPLANCURVE_EXPORT Q_protocolModbusPlanCurveCommand : public Q_protocolCommandBase
{
public:
	Q_protocolModbusPlanCurveCommand(void);
	virtual ~Q_protocolModbusPlanCurveCommand(void);
	
};





/** 
* @interface Q_protocolModbusPlanCurveCommandManager
* @brief modbus tcp server通讯规约命令管理
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class __Q_COMMMODBUSPLANCURVE_EXPORT Q_protocolModbusPlanCurveCommandManager : public Q_protocolCommandManagerBase
{
public:
	Q_protocolModbusPlanCurveCommandManager(void);
	virtual ~Q_protocolModbusPlanCurveCommandManager(void);

public:
	virtual bool addCommand(Q_protocolCommandBase* pCommand);
	
};






/** 
* @interface Q_protocolModbusPlanCurveSetting 
* @brief modbus tcp 服务端通讯规约配置
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class __Q_COMMMODBUSPLANCURVE_EXPORT Q_protocolModbusPlanCurveSetting : public Q_protocolSettingBase
{
public:
	Q_protocolModbusPlanCurveSetting(void);
	Q_protocolModbusPlanCurveSetting(Q_protocolModbusPlanCurveSetting *other);
	virtual ~Q_protocolModbusPlanCurveSetting(void);
	void setFilePath(char *);
	bool initSetting();
	char *getContentID(const int &v_idx,const int &v_grp=1);

	int getSyncFirst();//遥调功能码
	bool getPrintPacket();//打印报文标识
	bool getPrintError();//打印错误信息标识
	bool getPrintPrompt();//提示信息标识
	bool getPlanOn();//得到打印信息
	bool getSendClearBuffer();//得到发送一条清空一下缓冲区标识
	
	int getDeviceId();//读设备地址
	ModbusFloatUpSeq getFloatUpSeq();
	ModbusYTMethod   getYTMethod();
	//遥调字节顺序
	ModbusFloatUpSeq getYTByteSeq();
	void SetWndInfo(HWND wnd,int MsgID,int CmdID);
	HWND getWndHwnd();
	int getWndMsgID();
	int getWndCmdID();
	inline int getDebugToolMsgID(){return m_nDebugToolMsgID;}
	inline void setDebugToolMsgID(int nMsgID){m_nDebugToolMsgID = nMsgID;}
	virtual bool reloadLog();
	void SetSignalID(int vMsgID);
	
	int getRealPowerYCNo();//实时功率的遥测号
	int getRealRemoteYCNo();//实时调度功率的遥测号
	int getRealLocalYCNo();//实时本地功率的遥测号
	int getRealTatgetYCNo();//实时目标的遥测号

	int getRemoteFlagYXNo();//调度标志遥信号
	int getPlanTypeYXNo();//是调度还是本地的遥信号
	int getCurveType();//得到是AGC曲线还是AVC曲线
	int getAgcPlanID();
protected:
	vector<ST_ModCfgT > m_vContent;//内容
private:
	char m_sFilePath[MAX_PATH];//文件路径
	char LastError[1024];         //错误信息
	bool init();
private:
	HWND    m_wnd;
	int m_nMsgID;//消息编号;
	int m_nCmdID;
	int m_nAgcPlanID;//AGC曲线标志
	int m_nDebugToolMsgID;

};


/** 
* @interface Q_protocolModbusPlanCurve
* @brief Modbus tcp服务侧通讯规约
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class __Q_COMMMODBUSPLANCURVE_EXPORT Q_protocolModbusPlanCurve : public Q_protocolBase
{
public:
	Q_protocolModbusPlanCurve(void);
	virtual ~Q_protocolModbusPlanCurve(void);

public:
	void SetWndInfo(HWND wnd,int MsgID,int CmdID);
	//void SetCurveType(int nType);//设置曲线类型，1为AGC曲线，2为AVC曲线
	virtual bool				initializeProtocol();
	virtual bool				explainData();
	virtual bool				makeFrame();
	virtual bool				terminateProtocol();
	virtual bool				startProcess();
	virtual bool				stopProcess();

	//msg要打印信息 
	//len 打印信息长度
	//type类型0 信息 1错误信息 2接收 3发送
    void writefile(char *msg,int len, int type);
protected:
	Q_ComCpuEndian m_cpuEndian;//cpu对齐方式;
	HWND m_wnd;
	int m_nMsgID;
	int m_nCmdID;//消息编号
	//读遥测数据 
	//data去掉前两个字节Address与Function,当包括CRC校验码在内
	//len 是数据区的长度,也即是去掉adddress+function，又加上crc两个字节，所以打平
	virtual bool explainFrameYC(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	virtual bool explainFrameYT_M(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//asdu 发送的数据内容
	//len 为asdu.data区中数据的长度
	bool findSyncIndex(char* buff,int buffSize,int startPos,int& findPos);
	//len 是asdu 中从addr开始的报文区的长度
	virtual bool sendData(Q_MODBUS_ASDU2 *asdu,int len);
	virtual bool explainDataEx(char* buff,int nCount,int& nCurrentPos);
	//bool readPlanCurve(Q_MODBUS_ASDU *header,BYTE *data,const int &len,int startAddr,int regCount);
	//bool readFile(char *v_path,vector<MOD_PLAN_CURVE> &vec);
	//发送错误码
	bool sendErroCode(Q_MODBUS_ASDU *header,BYTE code);
	ModbusFloatUpSeq m_FloatUpSeq;
	ModbusFloatUpSeq m_YtByteSeq;
	ModbusYTMethod   m_YtMethod;
	Q_MODBUS_ExecCmd m_SigArray;
	bool m_bPrint;
	Q_protocolModbusPlanCurveSetting* m_ProtocolSetting;
	char m_LastError[240];

	bool getPlanCurve(eTypePlanCurve ePlanType);//得到xml文件的288个点
	bool getRealPower(float &fValue);//得到实时出力，遥测
	bool getRealRemote(float &fValue);//得到实时调度值，遥测
	bool getRealLocal(float &fValue);//得到实时本地值得，遥测
	bool getRealTarget(float &fValue);//得到实时目标值，遥测
	bool getRemoteflag(bool &bValue);//得到调度曲线命令，遥信
	bool getCurPlanType(float &fValue);//得到当前的状态是本地还是调度，遥信
	bool getTodayLocalCurve();//得到今天的曲线，如果没有，生成今天的曲线
private:
	bool ifNeedReadLocal();
	bool saveLocalPlan();
	bool SecondToTime(int nSecond,int &Hour,int &Minute,int &Second);
private:
	float m_fRemotePlan[288];
	float m_fLocalPlan[288];

	bool m_bNeedReadRemote;
//	bool m_bNeedReadLocal;
	bool m_bNeedSendRemote;
	bool m_bNeedSendLocal;
	int m_LocalReadCmd;//默认0同时表示读成功，2表示本地无曲线读失败
	int m_LocalWriteCmd;//默认0同时表示写成功，2表示写曲线失败
	int m_RemoteReadYCTime;

	int m_RealPowerYCNo;
	int m_RealRemoteYCNo;
	int m_RealLocalYCNo;
	int m_RealTatgetYCNo;
	int m_RemoteFlagYXNo;
	int m_PlanTypeYXNo;
	
	int m_CurDayOfMonth;
	int m_CurMonth;
	int m_CurYear;
	int m_CurveType;
};



#endif//__Q_protocolModbusPlanCurve_H_INCLUDED__
