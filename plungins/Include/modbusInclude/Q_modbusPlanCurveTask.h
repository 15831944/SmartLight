//*
//*
//* Q_modbusPlanCurveTask.h
//*
//*
#if !defined(__Q_modbusPlanCurveTask_H__INCLUDED__)
#define __Q_modbusPlanCurveTask_H__INCLUDED__


#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)



#include "Q_ModbusPlanCurveDef.h"
#include "Q_ModbusDefine.h"






/** 
 * @interface 
 * @brief Q_modbusPlanCurveTask class declare
 * @author yangxi
 * @date 2012-5-23
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMMODBUSPLANCURVE_EXPORT Q_modbusPlanCurveTask : public Q_tcpServerTask
{
public:
	Q_modbusPlanCurveTask(void);
	virtual ~Q_modbusPlanCurveTask(void);


public:
	virtual bool					acceptClient();


};




#pragma warning(pop)

#endif // !defined(__Q_modbusPlanCurveTask_H__INCLUDED__)


