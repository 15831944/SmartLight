#ifndef __Q_PROTOCOL_MODBUS_TCP_SERVERSTANDARD_EXPORT__
#define __Q_PROTOCOL_MODBUS_TCP_SERVERSTANDARD_EXPORT__

#ifdef WINDOWS_ENV
#pragma warning(disable:4251)
#endif

#include "Q_frameInclude.h"
#include "Q_ModbusSlaveDef.h"
#include "Q_runingUnitDefine.h"
#include "Q_tcpServerUnitInfo.h"

#include "Q_ModbusDefine.h"

#include <vector>
using namespace std;
/** 
* @interface Q_protocolModbusTcpServerStandardCommand 
* @brief modbus tcp server ͨѶ��Լ����
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/

//__Q_COMMMODBUSTCPSERVERSTANDARD_EXPORT
class Q_protocolModbusSlaveCommand : public Q_protocolCommandBase
{
public:
	Q_protocolModbusSlaveCommand(void);
	virtual ~Q_protocolModbusSlaveCommand(void);

public:
	


protected:
	
	
};





/** 
* @interface Q_protocolModbusTcpServerStandardCommandManager
* @brief modbus tcp serverͨѶ��Լ�������
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/

//__Q_COMMMODBUSTCPSERVERSTANDARD_EXPORT
class Q_protocolModbusSlaveCommandManager : public Q_protocolCommandManagerBase
{
public:
	Q_protocolModbusSlaveCommandManager(void);
	virtual ~Q_protocolModbusSlaveCommandManager(void);

public:
	virtual bool   addCommand(Q_protocolCommandBase* pCommand);
	

};






/** 
* @interface Q_protocolModbusTcpServerStandardSetting 
* @brief modbus tcp �����ͨѶ��Լ����
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/

//__Q_COMMMODBUSTCPSERVERSTANDARD_EXPORT
class Q_protocolModbusSlaveSetting : public Q_protocolSettingBase
{
public:
	Q_protocolModbusSlaveSetting(void);
	Q_protocolModbusSlaveSetting(Q_protocolModbusSlaveSetting *other);
	virtual ~Q_protocolModbusSlaveSetting(void);
	void setFilePath(char *);
	bool initSetting();
	char *getContentID(const int &v_idx,const int &v_grp=1);
	//�趨ң��ĵ�ַ��Χ����Ч��ң�Ÿ���
	void               setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	//�趨ң�ŵĵ�ַ��Χ����Ч��ң�Ÿ���
	void               setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount);

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

	//ң����ʼ�Ĵ�����ַ
	int                 getRegisterBegin();
	//ң����ʼ��Ȧ��ַ
	int                 getRelayBegin();

	//ң�ص�ַ
	int                 getYkFunCode();//ң�صĹ�����
	int                 getYkOnValue();//ң�غϵ�ֵ
	int                 getYkOffValue();//ң�ضϿ���ֵ
	int                 getYcFunCode();//ң�⹦����
	/*update by tjs 2015.04.17 begin*/
	int                 getYcUpType();//ң��������������
	/*end */
	int                 getYxFunCode();//ң�Ź�����
	int                 getYtFunCode();//ң��������
/*update by tjs 2015.04.17 begin*/
	int                 getYtDataType();//ȡ��ң����������
	int                 getYtType();//ң������1���2����
	/*end */
	int                 getSyncFirst();//ң��������
	bool                getSendClearBuffer();//�õ�����һ�����һ�»�������ʶ
	
	int getDeviceId();//���豸��ַ
	ModbusFloatUpSeq getFloatUpSeq();
	ModbusYTMethod   getYTMethod();
	//ң���ֽ�˳��
	ModbusFloatUpSeq getYTByteSeq();
	void SetWndInfo(HWND wnd,int MsgID,int CmdID);
	HWND getWndHwnd();
	int getWndMsgID();
	int getWndCmdID();
	virtual bool reloadLog();
	bool getCloseSyncCode();
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
	HWND    m_wnd;
	int m_nMsgID;//��Ϣ���;
	int m_nCmdID;
	int m_nDebugToolMsgID;
};


/** 
* @interface Q_protocolModbusTcpServerStandard
* @brief Modbus tcp�����ͨѶ��Լ
* @author yangxi
* @date 2012-5-23
* @see 
* @bug No know bugs so far.
*/

//__Q_COMMMODBUSTCPSERVERSTANDARD_EXPORT
class Q_protocolModbusSlave : public Q_protocolBase
{
public:
	Q_protocolModbusSlave(void);
	virtual ~Q_protocolModbusSlave(void);

public:
	void SetWndInfo(HWND wnd,int MsgID,int CmdID);
	
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


	void SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);


protected:

	Q_ComCpuEndian m_cpuEndian;//cpu���뷽ʽ;
	HWND m_wnd;
	int m_nMsgID;
	int m_nCmdID;//��Ϣ���
	int m_nDebugToolMsgID;
	//��ң������ 
	//dataȥ��ǰ�����ֽ�Address��Function,������CRCУ��������
	//len ���������ĳ���,Ҳ����ȥ��adddress+function���ּ���crc�����ֽڣ����Դ�ƽ
	virtual bool explainFrameYC(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//��ң��
	virtual bool explainFrameYX(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//ң��
	virtual bool explainFrameYK(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//ң��
	virtual bool explainFrameYT(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	virtual bool explainFrameYT_M(Q_MODBUS_ASDU *header,BYTE *data,const int &len);
	//asdu ���͵���������
	//len Ϊasdu.data�������ݵĳ���
	bool findSyncIndex(char* buff,int buffSize,int startPos,int& findPos);
	//len ��asdu �д�addr��ʼ�ı������ĳ���
	virtual bool sendData(Q_MODBUS_ASDU2 *asdu,int len);
	virtual bool explainDataEx(char* buff,int nCount,int& nCurrentPos);
	bool readFile(char *v_path,vector<MOD_PLAN_CURVE> &vec);
	//���ʹ�����
	bool sendErroCode(Q_MODBUS_ASDU *header,BYTE code);
	bool findYtExists(Q_MODBUS_ASDU *header,int startAddr,int &nYCindex,int &nYTindex);
	ModbusFloatUpSeq m_FloatUpSeq;
	ModbusFloatUpSeq m_YtByteSeq;
	ModbusYTMethod   m_YtMethod;
	Q_MODBUS_ExecCmd m_SigArray;
	Q_protocolModbusSlaveSetting* m_PrintSetting;
	char m_LastMsg [240];
	int m_yc_idx;
/* update ty tjs 2015.04.17*/
private:
	static const int YtDataTypeFloat = 1;    //ң������Ϊ����getYtDataType()����ֵ
	static const int YtDataTypeINT2 = 2;      //ң������Ϊ2�ֽ�����getYtDataType()����ֵ
	static const int YtDataTypeINT4 = 3;	  //ң������Ϊ4�ֽ�����getYtDataType()����ֵ

	static const int YtTypeMulti = 1;//д����Ĵ���
	static const int YtTypeSingle = 2;//д�����Ĵ���

	static const int YcUpTypeFloat = 1;    //ң������Ϊ����getYcUpType()����ֵ
	static const int YcUpTypeINT2 = 2;      //ң������Ϊ2�ֽ�����getYcUpType()����ֵ
	static const int YcUpTypeINT4 = 3;	  //ң������Ϊ4�ֽ�����getYcUpType()����ֵ
	/* end */
};


extern Q_dataManager *m_pMgrModbusSlaveProtocolSetting; //fs20150708



#endif//__Q_PROTOCOLMODBUSTCPSERVERSTANDARD_H_INCLUDED__
