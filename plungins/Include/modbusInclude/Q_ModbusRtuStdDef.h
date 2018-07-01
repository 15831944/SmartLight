//#pragma  once
/** @brief Define dllexport or dllimport macro. */
#ifndef __Q_MODBUS_RTU_STD01_SERVERSTANDARD_DEF_H_
#define __Q_MODBUS_RTU_STD01_SERVERSTANDARD_DEF_H_

#ifdef WINDOWS_ENV

#ifdef __Q_MODBUS_RTU_STD_IMPORTING
#define __Q_MODBUS_RTU_STD_EXPORT    __declspec(dllimport)
#else
#define __Q_MODBUS_RTU_STD_EXPORT    __declspec(dllexport)
#endif
#endif

#ifdef UNIX_ENV
#define   __Q_MODBUS_RTU_STD_EXPORT 
#endif

#endif