//*
//*
//* Q_protocolBase.h
//*
//*
#if !defined(__Q_PROTOCOLBASE_H__INCLUDED__)
#define __Q_PROTOCOLBASE_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif



#include "Q_frameAssistant.h"
#include "ThreadMutex.h"
#include "Q_dataManager.h"



/** 
 * @interface 
 * @brief Q_protocolSettingBase class declare
 * @author yangxi
 * @date 2011-12-30
 * @see 
 * @bug No know bugs so far.
 */

//fs20150820addupchannel
class __Q_COMMFRAME_EXPORT Q_protocolSettingBase
{
public:
	Q_protocolSettingBase(void);
	virtual ~Q_protocolSettingBase(void);
	virtual bool reloadLog(void) = 0;//重新加载Log配置
    void setChannelID(int v_chid);
	int getChannelID(void);	
	void setChannelType(int v_chtype);
	int getChannelType(void);	
	void setChannelClass(const char *className);
	const char* getChannelClass();
protected:
	int m_channel_id;//通道id
	int m_channel_type; 
	//char m_className[MAX_PATH];//类名称，用于子类

};




/** 
 * @interface 
 * @brief Q_protocolCommandBase class declare
 * @author yangxi
 * @date 2011-12-30
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_protocolCommandBase
{
public:
	Q_protocolCommandBase(void);
	virtual ~Q_protocolCommandBase(void);



public:
	inline void								setCommonType(Q_CommandType eType) { m_eCommandType = eType;}
	inline Q_CommandType					getCommonType(void) { return m_eCommandType;}
	inline void								setCommonTypeExt(Q_CommandTypeExt eType) { m_eCommandTypeExt = eType;}
	inline Q_CommandTypeExt					getCommonTypeExt(void) { return m_eCommandTypeExt;}


protected:
	Q_CommandType							m_eCommandType;
	Q_CommandTypeExt						m_eCommandTypeExt;

};
//命令扩展
class __Q_COMMFRAME_EXPORT Q_protocolCommandBaseEx:public Q_protocolCommandBase
{
public:
	Q_protocolCommandBaseEx(void);
	virtual ~Q_protocolCommandBaseEx(void);



public:
	int       m_param_1;//参数1
	int       m_param_2;//参数2
	int       m_param_3;//参数3

};



/** 
 * @interface 
 * @brief Q_protocolCommandManagerBase class declare
 * @author yangxi
 * @date 2011-12-28
 * @see 
 * @bug No know bugs so far.
 */

class __Q_COMMFRAME_EXPORT Q_protocolCommandManagerBase
{
public:
	Q_protocolCommandManagerBase(void);
	virtual ~Q_protocolCommandManagerBase(void);


public:
	virtual bool							addCommand(Q_protocolCommandBase* pCommand);
	virtual bool							addCommand(Q_CommandVal *v_val);
	virtual bool							clearList(void);
	virtual bool							removeCommandByIndex(int nIndex);
	virtual Q_protocolCommandBase*			getCommandByIndex(int nIndex);
	Q_protocolCommandBase**                 getCommandInfo(void);
	int getCommandCount();
	int getCurrentCommandCount();
protected:

	CThreadMutex						m_csHandle;
	Q_protocolCommandBase*					m_commandList[Q_COMMANDLIST_MAX];
	int m_nCurrentCommandCount;
	
};



/** 
 * @interface 
 * @brief Q_protocolConfig class declare
 * @author yangxi
 * @date 2011-12-28
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_protocolConfig
{
public:
	Q_protocolConfig(void);
	virtual ~Q_protocolConfig(void);


public:
	inline Q_protocolCommandManagerBase*	getProtocolCommandManager(void) { return m_pCommandManager;}
	inline void								setProtocolCommandManager(Q_protocolCommandManagerBase* p) { m_pCommandManager = p;}
	inline Q_commonBuffer*					getReceiveBuffer(void) { return m_pReceiveBuffer;}
	inline void								setReceiveBuffer(Q_commonBuffer* p) { m_pReceiveBuffer = p;}
	inline Q_commonBuffer*					getSendBuffer(void) { return m_pSendBuffer;}
	inline void								setSendBuffer(Q_commonBuffer* p) { m_pSendBuffer = p;}
	inline Q_protocolSettingBase*			getProtocolSetting(void) { return m_pProtocolSetting;}
	inline void								setProtocolSetting(Q_protocolSettingBase* p) { m_pProtocolSetting = p;}
	inline char*							getTagsFilePath(void) { return m_chTagFile;}
	inline void								setTagFilePath(char* chPath) { sprintf_s(m_chTagFile,Q_FRAME_DEFAULT_PATHLENGTH,chPath);}



protected:

	/** tag file. */ 
	char									m_chTagFile[Q_FRAME_DEFAULT_PATHLENGTH];

	/** receive data buffer. */ 
	Q_commonBuffer*							m_pReceiveBuffer;


	/** send data buffer. */ 
	Q_commonBuffer*							m_pSendBuffer;


	/** command manager. */ 
	Q_protocolCommandManagerBase*			m_pCommandManager;


	/** protocol setting. */ 
	Q_protocolSettingBase*					m_pProtocolSetting;
};




/** 
 * @interface 
 * @brief Q_protocolBase class declare
 * @author yangxi
 * @date 2011-12-28
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_protocolBase
{
public:
	Q_protocolBase(void);
	virtual ~Q_protocolBase(void);



public:
	virtual bool							initializeProtocol(void) = 0;
	virtual bool							explainData(void) = 0;
	virtual bool							makeFrame(void) = 0;
	virtual bool							terminateProtocol(void) = 0;
	virtual bool							startProcess(void) = 0;
	virtual bool							stopProcess(void) = 0;
	inline Q_CommonStatusType				getState(void) { return m_eProtocolStatus;}
	inline void								setProtocolConfig(Q_protocolConfig* pConfig) { m_pProcotolConfig = pConfig;}
	CThreadMutex						m_cs_protocol;
	inline void	setDlgHWND(HWND wnd){m_wnd = wnd;}
	inline void	setDlgMessageID(int nMsgID){m_nMsgID = nMsgID;}
	////设置发送状态添加人 杨帆
	//用于处理给调试工具传送数据使用
	void setSendMsg(BYTE nState);
	BYTE getSendMsg();
	//2015-06-23 实时库接口 tjs
	void setRtdbInf(Q_dataManager *mgr);
	Q_dataManager* getRtdbInf();
protected:
	Q_CommonStatusType						m_eProtocolStatus;
	Q_protocolConfig*						m_pProcotolConfig;
	Q_dataManager *m_pMgr;//2015-06-23 tjs

protected:
	void OnSendMsg(char *msg,unsigned int len);
	void OnRecvMsg(char *msg,unsigned int len);
	void OnErrMsg(char *msg,unsigned int len);
	void OnInfoMsg(char *msg,unsigned int len,Q_protocolCommandBase *pCommand = NULL);//命令提示信息
	void OnNomalInfoMsg(char *msg,unsigned int len);//提示信息
//	void OnSendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);//规约打印
	
	bool	m_bSendMsg;
	bool	m_bRecvMsg;
	bool	m_bNomalInfoMsg;
	bool	m_bInfoMsg;
	bool	m_bErrMsg;
	HWND    m_wnd;//界面hwnd
	int     m_nMsgID;//界面消息ID;

};

#ifdef WINDOWS_ENV

#pragma warning(pop)
#endif


#endif // !defined(__Q_PROTOCOLBASE_H__INCLUDED__)
