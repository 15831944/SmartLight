// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the COMMIEC104_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// COMMIEC104_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef COMMIEC104_EXPORTS
#define COMMIEC104_API __declspec(dllexport)
#else
#define COMMIEC104_API __declspec(dllimport)
#endif

// This class is exported from the CommIEC104.dll
class COMMIEC104_API CCommIEC104 {
public:
	CCommIEC104(void);
	// TODO: add your methods here.
};

extern COMMIEC104_API int nCommIEC104;

COMMIEC104_API int fnCommIEC104(void);
