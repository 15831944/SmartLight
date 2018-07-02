#include "stdafx.h"
#include "Q_ProtocolIEC104Slave.h"
#include "Q_IEC104CommFun.h"
#include "Q_104CP56Time2a.h"
#include "tinyxmlinclude.h"
class Q_protocolIEC104SlaveShanDongPlaneCurve : public Q_protocolIEC104Slave
{
public:
	Q_protocolIEC104SlaveShanDongPlaneCurve(void);
	virtual ~Q_protocolIEC104SlaveShanDongPlaneCurve(void){}
	virtual bool initializeProtocol();
protected:
	virtual int handleCommmd_SE(Q_cmdIEC104 *v_cmd);
private:
	int initPlaneCurveDate();//�Ѽƻ������ļ����ݶ�ȡ��д�뵽�ƻ����߶�Ӧ��ң���
	int readRemoutePlaneCurveFile(const char* shanDongFilePath, float fValue[96]);
	int readLocalPlaneCurveFile(const char* shanDongFilePath, float fValue[288]);
	int saveDataToYC(int startId, int endId, float* vFloat); 
	int writeFile(const char* xmlFileTemplatePath,float value, int id);//�������ݵ�����xml�ļ�
	//int readLocalAgcPlaneCurveFile(const char* shanDongFilePath, float fValue[96]);
	//int readRemoteAgcPlaneCurveFile(const char* shanDongFilePath, float fValue[96]);
	//int readLocalAvcPlaneCurveFile(const char* shanDongFilePath, float fValue[96]);
	//int readRemoteAvcPlaneCurveFile(const char* shanDongFilePath, float fValue[96]);

	int savePlaneCurveDate();//�Ѽƻ��������ݱ��浽�ƻ������ļ�

	int m_localAgcPlaneCurveYcIdStart;
	int m_localAgcPlaneCurveYcIdEnd;

	int m_RemoteAgcPlaneCurveYcIdStart;
	int m_RemoteAgcPlaneCurveYcIdEnd;

	int m_localAvcPlaneCurveYcIdStart;
	int m_localAvcPlaneCurveYcIdEnd;

	int m_RemoteAvcPlaneCurveYcIdStart;
	int m_RemoteAvcPlaneCurveYcIdEnd;
	
};