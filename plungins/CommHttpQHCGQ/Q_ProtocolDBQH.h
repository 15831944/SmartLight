#ifndef __Q_PROTOCOL_MODBUSTCP_MASTER_H_INCLUDED__
#define __Q_PROTOCOL_MODBUSTCP_MASTER_H_INCLUDED__


#ifdef WINDOWS_ENV
#pragma warning(disable:4251)
#endif
#include "Q_frameInclude.h"
#include "Q_runingUnitDefine.h"
#include "Q_HttpDBQHDefine.h"
#include <vector>
using namespace std;


// __Q_MODBUSTCP_MASTER_EXPORT
class Q_ProtocolDBQHCommand : public Q_protocolCommandBase
{
public:
	Q_ProtocolDBQHCommand(void);
	virtual ~Q_ProtocolDBQHCommand(void);
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

};


/*
* @interface Q_protocolModbusTcpServerStandardSetting 
* @brief modbus tcp 服务端通讯规约配置
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class  Q_ProtocolDBQHSetting : public Q_protocolSettingBase
{
public:
	Q_ProtocolDBQHSetting(void);
	virtual ~Q_ProtocolDBQHSetting(void);
	void setFilePath(char *);
	bool initSetting();
	char *getContentID(const UINT &v_idx,const UINT &v_grp=1);
	bool reloadLog();
	
protected:
	vector<ST_ModCfgT > m_vContent;//内容
	vector<ST_ModCfgT > m_vGroup;//分组名
	
private:
	char m_sFilePath[MAX_PATH];//文件路径
	char LastError[Q_FRAME_CONST1024];         //错误信息
	bool init(void);
private:
	
};


/** 
* @interface Q_protocolModbusTcpServerStandard
* @brief Modbus tcp服务侧通讯规约
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class  Q_ProtocolDBQH : public Q_protocolBase
{
public:
	Q_ProtocolDBQH(void);
	virtual ~Q_ProtocolDBQH(void);
	void SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);

public:
	virtual bool				initializeProtocol();
	virtual bool				explainData();
	virtual bool				makeFrame();
	virtual bool				terminateProtocol();
	virtual bool				startProcess();
	virtual bool				stopProcess();
	//电表的数据返回
	static size_t funcDianBiao(void *ptr, size_t size, size_t nmemb, void *stream);
	static size_t funcTransduce(void *ptr, size_t size, size_t nmemb, void *stream);
	static int hex_char_value(char c);
	static int hex_to_decimal(const char* szHex, int len);
	Q_dataManager *getDataManager();
	Q_ProtocolDBQHSetting*getSetting();
	time_t m_now;
	bool m_firstRun;
	//电表的数据
	bool handleFrameDianBiao();
	//传感器的数据
	bool handleFrameTransduce();
protected:
	bool makeFrameEx(Q_ProtocolDBQHCommand* pCommand);
	Q_ComCpuEndian m_cpuEndian;//cpu对齐方式;

	virtual bool explainDataEx(char* buff,int nCount,int& nCurrentPos);
	char m_LastMsg[Q_FRAME_CONST256];
private:
	Q_ProtocolDBQHCommand* m_LastCmd;
	vector<Q_ProtocolDBQHCommand*>m_vecCmd;

	int m_nMsgID;//消息编号;
	Q_ProtocolDBQHSetting* m_PrintSetting;
};
extern Q_ProtocolDBQH *g_Protocol;
#endif
