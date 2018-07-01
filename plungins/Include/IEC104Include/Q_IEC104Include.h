/** 
 * @interface 
 * @brief protocol104 classes include
 * @author 
 * @date 2011-12-28
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



/** @brief define __Q_COMMPROTOCOL104_IMPORTING macro. */
#ifndef __Q_COMMIEC104_IMPORTING
#define __Q_COMMIEC104_IMPORTING
#endif



#include "Q_104StdDefine.h"
#include "Q_IEC104Command.h"
#include "Q_protocolIEC104.h"
#include "Q_IEC104Setting.h"
#include "Q_104StandardTags.h"
#include "Q_IEC104CommFun.h"

