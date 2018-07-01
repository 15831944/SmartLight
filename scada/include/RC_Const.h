#ifndef __AFX_RC_CONST_H_2004_04_05__
#define __AFX_RC_CONST_H_2004_04_05__

#include "ElementOpr.h"
/*
	typedef struct 
	{
		UINT        		ID32;                              //内部ID32
		CHAR        		strName[40];                       //名称
		INT         		iStationAddr;                      //站址
		CHAR        		strSerial[420];                    //遥控序列
		INT         		iFlag;                             //标志
	}SCADA_RcSerial;		//遥控序列表
*/

//iFlag
#define OneStation_750_DEVICE_SUPPLY	1	//整站750全部开关送电序列（每个站有且仅有一个）
#define OneStation_750_DEVICE_STOP		2	//整站750全部开关停电序列
				//整站750全部开关停电序列的选中与否由程序自动形成
				//站三轨区间开关停电序列也采用该方式，区别是它的开关数少一般是2个
#define PART_10_DEVICE_CLASSIC			3	//10kv典型票
#define PART_10_DEVICE_GENERIC			4	//10kv一般票
#define PART_750_DEVICE_SUPPLY			5	//750部分开关送电序列
#define PART_750_DEVICE_STOP			6	//750部分开关停电序列
//#define Zone_750_DEVICE_STOP			7	//站三轨区间开关停电序列(一个站两条这类停电序列)
											//站三轨区间开关停电序列的选中与否由程序自动形成
											//相临站两条这类停电序列,构成完整三轨区间停电序列
#define OneStation_DEVICE_FG			8	//整站设备复归

//strSerial
//对PART_DEVICE_CLOSE、PART_DEVICE_OPEN
//大小、遥控点号、标志（选中）、目标、延时 ...

//对OneStation_750_DEVICE_CLOSE、OneStation_750_DEVICE_STOP
//大小
//开关遥控点号、标志（选中、被代、开关类型）、刀闸1遥控点号、刀闸2遥控点号、延时 
// ... 
//开关遥控点号、标志（选中、被代、开关类型）、刀闸1遥控点号、刀闸2遥控点号、延时 
//旁母开关遥控点号、标志（参与）、延时
#define Breaker_Select				1	//选中
#define Breaker_Substitute			(1<<1)	//被替代
#define Breaker_BP					(1<<2)	//旁母开关
#define Breaker_Source				(1<<3)	//总开关（刀闸5 1）
#define Breaker_Feeder				(1<<4)	//分闸开关（刀闸1为旁母刀闸、刀闸2为渠道柜刀闸）
#define NoDoSoftJudge				(1<<5)	//本间隔不进行软件闭锁判断
#define RCFail_NoContinue			(1<<6)	//遥控失败不再进行下面遥控
/*
//遥控闭锁类型定义，可能同时多个原因闭锁
#define RC_ATRESIA_LOCAL			1		//就地
#define RC_ATRESIA_SAME				(1<<1)	//本身位和目标位相同
#define RC_ATRESIA_MASK				(1<<2)	//开关屏蔽
#define RC_ATRESIA_CARD				(1<<3)	//开关挂牌
#define RC_ATRESIA_TEST				(1<<4)	//手车不在合位
#define RC_ATRESIA_GGEAR			(1<<5)	//地刀不在分位
#define RC_ATRESIA_RCING			(1<<6)	//遥控进行中
#define RC_ATRESIA_ABNORMAL			(1<<7)	//开关位置异常
#define RC_ATRESIA_GRANTED			(1<<8)	//开关已授权
#define RC_ATRESIA_UNGRANTED		(1<<9)	//开关未授权
#define RC_ENABLE_GOHOME			(1<<15)	//需要复归
*/
struct tabRCHead
{
	int  ID;
	char strStationName[40];
	char strEditUserName[20];
	char strOperUserName[20]; 
	char strRCStartTime[40];
	char strRCEndTime[40];
};

struct tabRCContent
{
	int  ID;
	int  iNo;
	char strContent[120];
	char strResultName[8];
};

typedef XArray<tabRCContent, tabRCContent&> CtabRCContentArray;

#endif // __AFX_RC_CONST_H_2004_04_05__