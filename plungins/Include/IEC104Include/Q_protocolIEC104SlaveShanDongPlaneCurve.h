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
	int initPlaneCurveDate();//把计划曲线文件数据读取并写入到计划曲线对应的遥测点
	int readRemoutePlaneCurveFile(const char* shanDongFilePath, float fValue[96]);
	int readLocalPlaneCurveFile(const char* shanDongFilePath, float fValue[288]);
	int saveDataToYC(int startId, int endId, float* vFloat); 
	int writeFile(const char* xmlFileTemplatePath,float value, int id);//保存数据到本地xml文件
	//int readLocalAgcPlaneCurveFile(const char* shanDongFilePath, float fValue[96]);
	//int readRemoteAgcPlaneCurveFile(const char* shanDongFilePath, float fValue[96]);
	//int readLocalAvcPlaneCurveFile(const char* shanDongFilePath, float fValue[96]);
	//int readRemoteAvcPlaneCurveFile(const char* shanDongFilePath, float fValue[96]);

	int savePlaneCurveDate();//把计划曲线数据保存到计划曲线文件

	int m_localAgcPlaneCurveYcIdStart;
	int m_localAgcPlaneCurveYcIdEnd;

	int m_RemoteAgcPlaneCurveYcIdStart;
	int m_RemoteAgcPlaneCurveYcIdEnd;

	int m_localAvcPlaneCurveYcIdStart;
	int m_localAvcPlaneCurveYcIdEnd;

	int m_RemoteAvcPlaneCurveYcIdStart;
	int m_RemoteAvcPlaneCurveYcIdEnd;
	
};