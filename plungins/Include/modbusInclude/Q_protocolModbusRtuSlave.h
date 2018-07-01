//*
//*
//* Q_protocolModbusTcpServerStandard.h
//*
//*
#pragma once

#ifndef __Q_PROTOCOL_MODBUS_RTU_SLAVE_H_INCLUDED__
#define __Q_PROTOCOL_MODBUS_RTU_SLAVE_H_INCLUDED__
#pragma warning(disable:4251)
#include "Q_frameInclude.h"
#include "Q_ModbusRtuSlaveDef.h"
#include "Q_ModbusDefine.h"

#include <vector>
using namespace std;
/** 
* @interface Q_protocolModbusTcpServerStandardCommand 
* @brief modbus rtu slave 通讯规约命令
* @author tanjingsheng
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class __Q_COMMMODBU_RTU_SLAVE_EXPORT Q_protocolModbusRtuSlaveCommand : public Q_protocolCommandBase
{
public:
	Q_protocolModbusRtuSlaveCommand(void);
	virtual ~Q_protocolModbusRtuSlaveCommand(void);

public:



protected:


};





/** 
* @interface Q_protocolModbusTcpServerStandardCommandManager
* @brief modbus tcp server通讯规约命令管理
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class __Q_COMMMODBU_RTU_SLAVE_EXPORT Q_protocolModbusRtuSlaveCommandManager : public Q_protocolCommandManagerBase
{
public:
	Q_protocolModbusRtuSlaveCommandManager(void);
	virtual ~Q_protocolModbusRtuSlaveCommandManager(void);

public:
	virtual bool				addCommand(Q_protocolCommandBase* pCommand);


};






/** 
* @interface Q_protocolModbusTcpServerStandardSetting 
* @brief modbus tcp 服务端通讯规约配置
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class __Q_COMMMODBU_RTU_SLAVE_EXPORT Q_protocolModbusRtuSlaveSetting : public Q_protocolSettingBase
{
public:
	Q_protocolModbusRtuSlaveSetting(void);
	Q_protocolModbusRtuSlaveSetting(Q_protocolModbusRtuSlaveSetting *other);
	virtual ~Q_protocolModbusRtuSlaveSetting(void);
	void setFilePath(char *);
	bool initSetting();
	char *getContentID(const int &v_idx,const int &v_grp=1);
	//设定遥测的地址范围及有效的遥信个数
	void               setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	//设定遥信的地址范围及有效的遥信个数
	void               setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	//遥测开始地址
	int                 getAnalogBegin();
	int                 getAnalogEnd();
	//遥信开始地址
	int                 getPointBegin();
	int                 getPointEnd();
	//遥测个数
	int                 getAnalogCount();
	//遥信个数
	int                 getPointCount();
	//计划曲线开始地址

	//遥测起始寄存器地址
	int                 getRegisterBegin();
	//遥信起始线圈地址
	int                 getRelayBegin();
	//遥调
	int                 getRegisterWriteBegin();
	//遥控地址
	int                 getRelayWriteBegin();
	int                 getYkFunCode();//遥控的功能码
	int                 getYkOnValue();//遥控合的值
	int                 getYkOffValue();//遥控断开的值
	int                 getYcFunCode();//遥测功能码
	int                 getYxFunCode();//遥信功能码
	int                 getYtFunCode();//遥调功能码
	int                 getSyncFirst();//遥调功能码
	bool                getPrintPacket();//打印报文标识
	bool                getPrintError();//打印错误信息标识
	bool                getPrintPrompt();//提示信息标识
	bool                getSendClearBuffer();//得到发送一条清空一下缓冲区标识

	int getDeviceId();//读设备地址
	ModbusFloatUpSeq getFloatUpSeq();
	ModbusYTMethod   getYTMethod();
	//遥调字节顺序
	ModbusFloatUpSeq getYTByteSeq();
	ModbusAnalogUpType getYcUpType();
	void SetWndInfo(HWND wnd,int MsgID);
	HWND getWndHwnd();
	int getWndMsgID();
	int getWndCmdID();
	virtual bool reloadLog();


protected:
	vector<ST_ModCfgT > m_vContent;//内容
	vector<ST_ModCfgT > m_vGroup;//分组名
private:
	char m_sFilePath[MAX_PATH];//文件路径
	char LastError[Q_FRAME_CONST1024];         //错误信息
	bool init();
private:
	int m_alg_count;//遥测个数
	int m_alg_begin;//遥测起始地址
	int m_alg_end;//遥测结束地址
	int m_pnt_count;//遥信息个数
	int m_pnt_begin;//遥信起始地址
	int m_pnt_end;//遥信结束地址
	int m_plan_addr_begin;//计划曲线开始地址
	int m_plan_addr_end;//计划曲线结束地址
	int m_plan_count;//计划曲线信息个数
	HWND    m_wnd;
	int m_nMsgID;//消息编号;
	int m_nCmdID;


};


/** 
* @interface Q_protocolModbusTcpServerStandard
* @brief Modbus tcp服务侧通讯规约
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class __Q_COMMMODBU_RTU_SLAVE_EXPORT Q_protocolModbusRtuSlave : public Q_protocolBase
{
public:
	Q_protocolModbusRtuSlave(void);
	virtual ~Q_protocolModbusRtuSlave(void);

public:
	void SetWndInfo(HWND wnd,int MsgID);

	virtual bool				initializeProtocol();
	virtual bool				explainData();
	virtual bool				makeFrame();
	virtual bool				terminateProtocol();
	virtual bool				startProcess();
	virtual bool				stopProcess();

	//msg要打印信息 
	//len 打印信息长度
	//type类型0 信息 1错误信息 2发送 3接收
	void writefile(char *msg,int len, int type);
protected:

	Q_ComCpuEndian m_cpuEndian;//cpu对齐方式;
	HWND m_wnd;
	int m_nMsgID;
	//读遥测数据 
	//data去掉前两个字节Address与Function,当包括CRC校验码在内
	//len 是数据区的长度,也即是去掉adddress+function，又加上crc两个字节，所以打平
	virtual bool explainFrameYC(Q_MODBUS_ASDU_RTU *header,BYTE *data);
	//读遥信
	virtual bool explainFrameYX(Q_MODBUS_ASDU_RTU *header,BYTE *data);
	//遥控
	virtual bool explainFrameYK(Q_MODBUS_ASDU_RTU *header,BYTE *data);
	//遥调
	virtual bool explainFrameYT(Q_MODBUS_ASDU_RTU *header,BYTE *data);
	virtual bool explainFrameYT_M(Q_MODBUS_ASDU_RTU *header,BYTE *data);
	//len 是asdu 中从addr开始的报文区的长度
	virtual bool sendData(Q_MODBUS_ASDU_RTU *asdu,int len);
	virtual bool explainDataEx(char* buff,int nCount,int& nCurrentPos);
	//发送错误码
	bool sendErroCode(Q_MODBUS_ASDU_RTU *header,BYTE code);
	WORD CRC16(BYTE* data, WORD len);
	ModbusFloatUpSeq m_FloatUpSeq;
	ModbusFloatUpSeq m_YtByteSeq;
	ModbusYTMethod   m_YtMethod;
	Q_MODBUS_ExecCmd m_SigArray;
	bool m_bPrint;
	Q_protocolModbusRtuSlaveSetting* m_PrintSetting;
	char m_LastError[240];
	int m_yc_idx;
};



#endif//__Q_PROTOCOL_MODBUS_RTU_SLAVE_H_INCLUDED__
