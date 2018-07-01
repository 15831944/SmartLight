#ifndef __Q_PROTOCOL_MODBUSTCP_MASTER_H_INCLUDED__
#define __Q_PROTOCOL_MODBUSTCP_MASTER_H_INCLUDED__


#ifdef WINDOWS_ENV
#pragma warning(disable:4251)
#endif
#include "json/json.h"
#include "Q_frameInclude.h"
#include "Q_runingUnitDefine.h"
#include "Q_HttpLightDefine.h"
#include "Q_dataManager.h"
#include <vector>
#include <curl/curl.h>
using namespace std;
//static int i;//用于标识楼层

// __Q_MODBUSTCP_MASTER_EXPORT
class Q_ProtocolLightCommand : public Q_protocolCommandBase
{
public:
	Q_ProtocolLightCommand(void);
	virtual ~Q_ProtocolLightCommand(void);
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
class  Q_ProtocolLightSetting : public Q_protocolSettingBase
{
public:
	Q_ProtocolLightSetting(void);
	virtual ~Q_ProtocolLightSetting(void);
	void setFilePath(char *);
	bool initSetting();
	char *getContentID(const UINT &v_idx,const UINT &v_grp);
	bool reloadLog();
	char *getUrl();//用i 来标识是几楼的地址，因为配置文件中有五种数据,i为全局变量
	char *getGid(int i);
	char *getStatus_DPU(int i);
	char *getBright_DPU(int i);
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
class  Q_ProtocolLight : public Q_protocolBase
{
public:
	Q_ProtocolLight(void);
	virtual ~Q_ProtocolLight(void);
	void SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);
	//string GetLifgtStatus(string url,string ip,string gid);

public:
	Json::Value root;
	virtual bool				initializeProtocol();
	virtual bool				explainData();
	virtual bool				makeFrame();
	virtual bool				terminateProtocol();
	virtual bool				startProcess();
	virtual bool				stopProcess();
	//灯的数据返回
	static size_t funcLight(void *ptr, size_t size, size_t nmemb, void *stream);
	static int hex_char_value(char c);
	static int hex_to_decimal(const char* szHex, int len);
    size_t funcLight2();
	SYSTEMTIME m_tSendTime;
	bool m_bSendFlag;
	Q_ProtocolLightSetting*getSetting();
	time_t m_now;
	bool m_firstRun;
	//灯的数据
	bool handleFrameLight();
	curl_slist *m_pCookies;//存cookies
	
protected:
	bool makeFrameEx(Q_ProtocolLightCommand* pCommand);
	Q_ComCpuEndian m_cpuEndian;//cpu对齐方式;

	virtual bool explainDataEx(char* buff,int nCount,int& nCurrentPos);
	char m_LastMsg[Q_FRAME_CONST256];
private:
	Q_ProtocolLightCommand* m_LastCmd;
	vector<Q_ProtocolLightCommand*>m_vecCmd;

	int m_nMsgID;//消息编号;
	Q_ProtocolLightSetting* m_PrintSetting;
	
};
extern Q_ProtocolLight *g_Protocol;
#endif
