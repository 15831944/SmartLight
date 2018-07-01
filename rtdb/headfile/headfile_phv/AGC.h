#ifndef __DATABASEAGC_h__
#define __DATABASEAGC_h__

#include "Rtdb_Struct.h"
#define DB_ID_OF_AGC                                31
//各表ID
#define AGC_TAB_PhvUnit_ID                        	3101	//AGC机组
#define AGC_TAB_Parameter_ID                      	3102	//AGC调节参数
#define AGC_TAB_Analog_ID                         	3103	//AGC参与计算遥测
#define AGC_TAB_Digit_ID                          	3104	//AGC参与计算遥信
#define AGC_TAB_Alarm_ID                          	3105	//AGC报警
#define AGC_TAB_LogRecord_ID                      	3106	//AGC动作日志
//各表在数据库中的索引
#define AGC_TAB_PhvUnit_INDEX                     	0	//AGC机组
#define AGC_TAB_Parameter_INDEX                   	1	//AGC调节参数
#define AGC_TAB_Analog_INDEX                      	2	//AGC参与计算遥测
#define AGC_TAB_Digit_INDEX                       	3	//AGC参与计算遥信
#define AGC_TAB_Alarm_INDEX                       	4	//AGC报警
#define AGC_TAB_LogRecord_INDEX                   	5	//AGC动作日志

//定义各字段在表中索引(AGC机组)
#define AGC_TAB_PhvUnit_FIELD_ID32_INDEX                    	0	//内部ID32
#define AGC_TAB_PhvUnit_FIELD_strName_INDEX                 	1	//机组名称
#define AGC_TAB_PhvUnit_FIELD_fValP_INDEX                   	2	//有功
#define AGC_TAB_PhvUnit_FIELD_fValQ_INDEX                   	3	//无功
#define AGC_TAB_PhvUnit_FIELD_tSnapTime_INDEX               	4	//时标量
#define AGC_TAB_PhvUnit_FIELD_iType_INDEX                   	5	//机组类型
#define AGC_TAB_PhvUnit_FIELD_iGroup_INDEX                  	6	//组号
#define AGC_TAB_PhvUnit_FIELD_fcurCap_INDEX                 	7	//有功额定
#define AGC_TAB_PhvUnit_FIELD_fAdMinLimit_INDEX             	8	//调节低限
#define AGC_TAB_PhvUnit_FIELD_iStatus_INDEX                 	9	//机组运行状态
#define AGC_TAB_PhvUnit_FIELD_fSendZone_INDEX               	10	//发送调节命令死区
#define AGC_TAB_PhvUnit_FIELD_fAdZone_INDEX                 	11	//调节死区
#define AGC_TAB_PhvUnit_FIELD_iChannelStatus_INDEX          	12	//通道状态
#define AGC_TAB_PhvUnit_FIELD_iEnabled_INDEX                	13	//启用标识
#define AGC_TAB_PhvUnit_FIELD_iFlag_INDEX                   	14	//标识
#define AGC_TAB_PhvUnit_FIELD_iPriority_INDEX               	15	//调节优先级
#define AGC_TAB_PhvUnit_FIELD_fDisPower_INDEX               	16	//分配有功功率
#define AGC_TAB_PhvUnit_FIELD_iUpLock_INDEX                 	17	//增闭锁
#define AGC_TAB_PhvUnit_FIELD_iDownLock_INDEX               	18	//减闭锁
#define AGC_TAB_PhvUnit_FIELD_iPowerAlgID_INDEX             	19	//有功对应遥测ID
#define AGC_TAB_PhvUnit_FIELD_iStatusDigitID_INDEX          	20	//机组状态对应遥信ID
#define AGC_TAB_PhvUnit_FIELD_iChannelDigitID_INDEX         	21	//逆变器通道对应遥信ID
#define AGC_TAB_PhvUnit_FIELD_iChannelID_INDEX              	22	//通道号
#define AGC_TAB_PhvUnit_FIELD_iDevice_INDEX                 	23	//设备编号
#define AGC_TAB_PhvUnit_FIELD_iYtNo_INDEX                   	24	//遥调点号
#define AGC_TAB_PhvUnit_FIELD_iYkNo_INDEX                   	25	//遥控点号
#define AGC_TAB_PhvUnit_FIELD_iAdTimesMax_INDEX             	26	//调节最大次数
#define AGC_TAB_PhvUnit_FIELD_iCanAddAlgID_INDEX            	27	//可调上限对应遥测ID
#define AGC_TAB_PhvUnit_FIELD_iCanSubAlgID_INDEX            	28	//可调下限对应遥测ID
#define AGC_TAB_PhvUnit_FIELD_iUpValAlgID_INDEX             	29	//指令返回值对应遥测ID
#define AGC_TAB_PhvUnit_FIELD_iModifyAdd_INDEX              	30	//调节误差修正增有功
#define AGC_TAB_PhvUnit_FIELD_iModifySub_INDEX              	31	//调节误差修正减有功
#define AGC_TAB_PhvUnit_FIELD_iProcCapAlgID_INDEX           	32	//开机容量对应遥测ID
#define AGC_TAB_PhvUnit_FIELD_iQAlgID_INDEX                 	33	//无功对应遥测ID
#define AGC_TAB_PhvUnit_FIELD_fFactor_INDEX                 	34	//遥调系数
#define AGC_TAB_PhvUnit_FIELD_nonename2_INDEX               	35	//备用字段

