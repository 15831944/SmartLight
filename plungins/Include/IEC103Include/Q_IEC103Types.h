#pragma once
#if !defined(__Q_IEC103_TYPE_DEF_H__INCLUDED__)
#define __Q_IEC103_TYPE_DEF_H__INCLUDED__
/** 
* @interface 
* @brief IEC-60870-104 protocol define
* @author 谭井胜 TanJingSheng
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
	BYTE type;//类型
	BYTE qual;//可变结构限定词
	BYTE tx_cause;//传送原因
	BYTE commom_asdu_1;//公共地址
}Q_IEC103_ASDU_header;

typedef struct Q_IEC103_ASDU{
	Q_IEC103_ASDU_header header;
	BYTE data[243];
}Q_IEC103_ASDU;

typedef struct Q_IEC103_APDU{
	Q_IEC103_APCI	apci;
	Q_IEC103_ASDU	asdu;
}Q_IEC103_APDU;
//APDU信息扩展
typedef struct Q_103ApduEx
{
	UINT type;//类型
	UINT asdu;//公共数据单元编号
	BYTE cause;//传送原因
	bool seq;//是否有序
	BYTE *msgbuf;//信息内容 (去掉APCI6个字节域)从类型域开始
	UINT len;//长度（去掉APCI的6个字节的报文总长度）
	UINT num;  //信息无素个数
}Q_103ApduEx;


//单点遥控 命令限定词
typedef struct Q_IEC103_ST_C_SC_SCO {
	BYTE		scs	:1; //单点遥控命令状态
	BYTE		res	:1; //占用为0
	BYTE		qu	:5; //遥控命令品质
	BYTE		se	:1; //选择/执行
}Q_IEC103_ST_C_SC_SCO;

//单点遥控 监视方向收到的数据
typedef struct Q_IEC103_ST_C_SC{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_IEC103_ST_C_SC_SCO  sco;
}Q_IEC103_ST_C_SP;

//双点遥控 命令限定词
typedef struct Q_IEC103_ST_C_DC_DCO {
	BYTE		dcs	:2; //双点遥控命令状态
	BYTE		qu	:5; //遥控命令品质
	BYTE		se	:1; //选择/执行
}Q_IEC103_ST_C_DC_DCO;

//双点遥控　监视方向收到的数据
typedef struct Q_IEC103_ST_C_DC {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_IEC103_ST_C_SC_SCO  dco;

}Q_IEC103_ST_C_DC;

//设定命令　　　命令限定词 
typedef struct Q_IEC103_ST_C_SE_QOS {
	BYTE		ql	:7; //设点命令品质
	BYTE		se	:1; //选择/执行
}Q_IEC103_ST_C_SE_QOS;


//设定命令 规一化值 设定命令限定词 临视方向上处理
typedef struct Q_IEC103_ST_C_SE {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	BYTE val1;
	BYTE val2:7,
s:1;  //符号位 1为负0为正　
	Q_IEC103_ST_C_SE_QOS qos;

}Q_IEC103_ST_C_SE;
//浮点类型
typedef struct Q_IEC103_ST_C_SE3 {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;BYTE val[4];
	Q_IEC103_ST_C_SE_QOS qos;

}Q_IEC103_ST_C_SE3;

//升降命令　　　命令限定词 
typedef struct Q_IEC103_ST_C_RC_RCO {
	BYTE  rcs	:2,
          qu    :5,
          se    :1;
}Q_IEC103_ST_C_RC_RCO;

//设定命令 规一化值 设定命令限定词 临视方向上处理
typedef struct Q_IEC103_ST_C_RC {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_IEC103_ST_C_RC_RCO rco;

}Q_IEC103_ST_C_RC;

// single point information w/quality
//带品质描词的单点遥信信息
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
	BYTE	dpi : 2,		// off=intransit / Open=1 Close=2 / Invlid=3  不确定或中间状态<1>：=确定状态开<2>=确定状态合<3>：=不确定或中间状态
rs1 : 1,		// reserve
rs2 : 1,		// reserve
bl  : 1,		// not blocked=0 / blocked=1
sb  : 1,		// not substituted=0 / substituted=1
nt  : 1,		// topical=0 / not topical=1 
iv  : 1;		// valid=0 / not valid=1
}Q_DIQ103;

// quality descriptor
//品质描述符
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
//单点遥信值
typedef struct Q_SP103{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_SIQ103	siq;
} Q_SP103;

// single point value
//双点遥信值
typedef struct Q_DP103{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;;
	Q_DIQ103	diq;
}Q_DP103;

//Measured Scaled sequence  顺序的测量值，标度量值
typedef struct Q_MS103Seq{
	BYTE	val[2]; //值
	Q_QDS103	qds; //带品质描述词
}Q_MS103Seq;

//Measured Scaled 标度化或规一化值遥测值
typedef struct Q_MS103{
	BYTE	addr0; //addr0、addr1、addr2 信息对象地址
	BYTE	addr1;
	BYTE	addr2;
	Q_MS103Seq seqv;
}Q_MS103;


// Measured Float sequence 
//短浮点型的 顺序 测量值信息
typedef struct Q_MF103Seq{
	unsigned char	val[4]; //值
	struct Q_QDS103	qds; //带品质描述词
}Q_MF103Seq;


// Measured Float 短浮点遥测
typedef struct Q_MF103{
	BYTE	addr0; //addr0、addr1、addr2 信息对象地址
	BYTE	addr1;
	BYTE	addr2;
	Q_MF103Seq seqv;
}Q_MF103;


typedef struct {
	BYTE	val[2]; //值
}Q_MS103Seq_21,Q_MF103Seq_21;

typedef struct {
	BYTE	addr0; //addr0、addr1、addr2 信息对象地址
	BYTE	addr1;
	BYTE	addr2;
	Q_MS103Seq_21 seqv;
}Q_MS103_21,Q_MF103_21;


//遥步
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

typedef Q_MF103Seq Q_BO103Seq;//32 比特串 顺序的
typedef Q_MF103 Q_BO103;         //32比特串 非顺序的

//单点遥控 设定命令限定词  ST表示是struct
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