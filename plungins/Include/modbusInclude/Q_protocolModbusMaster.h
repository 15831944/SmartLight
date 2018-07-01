#ifndef __Q_PROTOCOL_MODBUSTCP_MASTER_H_INCLUDED__
#define __Q_PROTOCOL_MODBUSTCP_MASTER_H_INCLUDED__


#ifdef WINDOWS_ENV
#pragma warning(disable:4251)
#endif
#include "Q_frameInclude.h"
#include "Q_ModbusTcpMasterDef.h"
#include "Q_ModbusDefine.h"
#include "Q_runingUnitDefine.h"
#include <vector>
using namespace std;


// __Q_MODBUSTCP_MASTER_EXPORT
class Q_protocolModbusMasterCommand : public Q_protocolCommandBase
{
public:
	Q_protocolModbusMasterCommand(void);
	virtual ~Q_protocolModbusMasterCommand(void);

public:
	//设备地址
	inline void					setDestinationId(unsigned char nId) { m_nDestinationId = nId;}
	inline unsigned char		getDestinationId() { return m_nDestinationId;}
    //命令类型
	inline void					setCommandModbusType(Q_Modbus_CommandType eType) { m_commandCDTType = eType;}
	inline Q_Modbus_CommandType    getCommandModbusType() { return m_commandCDTType;}

	inline void					setCommandModbusStatus(Q_Modbus_CommandStatus eStatus) { m_commandCDTStatus = eStatus;}
	inline Q_Modbus_CommandStatus	getCommandModbusStatus() { return m_commandCDTStatus;}

	//寄存器地址
	inline void					setObjectIndex(int nIndex) { m_nObjectIndex = nIndex;}
	inline int		getObjectIndex() { return m_nObjectIndex;}
	//值信息 如果为false为整型解析
	//inline void					setSwitchValue(bool bVal) { m_bOpen = bVal;}
	//inline bool					getSwitchValue() { return m_bOpen;}
	/////////////////湘电两字节四字节混合模式//////////////////////
	/*inline void					setBMixed(bool bVal) { m_bMixed = bVal;}	
	inline bool					getBMixed() { return m_bMixed;}*/

	inline void					setAnalogValue(float fValue) { m_fValue = fValue;}
	inline float				getAnalogValue() { return m_fValue;}
	inline void					setIntValue(int nValue) { m_nValue = nValue;}
	inline int				    getIntValue() { return m_nValue;}
	inline void					setPointType(int nValue) { m_nYxType = nValue;}
	inline int				    getPointType() { return m_nYxType;}
	//设置最大处理次数有时信息不反馈， 程序要根据这些内容来删除命令
	inline void				    setMaxTryCount(int nMaxTryCount) { m_nMaxTryCount=nMaxTryCount;}
	inline int				    getMaxTryCount() { return m_nMaxTryCount;}
	inline void				    setCurTryNum(int nCurTryNum) { m_nCurTryNum=nCurTryNum;}
	inline int				    getCurTryNum() { return m_nCurTryNum;}
	int m_curIdx;
	//设置遥测请求的命令
	void setYcRequireType(int n_type);
	Q_eExplainYcType getYcReuireType();
	//设置请求的功能码
	void setFuncCode(int funcode);
	int getFuncCode();
protected:
	unsigned char				m_nDestinationId; //站编号
	Q_Modbus_CommandType			m_commandCDTType;//命令类型
	Q_Modbus_CommandStatus			m_commandCDTStatus;//状态默认设定选择
	int				m_nObjectIndex;//站里面的设备编号
	//bool						m_bOpen;/// 数字量
	//bool                        m_bMixed;//湘电，两字节四字节混合形式
	float						m_fValue;//模拟量
	int                         m_nValue;//数字量
	int							m_nYxType;//遥信类型
	int                         m_nMaxTryCount;//最大试验数
	int                         m_nCurTryNum;//当前实验数
	int                         m_nLastCmdType;
	Q_eExplainYcType            m_eReqYcType;//遥测的类型
	int m_nFuncCode;//请求的功能码值
};





