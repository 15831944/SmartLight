#ifndef __DATABASEAGC_h__
#define __DATABASEAGC_h__

#include "Rtdb_Struct.h"
#define DB_ID_OF_AGC                                31
//各表ID
#define AGC_TAB_Group_ID                          	3101	//AGC分组
#define AGC_TAB_PhvUnit_ID                        	3102	//AGC机组
#define AGC_TAB_Parameter_ID                      	3103	//AGC参数
#define AGC_TAB_Analog_ID                         	3104	//AGC遥测
#define AGC_TAB_Digit_ID                          	3105	//AGC遥信
#define AGC_TAB_Alarm_ID                          	3106	//AGC报警
#define AGC_TAB_SocUnit_ID                        	3107	//AGC储能机组
#define AGC_TAB_LogRecord_ID                      	3108	//AGC动作日志
//各表在数据库中的索引
#define AGC_TAB_Group_INDEX                       	0	//AGC分组
#define AGC_TAB_PhvUnit_INDEX                     	1	//AGC机组
#define AGC_TAB_Parameter_INDEX                   	2	//AGC参数
#define AGC_TAB_Analog_INDEX                      	3	//AGC遥测
#define AGC_TAB_Digit_INDEX                       	4	//AGC遥信
#define AGC_TAB_Alarm_INDEX                       	5	//AGC报警
#define AGC_TAB_SocUnit_INDEX                     	6	//AGC储能机组
#define AGC_TAB_LogRecord_INDEX                   	7	//AGC动作日志

//定义各字段在表中索引(AGC分组)
#define AGC_TAB_Group_FIELD_ID32_INDEX                      	0	//内部ID32
#define AGC_TAB_Group_FIELD_strName_INDEX                   	1	//分组名称
#define AGC_TAB_Group_FIELD_iEnabled_INDEX                  	2	//启用标识
#define AGC_TAB_Group_FIELD_iPriority_INDEX                 	3	//调节优先级

//定义各字段在表中索引(AGC机组)
#define AGC_TAB_PhvUnit_FIELD_ID32_INDEX                    	0	//内部ID32
#define AGC_TAB_PhvUnit_FIELD_strName_INDEX                 	1	//机组名称
#define AGC_TAB_PhvUnit_FIELD_fValP_INDEX                   	2	//实时有功kW
#define AGC_TAB_PhvUnit_FIELD_fDisPower_INDEX               	3	//分配有功kW
#define AGC_TAB_PhvUnit_FIELD_fcurCap_INDEX                 	4	//有功额定kW
#define AGC_TAB_PhvUnit_FIELD_fAdMaxLimit_INDEX             	5	//可调上限kW
#define AGC_TAB_PhvUnit_FIELD_fAdMinLimit_INDEX             	6	//可调下限kW
#define AGC_TAB_PhvUnit_FIELD_iGroup_INDEX                  	7	//组号
#define AGC_TAB_PhvUnit_FIELD_iEnabled_INDEX                	8	//启用标识
#define AGC_TAB_PhvUnit_FIELD_iStatus_INDEX                 	9	//机组运行状态
#define AGC_TAB_PhvUnit_FIELD_iChannelStatus_INDEX          	10	//通道状态
#define AGC_TAB_PhvUnit_FIELD_iUpLock_INDEX                 	11	//增闭锁
#define AGC_TAB_PhvUnit_FIELD_iDownLock_INDEX               	12	//减闭锁
#define AGC_TAB_PhvUnit_FIELD_tSnapTime_INDEX               	13	//时标量
#define AGC_TAB_PhvUnit_FIELD_iType_INDEX                   	14	//机组类型
#define AGC_TAB_PhvUnit_FIELD_iFlag_INDEX                   	15	//标识
#define AGC_TAB_PhvUnit_FIELD_fValUp_INDEX                  	16	//目标指令返回值kW
#define AGC_TAB_PhvUnit_FIELD_fAdZone_INDEX                 	17	//调节死区kW
#define AGC_TAB_PhvUnit_FIELD_iModifyAdd_INDEX              	18	//增有功误差修正kW
#define AGC_TAB_PhvUnit_FIELD_iModifySub_INDEX              	19	//减有功误差修正kW
#define AGC_TAB_PhvUnit_FIELD_iAdTimesMax_INDEX             	20	//调节最大次数
#define AGC_TAB_PhvUnit_FIELD_iPowerAlgID_INDEX             	21	//有功遥测ID
#define AGC_TAB_PhvUnit_FIELD_iUpValAlgID_INDEX             	22	//指令返回值遥测ID
#define AGC_TAB_PhvUnit_FIELD_iStatusDigitID_INDEX          	23	//机组状态遥测ID
#define AGC_TAB_PhvUnit_FIELD_iChannelDigitID_INDEX         	24	//通道遥信ID
#define AGC_TAB_PhvUnit_FIELD_iChannelID_INDEX              	25	//通道号
#define AGC_TAB_PhvUnit_FIELD_iDevice_INDEX                 	26	//设备编号
#define AGC_TAB_PhvUnit_FIELD_iYtNo_INDEX                   	27	//遥调点号
#define AGC_TAB_PhvUnit_FIELD_iYkNo_INDEX                   	28	//遥控点号
#define AGC_TAB_PhvUnit_FIELD_fFactor_INDEX                 	29	//遥调系数
#define AGC_TAB_PhvUnit_FIELD_iQuality_INDEX                	30	//质量码
#define AGC_TAB_PhvUnit_FIELD_iDisPower_INDEX               	31	//分配有功遥测ID

