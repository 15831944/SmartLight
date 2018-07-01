#pragma  once
#ifdef __Q_MODBUSRTU_MASTER_IMPORTING
#define __Q_MODBUSRTU_MASTER_EXPORT    __declspec(dllimport)
#else
#define __Q_MODBUSRTU_MASTER_EXPORT    __declspec(dllexport)
#endif
