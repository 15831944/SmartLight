#pragma once
#if !defined(__Q_IEC103_TYPE_DEF_H__INCLUDED__)
#define __Q_IEC103_TYPE_DEF_H__INCLUDED__
/** 
* @interface 
* @brief IEC-60870-104 protocol define
* @author ̷��ʤ TanJingSheng
* @date 2012-2-10
* @see 
* @bug No know bugs so far.
*/

typedef struct Q_IEC103_APCI{
	BYTE start1;
	BYTE len1;
	BYTE len2;
	BYTE start2;
	BYTE con;
	BYTE addr;
}Q_IEC103_APCI;

typedef struct Q_IEC103_ASDU_header{
	BYTE type;//����
	BYTE qual;//�ɱ�ṹ�޶���
	BYTE tx_cause;//����ԭ��
	BYTE commom_asdu_1;//������ַ
}Q_IEC103_ASDU_header;

typedef struct Q_IEC103_ASDU{
	Q_IEC103_ASDU_header header;
	BYTE data[243];
}Q_IEC103_ASDU;

typedef struct Q_IEC103_APDU{
	Q_IEC103_APCI	apci;
	Q_IEC103_ASDU	asdu;
}Q_IEC103_APDU;
//APDU��Ϣ��չ
typedef struct Q_103ApduEx
{
	UINT type;//����
	UINT asdu;//�������ݵ�Ԫ���
	BYTE cause;//����ԭ��
	bool seq;//�Ƿ�����
	BYTE *msgbuf;//��Ϣ���� (ȥ��APCI6���ֽ���)��������ʼ
	UINT len;//���ȣ�ȥ��APCI��6���ֽڵı����ܳ��ȣ�
	UINT num;  //��Ϣ���ظ���
}Q_103ApduEx;


//����ң�� �����޶���
typedef struct Q_IEC103_ST_C_SC_SCO {
	BYTE		scs	:1; //����ң������״̬
	BYTE		res	:1; //ռ��Ϊ0
	BYTE		qu	:5; //ң������Ʒ��
	BYTE		se	:1; //ѡ��/ִ��
}Q_IEC103_ST_C_SC_SCO;

//����ң�� ���ӷ����յ�������
typedef struct Q_IEC103_ST_C_SC{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_IEC103_ST_C_SC_SCO  sco;
}Q_IEC103_ST_C_SP;

//˫��ң�� �����޶���
typedef struct Q_IEC103_ST_C_DC_DCO {
	BYTE		dcs	:2; //˫��ң������״̬
	BYTE		qu	:5; //ң������Ʒ��
	BYTE		se	:1; //ѡ��/ִ��
}Q_IEC103_ST_C_DC_DCO;

//˫��ң�ء����ӷ����յ�������
typedef struct Q_IEC103_ST_C_DC {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_IEC103_ST_C_SC_SCO  dco;

}Q_IEC103_ST_C_DC;

//�趨������������޶��� 
typedef struct Q_IEC103_ST_C_SE_QOS {
	BYTE		ql	:7; //�������Ʒ��
	BYTE		se	:1; //ѡ��/ִ��
}Q_IEC103_ST_C_SE_QOS;


//�趨���� ��һ��ֵ �趨�����޶��� ���ӷ����ϴ���
typedef struct Q_IEC103_ST_C_SE {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	BYTE val1;
	BYTE val2:7,
s:1;  //����λ 1Ϊ��0Ϊ����
	Q_IEC103_ST_C_SE_QOS qos;

}Q_IEC103_ST_C_SE;
//��������
typedef struct Q_IEC103_ST_C_SE3 {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;BYTE val[4];
	Q_IEC103_ST_C_SE_QOS qos;

}Q_IEC103_ST_C_SE3;

//����������������޶��� 
typedef struct Q_IEC103_ST_C_RC_RCO {
	BYTE  rcs	:2,
          qu    :5,
          se    :1;
}Q_IEC103_ST_C_RC_RCO;

//�趨���� ��һ��ֵ �趨�����޶��� ���ӷ����ϴ���
typedef struct Q_IEC103_ST_C_RC {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_IEC103_ST_C_RC_RCO rco;

}Q_IEC103_ST_C_RC;

