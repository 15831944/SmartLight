#pragma  once
/** @brief Define dllexport or dllimport macro. */
#ifdef __Q_COMMMODBUSPLANCURVE_IMPORTING
#define __Q_COMMMODBUSPLANCURVE_EXPORT    __declspec(dllimport)
#else
#define __Q_COMMMODBUSPLANCURVE_EXPORT    __declspec(dllexport)
#endif

enum eTypePlanCurve
{
	eCurveRemote = 1,//远方，调度的
	eCurveLocal = 2//本地，
};