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
	//asdu 发送的数据内容
	//len 为asdu.data区中数据的长度
protected:
	bool findSyncIndex(char* buff,int buffSize,int startPos,int& findPos);
	virtual bool sendData(Q_MODBUS_ASDU2 *asdu,int len);
	virtual bool explainDataEx(char* buff,int nCount,int& nCurrentPos);
};
#endif