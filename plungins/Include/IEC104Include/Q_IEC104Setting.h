
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
   eXmlDataTypeNull=0,//δ֪
   eXmlDataTypeInt=1,
   eXmlDataTypeFloat=2,
   eXmlDataTypeEnum=3,
   eXmlDataTypeString=4
}Q_XmlDataType;

typedef __Q_COMMIEC104_EXPORT struct ST_104CfgT
{
UINT group; //����
UINT id;    //���
char name[50]; //������
Q_XmlDataType type; // Int,Float,ENUM,String
char value[100]; //ֵ
}ST_104CfgT;

typedef enum Q_104FrameSeqVfy
{
	eFrameSeqVfyBoth=1,//ͬʱ���鷢�ͺͽ������" />
	eFrameSeqVfyLocTxRemRx=2,//ֻУ�鱾�෢�ͺͶԲ�������
	eFrameSeqVfyBothSend=3,  //ֻУ�鱾��ͶԲ෢�����
	eFrameSeqVfyNull=4 // ��У�鷢�ͺͽ������
}Q_104FrameSeqVfy;
//���ٷ�������˳��
typedef enum Q_104ICSendSeq
{
	eICSendSeqPMS=1,//ң�š�ң�⡢ң��
	eICSendSeqPSM=2,
	eICSendSeqMPS=3, 
	eICSendSeqMSP=4,
	eICSendSeqSPM=5,
	eICSendSeqSMP=6,

}Q_104ICSendSeq;
//�ϴ�ң������
enum Q_AsduYcType{
	eAsduYcType9=1,//ASDU9����һ��ֵ����ʱ�ꡢ��Ʒ��
	eAsduYcType11=2,//ASDU11����Ȼ�ֵ����ʱ��
	eAsduYcType13=3,//ASDU13���̸���������ʱ��
	eAsduYcType21=4//ASDU21����һ��ֵ����ʱ�ꡢ��Ʒ��
};
typedef enum  Q_104OpenAppMode
{
	eOpenAppMode_1=1, //�յ�U����֡������Ӧ�ô��䡢�յ�ֹͣ֡���ر�Ӧ�ô���
	eOpenAppMode_2=2,//TCP���ӽ���������Ӧ�ô��䡢TCP���ӹرռ��ر�Ӧ�ô���
	eOpenAppMode_3=3,//�յ�U����֡������Ӧ�ô��䡢TCP���ӹرռ��ر�Ӧ�ô���
	eOpenAppMode_4=4  //TCP���ӽ���������Ӧ�ô��䡢�յ�Uֹͣ֡���ر�Ӧ�ô���
}Q_104OpenAppMode;

