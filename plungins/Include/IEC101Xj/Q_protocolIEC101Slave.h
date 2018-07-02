#if !defined(__Q_PROTOCOL_104_SLAVE_H__INCLUDED__)
#define __Q_PROTOCOL_104_SLAVE_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma warning(push)
#pragma warning(disable: 4251) 
#endif
#include "Q_protocolIEC101.h"
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
	int SendInterrogateData(Q_cmdIEC104 *v_cmd);
protected:
	bool bStartFlg;//��ʹ������־ 	
	int ACsendno;
	int BCsendno;//ң��ң��仯���ͺ�
	int SOEsendno;//SOE���ͺ�
	Q_IEC104CmdState m_IC_State;//����״̬

	vector<IEC104_VARIBLE_YC *> m_vecVarYC;//�仯ң�⼯��
	vector<IEC104_VARIBLE_YX *> m_vecVarYX;//�仯ң�ż���
	vector<IEC104_VARIBLE_SOE *> m_vecVarSOE;//�仯ң��SOE����
	//SYSTEMTIME m_timeSOE;//ң�ű�λʱ��SOE
	bool bZongZhaoComplete;
	/**************************************************************************************************
	@Function: decodeMsg_I
	@Description:����I��ʽ֡
	@Input: msgbuf �������ݣ����ݴ�����ֵ��ʼ;len���ȣ���ʶ������ֵ��ʼ������������֡���ȡ�
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int decodeMsg_I(BYTE *msgbuf, const UINT len);
	/**************************************************************************************************
	@Function: decodeMsg_U
	@Description:����U��ʽ��֡
	@Input: msgbuf �������ݣ����ݴ�����ֵ��ʼ;len���ȣ���ʶ������ֵ��ʼ������������֡���ȡ�
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int decodeMsg_U(BYTE *msgbuf, const UINT len);
	int handleCommmd_AC(Q_cmdIEC104 *v_cmd);
	int handleCommmd_BC(Q_cmdIEC104 *v_cmd);
	int handleCommmd_BC_SOE(Q_cmdIEC104 *v_cmd);

	void CP56Time2aToTime(unsigned char *data, SYSTEMTIME *st);
	int SendIC_SP_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount);
	int SendIC_ME_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount);
	//˫��ң���ϴ�
	int SendIC_DP_Res(Q_IEC104_ASDU *msg,const int &nIdx,const int &nCount);
protected:
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
private:
	int send_IC_Response(const int &asdu,const int &cause);
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
	@Function: func_M_SP_NA_1
	@Description: ���ӷ����ϣ���������ң���ϴ�
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_SP_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_DP_NA_1
	@Description: ���ӷ����ϣ�˫��ң�ű�λ
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_DP_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_ME_NA_NB_1
	@Description: ���ӷ����ϣ���Ʒ�������Ĺ�һ��ֵ���Ȼ�ֵ
	@Input: msg ��Ϣ;scaled �Ƿ��Ǳ�Ȼ�ֵ��true��Ȼ�ֵ��falseΪ��Ȼ�ֵ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_ME_NA_NB_1(Q_104ApduEx *msg,const bool &scaled);
	/**************************************************************************************************
	@Function: func_M_ME_NC_1
	@Description:���ӷ����ϣ� �̸�����
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_ME_NC_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_ME_ND_1
	@Description:���ӷ����ϣ� ������
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_ME_ND_1(Q_104ApduEx *msg);//jwkadd
	
	/**************************************************************************************************
	@Function: func_M_SP_TB_1
	@Description: ���ӷ�����:��ʱ��ĵ���ң��
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_SP_TB_1(Q_104ApduEx *msg);
	void func_M_DP_TB_1(Q_104ApduEx *msg);
	void func_M_ME_TF_1(Q_104ApduEx *msg);
		/**************************************************************************************************
	@Function: time_to_IC()
	@Description: �ж��Ƿ�����ʱ��
	@Input: 
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	bool time_to_IC();
	//��������֡
	int func_TestFR(Q_104ApduEx *msg);
protected:
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
	virtual int handleCommmd_IC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_RC
	@Description:���� �������ң��
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void initVaribleData();//��ʹ���仯��
	void deleteVarData();//ɾ���仯������

	//���ͳ�ʹ����������
	int send_InitFinished(const int &asdu, const int &cause);

};

//extern Q_dataManager *m_pMgrIEC104SlaveProtocolSetting; //fs20150724


#endif