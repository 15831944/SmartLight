#ifndef __Q_PROTOCOL_MODBUS_TCP_SERVERSTANDARD_EXPORT__
#define __Q_PROTOCOL_MODBUS_TCP_SERVERSTANDARD_EXPORT__

#ifdef WINDOWS_ENV
#pragma warning(disable:4251)
#endif

#include "Q_frameInclude.h"
#include "Q_ModbusSlaveDef.h"
#include "Q_runingUnitDefine.h"
#include "Q_tcpServerUnitInfo.h"

#include "Q_ModbusDefine.h"

#include <vector>
using namespace std;
/** 
* @interface Q_protocolModbusTcpServerStandardCommand 
* @brief modbus tcp server 通讯规约命令
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/

//__Q_COMMMODBUSTCPSERVERSTANDARD_EXPORT
class Q_protocolModbusSlaveCommand : public Q_protocolCommandBase
{
public:
	Q_protocolModbusSlaveCommand(void);
	virtual ~Q_protocolModbusSlaveCommand(void);

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

//__Q_COMMMODBUSTCPSERVERSTANDARD_EXPORT
class Q_protocolModbusSlaveCommandManager : public Q_protocolCommandManagerBase
{
public:
	Q_protocolModbusSlaveCommandManager(void);
	virtual ~Q_protocolModbusSlaveCommandManager(void);

public:
	virtual bool   addCommand(Q_protocolCommandBase* pCommand);
	

};






/** 
* @interface Q_protocolModbusTcpServerStandardSetting 
* @brief modbus tcp 服务端通讯规约配置
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/

//__Q_COMMMODBUSTCPSERVERSTANDARD_EXPORT
class Q_protocolModbusSlaveSetting : public Q_protocolSettingBase
{
public:
	Q_protocolModbusSlaveSetting(void);
	Q_protocolModbusSlaveSetting(Q_protocolModbusSlaveSetting *other);
	virtual ~Q_protocolModbusSlaveSetting(void);
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

	//遥测起始寄存器地址
	int                 getRegisterBegin();
	//遥信起始线圈地址
	int                 getRelayBegin();

	//遥控地址
	int                 getYkFunCode();//遥控的功能码
	int                 getYkOnValue();//遥控合的值
	int                 getYkOffValue();//遥控断开的值
	int                 getYcFunCode();//遥测功能码
	/*update by tjs 2015.04.17 begin*/
	int                 getYcUpType();//遥测上送数据类型
	/*end */
	int                 getYxFunCode();//遥信功能码
	int                 getYtFunCode();//遥调功能码
/*update by tjs 2015.04.17 begin*/
	int                 getYtDataType();//取得遥调数据类型
	int                 getYtType();//遥调类型1多个2单个
	/*end */
	int                 getSyncFirst();//遥调功能码
	bool                getSendClearBuffer();//得到发送一条清空一下缓冲区标识
	
	int getDeviceId();//读设备地址
	ModbusFloatUpSeq getFloatUpSeq();
	ModbusYTMethod   getYTMethod();
	//遥调字节顺序
	ModbusFloatUpSeq getYTByteSeq();
	void SetWndInfo(HWND wnd,int MsgID,int CmdID);
	HWND getWndHwnd();
	int getWndMsgID();
	int getWndCmdID();
	virtual bool reloadLog();
	bool getCloseSyncCode();
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
	HWND    m_wnd;
	int m_nMsgID;//消息编号;
	int m_nCmdID;
	int m_nDebugToolMsgID;
};


/** 
* @interface Q_protocolModbusTcpServerStandard
* @brief Modbus tcp服务侧通讯规约
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/

//__Q_COMMMODBUSTCPSERVERSTANDARD_EXPORT
class Q_protocolModbusSlave : public Q_protocolBase
{
public:
	Q_protocolModbusSlave(void);
	virtual ~Q_protocolModbusSlave(void);

public:
	void SetWndInfo(HWND wnd,int MsgID,int CmdID);
	
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


	void SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);


protected:

	Q_ComCpuEndian m_cpuEndian;//cpu对齐方式;
	HWND m_wnd;
	int m_nMsgID;
	int m_nCmdID;//消息编号
	int m_nDebugToolMsgID;
	//读遥测数据 
	//data去掉前两个字节Address与Function,当包括CRC校验码在内
	//len 是数据区的长度,也即是去掉adddress+function，又加上crc两个字节，所以打平
	virtual bool explainFrameYC(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//读遥信
	virtual bool explainFrameYX(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//遥控
	virtual bool explainFrameYK(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//遥调
	virtual bool explainFrameYT(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	virtual bool explainFrameYT_M(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//asdu 发送的数据内容
	//len 为asdu.data区中数据的长度
	bool findSyncIndex(char* buff,int buffSize,int startPos,int& findPos);
	//len 是asdu 中从addr开始的报文区的长度
	virtual bool sendData(Q_MODBUS_ASDU2 *asdu,int len);
	virtual bool explainDataEx(char* buff,int nCount,int& nCurrentPos);
	bool readFile(char *v_path,vector<MOD_PLAN_CURVE> &vec);
	//发送错误码
	bool sendErroCode(Q_MODBUS_ASDU *header,BYTE code);
	bool findYtExists(Q_MODBUS_ASDU *header,int startAddr,int &nYCindex,int &nYTindex);
	ModbusFloatUpSeq m_FloatUpSeq;
	ModbusFloatUpSeq m_YtByteSeq;
	ModbusYTMethod   m_YtMethod;
	Q_MODBUS_ExecCmd m_SigArray;
	Q_protocolModbusSlaveSetting* m_PrintSetting;
	char m_LastMsg [240];
	int m_yc_idx;
/* update ty tjs 2015.04.17*/
private:
	static const int YtDataTypeFloat = 1;    //遥调类型为浮点getYtDataType()返回值
	static const int YtDataTypeINT2 = 2;      //遥调类型为2字节整型getYtDataType()返回值
	static const int YtDataTypeINT4 = 3;	  //遥调类型为4字节整型getYtDataType()返回值

	static const int YtTypeMulti = 1;//写多个寄存器
	static const int YtTypeSingle = 2;//写单个寄存器

	static const int YcUpTypeFloat = 1;    //遥测类型为浮点getYcUpType()返回值
	static const int YcUpTypeINT2 = 2;      //遥测类型为2字节整型getYcUpType()返回值
	static const int YcUpTypeINT4 = 3;	  //遥侧类型为4字节整型getYcUpType()返回值
	/* end */
};


extern Q_dataManager *m_pMgrModbusSlaveProtocolSetting; //fs20150708



#endif//__Q_PROTOCOLMODBUSTCPSERVERSTANDARD_H_INCLUDED__
