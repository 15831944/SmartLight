#ifndef __DATABASESCADA_h__
#define __DATABASESCADA_h__

#include "Rtdb_Struct.h"
#define DB_ID_OF_SCADA                              21
//各表ID
#define SCADA_TAB_TUnit_ID                        	2101	//采集单元表
#define SCADA_TAB_Channel_ID                      	2102	//通道表
#define SCADA_TAB_Analog_ID                       	2103	//模拟量
#define SCADA_TAB_Digit_ID                        	2104	//数字量
#define SCADA_TAB_Relay_ID                        	2105	//遥控表
#define SCADA_TAB_SetPoint_ID                     	2106	//遥调表
#define SCADA_TAB_Protocol_ID                     	2107	//规约表
#define SCADA_TAB_AnalogStat_ID                   	2108	//遥测统计表
#define SCADA_TAB_DigitStat_ID                    	2109	//遥信统计表
#define SCADA_TAB_Global_ID                       	2110	//SCADA全局变量表
#define SCADA_TAB_Formula_ID                      	2111	//计算公式表
//各表在数据库中的索引
#define SCADA_TAB_TUnit_INDEX                     	0	//采集单元表
#define SCADA_TAB_Channel_INDEX                   	1	//通道表
#define SCADA_TAB_Analog_INDEX                    	2	//模拟量
#define SCADA_TAB_Digit_INDEX                     	3	//数字量
#define SCADA_TAB_Relay_INDEX                     	4	//遥控表
#define SCADA_TAB_SetPoint_INDEX                  	5	//遥调表
#define SCADA_TAB_Protocol_INDEX                  	6	//规约表
#define SCADA_TAB_AnalogStat_INDEX                	7	//遥测统计表
#define SCADA_TAB_DigitStat_INDEX                 	8	//遥信统计表
#define SCADA_TAB_Global_INDEX                    	9	//SCADA全局变量表
#define SCADA_TAB_Formula_INDEX                   	10	//计算公式表

//定义各字段在表中索引(采集单元表)
#define SCADA_TAB_TUnit_FIELD_ID32_INDEX                    	0	//内部ID32
#define SCADA_TAB_TUnit_FIELD_uStationID_INDEX              	1	//从属厂站ID
#define SCADA_TAB_TUnit_FIELD_strID_INDEX                   	2	//名称
#define SCADA_TAB_TUnit_FIELD_strName_INDEX                 	3	//描述
#define SCADA_TAB_TUnit_FIELD_iType_INDEX                   	4	//类型
#define SCADA_TAB_TUnit_FIELD_lFlag_INDEX                   	5	//标志
#define SCADA_TAB_TUnit_FIELD_iStationAddr_INDEX            	6	//地址号
#define SCADA_TAB_TUnit_FIELD_dvtype_INDEX                  	7	//设备类型
#define SCADA_TAB_TUnit_FIELD_dvno_INDEX                    	8	//设备号
#define SCADA_TAB_TUnit_FIELD_iStatus_INDEX                 	9	//工作状态
#define SCADA_TAB_TUnit_FIELD_iStatusID_INDEX               	10	//设备状态遥信点
#define SCADA_TAB_TUnit_FIELD_iRecordFlag_INDEX             	11	//报文记录标志
#define SCADA_TAB_TUnit_FIELD_iRecordBegin_INDEX            	12	//报文记录开始时间
#define SCADA_TAB_TUnit_FIELD_iRecordEnd_INDEX              	13	//报文记录结束时间
#define SCADA_TAB_TUnit_FIELD_iRecPacketFlg_INDEX           	14	//报文信息传输开启
#define SCADA_TAB_TUnit_FIELD_iRecPromptFlg_INDEX           	15	//提示信息传输开启

//定义各字段在表中索引(通道表)
#define SCADA_TAB_Channel_FIELD_ID32_INDEX                  	0	//内部ID32
#define SCADA_TAB_Channel_FIELD_strName_INDEX               	1	//描述
#define SCADA_TAB_Channel_FIELD_iFlag_INDEX                 	2	//标志位
#define SCADA_TAB_Channel_FIELD_iType_INDEX                 	3	//通道类型
#define SCADA_TAB_Channel_FIELD_uProtocolID_INDEX           	4	//通讯规约ID
#define SCADA_TAB_Channel_FIELD_strProtocolPara_INDEX       	5	//规约配置文件
#define SCADA_TAB_Channel_FIELD_strLinkParam_INDEX          	6	//链路参数
#define SCADA_TAB_Channel_FIELD_iStatus_INDEX               	7	//运行状态
#define SCADA_TAB_Channel_FIELD_iStatusID_INDEX             	8	//运行状态遥信ID
#define SCADA_TAB_Channel_FIELD_tStartTime_INDEX            	9	//启用时间
#define SCADA_TAB_Channel_FIELD_strTransFile_INDEX          	10	//转发文件名
#define SCADA_TAB_Channel_FIELD_iRunAttr_INDEX              	11	//通道动态属性

