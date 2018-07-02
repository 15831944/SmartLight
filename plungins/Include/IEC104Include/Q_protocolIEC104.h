#if !defined(__Q_PROTOCOL_IEC104_H__INCLUDED__)
#define __Q_PROTOCOL_IEC104_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma warning(push)
#pragma warning(disable: 4251) 
#endif

#include "Q_frameInclude.h"

#include "CommIEC104Def.h"
#include "Q_104StdDefine.h"
#include "Q_IEC104Command.h"
#include "Q_IEC104Setting.h"
#include "Q_IEC104CommandManager.h"
#include "gSystemDef.h"


class __Q_COMMIEC104_EXPORT Q_protocolIEC104 : public Q_protocolBase
{
public:
	Q_protocolIEC104();
	virtual ~Q_protocolIEC104();
	virtual bool explainData();
	virtual bool		makeFrame();
	virtual bool		initializeProtocol();
	virtual int decodeMsg_I(BYTE *msgbuf, const UINT len)=0;
	virtual int decodeMsg_U(BYTE *msgbuf, const UINT len)=0;
	virtual void SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen);

	CFEPFuncRtdb *g_rtdbInfIEC104;

	//����������Ӧ��Ϣ
	///��λ����
	void setThreadPeriod(int nThreadPeriod){m_nThreadPeriod=nThreadPeriod;}
	/**************************************************************************************************
	@Function: verifyFrame
	@Description: У��֡�ṹ��ͬʱ����֡�ṹ����
	@Input: msgbuf �������ݣ����ݴ�����ֵ��ʼ;len���ȣ���ʶ������ֵ��ʼ������������֡���ȡ�
	@Output:
	@Return:Q_IEC104vfyRet �ṹ st . ��������m_bSucΪfalse��ͬʱm_nErrorCode��Ϊ��0��ֵ
	У��ɹ� m_bSucΪtrue,m_eFrameTypeΪ��Ӧ��֡����
	@Update:
	@Others: 
	//(1)I֡ʱУ�鹫������Ԫ��ַ
	//(2)�����Ƿ񳬳����С
	//(3)�����е����ݸ����Ƿ����յ�����Ŀ���
	//(4) I��ʽ��֡Ӧ����ʼ���ȴ���12
	//(5) ��ֹ���Ķ�ʧ�ͱ����ظ����ʹ���,�ѷ��͵����к����ѽ��յ����к�У�顣
	**************************************************************************************************/
	virtual Q_IEC104vfyRet verifyFrame(BYTE* msgbuf,UINT len);
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
	@Function: decodeMsg_U
	@Description:����socket�Ƿ�Ͽ���
	@Input: 
	@Output:
	@Return:
	@Update:
	@Others:ֻҪm_eProtocolStatus��ֵΪeCommonStatusError�ͱ�ʾ��socket��·�Ͽ�
	**************************************************************************************************/
	bool getSockConnectStatus();
	////
	//�ر���·
	////
	void closeLink(void);