/** 
* @interface Q_protocolModbusTcpServerStandardCommandManager
* @brief modbus tcp server通讯规约命令管理
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
//// __Q_MODBUSTCP_MASTER_EXPORT
class  Q_ModbusMasterCommandManager : public Q_protocolCommandManagerBase
{
public:
	Q_ModbusMasterCommandManager(void);
	virtual ~Q_ModbusMasterCommandManager(void);

public:
	virtual bool				addCommand(Q_protocolCommandBase* pCommand);
	Q_protocolModbusMasterCommand*		findCommand(Q_Modbus_CommandType eType,Q_Modbus_CommandStatus eStatus,int nObjIndex);
	bool						removeCommand(Q_Modbus_CommandType eType,Q_Modbus_CommandStatus eStatus,int nObjIndex);
	bool						removeCommand(Q_protocolCommandBase* pCommand);


};


/*
* @interface Q_protocolModbusTcpServerStandardSetting 
* @brief modbus tcp 服务端通讯规约配置
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class  Q_protocolModbusMasterSetting : public Q_protocolSettingBase
{
public:
	Q_protocolModbusMasterSetting(void);
	virtual ~Q_protocolModbusMasterSetting(void);
	void setFilePath(char *);
	bool initSetting();
	char *getContentID(const UINT &v_idx,const UINT &v_grp=1);
	//设定遥测的地址范围及有效的遥信个数
	void               setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	//设定遥信的地址范围及有效的遥信个数
	void               setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	//设定计划曲线遥测的地址范转及有效的信息个数
	void               setPlanScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
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
	int                 getPlanAddrBegin();
	//计划曲线结束地址
	int                 getPlanAddrEnd();
	//计划曲线信息个数
	int                 getPlanInfoCount();

	//遥测起始寄存器地址
	int                 getRegisterBegin();
	//遥信起始线圈地址
	int                 getRelayBegin();
	int                 getReadRegisterCode();//遥测读寄存器功能码
	int                 getReadYXRegisterCode();//遥信读寄存器功能码
	int                 getSyncFirstCode();//写多个寄存器功能码

	virtual bool reloadLog();
	int getWriteRegisterCode();//写线圈即遥调功能码
	int getWriteYxRegisterCode();//写线圈即遥控功能码

	int getPerTime();//发送遥测、遥信间隔时间
	int getSleepPerTimes();//寄存器最大数
	ModbusFloatUpSeq getFloatUpSeq();
	ModbusIntUpSeq getIntUpSeq();
	ModbusYTMethod   getYTMethod();
	int getAlgCmdCount();
	int getPntCmdCount();
	vector<ST_ModCmdCfg> m_vCommand;//命令
	vector<ST_ModCmdYXCfg> m_vYXCommand;//命令
	int getReadRegisterCodeEx();
	int getReadYXRegisterCodeEx();
	//得到起始码功能自动计算功能开启
	bool getStartCodeAutoFlg();
	//入库时设备号用功能码代替功能开启(三一重工用)
	//主要原因是因为三一重工存在设备地址一样、设备内编号不一样的不同记录
	//只是召唤的功能码有差异的记录,他们的寄存器号不唯一，所以特殊改
	//2017-1-11 6:42 tjs
	bool getFuncCodeEqualDeviceFlg();

	ModbusIntUpSeq getInt2BytesUpSeq();
protected:
	vector<ST_ModCfgT > m_vContent;//内容
	vector<ST_ModCfgT > m_vGroup;//分组名
	
private:
	char m_sFilePath[MAX_PATH];//文件路径
	char LastError[Q_FRAME_CONST1024];         //错误信息
	bool init(void);
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
	int m_alg_cmd_count;//遥测命令个数
	int m_pnt_cmd_count;//遥信命令个数
};


/** 
* @interface Q_protocolModbusTcpServerStandard
* @brief Modbus tcp服务侧通讯规约
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class  Q_protocolModbusMaster : public Q_protocolBase
{
public:
	Q_protocolModbusMaster(void);
	virtual ~Q_protocolModbusMaster(void);
	void SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);

public:
	virtual bool				initializeProtocol();
	virtual bool				explainData();
	virtual bool				makeFrame();
	virtual bool				terminateProtocol();
	virtual bool				startProcess();
	virtual bool				stopProcess();
	/**************************************************************************************************
	@Function: SetWndInfo
	@Description: 设置窗口句柄
	@Calls:
	@Called By:
	@Input: wnd 窗体句柄,msgID打印消息的ID
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//void SetWndInfo(HWND wnd,int MsgID);
protected:
	bool makeFrameEx(Q_protocolModbusMasterCommand* pCommand);
	Q_ComCpuEndian m_cpuEndian;//cpu对齐方式;
	//读遥测数据
	//data去掉前两个字节Address与Function,当包括CRC校验码在内
	//len 是数据区的长度,也即是去掉adddress+function，又加上crc两个字节，所以打平
	virtual bool explainFrameYC(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//读遥信
	virtual bool explainFrameYX(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//遥控
	virtual bool explainFrameYK(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//处理返回的遥调命令
	virtual bool explainFrameYT(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//写多个寄存器内容，遥调
	virtual bool explainFrameYT_M(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//处理四字节float遥调命令
	virtual bool handleFrameYT(Q_protocolModbusMasterCommand* pCommand);
	//处理int遥调命令(四字节的)
	bool handleFrameYtInt(Q_protocolModbusMasterCommand* pCommand);
	//处理int遥调命令(四字节的),支持有符号的4字节整形
	bool handleFrameYtInt4(Q_protocolModbusMasterCommand* pCommand);	
	//处理8字节double类型多字节遥调
	bool handleFrameYtFloat64Ex(Q_protocolModbusMasterCommand* pCommand);
	//处理遥测命令
	virtual bool handleFrameYC(Q_protocolModbusMasterCommand* pCommand);
	//处理遥信命令
	virtual bool handleFrameYX(Q_protocolModbusMasterCommand* pCommand);
	virtual bool handleFrameYT_M(Q_protocolModbusMasterCommand* pCommand);
	//处理非标准遥调float03
	virtual bool handleFrameYT_M_03(Q_protocolModbusMasterCommand* pCommand);
	//处理遥控命令
	virtual bool handelFrameYK(Q_protocolModbusMasterCommand* pCommand);
	//asdu 发送的数据内容
	//len 为asdu.data区中数据的长度
	bool findSyncIndex(char* buff,int buffSize,int startPos,int& findPos);
	//在数据库中查询数据
	bool fillTagInformation(LPQ_RECORDDATATAG pData,int nAdr,int nDevID);
	//len 是asdu 中从addr开始的报文区的长度
	virtual bool sendData(Q_MODBUS_ASDU2 *asdu,int len);
	virtual bool explainDataEx(char* buff,int nCount,int& nCurrentPos);
	//发送错误码
	bool sendErroCode(Q_MODBUS_ASDU *header,BYTE code);
//	char chInfo[Q_FRAME_CONST256];
	char m_LastMsg[Q_FRAME_CONST256];

	//因原函数大长，抽出来
	bool makeFrameYT(Q_protocolModbusMasterCommand*pCommand,int nFirstCommandIndex);
	//处理错误消息
	bool explainError(Q_MODBUS_ASDU *asdu);
private:
	bool explainFrameYCFloat(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	bool explainFrameYCMixed(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	bool explainFrameYCInt(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	bool explainFrameYCDouble8(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	bool explainFrameYCHuaWei(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	bool explainFrameYCInt4(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	bool explainFrameYCUnsignedInt4(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	bool explainFrameYCUShort(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	
private:
	void setNextAnalog(Q_protocolModbusMasterCommand* cmd);
	void setNextDigital(Q_protocolModbusMasterCommand* cmd);
private:
	Q_protocolModbusMasterCommand* m_LastCmd;
	vector<Q_protocolModbusMasterCommand*>m_vecCmd;
	ModbusFloatUpSeq m_FloatUpSeq;
	ModbusIntUpSeq m_IntUpSeq;
	ModbusYTMethod   m_YtMethod;
	//HWND					m_wnd;
	int m_nMsgID;//消息编号;
	Q_protocolModbusMasterSetting* m_PrintSetting;

	ModbusIntUpSeq m_Int2BytesUpSeq;//2字节

};
#endif