//定义各字段在表中索引(模拟量)
#define SCADA_TAB_Analog_FIELD_ID32_INDEX                   	0	//内部ID32
#define SCADA_TAB_Analog_FIELD_uTuID_INDEX                  	1	//采集单元ID
#define SCADA_TAB_Analog_FIELD_strID_INDEX                  	2	//名称
#define SCADA_TAB_Analog_FIELD_strName_INDEX                	3	//描述
#define SCADA_TAB_Analog_FIELD_lDPUID_INDEX                 	4	//所属DPU
#define SCADA_TAB_Analog_FIELD_iNoInDPU_INDEX               	5	//DPU中编号
#define SCADA_TAB_Analog_FIELD_fVal_INDEX                   	6	//工程值
#define SCADA_TAB_Analog_FIELD_iUnit_INDEX                  	7	//单位
#define SCADA_TAB_Analog_FIELD_tSnapTime_INDEX              	8	//时标量
#define SCADA_TAB_Analog_FIELD_iQuality_INDEX               	9	//质量码
#define SCADA_TAB_Analog_FIELD_iIndex_INDEX                 	10	//厂站中序号
#define SCADA_TAB_Analog_FIELD_FormulaID32_INDEX            	11	//公式表ID
#define SCADA_TAB_Analog_FIELD_iZfID_INDEX                  	12	//转发ID
#define SCADA_TAB_Analog_FIELD_iType_INDEX                  	13	//类型
#define SCADA_TAB_Analog_FIELD_iDataType_INDEX              	14	//数据类型
#define SCADA_TAB_Analog_FIELD_fRaw_INDEX                   	15	//原始值
#define SCADA_TAB_Analog_FIELD_fCoeff_INDEX                 	16	//系数
#define SCADA_TAB_Analog_FIELD_fOffset_INDEX                	17	//偏移量
#define SCADA_TAB_Analog_FIELD_lFlag_INDEX                  	18	//静态标志
#define SCADA_TAB_Analog_FIELD_fLowL_INDEX                  	19	//报警下限
#define SCADA_TAB_Analog_FIELD_fHighL_INDEX                 	20	//报警上限
#define SCADA_TAB_Analog_FIELD_fLowL2_INDEX                 	21	//报警下下限
#define SCADA_TAB_Analog_FIELD_fHighL2_INDEX                	22	//报警上上限
#define SCADA_TAB_Analog_FIELD_fDeadSec_INDEX               	23	//死区
#define SCADA_TAB_Analog_FIELD_fValidLL_INDEX               	24	//工程值下限
#define SCADA_TAB_Analog_FIELD_fValidHL_INDEX               	25	//工程值上限
#define SCADA_TAB_Analog_FIELD_fDayMaxV_INDEX               	26	//日最大值
#define SCADA_TAB_Analog_FIELD_fDayMinV_INDEX               	27	//日最小值
#define SCADA_TAB_Analog_FIELD_fDayAveV_INDEX               	28	//日平均值
#define SCADA_TAB_Analog_FIELD_tDayMaxTime_INDEX            	29	//日最大值出现时间
#define SCADA_TAB_Analog_FIELD_tDayMinTime_INDEX            	30	//日最小值出现时间
#define SCADA_TAB_Analog_FIELD_iAlmClass_INDEX              	31	//越限报警等级
#define SCADA_TAB_Analog_FIELD_iAlmClass2_INDEX             	32	//越限限报警等级
#define SCADA_TAB_Analog_FIELD_dvtype_INDEX                 	33	//设备类型
#define SCADA_TAB_Analog_FIELD_dvno_INDEX                   	34	//设备号
#define SCADA_TAB_Analog_FIELD_lDynaFlag_INDEX              	35	//动态标志
#define SCADA_TAB_Analog_FIELD_iSavePeriod_INDEX            	36	//存储周期

