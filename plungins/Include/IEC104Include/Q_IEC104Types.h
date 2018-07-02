
#if !defined(__Q_IEC104_TYPE_DEF_H__INCLUDED__)
#define __Q_IEC104_TYPE_DEF_H__INCLUDED__
/** 
* @interface 
* @brief IEC-60870-104 protocol define
* @author ̷��ʤ TanJingSheng
* @date 2012-2-10
* @see 
* @bug No know bugs so far.
*/

typedef struct Q_IEC104_APCI{
	BYTE start;
	BYTE len;
	BYTE field1;
	BYTE field2;
	BYTE field3;
	BYTE field4;
}Q_IEC104_APCI;

typedef struct Q_IEC104_ASDU_header{
	BYTE type;//����
	BYTE qual;//�ɱ�ṹ�޶���
	BYTE tx_cause_1;//����ԭ��1
	BYTE tx_cause_2;//����ԭ��2
	BYTE commom_asdu_1;//������ַ1
	BYTE commom_asdu_2;//����ַַ2
}Q_IEC104_ASDU_header;

typedef struct Q_IEC104_ASDU{
	Q_IEC104_ASDU_header header;
	BYTE data[243];
}Q_IEC104_ASDU;

typedef struct Q_IEC104_BIG_ASDU{
	Q_IEC104_ASDU_header header;
	BYTE data[10240];
}Q_IEC104_BIG_ASDU;

typedef struct Q_IEC104_APDU{
	Q_IEC104_APCI	apci;
	Q_IEC104_ASDU	asdu;
}Q_IEC104_APDU;
//APDU��Ϣ��չ
typedef struct Q_104ApduEx
{
	UINT type;//����
	UINT asdu;//�������ݵ�Ԫ���
	BYTE cause;//����ԭ��
	bool seq;//�Ƿ�����
	BYTE *msgbuf;//��Ϣ���� (ȥ��APCI6���ֽ���)��������ʼ
	UINT len;//���ȣ�ȥ��APCI��6���ֽڵı����ܳ��ȣ�
	UINT num;  //��Ϣ���ظ���
}Q_104ApduEx;


//����ң�� �����޶���
typedef struct Q_IEC104_ST_C_SC_SCO {
	BYTE		scs	:1; //����ң������״̬
	BYTE		res	:1; //ռ��Ϊ0
	BYTE		qu	:5; //ң������Ʒ��
	BYTE		se	:1; //ѡ��/ִ��
}Q_IEC104_ST_C_SC_SCO;

//����ң�� ���ӷ����յ�������
typedef struct Q_IEC104_ST_C_SC{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_IEC104_ST_C_SC_SCO  sco;
}Q_IEC104_ST_C_SP;
//ң��  ��ʱ��ĵ���ң��
typedef struct Q_IEC104_ST_C_SC_TA1 {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_IEC104_ST_C_SC_SCO  sco;
	BYTE CP56Time[7];
}Q_IEC104_ST_C_SC_TA1;

//˫��ң�� �����޶���
typedef struct Q_IEC104_ST_C_DC_DCO {
	BYTE		dcs	:2; //˫��ң������״̬
	BYTE		qu	:5; //ң������Ʒ��
	BYTE		se	:1; //ѡ��/ִ��
}Q_IEC104_ST_C_DC_DCO;

//˫��ң�ء����ӷ����յ�������
typedef struct Q_IEC104_ST_C_DC {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_IEC104_ST_C_SC_SCO  dco;

}Q_IEC104_ST_C_DC;

//�趨������������޶��� 
typedef struct Q_IEC104_ST_C_SE_QOS {
	BYTE		ql	:7; //�������Ʒ��
	BYTE		se	:1; //ѡ��/ִ��
}Q_IEC104_ST_C_SE_QOS;


//�趨���� ��һ��ֵ �趨�����޶��� ���ӷ����ϴ���
typedef struct Q_IEC104_ST_C_SE {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	BYTE val1;
	BYTE val2:7,
s:1;  //����λ 1Ϊ��0Ϊ����
	Q_IEC104_ST_C_SE_QOS qos;

}Q_IEC104_ST_C_SE;

//�趨���� ��ʱ��ĸ�����
typedef struct Q_IEC104_ST_C_SE_TC1 {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	BYTE val[4];//float��
	Q_IEC104_ST_C_SE_QOS qos;
	BYTE CP56Time[7];
}Q_IEC104_ST_C_SE_TC1;
//��������
typedef struct Q_IEC104_ST_C_SE3 {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	BYTE val[4];
	Q_IEC104_ST_C_SE_QOS qos;

}Q_IEC104_ST_C_SE3;

