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
   e103XmlDataTypeNull=0,//δ֪
   e103XmlDataTypeInt=1,
   e103XmlDataTypeFloat=2,
   e103XmlDataTypeEnum=3,
   e103XmlDataTypeString=4
};

typedef struct ST_103CfgT
{
UINT group; //����
UINT id;    //���
char name[50]; //������
Q_103XmlDataType type; // Int,Float,ENUM,String
char value[100]; //ֵ
}ST_103CfgT;

typedef enum Q_103FrameSeqVfy
{
	e103FrameSeqVfyBoth=1,//ͬʱ���鷢�ͺͽ������" />
	e103FrameSeqVfyLocTxRemRx=2,//ֻУ�鱾�෢�ͺͶԲ�������
	e103FrameSeqVfyBothSend=3,  //ֻУ�鱾��ͶԲ෢�����
	e103FrameSeqVfyNull=4 // ��У�鷢�ͺͽ������
};
//���ٷ�������˳��
typedef enum Q_103ICSendSeq
{
	e103ICSendSeqPMS=1,//ң�š�ң�⡢ң��
	e103ICSendSeqPSM=2,
	e103ICSendSeqMPS=3, 
	e103ICSendSeqMSP=4,
	e103ICSendSeqSPM=5,
	e103ICSendSeqSMP=6,

};
//�ϴ�ң������
enum Q_103_AsduYcType{
	e103AsduYcType9=1,//ASDU9����һ��ֵ����ʱ�ꡢ��Ʒ��
	e103AsduYcType11=2,//ASDU11����Ȼ�ֵ����ʱ��
	e103AsduYcType13=3,//ASDU13���̸���������ʱ��
	e103AsduYcType21=4//ASDU21����һ��ֵ����ʱ�ꡢ��Ʒ��
};
typedef enum  Q_103OpenAppMode
{
	e103OpenAppMode_1=1, //�յ�U����֡������Ӧ�ô��䡢�յ�ֹͣ֡���ر�Ӧ�ô���
	e103OpenAppMode_2=2,//TCP���ӽ���������Ӧ�ô��䡢TCP���ӹرռ��ر�Ӧ�ô���
	e103OpenAppMode_3=3,//�յ�U����֡������Ӧ�ô��䡢TCP���ӹرռ��ر�Ӧ�ô���
	e103OpenAppMode_4=4  //TCP���ӽ���������Ӧ�ô��䡢�յ�Uֹͣ֡���ر�Ӧ�ô���
};

typedef enum  Q_103AsduVfy //ASDU������ϢУ�����
{
	e103AsduVfy_Total=1,
	e103AsduVfy_Vsq=2,
	e103AsduVfy_Cot=3,
	e103AsduVfy_Addr=4,
	e103AsduVfy_VsqAddr=5,
	e103AsduVfy_CotAddr=6,
	e103AsduVfy_Null=7

};
typedef enum  Q_103IntSeq //ASDU������ϢУ�����
{
	e103IntSeqLittle=1, //�ӵ͵���
	e103IntSeqBig=2     //�Ӹߵ���

};
typedef enum  Q_103FloatSeq //ASDU������ϢУ�����
{
	e103FloatSeqLittle=1, //�ӵ͵���
	e103FloatSeqBig=2     //�Ӹߵ���
};
typedef enum  Q_103T1App //ASDU������ϢУ�����
{
	e103T1AppCloseLink=1
};
typedef enum  Q_103T2App //ASDU������ϢУ�����
{
	e103T2AppSendS=1
};
typedef enum  Q_103T3App //ASDU������ϢУ�����
{
	e103T3AppSendTest=1
};
//I֡���Ͳ���
typedef enum  Q_103IF_SendMode 
{
	e103IF_SendIgnoreK=1,//I֡���Ͳ���KֵӰ��
	e103IF_SendLimitByK=2 //������I֡δ��ȷ����Ŀ���ڵ���Kֵ��ֹͣ����

};
typedef enum  Q_103IF_RecvMode 
{
	e103IF_RecvBeforeW_Act=1 //I֡���Ͳ���KֵӰ��

};
//���յ�����վʱ�����ȷ��,���յ�����վʱ�侲̬������ȷ��,��Уʱǰ������ϵͳʱ�����ȷ��,��Уʱ�������ϵͳʱ�����ȷ��
typedef enum  Q_103CS_Act
{
	e103CS_Act_Master       =1,  //���յ�����վʱ�����ȷ��
	e103CS_Act_MasterFix    =2,   //���յ�����վʱ�侲̬������ȷ��
	e103CS_Act_OwnSysBf	 =3,//��Уʱǰ������ϵͳʱ�����ȷ��
	e103CS_Act_OwnSysAf     =4//��Уʱ�������ϵͳʱ�����ȷ��

};
typedef enum Q_103YtExecMode
{
	e103YtExecModeNull=0,
	e103YtExecModeDirect=0,//ֱ��ִ��
	e103YtExecModeSelect=1 //ѡ��ִ��
}Q_103YtExecMode;
//��������asdu����
typedef enum Q_103YsetAsdu
{
	e103YsetAsdu30=0, //��һ���趨
	e103YsetAsdu31=1,//��Ȼ��趨
	e103YsetAsdu32=2 //�������趨
}Q_103YsetAsdu;

