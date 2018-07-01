#ifndef __DATABASEAVC_h__
#define __DATABASEAVC_h__

#include "Rtdb_Struct.h"
#define DB_ID_OF_AVC                                51
//各表ID
#define AVC_TAB_Group_ID                          	5101	//分组
#define AVC_TAB_PhvUnit_ID                        	5102	//AVC机组
#define AVC_TAB_Parameter_ID                      	5103	//AVC调节参数
#define AVC_TAB_Analog_ID                         	5104	//AVC参与计算遥测
#define AVC_TAB_Digit_ID                          	5105	//AVC参与计算遥信
#define AVC_TAB_Alarm_ID                          	5106	//AVC报警
#define AVC_TAB_SvgUnit_ID                        	5107	//AVC无功补偿
#define AVC_TAB_FcUnit_ID                         	5108	//AVC电力电容器
#define AVC_TAB_Xfmr_ID                           	5109	//AVC变压器
#define AVC_TAB_LogRecord_ID                      	5110	//AVC动作日志
//各表在数据库中的索引
#define AVC_TAB_Group_INDEX                       	0	//分组
#define AVC_TAB_PhvUnit_INDEX                     	1	//AVC机组
#define AVC_TAB_Parameter_INDEX                   	2	//AVC调节参数
#define AVC_TAB_Analog_INDEX                      	3	//AVC参与计算遥测
#define AVC_TAB_Digit_INDEX                       	4	//AVC参与计算遥信
#define AVC_TAB_Alarm_INDEX                       	5	//AVC报警
#define AVC_TAB_SvgUnit_INDEX                     	6	//AVC无功补偿
#define AVC_TAB_FcUnit_INDEX                      	7	//AVC电力电容器
#define AVC_TAB_Xfmr_INDEX                        	8	//AVC变压器
#define AVC_TAB_LogRecord_INDEX                   	9	//AVC动作日志

//定义各字段在表中索引(分组)
#define AVC_TAB_Group_FIELD_ID32_INDEX                      	0	//内部ID32
#define AVC_TAB_Group_FIELD_strName_INDEX                   	1	//分组名称
#define AVC_TAB_Group_FIELD_iEnabled_INDEX                  	2	//启用标识

//定义各字段在表中索引(AVC机组)
#define AVC_TAB_PhvUnit_FIELD_ID32_INDEX                    	0	//内部ID32
#define AVC_TAB_PhvUnit_FIELD_strName_INDEX                 	1	//机组名称
#define AVC_TAB_PhvUnit_FIELD_iStatus_INDEX                 	2	//机组运行状态
#define AVC_TAB_PhvUnit_FIELD_fValQ_INDEX                   	3	//无功(kVar)
#define AVC_TAB_PhvUnit_FIELD_fDisQ_INDEX                   	4	//分配无功功率
#define AVC_TAB_PhvUnit_FIELD_tExecTime_INDEX               	5	//执行命令时间
#define AVC_TAB_PhvUnit_FIELD_iChannelStatus_INDEX          	6	//通道状态
#define AVC_TAB_PhvUnit_FIELD_iUpLock_INDEX                 	7	//增闭锁
#define AVC_TAB_PhvUnit_FIELD_iDownLock_INDEX               	8	//减闭锁
#define AVC_TAB_PhvUnit_FIELD_iFlag_INDEX                   	9	//标识
#define AVC_TAB_PhvUnit_FIELD_iEnabled_INDEX                	10	//启用标识
#define AVC_TAB_PhvUnit_FIELD_fRatingQ_INDEX                	11	//额定容量(kVar)
#define AVC_TAB_PhvUnit_FIELD_fAdMaxLimit_INDEX             	12	//调节高限(kVar)
#define AVC_TAB_PhvUnit_FIELD_fAdMinLimit_INDEX             	13	//调节低限(kVar)
#define AVC_TAB_PhvUnit_FIELD_fAdZone_INDEX                 	14	//调节死区(kVar)
#define AVC_TAB_PhvUnit_FIELD_iChannelID_INDEX              	15	//通道号
#define AVC_TAB_PhvUnit_FIELD_iDevice_INDEX                 	16	//设备编号
#define AVC_TAB_PhvUnit_FIELD_iYtNo_INDEX                   	17	//遥调点号
#define AVC_TAB_PhvUnit_FIELD_fFactor_INDEX                 	18	//遥调系数
#define AVC_TAB_PhvUnit_FIELD_iGroup_INDEX                  	19	//组号
#define AVC_TAB_PhvUnit_FIELD_iType_INDEX                   	20	//机组类型
#define AVC_TAB_PhvUnit_FIELD_iPriority_INDEX               	21	//调节优先级
#define AVC_TAB_PhvUnit_FIELD_tSnapTime_INDEX               	22	//时标量
#define AVC_TAB_PhvUnit_FIELD_iAdTimesMax_INDEX             	23	//调节最大次数
#define AVC_TAB_PhvUnit_FIELD_iStatusDigitID_INDEX          	24	//机组状态对应遥信ID
#define AVC_TAB_PhvUnit_FIELD_iChannelDigitID_INDEX         	25	//通道对应遥信ID
#define AVC_TAB_PhvUnit_FIELD_iQAlgID_INDEX                 	26	//无功对应遥测ID
#define AVC_TAB_PhvUnit_FIELD_iUpValAlgID_INDEX             	27	//指令返回值对应遥测ID
#define AVC_TAB_PhvUnit_FIELD_iDisQAlgID_INDEX              	28	//分配无功对应遥测ID
#define AVC_TAB_PhvUnit_FIELD_fUpVal_INDEX                  	29	//逆变器指令返回值
#define AVC_TAB_PhvUnit_FIELD_iQuality_INDEX                	30	//质量码

