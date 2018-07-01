// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#ifndef __AFX_STDAFX_OF_FRAMEDLL_H__
#define __AFX_STDAFX_OF_FRAMEDLL_H__

#ifdef WINDOWS_ENV
#ifndef _WIN32_WINNT 
#define _WIN32_WINNT 0x0502 
#endif
#endif

#include "gSystemDef.h"

#ifndef UNIX_ENV
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif
#pragma warning(disable:4996) 
#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS

//#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>                     // MFC support for Windows Common Controls
//#endif // _AFX_NO_AFXCMN_SUPPORT


#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")
#endif
#include <iostream>
// Windows Header Files:
//#include <windows.h>


//#ifdef _DEBUG
//#include "vld.h "
// TODO: reference additional headers your program requires here
//#endif
#endif