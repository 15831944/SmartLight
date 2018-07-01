/////////////////////////////////////////////////////////////////////////////
// StdAfx.h : include file for standard system include files				*
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_STDAFX_OF_RTDBDLL_H__
#define __AFX_STDAFX_OF_RTDBDLL_H__
#ifdef WINDOWS_ENV
#ifndef _WIN32_WINNT 
#define _WIN32_WINNT 0x0502 
#endif
#endif

#include "gSystemDef.h"

#ifndef UNIX_ENV
#pragma warning(disable:4996) 
#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS
//提示sprintf不安全之类的错误


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")


//{{AFX_INSERT_LOCATION}}

#endif

#endif // __AFX_STDAFX_OF_RTDBDLL_H__
