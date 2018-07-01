#ifndef __Q_IEC104_Command_H_
#define  __Q_IEC104_Command_H_

#include "Q_frameInclude.h"

class   Q_IEC104_Struct
{
public:
	int			nRxCounter;//当前接收到的帧序号
	int			nLastAckTx;//最后确认已经发送的帧数   //存在这样的情况，发送了很多帧，但是都丢失了或者其它情况，所有没有收到确认。
	int			nTxCounter;//当前发送的帧序号
	int			nLastAckRx; //最后接收到的确认帧数
	int         nCurW;//当前W值
	int         nW;//W值
	int         nK;//K值
	Q_IEC104_Struct(void)
	{
		setDefault();
	}
	void setDefault(void)
	{
		nRxCounter=0;
		nLastAckTx=0;
		nTxCounter=0;
		nLastAckRx=0;
		nW=0;
		nK=0;
		nCurW=0;
	}
};
/*===============命令定义================================*/
class   Q_IEC104StandardCommand:public Q_protocolCommandBase
{   
public:
	Q_IEC104StandardCommand(){m_bError=false;m_nMaxTryCount=30;m_nCurTryNum=0;}
	inline void				  setCommand104Type(Q_IEC104CmdType eType) { m_eType = eType;}
	inline Q_IEC104CmdType    getCommand104Type() { return m_eType;}
	bool m_bError;//是否出错
	bool isBeyondTestNum(){return m_nCurTryNum>m_nMaxTryCount?true:false;}
	void addTestNum(){m_nCurTryNum++;}
	void setTestNumDefault(){m_nCurTryNum=0;}
	void setMaxTestNum(int n_max){m_nMaxTryCount=n_max;}
private:
	int                         m_nMaxTryCount;//最大试验数
	int                         m_nCurTryNum;//当前实验数
	Q_IEC104CmdType m_eType;//命令类型
	
};
//104命令基类
class   Q_cmdIEC104:public Q_IEC104StandardCommand
{
public:
	Q_IEC104CmdState m_eState;//命令状态
	WORD m_nTxCause;//传送原因
	int  m_nComAsdu;//公共地址
	int  m_nInfoAddr;//信息对象地址
	bool m_bExec;//标识解析后是否处理,true表示处理完成,false表示未处理
	int  m_nPointNo;//点号
	int                         m_nMaxTryCount;//最大试验数
	int                         m_nCurTryNum;//当前实验数
public:
	Q_cmdIEC104(void)
	{
		setCommonType(eCommandTypeOnce);
		setCommand104Type(eCmdTypeC_Null);
		m_eState=eCmdStateNull;
		m_nTxCause=0;
		m_nPointNo=0;
		m_nComAsdu=0;
		m_nInfoAddr=0;
		m_bExec=true;
		m_bError=false;
		m_nMaxTryCount=5;
		m_nCurTryNum=0;


	}
	virtual ~Q_cmdIEC104(void){};
};

typedef list<Q_IEC104StandardCommand *>::iterator IT_CmdListP;


//单点遥控命令
class   Q_cmdIEC104_SP:public Q_cmdIEC104
{
public:
	BYTE m_cOnOff;// 0 断开，1合上 2 不允许
	BYTE m_cQuality; //质量
	BYTE m_nValue;//记录原值
	Q_cmdIEC104_SP(void):Q_cmdIEC104()
	{
		setCommand104Type(eCmdTypeC_SC);
		m_cOnOff=2;
		m_cQuality=0;
		m_nValue=0;
	}
};

//双点遥控命令
class   Q_cmdIEC104_DP:public Q_cmdIEC104
{
public:
	BYTE m_cOnOff;// 0 断开，1合上 2 不允许
	BYTE m_cQuality; //质量
	BYTE m_nValue;//记录原值
	Q_cmdIEC104_DP(void):Q_cmdIEC104()
	{
		setCommand104Type(eCmdTypeC_SC);
		m_cOnOff=0;
		m_cQuality=0;
		m_nValue=0;
	}
};

//设定命令
class   Q_cmdIEC104_SE:public Q_cmdIEC104
{
public:
	short m_cOnOff;// 设定的值
	float m_fVal;//浮点弄使用
	BYTE m_cQuality; //质量
	BYTE m_nValue[3];//记录原始值
	BYTE m_nExecFlg;//执行标志计数
	BYTE m_cFuncCode;//功能码以示区分是规一化、标度化、短浮点型 0x30,0x31,0x32
	Q_cmdIEC104_SE(void):Q_cmdIEC104()
	{
		setCommand104Type(eCmdTypeC_SE);
		m_cOnOff=0;
		m_cQuality=0;
		m_nExecFlg=0;
		m_fVal=0.0f;
		m_cFuncCode=0x30;//默认为归一化值
	}
};

//升降命令
class   Q_cmdIEC104_RC:public Q_cmdIEC104
{
public:
	BYTE m_cOnOff;// 设定的值 不允许；1  ：降一步；2  ：升一步；3  ：不允许
	BYTE m_cQuality; //质量 暂未应用，缺省为0
	BYTE m_nValue;//记录原始值
	Q_cmdIEC104_RC(void):Q_cmdIEC104()
	{
		setCommand104Type(eCmdTypeC_RC);
		m_cOnOff=0;
		m_cQuality=0;
	}
};

//总召命令
class   Q_cmdIEC104_IC:public Q_cmdIEC104
{
public:
	BYTE m_cQuality; //召唤品质描述词
	bool m_bflgSendDigital;//发送遥信是否结束
	bool m_bflgSendAnlog;//发送遥测是否结束
	int m_nDigitalPos;//遥信发送位置
	int m_nAnalogPos;//遥测发送位置
	Q_cmdIEC104_IC():Q_cmdIEC104()
	{
		setCommand104Type(eCmdTypeC_IC);
		m_cQuality=0;
		m_bflgSendDigital=false;
		m_bflgSendAnlog=false;
		m_nDigitalPos=0;
		m_nAnalogPos=0;
	}
};
//时钟同步命令
class   Q_cmdIEC104_CS:public Q_cmdIEC104
{
public:
	BYTE m_cTime[7]; //时间
	Q_cmdIEC104_CS(void):Q_cmdIEC104()
	{
		setCommand104Type(eCmdTypeC_CS);
	}
};
class   Q_cmdIEC104_AC:public Q_cmdIEC104
{
public:
	bool m_cOnOff;// true:发送 false:不发送
	bool m_cSendSOE;
	int m_nTime;  // 发送间隔
	int asdu_addr;
	int ycbegin;
	int yxbegin;
	time_t timeflag;
	Q_cmdIEC104_AC(void):Q_cmdIEC104()
	{
		setCommand104Type(eCmdTypeAnalogchange);
		m_cOnOff = 1;
		m_cSendSOE = false;
		asdu_addr = 1;
		m_nTime = 300;
		ycbegin = 1;
		yxbegin = 1;
		timeflag = time(NULL);
	}
};
#endif