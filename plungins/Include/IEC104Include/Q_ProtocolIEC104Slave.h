#if !defined(__Q_PROTOCOL_104_SLAVE_H__INCLUDED__)
#define __Q_PROTOCOL_104_SLAVE_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma warning(push)
#pragma warning(disable: 4251) 
#endif

#include "Q_frameInclude.h"
#include "CommIEC104Def.h"
#include "CommIEC104SlaveDef.h"
#include "Q_104StdDefine.h"
#include "Q_IEC104Setting.h"
#include "Q_IEC104CommandManager.h"
#include "Q_protocolIEC104.h"
#include <vector>
using namespace std;

typedef struct _IEC104_VARIBLE_YC
{
	int id;
	float value;
	//int m_nZfID;//���ת��ID��Ϊ0��ȡת��ID��ֵ
	_IEC104_VARIBLE_YC()
	{
		id=-1;
		value=0;
	//	m_nZfID=0;
	}
}IEC104_VARIBLE_YC;
typedef struct _IEC104_VARIBLE_YX
{
	int id;
	bool value;
	//int m_nZfID;//���ת��ID��Ϊ0��ȡת��ID��ֵ
	_IEC104_VARIBLE_YX()
	{
     id=-1;
	 value=false;
	// m_nZfID=0;
	}
}IEC104_VARIBLE_YX;
typedef struct _IEC104_VARIBLE_SOE
{
	int id;//DPU�б��
	bool value;
	bool changed;//�Ƿ��λ
	SYSTEMTIME timeSOE;//ң�ű�λʱ��SOE
	//int m_nZfID;//���ת��ID��Ϊ0��ȡת��ID��ֵ
	_IEC104_VARIBLE_SOE()
	{
     id=-1;
	 value=false;
	 changed = false;
	// m_nZfID=0;
	}
}IEC104_VARIBLE_SOE;
/** 
* @interface 
* @brief IEC-60870-5-104 ��׼ͨѶ��Լ ��վ��
* @author TanJingSheng
* @date 2012-05-02
* @see 
* @bug No know bugs so far.
*/
class Q_protocolIEC104Slave : public Q_protocolIEC104
{
public:
	Q_protocolIEC104Slave(void);
	virtual ~Q_protocolIEC104Slave(void);
public:
	virtual bool		terminateProtocol();

	virtual bool	   startProcess();
	virtual bool	   stopProcess();
	virtual bool initializeProtocol();

