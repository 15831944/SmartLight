#ifndef __Q_GSETTING_H_DEFINED_
#define __Q_GSETTING_H_DEFINED_

#include "Q_frameInclude.h"
#include "Q_runingUnitDefine.h"
#include "Log.h"
#include "ScadaAPI.h"
#include <vector>
using namespace std;
typedef struct ST_GeCfgT
{
	int id;
	char name[100]; //值
	char value[50];
	int grpid;
}ST_GeCfgT;
typedef struct ST_GeNodeInfo
{
	UINT             id;//结点编号
	char			name[Q_DATAMANAGER_TAGNAMELENGTH];//结点名
	BYTE runmethod;//运行方式1单机运行2冷备用运行3热备用运行
	BYTE			type;//是否是主机
	UINT             defaultmaster;//默认主机结点号
	BYTE             status; //状态0 offline,2启动中 1运行
	long             timestamp;//时标
	UINT             currentnode;//当前结点id
	BYTE             error;
	ST_GeNodeInfo(void)
	{
		id=0;
		memset(name,NULL,sizeof(name));
		runmethod=1;
		type=0;
		defaultmaster=0;
		status=0;
		timestamp=0;
		currentnode=0;
		error=0;
	}
}ST_GeNodeInfo;

class Q_GSetting
{
public:
	Q_GSetting();
	virtual ~Q_GSetting();
	void setXmlPath(char *);

	//是否自动运行true自动运行
	bool getAutoRun();
	//得到是否启用agc算法标志位
	bool getAgcalgorithm();
	//得到是否启用avc算法标志位
	bool getAvcalgorithm();
	//得到是否启用数据处理标志位
	bool getDataHandle();
	//得到记录日志标识
	bool getDataRecordFlg();
	//得到断面保存标识
	bool getDataSaveFlg();
	//得到断面时间长度
	int getDataSaveTime();
	//启用调度通道断开切为备机标识
	bool getSwitchWhenDiaoCrash();
	//样板风机计算进程开关
	bool getSampleGenerator();
	//电容器投切记录进程开关
	bool getFCCounting();
	//得到调度通道id
	int getDiaoduChannelID();
	//得到持续断开再次切换秒数
	int getSwithSecond();
	//得到接收组态切换命令的遥信id
	int getSwitchYXID();
	//得到双机备用的当前主机编号使用的遥测ID
	int getCurMasterYCID();
	////日志管理1,9
	bool getLogManager();
	//调试服务支持
	bool getDebugSerivce();
	bool init();
	char *getContentID(const int &v_idx,const int &v_grp=1);
	vector<ST_GeCtrlChl> m_vChannel;//内容
	vector<ST_GeNodeInfo> m_vNodeInfo;//内容
	
	//记录客户端通道的数据
	bool addChannel(Q_channelBase* pChannel);
	int getChannelCount();
	Q_channelBase* getChannel(int i);
	//记录服务器端通道数据
	bool addServerUnit(Q_tcpServerUnitInfo* pUnit);
	int getServerUnitCount();
	Q_tcpServerUnitInfo*  getServerUnit(int i);
	void setServerUnitCount(int v_count);
	void setChannelCount(int v_count);

	//记录备用通道
	bool addResChannel(Q_channelBase* pCh);
	int getChannelCountRes();
	Q_channelBase*  getResChannel(int i);
	void setChannelCountRes(int v_count);
	bool initBase();
	bool initNode();
	bool initChannel();
	//从实时库中读通道相关数据
	bool initChannelFromRtdb();
	void setLog(Log *v_log);
	//设置数据管理
    void setDataMgr(Q_dataManager *mgr);
	bool getProtoInfoByTypeID(int typeID,SCADA_Protocol &o_item);
private:
	//得到通讯链路表
	void GetCommLinkTable(CDBTable &v_table);
	//根据规约ID查询规约动态链接库名
	//char *GetProtocolLibName(int id,char *name);

	
private:
	char m_Path[MAX_PATH];
	char m_LastMsg[MAX_PATH];//消息
	vector<ST_GeCfgT > m_vContent;//内容
	int m_AlgCount;
	int m_PointCount;
	int m_YKCount;
	int m_YTCount;
	Q_channelBase*				m_arrayChannel[Q_FRAME_CONST32];//最多支持16个客户端的配置
	int m_nChannelCount;
	Q_tcpServerUnitInfo*        m_arrayServerUnit[Q_FRAME_CONST8];//最多支持8个服务器的配置
	int m_nServerUnitCount;//服务器个数
	Q_channelBase*				m_arrayChannelRes[Q_FRAME_CONST2];//最多支持2个客户端的配置,其实只能用到一个
	int m_nChannelCountRes;//服务器个数
	int m_CurMasterYCID;//标示当前主机是哪台的遥测号
	Log *m_log;
	Q_dataManager *m_pMgr;

};

#endif