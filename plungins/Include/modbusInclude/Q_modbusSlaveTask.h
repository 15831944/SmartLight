#ifndef __Q_MODBUSTCPSERVERTASK_H__INCLUDED__
#define __Q_MODBUSTCPSERVERTASK_H__INCLUDED__


#ifdef WINDOWS_ENV
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif

#include "Q_frameInclude.h"
#include "Q_ModbusSlaveDef.h"
#include "Q_ModbusDefine.h"






/** 
 * @interface 
 * @brief Q_modbusTcpServerTask class declare
 * @author yangxi
 * @date 2012-5-23
 * @see 
 * @bug No know bugs so far.
 */
//__Q_COMMMODBUSTCPSERVERSTANDARD_EXPORT
class  Q_modbusSlaveTask : public Q_tcpServerTask
{
public:
	Q_modbusSlaveTask(void);
	virtual ~Q_modbusSlaveTask(void);


public:
	virtual bool					acceptClient();


};




#pragma warning(pop)

#endif // !defined(__Q_MODBUSTCPSERVERTASK_H__INCLUDED__)