//定义各字段在表中索引(AVC调节参数)
#define AVC_TAB_Parameter_FIELD_ID32_INDEX                  	0	//内部ID32
#define AVC_TAB_Parameter_FIELD_strName_INDEX               	1	//定值名称
#define AVC_TAB_Parameter_FIELD_strKey_INDEX                	2	//关键字
#define AVC_TAB_Parameter_FIELD_iType_INDEX                 	3	//定值类型
#define AVC_TAB_Parameter_FIELD_iVal_INDEX                  	4	//定值

//定义各字段在表中索引(AVC参与计算遥测)
#define AVC_TAB_Analog_FIELD_ID32_INDEX                     	0	//内部ID32
#define AVC_TAB_Analog_FIELD_strName_INDEX                  	1	//名称
#define AVC_TAB_Analog_FIELD_strKey_INDEX                   	2	//关键字
#define AVC_TAB_Analog_FIELD_iType_INDEX                    	3	//数据类型
#define AVC_TAB_Analog_FIELD_fVal_INDEX                     	4	//值
#define AVC_TAB_Analog_FIELD_iAnalogID_INDEX                	5	//遥测ID
#define AVC_TAB_Analog_FIELD_tSnapTime_INDEX                	6	//时标量
#define AVC_TAB_Analog_FIELD_iGroup_INDEX                   	7	//组号
#define AVC_TAB_Analog_FIELD_iQuality_INDEX                 	8	//质量码

//定义各字段在表中索引(AVC参与计算遥信)
#define AVC_TAB_Digit_FIELD_ID32_INDEX                      	0	//内部ID32
#define AVC_TAB_Digit_FIELD_strName_INDEX                   	1	//定值名称
#define AVC_TAB_Digit_FIELD_strKey_INDEX                    	2	//关键字
#define AVC_TAB_Digit_FIELD_iType_INDEX                     	3	//数据类型
#define AVC_TAB_Digit_FIELD_iVal_INDEX                      	4	//值
#define AVC_TAB_Digit_FIELD_iDigitID_INDEX                  	5	//遥信ID
#define AVC_TAB_Digit_FIELD_tSnapTime_INDEX                 	6	//时标量
#define AVC_TAB_Digit_FIELD_iGroup_INDEX                    	7	//组号
#define AVC_TAB_Digit_FIELD_iQuality_INDEX                  	8	//质量码

