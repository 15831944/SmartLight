#pragma once

#if !defined(__Q_IEC103_SETTING_H__INCLUDED__)
#define __Q_IEC103_SETTING_H__INCLUDED__

#pragma warning(push)
#pragma warning(disable: 4251) 

#include "CommIEC103Def.h"
#include "Q_103StdDefine.h"
//#include "Q_CommUtility.h"
#include <vector>
using namespace std;
#include "Q_103StandardTags.h"


typedef enum Q_103XmlDataType
{
   e103XmlDataTypeNull=0,//未知
   e103XmlDataTypeInt=1,
   e103XmlDataTypeFloat=2,
   e103XmlDataTypeEnum=3,
   e103XmlDataTypeString=4
};

typedef struct ST_103CfgT
{
UINT group; //分组
UINT id;    //编号
char name[50]; //配置名
Q_103XmlDataType type; // Int,Float,ENUM,String
char value[100]; //值
}ST_103CfgT;

typedef enum Q_103FrameSeqVfy
{
	e103FrameSeqVfyBoth=1,//同时检验发送和接收序号" />
	e103FrameSeqVfyLocTxRemRx=2,//只校验本侧发送和对侧接收序号
	e103FrameSeqVfyBothSend=3,  //只校验本侧和对侧发送序号
	e103FrameSeqVfyNull=4 // 不校验发送和接收序号
};
//总召发送数据顺序
typedef enum Q_103ICSendSeq
{
	e103ICSendSeqPMS=1,//遥信、遥测、遥步
	e103ICSendSeqPSM=2,
	e103ICSendSeqMPS=3, 
	e103ICSendSeqMSP=4,
	e103ICSendSeqSPM=5,
	e103ICSendSeqSMP=6,

};
//上传遥测类型
enum Q_103_AsduYcType{
	e103AsduYcType9=1,//ASDU9、规一化值、无时标、有品质
	e103AsduYcType11=2,//ASDU11、标度化值、无时标
	e103AsduYcType13=3,//ASDU13、短浮点数、无时标
	e103AsduYcType21=4//ASDU21、规一化值、无时标、无品质
};
typedef enum  Q_103OpenAppMode
{
	e103OpenAppMode_1=1, //收到U启动帧即开放应用传输、收到停止帧即关闭应用传输
	e103OpenAppMode_2=2,//TCP连接建立即开放应用传输、TCP连接关闭即关闭应用传输
	e103OpenAppMode_3=3,//收到U启动帧即开放应用传输、TCP连接关闭即关闭应用传输
	e103OpenAppMode_4=4  //TCP连接建立即开放应用传输、收到U停止帧即关闭应用传输
};

typedef enum  Q_103AsduVfy //ASDU公共信息校验策略
{
	e103AsduVfy_Total=1,
	e103AsduVfy_Vsq=2,
	e103AsduVfy_Cot=3,
	e103AsduVfy_Addr=4,
	e103AsduVfy_VsqAddr=5,
	e103AsduVfy_CotAddr=6,
	e103AsduVfy_Null=7

};
typedef enum  Q_103IntSeq //ASDU公共信息校验策略
{
	e103IntSeqLittle=1, //从低到高
	e103IntSeqBig=2     //从高到低

};
typedef enum  Q_103FloatSeq //ASDU公共信息校验策略
{
	e103FloatSeqLittle=1, //从低到高
	e103FloatSeqBig=2     //从高到低
};
typedef enum  Q_103T1App //ASDU公共信息校验策略
{
	e103T1AppCloseLink=1
};
typedef enum  Q_103T2App //ASDU公共信息校验策略
{
	e103T2AppSendS=1
};
typedef enum  Q_103T3App //ASDU公共信息校验策略
{
	e103T3AppSendTest=1
};
//I帧发送策略
typedef enum  Q_103IF_SendMode 
{
	e103IF_SendIgnoreK=1,//I帧发送不受K值影响
	e103IF_SendLimitByK=2 //发出的I帧未被确认数目大于等于K值即停止发送

};
typedef enum  Q_103IF_RecvMode 
{
	e103IF_RecvBeforeW_Act=1 //I帧发送不受K值影响

};
//以收到的主站时间进行确认,以收到的主站时间静态补偿后确认,以校时前的自有系统时间进行确认,以校时后的自有系统时间进行确认
typedef enum  Q_103CS_Act
{
	e103CS_Act_Master       =1,  //以收到的主站时间进行确认
	e103CS_Act_MasterFix    =2,   //以收到的主站时间静态补偿后确认
	e103CS_Act_OwnSysBf	 =3,//以校时前的自有系统时间进行确认
	e103CS_Act_OwnSysAf     =4//以校时后的自有系统时间进行确认

};
typedef enum Q_103YtExecMode
{
	e103YtExecModeNull=0,
	e103YtExecModeDirect=0,//直接执行
	e103YtExecModeSelect=1 //选择执行
}Q_103YtExecMode;
//设置命令asdu类型
typedef enum Q_103YsetAsdu
{
	e103YsetAsdu30=0, //规一化设定
	e103YsetAsdu31=1,//标度化设定
	e103YsetAsdu32=2 //浮点型设定
}Q_103YsetAsdu;

