#pragma once
#include <WINSOCK2.H>
#include <WS2TCPIP.H>


#pragma comment(lib,"WS2_32.LIB")


#include <vector>
using namespace std;



/** @brief define __Q_COMMPROTOCOL104_IMPORTING macro. */
#ifndef __Q_COMMIEC104SLAVE_IMPORTING
#define __Q_COMMIEC104SLAVE_IMPORTING
#endif

#include "CommIEC104SlaveDef.h"
#include "Q_104SlaveTask.h"
#include "Q_ProtocolIEC104Slave.h"


