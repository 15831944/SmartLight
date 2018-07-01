#ifndef __Q_CFUNCUTIL_DEF__H__
#define __Q_CFUNCUTIL_DEF__H__

#ifdef WINDOWS_ENV
#ifdef __Q_CFUNCUTIL_IMPORTING
#define __Q_CFUNCUTIL_EXPORT    __declspec(dllimport)
#else
#define __Q_CFUNCUTIL_EXPORT    __declspec(dllexport)
#endif
#endif

#ifdef UNIX_ENV
#define   __Q_CFUNCUTIL_EXPORT 
#endif

#endif