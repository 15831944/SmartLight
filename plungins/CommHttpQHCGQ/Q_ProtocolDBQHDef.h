#ifndef __Q_MODBUS_TCP_MASTER_DEF_H_
#define __Q_MODBUS_TCP_MASTER_DEF_H_

#ifdef WINDOWS_ENV

#ifdef __Q_HTTP_QINGHUA_IMPORTING
#define __Q_HTTP_QINGHUA_EXPORT    __declspec(dllimport)
#else
#define __Q_HTTP_QINGHUA_EXPORT    __declspec(dllexport)
#endif

#endif


#ifdef UNIX_ENV
#define   __Q_HTTP_QINGHUA_EXPORT 
#endif

#endif
