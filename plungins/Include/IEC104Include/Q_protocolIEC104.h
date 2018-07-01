#if !defined(__Q_PROTOCOL_IEC104_H__INCLUDED__)
#define __Q_PROTOCOL_IEC104_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma warning(push)
#pragma warning(disable: 4251) 
#endif

#include "Q_frameInclude.h"

#include "CommIEC104Def.h"
#include "Q_104StdDefine.h"
#include "Q_IEC104Command.h"
#include "Q_IEC104Setting.h"
#include "Q_IEC104CommandManager.h"
#include "gSystemDef.h"


class __Q_COMMIEC104_EXPORT Q_protocolIEC104 : public Q_protocolBase
{
public:
	Q_protocolIEC104();
	virtual ~Q_protocolIEC104();
	virtual bool explainData();
	virtual bool		makeFrame();
	virtual bool		initializeProtocol();
	virtual int decodeMsg_I(BYTE *msgbuf, const UINT len)=0;
	virtual int decodeMsg_U(BYTE *msgbuf, const UINT len)=0;
	virtual void SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);

	CFEPFuncRtdb *g_rtdbInfIEC104;

	//用来触发响应消息
	///单位毫秒
	void setThreadPeriod(int nThreadPeriod){m_nThreadPeriod=nThreadPeriod;}
	/**************************************************************************************************
	@Function: verifyFrame
	@Description: 校验帧结构，同时分析帧结构类型
	@Input: msgbuf 数据内容，内容从类型值开始;len长度，标识从类型值开始到结束的内容帧长度。
	@Output:
	@Return:Q_IEC104vfyRet 结构 st . 如果出错后，m_bSuc为false，同时m_nErrorCode置为非0的值
	校验成功 m_bSuc为true,m_eFrameType为相应的帧类型
	@Update:
	@Others: 
	//(1)I帧时校验公共服务单元地址
	//(2)数据是否超长或过小
	//(3)报文中的数据个数是否与收到的数目相等
	//(4) I格式的帧应该启始长度大于12
	//(5) 防止报文丢失和报文重复传送处理,已发送的序列号与已接收的序列号校验。
	**************************************************************************************************/
	virtual Q_IEC104vfyRet verifyFrame(BYTE* msgbuf,UINT len);
	/**************************************************************************************************
	@Function: handleCommmd_CS
	@Description:解析数据、校验数据
	@Input:recvTool 接收缓冲区处理工作
	@Output:
	@Return:成功返回 Q_104SUC;失败 Q_104FAIL;
	@Update:
	@Others:解析完数据后，即删除解析后的数据
	**************************************************************************************************/
	inline bool handleAnalyseData(Q_commonBuffer *recvTool);

	/**************************************************************************************************
	@Function: decodeMsg_U
	@Description:分析socket是否断开了
	@Input: 
	@Output:
	@Return:
	@Update:
	@Others:只要m_eProtocolStatus的值为eCommonStatusError就表示，socket链路断开
	**************************************************************************************************/
	bool getSockConnectStatus();
	////
	//关闭链路
	////
	void closeLink(void);