typedef enum Q_103YkAsdu
{
	e103YkAsdu9=0, //双点遥控
	e103YkAsdu10=1 //单点遥控
}Q_103YkAsdu;//遥控asdu

typedef struct Q_103_SrvPort
{
	char ip[30];//ip地址
	int port;
	bool valid;//是否有效
}Q_103_SrvPort;
class  Q_103BaseCfg
{
public:
	  Q_103_SrvPort localServer; //本端服务器
	  Q_103_SrvPort remServer[MAX_103_REMOTE_NUM];//对端，最多10个
	  int    m_nAsduLen;//应用层地址长度
	  int    m_nAsduAddr;//应用层地址
	  int    m_nTxCauseLen;//传送原因长度
	  int    m_nMaxFrameLen;
	  //int	m_nStationAddr1;
	  //int	m_nStationAddr2;
	  vector<int>m_vecStationAddr;
	  Q_103OpenAppMode m_eOpenAppMethord;
	  //帧序号校验策略
	  
	  Q_103FrameSeqVfy m_eFrameSeqVfy;
	  Q_IEC103_ST_Type m_eCurStType;
	  Q_103AsduVfy   m_eAsduVfy;
	  Q_103IntSeq    m_eIntSeq;
	  Q_103FloatSeq  m_eFloatSeq;
	  Q_103T1App     m_eT1App;
	  Q_103T2App     m_eT2App;
	  Q_103T3App     m_eT3App;
	  Q_103IF_SendMode m_eIF_Send;
	  Q_103IF_RecvMode m_eIF_Recv;

      int m_nK;// maximum difference N(R) and N(S)
	  int m_nW;// lastest ack after receiving W I-Format message.
      int m_nT0;//通信中断判断时限(秒)
	  int m_nT1;//超时时间T1值(秒)
	  int m_nT2;//超时时间T2值(秒)
	  int m_nT3;//超时时间T3值(秒)
	  int m_nFrameI_MaxCounter;//I帧最大数目
	  bool m_bSendSFrame;//是否发送S帧
	  bool m_bSendStartDt;//是否发送启动帧
	  bool m_bSendIC;//是否发送总召
	  bool m_bSendAC;//是否发送变化遥测
	  int m_acTime;  //变化量上送间隔
public:
	Q_103BaseCfg(void)
	{
		localServer.valid=false;
		for(int i=0;i<MAX_103_REMOTE_NUM;i++)
		{
			remServer[i].valid=false;
			memset(remServer[i].ip,'\0',sizeof(remServer[i].ip));
			remServer[i].port=0;
		}
		
		m_nT0=30;
		m_nT1=15;
		m_nT2=10;
		m_nT3=30;
		m_nK=12;
		m_nW=8;
		m_eFrameSeqVfy=e103FrameSeqVfyNull;
		m_nFrameI_MaxCounter=0x7FFF;
		m_eCurStType=e103Station_Unknown;
		m_eAsduVfy=e103AsduVfy_Total;

		m_nFrameI_MaxCounter=32767;

		m_eIntSeq=e103IntSeqLittle;
		m_eFloatSeq=e103FloatSeqLittle;
		m_eT1App=e103T1AppCloseLink;
		m_eT2App=e103T2AppSendS;
		m_eT3App=e103T3AppSendTest;
		m_eIF_Send=e103IF_SendLimitByK;
		m_eIF_Recv=e103IF_RecvBeforeW_Act;
		m_bSendSFrame=false;
		m_bSendIC=true;
		m_bSendStartDt=true;
		//m_nStationAddr1=0;
		//m_nStationAddr2=0;
		m_vecStationAddr.clear();
	}
};
//站召换配置
class  Q_103StCfg
{
public:
	int n_YxNumPerFrame;//每帧遥信个数
	int n_YcNumPerFrame;//每帧遥测个数
	int n_YbNumPerFrame;//每帧遥步个数
	int n_IcInterval;//总召间隔
	Q_103_AsduYcType m_103_AsdnYcType;//jwk add
	int m_YcBeginIndex;//遥测起始地址
	int m_YxBeginIndex;//遥信起始地址
	Q_103StCfg(void)
	{
     n_YxNumPerFrame=50;
     n_YcNumPerFrame=20;
     n_YbNumPerFrame=50;
	 n_IcInterval=10;
//	 m_AsdnYcType=e103AsduYcType13;
	 m_YcBeginIndex=0;
	 m_YxBeginIndex=0;
	}


};