typedef enum Q_103YkAsdu
{
	e103YkAsdu9=0, //˫��ң��
	e103YkAsdu10=1 //����ң��
}Q_103YkAsdu;//ң��asdu

typedef struct Q_103_SrvPort
{
	char ip[30];//ip��ַ
	int port;
	bool valid;//�Ƿ���Ч
}Q_103_SrvPort;
class  Q_103BaseCfg
{
public:
	  Q_103_SrvPort localServer; //���˷�����
	  Q_103_SrvPort remServer[MAX_103_REMOTE_NUM];//�Զˣ����10��
	  int    m_nAsduLen;//Ӧ�ò��ַ����
	  int    m_nAsduAddr;//Ӧ�ò��ַ
	  int    m_nTxCauseLen;//����ԭ�򳤶�
	  int    m_nMaxFrameLen;
	  //int	m_nStationAddr1;
	  //int	m_nStationAddr2;
	  vector<int>m_vecStationAddr;
	  Q_103OpenAppMode m_eOpenAppMethord;
	  //֡���У�����
	  
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
//վ�ٻ�����
class  Q_103StCfg
{
public:
	int n_YxNumPerFrame;//ÿ֡ң�Ÿ���
	int n_YcNumPerFrame;//ÿ֡ң�����
	int n_YbNumPerFrame;//ÿ֡ң������
	int n_IcInterval;//���ټ��
	Q_103_AsduYcType m_103_AsdnYcType;//jwk add
	int m_YcBeginIndex;//ң����ʼ��ַ
	int m_YxBeginIndex;//ң����ʼ��ַ
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

//��������
class Q_103CtrlCfg
{
public:
	Q_103CtrlCfg(void){};
	virtual ~Q_103CtrlCfg(void){};
	Q_103YtExecMode m_YtExecMode;//ң��ģʽ
	Q_103YtExecMode m_YkExecMode;// ң��ģʽ
	Q_103YsetAsdu   m_YsetAsduType;//��������ʹ��ASDU����
	Q_103YkAsdu m_YkAsduType;
	int             getAsdu48Mode;

};
class  Q_103SyncClockCfg
{
public:
	Q_103SyncClockCfg(void){};
	virtual ~Q_103SyncClockCfg(void){};
	int m_nFixUsecs;//ʱ��ͬ��Ӧ�ò�ȷ��ʱ�侲̬����ֵ(����)
	Q_103CS_Act  m_eActMethord;//ʱ��ͬ��Ӧ�ò�ȷ�ϲ���

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
	bool bPrintTotal;//��ӡȫ��
	bool bPrintExplain;//��ӡ������Ϣ
	bool bPrintPrompt;//��ӡ��ʾ��Ϣ
	bool bPrintError;//��ӡ������Ϣ
};
class  Q_IEC103Setting : public Q_protocolSettingBase
{
public:
	Q_IEC103Setting(void);
	Q_IEC103Setting(Q_IEC103Setting *other);
	void EvalData(Q_IEC103Setting *other);
	virtual ~Q_IEC103Setting(void);
    void setFilePath(char *);
	//���õ�������ļ�λ��
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

	inline Q_103StandardTags*	getTagsManager() { return m_103TagManager;}
    char *getContentID(const int &v_idx,const int &v_grp=1);
protected:
	vector<ST_103CfgT> m_vContent;//����
	vector<ST_103CfgT> m_vGroup;//������
	
private:
	char m_sFilePath[MAX_PATH];//�ļ�·��
	char		LastError[Q_FRAME_CONST1024];         //������Ϣ
	bool init();
	Q_103StandardTags			*m_103TagManager;
	HWND					m_wnd;
	int m_nMsgID;//��Ϣ���;
	int m_nCmdSignalID;

	

};
#endif