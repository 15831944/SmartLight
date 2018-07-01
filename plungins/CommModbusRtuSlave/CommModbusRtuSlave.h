// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the COMMMODBUSRTUSLAVE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// COMMMODBUSRTUSLAVE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef COMMMODBUSRTUSLAVE_EXPORTS
#define COMMMODBUSRTUSLAVE_API __declspec(dllexport)
#else
#define COMMMODBUSRTUSLAVE_API __declspec(dllimport)
#endif

// This class is exported from the CommModbusRtuSlave.dll
class COMMMODBUSRTUSLAVE_API CCommModbusRtuSlave {
public:
	CCommModbusRtuSlave(void);
	// TODO: add your methods here.
};

extern COMMMODBUSRTUSLAVE_API int nCommModbusRtuSlave;

COMMMODBUSRTUSLAVE_API int fnCommModbusRtuSlave(void);
