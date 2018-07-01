//*
//*
//* Q_channelBase.h
//*
//*
#if !defined(__Q_TASKBASE_H__INCLUDED__)
#define __Q_TASKBASE_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif


#include "Q_frameDefine.h"
#include "Q_frameAssistant.h"
#include "Q_channelBase.h"


/** 
 * @interface 
 * @brief Q_taskBase class declare
 * @author yangxi
 * @date 2012-1-1
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_taskBase
{
public:
	Q_taskBase(void);
	virtual ~Q_taskBase(void);
public:

	virtual bool				initializeTask(void);
	virtual bool				terminateTask(void);
    virtual bool				stopTask(void);
	virtual bool				checkTask(void);
	virtual bool				receiveData(void);
	virtual bool				sendData(void);
	virtual bool				explainData(void);
	virtual bool				clearData(void);

	virtual bool				switchPrimaryChannel(void);
	virtual bool				sendMessage(void);

	virtual bool				addProtocolCommand(Q_protocolCommandBase* pCommand);
	virtual bool				addProtocolCommand(Q_CommandVal* pCommand);


	bool						addChannel(Q_channelBase* pChannel);
	Q_channelBase*				getPrimaryChannel(void);
	Q_CommonStatusType			getPrimaryChannelStatus(void);
	void						setBackupMode(Q_CommonBackupMode eMode) { m_eBackupMode = eMode;}
	inline void					setSwitchMode(bool bAutoSwitch) { m_bAutoSwitchChannel = bAutoSwitch;}
	inline bool					getSwitchMode(void) { return m_bAutoSwitchChannel;}
	int                         getChannelCount(void);
	Q_channelBase*              getChannelByIndex(int i);
	//2015-06-23 实时库接口 tjs
	void setRtdbInf(Q_dataManager *mgr);
	//2016.5.19 增加检测是否主机运行代码 wenjp
	//备注：谭井胜2016-10-27
	//type 0时不对通道做任何操作,1当为备机时如果当前状态为运行，关闭连接,如果当前状态为退出，启动链接
	//返回值true主机运行 flase备机运行
	bool CheckMasterRun(int type=0);

protected:
	Q_CommonBackupMode			m_eBackupMode;
	int							m_nChannelCount;
	bool						m_bAutoSwitchChannel;
	Q_channelBase*				m_arrayChannel[Q_TASK_MAXCHANNELNUMBER];
	Q_dataManager *m_pMgr;
	//2016.5.19 增加检测是否主机运行代码 wenjp
    time_t tLastCheck;
	time_t tTaskCheck;
    int tLastStatus;
};
#ifdef WINDOWS_ENV

#pragma warning(pop)
#endif

#endif // !defined(__Q_TASKBASE_H__INCLUDED__)
