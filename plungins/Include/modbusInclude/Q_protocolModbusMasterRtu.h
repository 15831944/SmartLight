#pragma once

#ifndef __Q_PROTOCOL_MODBUSRTU_MASTER_H_INCLUDED__
#define __Q_PROTOCOL_MODBUSRTU_MASTER_H_INCLUDED__
#pragma warning(disable:4251)
#include "Q_protocolModbusMasterRtuCommand.h"
#include "Q_ModbusMasterRtuCommandManager.h"
#include "Q_protocolModbusMasterRtuSetting.h"
#define IRRADIANCE_ADDR 452
class __Q_MODBUSRTU_MASTER_EXPORT Q_protocolModbusMasterRtu : public Q_protocolBase
{
public:
	Q_protocolModbusMasterRtu(void);
	virtual ~Q_protocolModbusMasterRtu(void);

public:
	virtual bool				initializeProtocol();
	virtual bool				explainData();
	virtual bool				makeFrame();
	virtual bool				terminateProtocol();
	virtual bool				startProcess();
	virtual bool				stopProcess();

	void SetWndInfo(HWND wnd,int MsgID);
	//msgҪ��ӡ��Ϣ 
	//len ��ӡ��Ϣ����
	//type����0 ��Ϣ 1������Ϣ 2���� 3����
	void writefile(char *msg,int len, int type);
protected:
	bool makeFrameEx(Q_protocolModbusMasterRtuCommand* pCommand);
	//��ң������ 
    virtual bool explainFrameYC(Q_MODBUS_ASDU_RTU *asdu,BYTE *data);
	//�����ص�ң������
	virtual bool explainFrameYT(Q_MODBUS_ASDU_RTU *asdu,BYTE *data);
	//����ң������
	virtual bool handleFrameYT(Q_protocolModbusMasterRtuCommand* pCommand);
	//����ң������
	virtual bool handleFrameYC(Q_protocolModbusMasterRtuCommand* pCommand);

    bool fillTagInformation(LPQ_RECORDDATATAG pData,int nAdr,int nDevID);

    virtual bool explainDataEx(BYTE* buff,int nCount);
    virtual bool sendData(Q_MODBUS_ASDU_RTU *asdu,int len);
	//���ʹ�����
	bool sendErroCode(Q_MODBUS_ASDU_RTU*data,BYTE code);

	virtual void onRxMsg(BYTE *msgbuf, unsigned int len);
	void         onTxMsg(BYTE *msgbuf, unsigned int len);
	virtual void OnErrorMsg(char *msg,unsigned int len);
	virtual void OnShowMsg(char *msg,unsigned int len);

	Q_ComCpuEndian m_cpuEndian;//cpu���뷽ʽ;
    char chInfo[Q_FRAME_CONST128];
    char m_LastMsg[Q_FRAME_CONST1024];
    BOOL m_crcCheckOk;
    CRITICAL_SECTION m_cs;
private:
	Q_protocolModbusMasterRtuCommand* m_LastCmd;
	ModbusFloatUpSeq m_FloatUpSeq;
	ModbusYTMethod   m_YtMethod;
	HWND             m_wnd;
	int              m_nMsgID;//��Ϣ���;
	bool			 m_bFetchIrradiance;
public:
    WORD CRC16(BYTE* data, WORD len);
};



#endif