//定义各字段在表中索引(AGC参数)
#define AGC_TAB_Parameter_FIELD_ID32_INDEX                  	0	//内部ID32
#define AGC_TAB_Parameter_FIELD_strName_INDEX               	1	//定值名称
#define AGC_TAB_Parameter_FIELD_strKey_INDEX                	2	//关键字
#define AGC_TAB_Parameter_FIELD_iType_INDEX                 	3	//定值类型
#define AGC_TAB_Parameter_FIELD_iVal_INDEX                  	4	//整形定值

//定义各字段在表中索引(AGC遥测)
#define AGC_TAB_Analog_FIELD_ID32_INDEX                     	0	//内部ID32
#define AGC_TAB_Analog_FIELD_strName_INDEX                  	1	//名称
#define AGC_TAB_Analog_FIELD_strKey_INDEX                   	2	//关键字
#define AGC_TAB_Analog_FIELD_iType_INDEX                    	3	//数据类型
#define AGC_TAB_Analog_FIELD_fVal_INDEX                     	4	//值
#define AGC_TAB_Analog_FIELD_iAnalogID_INDEX                	5	//遥测ID
#define AGC_TAB_Analog_FIELD_tSnapTime_INDEX                	6	//时标量
#define AGC_TAB_Analog_FIELD_iQuality_INDEX                 	7	//质量码

//定义各字段在表中索引(AGC遥信)
#define AGC_TAB_Digit_FIELD_ID32_INDEX                      	0	//内部ID32
#define AGC_TAB_Digit_FIELD_strName_INDEX                   	1	//定值名称
#define AGC_TAB_Digit_FIELD_strKey_INDEX                    	2	//关键字
#define AGC_TAB_Digit_FIELD_iType_INDEX                     	3	//数据类型
#define AGC_TAB_Digit_FIELD_iVal_INDEX                      	4	//值
#define AGC_TAB_Digit_FIELD_iDigitID_INDEX                  	5	//遥信ID
#define AGC_TAB_Digit_FIELD_tSnapTime_INDEX                 	6	//时标量
#define AGC_TAB_Digit_FIELD_iQuality_INDEX                  	7	//质量码

//定义各字段在表中索引(AGC报警)
#define AGC_TAB_Alarm_FIELD_ID32_INDEX                      	0	//内部ID32
#define AGC_TAB_Alarm_FIELD_strName_INDEX                   	1	//定值名称
#define AGC_TAB_Alarm_FIELD_strKey_INDEX                    	2	//关键字
#define AGC_TAB_Alarm_FIELD_iVal_INDEX                      	3	//值
#define AGC_TAB_Alarm_FIELD_iDigitID_INDEX                  	4	//遥信ID
#define AGC_TAB_Alarm_FIELD_tSnapTime_INDEX                 	5	//时标量

