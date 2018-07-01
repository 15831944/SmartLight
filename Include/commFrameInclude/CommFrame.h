// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the COMMFRAME_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// COMMFRAME_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef COMMFRAME_EXPORTS
#define COMMFRAME_API __declspec(dllexport)
#else
#define COMMFRAME_API __declspec(dllimport)
#endif



// This class is exported from the CommFrame.dll
class COMMFRAME_API CCommFrame {
public:
	CCommFrame(void);
	// TODO: add your methods here.
};

extern COMMFRAME_API int nCommFrame;

COMMFRAME_API int fnCommFrame(void);
