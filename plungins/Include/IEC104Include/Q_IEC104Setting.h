
#if !defined(__Q_IEC104_SETTING_H__INCLUDED__)
#define __Q_IEC104_SETTING_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma warning(push)
#pragma warning(disable: 4251) 
#endif

#include "Q_104StdDefine.h"
#include "CommIEC104Def.h"
#include "tinyxmlinclude.h"
#include "gSystemDef.h"

#include <vector>
using namespace std;
#include "Q_104StandardTags.h"


typedef enum Q_XmlDataType
{
   eXmlDataTypeNull=0,//未知
   eXmlDataTypeInt=1,
   eXmlDataTypeFloat=2,
   eXmlDataTypeEnum=3,
   eXmlDataTypeString=4
}Q_XmlDataType;

typedef __Q_COMMIEC104_EXPORT struct ST_104CfgT
{
UINT group; //分组
UINT id;    //编号
char name[50]; //配置名
Q_XmlDataType type; // Int,Float,ENUM,String
char value[100]; //值
}ST_104CfgT;

typedef enum Q_104FrameSeqVfy
{
	eFrameSeqVfyBoth=1,//同时检验发送和接收序号" />
	eFrameSeqVfyLocTxRemRx=2,//只校验本侧发送和对侧接收序号
	eFrameSeqVfyBothSend=3,  //只校验本侧和对侧发送序号
	eFrameSeqVfyNull=4 // 不校验发送和接收序号
}Q_104FrameSeqVfy;
//总召发送数据顺序
typedef enum Q_104ICSendSeq
{
	eICSendSeqPMS=1,//遥信、遥测、遥步
	eICSendSeqPSM=2,
	eICSendSeqMPS=3, 
	eICSendSeqMSP=4,
	eICSendSeqSPM=5,
	eICSendSeqSMP=6,

}Q_104ICSendSeq;
//上传遥测类型
enum Q_AsduYcType{
	eAsduYcType9=1,//ASDU9、规一化值、无时标、有品质
	eAsduYcType11=2,//ASDU11、标度化值、无时标
	eAsduYcType13=3,//ASDU13、短浮点数、无时标
	eAsduYcType21=4//ASDU21、规一化值、无时标、无品质
};
typedef enum  Q_104OpenAppMode
{
	eOpenAppMode_1=1, //收到U启动帧即开放应用传输、收到停止帧即关闭应用传输
	eOpenAppMode_2=2,//TCP连接建立即开放应用传输、TCP连接关闭即关闭应用传输
	eOpenAppMode_3=3,//收到U启动帧即开放应用传输、TCP连接关闭即关闭应用传输
	eOpenAppMode_4=4  //TCP连接建立即开放应用传输、收到U停止帧即关闭应用传输
}Q_104OpenAppMode;

typedef enum  Q_104AsduVfy //ASDU公共信息校验策略
{
	eAsduVfy_Total=1,
	eAsduVfy_Vsq=2,
	eAsduVfy_Cot=3,
	eAsduVfy_Addr=4,
	eAsduVfy_VsqAddr=5,
	eAsduVfy_CotAddr=6,
	eAsduVfy_Null=7

}Q_104AsduVfy;
typedef enum  Q_104IntSeq //遥测数据类型整型上送顺序
{
	eIntSeqLittle=1, //从低到高
	eIntSeqBig=2     //从高到低

}Q_104IntSeq;
typedef enum  Q_104FloatSeq //遥测数据类型浮点型上送顺序
{
	eFloatSeqLittle=1, //从低到高
	eFloatSeqBig=2     //从高到低
}Q_104FloatSeq;
typedef enum  Q_104T1App //ASDU公共信息校验策略
{
	eT1AppCloseLink=1
}Q_104T1App;
typedef enum  Q_104T2App //ASDU公共信息校验策略
{
	eT2AppSendS=1
}Q_104T2App;
typedef enum  Q_104T3App //ASDU公共信息校验策略
{
	eT3AppSendTest=1
}Q_104T3App;
//I帧发送策略
typedef enum  Q_104IF_SendMode 
{
	eIF_SendIgnoreK=1,//I帧发送不受K值影响
	eIF_SendLimitByK=2 //发出的I帧未被确认数目大于等于K值即停止发送

}Q_104IF_SendMode;
typedef enum  Q_104IF_RecvMode 
{
	eIF_RecvBeforeW_Act=1 //I帧发送不受K值影响

}Q_104IF_RecvMode;
//以收到的主站时间进行确认,以收到的主站时间静态补偿后确认,以校时前的自有系统时间进行确认,以校时后的自有系统时间进行确认
typedef enum  Q_104CS_Act
{
	eCS_Act_Master       =1,  //以收到的主站时间进行确认
	eCS_Act_MasterFix    =2,   //以收到的主站时间静态补偿后确认
	eCS_Act_OwnSysBf	 =3,//以校时前的自有系统时间进行确认
	eCS_Act_OwnSysAf     =4//以校时后的自有系统时间进行确认

}Q_104CS_Act;
typedef enum Q_104YtExecMode
{
	eYtExecModeNull=0,
	eYtExecModeDirect=2,//直接执行
	eYtExecModeSelect=1 //选择执行
}Q_104YtExecMode;
//设置命令asdu类型
typedef enum Q_104YsetAsdu
{
	eYsetAsdu30=0,//规一化设定
	eYsetAsdu31=1,//标度化设定
	eYsetAsdu32=2,//浮点型设定
	eYsetAsdu3F=3 //带时标的浮点型
}Q_104YsetAsdu;
//遥控asdu类型
typedef enum Q_104YKAsdu
{
	eYKAsdu2D=0,//单点遥控
	eYKAsdu3A=1 //带时标的遥控
}Q_104YKAsdu;

