#pragma  once
/** @brief Define dllexport or dllimport macro. */
#ifdef __Q_COMMMODBUSRTU_IMPORTING
#define __Q_COMMMODBU_RTU_SLAVE_EXPORT    __declspec(dllimport)
#else
#define __Q_COMMMODBU_RTU_SLAVE_EXPORT    __declspec(dllexport)
#endif
