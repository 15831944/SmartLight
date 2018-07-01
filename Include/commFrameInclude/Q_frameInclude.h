#ifndef __Q_FRAME_INCLUDE_H_
#define __Q_FRAME_INCLUDE_H_
/** 
 * @interface 
 * @brief frame classes include
 * @author yangxi
 * @date 2011-12-28
 * @see 
 * @bug No know bugs so far.
 */
//#ifdef WINDOWS_ENV
//#include <WINSOCK2.H>
//#include <WS2TCPIP.H>
//#pragma comment(lib,"WS2_32.LIB")
//#include <stdio.h>
//#include <assert.h>

//#endif

#include <vector>
#include <map>
using namespace std;

/** @brief define __Q_COMMFRAME_IMPORTING macro. */
#ifndef __Q_COMMFRAME_IMPORTING
#define __Q_COMMFRAME_IMPORTING
#endif

#include "Q_frameDefine.h"
#include "Q_protocolBase.h"
#include "Q_frameAssistant.h"
#include "Q_serialLink.h"
#include "Q_tcpClientLink.h"
#include "Q_simpleThread.h"
#include "Q_taskBase.h"
#include "Q_taskManager.h"
#include "Q_dataManager.h"
#include "Q_globalData.h"

#include "Q_canbusLink.h"

#include "Q_tcpServerRunningUnit.h"
#include "Q_reserveManager.h"
#include "Q_reservethread.h"
#include "Q_udpClientLink.h"
#include "Q_udpServerLink.h"
#include "FEPFuncRtdb.h"


#endif