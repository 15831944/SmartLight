
#if !defined(__Q_IEC104_TYPE_DEF_H__INCLUDED__)
#define __Q_IEC104_TYPE_DEF_H__INCLUDED__
/** 
* @interface 
* @brief IEC-60870-104 protocol define
* @author 谭井胜 TanJingSheng
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
	BYTE type;//类型
	BYTE qual;//可变结构限定词
	BYTE tx_cause_1;//传送原因1
	BYTE tx_cause_2;//传送原因2
	BYTE commom_asdu_1;//公共地址1
	BYTE commom_asdu_2;//公共址址2
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
//APDU信息扩展
typedef struct Q_104ApduEx
{
	UINT type;//类型
	UINT asdu;//公共数据单元编号
	BYTE cause;//传送原因
	bool seq;//是否有序
	BYTE *msgbuf;//信息内容 (去掉APCI6个字节域)从类型域开始
	UINT len;//长度（去掉APCI的6个字节的报文总长度）
	UINT num;  //信息无素个数
}Q_104ApduEx;


//单点遥控 命令限定词
typedef struct Q_IEC104_ST_C_SC_SCO {
	BYTE		scs	:1; //单点遥控命令状态
	BYTE		res	:1; //占用为0
	BYTE		qu	:5; //遥控命令品质
	BYTE		se	:1; //选择/执行
}Q_IEC104_ST_C_SC_SCO;

//单点遥控 监视方向收到的数据
typedef struct Q_IEC104_ST_C_SC{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_IEC104_ST_C_SC_SCO  sco;
}Q_IEC104_ST_C_SP;
//遥控  带时标的单点遥控
typedef struct Q_IEC104_ST_C_SC_TA1 {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_IEC104_ST_C_SC_SCO  sco;
	BYTE CP56Time[7];
}Q_IEC104_ST_C_SC_TA1;

//双点遥控 命令限定词
typedef struct Q_IEC104_ST_C_DC_DCO {
	BYTE		dcs	:2; //双点遥控命令状态
	BYTE		qu	:5; //遥控命令品质
	BYTE		se	:1; //选择/执行
}Q_IEC104_ST_C_DC_DCO;

//双点遥控　监视方向收到的数据
typedef struct Q_IEC104_ST_C_DC {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_IEC104_ST_C_SC_SCO  dco;

}Q_IEC104_ST_C_DC;

//设定命令　　　命令限定词 
typedef struct Q_IEC104_ST_C_SE_QOS {
	BYTE		ql	:7; //设点命令品质
	BYTE		se	:1; //选择/执行
}Q_IEC104_ST_C_SE_QOS;


//设定命令 规一化值 设定命令限定词 临视方向上处理
typedef struct Q_IEC104_ST_C_SE {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	BYTE val1;
	BYTE val2:7,
s:1;  //符号位 1为负0为正　
	Q_IEC104_ST_C_SE_QOS qos;

}Q_IEC104_ST_C_SE;

//设定命令 带时标的浮点型
typedef struct Q_IEC104_ST_C_SE_TC1 {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	BYTE val[4];//float　
	Q_IEC104_ST_C_SE_QOS qos;
	BYTE CP56Time[7];
}Q_IEC104_ST_C_SE_TC1;
//浮点类型
typedef struct Q_IEC104_ST_C_SE3 {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	BYTE val[4];
	Q_IEC104_ST_C_SE_QOS qos;

}Q_IEC104_ST_C_SE3;

//升降命令　　　命令限定词 
typedef struct Q_IEC104_ST_C_RC_RCO {
	BYTE  rcs	:2,
          qu    :5,
          se    :1;
}Q_IEC104_ST_C_RC_RCO;

//设定命令 规一化值 设定命令限定词 临视方向上处理
typedef struct Q_IEC104_ST_C_RC {
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_IEC104_ST_C_RC_RCO rco;

}Q_IEC104_ST_C_RC;

// single point information w/quality
//带品质描词的单点遥信信息
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
	BYTE	dpi : 2,		// off=intransit / Open=1 Close=2 / Invlid=3  不确定或中间状态<1>：=确定状态开<2>=确定状态合<3>：=不确定或中间状态
rs1 : 1,		// reserve
rs2 : 1,		// reserve
bl  : 1,		// not blocked=0 / blocked=1
sb  : 1,		// not substituted=0 / substituted=1
nt  : 1,		// topical=0 / not topical=1 
iv  : 1;		// valid=0 / not valid=1
}Q_DIQ104;

// quality descriptor
//品质描述符
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
//单点遥信值
typedef struct Q_SP104{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;
	Q_SIQ104	siq;
} Q_SP104;

// single point value
//双点遥信值
typedef struct Q_DP104{
	BYTE addr0;
	BYTE addr1;
	BYTE addr2;;
	Q_DIQ104	diq;
}Q_DP104;

//Measured Scaled sequence  顺序的测量值，标度量值
typedef struct Q_MS104Seq{
	BYTE	val[2]; //值
	Q_QDS104	qds; //带品质描述词
}Q_MS104Seq;

//Measured Scaled 标度化或规一化值遥测值
typedef struct Q_MS104{
	BYTE	addr0; //addr0、addr1、addr2 信息对象地址
	BYTE	addr1;
	BYTE	addr2;
	Q_MS104Seq seqv;
}Q_MS104;


// Measured Float sequence 
//短浮点型的 顺序 测量值信息
typedef struct Q_MF104Seq{
	unsigned char	val[4]; //值
	struct Q_QDS104	qds; //带品质描述词
}Q_MF104Seq;


// Measured Float 短浮点遥测
typedef struct Q_MF104{
	BYTE	addr0; //addr0、addr1、addr2 信息对象地址
	BYTE	addr1;
	BYTE	addr2;
	Q_MF104Seq seqv;
}Q_MF104;


typedef struct {
	BYTE	val[2]; //值
}Q_MS104Seq_21,Q_MF104Seq_21;

typedef struct {
	BYTE	addr0; //addr0、addr1、addr2 信息对象地址
	BYTE	addr1;
	BYTE	addr2;
	Q_MS104Seq_21 seqv;
}Q_MS104_21,Q_MF104_21;


//遥步
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

typedef Q_MF104Seq Q_BO104Seq;//32 比特串 顺序的
typedef Q_MF104 Q_BO104;         //32比特串 非顺序的

//单点遥控 设定命令限定词  ST表示是struct
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