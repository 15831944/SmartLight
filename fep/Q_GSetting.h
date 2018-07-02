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
	char name[100]; //ֵ
	char value[50];
	int grpid;
}ST_GeCfgT;
typedef struct ST_GeNodeInfo
{
	UINT             id;//�����
	char			name[Q_DATAMANAGER_TAGNAMELENGTH];//�����
	BYTE runmethod;//���з�ʽ1��������2�䱸������3�ȱ�������
	BYTE			type;//�Ƿ�������
	UINT             defaultmaster;//Ĭ����������
	BYTE             status; //״̬0 offline,2������ 1����
	long             timestamp;//ʱ��
	UINT             currentnode;//��ǰ���id
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

	//�Ƿ��Զ�����true�Զ�����
	bool getAutoRun();
	//�õ��Ƿ�����agc�㷨��־λ
	bool getAgcalgorithm();
	//�õ��Ƿ�����avc�㷨��־λ
	bool getAvcalgorithm();
	//�õ��Ƿ��������ݴ����־λ
	bool getDataHandle();
	//�õ���¼��־��ʶ
	bool getDataRecordFlg();
	//�õ����汣���ʶ
	bool getDataSaveFlg();
	//�õ�����ʱ�䳤��
	int getDataSaveTime();
	//���õ���ͨ���Ͽ���Ϊ������ʶ
	bool getSwitchWhenDiaoCrash();
	//������������̿���
	bool getSampleGenerator();
	//������Ͷ�м�¼���̿���
	bool getFCCounting();
	//�õ�����ͨ��id
	int getDiaoduChannelID();
	//�õ������Ͽ��ٴ��л�����
	int getSwithSecond();
	//�õ�������̬�л������ң��id
	int getSwitchYXID();
	//�õ�˫�����õĵ�ǰ�������ʹ�õ�ң��ID
	int getCurMasterYCID();
	////��־����1,9
	bool getLogManager();
	//���Է���֧��
	bool getDebugSerivce();
	bool init();
	char *getContentID(const int &v_idx,const int &v_grp=1);
	vector<ST_GeCtrlChl> m_vChannel;//����
	vector<ST_GeNodeInfo> m_vNodeInfo;//����
	
	//��¼�ͻ���ͨ��������
	bool addChannel(Q_channelBase* pChannel);
	int getChannelCount();
	Q_channelBase* getChannel(int i);
	//��¼��������ͨ������
	bool addServerUnit(Q_tcpServerUnitInfo* pUnit);
	int getServerUnitCount();
	Q_tcpServerUnitInfo*  getServerUnit(int i);
	void setServerUnitCount(int v_count);
	void setChannelCount(int v_count);

	//��¼����ͨ��
	bool addResChannel(Q_channelBase* pCh);
	int getChannelCountRes();
	Q_channelBase*  getResChannel(int i);
	void setChannelCountRes(int v_count);
	bool initBase();
	bool initNode();
	bool initChannel();
	//��ʵʱ���ж�ͨ���������
	bool initChannelFromRtdb();
	void setLog(Log *v_log);
	//�������ݹ���
    void setDataMgr(Q_dataManager *mgr);
	bool getProtoInfoByTypeID(int typeID,SCADA_Protocol &o_item);
private:
	//�õ�ͨѶ��·��
	void GetCommLinkTable(CDBTable &v_table);
	//���ݹ�ԼID��ѯ��Լ��̬���ӿ���
	//char *GetProtocolLibName(int id,char *name);

	
private:
	char m_Path[MAX_PATH];
	char m_LastMsg[MAX_PATH];//��Ϣ
	vector<ST_GeCfgT > m_vContent;//����
	int m_AlgCount;
	int m_PointCount;
	int m_YKCount;
	int m_YTCount;
	Q_channelBase*				m_arrayChannel[Q_FRAME_CONST32];//���֧��16���ͻ��˵�����
	int m_nChannelCount;
	Q_tcpServerUnitInfo*        m_arrayServerUnit[Q_FRAME_CONST8];//���֧��8��������������
	int m_nServerUnitCount;//����������
	Q_channelBase*				m_arrayChannelRes[Q_FRAME_CONST2];//���֧��2���ͻ��˵�����,��ʵֻ���õ�һ��
	int m_nChannelCountRes;//����������
	int m_CurMasterYCID;//��ʾ��ǰ��������̨��ң���
	Log *m_log;
	Q_dataManager *m_pMgr;

};

#endif