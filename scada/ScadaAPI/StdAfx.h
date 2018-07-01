// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__92A1CC2C_4E4A_4AEB_8563_7D43566D026A__INCLUDED_)
#define AFX_STDAFX_H__92A1CC2C_4E4A_4AEB_8563_7D43566D026A__INCLUDED_
#include "gSystemDef.h"

#ifdef WINDOWS_ENV
//提示sprintf不安全之类的错误 begin
#pragma warning(disable:4996) 
#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS
//提示sprintf不安全之类的错误 end

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif

#endif // !defined(AFX_STDAFX_H__92A1CC2C_4E4A_4AEB_8563_7D43566D026A__INCLUDED_)
