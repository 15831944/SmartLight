#if !defined(__Q_PROTOCOL_104_SLAVE_H__INCLUDED__)
#define __Q_PROTOCOL_104_SLAVE_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma warning(push)
#pragma warning(disable: 4251) 
#endif
#include "Q_protocolIEC101.h"
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
	int SendInterrogateData(Q_cmdIEC104 *v_cmd);
protected:
	bool bStartFlg;//初使结束标志 	
	int ACsendno;
	int BCsendno;//遥信遥测变化上送号
	int SOEsendno;//SOE上送号
	Q_IEC104CmdState m_IC_State;//总召状态

	vector<IEC104_VARIBLE_YC *> m_vecVarYC;//变化遥测集合
	vector<IEC104_VARIBLE_YX *> m_vecVarYX;//变化遥信集合
	vector<IEC104_VARIBLE_SOE *> m_vecVarSOE;//变化遥信SOE集合
	//SYSTEMTIME m_timeSOE;//遥信变位时间SOE
	bool bZongZhaoComplete;
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
	int handleCommmd_AC(Q_cmdIEC104 *v_cmd);
	int handleCommmd_BC(Q_cmdIEC104 *v_cmd);
	int handleCommmd_BC_SOE(Q_cmdIEC104 *v_cmd);

	void CP56Time2aToTime(unsigned char *data, SYSTEMTIME *st);
	int SendIC_SP_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount);
	int SendIC_ME_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount);
	//双点遥信上传
	int SendIC_DP_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount);
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
	void func_M_DP_TB_1(Q_104ApduEx *msg);
	void func_M_ME_TF_1(Q_104ApduEx *msg);
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
	//解析测试帧
	int func_TestFR(Q_104ApduEx *msg);
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
	@Function: handleCommmd_RC
	@Description:处理 升降命令　遥调
	@Input:v_cmd 命令
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void initVaribleData();//初使化变化量
	void deleteVarData();//删除变化量数据

	//发送初使化结束报文
	int send_InitFinished(const int &asdu, const int &cause);

};

//extern Q_dataManager *m_pMgrIEC104SlaveProtocolSetting; //fs20150724


#endif