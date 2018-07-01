#if !defined(__Q_CommIEC104Def_H__INCLUDED__)
#define __Q_CommIEC104Def_H__INCLUDED__
/** @brief Define dllexport or dllimport macro. */

#ifdef WINDOWS_ENV

#ifdef __Q_COMM101Xj_IMPORTING
#define __Q_COMMIEC101_EXPORT    __declspec(dllimport)
#else
#define __Q_COMMIEC101_EXPORT    __declspec(dllexport)
#endif

#endif //end WINDOWS_ENV

#ifdef UNIX_ENV
#define   __Q_COMMIEC101_EXPORT 
#endif



#endif