//定义各字段在表中索引(数字量)
#define SCADA_TAB_Digit_FIELD_ID32_INDEX                    	0	//内部ID32
#define SCADA_TAB_Digit_FIELD_uTuID_INDEX                   	1	//采集单元ID
#define SCADA_TAB_Digit_FIELD_strID_INDEX                   	2	//名称
#define SCADA_TAB_Digit_FIELD_strName_INDEX                 	3	//描述
#define SCADA_TAB_Digit_FIELD_lDPUID_INDEX                  	4	//所属DPU
#define SCADA_TAB_Digit_FIELD_iNoInDPU_INDEX                	5	//DPU中编号
#define SCADA_TAB_Digit_FIELD_iVal_INDEX                    	6	//工程值
#define SCADA_TAB_Digit_FIELD_tSnapTime_INDEX               	7	//时标量
#define SCADA_TAB_Digit_FIELD_iQuality_INDEX                	8	//质量码
#define SCADA_TAB_Digit_FIELD_iIndex_INDEX                  	9	//在厂站中序号
#define SCADA_TAB_Digit_FIELD_iType_INDEX                   	10	//类型
#define SCADA_TAB_Digit_FIELD_iRaw_INDEX                    	11	//原始值
#define SCADA_TAB_Digit_FIELD_FormulaID32_INDEX             	12	//公式表ID
#define SCADA_TAB_Digit_FIELD_iZfID_INDEX                   	13	//转发ID
#define SCADA_TAB_Digit_FIELD_lFlag_INDEX                   	14	//静态标志
#define SCADA_TAB_Digit_FIELD_iDataType_INDEX               	15	//数据类型
#define SCADA_TAB_Digit_FIELD_iLastTime_INDEX               	16	//去抖时间(秒)
#define SCADA_TAB_Digit_FIELD_iOpenSum_INDEX                	17	//统计分次数
#define SCADA_TAB_Digit_FIELD_iCloseSum_INDEX               	18	//统计合次数
#define SCADA_TAB_Digit_FIELD_iAlmClass_INDEX               	19	//报警等级
#define SCADA_TAB_Digit_FIELD_lDynaFlag_INDEX               	20	//动态标志
#define SCADA_TAB_Digit_FIELD_dvtype_INDEX                  	21	//设备类型
#define SCADA_TAB_Digit_FIELD_dvno_INDEX                    	22	//设备号

//定义各字段在表中索引(遥控表)
#define SCADA_TAB_Relay_FIELD_ID32_INDEX                    	0	//内部ID32
#define SCADA_TAB_Relay_FIELD_uTuID_INDEX                   	1	//采集单元ID
#define SCADA_TAB_Relay_FIELD_strID_INDEX                   	2	//名称
#define SCADA_TAB_Relay_FIELD_strName_INDEX                 	3	//描述
#define SCADA_TAB_Relay_FIELD_lDPUID_INDEX                  	4	//所属DPU
#define SCADA_TAB_Relay_FIELD_iNoInDPU_INDEX                	5	//DPU中编号
#define SCADA_TAB_Relay_FIELD_LogicalClose_INDEX            	6	//逻辑控合
#define SCADA_TAB_Relay_FIELD_LogicalOpen_INDEX             	7	//逻辑控分
#define SCADA_TAB_Relay_FIELD_ulYXID32_INDEX                	8	//在遥信表中的ID
#define SCADA_TAB_Relay_FIELD_selecttime_INDEX              	9	//选择时间
#define SCADA_TAB_Relay_FIELD_exectime_INDEX                	10	//执行时间
#define SCADA_TAB_Relay_FIELD_iZF_INDEX                     	11	//是否转发
#define SCADA_TAB_Relay_FIELD_izfTuID_INDEX                 	12	//转发目的通道编号
#define SCADA_TAB_Relay_FIELD_izfDPUID_INDEX                	13	//转发设备地址
#define SCADA_TAB_Relay_FIELD_izfNoInDPU_INDEX              	14	//转发点号
#define SCADA_TAB_Relay_FIELD_izfProcessID_INDEX            	15	//转发进程ID
#define SCADA_TAB_Relay_FIELD_iType_INDEX                   	16	//类型
#define SCADA_TAB_Relay_FIELD_lFlag_INDEX                   	17	//标志
#define SCADA_TAB_Relay_FIELD_iDataType_INDEX               	18	//数据类型
#define SCADA_TAB_Relay_FIELD_iIndex_INDEX                  	19	//在厂站中的序号
#define SCADA_TAB_Relay_FIELD_uAlarmClass_INDEX             	20	//报警等级
#define SCADA_TAB_Relay_FIELD_iexecret_INDEX                	21	//执行结果
#define SCADA_TAB_Relay_FIELD_iMaxSelectTime_INDEX          	22	//选择最大时延(s)
#define SCADA_TAB_Relay_FIELD_iMaxExecuteTime_INDEX         	23	//执行最大时延(s)
#define SCADA_TAB_Relay_FIELD_iCancelMaxTime_INDEX          	24	//撤销最大时延(s)

