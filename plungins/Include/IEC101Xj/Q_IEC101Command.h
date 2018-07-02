#ifndef __Q_IEC104_Command_H_
#define  __Q_IEC104_Command_H_

#include "Q_frameInclude.h"

class   Q_IEC104_Struct
{
public:
	int			nRxCounter;//��ǰ���յ���֡���
	int			nLastAckTx;//���ȷ���Ѿ����͵�֡��   //��������������������˺ܶ�֡�����Ƕ���ʧ�˻����������������û���յ�ȷ�ϡ�
	int			nTxCounter;//��ǰ���͵�֡���
	int			nLastAckRx; //�����յ���ȷ��֡��
	int         nCurW;//��ǰWֵ
	int         nW;//Wֵ
	int         nK;//Kֵ
	Q_IEC104_Struct(void)
	{
		setDefault();
	}
	void setDefault(void)
	{
		nRxCounter=0;
		nLastAckTx=0;
		nTxCounter=0;
		nLastAckRx=0;
		nW=0;
		nK=0;
		nCurW=0;
	}
};
/*===============�����================================*/
class   Q_IEC104StandardCommand:public Q_protocolCommandBase
{   
public:
	Q_IEC104StandardCommand(){m_bError=false;m_nMaxTryCount=30;m_nCurTryNum=0;}
	inline void				  setCommand104Type(Q_IEC104CmdType eType) { m_eType = eType;}
	inline Q_IEC104CmdType    getCommand104Type() { return m_eType;}
	bool m_bError;//�Ƿ����
	bool isBeyondTestNum(){return m_nCurTryNum>m_nMaxTryCount?true:false;}
	void addTestNum(){m_nCurTryNum++;}
	void setTestNumDefault(){m_nCurTryNum=0;}
	void setMaxTestNum(int n_max){m_nMaxTryCount=n_max;}
private:
	int                         m_nMaxTryCount;//���������
	int                         m_nCurTryNum;//��ǰʵ����
	Q_IEC104CmdType m_eType;//��������
	
};
//104�������
class   Q_cmdIEC104:public Q_IEC104StandardCommand
{
public:
	Q_IEC104CmdState m_eState;//����״̬
	WORD m_nTxCause;//����ԭ��
	int  m_nComAsdu;//������ַ
	int  m_nInfoAddr;//��Ϣ�����ַ
	bool m_bExec;//��ʶ�������Ƿ���,true��ʾ�������,false��ʾδ����
	int  m_nPointNo;//���
	int                         m_nMaxTryCount;//���������
	int                         m_nCurTryNum;//��ǰʵ����
public:
	Q_cmdIEC104(void)
	{
		setCommonType(eCommandTypeOnce);
		setCommand104Type(eCmdTypeC_Null);
		m_eState=eCmdStateNull;
		m_nTxCause=0;
		m_nPointNo=0;
		m_nComAsdu=0;
		m_nInfoAddr=0;
		m_bExec=true;
		m_bError=false;
		m_nMaxTryCount=5;
		m_nCurTryNum=0;


	}
	virtual ~Q_cmdIEC104(void){};
};

typedef list<Q_IEC104StandardCommand *>::iterator IT_CmdListP;


//����ң������
class   Q_cmdIEC104_SP:public Q_cmdIEC104
{
public:
	BYTE m_cOnOff;// 0 �Ͽ���1���� 2 ������
	BYTE m_cQuality; //����
	BYTE m_nValue;//��¼ԭֵ
	Q_cmdIEC104_SP(void):Q_cmdIEC104()
	{
		setCommand104Type(eCmdTypeC_SC);
		m_cOnOff=2;
		m_cQuality=0;
		m_nValue=0;
	}
};

//˫��ң������
class   Q_cmdIEC104_DP:public Q_cmdIEC104
{
public:
	BYTE m_cOnOff;// 0 �Ͽ���1���� 2 ������
	BYTE m_cQuality; //����
	BYTE m_nValue;//��¼ԭֵ
	Q_cmdIEC104_DP(void):Q_cmdIEC104()
	{
		setCommand104Type(eCmdTypeC_SC);
		m_cOnOff=0;
		m_cQuality=0;
		m_nValue=0;
	}
};

//�趨����
class   Q_cmdIEC104_SE:public Q_cmdIEC104
{
public:
	short m_cOnOff;// �趨��ֵ
	float m_fVal;//����Ūʹ��
	BYTE m_cQuality; //����
	BYTE m_nValue[3];//��¼ԭʼֵ
	BYTE m_nExecFlg;//ִ�б�־����
	BYTE m_cFuncCode;//��������ʾ�����ǹ�һ������Ȼ����̸����� 0x30,0x31,0x32
	Q_cmdIEC104_SE(void):Q_cmdIEC104()
	{
		setCommand104Type(eCmdTypeC_SE);
		m_cOnOff=0;
		m_cQuality=0;
		m_nExecFlg=0;
		m_fVal=0.0f;
		m_cFuncCode=0x30;//Ĭ��Ϊ��һ��ֵ
	}
};

//��������
class   Q_cmdIEC104_RC:public Q_cmdIEC104
{
public:
	BYTE m_cOnOff;// �趨��ֵ ������1  ����һ����2  ����һ����3  ��������
	BYTE m_cQuality; //���� ��δӦ�ã�ȱʡΪ0
	BYTE m_nValue;//��¼ԭʼֵ
	Q_cmdIEC104_RC(void):Q_cmdIEC104()
	{
		setCommand104Type(eCmdTypeC_RC);
		m_cOnOff=0;
		m_cQuality=0;
	}
};

//��������
class   Q_cmdIEC104_IC:public Q_cmdIEC104
{
public:
	BYTE m_cQuality; //�ٻ�Ʒ��������
	bool m_bflgSendDigital;//����ң���Ƿ����
	bool m_bflgSendAnlog;//����ң���Ƿ����
	int m_nDigitalPos;//ң�ŷ���λ��
	int m_nAnalogPos;//ң�ⷢ��λ��
	Q_cmdIEC104_IC():Q_cmdIEC104()
	{
		setCommand104Type(eCmdTypeC_IC);
		m_cQuality=0;
		m_bflgSendDigital=false;
		m_bflgSendAnlog=false;
		m_nDigitalPos=0;
		m_nAnalogPos=0;
	}
};
//ʱ��ͬ������
class   Q_cmdIEC104_CS:public Q_cmdIEC104
{
public:
	BYTE m_cTime[7]; //ʱ��
	Q_cmdIEC104_CS(void):Q_cmdIEC104()
	{
		setCommand104Type(eCmdTypeC_CS);
	}
};
class   Q_cmdIEC104_AC:public Q_cmdIEC104
{
public:
	bool m_cOnOff;// true:���� false:������
	bool m_cSendSOE;
	int m_nTime;  // ���ͼ��
	int asdu_addr;
	int ycbegin;
	int yxbegin;
	time_t timeflag;
	Q_cmdIEC104_AC(void):Q_cmdIEC104()
	{
		setCommand104Type(eCmdTypeAnalogchange);
		m_cOnOff = 1;
		m_cSendSOE = false;
		asdu_addr = 1;
		m_nTime = 300;
		ycbegin = 1;
		yxbegin = 1;
		timeflag = time(NULL);
	}
};
#endif