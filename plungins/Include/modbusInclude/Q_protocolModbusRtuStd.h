//*
//*
//* Q_protocolModbusTcpServerStandard.h
//*
//*

#ifndef __Q_PROTOCOL_MODBUS_RTU_STD_H_INCLUDED__
#define __Q_PROTOCOL_MODBUS_RTU_STD_H_INCLUDED__
#ifdef WINDOWS_ENV
#pragma warning(disable:4251)
#endif
#include "Q_frameInclude.h"
#include "Q_ModbusRtuStdDef.h"
#include "Q_ModbusDefine.h"
#include "Q_runingUnitDefine.h"
#include <vector>
using namespace std;
class Q_protocolModbusRtuStdCommand : public Q_protocolCommandBase
{
public:
	Q_protocolModbusRtuStdCommand(void);
	virtual ~Q_protocolModbusRtuStdCommand(void);

public:
	//�豸��ַ
	inline void					setDestinationId(unsigned char nId) { m_nDestinationId = nId;}
	inline unsigned char		getDestinationId() { return m_nDestinationId;}
	//��������
	inline void					setCommandModbusType(Q_Modbus_CommandType eType) { m_commandCDTType = eType;}
	inline Q_Modbus_CommandType    getCommandModbusType() { return m_commandCDTType;}

	inline void					setCommandModbusStatus(Q_Modbus_CommandStatus eStatus) { m_commandCDTStatus = eStatus;}
	inline Q_Modbus_CommandStatus	getCommandModbusStatus() { return m_commandCDTStatus;}

	//�Ĵ�����ַ
	inline void					setObjectIndex(int nIndex) { m_nObjectIndex = nIndex;}
	inline int		getObjectIndex() { return m_nObjectIndex;}
	//ֵ��Ϣ
	inline void					setSwitchValue(bool bVal) { m_bOpen = bVal;}
	inline bool					getSwitchValue() { return m_bOpen;}
	inline void					setAnalogValue(float fValue) { m_fValue = fValue;}
	inline float				getAnalogValue() { return m_fValue;}
	inline void					setIntValue(int nValue) { m_nValue = nValue;}
	inline int				    getIntValue() { return m_nValue;}
	//��������������ʱ��Ϣ�������� ����Ҫ������Щ������ɾ������
	inline void				    setMaxTryCount(int nMaxTryCount) { m_nMaxTryCount=nMaxTryCount;}
	inline int				    getMaxTryCount() { return m_nMaxTryCount;}
	inline void				    setCurTryNum(int nCurTryNum) { m_nCurTryNum=nCurTryNum;}
	inline int				    getCurTryNum() { return m_nCurTryNum;}
	int m_curIdx;
protected:
	unsigned char				m_nDestinationId; //վ���
	Q_Modbus_CommandType			m_commandCDTType;//��������
	Q_Modbus_CommandStatus			m_commandCDTStatus;//״̬Ĭ���趨ѡ��
	int				m_nObjectIndex;//վ������豸���
	bool						m_bOpen;/// ������
	float						m_fValue;//ģ����
	int                         m_nValue;//������
	int                         m_nMaxTryCount;//���������
	int                         m_nCurTryNum;//��ǰʵ����
};