//定义各字段在表中索引(遥调表)
#define SCADA_TAB_SetPoint_FIELD_ID32_INDEX                 	0	//内部ID32
#define SCADA_TAB_SetPoint_FIELD_uTuID_INDEX                	1	//采集单元ID
#define SCADA_TAB_SetPoint_FIELD_strID_INDEX                	2	//名称
#define SCADA_TAB_SetPoint_FIELD_strName_INDEX              	3	//描述
#define SCADA_TAB_SetPoint_FIELD_fVal_INDEX                 	4	//工程值
#define SCADA_TAB_SetPoint_FIELD_lDPUID_INDEX               	5	//所属DPU
#define SCADA_TAB_SetPoint_FIELD_iNoInDPU_INDEX             	6	//DPU中编号
#define SCADA_TAB_SetPoint_FIELD_selecttime_INDEX           	7	//选择时间
#define SCADA_TAB_SetPoint_FIELD_exectime_INDEX             	8	//执行时间
#define SCADA_TAB_SetPoint_FIELD_fFactor_INDEX              	9	//遥调系数
#define SCADA_TAB_SetPoint_FIELD_ulYCID32_INDEX             	10	//在遥测表中的ID
#define SCADA_TAB_SetPoint_FIELD_iIndex_INDEX               	11	//在厂站中的序号
#define SCADA_TAB_SetPoint_FIELD_iZF_INDEX                  	12	//是否转发
#define SCADA_TAB_SetPoint_FIELD_izfTuID_INDEX              	13	//转发目的通道编号
#define SCADA_TAB_SetPoint_FIELD_izfDPUID_INDEX             	14	//转发设备地址
#define SCADA_TAB_SetPoint_FIELD_izfNoInDPU_INDEX           	15	//转发点号
#define SCADA_TAB_SetPoint_FIELD_izfProcessID_INDEX         	16	//转发进程ID
#define SCADA_TAB_SetPoint_FIELD_lFlag_INDEX                	17	//标志
#define SCADA_TAB_SetPoint_FIELD_iDataType_INDEX            	18	//数据类型
#define SCADA_TAB_SetPoint_FIELD_iexecret_INDEX             	19	//执行结果

//定义各字段在表中索引(规约表)
#define SCADA_TAB_Protocol_FIELD_ID32_INDEX                 	0	//内部ID32
#define SCADA_TAB_Protocol_FIELD_strName_INDEX              	1	//名称
#define SCADA_TAB_Protocol_FIELD_strDLLName_INDEX           	2	//规约库名
#define SCADA_TAB_Protocol_FIELD_strClassName_INDEX         	3	//规约类名
#define SCADA_TAB_Protocol_FIELD_iProtocolType_INDEX        	4	//规约类型
#define SCADA_TAB_Protocol_FIELD_iProtocolSubType_INDEX     	5	//规约子类型
#define SCADA_TAB_Protocol_FIELD_strTemplate_INDEX          	6	//规约模板文件

