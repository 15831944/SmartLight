#pragma once
/** @brief Define dllexport or dllimport macro. */
#ifdef __Q_COMMIEC103_IMPORTING
#define __Q_COMMIEC103_EXPORT    __declspec(dllimport)
#else
#define __Q_COMMIEC103_EXPORT    __declspec(dllexport)
#endif