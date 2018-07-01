#if !defined(__Q_PROTOCOL_103_SLAVE_H__INCLUDED__)
#define __Q_PROTOCOL_103_SLAVE_H__INCLUDED__

#pragma warning(push)
#pragma warning(disable: 4251) 

#include "Q_frameInclude.h"
#include "CommIEC103Def.h"
#include "CommIEC103SlaveDef.h"
#include "Q_103StdDefine.h"
#include "Q_IEC103Setting.h"
#include "Q_IEC103CommandManager.h"
#include "Q_protocolIEC103.h"
#include "Q_runingUnitDefine.h"

/** 
* @interface 
* @brief IEC-60870-5-103 标准通讯规约 子站端
* @author TanJingSheng
* @date 2012-05-02
* @see 
* @bug No know bugs so far.
*/

enum e103FrameType
{
	e103ASDUNull = 0,
	e103ASDU0AH_Data = 1,
	e103ASDU0AH_Command = 2,
	e103ASDU05H = 3,
	e103ASDU17H = 4,
	e103ASDU1AH = 5,
	e103ASDU1BH = 6,
	e103ASDU1CH = 7,
	e103ASDU1DH = 8,
	e103ASDU1EH = 9,
	e103ASDU1FH = 10,
};

class  Q_protocolIEC103Slave : public Q_protocolIEC103
{
public:
	virtual bool startProcess();
	virtual bool stopProcess();
	virtual bool explainData();
	virtual bool makeFrame();
	inline bool handleAnalyseData(Q_commonBuffer *recvTool,int &nReadLen,e103FrameType e103type);
protected:
	bool decodeImsg(BYTE *msgbuf,int nLen,int &readlen,e103FrameType e103type);
	bool Rec_TCP_Data(BYTE *msgbuf,int nLen,int &readlen);//返回false表示数据不够
	bool explainYT(BYTE *msg,int nLen,int &readlen);
	bool explainYK(BYTE *msg,int nLen,int &readlen);
	bool findBeginByte(e103FrameType &eType,Q_commonBuffer *recvTool);

	int send_I_Msg();
	int send_U_Msg(int cmd);
	int send_S_Msg(void);
	void send_YT_preset(int device, int dotno, float value);
	void send_YT_exe(int device, int dotno, float value);

	void send_YK_preset(int device, int dotno, BYTE value);
	void send_YK_exe(int device, int dotno, BYTE value);
	//遥调
	virtual int handleCommmd_YT(Q_cmdIEC103 *v_cmd);
	//遥控
	virtual int handleCommmd_YK(Q_cmdIEC103 *v_cmd);

	void send_YK_preset_DP(int device, int dotno, BYTE value);
	void send_YK_exe_DP(int device, int dotno, BYTE value);
	bool FindYT(int &YTtoYCID32, int &YTID32, SCADA_SetPoint &curSetPoint, Q_cmdIEC103_SE*cmd);

	void SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);
	virtual void onTxMsg(BYTE *msgbuf, unsigned int len);
	virtual void onRxMsg(BYTE *msgbuf, unsigned int len);
	virtual void OnErrorMsg(char *msg,unsigned int len);
	virtual void OnShowMsg(char *msg,unsigned int len);
	BYTE m_ReciveBuffer[MAX_RECIVE_PACKET_NUM];
};

extern Q_dataManager *m_pMgrIEC103Slave; 
#endif