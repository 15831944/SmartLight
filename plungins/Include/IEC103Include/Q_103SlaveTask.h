//*
//*
//* Q_103SlaveTask.h
//*
//*
#if !defined(__Q_103TCPSERVERTASK_H__INCLUDED__)
#define __Q_103TCPSERVERTASK_H__INCLUDED__


#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)



#include "CommIEC103SlaveDef.h"
#include "Q_103StdDefine.h"




/** 
* @interface 
* @brief Q_modbusTcpServerTask class declare
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class  Q_103SlaveTask : public Q_tcpServerTask
{
public:
	Q_103SlaveTask(void);
	virtual ~Q_103SlaveTask(void);


public:
	virtual bool					acceptClient();
};




#pragma warning(pop)

#endif // !defined(__Q_MODBUSTCPSERVERTASK_H__INCLUDED__)