//����������������޶��� 
typedef struct Q_IEC104_ST_C_RC_RCO {
	BYTE  rcs	:2,
          qu    :5,
          se    :1;
}Q_IEC104_ST_C_RC_RCO;

//�趨���� ��һ��ֵ �趨�����޶��� ���ӷ����ϴ���
typedef struct Q_IEC104_ST_C_RC {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_IEC104_ST_C_RC_RCO rco;

}Q_IEC104_ST_C_RC;

// single point information w/quality
//��Ʒ����ʵĵ���ң����Ϣ
typedef struct Q_SIQ104{
	BYTE	spi : 1,		// off=0 / on=1
rs1 : 1,		// reserve
rs2 : 1,		// reserve
rs3 : 1,		// reserve
bl  : 1,		// not blocked=0 / blocked=1
sb  : 1,		// not substituted=0 / substituted=1
nt  : 1,		// topical=0 / not topical=1 
iv  : 1;		// valid=0 / not valid=1
}Q_104_SIQ104;

typedef struct Q_DIQ104{
	BYTE	dpi : 2,		// off=intransit / Open=1 Close=2 / Invlid=3  ��ȷ�����м�״̬<1>��=ȷ��״̬��<2>=ȷ��״̬��<3>��=��ȷ�����м�״̬
rs1 : 1,		// reserve
rs2 : 1,		// reserve
bl  : 1,		// not blocked=0 / blocked=1
sb  : 1,		// not substituted=0 / substituted=1
nt  : 1,		// topical=0 / not topical=1 
iv  : 1;		// valid=0 / not valid=1
}Q_DIQ104;

// quality descriptor
//Ʒ��������
typedef struct Q_QDS104{
	BYTE	ov	: 1,		// no overflow=0 / overflow=1
rs1 : 1,		// reserve
rs2 : 1,		// reserve
rs3 : 1,		// reserve
bl  : 1,		// not blocked=0 / blocked=1
sb  : 1,		// not substituted=0 / substituted=1
nt  : 1,		// topical=0 / not topical=1 
iv  : 1;		// valid=0 / not valid=1
}Q_QDS104;



// single point value
//����ң��ֵ
typedef struct Q_SP104{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_SIQ104	siq;
} Q_SP104;

// single point value
//˫��ң��ֵ
typedef struct Q_DP104{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;;
	Q_DIQ104	diq;
}Q_DP104;

//Measured Scaled sequence  ˳��Ĳ���ֵ�������ֵ
typedef struct Q_MS104Seq{
	BYTE	val[2]; //ֵ
	Q_QDS104	qds; //��Ʒ��������
}Q_MS104Seq;

//Measured Scaled ��Ȼ����һ��ֵң��ֵ
typedef struct Q_MS104{
	BYTE	addr0; //addr0��addr1��addr2 ��Ϣ�����ַ
	BYTE	addr1;
	BYTE	addr2;
	Q_MS104Seq seqv;
}Q_MS104;


// Measured Float sequence 
//�̸����͵� ˳�� ����ֵ��Ϣ
typedef struct Q_MF104Seq{
	unsigned char	val[4]; //ֵ
	struct Q_QDS104	qds; //��Ʒ��������
}Q_MF104Seq;


// Measured Float �̸���ң��
typedef struct Q_MF104{
	BYTE	addr0; //addr0��addr1��addr2 ��Ϣ�����ַ
	BYTE	addr1;
	BYTE	addr2;
	Q_MF104Seq seqv;
}Q_MF104;


typedef struct {
	BYTE	val[2]; //ֵ
}Q_MS104Seq_21,Q_MF104Seq_21;

typedef struct {
	BYTE	addr0; //addr0��addr1��addr2 ��Ϣ�����ַ
	BYTE	addr1;
	BYTE	addr2;
	Q_MS104Seq_21 seqv;
}Q_MS104_21,Q_MF104_21;


//ң��
typedef struct Q_M_ST104Seq{
	BYTE val;
	Q_QDS104 qds;
}Q_M_ST104_SEQ;

typedef struct Q_M_ST104{
	BYTE	addr0;
	BYTE	addr1;
	BYTE	addr2;
	Q_M_ST104Seq  mst;

}Q_M_ST104;

typedef Q_MF104Seq Q_BO104Seq;//32 ���ش� ˳���
typedef Q_MF104 Q_BO104;         //32���ش� ��˳���

//����ң�� �趨�����޶���  ST��ʾ��struct
typedef struct Q_IEC104_ST_C_CS{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	BYTE val[7];
}Q_IEC104_ST_C_CS;

//
typedef struct Q_IEC104_ST_C_TS_TA_1{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	UINT TSC1;
	UINT TSC2;
	BYTE val[7];
}Q_IEC104_ST_C_TS_TA_1;

#endif