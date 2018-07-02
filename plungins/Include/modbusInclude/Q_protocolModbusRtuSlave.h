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
* @brief modbus rtu slave ͨѶ��Լ����
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
* @brief modbus tcp serverͨѶ��Լ�������
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
* @brief modbus tcp �����ͨѶ��Լ����
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
	//�趨ң��ĵ�ַ��Χ����Ч��ң�Ÿ���
	void               setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	//�趨ң�ŵĵ�ַ��Χ����Ч��ң�Ÿ���
	void               setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	//ң�⿪ʼ��ַ
	int                 getAnalogBegin();
	int                 getAnalogEnd();
	//ң�ſ�ʼ��ַ
	int                 getPointBegin();
	int                 getPointEnd();
	//ң�����
	int                 getAnalogCount();
	//ң�Ÿ���
	int                 getPointCount();
	//�ƻ����߿�ʼ��ַ

	//ң����ʼ�Ĵ�����ַ
	int                 getRegisterBegin();
	//ң����ʼ��Ȧ��ַ
	int                 getRelayBegin();
	//ң��
	int                 getRegisterWriteBegin();
	//ң�ص�ַ
	int                 getRelayWriteBegin();
	int                 getYkFunCode();//ң�صĹ�����
	int                 getYkOnValue();//ң�غϵ�ֵ
	int                 getYkOffValue();//ң�ضϿ���ֵ
	int                 getYcFunCode();//ң�⹦����
	int                 getYxFunCode();//ң�Ź�����
	int                 getYtFunCode();//ң��������
	int                 getSyncFirst();//ң��������
	bool                getPrintPacket();//��ӡ���ı�ʶ
	bool                getPrintError();//��ӡ������Ϣ��ʶ
	bool                getPrintPrompt();//��ʾ��Ϣ��ʶ
	bool                getSendClearBuffer();//�õ�����һ�����һ�»�������ʶ

	int getDeviceId();//���豸��ַ
	ModbusFloatUpSeq getFloatUpSeq();
	ModbusYTMethod   getYTMethod();
	//ң���ֽ�˳��
	ModbusFloatUpSeq getYTByteSeq();
	ModbusAnalogUpType getYcUpType();
	void SetWndInfo(HWND wnd,int MsgID);
	HWND getWndHwnd();
	int getWndMsgID();
	int getWndCmdID();
	virtual bool reloadLog();


protected:
	vector<ST_ModCfgT > m_vContent;//����
	vector<ST_ModCfgT > m_vGroup;//������
private:
	char m_sFilePath[MAX_PATH];//�ļ�·��
	char LastError[Q_FRAME_CONST1024];         //������Ϣ
	bool init();
private:
	int m_alg_count;//ң�����
	int m_alg_begin;//ң����ʼ��ַ
	int m_alg_end;//ң�������ַ
	int m_pnt_count;//ң��Ϣ����
	int m_pnt_begin;//ң����ʼ��ַ
	int m_pnt_end;//ң�Ž�����ַ
	int m_plan_addr_begin;//�ƻ����߿�ʼ��ַ
	int m_plan_addr_end;//�ƻ����߽�����ַ
	int m_plan_count;//�ƻ�������Ϣ����
	HWND    m_wnd;
	int m_nMsgID;//��Ϣ���;
	int m_nCmdID;


};


/** 
* @interface Q_protocolModbusTcpServerStandard
* @brief Modbus tcp�����ͨѶ��Լ
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

	//msgҪ��ӡ��Ϣ 
	//len ��ӡ��Ϣ����
	//type����0 ��Ϣ 1������Ϣ 2���� 3����
	void writefile(char *msg,int len, int type);
protected:

	Q_ComCpuEndian m_cpuEndian;//cpu���뷽ʽ;
	HWND m_wnd;
	int m_nMsgID;
	//��ң������ 
	//dataȥ��ǰ�����ֽ�Address��Function,������CRCУ��������
	//len ���������ĳ���,Ҳ����ȥ��adddress+function���ּ���crc�����ֽڣ����Դ�ƽ
	virtual bool explainFrameYC(Q_MODBUS_ASDU_RTU *header,BYTE *data);
	//��ң��
	virtual bool explainFrameYX(Q_MODBUS_ASDU_RTU *header,BYTE *data);
	//ң��
	virtual bool explainFrameYK(Q_MODBUS_ASDU_RTU *header,BYTE *data);
	//ң��
	virtual bool explainFrameYT(Q_MODBUS_ASDU_RTU *header,BYTE *data);
	virtual bool explainFrameYT_M(Q_MODBUS_ASDU_RTU *header,BYTE *data);
	//len ��asdu �д�addr��ʼ�ı������ĳ���
	virtual bool sendData(Q_MODBUS_ASDU_RTU *asdu,int len);
	virtual bool explainDataEx(char* buff,int nCount,int& nCurrentPos);
	//���ʹ�����
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