// single point information w/quality
//��Ʒ����ʵĵ���ң����Ϣ
typedef struct Q_SIQ103{
	BYTE	spi : 1,		// off=0 / on=1
rs1 : 1,		// reserve
rs2 : 1,		// reserve
rs3 : 1,		// reserve
bl  : 1,		// not blocked=0 / blocked=1
sb  : 1,		// not substituted=0 / substituted=1
nt  : 1,		// topical=0 / not topical=1 
iv  : 1;		// valid=0 / not valid=1
}Q_103_SIQ103;

typedef struct Q_DIQ103{
	BYTE	dpi : 2,		// off=intransit / Open=1 Close=2 / Invlid=3  ��ȷ�����м�״̬<1>��=ȷ��״̬��<2>=ȷ��״̬��<3>��=��ȷ�����м�״̬
rs1 : 1,		// reserve
rs2 : 1,		// reserve
bl  : 1,		// not blocked=0 / blocked=1
sb  : 1,		// not substituted=0 / substituted=1
nt  : 1,		// topical=0 / not topical=1 
iv  : 1;		// valid=0 / not valid=1
}Q_DIQ103;

// quality descriptor
//Ʒ��������
typedef struct Q_QDS103{
	BYTE	ov	: 1,		// no overflow=0 / overflow=1
rs1 : 1,		// reserve
rs2 : 1,		// reserve
rs3 : 1,		// reserve
bl  : 1,		// not blocked=0 / blocked=1
sb  : 1,		// not substituted=0 / substituted=1
nt  : 1,		// topical=0 / not topical=1 
iv  : 1;		// valid=0 / not valid=1
}Q_QDS103;



// single point value
//����ң��ֵ
typedef struct Q_SP103{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_SIQ103	siq;
} Q_SP103;

// single point value
//˫��ң��ֵ
typedef struct Q_DP103{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;;
	Q_DIQ103	diq;
}Q_DP103;

//Measured Scaled sequence  ˳��Ĳ���ֵ�������ֵ
typedef struct Q_MS103Seq{
	BYTE	val[2]; //ֵ
	Q_QDS103	qds; //��Ʒ��������
}Q_MS103Seq;

//Measured Scaled ��Ȼ����һ��ֵң��ֵ
typedef struct Q_MS103{
	BYTE	addr0; //addr0��addr1��addr2 ��Ϣ�����ַ
	BYTE	addr1;
	BYTE	addr2;
	Q_MS103Seq seqv;
}Q_MS103;


// Measured Float sequence 
//�̸����͵� ˳�� ����ֵ��Ϣ
typedef struct Q_MF103Seq{
	unsigned char	val[4]; //ֵ
	struct Q_QDS103	qds; //��Ʒ��������
}Q_MF103Seq;


// Measured Float �̸���ң��
typedef struct Q_MF103{
	BYTE	addr0; //addr0��addr1��addr2 ��Ϣ�����ַ
	BYTE	addr1;
	BYTE	addr2;
	Q_MF103Seq seqv;
}Q_MF103;


typedef struct {
	BYTE	val[2]; //ֵ
}Q_MS103Seq_21,Q_MF103Seq_21;

typedef struct {
	BYTE	addr0; //addr0��addr1��addr2 ��Ϣ�����ַ
	BYTE	addr1;
	BYTE	addr2;
	Q_MS103Seq_21 seqv;
}Q_MS103_21,Q_MF103_21;


//ң��
typedef struct Q_M_ST103Seq{
	BYTE val;
	Q_QDS103 qds;
}Q_M_ST103_SEQ;

typedef struct Q_M_ST103{
	BYTE	addr0;
	BYTE	addr1;
	BYTE	addr2;
	Q_M_ST103Seq  mst;

}Q_M_ST103;

typedef Q_MF103Seq Q_BO103Seq;//32 ���ش� ˳���
typedef Q_MF103 Q_BO103;         //32���ش� ��˳���

//����ң�� �趨�����޶���  ST��ʾ��struct
typedef struct Q_IEC103_ST_C_CS{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	BYTE val[7];
}Q_IEC103_ST_C_CS;

typedef struct Q_IEC103_ASDU_COMMON_WRITE{
	BYTE fun;
	BYTE inf;
	BYTE rii;
	BYTE ngd;
	BYTE gin_group;
	BYTE gin_no;
	BYTE kod;
	BYTE gdd_type;
	BYTE gdd_datawidth;
	BYTE gdd_count;
}Q_IEC103_ASDU_COMMON_WRITE;
#endif