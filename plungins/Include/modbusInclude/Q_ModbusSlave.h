#pragma once

#ifndef Q_MODBUS_SLAVE_H
#define Q_MODBUS_SLAVE_H

#include "Q_ModbusDef.h"
#include "Q_ModbusDefine.h"
#include "Q_protocolBase.h"

class __Q_COMMMODBUS_EXPORT Q_ModbusSlave : public Q_protocolBase
{
public:
	Q_ModbusSlave(void);
	virtual ~Q_ModbusSlave(void);

public:
	virtual bool				initializeProtocol();
	virtual bool				explainData();
	virtual bool				makeFrame();
	virtual bool				terminateProtocol();
	virtual bool				startProcess();
	virtual bool				stopProcess();
private:
	//��ң������ 
	//dataȥ��ǰ�����ֽ�Address��Function,������CRCУ��������
	//len ���������ĳ���,Ҳ����ȥ��adddress+function���ּ���crc�����ֽڣ����Դ�ƽ
	virtual bool explainFrameYC(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//��ң��
	virtual bool explainFrameYX(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//ң��
	virtual bool explainFrameYK(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//ң��
	virtual bool explainFrameYT(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//asdu ���͵���������
	//len Ϊasdu.data�������ݵĳ���
protected:
	bool findSyncIndex(char* buff,int buffSize,int startPos,int& findPos);
	virtual bool sendData(Q_MODBUS_ASDU2 *asdu,int len);
	virtual bool explainDataEx(char* buff,int nCount,int& nCurrentPos);
};
#endif