//定义各字段在表中索引(遥测统计表)
#define SCADA_TAB_AnalogStat_FIELD_ID32_INDEX               	0	//内部ID32
#define SCADA_TAB_AnalogStat_FIELD_tTime_INDEX              	1	//时标量
#define SCADA_TAB_AnalogStat_FIELD_ulDayNormal_INDEX        	2	//日正常累计时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_ulDayHigh_INDEX          	3	//日越上限累计时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_ulDayLow_INDEX           	4	//日越下限累计时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_ulDayStop_INDEX          	5	//日停运累计时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_fDayHiRatio_INDEX        	6	//日越上限率
#define SCADA_TAB_AnalogStat_FIELD_fDayLowRatio_INDEX       	7	//日越下限率
#define SCADA_TAB_AnalogStat_FIELD_fDayNorRatio_INDEX       	8	//日合格率
#define SCADA_TAB_AnalogStat_FIELD_ulMonthNormal_INDEX      	9	//月正常累计时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_ulMonthHigh_INDEX        	10	//月越上限累计时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_ulMonthLow_INDEX         	11	//月越下限累计时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_ulMonthStop_INDEX        	12	//月停运累计时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_fMonthHiRatio_INDEX      	13	//月越上限率
#define SCADA_TAB_AnalogStat_FIELD_fMonthLowRatio_INDEX     	14	//月越下限率
#define SCADA_TAB_AnalogStat_FIELD_fMonthNorRatio_INDEX     	15	//月合格率
#define SCADA_TAB_AnalogStat_FIELD_ulYearNormal_INDEX       	16	//年正常累计时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_ulYearHigh_INDEX         	17	//年越上限累计时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_ulYearLow_INDEX          	18	//年越下限累计时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_ulYearStop_INDEX         	19	//年停运累计时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_fYearHiRatio_INDEX       	20	//年越上限率
#define SCADA_TAB_AnalogStat_FIELD_fYearLowRatio_INDEX      	21	//年越下限率
#define SCADA_TAB_AnalogStat_FIELD_fYearNorRatio_INDEX      	22	//年合格率
#define SCADA_TAB_AnalogStat_FIELD_iHighL_INDEX             	23	//日越上限次数
#define SCADA_TAB_AnalogStat_FIELD_iLowL_INDEX              	24	//日越下限次数
#define SCADA_TAB_AnalogStat_FIELD_iStatTime_INDEX          	25	//统计间隔
#define SCADA_TAB_AnalogStat_FIELD_iOverMaxNum_INDEX        	26	//最大越限次数
#define SCADA_TAB_AnalogStat_FIELD_iTimeSpan_INDEX          	27	//累计时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_iOverNum_INDEX           	28	//越限次数
#define SCADA_TAB_AnalogStat_FIELD_iOverMaxTime_INDEX       	29	//最长越限时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_iOverTime_INDEX          	30	//越限时间(秒)
#define SCADA_TAB_AnalogStat_FIELD_iOverNumClass_INDEX      	31	//计次越限报警等级
#define SCADA_TAB_AnalogStat_FIELD_iOverTimeClass_INDEX     	32	//计时越限报警等级
#define SCADA_TAB_AnalogStat_FIELD_iDelayMax_INDEX          	33	//延时报警时限
#define SCADA_TAB_AnalogStat_FIELD_iDelayTime_INDEX         	34	//延时累计
#define SCADA_TAB_AnalogStat_FIELD_fhourMaxV_INDEX          	35	//小时最大
#define SCADA_TAB_AnalogStat_FIELD_fhourMinV_INDEX          	36	//小时最小
#define SCADA_TAB_AnalogStat_FIELD_fhourAveV_INDEX          	37	//小时平均

//定义各字段在表中索引(遥信统计表)
#define SCADA_TAB_DigitStat_FIELD_ID32_INDEX                	0	//内部ID32
#define SCADA_TAB_DigitStat_FIELD_tTime_INDEX               	1	//时标量
#define SCADA_TAB_DigitStat_FIELD_tStartTime_INDEX          	2	//统计开始时间
#define SCADA_TAB_DigitStat_FIELD_ulDayNormal_INDEX         	3	//日运行时间累加(秒)
#define SCADA_TAB_DigitStat_FIELD_ulDayStop_INDEX           	4	//日停运时间累加(秒)
#define SCADA_TAB_DigitStat_FIELD_fDayNorRatio_INDEX        	5	//日运行时间百分数
#define SCADA_TAB_DigitStat_FIELD_ulMonthNormal_INDEX       	6	//月运行时间累加(秒)
#define SCADA_TAB_DigitStat_FIELD_ulMonthStop_INDEX         	7	//月停运时间累加(秒)
#define SCADA_TAB_DigitStat_FIELD_fMonthNorRatio_INDEX      	8	//月运行时间百分数
#define SCADA_TAB_DigitStat_FIELD_ulYearNormal_INDEX        	9	//年运行时间累加(秒)
#define SCADA_TAB_DigitStat_FIELD_ulYearStop_INDEX          	10	//年停运时间累加(秒)
#define SCADA_TAB_DigitStat_FIELD_fYearNorRatio_INDEX       	11	//年运行时间百分数
#define SCADA_TAB_DigitStat_FIELD_iOCTime_INDEX             	12	//统计间隔(秒)
#define SCADA_TAB_DigitStat_FIELD_iOCMaxNum_INDEX           	13	//间隔内最大启动次数
#define SCADA_TAB_DigitStat_FIELD_iTimeSpan_INDEX           	14	//累计时间(秒)
#define SCADA_TAB_DigitStat_FIELD_iOCNum_INDEX              	15	//设备启动次数
#define SCADA_TAB_DigitStat_FIELD_iMaxRunTime_INDEX         	16	//最长运行时间(秒)
#define SCADA_TAB_DigitStat_FIELD_iRunTime_INDEX            	17	//运行时间(秒)
#define SCADA_TAB_DigitStat_FIELD_iOverNumClass_INDEX       	18	//计次越限报警等级
#define SCADA_TAB_DigitStat_FIELD_iOverTimeClass_INDEX      	19	//计时越限报警等级
#define SCADA_TAB_DigitStat_FIELD_iDelayMax_INDEX           	20	//延时报警时限
#define SCADA_TAB_DigitStat_FIELD_iDelayTime_INDEX          	21	//延时累计

