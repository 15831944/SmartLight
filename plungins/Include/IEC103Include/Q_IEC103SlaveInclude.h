#pragma once
#include <WINSOCK2.H>
#include <WS2TCPIP.H>


#pragma comment(lib,"WS2_32.LIB")


#include <vector>
using namespace std;



/** @brief define __Q_COMMPROTOCOL103_IMPORTING macro. */
#ifndef __Q_COMMIEC103SLAVE_IMPORTING
#define __Q_COMMIEC103SLAVE_IMPORTING
#endif

#include "CommIEC103SlaveDef.h"
#include "Q_103SlaveTask.h"
#include "Q_ProtocolIEC103Slave.h"