//定义各字段在表中索引(AVC报警)
#define AVC_TAB_Alarm_FIELD_ID32_INDEX                      	0	//内部ID32
#define AVC_TAB_Alarm_FIELD_strName_INDEX                   	1	//定值名称
#define AVC_TAB_Alarm_FIELD_strKey_INDEX                    	2	//关键字
#define AVC_TAB_Alarm_FIELD_iVal_INDEX                      	3	//值
#define AVC_TAB_Alarm_FIELD_iDigitID_INDEX                  	4	//遥信ID
#define AVC_TAB_Alarm_FIELD_tSnapTime_INDEX                 	5	//时标量
#define AVC_TAB_Alarm_FIELD_iGroup_INDEX                    	6	//组号

//定义各字段在表中索引(AVC无功补偿)
#define AVC_TAB_SvgUnit_FIELD_ID32_INDEX                    	0	//内部ID32
#define AVC_TAB_SvgUnit_FIELD_strName_INDEX                 	1	//机组名称
#define AVC_TAB_SvgUnit_FIELD_iStatus_INDEX                 	2	//机组运行状态
#define AVC_TAB_SvgUnit_FIELD_fValQ_INDEX                   	3	//无功(kVar)
#define AVC_TAB_SvgUnit_FIELD_fDisQ_INDEX                   	4	//分配无功功率(kVar)
#define AVC_TAB_SvgUnit_FIELD_fUpVal_INDEX                  	5	//目标指令上送
#define AVC_TAB_SvgUnit_FIELD_tExecTime_INDEX               	6	//执行命令时间
#define AVC_TAB_SvgUnit_FIELD_iChannelStatus_INDEX          	7	//通道状态
#define AVC_TAB_SvgUnit_FIELD_iUpLock_INDEX                 	8	//增闭锁
#define AVC_TAB_SvgUnit_FIELD_iDownLock_INDEX               	9	//减闭锁
#define AVC_TAB_SvgUnit_FIELD_iSvgExp_INDEX                 	10	//SVG故障
#define AVC_TAB_SvgUnit_FIELD_iFlag_INDEX                   	11	//标识
#define AVC_TAB_SvgUnit_FIELD_tSnapTime_INDEX               	12	//时标量
#define AVC_TAB_SvgUnit_FIELD_iEnabled_INDEX                	13	//启用标识
#define AVC_TAB_SvgUnit_FIELD_fCapacity_INDEX               	14	//额定容量(kVar)
#define AVC_TAB_SvgUnit_FIELD_fAdMaxLimit_INDEX             	15	//调节高限(kVar)
#define AVC_TAB_SvgUnit_FIELD_fAdMinLimit_INDEX             	16	//调节低限(kVar)
#define AVC_TAB_SvgUnit_FIELD_iType_INDEX                   	17	//机组类型
#define AVC_TAB_SvgUnit_FIELD_iGroup_INDEX                  	18	//组号
#define AVC_TAB_SvgUnit_FIELD_fRateVolt_INDEX               	19	//额定电压
#define AVC_TAB_SvgUnit_FIELD_iChannelID_INDEX              	20	//通道号
#define AVC_TAB_SvgUnit_FIELD_iDevice_INDEX                 	21	//设备编号
#define AVC_TAB_SvgUnit_FIELD_iYtNo_INDEX                   	22	//遥调点号
#define AVC_TAB_SvgUnit_FIELD_fYtFactor_INDEX               	23	//遥调系数
#define AVC_TAB_SvgUnit_FIELD_iStatusDigitID_INDEX          	24	//机组状态对应遥信ID
#define AVC_TAB_SvgUnit_FIELD_iChannelDigitID_INDEX         	25	//通道对应遥信ID
#define AVC_TAB_SvgUnit_FIELD_iSvgExpDigitID_INDEX          	26	//SVG故障对应遥信ID
#define AVC_TAB_SvgUnit_FIELD_iUpBlockDigitID_INDEX         	27	//增闭锁对应遥信ID
#define AVC_TAB_SvgUnit_FIELD_iDownBlockDigitID_INDEX       	28	//减闭锁对应遥信ID
#define AVC_TAB_SvgUnit_FIELD_iQAlgID_INDEX                 	29	//无功对应遥测ID
#define AVC_TAB_SvgUnit_FIELD_iLimitSource_INDEX            	30	//调节上下限来源
#define AVC_TAB_SvgUnit_FIELD_iAdMaxLimitAlgID_INDEX        	31	//调节上限对应遥测ID
#define AVC_TAB_SvgUnit_FIELD_iAdMinLimitAlgID_INDEX        	32	//调节下限对应遥测ID
#define AVC_TAB_SvgUnit_FIELD_iCmdBaseValSource_INDEX       	33	//当前命令基值来源
#define AVC_TAB_SvgUnit_FIELD_iUpValAlgID_INDEX             	34	//目标指令上送对应遥测ID
#define AVC_TAB_SvgUnit_FIELD_iDisQAlgID_INDEX              	35	//分配无功对应遥测ID
#define AVC_TAB_SvgUnit_FIELD_iQuality_INDEX                	36	//质量码

