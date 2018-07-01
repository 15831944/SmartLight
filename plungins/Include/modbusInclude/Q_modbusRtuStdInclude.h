//*
//*
//* Q_modbusTcpServerStandInclude.h
//*
//*

#ifndef __Q_MODBUS_RTU_STD_DINCLUDE_H_INCLUDE__
#define __Q_MODBUS_RTU_STD_DINCLUDE_H_INCLUDE__

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




#ifndef __Q_MODBUS_RTU_STD_IMPORTING
#define __Q_MODBUS_RTU_STD_IMPORTING
#endif


#include "Q_ModbusRtuStdDef.h"
#include "Q_ModbusDefine.h"
#include "Q_protocolModbusRtuStd.h"




#endif//__Q_MODBUSTCPSERVERSTANDARDINCLUDE_H_INCLUDE__