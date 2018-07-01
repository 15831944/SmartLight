#ifndef __Q_TINYXMLDEF_H_
#define __Q_TINYXMLDEF_H_

#ifdef WINDOWS_ENV
#ifdef __Q_TINYXML_IMPORTING
#define __Q_TINYXML_EXPORT    __declspec(dllimport)
#else
#define __Q_TINYXML_EXPORT     __declspec(dllexport)
#endif
#endif

#ifdef UNIX_ENV
#define   __Q_TINYXML_EXPORT 
#endif

#endif