//定义各字段在表中索引(AVC电力电容器)
#define AVC_TAB_FcUnit_FIELD_ID32_INDEX                     	0	//内部ID32
#define AVC_TAB_FcUnit_FIELD_strName_INDEX                  	1	//机组名称
#define AVC_TAB_FcUnit_FIELD_fValQ_INDEX                    	2	//当前无功(kVar)
#define AVC_TAB_FcUnit_FIELD_iReqOn_INDEX                   	3	//需要投入
#define AVC_TAB_FcUnit_FIELD_iReqOff_INDEX                  	4	//需要退出
#define AVC_TAB_FcUnit_FIELD_iStatus_INDEX                  	5	//机组运行状态
#define AVC_TAB_FcUnit_FIELD_iOnOffTimes_INDEX              	6	//电容器当天投切次数
#define AVC_TAB_FcUnit_FIELD_iFlag_INDEX                    	7	//标识
#define AVC_TAB_FcUnit_FIELD_fcurCap_INDEX                  	8	//额定容量(kVar)
#define AVC_TAB_FcUnit_FIELD_iGroup_INDEX                   	9	//组号
#define AVC_TAB_FcUnit_FIELD_fOnffMaxTimes_INDEX            	10	//电容器当天最大投切次数
#define AVC_TAB_FcUnit_FIELD_tSnapTime_INDEX                	11	//时标量
#define AVC_TAB_FcUnit_FIELD_iEnabled_INDEX                 	12	//启用标识
#define AVC_TAB_FcUnit_FIELD_iStatusDigitID_INDEX           	13	//机组状态对应遥信ID
#define AVC_TAB_FcUnit_FIELD_iOnOffTimesAlgID_INDEX         	14	//电容器当天投切次数测ID
#define AVC_TAB_FcUnit_FIELD_iOnFlgDigitID_INDEX            	15	//需要投入标识遥信ID
#define AVC_TAB_FcUnit_FIELD_iOffFlgDigitID_INDEX           	16	//需要退出标识遥信ID
#define AVC_TAB_FcUnit_FIELD_iQuality_INDEX                 	17	//质量码
#define AVC_TAB_FcUnit_FIELD_iValAlgID_INDEX                	18	//当前无功遥测ID