//定义各字段在表中索引(SCADA全局变量表)
#define SCADA_TAB_Global_FIELD_ID32_INDEX                   	0	//内部ID32
#define SCADA_TAB_Global_FIELD_iNetHeartPeriod_INDEX        	1	//网络判死(秒)
#define SCADA_TAB_Global_FIELD_iNodeHeartPeriod_INDEX       	2	//节点判死(秒)
#define SCADA_TAB_Global_FIELD_iProcessHeartPeriod_INDEX    	3	//进程判死(秒)
#define SCADA_TAB_Global_FIELD_iLogoutTime_INDEX            	4	//无操作注销最长时间(m)
#define SCADA_TAB_Global_FIELD_iFormulaPeriod_INDEX         	5	//无操作注销最长时间(m)

//定义各字段在表中索引(计算公式表)
#define SCADA_TAB_Formula_FIELD_ID32_INDEX                  	0	//内部ID32
#define SCADA_TAB_Formula_FIELD_strName_INDEX               	1	//名称
#define SCADA_TAB_Formula_FIELD_iValID_INDEX                	2	//结果值ID
#define SCADA_TAB_Formula_FIELD_strExpression_INDEX         	3	//公式表达式
#define SCADA_TAB_Formula_FIELD_lFlag_INDEX                 	4	//结果值类型