protected:
	void setDefaultValue(void);
	//
	//	发送I格式帧 len是 apdu的长度不包括前四个字节
	//
	virtual int send_I_Msg(BYTE *msgbuf, UINT len);
	int send_U_Msg(int cmd);
	int send_S_Msg(void);
	/**************************************************************************************************
	@Function: mirror_I_Msg
	@Description: 镜向发送I帧数据
	@Input: msgbuf 数据内容，内容从类型值开始;len长度，标识从类型值开始到结束的内容帧长度。
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int mirror_I_Msg(BYTE *msgbuf,const UINT &len);
	//
	//	开始命令
	//
	int sendStartDT(void);
	//
	//	终止命令
	//
	int sendStopDT(void);

	//
	//	测试帧命令
	//
	int sendTestFR(void);
public:
	char		LastError[Q_FRAME_CONST256];         //错误信息
protected:
    int m_nThreadPeriod;//用来临时用
	Q_IEC104Setting *m_ProSetting;
	Q_ComCpuEndian m_cpuEndian;//cpu对齐方式
	Q_IEC104_Struct *m_st;
	Q_104BaseCfg *m_cfgBase;//基本配置
	Q_104StCfg   *m_cfgSt;//站召唤配置
	Q_104CtrlCfg *m_cfgControl;//控制配置
	Q_IEC104CommandManager *m_listCmd;//命令列表 //不由协议来释放
	Q_104SyncClockCfg   *m_cfgSyncClock;//时钟同步配置
	Q_104StandardTags* m_tags;//数据标记用

	

	time_t          now;                    //当前时间，做计算时间差用

	time_t			m_nTimerConnect;		// 时间计数,关联T0 是主站发起连接请求的周期间隔时间

	time_t			m_nTimerConfirm;		// 时间计数,关联T1 发送或测试APDU的超时 

	time_t			m_nTimer_S_Ack;		// 时间计数,关联T2 无数据报文时确认的超时
	bool		m_bTimer_S_Ack;

	time_t			m_nTimer_U_Test;		// 时间计数,关联T3 长期空闲状态下发送测试帧的超时
	bool        m_bSendTestFR;           //是否启动信道TestFR帧测试
	bool        m_bStopSend;             //停止发送 当未确认I格式APDU达到k个时，发送方停止传送
	Q_IEC104_ErrorCode    m_errorNo; //错误码
	time_t			timer_U_Test;		// associated of T3 param
	bool		timer_U_Testflag;
	time_t      timer_ICTimeflag;

protected:
	/**************************************************************************************************
	@Function: onTxMsg
	@Description:当发送数据时调用方虚方法
	@Input: msgbuf 数据内容，内容从类型值开始;len长度，标识从类型值开始到结束的内容帧长度。
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual void onTxMsg(BYTE *msgbuf, unsigned int len);
	/**************************************************************************************************
	@Function: onTxMsg
	@Description:当接收到数据时调用方虚方法
	@Input: msgbuf 数据内容，内容从类型值开始;len长度，标识从类型值开始到结束的内容帧长度。
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual void onRxMsg(BYTE *msgbuf, unsigned int len);
	virtual void OnError(int errorCode,int lineNum);
	virtual void OnErrorMsg(char *msg,unsigned int len);
	//打印规约提示信息，一般用于遥调、遥控打印
	virtual void OnShowMsg(char *msg,unsigned int len);
	//打印规约解决信息，一般用于遥测打印  tjs 2013-11-17
	virtual void OnShowExplainMsg(char *msg,unsigned int len);
	void writefile(char *log);


	/*===================================处理============================================*/
	/**************************************************************************************************
	@Function: sendInitFinished
	@Description: 处理命令扩展
	@Calls:
	@Called By:
	@Input: asdu 表示公共地址
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_Ex(Q_cmdIEC104 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_IC
	@Description:处理总召命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_IC(Q_cmdIEC104 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_SC
	@Description:处理单点遥控命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_SC(Q_cmdIEC104 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_DC
	@Description:处理双点遥控命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_DC(Q_cmdIEC104 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_CS
	@Description:处理时钟同步命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_CS(Q_cmdIEC104 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_SE
	@Description:处理　设定命令　规一化值　
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_SE(Q_cmdIEC104 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_RC
	@Description:处理 升降命令　遥调
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_RC(Q_cmdIEC104 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_AC
	@Description: 变化遥测
	@Calls:
	@Called By:
	@Input: 
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_AC(Q_cmdIEC104 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_BC
	@Description: 变化遥信
	@Calls:
	@Called By:
	@Input: 
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_BC(Q_cmdIEC104 *v_cmd)=0;
		/**************************************************************************************************
	@Function: handleCommmd_BC_SOE
	@Description: SOE上送
	@Calls:
	@Called By:
	@Input: 
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_BC_SOE(Q_cmdIEC104 *v_cmd)=0;
};
#endif