//定义各字段在表中索引(AGC调节参数)
#define AGC_TAB_Parameter_FIELD_ID32_INDEX                  	0	//内部ID32
#define AGC_TAB_Parameter_FIELD_strName_INDEX               	1	//定值名称
#define AGC_TAB_Parameter_FIELD_strKey_INDEX                	2	//关键字
#define AGC_TAB_Parameter_FIELD_iType_INDEX                 	3	//定值类型
#define AGC_TAB_Parameter_FIELD_iVal_INDEX                  	4	//整形定值

//定义各字段在表中索引(AGC参与计算遥测)
#define AGC_TAB_Analog_FIELD_ID32_INDEX                     	0	//内部ID32
#define AGC_TAB_Analog_FIELD_strName_INDEX                  	1	//名称
#define AGC_TAB_Analog_FIELD_strKey_INDEX                   	2	//关键字
#define AGC_TAB_Analog_FIELD_iType_INDEX                    	3	//数据类型
#define AGC_TAB_Analog_FIELD_fVal_INDEX                     	4	//值
#define AGC_TAB_Analog_FIELD_iAnalogID_INDEX                	5	//遥测ID
#define AGC_TAB_Analog_FIELD_tSnapTime_INDEX                	6	//时标量

//定义各字段在表中索引(AGC参与计算遥信)
#define AGC_TAB_Digit_FIELD_ID32_INDEX                      	0	//内部ID32
#define AGC_TAB_Digit_FIELD_strName_INDEX                   	1	//定值名称
#define AGC_TAB_Digit_FIELD_strKey_INDEX                    	2	//关键字
#define AGC_TAB_Digit_FIELD_iType_INDEX                     	3	//数据类型
#define AGC_TAB_Digit_FIELD_iVal_INDEX                      	4	//值
#define AGC_TAB_Digit_FIELD_iDigitID_INDEX                  	5	//遥信ID
#define AGC_TAB_Digit_FIELD_tSnapTime_INDEX                 	6	//时标量

//定义各字段在表中索引(AGC报警)
#define AGC_TAB_Alarm_FIELD_ID32_INDEX                      	0	//内部ID32
#define AGC_TAB_Alarm_FIELD_strName_INDEX                   	1	//定值名称
#define AGC_TAB_Alarm_FIELD_strKey_INDEX                    	2	//关键字
#define AGC_TAB_Alarm_FIELD_iVal_INDEX                      	3	//值
#define AGC_TAB_Alarm_FIELD_iDigitID_INDEX                  	4	//遥信ID
#define AGC_TAB_Alarm_FIELD_tSnapTime_INDEX                 	5	//时标量

