#if !defined(__Q_PROTOCOL_104_STANDARD_H__INCLUDED__)
#define __Q_PROTOCOL_104_STANDARD_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma warning(push)
#pragma warning(disable: 4251) 
#endif

#include "CommIEC101Def.h"
#include "CommIEC101MasterDef.h"
#include "Q_104StdDefine.h"
#include "Q_IEC101Command.h"
#include "Q_IEC101Setting.h"
#include "Q_IEC101CommandManager.h"
#include "Q_protocolIEC101.h"
#include "Q_runingUnitDefine.h"


#include "Q_IEC104CommFun.h"
#include "Q_104CP56Time2a.h"
#include "Q_104StandardTags.h"

#ifdef UNIX_ENV
//
// System time is represented with the following structure:
//
//typedef struct _SYSTEMTIME {
//	WORD wYear;
//	WORD wMonth;
//	WORD wDayOfWeek;
//	WORD wDay;
//	WORD wHour;
//	WORD wMinute;
//	WORD wSecond;
//	WORD wMilliseconds;
//} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;
#endif

/** 
* @interface Q_protocolCdt "Q_protocolIEC104Master.h"
* @brief IEC-60870-5-104 标准通讯规约
* @author tanjingsheng
* @date 2012-02-06
* @see 
* @bug No know bugs so far.
*/
// __Q_COMMIEC104MASTER_EXPORT
class Q_protocolIEC104Master : public Q_protocolIEC104
{
public:
	Q_protocolIEC104Master(void);
	virtual ~Q_protocolIEC104Master(void);
protected:
	bool bStartFlg;//初使结束标志 

public:
	virtual bool	   terminateProtocol();
	virtual bool	   startProcess();
	virtual bool	   stopProcess();