//定义各字段在表中索引(AVC变压器)
#define AVC_TAB_Xfmr_FIELD_ID32_INDEX                       	0	//内部ID32
#define AVC_TAB_Xfmr_FIELD_strName_INDEX                    	1	//机组名称
#define AVC_TAB_Xfmr_FIELD_iStatus_INDEX                    	2	//变压器状态
#define AVC_TAB_Xfmr_FIELD_iCurTap_INDEX                    	3	//当前档位
#define AVC_TAB_Xfmr_FIELD_iDisTap_INDEX                    	4	//分配执行的档位
#define AVC_TAB_Xfmr_FIELD_iHighBrk_INDEX                   	5	//高压侧开关状态
#define AVC_TAB_Xfmr_FIELD_iLowBrk_INDEX                    	6	//低压侧开关状态
#define AVC_TAB_Xfmr_FIELD_iChannelStatus_INDEX             	7	//通道状态
#define AVC_TAB_Xfmr_FIELD_iAlarmDownTapTip_INDEX           	8	//降档升电压报警
#define AVC_TAB_Xfmr_FIELD_iAlarmUpTapTip_INDEX             	9	//升档降电压报警
#define AVC_TAB_Xfmr_FIELD_fHighP_INDEX                     	10	//高压侧有功
#define AVC_TAB_Xfmr_FIELD_fHighQ_INDEX                     	11	//高压侧无功
#define AVC_TAB_Xfmr_FIELD_fHighVoltage_INDEX               	12	//高压侧电压
#define AVC_TAB_Xfmr_FIELD_fHighCurrent_INDEX               	13	//高压侧电流
#define AVC_TAB_Xfmr_FIELD_fLowP_INDEX                      	14	//低压侧有功
#define AVC_TAB_Xfmr_FIELD_fLowQ_INDEX                      	15	//低压侧无功
#define AVC_TAB_Xfmr_FIELD_fLowVoltage_INDEX                	16	//低压侧电压
#define AVC_TAB_Xfmr_FIELD_fLowCurrent_INDEX                	17	//低压侧电流
#define AVC_TAB_Xfmr_FIELD_iAdTimesThisDay_INDEX            	18	//当天调节次数
#define AVC_TAB_Xfmr_FIELD_iUpVoltBlock_INDEX               	19	//升压闭锁
#define AVC_TAB_Xfmr_FIELD_iDownVoltBlock_INDEX             	20	//降压闭锁
#define AVC_TAB_Xfmr_FIELD_iExption_INDEX                   	21	//变压器故障
#define AVC_TAB_Xfmr_FIELD_tSnapTime_INDEX                  	22	//时标量
#define AVC_TAB_Xfmr_FIELD_iEnabled_INDEX                   	23	//启用标识
#define AVC_TAB_Xfmr_FIELD_fCurCap_INDEX                    	24	//额定容量(kVar)
#define AVC_TAB_Xfmr_FIELD_fHighFrenqucy_INDEX              	25	//额定频率
#define AVC_TAB_Xfmr_FIELD_fRatedHighVolt_INDEX             	26	//高压侧额定电压(kV)
#define AVC_TAB_Xfmr_FIELD_fRatedLowVolt_INDEX              	27	//低压侧额定电压(kV)
#define AVC_TAB_Xfmr_FIELD_iRatedTapPos_INDEX               	28	//额定档位
#define AVC_TAB_Xfmr_FIELD_iAutoFlg_INDEX                   	29	//自动调节标识
#define AVC_TAB_Xfmr_FIELD_iType_INDEX                      	30	//自动调节标识
#define AVC_TAB_Xfmr_FIELD_iMaxTapLimit_INDEX               	31	//最高调节档位限制
#define AVC_TAB_Xfmr_FIELD_iMinTapLimit_INDEX               	32	//最低调节档位限制
#define AVC_TAB_Xfmr_FIELD_iFlag_INDEX                      	33	//标识
#define AVC_TAB_Xfmr_FIELD_iGroup_INDEX                     	34	//组号
#define AVC_TAB_Xfmr_FIELD_iChannelID_INDEX                 	35	//通道号
#define AVC_TAB_Xfmr_FIELD_iDevice_INDEX                    	36	//设备编号
#define AVC_TAB_Xfmr_FIELD_iYtNo_INDEX                      	37	//遥调点号
#define AVC_TAB_Xfmr_FIELD_iYtFactor_INDEX                  	38	//遥调系数
#define AVC_TAB_Xfmr_FIELD_iCurTapAlgID_INDEX               	39	//当前档位遥测点号ID
#define AVC_TAB_Xfmr_FIELD_iHighPAlgID_INDEX                	40	//高压侧实时有功遥测ID
#define AVC_TAB_Xfmr_FIELD_iHigthQAlgID_INDEX               	41	//高压侧实时无功遥测ID
#define AVC_TAB_Xfmr_FIELD_iHighFrenqucyAlgID_INDEX         	42	//高压侧实时频率遥测ID
#define AVC_TAB_Xfmr_FIELD_iHighVoltAlgID_INDEX             	43	//高压侧电压遥测ID
#define AVC_TAB_Xfmr_FIELD_iHighCurrentAlgID_INDEX          	44	//高压侧电流遥测ID
#define AVC_TAB_Xfmr_FIELD_iLowPAlgID_INDEX                 	45	//低压侧有功遥测ID
#define AVC_TAB_Xfmr_FIELD_iLowQAlgID_INDEX                 	46	//低压侧无功遥测ID
#define AVC_TAB_Xfmr_FIELD_iLowVoltAlgID_INDEX              	47	//低压侧电压遥测ID
#define AVC_TAB_Xfmr_FIELD_iLowCurrentAlgID_INDEX           	48	//低压侧电流遥测ID
#define AVC_TAB_Xfmr_FIELD_iAdTimesAlgID_INDEX              	49	//当天调节次数遥测ID
#define AVC_TAB_Xfmr_FIELD_iDisTapAlgID_INDEX               	50	//分配执行的档位遥测ID
#define AVC_TAB_Xfmr_FIELD_iHighBrkDigitID_INDEX            	51	//高压侧开关状态遥信ID
#define AVC_TAB_Xfmr_FIELD_iLowBrkDigitID_INDEX             	52	//低压侧开关状态遥信ID
#define AVC_TAB_Xfmr_FIELD_iChannelDigitID_INDEX            	53	//通道状态遥信ID
#define AVC_TAB_Xfmr_FIELD_iUpBlockDigitID_INDEX            	54	//升压闭锁ID
#define AVC_TAB_Xfmr_FIELD_iDownBlockDigitID_INDEX          	55	//降压闭锁ID
#define AVC_TAB_Xfmr_FIELD_iExceptionDigitID_INDEX          	56	//变压器故障遥信ID
#define AVC_TAB_Xfmr_FIELD_iDownTapTipDigitID_INDEX         	57	//降档升电压报警遥信ID
#define AVC_TAB_Xfmr_FIELD_iUpTapTipDigitID_INDEX           	58	//升档降电压报警遥信ID
#define AVC_TAB_Xfmr_FIELD_iQuality_INDEX                   	59	//质量码

