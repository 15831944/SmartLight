// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the COMMIEC103_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// COMMIEC103_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef COMMIEC103_EXPORTS
#define COMMIEC103_API __declspec(dllexport)
#else
#define COMMIEC103_API __declspec(dllimport)
#endif

// This class is exported from the CommIEC104.dll
class COMMIEC103_API CCommIEC103 {
public:
	CCommIEC103(void);
	// TODO: add your methods here.
};

extern COMMIEC103_API int nCommIEC103;

COMMIEC103_API int fnCommIEC103(void);
