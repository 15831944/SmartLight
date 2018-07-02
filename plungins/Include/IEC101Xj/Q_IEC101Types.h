
#if !defined(__Q_IEC104_TYPE_DEF_H__INCLUDED__)
#define __Q_IEC104_TYPE_DEF_H__INCLUDED__
/** 
* @interface 
* @brief IEC-60870-101 protocol define
* @author ̷��ʤ TanJingSheng
* @date 2012-2-10
* @see 
* @bug No know bugs so far.
*/
#include <time.h>

typedef struct Q_IEC104_APCI{
	BYTE start;
	BYTE len;
	BYTE len2;
	BYTE start2;
	BYTE control;
	BYTE addr1[2];
}Q_IEC104_APCI;
//short frame 
/*
�����ַ���10H��
������C��
��·��ַ��A��
֡У��ͣ�CS��
�����ַ���16H��

*/

//������
typedef struct Q_CtrlZone
{
	BYTE	fun : 4;
	BYTE	fcv : 1;
	BYTE	fcb : 1;
	BYTE	prm : 1;
	BYTE	dir : 1;
}Q_CtrlZone;

typedef struct Q_IEC104_SF
{
	BYTE start;
	Q_CtrlZone ctrl;
	BYTE addr;
	BYTE cs;
	BYTE end;
}Q_IEC104_SF;


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
typedef struct Q_SIQ101{
	BYTE	spi : 1,		// off=0 / on=1
rs1 : 1,		// reserve
rs2 : 1,		// reserve
rs3 : 1,		// reserve
bl  : 1,		// not blocked=0 / blocked=1
sb  : 1,		// not substituted=0 / substituted=1
nt  : 1,		// topical=0 / not topical=1 
iv  : 1;		// valid=0 / not valid=1
}Q_104_SIQ101;

typedef struct Q_DIQ101{
	BYTE	dpi : 2,		// off=intransit / Open=1 Close=2 / Invlid=3  ��ȷ�����м�״̬<1>��=ȷ��״̬��<2>=ȷ��״̬��<3>��=��ȷ�����м�״̬
rs1 : 1,		// reserve
rs2 : 1,		// reserve
bl  : 1,		// not blocked=0 / blocked=1
sb  : 1,		// not substituted=0 / substituted=1
nt  : 1,		// topical=0 / not topical=1 
iv  : 1;		// valid=0 / not valid=1
}Q_DIQ101;

// quality descriptor
//Ʒ��������
typedef struct Q_QDS101{
	BYTE	ov	: 1,		// no overflow=0 / overflow=1
rs1 : 1,		// reserve
rs2 : 1,		// reserve
rs3 : 1,		// reserve
bl  : 1,		// not blocked=0 / blocked=1
sb  : 1,		// not substituted=0 / substituted=1
nt  : 1,		// topical=0 / not topical=1 
iv  : 1;		// valid=0 / not valid=1
}Q_QDS101;



// single point value
//����ң��ֵ
typedef struct Q_SP101{
	BYTE addr0;
	BYTE addr1;
	Q_SIQ101	siq;
} Q_SP101;

// single point value
//˫��ң��ֵ
typedef struct Q_DP101{
	BYTE addr0;
	BYTE addr1;

	Q_DIQ101	diq;
}Q_DP101;

//Measured Scaled sequence  ˳��Ĳ���ֵ�������ֵ
typedef struct Q_MS101Seq{
	BYTE	val[2]; //ֵ
	Q_QDS101	qds; //��Ʒ��������
}Q_MS101Seq;

//Measured Scaled ��Ȼ����һ��ֵң��ֵ
typedef struct Q_MS101{
	BYTE	addr0; //addr0��addr1��addr2 ��Ϣ�����ַ
	BYTE	addr1;

	Q_MS101Seq seqv;
}Q_MS101;


// Measured Float sequence 
//�̸����͵� ˳�� ����ֵ��Ϣ
typedef struct Q_MF101Seq{
	unsigned char	val[4]; //ֵ
	struct Q_QDS101	qds; //��Ʒ��������
}Q_MF101Seq;


// Measured Float �̸���ң��
typedef struct Q_MF101{
	BYTE	addr0; //addr0��addr1��addr2 ��Ϣ�����ַ
	BYTE	addr1;

	Q_MF101Seq seqv;
}Q_MF101;


typedef struct {
	BYTE	val[2]; //ֵ
}Q_MS101Seq_21,Q_MF101Seq_21;

typedef struct {
	BYTE	addr0; //addr0��addr1��addr2 ��Ϣ�����ַ
	BYTE	addr1;

	Q_MS101Seq_21 seqv;
}Q_MS101_21,Q_MF101_21;


//ң��
typedef struct Q_M_ST101Seq{
	BYTE val;
	Q_QDS101 qds;
}Q_M_ST101_SEQ;

typedef struct Q_M_ST101{
	BYTE	addr0;
	BYTE	addr1;
	BYTE	addr2;
	Q_M_ST101Seq  mst;

}Q_M_ST101;

typedef Q_MF101Seq Q_BO101Seq;//32 ���ش� ˳���
typedef Q_MF101 Q_BO101;         //32���ش� ��˳���

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

//����֡
typedef struct Q_IEC104_TEST_FR{
	BYTE	addr0; 
	BYTE	addr1;
	BYTE data1;
	BYTE data2;
}Q_IEC104_TEST_FR;

#endif