typedef enum  Q_104AsduVfy //ASDU������ϢУ�����
{
	eAsduVfy_Total=1,
	eAsduVfy_Vsq=2,
	eAsduVfy_Cot=3,
	eAsduVfy_Addr=4,
	eAsduVfy_VsqAddr=5,
	eAsduVfy_CotAddr=6,
	eAsduVfy_Null=7

}Q_104AsduVfy;
typedef enum  Q_104IntSeq //ң������������������˳��
{
	eIntSeqLittle=1, //�ӵ͵���
	eIntSeqBig=2     //�Ӹߵ���

}Q_104IntSeq;
typedef enum  Q_104FloatSeq //ң���������͸���������˳��
{
	eFloatSeqLittle=1, //�ӵ͵���
	eFloatSeqBig=2     //�Ӹߵ���
}Q_104FloatSeq;
typedef enum  Q_104T1App //ASDU������ϢУ�����
{
	eT1AppCloseLink=1
}Q_104T1App;
typedef enum  Q_104T2App //ASDU������ϢУ�����
{
	eT2AppSendS=1
}Q_104T2App;
typedef enum  Q_104T3App //ASDU������ϢУ�����
{
	eT3AppSendTest=1
}Q_104T3App;
//I֡���Ͳ���
typedef enum  Q_104IF_SendMode 
{
	eIF_SendIgnoreK=1,//I֡���Ͳ���KֵӰ��
	eIF_SendLimitByK=2 //������I֡δ��ȷ����Ŀ���ڵ���Kֵ��ֹͣ����

}Q_104IF_SendMode;
typedef enum  Q_104IF_RecvMode 
{
	eIF_RecvBeforeW_Act=1 //I֡���Ͳ���KֵӰ��

}Q_104IF_RecvMode;
//���յ�����վʱ�����ȷ��,���յ�����վʱ�侲̬������ȷ��,��Уʱǰ������ϵͳʱ�����ȷ��,��Уʱ�������ϵͳʱ�����ȷ��
typedef enum  Q_104CS_Act
{
	eCS_Act_Master       =1,  //���յ�����վʱ�����ȷ��
	eCS_Act_MasterFix    =2,   //���յ�����վʱ�侲̬������ȷ��
	eCS_Act_OwnSysBf	 =3,//��Уʱǰ������ϵͳʱ�����ȷ��
	eCS_Act_OwnSysAf     =4//��Уʱ�������ϵͳʱ�����ȷ��

}Q_104CS_Act;
typedef enum Q_104YtExecMode
{
	eYtExecModeNull=0,
	eYtExecModeDirect=2,//ֱ��ִ��
	eYtExecModeSelect=1 //ѡ��ִ��
}Q_104YtExecMode;
//��������asdu����
typedef enum Q_104YsetAsdu
{
	eYsetAsdu30=0,//��һ���趨
	eYsetAsdu31=1,//��Ȼ��趨
	eYsetAsdu32=2,//�������趨
	eYsetAsdu3F=3 //��ʱ��ĸ�����
}Q_104YsetAsdu;
//ң��asdu����
typedef enum Q_104YKAsdu
{
	eYKAsdu2D=0,//����ң��
	eYKAsdu3A=1 //��ʱ���ң��
}Q_104YKAsdu;

typedef struct Q_SrvPort
{
	char ip[30];//ip��ַ
	int port;
	bool valid;//�Ƿ���Ч
}Q_SrvPort;
class  __Q_COMMIEC104_EXPORT Q_104BaseCfg
{
public:
	  Q_SrvPort localServer; //���˷�����
	  Q_SrvPort remServer[MAX_REMOTE_NUM];//�Զˣ����32��
	  int    m_nAsduLen;//Ӧ�ò��ַ����
	  int    m_nAsduAddr;//Ӧ�ò��ַ
	  int    m_nTxCauseLen;//����ԭ�򳤶�
	  int    m_nMaxFrameLen;
	 
	  Q_104OpenAppMode m_eOpenAppMethord;
	  //֡���У�����
	  
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
      int m_nT0;//ͨ���ж��ж�ʱ��(��)
	  int m_nT1;//��ʱʱ��T1ֵ(��)
	  int m_nT2;//��ʱʱ��T2ֵ(��)
	  int m_nT3;//��ʱʱ��T3ֵ(��)
	  int m_nFrameI_MaxCounter;//I֡�����Ŀ
	  bool m_bSendSFrame;//�Ƿ���S֡
	  bool m_bSendStartDt;//�Ƿ�������֡
	  bool m_bSendIC;//�Ƿ�������
	  bool m_bSendAC;//�Ƿ��ͱ仯ң��
	  int m_acTime;  //�仯�����ͼ��
	  bool m_bSendSOE;//�Ƿ���SOE֡

	  int m_lAgcPlCYCStart;//����AGC�ƻ�����ң����ʼ��ַ
      int m_lAgcPlCYCEnd;//����AGC�ƻ�����ң�������ַ
	  int m_rAgcPlCYCStart;//Զ��AGC�ƻ�����ң����ʼ��ַ
      int m_rAgcPlCYCEnd;//Զ��AGC�ƻ�����ң�������ַ