//定义各字段在表中索引(AVC动作日志)
#define AVC_TAB_LogRecord_FIELD_ID32_INDEX                  	0	//内部ID32
#define AVC_TAB_LogRecord_FIELD_tSnapTime_INDEX             	1	//时标量
#define AVC_TAB_LogRecord_FIELD_strText_INDEX               	2	//内容



typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //分组名称
	INT         		iEnabled;                          //启用标识
}AVC_Group;		//分组

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //机组名称
	INT         		iStatus;                           //机组运行状态
	FLOAT       		fValQ;                             //无功(kVar)
	FLOAT       		fDisQ;                             //分配无功功率
	SYSTEMTIME  		tExecTime;                         //执行命令时间
	INT         		iChannelStatus;                    //通道状态
	INT         		iUpLock;                           //增闭锁
	INT         		iDownLock;                         //减闭锁
	INT         		iFlag;                             //标识
	INT         		iEnabled;                          //启用标识
	FLOAT       		fRatingQ;                          //额定容量(kVar)
	FLOAT       		fAdMaxLimit;                       //调节高限(kVar)
	FLOAT       		fAdMinLimit;                       //调节低限(kVar)
	FLOAT       		fAdZone;                           //调节死区(kVar)
	INT         		iChannelID;                        //通道号
	INT         		iDevice;                           //设备编号
	INT         		iYtNo;                             //遥调点号
	FLOAT       		fFactor;                           //遥调系数
	UINT        		iGroup;                            //组号
	INT         		iType;                             //机组类型
	INT         		iPriority;                         //调节优先级
	SYSTEMTIME  		tSnapTime;                         //时标量
	INT         		iAdTimesMax;                       //调节最大次数
	INT         		iStatusDigitID;                    //机组状态对应遥信ID
	INT         		iChannelDigitID;                   //通道对应遥信ID
	INT         		iQAlgID;                           //无功对应遥测ID
	INT         		iUpValAlgID;                       //指令返回值对应遥测ID
	INT         		iDisQAlgID;                        //分配无功对应遥测ID
	FLOAT       		fUpVal;                            //逆变器指令返回值
	INT         		iQuality;                          //质量码
}AVC_PhvUnit;		//AVC机组

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //定值名称
	CHAR        		strKey[48];                        //关键字
	INT         		iType;                             //定值类型
	INT         		iVal;                              //定值
}AVC_Parameter;		//AVC调节参数

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //名称
	CHAR        		strKey[48];                        //关键字
	INT         		iType;                             //数据类型
	FLOAT       		fVal;                              //值
	INT         		iAnalogID;                         //遥测ID
	SYSTEMTIME  		tSnapTime;                         //时标量
	UINT        		iGroup;                            //组号
	INT         		iQuality;                          //质量码
}AVC_Analog;		//AVC参与计算遥测

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //定值名称
	CHAR        		strKey[48];                        //关键字
	INT         		iType;                             //数据类型
	INT         		iVal;                              //值
	INT         		iDigitID;                          //遥信ID
	SYSTEMTIME  		tSnapTime;                         //时标量
	UINT        		iGroup;                            //组号
	INT         		iQuality;                          //质量码
}AVC_Digit;		//AVC参与计算遥信

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //定值名称
	CHAR        		strKey[48];                        //关键字
	INT         		iVal;                              //值
	INT         		iDigitID;                          //遥信ID
	SYSTEMTIME  		tSnapTime;                         //时标量
	UINT        		iGroup;                            //组号
}AVC_Alarm;		//AVC报警

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //机组名称
	INT         		iStatus;                           //机组运行状态
	FLOAT       		fValQ;                             //无功(kVar)
	FLOAT       		fDisQ;                             //分配无功功率(kVar)
	FLOAT       		fUpVal;                            //目标指令上送
	SYSTEMTIME  		tExecTime;                         //执行命令时间
	INT         		iChannelStatus;                    //通道状态
	INT         		iUpLock;                           //增闭锁
	INT         		iDownLock;                         //减闭锁
	INT         		iSvgExp;                           //SVG故障
	INT         		iFlag;                             //标识
	SYSTEMTIME  		tSnapTime;                         //时标量
	INT         		iEnabled;                          //启用标识
	FLOAT       		fCapacity;                         //额定容量(kVar)
	FLOAT       		fAdMaxLimit;                       //调节高限(kVar)
	FLOAT       		fAdMinLimit;                       //调节低限(kVar)
	INT         		iType;                             //机组类型
	UINT        		iGroup;                            //组号
	FLOAT       		fRateVolt;                         //额定电压
	INT         		iChannelID;                        //通道号
	INT         		iDevice;                           //设备编号
	INT         		iYtNo;                             //遥调点号
	FLOAT       		fYtFactor;                         //遥调系数
	INT         		iStatusDigitID;                    //机组状态对应遥信ID
	INT         		iChannelDigitID;                   //通道对应遥信ID
	INT         		iSvgExpDigitID;                    //SVG故障对应遥信ID
	INT         		iUpBlockDigitID;                   //增闭锁对应遥信ID
	INT         		iDownBlockDigitID;                 //减闭锁对应遥信ID
	INT         		iQAlgID;                           //无功对应遥测ID
	INT         		iLimitSource;                      //调节上下限来源
	INT         		iAdMaxLimitAlgID;                  //调节上限对应遥测ID
	INT         		iAdMinLimitAlgID;                  //调节下限对应遥测ID
	INT         		iCmdBaseValSource;                 //当前命令基值来源
	INT         		iUpValAlgID;                       //目标指令上送对应遥测ID
	INT         		iDisQAlgID;                        //分配无功对应遥测ID
	INT         		iQuality;                          //质量码
}AVC_SvgUnit;		//AVC无功补偿

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //机组名称
	FLOAT       		fValQ;                             //当前无功(kVar)
	INT         		iReqOn;                            //需要投入
	INT         		iReqOff;                           //需要退出
	INT         		iStatus;                           //机组运行状态
	INT         		iOnOffTimes;                       //电容器当天投切次数
	INT         		iFlag;                             //标识
	FLOAT       		fcurCap;                           //额定容量(kVar)
	UINT        		iGroup;                            //组号
	INT         		fOnffMaxTimes;                     //电容器当天最大投切次数
	SYSTEMTIME  		tSnapTime;                         //时标量
	INT         		iEnabled;                          //启用标识
	INT         		iStatusDigitID;                    //机组状态对应遥信ID
	INT         		iOnOffTimesAlgID;                  //电容器当天投切次数测ID
	INT         		iOnFlgDigitID;                     //需要投入标识遥信ID
	INT         		iOffFlgDigitID;                    //需要退出标识遥信ID
	INT         		iQuality;                          //质量码
	INT         		iValAlgID;                         //当前无功遥测ID
}AVC_FcUnit;		//AVC电力电容器

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //机组名称
	INT         		iStatus;                           //变压器状态
	INT         		iCurTap;                           //当前档位
	INT         		iDisTap;                           //分配执行的档位
	INT         		iHighBrk;                          //高压侧开关状态
	INT         		iLowBrk;                           //低压侧开关状态
	INT         		iChannelStatus;                    //通道状态
	INT         		iAlarmDownTapTip;                  //降档升电压报警
	INT         		iAlarmUpTapTip;                    //升档降电压报警
	FLOAT       		fHighP;                            //高压侧有功
	FLOAT       		fHighQ;                            //高压侧无功
	FLOAT       		fHighVoltage;                      //高压侧电压
	FLOAT       		fHighCurrent;                      //高压侧电流
	FLOAT       		fLowP;                             //低压侧有功
	FLOAT       		fLowQ;                             //低压侧无功
	FLOAT       		fLowVoltage;                       //低压侧电压
	FLOAT       		fLowCurrent;                       //低压侧电流
	INT         		iAdTimesThisDay;                   //当天调节次数
	INT         		iUpVoltBlock;                      //升压闭锁
	INT         		iDownVoltBlock;                    //降压闭锁
	INT         		iExption;                          //变压器故障
	SYSTEMTIME  		tSnapTime;                         //时标量
	INT         		iEnabled;                          //启用标识
	FLOAT       		fCurCap;                           //额定容量(kVar)
	FLOAT       		fHighFrenqucy;                     //额定频率
	FLOAT       		fRatedHighVolt;                    //高压侧额定电压(kV)
	FLOAT       		fRatedLowVolt;                     //低压侧额定电压(kV)
	INT         		iRatedTapPos;                      //额定档位
	INT         		iAutoFlg;                          //自动调节标识
	INT         		iType;                             //自动调节标识
	INT         		iMaxTapLimit;                      //最高调节档位限制
	INT         		iMinTapLimit;                      //最低调节档位限制
	INT         		iFlag;                             //标识
	UINT        		iGroup;                            //组号
	INT         		iChannelID;                        //通道号
	INT         		iDevice;                           //设备编号
	INT         		iYtNo;                             //遥调点号
	INT         		iYtFactor;                         //遥调系数
	INT         		iCurTapAlgID;                      //当前档位遥测点号ID
	INT         		iHighPAlgID;                       //高压侧实时有功遥测ID
	INT         		iHigthQAlgID;                      //高压侧实时无功遥测ID
	INT         		iHighFrenqucyAlgID;                //高压侧实时频率遥测ID
	INT         		iHighVoltAlgID;                    //高压侧电压遥测ID
	INT         		iHighCurrentAlgID;                 //高压侧电流遥测ID
	INT         		iLowPAlgID;                        //低压侧有功遥测ID
	INT         		iLowQAlgID;                        //低压侧无功遥测ID
	INT         		iLowVoltAlgID;                     //低压侧电压遥测ID
	INT         		iLowCurrentAlgID;                  //低压侧电流遥测ID
	INT         		iAdTimesAlgID;                     //当天调节次数遥测ID
	INT         		iDisTapAlgID;                      //分配执行的档位遥测ID
	INT         		iHighBrkDigitID;                   //高压侧开关状态遥信ID
	INT         		iLowBrkDigitID;                    //低压侧开关状态遥信ID
	INT         		iChannelDigitID;                   //通道状态遥信ID
	INT         		iUpBlockDigitID;                   //升压闭锁ID
	INT         		iDownBlockDigitID;                 //降压闭锁ID
	INT         		iExceptionDigitID;                 //变压器故障遥信ID
	INT         		iDownTapTipDigitID;                //降档升电压报警遥信ID
	INT         		iUpTapTipDigitID;                  //升档降电压报警遥信ID
	INT         		iQuality;                          //质量码
}AVC_Xfmr;		//AVC变压器

typedef struct 
{
	INT         		ID32;                              //内部ID32
	SYSTEMTIME  		tSnapTime;                         //时标量
	CHAR        		strText[256];                      //内容
}AVC_LogRecord;		//AVC动作日志


#endif   //__DATABASEAVC_h__