typedef struct Q_SrvPort
{
	char ip[30];//ip地址
	int port;
	bool valid;//是否有效
}Q_SrvPort;
class  __Q_COMMIEC104_EXPORT Q_104BaseCfg
{
public:
	  Q_SrvPort localServer; //本端服务器
	  Q_SrvPort remServer[MAX_REMOTE_NUM];//对端，最多32个
	  int    m_nAsduLen;//应用层地址长度
	  int    m_nAsduAddr;//应用层地址
	  int    m_nTxCauseLen;//传送原因长度
	  int    m_nMaxFrameLen;
	 
	  Q_104OpenAppMode m_eOpenAppMethord;
	  //帧序号校验策略
	  
	  Q_104FrameSeqVfy m_eFrameSeqVfy;
	  Q_IEC104_ST_Type m_eCurStType;
	  Q_104AsduVfy   m_eAsduVfy;
	  Q_104IntSeq    m_eIntSeq;
	  Q_104FloatSeq  m_eFloatSeq;
	  Q_104T1App     m_eT1App;
	  Q_104T2App     m_eT2App;
	  Q_104T3App     m_eT3App;
	  Q_104IF_SendMode m_eIF_Send;
	  Q_104IF_RecvMode m_eIF_Recv;

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
	  bool m_bSendSOE;//是否发送SOE帧

	  int m_lAgcPlCYCStart;//本地AGC计划曲线遥测起始地址
      int m_lAgcPlCYCEnd;//本地AGC计划曲线遥测结束地址
	  int m_rAgcPlCYCStart;//远方AGC计划曲线遥测起始地址
      int m_rAgcPlCYCEnd;//远方AGC计划曲线遥测结束地址

	  int m_lAvcPlCYCStart;//本地AVC计划曲线遥测起始地址
      int m_lAvcPlCYCEnd;//本地AVC计划曲线遥测结束地址
	  int m_rAvcPlCYCStart;//远方AVC计划曲线遥测起始地址
      int m_rAvcPlCYCEnd;//远方AVC计划曲线遥测结束地址

	  char m_lAgcPath[MAX_PATH];//本地AGC计划曲线文件目录
	  char m_rAgcPath[MAX_PATH];
	  char m_lAvcPath[MAX_PATH];
	  char m_rAvcPath[MAX_PATH];

public:
	Q_104BaseCfg(void)
	{
		localServer.valid=false;
		for(int i=0;i<MAX_REMOTE_NUM;i++)
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
		m_eFrameSeqVfy=eFrameSeqVfyNull;
		m_nFrameI_MaxCounter=0x7FFF;
		m_eCurStType=eStation_Unknown;
		m_eAsduVfy=eAsduVfy_Total;

		m_nFrameI_MaxCounter=32767;

		m_eIntSeq=eIntSeqLittle;
		m_eFloatSeq=eFloatSeqLittle;
		m_eT1App=eT1AppCloseLink;
		m_eT2App=eT2AppSendS;
		m_eT3App=eT3AppSendTest;
		m_eIF_Send=eIF_SendLimitByK;
		m_eIF_Recv=eIF_RecvBeforeW_Act;
		m_bSendSFrame=false;
		m_bSendIC=true;
		m_bSendStartDt=true;
	}
};
//站召换配置
class __Q_COMMIEC104_EXPORT  Q_104StCfg
{
public:
	int n_YxNumPerFrame;//每帧遥信个数
	int n_YcNumPerFrame;//每帧遥测个数
	int n_YbNumPerFrame;//每帧遥步个数
	int n_IcInterval;//总召间隔
	Q_AsduYcType m_AsdnYcType;//jwk add
	int m_YcBeginIndex;//遥测起始地址
	int m_YxBeginIndex;//遥信起始地址
	Q_104StCfg(void)
	{
     n_YxNumPerFrame=50;
     n_YcNumPerFrame=20;
     n_YbNumPerFrame=50;
	 n_IcInterval=10;
	 m_AsdnYcType=eAsduYcType13;
	 m_YcBeginIndex=0;
	 m_YxBeginIndex=0;
	}


};

