//*
//*
//* Q_protocolModbusPlanCurve.h
//*
//*
#pragma once

#ifndef __Q_protocolModbusPlanCurve_H_INCLUDED__
#define __Q_protocolModbusPlanCurve_H_INCLUDED__
#pragma warning(disable:4251)
#include "Q_frameInclude.h"
#include "Q_ModbusPlanCurveDef.h"
#include "Q_ModbusDefine.h"

#include <vector>
using namespace std;


/** 
* @interface Q_protocolModbusPlanCurveCommand 
* @brief modbus tcp server ͨѶ��Լ����
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class __Q_COMMMODBUSPLANCURVE_EXPORT Q_protocolModbusPlanCurveCommand : public Q_protocolCommandBase
{
public:
	Q_protocolModbusPlanCurveCommand(void);
	virtual ~Q_protocolModbusPlanCurveCommand(void);
	
};





/** 
* @interface Q_protocolModbusPlanCurveCommandManager
* @brief modbus tcp serverͨѶ��Լ�������
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class __Q_COMMMODBUSPLANCURVE_EXPORT Q_protocolModbusPlanCurveCommandManager : public Q_protocolCommandManagerBase
{
public:
	Q_protocolModbusPlanCurveCommandManager(void);
	virtual ~Q_protocolModbusPlanCurveCommandManager(void);

public:
	virtual bool addCommand(Q_protocolCommandBase* pCommand);
	
};






/** 
* @interface Q_protocolModbusPlanCurveSetting 
* @brief modbus tcp �����ͨѶ��Լ����
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class __Q_COMMMODBUSPLANCURVE_EXPORT Q_protocolModbusPlanCurveSetting : public Q_protocolSettingBase
{
public:
	Q_protocolModbusPlanCurveSetting(void);
	Q_protocolModbusPlanCurveSetting(Q_protocolModbusPlanCurveSetting *other);
	virtual ~Q_protocolModbusPlanCurveSetting(void);
	void setFilePath(char *);
	bool initSetting();
	char *getContentID(const int &v_idx,const int &v_grp=1);

	int getSyncFirst();//ң��������
	bool getPrintPacket();//��ӡ���ı�ʶ
	bool getPrintError();//��ӡ������Ϣ��ʶ
	bool getPrintPrompt();//��ʾ��Ϣ��ʶ
	bool getPlanOn();//�õ���ӡ��Ϣ
	bool getSendClearBuffer();//�õ�����һ�����һ�»�������ʶ
	
	int getDeviceId();//���豸��ַ
	ModbusFloatUpSeq getFloatUpSeq();
	ModbusYTMethod   getYTMethod();
	//ң���ֽ�˳��
	ModbusFloatUpSeq getYTByteSeq();
	void SetWndInfo(HWND wnd,int MsgID,int CmdID);
	HWND getWndHwnd();
	int getWndMsgID();
	int getWndCmdID();
	inline int getDebugToolMsgID(){return m_nDebugToolMsgID;}
	inline void setDebugToolMsgID(int nMsgID){m_nDebugToolMsgID = nMsgID;}
	virtual bool reloadLog();
	void SetSignalID(int vMsgID);
	
	int getRealPowerYCNo();//ʵʱ���ʵ�ң���
	int getRealRemoteYCNo();//ʵʱ���ȹ��ʵ�ң���
	int getRealLocalYCNo();//ʵʱ���ع��ʵ�ң���
	int getRealTatgetYCNo();//ʵʱĿ���ң���

	int getRemoteFlagYXNo();//���ȱ�־ң�ź�
	int getPlanTypeYXNo();//�ǵ��Ȼ��Ǳ��ص�ң�ź�
	int getCurveType();//�õ���AGC���߻���AVC����
	int getAgcPlanID();
protected:
	vector<ST_ModCfgT > m_vContent;//����
private:
	char m_sFilePath[MAX_PATH];//�ļ�·��
	char LastError[1024];         //������Ϣ
	bool init();
private:
	HWND    m_wnd;
	int m_nMsgID;//��Ϣ���;
	int m_nCmdID;
	int m_nAgcPlanID;//AGC���߱�־
	int m_nDebugToolMsgID;

};


/** 
* @interface Q_protocolModbusPlanCurve
* @brief Modbus tcp�����ͨѶ��Լ
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class __Q_COMMMODBUSPLANCURVE_EXPORT Q_protocolModbusPlanCurve : public Q_protocolBase
{
public:
	Q_protocolModbusPlanCurve(void);
	virtual ~Q_protocolModbusPlanCurve(void);

public:
	void SetWndInfo(HWND wnd,int MsgID,int CmdID);
	//void SetCurveType(int nType);//�����������ͣ�1ΪAGC���ߣ�2ΪAVC����
	virtual bool				initializeProtocol();
	virtual bool				explainData();
	virtual bool				makeFrame();
	virtual bool				terminateProtocol();
	virtual bool				startProcess();
	virtual bool				stopProcess();

	//msgҪ��ӡ��Ϣ 
	//len ��ӡ��Ϣ����
	//type����0 ��Ϣ 1������Ϣ 2���� 3����
    void writefile(char *msg,int len, int type);
protected:
	Q_ComCpuEndian m_cpuEndian;//cpu���뷽ʽ;
	HWND m_wnd;
	int m_nMsgID;
	int m_nCmdID;//��Ϣ���
	//��ң������ 
	//dataȥ��ǰ�����ֽ�Address��Function,������CRCУ��������
	//len ���������ĳ���,Ҳ����ȥ��adddress+function���ּ���crc�����ֽڣ����Դ�ƽ
	virtual bool explainFrameYC(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	virtual bool explainFrameYT_M(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//asdu ���͵���������
	//len Ϊasdu.data�������ݵĳ���
	bool findSyncIndex(char* buff,int buffSize,int startPos,int& findPos);
	//len ��asdu �д�addr��ʼ�ı������ĳ���
	virtual bool sendData(Q_MODBUS_ASDU2 *asdu,int len);
	virtual bool explainDataEx(char* buff,int nCount,int& nCurrentPos);
	//bool readPlanCurve(Q_MODBUS_ASDU *header,BYTE *data,const int &len,int startAddr,int regCount);
	//bool readFile(char *v_path,vector<MOD_PLAN_CURVE> &vec);
	//���ʹ�����
	bool sendErroCode(Q_MODBUS_ASDU *header,BYTE code);
	ModbusFloatUpSeq m_FloatUpSeq;
	ModbusFloatUpSeq m_YtByteSeq;
	ModbusYTMethod   m_YtMethod;
	Q_MODBUS_ExecCmd m_SigArray;
	bool m_bPrint;
	Q_protocolModbusPlanCurveSetting* m_ProtocolSetting;
	char m_LastError[240];

	bool getPlanCurve(eTypePlanCurve ePlanType);//�õ�xml�ļ���288����
	bool getRealPower(float &fValue);//�õ�ʵʱ������ң��
	bool getRealRemote(float &fValue);//�õ�ʵʱ����ֵ��ң��
	bool getRealLocal(float &fValue);//�õ�ʵʱ����ֵ�ã�ң��
	bool getRealTarget(float &fValue);//�õ�ʵʱĿ��ֵ��ң��
	bool getRemoteflag(bool &bValue);//�õ������������ң��
	bool getCurPlanType(float &fValue);//�õ���ǰ��״̬�Ǳ��ػ��ǵ��ȣ�ң��
	bool getTodayLocalCurve();//�õ���������ߣ����û�У����ɽ��������
private:
	bool ifNeedReadLocal();
	bool saveLocalPlan();
	bool SecondToTime(int nSecond,int &Hour,int &Minute,int &Second);
private:
	float m_fRemotePlan[288];
	float m_fLocalPlan[288];

	bool m_bNeedReadRemote;
//	bool m_bNeedReadLocal;
	bool m_bNeedSendRemote;
	bool m_bNeedSendLocal;
	int m_LocalReadCmd;//Ĭ��0ͬʱ��ʾ���ɹ���2��ʾ���������߶�ʧ��
	int m_LocalWriteCmd;//Ĭ��0ͬʱ��ʾд�ɹ���2��ʾд����ʧ��
	int m_RemoteReadYCTime;

	int m_RealPowerYCNo;
	int m_RealRemoteYCNo;
	int m_RealLocalYCNo;
	int m_RealTatgetYCNo;
	int m_RemoteFlagYXNo;
	int m_PlanTypeYXNo;
	
	int m_CurDayOfMonth;
	int m_CurMonth;
	int m_CurYear;
	int m_CurveType;
};



#endif//__Q_protocolModbusPlanCurve_H_INCLUDED__