typedef struct 
{
	UINT        		ID32;                              //内部ID32
	UINT        		uStationID;                        //从属厂站ID
	CHAR        		strID[20];                         //名称
	CHAR        		strName[40];                       //描述
	INT         		iType;                             //类型
	INT         		lFlag;                             //标志
	INT         		iStationAddr;                      //地址号
	INT         		dvtype;                            //设备类型
	INT         		dvno;                              //设备号
	INT         		iStatus;                           //工作状态
	INT         		iStatusID;                         //设备状态遥信点
	INT         		iRecordFlag;                       //报文记录标志
	INT         		iRecordBegin;                      //报文记录开始时间
	INT         		iRecordEnd;                        //报文记录结束时间
	INT         		iRecPacketFlg;                     //报文信息传输开启
	INT         		iRecPromptFlg;                     //提示信息传输开启
}SCADA_TUnit;		//采集单元表

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[80];                       //描述
	INT         		iFlag;                             //标志位
	INT         		iType;                             //通道类型
	INT         		uProtocolID;                       //通讯规约ID
	CHAR        		strProtocolPara[128];              //规约配置文件
	CHAR        		strLinkParam[128];                 //链路参数
	INT         		iStatus;                           //运行状态
	INT         		iStatusID;                         //运行状态遥信ID
	SYSTEMTIME  		tStartTime;                        //启用时间
	CHAR        		strTransFile[40];                  //转发文件名
	INT         		iRunAttr;                          //通道动态属性
}SCADA_Channel;		//通道表

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	UINT        		uTuID;                             //采集单元ID
	CHAR        		strID[32];                         //名称
	CHAR        		strName[80];                       //描述
	INT         		lDPUID;                            //所属DPU
	INT         		iNoInDPU;                          //DPU中编号
	FLOAT       		fVal;                              //工程值
	INT         		iUnit;                             //单位
	SYSTEMTIME  		tSnapTime;                         //时标量
	INT         		iQuality;                          //质量码
	INT         		iIndex;                            //厂站中序号
	INT         		FormulaID32;                       //公式表ID
	INT         		iZfID;                             //转发ID
	INT         		iType;                             //类型
	INT         		iDataType;                         //数据类型
	FLOAT       		fRaw;                              //原始值
	FLOAT       		fCoeff;                            //系数
	FLOAT       		fOffset;                           //偏移量
	INT         		lFlag;                             //静态标志
	FLOAT       		fLowL;                             //报警下限
	FLOAT       		fHighL;                            //报警上限
	FLOAT       		fLowL2;                            //报警下下限
	FLOAT       		fHighL2;                           //报警上上限
	FLOAT       		fDeadSec;                          //死区
	FLOAT       		fValidLL;                          //工程值下限
	FLOAT       		fValidHL;                          //工程值上限
	FLOAT       		fDayMaxV;                          //日最大值
	FLOAT       		fDayMinV;                          //日最小值
	FLOAT       		fDayAveV;                          //日平均值
	SYSTEMTIME  		tDayMaxTime;                       //日最大值出现时间
	SYSTEMTIME  		tDayMinTime;                       //日最小值出现时间
	UINT        		iAlmClass;                         //越限报警等级
	UINT        		iAlmClass2;                        //越限限报警等级
	UINT        		dvtype;                            //设备类型
	UINT        		dvno;                              //设备号
	INT         		lDynaFlag;                         //动态标志
	INT         		iSavePeriod;                       //存储周期
}SCADA_Analog;		//模拟量

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	UINT        		uTuID;                             //采集单元ID
	CHAR        		strID[32];                         //名称
	CHAR        		strName[80];                       //描述
	INT         		lDPUID;                            //所属DPU
	INT         		iNoInDPU;                          //DPU中编号
	INT         		iVal;                              //工程值
	SYSTEMTIME  		tSnapTime;                         //时标量
	INT         		iQuality;                          //质量码
	INT         		iIndex;                            //在厂站中序号
	INT         		iType;                             //类型
	INT         		iRaw;                              //原始值
	INT         		FormulaID32;                       //公式表ID
	INT         		iZfID;                             //转发ID
	INT         		lFlag;                             //静态标志
	INT         		iDataType;                         //数据类型
	INT         		iLastTime;                         //去抖时间(秒)
	INT         		iOpenSum;                          //统计分次数
	INT         		iCloseSum;                         //统计合次数
	UINT        		iAlmClass;                         //报警等级
	INT         		lDynaFlag;                         //动态标志
	UINT        		dvtype;                            //设备类型
	UINT        		dvno;                              //设备号
}SCADA_Digit;		//数字量

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	UINT        		uTuID;                             //采集单元ID
	CHAR        		strID[32];                         //名称
	CHAR        		strName[80];                       //描述
	INT         		lDPUID;                            //所属DPU
	INT         		iNoInDPU;                          //DPU中编号
	UINT        		LogicalClose;                      //逻辑控合
	UINT        		LogicalOpen;                       //逻辑控分
	INT         		ulYXID32;                          //在遥信表中的ID
	SYSTEMTIME  		selecttime;                        //选择时间
	SYSTEMTIME  		exectime;                          //执行时间
	INT         		iZF;                               //是否转发
	UINT        		izfTuID;                           //转发目的通道编号
	UINT        		izfDPUID;                          //转发设备地址
	UINT        		izfNoInDPU;                        //转发点号
	INT         		izfProcessID;                      //转发进程ID
	INT         		iType;                             //类型
	INT         		lFlag;                             //标志
	INT         		iDataType;                         //数据类型
	INT         		iIndex;                            //在厂站中的序号
	INT         		uAlarmClass;                       //报警等级
	UINT        		iexecret;                          //执行结果
	INT         		iMaxSelectTime;                    //选择最大时延(s)
	INT         		iMaxExecuteTime;                   //执行最大时延(s)
	INT         		iCancelMaxTime;                    //撤销最大时延(s)
}SCADA_Relay;		//遥控表

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	UINT        		uTuID;                             //采集单元ID
	CHAR        		strID[32];                         //名称
	CHAR        		strName[80];                       //描述
	FLOAT       		fVal;                              //工程值
	INT         		lDPUID;                            //所属DPU
	INT         		iNoInDPU;                          //DPU中编号
	SYSTEMTIME  		selecttime;                        //选择时间
	SYSTEMTIME  		exectime;                          //执行时间
	FLOAT       		fFactor;                           //遥调系数
	INT         		ulYCID32;                          //在遥测表中的ID
	INT         		iIndex;                            //在厂站中的序号
	INT         		iZF;                               //是否转发
	UINT        		izfTuID;                           //转发目的通道编号
	UINT        		izfDPUID;                          //转发设备地址
	UINT        		izfNoInDPU;                        //转发点号
	INT         		izfProcessID;                      //转发进程ID
	INT         		lFlag;                             //标志
	INT         		iDataType;                         //数据类型
	UINT        		iexecret;                          //执行结果
}SCADA_SetPoint;		//遥调表

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[24];                       //名称
	CHAR        		strDLLName[48];                    //规约库名
	CHAR        		strClassName[48];                  //规约类名
	INT         		iProtocolType;                     //规约类型
	INT         		iProtocolSubType;                  //规约子类型
	CHAR        		strTemplate[48];                   //规约模板文件
}SCADA_Protocol;		//规约表

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	SYSTEMTIME  		tTime;                             //时标量
	INT         		ulDayNormal;                       //日正常累计时间(秒)
	INT         		ulDayHigh;                         //日越上限累计时间(秒)
	INT         		ulDayLow;                          //日越下限累计时间(秒)
	INT         		ulDayStop;                         //日停运累计时间(秒)
	FLOAT       		fDayHiRatio;                       //日越上限率
	FLOAT       		fDayLowRatio;                      //日越下限率
	FLOAT       		fDayNorRatio;                      //日合格率
	INT         		ulMonthNormal;                     //月正常累计时间(秒)
	INT         		ulMonthHigh;                       //月越上限累计时间(秒)
	INT         		ulMonthLow;                        //月越下限累计时间(秒)
	INT         		ulMonthStop;                       //月停运累计时间(秒)
	FLOAT       		fMonthHiRatio;                     //月越上限率
	FLOAT       		fMonthLowRatio;                    //月越下限率
	FLOAT       		fMonthNorRatio;                    //月合格率
	INT         		ulYearNormal;                      //年正常累计时间(秒)
	INT         		ulYearHigh;                        //年越上限累计时间(秒)
	INT         		ulYearLow;                         //年越下限累计时间(秒)
	INT         		ulYearStop;                        //年停运累计时间(秒)
	FLOAT       		fYearHiRatio;                      //年越上限率
	FLOAT       		fYearLowRatio;                     //年越下限率
	FLOAT       		fYearNorRatio;                     //年合格率
	INT         		iHighL;                            //日越上限次数
	INT         		iLowL;                             //日越下限次数
	INT         		iStatTime;                         //统计间隔
	INT         		iOverMaxNum;                       //最大越限次数
	INT         		iTimeSpan;                         //累计时间(秒)
	INT         		iOverNum;                          //越限次数
	INT         		iOverMaxTime;                      //最长越限时间(秒)
	INT         		iOverTime;                         //越限时间(秒)
	INT         		iOverNumClass;                     //计次越限报警等级
	INT         		iOverTimeClass;                    //计时越限报警等级
	INT         		iDelayMax;                         //延时报警时限
	INT         		iDelayTime;                        //延时累计
	FLOAT       		fhourMaxV;                         //小时最大
	FLOAT       		fhourMinV;                         //小时最小
	FLOAT       		fhourAveV;                         //小时平均
}SCADA_AnalogStat;		//遥测统计表

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	SYSTEMTIME  		tTime;                             //时标量
	SYSTEMTIME  		tStartTime;                        //统计开始时间
	INT         		ulDayNormal;                       //日运行时间累加(秒)
	INT         		ulDayStop;                         //日停运时间累加(秒)
	FLOAT       		fDayNorRatio;                      //日运行时间百分数
	INT         		ulMonthNormal;                     //月运行时间累加(秒)
	INT         		ulMonthStop;                       //月停运时间累加(秒)
	FLOAT       		fMonthNorRatio;                    //月运行时间百分数
	INT         		ulYearNormal;                      //年运行时间累加(秒)
	INT         		ulYearStop;                        //年停运时间累加(秒)
	FLOAT       		fYearNorRatio;                     //年运行时间百分数
	INT         		iOCTime;                           //统计间隔(秒)
	INT         		iOCMaxNum;                         //间隔内最大启动次数
	INT         		iTimeSpan;                         //累计时间(秒)
	INT         		iOCNum;                            //设备启动次数
	INT         		iMaxRunTime;                       //最长运行时间(秒)
	INT         		iRunTime;                          //运行时间(秒)
	INT         		iOverNumClass;                     //计次越限报警等级
	INT         		iOverTimeClass;                    //计时越限报警等级
	INT         		iDelayMax;                         //延时报警时限
	INT         		iDelayTime;                        //延时累计
}SCADA_DigitStat;		//遥信统计表

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	INT         		iNetHeartPeriod;                   //网络判死(秒)
	INT         		iNodeHeartPeriod;                  //节点判死(秒)
	INT         		iProcessHeartPeriod;               //进程判死(秒)
	INT         		iLogoutTime;                       //无操作注销最长时间(m)
	INT         		iFormulaPeriod;                    //无操作注销最长时间(m)
}SCADA_Global;		//SCADA全局变量表

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[40];                       //名称
	UINT        		iValID;                            //结果值ID
	CHAR        		strExpression[240];                //公式表达式
	INT         		lFlag;                             //结果值类型
}SCADA_Formula;		//计算公式表


#endif   //__DATABASESCADA_h__
