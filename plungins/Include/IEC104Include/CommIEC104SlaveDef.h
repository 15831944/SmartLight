#pragma once
/** @brief Define dllexport or dllimport macro. */
#ifdef __Q_COMMIEC104SLAVE_IMPORTING
#define __Q_COMMIEC104SLAVE_EXPORT    __declspec(dllimport)
#else
#define __Q_COMMIEC104SLAVE_EXPORT    __declspec(dllexport)
#endif