protected:
	void setDefaultValue(void);
	//
	//	����I��ʽ֡ len�� apdu�ĳ��Ȳ�����ǰ�ĸ��ֽ�
	//
	virtual int send_I_Msg(BYTE *msgbuf, UINT len);
	int send_U_Msg(int cmd);
	int send_S_Msg(void);
	/**************************************************************************************************
	@Function: mirror_I_Msg
	@Description: ������I֡����
	@Input: msgbuf �������ݣ����ݴ�����ֵ��ʼ;len���ȣ���ʶ������ֵ��ʼ������������֡���ȡ�
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	int mirror_I_Msg(BYTE *msgbuf,const UINT &len);
	//
	//	��ʼ����
	//
	int sendStartDT(void);
	//
	//	��ֹ����
	//
	int sendStopDT(void);

	//
	//	����֡����
	//
	int sendTestFR(void);
public:
	char		LastError[Q_FRAME_CONST256];         //������Ϣ
protected:
    int m_nThreadPeriod;//������ʱ��
	Q_IEC104Setting *m_ProSetting;
	Q_ComCpuEndian m_cpuEndian;//cpu���뷽ʽ
	Q_IEC104_Struct *m_st;
	Q_104BaseCfg *m_cfgBase;//��������
	Q_104StCfg   *m_cfgSt;//վ�ٻ�����
	Q_104CtrlCfg *m_cfgControl;//��������
	Q_IEC104CommandManager *m_listCmd;//�����б� //����Э�����ͷ�
	Q_104SyncClockCfg   *m_cfgSyncClock;//ʱ��ͬ������
	Q_104StandardTags* m_tags;//���ݱ����

	

	time_t          now;                    //��ǰʱ�䣬������ʱ�����

	time_t			m_nTimerConnect;		// ʱ�����,����T0 ����վ����������������ڼ��ʱ��

	time_t			m_nTimerConfirm;		// ʱ�����,����T1 ���ͻ����APDU�ĳ�ʱ 

	time_t			m_nTimer_S_Ack;		// ʱ�����,����T2 �����ݱ���ʱȷ�ϵĳ�ʱ
	bool		m_bTimer_S_Ack;

	time_t			m_nTimer_U_Test;		// ʱ�����,����T3 ���ڿ���״̬�·��Ͳ���֡�ĳ�ʱ
	bool        m_bSendTestFR;           //�Ƿ������ŵ�TestFR֡����
	bool        m_bStopSend;             //ֹͣ���� ��δȷ��I��ʽAPDU�ﵽk��ʱ�����ͷ�ֹͣ����
	Q_IEC104_ErrorCode    m_errorNo; //������
	time_t			timer_U_Test;		// associated of T3 param
	bool		timer_U_Testflag;
	time_t      timer_ICTimeflag;

protected:
	/**************************************************************************************************
	@Function: onTxMsg
	@Description:����������ʱ���÷��鷽��
	@Input: msgbuf �������ݣ����ݴ�����ֵ��ʼ;len���ȣ���ʶ������ֵ��ʼ������������֡���ȡ�
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual void onTxMsg(BYTE *msgbuf, unsigned int len);
	/**************************************************************************************************
	@Function: onTxMsg
	@Description:�����յ�����ʱ���÷��鷽��
	@Input: msgbuf �������ݣ����ݴ�����ֵ��ʼ;len���ȣ���ʶ������ֵ��ʼ������������֡���ȡ�
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual void onRxMsg(BYTE *msgbuf, unsigned int len);
	virtual void OnError(int errorCode,int lineNum);
	virtual void OnErrorMsg(char *msg,unsigned int len);
	//��ӡ��Լ��ʾ��Ϣ��һ������ң����ң�ش�ӡ
	virtual void OnShowMsg(char *msg,unsigned int len);
	//��ӡ��Լ�����Ϣ��һ������ң���ӡ  tjs 2013-11-17
	virtual void OnShowExplainMsg(char *msg,unsigned int len);
	void writefile(char *log);


	/*===================================����============================================*/
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
	virtual int handleCommmd_Ex(Q_cmdIEC104 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_IC
	@Description:������������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_IC(Q_cmdIEC104 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_SC
	@Description:������ң������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_SC(Q_cmdIEC104 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_DC
	@Description:����˫��ң������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_DC(Q_cmdIEC104 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_CS
	@Description:����ʱ��ͬ������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_CS(Q_cmdIEC104 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_SE
	@Description:�����趨�����һ��ֵ��
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_SE(Q_cmdIEC104 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_RC
	@Description:���� �������ң��
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual int handleCommmd_RC(Q_cmdIEC104 *v_cmd)=0;
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
	virtual int handleCommmd_AC(Q_cmdIEC104 *v_cmd)=0;
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
	virtual int handleCommmd_BC(Q_cmdIEC104 *v_cmd)=0;
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
	virtual int handleCommmd_BC_SOE(Q_cmdIEC104 *v_cmd)=0;
};
#endif