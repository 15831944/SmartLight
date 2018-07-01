#pragma once
#if !defined(__Q_IEC103COMM_FUN_H__INCLUDED__)
#define __Q_IEC103COMM_FUN_H__INCLUDED__

#include "CommIEC103Def.h"

#if	defined(WIN32)||defined(WIN64)
#include <string.h>
void  NU_ZERO( void *buffer,size_t count );
#else


#endif

/*return 1 : little-endian, return 0:big-endian*/
int    checkCPUendian();
bool   isBigEndian();

#endif