	  int m_lAvcPlCYCStart;//����AVC�ƻ�����ң����ʼ��ַ
      int m_lAvcPlCYCEnd;//����AVC�ƻ�����ң�������ַ
	  int m_rAvcPlCYCStart;//Զ��AVC�ƻ�����ң����ʼ��ַ
      int m_rAvcPlCYCEnd;//Զ��AVC�ƻ�����ң�������ַ

	  char m_lAgcPath[MAX_PATH];//����AGC�ƻ������ļ�Ŀ¼
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
//վ�ٻ�����
class __Q_COMMIEC104_EXPORT  Q_104StCfg
{
public:
	int n_YxNumPerFrame;//ÿ֡ң�Ÿ���
	int n_YcNumPerFrame;//ÿ֡ң�����
	int n_YbNumPerFrame;//ÿ֡ң������
	int n_IcInterval;//���ټ��
	Q_AsduYcType m_AsdnYcType;//jwk add
	int m_YcBeginIndex;//ң����ʼ��ַ
	int m_YxBeginIndex;//ң����ʼ��ַ
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

//��������
class __Q_COMMIEC104_EXPORT Q_104CtrlCfg
{
public:
	Q_104CtrlCfg(void){};
	virtual ~Q_104CtrlCfg(void){};
	Q_104YtExecMode m_YtExecMode;//ң��ģʽ
	Q_104YtExecMode m_YkExecMode;// ң��ģʽ
	Q_104YsetAsdu   m_YsetAsduType;//��������ʹ��ASDU����
	Q_104YKAsdu     m_YKAsduType;//ң������
	int             getAsdu48Mode;

};
class __Q_COMMIEC104_EXPORT Q_104SyncClockCfg
{
public:
	Q_104SyncClockCfg(void);
	virtual ~Q_104SyncClockCfg(void){};
	int m_nFixUsecs;//ʱ��ͬ��Ӧ�ò�ȷ��ʱ�侲̬����ֵ(����)
	Q_104CS_Act  m_eActMethord;//ʱ��ͬ��Ӧ�ò�ȷ�ϲ���
	bool m_bExecCS;//�Ƿ���Ӧ��ʱ��


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
	bool bPrintTotal;//��ӡȫ��
	bool bPrintExplain;//��ӡ������Ϣ
	bool bPrintPrompt;//��ӡ��ʾ��Ϣ
	bool bPrintError;//��ӡ������Ϣ
};
class __Q_COMMIEC104_EXPORT Q_IEC104Setting : public Q_protocolSettingBase
{
public:
	Q_IEC104Setting(void);
	Q_IEC104Setting(Q_IEC104Setting *other);
	void EvalData(Q_IEC104Setting *other);
	virtual ~Q_IEC104Setting(void);
    void setFilePath(char *);
	//���õ�������ļ�λ��
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

	//��ӡ��Ϣ����
	bool getPrintSetting(Q_LogType type);
	/**************************************************************************************************
	@Function: SetWndInfo
	@Description: ���ô��ھ��
	@Calls:
	@Called By:
	@Input: wnd ������,msgID��ӡ��Ϣ��ID
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

    int getZTChlYCBegin();//fs20150820��ȡң������Ԫ����ʼ��ַ
	int getZTChlYXBegin();//fs20150820��ȡң������Ԫ����ʼ��ַ

	bool getIsEnableACSend();//fs20150820��ȡң������Ԫ����ʼ��ַ

protected:
	vector<ST_104CfgT> m_vContent;//����
	vector<ST_104CfgT> m_vGroup;//������
	
private:
	char m_sFilePath[MAX_PATH];//�ļ�·��
	char		LastError[Q_FRAME_CONST1024];         //������Ϣ
	bool init();
	Q_104StandardTags			*m_104TagManager;
	HWND					m_wnd;
	int m_nMsgID;//��Ϣ���;
	int m_nCmdSignalID;
	int m_nDebugToolMsgID;
	char m_strClientAddr[50];//�ͻ���IP��ַ
	char m_className[MAX_PATH];//�����ƣ���������
};
#endif