	void SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);
	//�趨����ִ������ı��
	void SetSigExecCmd(int MsgID);
	
	/**************************************************************************************************
	@Function: TxHandler
	@Description: ������ ɨ��֮������飬���磺(1)���ڿ���״̬�·��Ͳ���֡�ĳ�ʱ  ���Ӳ��ԣ�
	(2)��δȷ��I��ʽAPDU�ﵽk��ʱ�����ͷ�ֹͣ����
	(3) ���ͻ����APDU�ĳ�ʱ
	(4)��ʱ��û�����ݷ��ͣ�����S֡ȷ�Ͻ��յ������ݡ�
	(5)������û�д�����δ�����Kֵ�ﵽKʱ�����ͷ�ֹͣ������Ϣ
	עKȡֵ12��Wȡֵ8�������ͻ��������յ�K��I�壬�ͷ���һS������ȷ�ϣ�

	@Input: msgbuf �������ݣ����ݴ�����ֵ��ʼ;len���ȣ���ʶ������ֵ��ʼ������������֡���ȡ�
	@Output:
	@Return:
	@Update:
	@Others:��Ҫ����һ���������߳������������
	**************************************************************************************************/
	virtual bool TxHandler();
	/**************************************************************************************************
	@Function: decodeMsg_I
	@Description:����I��ʽ֡
	@Input: msgbuf �������ݣ����ݴ�����ֵ��ʼ;len���ȣ���ʶ������ֵ��ʼ������������֡���ȡ�
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int decodeMsg_I(BYTE *msgbuf, const UINT len);
	/**************************************************************************************************
	@Function: decodeMsg_U
	@Description:����U��ʽ��֡
	@Input: msgbuf �������ݣ����ݴ�����ֵ��ʼ;len���ȣ���ʶ������ֵ��ʼ������������֡���ȡ�
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int decodeMsg_U(BYTE *msgbuf, const UINT len);
protected:
	/**************************************************************************************************
	@Function: sendInterrogateInd
	@Description:  �������ټ���ȷ���뷢������ֹͣȷ��
	@Input: asdu ��ʾ������ַ,cause ����ԭ��һ��ֻ��0x07 ����ȷ����0x0A ������ֹ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int sendInterrogateInd(int asdu,int cause);
	/**************************************************************************************************
	@Function: send_IC_Response
	@Description:  �������ټ���ȷ���뷢������ֹͣȷ��
	@Input: asdu ��ʾ������ַ,cause ����ԭ��һ��ֻ��0x07 ����ȷ����0x0A ������ֹ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int send_IC_Response(int asdu,int cause);
	/**************************************************************************************************
	@Function: send_SP_Response
	@Description:  ���͵�����Ϣ�ķ���ֵ
	@Input: asdu ��ʾ������ַ,cause ����ԭ��,adr ��ַ ,value��Ӧ��ֵ�� type Ĭ���ǵ���ң��
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int sendYkResponse(const int &asdu,const int &cause,const int &adr,BYTE *value,const int &type,const UINT &len=1);
	/**************************************************************************************************
	@Function: send_CS_Response
	@Description:  ����ʱ��ͬ��������Ϣ
	@Input: asdu ��ʾ������ַ,cause ����ԭ��,data �������� ֻ��ʱ��������Ϣ,len ʱ��������Ϣ���ݳ���
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int send_CS_Response(int asdu,int cause,BYTE *data,const UINT &len);
	/**************************************************************************************************
	@Function: send_C_TS_TA_1_Response
	@Description:  ���ʹ�ʱ��Ĳ������������Ϣ
	@Input: asdu ��ʾ������ַ,cause ����ԭ��,data �������� ֻ��ʱ��������Ϣ,len ʱ��������Ϣ���ݳ���
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int send_C_TS_TA_1_Response(int asdu,int cause,BYTE *data,const UINT &len);
	/**************************************************************************************************
	@Function: sendInitFinished
	@Description: ��ʹ������
	@Input: asdu ��ʾ������ַ
	@Output:
	@Return:
	@Update:
	@Others:Ӧ��������վ����֮����վ��ʹ�����֮�����վ���ͣ�Ȼ����վ�ŷ������������һ�����û�з��͡�����ĿǰҲ�����㷢�͡�
	**************************************************************************************************/
	int sendInitFinished(int asdu);
	/**************************************************************************************************
	@Function: SendInterrogateData
	@Description: �������ٷ�������
	@Input: v_cmd ������Ϣ��
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//int SendInterrogateData(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: sendInitFinished
	@Description: ����I��ʽ��֡��չ
	@Input: type ��Ϣ����,asdu������Ϣ��ַ,cause����ԭ��,msgbuf��Ϣ������
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int decodeMsg_I_Ex(Q_104ApduEx *msg){return 0;};
	virtual int handleCommmd_SE(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: sendInitFinished
	@Description: ����������չ
	@Calls:
	@Called By:
	@Input: asdu ��ʾ������ַ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_Ex(Q_cmdIEC104 *v_cmd){return 0;};
	/**************************************************************************************************
	@Function: handleCommmd_AC
	@Description: �仯ң��
	@Calls:
	@Called By:
	@Input: 
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_AC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_BC
	@Description: �仯ң��
	@Calls:
	@Called By:
	@Input: 
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_BC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_BC_SOE
	@Description: SOE����
	@Calls:
	@Called By:
	@Input: 
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_BC_SOE(Q_cmdIEC104 *v_cmd);

protected:
	//fs20150724
	vector<IEC104_VARIBLE_YC *> m_vecVarYC;//�仯ң�⼯��
	vector<IEC104_VARIBLE_YX *> m_vecVarYX;//�仯ң�ż���
	vector<IEC104_VARIBLE_SOE *> m_vecVarSOE;//�仯ң��SOE����
	//SYSTEMTIME m_timeSOE;//ң�ű�λʱ��SOE
	bool bZongZhaoComplete;
	
	int SendInterrogateData(Q_cmdIEC104 *v_cmd);
private:
	Q_104YtExecMode m_YtExeMode;
	Q_104YtExecMode m_YkExeMode;
	int m_nExecCmdMsgID;//��Ϣ��� ִ���ⲿ������Ϣ���
	Q_IEC104_ExecCmd m_SigArray;//104ִ�������¼

	//��һ��ң��ָ���¼����ֹ�ظ������ظ�addcommand
	BYTE last_sel_cOnOff;
	int  last_sel_nInfoAddr;
	time_t sel_timestmp;
	time_t exe_timestmp;
	BYTE last_exe_cOnOff;
	int  last_exe_nInfoAddr;

	int ACsendno;
	int BCsendno;//ң��ң��仯���ͺ�
	int SOEsendno;//SOE���ͺ�

	///////////////////////////////
	/*===================================����============================================*/
	/**************************************************************************************************
	@Function: func_Interrogate
	@Description: ������������
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_Interrogate(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_SC_NA_1
	@Description: ��������ң������
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_SC_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_DC_NA_1
	@Description: ����˫��ң������
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_DC_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_SE_NA_1
	@Description: ���� �趨���� ��һ��ֵ ������ң�� �� ң�� Ӧ��
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_SE_NA_1(Q_104ApduEx *msg);
	/*
	@Function:func_C_SE_NB_1
	@Description:�����趨��Ȼ�ֵ����
	*/
	int func_C_SE_NB_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function:  func_C_SE_NC_1
	@Description: ���� �趨���� ������ 
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_SE_NC_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_RC_NA_1
	@Description: �������ң��ʹ��
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_RC_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_CS
	@Description: ��վ������ʱ��ͬ������
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:ʱ���ʽΪCP56Time2a
	**************************************************************************************************/
	int func_C_CS(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_TS_TA_1
	@Description: ��վ����ʱ��Ĳ�������
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:ʱ���ʽΪCP56Time2a
	**************************************************************************************************/
	int func_C_TS_TA_1(Q_104ApduEx *msg);

	/*===================================����============================================*/
	/**************************************************************************************************
	@Function: handleCommmd_IC
	@Description:������������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int handleCommmd_IC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_SC
	@Description:������ң������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int handleCommmd_SC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_DC
	@Description:����˫��ң������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int handleCommmd_DC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_CS
	@Description:����ʱ��ͬ������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int handleCommmd_CS(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_RC
	@Description:���� �������ң��
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int handleCommmd_RC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_CS
	@Description:�������ݡ�У������
	@Input:recvTool ���ջ�����������
	@Output:
	@Return:�ɹ����� Q_104SUC;ʧ�� Q_104FAIL;
	@Update:
	@Others:���������ݺ󣬼�ɾ�������������
	**************************************************************************************************/
	inline bool handleAnalyseData(Q_commonBuffer *recvTool);
	/**************************************************************************************************
	@Function: SendIC_DP_Res
	@Description:�������󣬷���˫�㷵������
	@Input:,msg ��Ϣ
	vTable Ҫ�������ݵ��
	@Output:
	@Return:�ɹ����� Q_104SUC;ʧ�� Q_104FAIL;
	@Update:
	@Others:
	**************************************************************************************************/
	//int SendIC_DP_Res(Q_IEC104_ASDU *msg,vector<Q_ST_104PT *> &vTable,const int &nIdx,const int &nCount);
	/**************************************************************************************************
	@Function: SendIC_SP_Res
	@Description:�������󣬷��͵��㷵������
	@Input:nIdx ��ʼ����
	@Output:nCount���͵���Ŀ
	@Return:�ɹ����� Q_104SUC;ʧ�� Q_104FAIL;
	@Update:
	@Others:
	**************************************************************************************************/
	int SendIC_SP_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount);
	/**************************************************************************************************
	@Function: SendIC_ME_Res
	@Description:�������󣬷���ң�ⷵ������
	@Input:msg ��Ϣ vTable Ҫ�������ݵ�� ,nIdx ��ʼ����,nCount���͵���Ŀ
	@Output:
	@Return:�ɹ����� Q_104SUC;ʧ�� Q_104FAIL;
	@Update:
	@Others:
	**************************************************************************************************/
	int SendIC_ME_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount);
	int sendYtCommand(const int &asdu,const int &cause,Q_IEC104_ST_C_SE *st);
	int sendYtCommand3(const int &asdu,const int &cause,Q_IEC104_ST_C_SE3 *st);
	void initVaribleData();//��ʹ���仯��
	void deleteVarData();//ɾ���仯������

};

extern Q_dataManager *m_pMgrIEC104SlaveProtocolSetting; //fs20150724


#endif