#if !defined(__Q_PROTOCOL_104_SLAVE_H__INCLUDED__)
#define __Q_PROTOCOL_104_SLAVE_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma warning(push)
#pragma warning(disable: 4251) 
#endif

#include "Q_frameInclude.h"
#include "CommIEC104Def.h"
#include "CommIEC104SlaveDef.h"
#include "Q_104StdDefine.h"
#include "Q_IEC104Setting.h"
#include "Q_IEC104CommandManager.h"
#include "Q_protocolIEC104.h"
#include <vector>
using namespace std;

typedef struct _IEC104_VARIBLE_YC
{
	int id;
	float value;
	//int m_nZfID;//如果转发ID不为0就取转发ID的值
	_IEC104_VARIBLE_YC()
	{
		id=-1;
		value=0;
	//	m_nZfID=0;
	}
}IEC104_VARIBLE_YC;
typedef struct _IEC104_VARIBLE_YX
{
	int id;
	bool value;
	//int m_nZfID;//如果转发ID不为0就取转发ID的值
	_IEC104_VARIBLE_YX()
	{
     id=-1;
	 value=false;
	// m_nZfID=0;
	}
}IEC104_VARIBLE_YX;
typedef struct _IEC104_VARIBLE_SOE
{
	int id;//DPU中编号
	bool value;
	bool changed;//是否变位
	SYSTEMTIME timeSOE;//遥信变位时间SOE
	//int m_nZfID;//如果转发ID不为0就取转发ID的值
	_IEC104_VARIBLE_SOE()
	{
     id=-1;
	 value=false;
	 changed = false;
	// m_nZfID=0;
	}
}IEC104_VARIBLE_SOE;
/** 
* @interface 
* @brief IEC-60870-5-104 标准通讯规约 子站端
* @author TanJingSheng
* @date 2012-05-02
* @see 
* @bug No know bugs so far.
*/
class Q_protocolIEC104Slave : public Q_protocolIEC104
{
public:
	Q_protocolIEC104Slave(void);
	virtual ~Q_protocolIEC104Slave(void);
public:
	virtual bool		terminateProtocol();

	virtual bool	   startProcess();
	virtual bool	   stopProcess();
	virtual bool initializeProtocol();

