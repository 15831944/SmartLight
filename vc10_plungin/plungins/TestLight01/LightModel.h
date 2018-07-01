#ifndef __Q_PROTOCOL_MODBUSTCP_MASTER_H_INCLUDED__
#define __Q_PROTOCOL_MODBUSTCP_MASTER_H_INCLUDED__

#endif
#ifdef WINDOWS_ENV
#pragma warning(disable:4251)
#endif
#include "F:\Gkpro\ldns\code\include\commFrameInclude\Q_frameInclude.h"
#include "F:\Gkpro\ldns\code\Include\commFrameInclude\Q_runingUnitDefine.h"
#include "F:\Gkpro\ldns\code\Include\commFrameInclude\Q_runningUnit.h"
#include "F:\Gkpro\ldns\code\plungins\CommHttpQHCGQ\Q_HttpDBQHDefine.h"
#include "F:\Gkpro\ldns\code\Include\commFrameInclude\Q_protocolBase.h"
class LightCommand : public Q_protocolCommandBase
{
public:
	LightCommand(void);
	virtual ~LightCommand(void);
};
class LightControl :public Q_protocolBase
{
public:
	LightControl(void);
	virtual ~LightControl(void);
	void SendMsgPrint(int nchannelid, int nmsgtype, BYTE* mse, int nlen);
public:
	virtual bool				initializeProtocol();
	virtual bool				explainData();
	virtual bool				makeFrame();
	virtual bool				terminateProtocol();
	virtual bool				startProcess();
	virtual bool				stopProcess();
	//灯的数据返回
	static size_t funcGetLight(void *strptr, size_t size, size_t nmemb, void *stream);
	//灯的数据
	bool handleFrameLight(string url);
	//控制组灯的开关
	bool funSetGroupLight(BYTE DEVICE[],string url);
	//控制单个灯的开关
	bool funSetSingleLight(BYTE DEVICE[],string url);
	//控制组灯的明暗
	bool funSetGroupBLight(BYTE DEVICE[]);
	//控制单个灯的明暗
	bool funSetSingleBLight(BYTE DEVICE[]);

private:
	int m_nMsgID;//消息编号
};