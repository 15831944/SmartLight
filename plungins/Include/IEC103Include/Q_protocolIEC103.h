#if !defined(__Q_PROTOCOL_IEC103_H__INCLUDED__)
#define __Q_PROTOCOL_IEC103_H__INCLUDED__

#pragma warning(push)
#pragma warning(disable: 4251) 


#include "CommIEC103Def.h"
#include "Q_103StdDefine.h"
#include "Q_IEC103Command.h"
#include "Q_IEC103Setting.h"
#include "Q_IEC103CommandManager.h"
#include "tinyxmlinclude.h"

class Q_protocolIEC103 : public Q_protocolBase
{
public:
	Q_protocolIEC103();
	virtual ~Q_protocolIEC103();
	virtual bool explainData();
	virtual bool		makeFrame();
	virtual bool		initializeProtocol();

	CFEPFuncRtdb *g_rtdbInfIEC103;

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
	virtual Q_IEC103vfyRet verifyFrame(BYTE* msgbuf,UINT len);
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
	int this_command_flag;
	int next_command_flag;
	void setYTvalue(int device, int dotno, float value);
	void setYKstate(int device, int dotno, bool value);
protected:
	void setDefaultValue(void);
	//
	//	发送I格式帧 len是 apdu的长度不包括前四个字节
	//
	int send_I_Msg();
	int send_U_Msg(int cmd);
	int send_S_Msg(void);
	void send_YT_preset(int device, int dotno, float value);
	void send_YK_preset(int device, int dotno, bool value);
	void send_YT_exe(int device);
	void send_YK_exe(int device, int dotno, bool value);
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




protected:
    int m_nThreadPeriod;//用来临时用
	Q_IEC103Setting *m_ProSetting;
	Q_ComCpuEndian m_cpuEndian;//cpu对齐方式
	Q_IEC103_Struct *m_st;
	Q_103BaseCfg *m_cfgBase;//基本配置
	Q_103StCfg   *m_cfgSt;//站召唤配置
	Q_103CtrlCfg *m_cfgControl;//控制配置
	Q_IEC103CommandManager *m_listCmd;//命令列表 //不由协议来释放
	Q_103SyncClockCfg   *m_cfgSyncClock;//时钟同步配置
	Q_103StandardTags* m_tags;//数据标记用
	//Q_channelBase*							m_clientChannel;
	int Q_103NoDataTimes;
	int Q_103TWODataTimes;
	int sleepflag;



	char		LastError[200];         //错误信息

	time_t          now;                    //当前时间，做计算时间差用

	time_t			m_nTimerConnect;		// 时间计数

	time_t			m_nTimerConfirm;		// 时间计数 暂停计时

	time_t			m_nTimer_S_Ack;		// 时间计数,关联T2 无数据报文时确认的超时
	bool		m_bTimer_S_Ack;

	time_t			m_nTimer_U_Test;		// 时间计数,关联T3 长期空闲状态下发送测试帧的超时
	bool        m_bSendTestFR;           //是否启动信道TestFR帧测试
	bool        m_bStopSend;             //停止发送 当未确认I格式APDU达到k个时，发送方停止传送
	Q_IEC103_ErrorCode    m_errorNo; //错误码
	time_t			timer_U_Test;		// associated of T3 param
	bool		timer_U_Testflag;
	bool        FCB;
	bool        FCV;
	int         yt_device;
	int         yt_dotno;
	float         yt_value;
	int         yk_device;
	int         yk_dotno;
	bool        yk_value;

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
	virtual void OnShowMsg(char *msg,unsigned int len);

	

	void writefile(char *log);

			/**************************************************************************************************
	@Function: decodeMsg_I
	@Description: 解析长帧
	@Input: msgbuf 数据内容，内容从类型值开始;len长度，标识从类型值开始到结束的内容帧长度。
	@Output:
	@Return:
	@Update:
	@Others: 
	**************************************************************************************************/
	int decodeImsg(BYTE *msgbuf, const UINT len);
	/**************************************************************************************************
	@Function: decodeMsg_U
	@Description: 解析短帧
	@Input: msgbuf 数据内容，内容从类型值开始;len长度，标识从类型值开始到结束的内容帧长度。
	@Output:
	@Return:
	@Update:
	@Others: 
	**************************************************************************************************/
//	void rec_short_msg(BYTE CODE, const UINT len);
	int decodeUmsg(BYTE CODE, const UINT len);


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
	//virtual int handleCommmd_Ex(Q_cmdIEC103 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_IC
	@Description:处理总召命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//virtual int handleCommmd_IC(Q_cmdIEC103 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_SC
	@Description:处理单点遥控命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//virtual int handleCommmd_SC(Q_cmdIEC103 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_DC
	@Description:处理双点遥控命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//virtual int handleCommmd_DC(Q_cmdIEC103 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_CS
	@Description:处理时钟同步命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//virtual int handleCommmd_CS(Q_cmdIEC103 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_SE
	@Description:处理　设定命令　规一化值　
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//virtual int handleCommmd_SE(Q_cmdIEC103 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_RC
	@Description:处理 升降命令　遥调
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//virtual int handleCommmd_RC(Q_cmdIEC103 *v_cmd)=0;
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
	//virtual int handleCommmd_AC(Q_cmdIEC103 *v_cmd)=0;
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
	//virtual int handleCommmd_BC(Q_cmdIEC103 *v_cmd)=0;
	bool	terminateProtocol(){ return true;};
	virtual bool	startProcess();
	bool	stopProcess();

	BYTE CalculateCheck( BYTE *msgbuf ,int num);

	void Rec_TCP_Data(BYTE *msgbuf);
	void Rec_All_YX(BYTE *msgbuf);
	void Rec_SOE(BYTE *msgbuf);
	void Rec_CHange_YX(BYTE *msgbuf);
	void Rec_All_YC(BYTE *msgbuf);
	void RestartCh();



};
#endif