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
	//�Ƶ����ݷ���
	static size_t funcGetLight(void *strptr, size_t size, size_t nmemb, void *stream);
	//�Ƶ�����
	bool handleFrameLight(string url);
	//������ƵĿ���
	bool funSetGroupLight(BYTE DEVICE[],string url);
	//���Ƶ����ƵĿ���
	bool funSetSingleLight(BYTE DEVICE[],string url);
	//������Ƶ�����
	bool funSetGroupBLight(BYTE DEVICE[]);
	//���Ƶ����Ƶ�����
	bool funSetSingleBLight(BYTE DEVICE[]);

private:
	int m_nMsgID;//��Ϣ���
};