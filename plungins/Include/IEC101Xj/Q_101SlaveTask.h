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


#define DATA_UPLOAD_DELAY  30 //IEC104��վ������ʱ����ʱ�䣬��λ�롣Ϊ��ֹɽ���ֳ�������������Ŀ���

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
	//�ж��Ƿ���Ҫ����Setting�ļ�
	//chid ��ͨ����
	//����ֵtrue���¶�ȡ�����ļ���ʹ��Setting
	//false�����¶�ȡ�����ļ�
	bool checkReInitSetting(int chid,char out_strFilePath[]);
	//��ʹ���µ��ļ�
	bool initNewSetting(int chid,Q_IEC104Setting* proSetting);
	time_t m_creatTime;
};



#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif

#endif // !defined(__Q_MODBUSTCPSERVERTASK_H__INCLUDED__)


