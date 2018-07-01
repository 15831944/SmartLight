//*
//*
//* Q_modbusTcpServerTask.h
//*
//*
#if !defined(__Q_104TCPSERVERTASK_H__INCLUDED__)
#define __Q_104TCPSERVERTASK_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif


#include "CommIEC104SlaveDef.h"
#include "Q_104StdDefine.h"


#define DATA_UPLOAD_DELAY  30 //IEC104子站数据延时上送时间，单位秒。为防止山东现场数据跳变引起的考核

/** 
* @interface 
* @brief Q_modbusTcpServerTask class declare
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
#include "Q_ProtocolIEC104Slave.h"
class Q_104SlaveTask : public Q_tcpServerTask
{
public:
	Q_104SlaveTask(void);
	virtual ~Q_104SlaveTask(void);


public:
	virtual bool					acceptClient();
private:
	//判断是否需要重新Setting文件
	//chid 是通道号
	//返回值true重新读取配置文件初使化Setting
	//false不重新读取配置文件
	bool checkReInitSetting(int chid,char out_strFilePath[]);
	//初使化新的文件
	bool initNewSetting(int chid,Q_IEC104Setting* proSetting);
	time_t m_creatTime;
};



#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif

#endif // !defined(__Q_MODBUSTCPSERVERTASK_H__INCLUDED__)