	void SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);
	//设定用于执行命令的编号
	void SetSigExecCmd(int MsgID);
	
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
	int decodeMsg_I(BYTE *msgbuf, const UINT len);
	/**************************************************************************************************
	@Function: decodeMsg_U
	@Description:解析U格式的帧
	@Input: msgbuf 数据内容，内容从类型值开始;len长度，标识从类型值开始到结束的内容帧长度。
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int decodeMsg_U(BYTE *msgbuf, const UINT len);
protected:
	/**************************************************************************************************
	@Function: sendInterrogateInd
	@Description:  发送总召激活确认与发送总召停止确认
	@Input: asdu 表示公共地址,cause 传送原因一般只有0x07 激活确认与0x0A 激活终止
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int sendInterrogateInd(int asdu,int cause);
	/**************************************************************************************************
	@Function: send_IC_Response
	@Description:  发送总召激活确认与发送总召停止确认
	@Input: asdu 表示公共地址,cause 传送原因一般只有0x07 激活确认与0x0A 激活终止
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int send_IC_Response(int asdu,int cause);
	/**************************************************************************************************
	@Function: send_SP_Response
	@Description:  发送单点信息的返回值
	@Input: asdu 表示公共地址,cause 传送原因,adr 地址 ,value对应的值。 type 默认是单点遥控
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int sendYkResponse(const int &asdu,const int &cause,const int &adr,BYTE *value,const int &type,const UINT &len=1);
	/**************************************************************************************************
	@Function: send_CS_Response
	@Description:  发送时钟同步回馈信息
	@Input: asdu 表示公共地址,cause 传送原因,data 传送数据 只是时间内容信息,len 时间内容信息数据长度
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int send_CS_Response(int asdu,int cause,BYTE *data,const UINT &len);
	/**************************************************************************************************
	@Function: send_C_TS_TA_1_Response
	@Description:  发送带时标的测试命令回馈信息
	@Input: asdu 表示公共地址,cause 传送原因,data 传送数据 只是时间内容信息,len 时间内容信息数据长度
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int send_C_TS_TA_1_Response(int asdu,int cause,BYTE *data,const UINT &len);
	/**************************************************************************************************
	@Function: sendInitFinished
	@Description: 初使化结束
	@Input: asdu 表示公共地址
	@Output:
	@Return:
	@Update:
	@Others:应该是在主站连接之后，子站初使化完成之后给主站发送，然后主站才发送总召命令。但一般好像都没有发送。所以目前也不打算发送。
	**************************************************************************************************/
	int sendInitFinished(int asdu);
	/**************************************************************************************************
	@Function: SendInterrogateData
	@Description: 发送总召返回数据
	@Input: v_cmd 命令信息。
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//int SendInterrogateData(Q_cmdIEC104 *v_cmd);
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
	virtual int handleCommmd_SE(Q_cmdIEC104 *v_cmd);
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
	virtual int handleCommmd_AC(Q_cmdIEC104 *v_cmd);
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
	virtual int handleCommmd_BC(Q_cmdIEC104 *v_cmd);
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
	virtual int handleCommmd_BC_SOE(Q_cmdIEC104 *v_cmd);

protected:
	//fs20150724
	vector<IEC104_VARIBLE_YC *> m_vecVarYC;//变化遥测集合
	vector<IEC104_VARIBLE_YX *> m_vecVarYX;//变化遥信集合
	vector<IEC104_VARIBLE_SOE *> m_vecVarSOE;//变化遥信SOE集合
	//SYSTEMTIME m_timeSOE;//遥信变位时间SOE
	bool bZongZhaoComplete;
	
	int SendInterrogateData(Q_cmdIEC104 *v_cmd);
private:
	Q_104YtExecMode m_YtExeMode;
	Q_104YtExecMode m_YkExeMode;
	int m_nExecCmdMsgID;//消息编号 执行外部命令消息编号
	Q_IEC104_ExecCmd m_SigArray;//104执行命令记录

	//上一条遥控指令记录，防止重复报文重复addcommand
	BYTE last_sel_cOnOff;
	int  last_sel_nInfoAddr;
	time_t sel_timestmp;
	time_t exe_timestmp;
	BYTE last_exe_cOnOff;
	int  last_exe_nInfoAddr;

	int ACsendno;
	int BCsendno;//遥信遥测变化上送号
	int SOEsendno;//SOE上送号

	///////////////////////////////
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
	@Description: 解析 设定命令 规一化值 不区分遥测 与 遥信 应该
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_SE_NA_1(Q_104ApduEx *msg);
	/*
	@Function:func_C_SE_NB_1
	@Description:解析设定标度化值命令
	*/
	int func_C_SE_NB_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function:  func_C_SE_NC_1
	@Description: 解析 设定命令 浮点型 
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_SE_NC_1(Q_104ApduEx *msg);
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
	@Function: func_C_TS_TA_1
	@Description: 从站：带时标的测试命令
	@Input: msg 信息
	@Output:
	@Return:
	@Update:
	@Others:时间格式为CP56Time2a
	**************************************************************************************************/
	int func_C_TS_TA_1(Q_104ApduEx *msg);

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
	int handleCommmd_IC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_SC
	@Description:处理单点遥控命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int handleCommmd_SC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_DC
	@Description:处理双点遥控命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int handleCommmd_DC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_CS
	@Description:处理时钟同步命令
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int handleCommmd_CS(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_RC
	@Description:处理 升降命令　遥调
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int handleCommmd_RC(Q_cmdIEC104 *v_cmd);
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
	@Function: SendIC_DP_Res
	@Description:总召请求，发送双点返回数据
	@Input:,msg 信息
	vTable 要发送数据点表
	@Output:
	@Return:成功返回 Q_104SUC;失败 Q_104FAIL;
	@Update:
	@Others:
	**************************************************************************************************/
	//int SendIC_DP_Res(Q_IEC104_ASDU *msg,vector<Q_ST_104PT *> &vTable,const int &nIdx,const int &nCount);
	/**************************************************************************************************
	@Function: SendIC_SP_Res
	@Description:总召请求，发送单点返回数据
	@Input:nIdx 开始索引
	@Output:nCount发送的数目
	@Return:成功返回 Q_104SUC;失败 Q_104FAIL;
	@Update:
	@Others:
	**************************************************************************************************/
	int SendIC_SP_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount);
	/**************************************************************************************************
	@Function: SendIC_ME_Res
	@Description:总召请求，发送遥测返回数据
	@Input:msg 信息 vTable 要发送数据点表 ,nIdx 开始索引,nCount发送的数目
	@Output:
	@Return:成功返回 Q_104SUC;失败 Q_104FAIL;
	@Update:
	@Others:
	**************************************************************************************************/
	int SendIC_ME_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount);
	int sendYtCommand(const int &asdu,const int &cause,Q_IEC104_ST_C_SE *st);
	int sendYtCommand3(const int &asdu,const int &cause,Q_IEC104_ST_C_SE3 *st);
	void initVaribleData();//初使化变化量
	void deleteVarData();//删除变化量数据

};

extern Q_dataManager *m_pMgrIEC104SlaveProtocolSetting; //fs20150724


#endif