#ifndef __Q_PROTOCOL_MODBUSTCP_MASTER_H_INCLUDED__
#define __Q_PROTOCOL_MODBUSTCP_MASTER_H_INCLUDED__


#ifdef WINDOWS_ENV
#pragma warning(disable:4251)
#endif
#include "Q_frameInclude.h"
#include "Q_ModbusTcpMasterDef.h"
#include "Q_ModbusDefine.h"
#include "Q_runingUnitDefine.h"
#include <vector>
using namespace std;


// __Q_MODBUSTCP_MASTER_EXPORT
class Q_protocolModbusMasterCommand : public Q_protocolCommandBase
{
public:
	Q_protocolModbusMasterCommand(void);
	virtual ~Q_protocolModbusMasterCommand(void);

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
	//ֵ��Ϣ ���ΪfalseΪ���ͽ���
	//inline void					setSwitchValue(bool bVal) { m_bOpen = bVal;}
	//inline bool					getSwitchValue() { return m_bOpen;}
	/////////////////������ֽ����ֽڻ��ģʽ//////////////////////
	/*inline void					setBMixed(bool bVal) { m_bMixed = bVal;}	
	inline bool					getBMixed() { return m_bMixed;}*/

	inline void					setAnalogValue(float fValue) { m_fValue = fValue;}
	inline float				getAnalogValue() { return m_fValue;}
	inline void					setIntValue(int nValue) { m_nValue = nValue;}
	inline int				    getIntValue() { return m_nValue;}
	inline void					setPointType(int nValue) { m_nYxType = nValue;}
	inline int				    getPointType() { return m_nYxType;}
	//��������������ʱ��Ϣ�������� ����Ҫ������Щ������ɾ������
	inline void				    setMaxTryCount(int nMaxTryCount) { m_nMaxTryCount=nMaxTryCount;}
	inline int				    getMaxTryCount() { return m_nMaxTryCount;}
	inline void				    setCurTryNum(int nCurTryNum) { m_nCurTryNum=nCurTryNum;}
	inline int				    getCurTryNum() { return m_nCurTryNum;}
	int m_curIdx;
	//����ң�����������
	void setYcRequireType(int n_type);
	Q_eExplainYcType getYcReuireType();
	//��������Ĺ�����
	void setFuncCode(int funcode);
	int getFuncCode();
protected:
	unsigned char				m_nDestinationId; //վ���
	Q_Modbus_CommandType			m_commandCDTType;//��������
	Q_Modbus_CommandStatus			m_commandCDTStatus;//״̬Ĭ���趨ѡ��
	int				m_nObjectIndex;//վ������豸���
	//bool						m_bOpen;/// ������
	//bool                        m_bMixed;//��磬���ֽ����ֽڻ����ʽ
	float						m_fValue;//ģ����
	int                         m_nValue;//������
	int							m_nYxType;//ң������
	int                         m_nMaxTryCount;//���������
	int                         m_nCurTryNum;//��ǰʵ����
	int                         m_nLastCmdType;
	Q_eExplainYcType            m_eReqYcType;//ң�������
	int m_nFuncCode;//����Ĺ�����ֵ
};





