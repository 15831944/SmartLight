#pragma once
/** @brief Define dllexport or dllimport macro. */
#ifdef __Q_COMMIEC103SLAVE_IMPORTING
#define __Q_COMMIEC103SLAVE_EXPORT    __declspec(dllimport)
#else
#define __Q_COMMIEC103SLAVE_EXPORT    __declspec(dllexport)
#endif