//控制配置
class __Q_COMMIEC104_EXPORT Q_104CtrlCfg
{
public:
	Q_104CtrlCfg(void){};
	virtual ~Q_104CtrlCfg(void){};
	Q_104YtExecMode m_YtExecMode;//遥调模式
	Q_104YtExecMode m_YkExecMode;// 遥控模式
	Q_104YsetAsdu   m_YsetAsduType;//设置命令使用ASDU类型
	Q_104YKAsdu     m_YKAsduType;//遥控类型
	int             getAsdu48Mode;

};
class __Q_COMMIEC104_EXPORT Q_104SyncClockCfg
{
public:
	Q_104SyncClockCfg(void);
	virtual ~Q_104SyncClockCfg(void){};
	int m_nFixUsecs;//时钟同步应用层确认时间静态补偿值(毫秒)
	Q_104CS_Act  m_eActMethord;//时钟同步应用层确认策略
	bool m_bExecCS;//是否响应对时。


};

class __Q_COMMIEC104_EXPORT Q_104PrintCfg
{
public:
	Q_104PrintCfg(void)
	{
		bPrintTotal=false;
		bPrintExplain=false;
		bPrintPrompt=false;
		bPrintError=false;
	};
	virtual ~Q_104PrintCfg(void){};
	bool bPrintTotal;//打印全部
	bool bPrintExplain;//打印解释信息
	bool bPrintPrompt;//打印提示信息
	bool bPrintError;//打印错误信息
};
class __Q_COMMIEC104_EXPORT Q_IEC104Setting : public Q_protocolSettingBase
{
public:
	Q_IEC104Setting(void);
	Q_IEC104Setting(Q_IEC104Setting *other);
	void EvalData(Q_IEC104Setting *other);
	virtual ~Q_IEC104Setting(void);
    void setFilePath(char *);
	//设置点表配置文件位置
	bool setPtFilePath(char *);
	Q_104BaseCfg *getBaseSetting(Q_104BaseCfg *v_cfg);
	Q_104StCfg   *getStationSetting(Q_104StCfg *v_cfg);
    Q_104CtrlCfg *getControlSetting(Q_104CtrlCfg *v_cfg);
	Q_104SyncClockCfg *getSyncClockSetting(Q_104SyncClockCfg *v_cfg);
	void setChannelClass(const char *className);
	const char* getChannelClass();
    bool initSetting();
	virtual bool reloadLog();

	inline void setTagsManager(Q_104StandardTags* vTags) 
	{
		m_104TagManager=vTags;
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

	inline Q_104StandardTags*	getTagsManager() { return m_104TagManager;}
    char *getContentID(const int &v_idx,const int &v_grp=1);

	void setClientAddr(char *v_addr);
	char *getClientAddr();

    int getZTChlYCBegin();//fs20150820获取遥测数据元素起始地址
	int getZTChlYXBegin();//fs20150820获取遥信数据元素起始地址

	bool getIsEnableACSend();//fs20150820获取遥信数据元素起始地址

protected:
	vector<ST_104CfgT> m_vContent;//内容
	vector<ST_104CfgT> m_vGroup;//分组名
	
private:
	char m_sFilePath[MAX_PATH];//文件路径
	char		LastError[Q_FRAME_CONST1024];         //错误信息
	bool init();
	Q_104StandardTags			*m_104TagManager;
	HWND					m_wnd;
	int m_nMsgID;//消息编号;
	int m_nCmdSignalID;
	int m_nDebugToolMsgID;
	char m_strClientAddr[50];//客户端IP地址
	char m_className[MAX_PATH];//类名称，用于子类
};
#endif