//定义各字段在表中索引(AGC储能机组)
#define AGC_TAB_SocUnit_FIELD_ID32_INDEX                    	0	//内部ID32
#define AGC_TAB_SocUnit_FIELD_strName_INDEX                 	1	//机组名称
#define AGC_TAB_SocUnit_FIELD_fValP_INDEX                   	2	//实时有功kW
#define AGC_TAB_SocUnit_FIELD_fBatteryPower_INDEX           	3	//电池电量%
#define AGC_TAB_SocUnit_FIELD_fcurCap_INDEX                 	4	//额定kWh
#define AGC_TAB_SocUnit_FIELD_fValAC_INDEX                  	5	//交流电流
#define AGC_TAB_SocUnit_FIELD_fValUa_INDEX                  	6	//交流电压
#define AGC_TAB_SocUnit_FIELD_fValUp_INDEX                  	7	//有功设定值上送
#define AGC_TAB_SocUnit_FIELD_fAdMaxLimit_INDEX             	8	//充电上限%
#define AGC_TAB_SocUnit_FIELD_fAdMinLimit_INDEX             	9	//放电下限%
#define AGC_TAB_SocUnit_FIELD_iGroup_INDEX                  	10	//组号
#define AGC_TAB_SocUnit_FIELD_iEnabled_INDEX                	11	//启用标识
#define AGC_TAB_SocUnit_FIELD_iStatus_INDEX                 	12	//机组运行状态
#define AGC_TAB_SocUnit_FIELD_iChannelStatus_INDEX          	13	//通道状态
#define AGC_TAB_SocUnit_FIELD_iExpStatus_INDEX              	14	//机组故障状态
#define AGC_TAB_SocUnit_FIELD_iUpLock_INDEX                 	15	//增闭锁
#define AGC_TAB_SocUnit_FIELD_iDownLock_INDEX               	16	//减闭锁
#define AGC_TAB_SocUnit_FIELD_tSnapTime_INDEX               	17	//时标量
#define AGC_TAB_SocUnit_FIELD_iFlag_INDEX                   	18	//标识
#define AGC_TAB_SocUnit_FIELD_fAdZone_INDEX                 	19	//调节死区kW
#define AGC_TAB_SocUnit_FIELD_iPowerAlgID_INDEX             	20	//有功遥测ID
#define AGC_TAB_SocUnit_FIELD_iStatusDigitID_INDEX          	21	//机组状态遥测ID
#define AGC_TAB_SocUnit_FIELD_iChannelDigitID_INDEX         	22	//通道遥信ID
#define AGC_TAB_SocUnit_FIELD_iExpStatusDigitID_INDEX       	23	//故障状态遥信ID
#define AGC_TAB_SocUnit_FIELD_iChannelID_INDEX              	24	//通道号
#define AGC_TAB_SocUnit_FIELD_iDevice_INDEX                 	25	//设备编号
#define AGC_TAB_SocUnit_FIELD_iYkNo_INDEX                   	26	//遥控点号
#define AGC_TAB_SocUnit_FIELD_iYtNo_INDEX                   	27	//有功遥调点号
#define AGC_TAB_SocUnit_FIELD_iQuality_INDEX                	28	//质量码
#define AGC_TAB_SocUnit_FIELD_iSocAlgID_INDEX               	29	//电池电量遥测ID
#define AGC_TAB_SocUnit_FIELD_iUaAlgID_INDEX                	30	//交流电压遥测ID
#define AGC_TAB_SocUnit_FIELD_iIaAlgID_INDEX                	31	//交流电流遥测ID
#define AGC_TAB_SocUnit_FIELD_iBStatus_INDEX                	32	//电池状态
#define AGC_TAB_SocUnit_FIELD_iBStatusAlgID_INDEX           	33	//电池状态遥测ID
#define AGC_TAB_SocUnit_FIELD_iUpValAlgID_INDEX             	34	//遥调上送遥测ID
#define AGC_TAB_SocUnit_FIELD_fDisPower_INDEX               	35	//分配有功kW
#define AGC_TAB_SocUnit_FIELD_iDisPower_INDEX               	36	//分配有功遥测ID
#define AGC_TAB_SocUnit_FIELD_fFactor_INDEX                 	37	//遥调系数

//定义各字段在表中索引(AGC动作日志)
#define AGC_TAB_LogRecord_FIELD_ID32_INDEX                  	0	//内部ID32
#define AGC_TAB_LogRecord_FIELD_tSnapTime_INDEX             	1	//时标量
#define AGC_TAB_LogRecord_FIELD_strText_INDEX               	2	//内容



typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //分组名称
	INT         		iEnabled;                          //启用标识
	INT         		iPriority;                         //调节优先级
}AGC_Group;		//AGC分组

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //机组名称
	FLOAT       		fValP;                             //实时有功kW
	FLOAT       		fDisPower;                         //分配有功kW
	FLOAT       		fcurCap;                           //有功额定kW
	FLOAT       		fAdMaxLimit;                       //可调上限kW
	FLOAT       		fAdMinLimit;                       //可调下限kW
	UINT        		iGroup;                            //组号
	INT         		iEnabled;                          //启用标识
	INT         		iStatus;                           //机组运行状态
	INT         		iChannelStatus;                    //通道状态
	INT         		iUpLock;                           //增闭锁
	INT         		iDownLock;                         //减闭锁
	SYSTEMTIME  		tSnapTime;                         //时标量
	INT         		iType;                             //机组类型
	INT         		iFlag;                             //标识
	FLOAT       		fValUp;                            //目标指令返回值kW
	FLOAT       		fAdZone;                           //调节死区kW
	FLOAT       		iModifyAdd;                        //增有功误差修正kW
	FLOAT       		iModifySub;                        //减有功误差修正kW
	INT         		iAdTimesMax;                       //调节最大次数
	INT         		iPowerAlgID;                       //有功遥测ID
	INT         		iUpValAlgID;                       //指令返回值遥测ID
	INT         		iStatusDigitID;                    //机组状态遥测ID
	INT         		iChannelDigitID;                   //通道遥信ID
	INT         		iChannelID;                        //通道号
	INT         		iDevice;                           //设备编号
	INT         		iYtNo;                             //遥调点号
	INT         		iYkNo;                             //遥控点号
	FLOAT       		fFactor;                           //遥调系数
	INT         		iQuality;                          //质量码
	INT         		iDisPower;                         //分配有功遥测ID
}AGC_PhvUnit;		//AGC机组

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //定值名称
	CHAR        		strKey[48];                        //关键字
	INT         		iType;                             //定值类型
	INT         		iVal;                              //整形定值
}AGC_Parameter;		//AGC参数

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //名称
	CHAR        		strKey[48];                        //关键字
	INT         		iType;                             //数据类型
	FLOAT       		fVal;                              //值
	INT         		iAnalogID;                         //遥测ID
	SYSTEMTIME  		tSnapTime;                         //时标量
	INT         		iQuality;                          //质量码
}AGC_Analog;		//AGC遥测

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //定值名称
	CHAR        		strKey[48];                        //关键字
	INT         		iType;                             //数据类型
	INT         		iVal;                              //值
	INT         		iDigitID;                          //遥信ID
	SYSTEMTIME  		tSnapTime;                         //时标量
	INT         		iQuality;                          //质量码
}AGC_Digit;		//AGC遥信

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
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //机组名称
	FLOAT       		fValP;                             //实时有功kW
	FLOAT       		fBatteryPower;                     //电池电量%
	FLOAT       		fcurCap;                           //额定kWh
	FLOAT       		fValAC;                            //交流电流
	FLOAT       		fValUa;                            //交流电压
	FLOAT       		fValUp;                            //有功设定值上送
	FLOAT       		fAdMaxLimit;                       //充电上限%
	FLOAT       		fAdMinLimit;                       //放电下限%
	UINT        		iGroup;                            //组号
	INT         		iEnabled;                          //启用标识
	INT         		iStatus;                           //机组运行状态
	INT         		iChannelStatus;                    //通道状态
	INT         		iExpStatus;                        //机组故障状态
	INT         		iUpLock;                           //增闭锁
	INT         		iDownLock;                         //减闭锁
	SYSTEMTIME  		tSnapTime;                         //时标量
	INT         		iFlag;                             //标识
	FLOAT       		fAdZone;                           //调节死区kW
	INT         		iPowerAlgID;                       //有功遥测ID
	INT         		iStatusDigitID;                    //机组状态遥测ID
	INT         		iChannelDigitID;                   //通道遥信ID
	INT         		iExpStatusDigitID;                 //故障状态遥信ID
	INT         		iChannelID;                        //通道号
	INT         		iDevice;                           //设备编号
	INT         		iYkNo;                             //遥控点号
	INT         		iYtNo;                             //有功遥调点号
	INT         		iQuality;                          //质量码
	INT         		iSocAlgID;                         //电池电量遥测ID
	INT         		iUaAlgID;                          //交流电压遥测ID
	INT         		iIaAlgID;                          //交流电流遥测ID
	INT         		iBStatus;                          //电池状态
	INT         		iBStatusAlgID;                     //电池状态遥测ID
	INT         		iUpValAlgID;                       //遥调上送遥测ID
	FLOAT       		fDisPower;                         //分配有功kW
	INT         		iDisPower;                         //分配有功遥测ID
	FLOAT       		fFactor;                           //遥调系数
}AGC_SocUnit;		//AGC储能机组

typedef struct 
{
	INT         		ID32;                              //内部ID32
	SYSTEMTIME  		tSnapTime;                         //时标量
	CHAR        		strText[256];                      //内容
}AGC_LogRecord;		//AGC动作日志


#endif   //__DATABASEAGC_h__
