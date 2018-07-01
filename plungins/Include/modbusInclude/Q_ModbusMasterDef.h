#pragma once

/** @brief Define dllexport or dllimport macro. */
#ifdef __Q_MODBUS_STD_IMPORTING
#define __Q_MODBUS_STD_EXPORT    __declspec(dllimport)
#else
#define __Q_MODBUS_STD_EXPORT    __declspec(dllexport)
#endif

//#ifndef __Q_COMM_MODBUS_STD_DEF__H__
//#define __Q_COMM_MODBUS_STD_DEF__H__
//
//#ifdef WINDOWS_ENV
//#ifdef __Q_MODBUS_STD_IMPORTING
//#define __Q_MODBUS_STD_EXPORT    __declspec(dllimport)
//#else
//#define __Q_MODBUS_STD_EXPORT    __declspec(dllexport)
//#endif
//#endif
//
//#ifdef UNIX_ENV
//#define   __Q_MODBUS_STD_EXPORT 
//#endif
//
//#endif