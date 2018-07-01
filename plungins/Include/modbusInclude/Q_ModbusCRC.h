#pragma once
#include "Q_ModbusSlaveDef.h"
#include "Q_ModbusDefine.h"

class __Q_COMMMODBUSTCPSERVERSTANDARD_EXPORT Q_ModbusCRC
{
public:
	static unsigned short Q_ModbusCRC::CRC16(BYTE *puchMsg,unsigned short usDataLen);
private:
	static const BYTE auchCRCHi[];
	static const BYTE auchCRCLo[];
};