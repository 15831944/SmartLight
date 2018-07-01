//*
//*
//* Q_modbusTcpServerStandInclude.h
//*
//*

#ifndef __Q_MODBUSTCPSERVERSTANDARDINCLUDE_H_INCLUDE__
#define __Q_MODBUSTCPSERVERSTANDARDINCLUDE_H_INCLUDE__

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




#ifndef __Q_COMMMODBUSTCPSERVERSTANDARD_IMPORTING
#define __Q_COMMMODBUSTCPSERVERSTANDARD_IMPORTING
#endif


#include "Q_ModbusSlaveDef.h"
#include "Q_ModbusDefine.h"
#include "Q_modbusSlaveTask.h"
#include "Q_protocolModbusSlave.h"





#endif//__Q_MODBUSTCPSERVERSTANDARDINCLUDE_H_INCLUDE__