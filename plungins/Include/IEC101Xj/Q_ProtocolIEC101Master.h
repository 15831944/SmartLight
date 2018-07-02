#if !defined(__Q_PROTOCOL_104_STANDARD_H__INCLUDED__)
#define __Q_PROTOCOL_104_STANDARD_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma warning(push)
#pragma warning(disable: 4251) 
#endif

#include "CommIEC101Def.h"
#include "CommIEC101MasterDef.h"
#include "Q_104StdDefine.h"
#include "Q_IEC101Command.h"
#include "Q_IEC101Setting.h"
#include "Q_IEC101CommandManager.h"
#include "Q_protocolIEC101.h"
#include "Q_runingUnitDefine.h"


#include "Q_IEC104CommFun.h"
#include "Q_104CP56Time2a.h"
#include "Q_104StandardTags.h"

#ifdef UNIX_ENV
//
// System time is represented with the following structure:
//
//typedef struct _SYSTEMTIME {
//	WORD wYear;
//	WORD wMonth;
//	WORD wDayOfWeek;
//	WORD wDay;
//	WORD wHour;
//	WORD wMinute;
//	WORD wSecond;
//	WORD wMilliseconds;
//} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;
#endif

/** 
* @interface Q_protocolCdt "Q_protocolIEC104Master.h"
* @brief IEC-60870-5-104 ��׼ͨѶ��Լ
* @author tanjingsheng
* @date 2012-02-06
* @see 
* @bug No know bugs so far.
*/
// __Q_COMMIEC104MASTER_EXPORT
class Q_protocolIEC104Master : public Q_protocolIEC104
{
public:
	Q_protocolIEC104Master(void);
	virtual ~Q_protocolIEC104Master(void);
protected:
	bool bStartFlg;//��ʹ������־ 

public:
	virtual bool	   terminateProtocol();
	virtual bool	   startProcess();
	virtual bool	   stopProcess();

	void SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);
	//char LastError     [Q_FRAME_CONST256];//fs

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
	int handleCommmd_AC(Q_cmdIEC104 *v_cmd){return 0;};
	int handleCommmd_BC(Q_cmdIEC104 *v_cmd){return 0;};
	int handleCommmd_BC_SOE(Q_cmdIEC104 *v_cmd){return 0;};

	void CP56Time2aToTime(unsigned char *data, SYSTEMTIME *st);

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
	@Description: ���� �趨���� ��һ��ֵ 
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_SE_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_SE_NC_1
	@Description: ���� �趨���� �̸�����ֵ 
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_SE_NC_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_SC_TA_1
	@Description: ���� ��ʱ���ң������ 
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_SC_TA_1(Q_104ApduEx *msg);
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
	/*@Function: func_M_IT_NA_1
	@Description: ���ӷ�����:�ۼ����������� 
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_IT_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_ST_NA_1
	@Description: ���ӷ�����:ң����Ϣ����
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_ST_NA_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_BO_NA_1
	@Description: ���ӷ�����:32λ���ش�
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_BO_NA_1(Q_104ApduEx *msg);
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
	/**************************************************************************************************
	@Function: func_M_ME_TF_1
	@Description: ���ӷ�����:��ʱ��Ķ̸���ң��
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_ME_TF_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_M_BO_TB_1
	@Description: ���ӷ�����:32λ���ش�
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void func_M_BO_TB_1(Q_104ApduEx *msg);
	/**************************************************************************************************
	@Function: func_C_SE_TC_1
	@Description: ���� �趨���� ��ʱ��ĸ����� 
	@Input: msg ��Ϣ
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int func_C_SE_TC_1(Q_104ApduEx *msg);
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
	///˫���ʱ��
	void func_M_DP_TB_1(Q_104ApduEx *msg);
	
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
	@Function: handleCommmd_SC
	@Description:������ң������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_SC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_DC
	@Description:����˫��ң������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_DC(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_CS
	@Description:����ʱ��ͬ������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_CS(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_SE
	@Description:�����趨�����һ��ֵ��
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_SE(Q_cmdIEC104 *v_cmd);
	/**************************************************************************************************
	@Function: handleCommmd_RC
	@Description:���� �������ң��
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/

	virtual int handleCommmd_RC(Q_cmdIEC104 *v_cmd);
	int sendYtCommand(const int &asdu,const int &cause,Q_IEC104_ST_C_SE *st);
	int sendYtCommand3(const int &asdu,const int &cause,Q_IEC104_ST_C_SE3 *st);
	int sendYkCommand(const int &asdu,const int &cause,Q_IEC104_ST_C_SC *st);
	int sendYkCommand1(const int &asdu,const int &cause,Q_IEC104_ST_C_SC_TA1 *st);//��ʱ���
	int sendYtCommandFloatWithTime(const int &asdu,const int &cause,Q_IEC104_ST_C_SE_TC1 *st);

	/**************************************************************************************************
	@Function: FindYK
	@Description: �ж�ң�ص��Ƿ����
	@Input:  cmd-����
	@Output: YKtoYXID32-ң�ض�Ӧ��ң��id YKID32-ң��id curRelay-ң�����ݽṹ
	@Return: true or false
	@Update:
	@Others:
	**************************************************************************************************/
	bool FindYK(int &YKtoYXID32, int &YKID32, SCADA_Relay &curRelay, Q_cmdIEC104_SP*cmd);

	/**************************************************************************************************
	@Function: FindYT
	@Description: �ж�ң�����Ƿ����
	@Input:  cmd-����
	@Output: YKtoYXID32-ң����Ӧ��ң��id YKID32-ң��id curRelay-ң�����ݽṹ
	@Return: true or false
	@Update:
	@Others:
	**************************************************************************************************/
    bool FindYT(int &YTtoYCID32, int &YTID32, SCADA_SetPoint &curSetPoint, Q_cmdIEC104_SE*cmd);
};

#endif