/** 
* @interface Q_protocolModbusTcpServerStandardCommandManager
* @brief modbus tcp serverͨѶ��Լ�������
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
//// __Q_MODBUSTCP_MASTER_EXPORT
class  Q_ModbusMasterCommandManager : public Q_protocolCommandManagerBase
{
public:
	Q_ModbusMasterCommandManager(void);
	virtual ~Q_ModbusMasterCommandManager(void);

public:
	virtual bool				addCommand(Q_protocolCommandBase* pCommand);
	Q_protocolModbusMasterCommand*		findCommand(Q_Modbus_CommandType eType,Q_Modbus_CommandStatus eStatus,int nObjIndex);
	bool						removeCommand(Q_Modbus_CommandType eType,Q_Modbus_CommandStatus eStatus,int nObjIndex);
	bool						removeCommand(Q_protocolCommandBase* pCommand);


};


/*
* @interface Q_protocolModbusTcpServerStandardSetting 
* @brief modbus tcp �����ͨѶ��Լ����
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/
class  Q_protocolModbusMasterSetting : public Q_protocolSettingBase
{
public:
	Q_protocolModbusMasterSetting(void);
	virtual ~Q_protocolModbusMasterSetting(void);
	void setFilePath(char *);
	bool initSetting();
	char *getContentID(const UINT &v_idx,const UINT &v_grp=1);
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
	int                 getSyncFirstCode();//д����Ĵ���������

	virtual bool reloadLog();
	int getWriteRegisterCode();//д��Ȧ��ң��������
	int getWriteYxRegisterCode();//д��Ȧ��ң�ع�����

	int getPerTime();//����ң�⡢ң�ż��ʱ��
	int getSleepPerTimes();//�Ĵ��������
	ModbusFloatUpSeq getFloatUpSeq();
	ModbusIntUpSeq getIntUpSeq();
	ModbusYTMethod   getYTMethod();
	int getAlgCmdCount();
	int getPntCmdCount();
	vector<ST_ModCmdCfg> m_vCommand;//����
	vector<ST_ModCmdYXCfg> m_vYXCommand;//����
	int getReadRegisterCodeEx();
	int getReadYXRegisterCodeEx();
	//�õ���ʼ�빦���Զ����㹦�ܿ���
	bool getStartCodeAutoFlg();
	//���ʱ�豸���ù�������湦�ܿ���(��һ�ع���)
	//��Ҫԭ������Ϊ��һ�ع������豸��ַһ�����豸�ڱ�Ų�һ���Ĳ�ͬ��¼
	//ֻ���ٻ��Ĺ������в���ļ�¼,���ǵļĴ����Ų�Ψһ�����������
	//2017-1-11 6:42 tjs
	bool getFuncCodeEqualDeviceFlg();

	ModbusIntUpSeq getInt2BytesUpSeq();
protected:
	vector<ST_ModCfgT > m_vContent;//����
	vector<ST_ModCfgT > m_vGroup;//������
	
private:
	char m_sFilePath[MAX_PATH];//�ļ�·��
	char LastError[Q_FRAME_CONST1024];         //������Ϣ
	bool init(void);
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
class  Q_protocolModbusMaster : public Q_protocolBase
{
public:
	Q_protocolModbusMaster(void);
	virtual ~Q_protocolModbusMaster(void);
	void SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);

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
	//void SetWndInfo(HWND wnd,int MsgID);
protected:
	bool makeFrameEx(Q_protocolModbusMasterCommand* pCommand);
	Q_ComCpuEndian m_cpuEndian;//cpu���뷽ʽ;
	//��ң������
	//dataȥ��ǰ�����ֽ�Address��Function,������CRCУ��������
	//len ���������ĳ���,Ҳ����ȥ��adddress+function���ּ���crc�����ֽڣ����Դ�ƽ
	virtual bool explainFrameYC(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//��ң��
	virtual bool explainFrameYX(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//ң��
	virtual bool explainFrameYK(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//�����ص�ң������
	virtual bool explainFrameYT(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//д����Ĵ������ݣ�ң��
	virtual bool explainFrameYT_M(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//�������ֽ�floatң������
	virtual bool handleFrameYT(Q_protocolModbusMasterCommand* pCommand);
	//����intң������(���ֽڵ�)
	bool handleFrameYtInt(Q_protocolModbusMasterCommand* pCommand);
	//����intң������(���ֽڵ�),֧���з��ŵ�4�ֽ�����
	bool handleFrameYtInt4(Q_protocolModbusMasterCommand* pCommand);	
	//����8�ֽ�double���Ͷ��ֽ�ң��
	bool handleFrameYtFloat64Ex(Q_protocolModbusMasterCommand* pCommand);
	//����ң������
	virtual bool handleFrameYC(Q_protocolModbusMasterCommand* pCommand);
	//����ң������
	virtual bool handleFrameYX(Q_protocolModbusMasterCommand* pCommand);
	virtual bool handleFrameYT_M(Q_protocolModbusMasterCommand* pCommand);
	//����Ǳ�׼ң��float03
	virtual bool handleFrameYT_M_03(Q_protocolModbusMasterCommand* pCommand);
	//����ң������
	virtual bool handelFrameYK(Q_protocolModbusMasterCommand* pCommand);
	//asdu ���͵���������
	//len Ϊasdu.data�������ݵĳ���
	bool findSyncIndex(char* buff,int buffSize,int startPos,int& findPos);
	//�����ݿ��в�ѯ����
	bool fillTagInformation(LPQ_RECORDDATATAG pData,int nAdr,int nDevID);
	//len ��asdu �д�addr��ʼ�ı������ĳ���
	virtual bool sendData(Q_MODBUS_ASDU2 *asdu,int len);
	virtual bool explainDataEx(char* buff,int nCount,int& nCurrentPos);
	//���ʹ�����
	bool sendErroCode(Q_MODBUS_ASDU *header,BYTE code);
//	char chInfo[Q_FRAME_CONST256];
	char m_LastMsg[Q_FRAME_CONST256];

	//��ԭ�����󳤣������
	bool makeFrameYT(Q_protocolModbusMasterCommand*pCommand,int nFirstCommandIndex);
	//���������Ϣ
	bool explainError(Q_MODBUS_ASDU *asdu);
private:
	bool explainFrameYCFloat(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	bool explainFrameYCMixed(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	bool explainFrameYCInt(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	bool explainFrameYCDouble8(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	bool explainFrameYCHuaWei(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	bool explainFrameYCInt4(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	bool explainFrameYCUnsignedInt4(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	bool explainFrameYCUShort(Q_MODBUS_ASDU *header,BYTE *data,const int &len,Q_protocolModbusMasterCommand*cmd,int startAddr);
	
private:
	void setNextAnalog(Q_protocolModbusMasterCommand* cmd);
	void setNextDigital(Q_protocolModbusMasterCommand* cmd);
private:
	Q_protocolModbusMasterCommand* m_LastCmd;
	vector<Q_protocolModbusMasterCommand*>m_vecCmd;
	ModbusFloatUpSeq m_FloatUpSeq;
	ModbusIntUpSeq m_IntUpSeq;
	ModbusYTMethod   m_YtMethod;
	//HWND					m_wnd;
	int m_nMsgID;//��Ϣ���;
	Q_protocolModbusMasterSetting* m_PrintSetting;

	ModbusIntUpSeq m_Int2BytesUpSeq;//2�ֽ�

};
#endif
