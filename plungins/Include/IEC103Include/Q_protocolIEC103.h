#if !defined(__Q_PROTOCOL_IEC103_H__INCLUDED__)
#define __Q_PROTOCOL_IEC103_H__INCLUDED__

#pragma warning(push)
#pragma warning(disable: 4251) 


#include "CommIEC103Def.h"
#include "Q_103StdDefine.h"
#include "Q_IEC103Command.h"
#include "Q_IEC103Setting.h"
#include "Q_IEC103CommandManager.h"
#include "tinyxmlinclude.h"

class Q_protocolIEC103 : public Q_protocolBase
{
public:
	Q_protocolIEC103();
	virtual ~Q_protocolIEC103();
	virtual bool explainData();
	virtual bool		makeFrame();
	virtual bool		initializeProtocol();

	CFEPFuncRtdb *g_rtdbInfIEC103;

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
	virtual Q_IEC103vfyRet verifyFrame(BYTE* msgbuf,UINT len);
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
	int this_command_flag;
	int next_command_flag;
	void setYTvalue(int device, int dotno, float value);
	void setYKstate(int device, int dotno, bool value);
protected:
	void setDefaultValue(void);
	//
	//	����I��ʽ֡ len�� apdu�ĳ��Ȳ�����ǰ�ĸ��ֽ�
	//
	int send_I_Msg();
	int send_U_Msg(int cmd);
	int send_S_Msg(void);
	void send_YT_preset(int device, int dotno, float value);
	void send_YK_preset(int device, int dotno, bool value);
	void send_YT_exe(int device);
	void send_YK_exe(int device, int dotno, bool value);
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




protected:
    int m_nThreadPeriod;//������ʱ��
	Q_IEC103Setting *m_ProSetting;
	Q_ComCpuEndian m_cpuEndian;//cpu���뷽ʽ
	Q_IEC103_Struct *m_st;
	Q_103BaseCfg *m_cfgBase;//��������
	Q_103StCfg   *m_cfgSt;//վ�ٻ�����
	Q_103CtrlCfg *m_cfgControl;//��������
	Q_IEC103CommandManager *m_listCmd;//�����б� //����Э�����ͷ�
	Q_103SyncClockCfg   *m_cfgSyncClock;//ʱ��ͬ������
	Q_103StandardTags* m_tags;//���ݱ����
	//Q_channelBase*							m_clientChannel;
	int Q_103NoDataTimes;
	int Q_103TWODataTimes;
	int sleepflag;



	char		LastError[200];         //������Ϣ

	time_t          now;                    //��ǰʱ�䣬������ʱ�����

	time_t			m_nTimerConnect;		// ʱ�����

	time_t			m_nTimerConfirm;		// ʱ����� ��ͣ��ʱ

	time_t			m_nTimer_S_Ack;		// ʱ�����,����T2 �����ݱ���ʱȷ�ϵĳ�ʱ
	bool		m_bTimer_S_Ack;

	time_t			m_nTimer_U_Test;		// ʱ�����,����T3 ���ڿ���״̬�·��Ͳ���֡�ĳ�ʱ
	bool        m_bSendTestFR;           //�Ƿ������ŵ�TestFR֡����
	bool        m_bStopSend;             //ֹͣ���� ��δȷ��I��ʽAPDU�ﵽk��ʱ�����ͷ�ֹͣ����
	Q_IEC103_ErrorCode    m_errorNo; //������
	time_t			timer_U_Test;		// associated of T3 param
	bool		timer_U_Testflag;
	bool        FCB;
	bool        FCV;
	int         yt_device;
	int         yt_dotno;
	float         yt_value;
	int         yk_device;
	int         yk_dotno;
	bool        yk_value;

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
	virtual void OnShowMsg(char *msg,unsigned int len);

	

	void writefile(char *log);

			/**************************************************************************************************
	@Function: decodeMsg_I
	@Description: ������֡
	@Input: msgbuf �������ݣ����ݴ�����ֵ��ʼ;len���ȣ���ʶ������ֵ��ʼ������������֡���ȡ�
	@Output:
	@Return:
	@Update:
	@Others: 
	**************************************************************************************************/
	int decodeImsg(BYTE *msgbuf, const UINT len);
	/**************************************************************************************************
	@Function: decodeMsg_U
	@Description: ������֡
	@Input: msgbuf �������ݣ����ݴ�����ֵ��ʼ;len���ȣ���ʶ������ֵ��ʼ������������֡���ȡ�
	@Output:
	@Return:
	@Update:
	@Others: 
	**************************************************************************************************/
//	void rec_short_msg(BYTE CODE, const UINT len);
	int decodeUmsg(BYTE CODE, const UINT len);


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
	//virtual int handleCommmd_Ex(Q_cmdIEC103 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_IC
	@Description:������������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//virtual int handleCommmd_IC(Q_cmdIEC103 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_SC
	@Description:������ң������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//virtual int handleCommmd_SC(Q_cmdIEC103 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_DC
	@Description:����˫��ң������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//virtual int handleCommmd_DC(Q_cmdIEC103 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_CS
	@Description:����ʱ��ͬ������
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//virtual int handleCommmd_CS(Q_cmdIEC103 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_SE
	@Description:�����趨�����һ��ֵ��
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//virtual int handleCommmd_SE(Q_cmdIEC103 *v_cmd)=0;
	/**************************************************************************************************
	@Function: handleCommmd_RC
	@Description:���� �������ң��
	@Input:v_cmd ����
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	//virtual int handleCommmd_RC(Q_cmdIEC103 *v_cmd)=0;
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
	//virtual int handleCommmd_AC(Q_cmdIEC103 *v_cmd)=0;
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
	//virtual int handleCommmd_BC(Q_cmdIEC103 *v_cmd)=0;
	bool	terminateProtocol(){ return true;};
	virtual bool	startProcess();
	bool	stopProcess();

	BYTE CalculateCheck( BYTE *msgbuf ,int num);

	void Rec_TCP_Data(BYTE *msgbuf);
	void Rec_All_YX(BYTE *msgbuf);
	void Rec_SOE(BYTE *msgbuf);
	void Rec_CHange_YX(BYTE *msgbuf);
	void Rec_All_YC(BYTE *msgbuf);
	void RestartCh();



};
#endif