// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the COMMMODBUSRTUSTD_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// COMMMODBUSRTUSTD_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef COMMMODBUSRTUSTD_EXPORTS
#define COMMMODBUSRTUSTD_API __declspec(dllexport)
#else
#define COMMMODBUSRTUSTD_API __declspec(dllimport)
#endif

// This class is exported from the CommModbusRtuStd.dll
class COMMMODBUSRTUSTD_API CCommModbusRtuStd {
public:
	CCommModbusRtuStd(void);
	// TODO: add your methods here.
};

extern COMMMODBUSRTUSTD_API int nCommModbusRtuStd;

COMMMODBUSRTUSTD_API int fnCommModbusRtuStd(void);
