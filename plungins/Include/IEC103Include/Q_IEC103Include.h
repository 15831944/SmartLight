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
#ifndef __Q_COMMIEC103_IMPORTING
#define __Q_COMMIEC103_IMPORTING
#endif



#include "Q_103StdDefine.h"
#include "Q_IEC103Command.h"
#include "Q_protocolIEC103.h"
#include "Q_IEC103Setting.h"
#include "Q_103StandardTags.h"
#include "Q_IEC103CommFun.h"