/** 
* @interface Q_protocolModbusTcpServerStandardCommandManager
* @brief modbus tcp serverͨѶ��Լ�������
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class Q_ModbusRtuStdCmdMgr : public Q_protocolCommandManagerBase
{
public:
	Q_ModbusRtuStdCmdMgr(void);
	virtual ~Q_ModbusRtuStdCmdMgr(void);

public:
	virtual bool				addCommand(Q_protocolCommandBase* pCommand);
	virtual bool	addCommand(Q_CommandVal *v_val);
	Q_protocolModbusRtuStdCommand*		findCommand(Q_Modbus_CommandType eType,Q_Modbus_CommandStatus eStatus,int nObjIndex);
	bool						removeCommand(Q_Modbus_CommandType eType,Q_Modbus_CommandStatus eStatus,int nObjIndex);
	bool						removeCommand(Q_protocolCommandBase* pCommand);


};






/** 
* @interface Q_protocolModbusTcpServerStandardSetting 
* @brief modbus tcp �����ͨѶ��Լ����
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class Q_protcolModbusRtuStdSetting : public Q_protocolSettingBase
{
public:
	Q_protcolModbusRtuStdSetting(void);
	virtual ~Q_protcolModbusRtuStdSetting(void);
	void setFilePath(char *);
	bool initSetting();
	char *getContentID(const int &v_idx,const int &v_grp=1);
	//�趨ң��ĵ�ַ��Χ����Ч��ң�Ÿ���
	void               setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	//�趨ң�ŵĵ�ַ��Χ����Ч��ң�Ÿ���
	void               setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	//�趨�ƻ�����ң��ĵ�ַ��ת����Ч����Ϣ����
	void               setPlanScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	//ң�⿪ʼ��ַ
	int                 getAnalogBegin();
	int                 getAnalogEnd();
	//ң�ſ�ʼ��ַ
	int                 getPointBegin();
	int                 getPointEnd();
	//ң�����
	int                 getAnalogCount();
	//ң�Ÿ���
	int                 getPointCount();
	//�ƻ����߿�ʼ��ַ
	int                 getPlanAddrBegin();
	//�ƻ����߽�����ַ
	int                 getPlanAddrEnd();
	//�ƻ�������Ϣ����
	int                 getPlanInfoCount();

	//ң����ʼ�Ĵ�����ַ
	int                 getRegisterBegin();
	//ң����ʼ��Ȧ��ַ
	int                 getRelayBegin();
	int                 getReadRegisterCode();//ң����Ĵ���������
	int                 getReadYXRegisterCode();//ң�Ŷ��Ĵ���������
	int                 getYTCode();//ң��
	int                 getYKCode();//ң��
	bool                getPrintPacket();//�õ���ӡ��Ϣ
	bool                getPrintPrompt();//��ӡ��ʾ��Ϣ
	bool                getPrintError();//��ӡ������Ϣ
	bool                getPrintTranslate();//��ӡ��Լ������Ϣ
	virtual bool reloadLog();


	int getDeviceId();//���豸��ַ
	int getPerTime();//����ң�⡢ң�ż��ʱ��
	int getSleepPerTimes();//�Ĵ��������
	ModbusFloatUpSeq getFloatUpSeq();
	ModbusCheckCodSeq getCheckCodeSeq();
	ModbusIntUpSeq getIntUpSeq();
	int getCRCErrorHandle();
	NewModbusYTMethod   getYTMethod();
	int getAlgCmdCount();
	int getPntCmdCount();
	vector<ST_ModCmdCfg> m_vCommand;// ����
	vector<ST_ModCmdYXCfg> m_vYXCommand;// ����
protected:
	vector<ST_ModCfgT > m_vContent;//����
	vector<ST_ModCfgT > m_vGroup;//������

private:
	char m_sFilePath[MAX_PATH];//�ļ�·��
	char LastError[Q_FRAME_CONST1024];         //������Ϣ
	bool init();
private:
	int m_alg_count;//ң�����
	int m_alg_begin;//ң����ʼ��ַ
	int m_alg_end;//ң�������ַ
	int m_pnt_count;//ң��Ϣ����
	int m_pnt_begin;//ң����ʼ��ַ
	int m_pnt_end;//ң�Ž�����ַ
	int m_plan_addr_begin;//�ƻ����߿�ʼ��ַ
	int m_plan_addr_end;//�ƻ����߽�����ַ
	int m_plan_count;//�ƻ�������Ϣ����
	int m_alg_cmd_count;//ң���������
	int m_pnt_cmd_count;//ң���������

};


/** 
* @interface Q_protocolModbusTcpServerStandard
* @brief Modbus tcp�����ͨѶ��Լ
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
//class __Q_MODBUS_RTU_STD_EXPORT Q_protocolModbusRtuStd : public Q_protocolBase
class Q_protocolModbusRtuStd : public Q_protocolBase
{
public:
	Q_protocolModbusRtuStd(void);
	virtual ~Q_protocolModbusRtuStd(void);

public:
	virtual bool				initializeProtocol();
	virtual bool				explainData();
	virtual bool				makeFrame();
	virtual bool				terminateProtocol();
	virtual bool				startProcess();
	virtual bool				stopProcess();

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
protected:
	bool makeFrameEx(Q_protocolModbusRtuStdCommand* pCommand);
	Q_ComCpuEndian m_cpuEndian;//cpu���뷽ʽ;
	//��ң������ 
	//dataȥ��ǰ�����ֽ�Address��Function,������CRCУ��������
	//len ���������ĳ���,Ҳ����ȥ��adddress+function���ּ���crc�����ֽڣ����Դ�ƽ
	virtual bool explainFrameYC(Q_MODBUS_ASDU_RTU *asdu,BYTE *data);
	//��ң��
	virtual bool explainFrameYX(Q_MODBUS_ASDU_RTU *asdu,BYTE *data);
	//ң��
	virtual bool explainFrameYK(Q_MODBUS_ASDU_RTU *asdu,BYTE *data);
	//�����ص�ң������
	virtual bool explainFrameYT(Q_MODBUS_ASDU_RTU *asdu,BYTE *data);
	//����4�ֽ�intң��
	virtual bool explainFrameYT_Int4(Q_MODBUS_ASDU_RTU *asdu,BYTE *data);
	//д����Ĵ������ݣ�ң��
	virtual bool explainFrameYT_M(Q_MODBUS_ASDU_RTU *asdu,BYTE *data);
	//����ң������
	virtual bool handleFrameYT(Q_protocolModbusRtuStdCommand* pCommand);
	//����ң������
	virtual bool handleFrameYT_Int4(Q_protocolModbusRtuStdCommand* pCommand);
	//����ң������
	virtual bool handleFrameYC(Q_protocolModbusRtuStdCommand* pCommand);
	//����ң������
	virtual bool handleFrameYX(Q_protocolModbusRtuStdCommand* pCommand);
	virtual bool handleFrameYT_M(Q_protocolModbusRtuStdCommand* pCommand);
	//����ң������
	virtual bool handelFrameYK(Q_protocolModbusRtuStdCommand* pCommand);
	//�����ݿ��в�ѯ����
	bool fillTagInformation(LPQ_RECORDDATATAG pData,int nAdr,int nDevID);
	//len ��asdu �д�addr��ʼ�ı������ĳ���
	virtual bool sendData(Q_MODBUS_ASDU_RTU *asdu,int len);
	virtual bool explainDataEx(char* buff,int nCount,int& nCurrentPos);
	//���ʹ�����
	bool sendErroCode(Q_MODBUS_ASDU_RTU *header,BYTE code);
	char chInfo[Q_FRAME_CONST128];
	char m_LastMsg[Q_FRAME_CONST1024];
	virtual void onRxMsg(BYTE *msgbuf, unsigned int len);
	void onTxMsg(BYTE *msgbuf, unsigned int len);
	virtual void OnErrorMsg(char *msg,unsigned int len);
	virtual void OnShowMsg(char *msg,unsigned int len);
	WORD CRC16(BYTE* data, WORD len);
	void SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);
	
private:
	Q_protocolModbusRtuStdCommand* m_LastCmd;
	Q_protocolModbusRtuStdCommand* m_LastCmdForCmd;
	vector<Q_protocolModbusRtuStdCommand*>m_vecCmd;
	ModbusFloatUpSeq m_FloatUpSeq;
	ModbusIntUpSeq m_IntUpSeq;
	ModbusCheckCodSeq m_CheckCodeSeq;
	NewModbusYTMethod   m_YtMethod;
	int m_CRCErroHandle; //����10֡����CRCУ������Ƿ��ж�Ϊͨ���Ͽ�0��1��
	int m_CRCErroCount;
	HWND					m_wnd;
	int m_nMsgID;//��Ϣ���;
	Q_protcolModbusRtuStdSetting* m_PrintSetting;

	void setNextAnalog(Q_protocolModbusRtuStdCommand* cmd);
	void setNextDigital(Q_protocolModbusRtuStdCommand* cmd);
	time_t m_ReciveTime;
};



#endif
