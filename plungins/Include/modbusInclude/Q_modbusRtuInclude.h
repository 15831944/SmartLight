//*
//*
//* Q_modbusTcpServerStandInclude.h
//*
//*

#ifndef __Q_MODBUS_RTU_INCLUDE_H_INCLUDE__
#define __Q_MODBUS_RTU_INCLUDE_H_INCLUDE__

/** 
* @interface 
* @brief modbus tcp server classes include
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/

#include <WINSOCK2.H>
#include <WS2TCPIP.H>


#pragma comment(lib,"WS2_32.LIB")


#include <vector>
using namespace std;


#include <stdio.h>
#include <assert.h>




#ifndef __Q_YH_MODBUS_MASTER_IMPORTING
#define __Q_YH_MODBUS_MASTER_IMPORTING
#endif


#ifndef __Q_MODBUSRTU_MASTER_IMPORTING
#define __Q_MODBUSRTU_MASTER_IMPORTING
#endif

#include "Q_ModbusDefine.h"
#include "Q_protocolModbusMasterRtu.h"
#include "Q_protocolModbusMasterRtuCommand.h"
#include "Q_protocolModbusMasterRtuSetting.h"
#include "Q_ModbusMasterRtuCommandManager.h"
#include "Q_YHModbusMasterDef.h"
#include "Q_YHModbusMasterCommandManager.h"
#include "Q_protocolYHModbusMaster.h"
#include "Q_protocolYHModbusMasterCommand.h"
#include "Q_protocolYHModbusMasterSetting.h"
#endif//__Q_MODBUSTCPSERVERSTANDARDINCLUDE_H_INCLUDE__