//定义各字段在表中索引(AGC动作日志)
#define AGC_TAB_LogRecord_FIELD_tSnapTime_INDEX             	0	//时标量
#define AGC_TAB_LogRecord_FIELD_strText_INDEX               	1	//内容



typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //机组名称
	FLOAT       		fValP;                             //有功
	FLOAT       		fValQ;                             //无功
	SYSTEMTIME  		tSnapTime;                         //时标量
	UINT        		iType;                             //机组类型
	UINT        		iGroup;                            //组号
	FLOAT       		fcurCap;                           //有功额定
	FLOAT       		fAdMinLimit;                       //调节低限
	INT         		iStatus;                           //机组运行状态
	FLOAT       		fSendZone;                         //发送调节命令死区
	FLOAT       		fAdZone;                           //调节死区
	INT         		iChannelStatus;                    //通道状态
	INT         		iEnabled;                          //启用标识
	INT         		iFlag;                             //标识
	INT         		iPriority;                         //调节优先级
	FLOAT       		fDisPower;                         //分配有功功率
	INT         		iUpLock;                           //增闭锁
	INT         		iDownLock;                         //减闭锁
	INT         		iPowerAlgID;                       //有功对应遥测ID
	INT         		iStatusDigitID;                    //机组状态对应遥信ID
	INT         		iChannelDigitID;                   //逆变器通道对应遥信ID
	INT         		iChannelID;                        //通道号
	INT         		iDevice;                           //设备编号
	INT         		iYtNo;                             //遥调点号
	INT         		iYkNo;                             //遥控点号
	INT         		iAdTimesMax;                       //调节最大次数
	INT         		iCanAddAlgID;                      //可调上限对应遥测ID
	INT         		iCanSubAlgID;                      //可调下限对应遥测ID
	INT         		iUpValAlgID;                       //指令返回值对应遥测ID
	INT         		iModifyAdd;                        //调节误差修正增有功
	INT         		iModifySub;                        //调节误差修正减有功
	INT         		iProcCapAlgID;                     //开机容量对应遥测ID
	INT         		iQAlgID;                           //无功对应遥测ID
	FLOAT       		fFactor;                           //遥调系数
	INT         		nonename2;                         //备用字段
}AGC_PhvUnit;		//AGC机组

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //定值名称
	CHAR        		strKey[48];                        //关键字
	INT         		iType;                             //定值类型
	INT         		iVal;                              //整形定值
}AGC_Parameter;		//AGC调节参数

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //名称
	CHAR        		strKey[48];                        //关键字
	INT         		iType;                             //数据类型
	FLOAT       		fVal;                              //值
	INT         		iAnalogID;                         //遥测ID
	SYSTEMTIME  		tSnapTime;                         //时标量
}AGC_Analog;		//AGC参与计算遥测

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //定值名称
	CHAR        		strKey[48];                        //关键字
	INT         		iType;                             //数据类型
	INT         		iVal;                              //值
	INT         		iDigitID;                          //遥信ID
	SYSTEMTIME  		tSnapTime;                         //时标量
}AGC_Digit;		//AGC参与计算遥信

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //定值名称
	CHAR        		strKey[48];                        //关键字
	INT         		iVal;                              //值
	INT         		iDigitID;                          //遥信ID
	SYSTEMTIME  		tSnapTime;                         //时标量
}AGC_Alarm;		//AGC报警

typedef struct 
{
	SYSTEMTIME  		tSnapTime;                         //时标量
	CHAR        		strText[256];                      //内容
}AGC_LogRecord;		//AGC动作日志


#endif   //__DATABASEAGC_h__
