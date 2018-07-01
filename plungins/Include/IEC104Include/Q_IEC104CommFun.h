
#if !defined(__Q_IEC104COMM_FUN_H__INCLUDED__)
#define __Q_IEC104COMM_FUN_H__INCLUDED__

#include "CommIEC104Def.h"

#if	defined(WIN32)||defined(WIN64)
#include <string.h>
void __Q_COMMIEC104_EXPORT NU_ZERO( void *buffer,size_t count );
#else


#endif

#ifdef WINDOWS_ENV
/*return 1 : little-endian, return 0:big-endian*/
int  __Q_COMMIEC104_EXPORT  checkCPUendian();
bool __Q_COMMIEC104_EXPORT  isBigEndian();
#endif
#endif