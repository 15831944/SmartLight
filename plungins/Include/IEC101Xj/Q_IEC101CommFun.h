
#if !defined(__Q_IEC104COMM_FUN_H__INCLUDED__)
#define __Q_IEC104COMM_FUN_H__INCLUDED__



#if	defined(WIN32)||defined(WIN64)
#include <string.h>
void  NU_ZERO( void *buffer,size_t count );
#else


#endif

#ifdef WINDOWS_ENV
/*return 1 : little-endian, return 0:big-endian*/
int checkCPUendian();
bool isBigEndian();
#endif
#endif