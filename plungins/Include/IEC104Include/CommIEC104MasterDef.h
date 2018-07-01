#ifndef __Q_COMMIEC104_TCP_MASTER_DEF_H_
#define __Q_COMMIEC104_TCP_MASTER_DEF_H_

#ifdef WINDOWS_ENV

#ifdef __Q_COMMIEC104MASTER_IMPORTING
#define __Q_COMMIEC104MASTER_EXPORT    __declspec(dllimport)
#else
#define __Q_COMMIEC104MASTER_EXPORT    __declspec(dllexport)
#endif

#endif


#ifdef UNIX_ENV
#define   __Q_COMMIEC104MASTER__EXPORT 
#endif

#endif