//控制配置
class Q_103CtrlCfg
{
public:
	Q_103CtrlCfg(void){};
	virtual ~Q_103CtrlCfg(void){};
	Q_103YtExecMode m_YtExecMode;//遥调模式
	Q_103YtExecMode m_YkExecMode;// 遥控模式
	Q_103YsetAsdu   m_YsetAsduType;//设置命令使用ASDU类型
	Q_103YkAsdu m_YkAsduType;
	int             getAsdu48Mode;

};
class  Q_103SyncClockCfg
{
public:
	Q_103SyncClockCfg(void){};
	virtual ~Q_103SyncClockCfg(void){};
	int m_nFixUsecs;//时钟同步应用层确认时间静态补偿值(毫秒)
	Q_103CS_Act  m_eActMethord;//时钟同步应用层确认策略

};

class  Q_103PrintCfg
{
public:
	Q_103PrintCfg(void)
	{
		bPrintTotal=false;
		bPrintExplain=false;
		bPrintPrompt=false;
		bPrintError=false;
	};
	virtual ~Q_103PrintCfg(void){};
	bool bPrintTotal;//打印全部
	bool bPrintExplain;//打印解释信息
	bool bPrintPrompt;//打印提示信息
	bool bPrintError;//打印错误信息
};
class  Q_IEC103Setting : public Q_protocolSettingBase
{
public:
	Q_IEC103Setting(void);
	Q_IEC103Setting(Q_IEC103Setting *other);
	void EvalData(Q_IEC103Setting *other);
	virtual ~Q_IEC103Setting(void);
    void setFilePath(char *);
	//设置点表配置文件位置
	bool setPtFilePath(char *);
	Q_103BaseCfg *getBaseSetting(Q_103BaseCfg *v_cfg);
	Q_103BaseCfg *getBaseSettingEx(Q_103BaseCfg *v_cfg);
	Q_103StCfg   *getStationSetting(Q_103StCfg *v_cfg);
    Q_103CtrlCfg *getControlSetting(Q_103CtrlCfg *v_cfg);
	Q_103SyncClockCfg *getSyncClockSetting(Q_103SyncClockCfg *v_cfg);
    bool initSetting();
	virtual bool reloadLog();

	inline void setTagsManager(Q_103StandardTags* vTags) 
	{
		m_103TagManager=vTags;
	}

	//打印信息配置
	bool getPrintSetting(Q_LogType type);
	/**************************************************************************************************
	@Function: SetWndInfo
	@Description: 设置窗口句柄
	@Calls:
	@Called By:
	@Input: wnd 窗体句柄,msgID打印消息的ID
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void SetWndInfo(HWND wnd,int MsgID);
	HWND getWndHwnd(){return m_wnd;}
	int getWndMsgID(){return m_nMsgID;}
	void setCmdSignalID(int v_id){m_nCmdSignalID=v_id;}
	int getCmdSignalID(){return m_nCmdSignalID;}

	inline Q_103StandardTags*	getTagsManager() { return m_103TagManager;}
    char *getContentID(const int &v_idx,const int &v_grp=1);
protected:
	vector<ST_103CfgT> m_vContent;//内容
	vector<ST_103CfgT> m_vGroup;//分组名
	
private:
	char m_sFilePath[MAX_PATH];//文件路径
	char		LastError[Q_FRAME_CONST1024];         //错误信息
	bool init();
	Q_103StandardTags			*m_103TagManager;
	HWND					m_wnd;
	int m_nMsgID;//消息编号;
	int m_nCmdSignalID;

	

};
#endif