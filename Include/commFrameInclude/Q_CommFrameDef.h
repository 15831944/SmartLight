#ifndef __Q_COMMMFRAME_DEF__H__
#define __Q_COMMMFRAME_DEF__H__

#ifdef WINDOWS_ENV
#ifdef __Q_COMMFRAME_IMPORTING
#define __Q_COMMFRAME_EXPORT    __declspec(dllimport)
#else
#define __Q_COMMFRAME_EXPORT    __declspec(dllexport)
#endif
#endif

#ifdef UNIX_ENV
#define   __Q_COMMFRAME_EXPORT 
#endif

#endif