	void SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);
	//char LastError     [Q_FRAME_CONST256];//fs

	/**************************************************************************************************
	@Function: TxHandler
	@Description: 处理背景 扫描之类的事情，例如：(1)长期空闲状态下发送测试帧的超时  连接测试，
	                                             (2)当未确认I格式APDU达到k个时，发送方停止传送
												 (3) 发送或测试APDU的超时
												 (4)长时间没有数据发送，发送S帧确认接收到的数据　
												 (5)这个检查没有处理。当未处理的K值达到K时，发送方停止发送信息
												 注K取值12，W取值8。即当客户端连续收到K个I桢，就发送一S桢予以确认；
												 
	@Input: msgbuf 数据内容，内容从类型值开始;len长度，标识从类型值开始到结束的内容帧长度。
	@Output:
	@Return:
	@Update:
	@Others:需要用有一个单独的线程启动这个方法
	**************************************************************************************************/
	virtual bool TxHandler();
	/**************************************************************************************************
	@Function: decodeMsg_I
	@Description:解析I格式帧
	@Input: msgbuf 数据内容，内容从类型值开始;len长度，标识从类型值开始到结束的内容帧长度。
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int decodeMsg_I(BYTE *msgbuf, const UINT len);
	/**************************************************************************************************
	@Function: decodeMsg_U
	@Description:解析U格式的帧
	@Input: msgbuf 数据内容，内容从类型值开始;len长度，标识从类型值开始到结束的内容帧长度。
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int decodeMsg_U(BYTE *msgbuf, const UINT len);
	int handleCommmd_AC(Q_cmdIEC104 *v_cmd){return 0;};
	int handleCommmd_BC(Q_cmdIEC104 *v_cmd){return 0;};
	int handleCommmd_BC_SOE(Q_cmdIEC104 *v_cmd){return 0;};

	void CP56Time2aToTime(unsigned char *data, SYSTEMTIME *st);

protected:
	/**************************************************************************************************
	@Function: sendInitFinished
	@Description: 解析I格式的帧扩展
	@Input: type 信息类型,asdu公共信息地址,cause传送原因,msgbuf信息体内容
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int decodeMsg_I_Ex(Q_104ApduEx *msg){return 0;};
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
	virtual int handleCommmd_Ex(Q_cmdIEC104 *v_cmd){return 0;};
private:
	int send_IC_Response(const int &asdu,const int &cause);
/*===================================解析============================================*/
	/**************************************************************************************************
	@Function: func_Interrogate
	@Description: 解析总召内容
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_Interrogate(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_SC_NA_1
	@Description: 解析单点遥控命令
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_SC_NA_1(Q_104ApduEx *msg);

	/**************************************************************************************************
	@Function: func_C_DC_NA_1
	@Description: 解析双点遥控命令
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_DC_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_SE_NA_1
	@Description: 解析 设定命令 规一化值 
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_SE_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_SE_NC_1
	@Description: 解析 设定命令 短浮点型值 
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_SE_NC_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_SC_TA_1
	@Description: 解析 带时标的遥控命令 
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_SC_TA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_RC_NA_1
	@Description: 升降命令　遥调使用
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_RC_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_CS
	@Description: 从站：解析时钟同步命令
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:时间格式为CP56Time2a
	**************************************************************************************************/
	int func_C_CS(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_SP_NA_1
	@Description: 监视方向上：解析单点遥信上传
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_SP_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_DP_NA_1
	@Description: 监视方向上：双点遥信变位
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_DP_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_ME_NA_NB_1
	@Description: 监视方向上：带品质描述的规一化值与标度化值
	@Input: msg 信息;scaled 是否是标度化值，true标度化值，false为规度化值
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_ME_NA_NB_1(Q_104ApduEx *msg,const bool &scaled);
	/**************************************************************************************************
	@Function: func_M_ME_NC_1
	@Description:监视方向上： 短浮点数
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_ME_NC_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_ME_ND_1
	@Description:监视方向上： 短整型
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_ME_ND_1(Q_104ApduEx *msg);//jwkadd
	/*@Function: func_M_IT_NA_1
	@Description: 监视方向上:累计量解析处理 
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_IT_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_ST_NA_1
	@Description: 监视方向上:遥步信息处理
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_ST_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_BO_NA_1
	@Description: 监视方向上:32位比特串
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_BO_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_SP_TB_1
	@Description: 监视方向上:带时标的单点遥信
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_SP_TB_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_ME_TF_1
	@Description: 监视方向上:带时标的短浮点遥测
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_ME_TF_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_BO_TB_1
	@Description: 监视方向上:32位比特串
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_BO_TB_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_SE_TC_1
	@Description: 解析 设定命令 带时标的浮点型 
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_SE_TC_1(Q_104ApduEx *msg);
		/**************************************************************************************************
	@Function: time_to_IC()
	@Description: 判定是否到总召时间
	@Input: 
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	bool time_to_IC();
	///双点带时标
	void func_M_DP_TB_1(Q_104ApduEx *msg);
	
protected:
/*===================================处理============================================*/
	/**************************************************************************************************
	@Function: handleCommmd_IC
	@Description:处理总召命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_IC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_SC
	@Description:处理单点遥控命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_SC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_DC
	@Description:处理双点遥控命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_DC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_CS
	@Description:处理时钟同步命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_CS(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_SE
	@Description:处理　设定命令　规一化值　
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_SE(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_RC
	@Description:处理 升降命令　遥调
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/

	virtual int handleCommmd_RC(Q_cmdIEC104 *v_cmd);
	int sendYtCommand(const int &asdu,const int &cause,Q_IEC104_ST_C_SE *st);
	int sendYtCommand3(const int &asdu,const int &cause,Q_IEC104_ST_C_SE3 *st);
	int sendYkCommand(const int &asdu,const int &cause,Q_IEC104_ST_C_SC *st);
	int sendYkCommand1(const int &asdu,const int &cause,Q_IEC104_ST_C_SC_TA1 *st);//带时标的
	int sendYtCommandFloatWithTime(const int &asdu,const int &cause,Q_IEC104_ST_C_SE_TC1 *st);

	/**************************************************************************************************
	@Function: FindYK
	@Description: 判断遥控点是否存在
	@Input:  cmd-命令
	@Output: YKtoYXID32-遥控对应的遥信id YKID32-遥控id curRelay-遥控数据结构
	@Return: true or false
	@Update:
	@Others:
	**************************************************************************************************/
	bool FindYK(int &YKtoYXID32, int &YKID32, SCADA_Relay &curRelay, Q_cmdIEC104_SP*cmd);

	/**************************************************************************************************
	@Function: FindYT
	@Description: 判断遥调点是否存在
	@Input:  cmd-命令
	@Output: YKtoYXID32-遥调对应的遥测id YKID32-遥调id curRelay-遥调数据结构
	@Return: true or false
	@Update:
	@Others:
	**************************************************************************************************/
    bool FindYT(int &YTtoYCID32, int &YTID32, SCADA_SetPoint &curSetPoint, Q_cmdIEC104_SE*cmd);
};

#endif