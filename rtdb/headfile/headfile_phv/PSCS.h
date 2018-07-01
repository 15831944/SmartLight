#ifndef __DATABASEPSCS_h__
#define __DATABASEPSCS_h__

#include "Rtdb_Struct.h"
#define DB_ID_OF_PSCS                               41
//各表ID
#define PSCS_TAB_LineInfo_ID                      	4101	//线路
#define PSCS_TAB_Parameter_ID                     	4102	//PSCS调节参数
#define PSCS_TAB_Analog_ID                        	4103	//PSCS参与计算遥测
#define PSCS_TAB_Digit_ID                         	4104	//PSCS参与计算遥信
#define PSCS_TAB_Setting_ID                       	4105	//PSCS参与定值参数
#define PSCS_TAB_Alarm_ID                         	4106	//PSCS报警
#define PSCS_TAB_LogRecord_ID                     	4107	//PSCS动作日志
//各表在数据库中的索引
#define PSCS_TAB_LineInfo_INDEX                   	0	//线路
#define PSCS_TAB_Parameter_INDEX                  	1	//PSCS调节参数
#define PSCS_TAB_Analog_INDEX                     	2	//PSCS参与计算遥测
#define PSCS_TAB_Digit_INDEX                      	3	//PSCS参与计算遥信
#define PSCS_TAB_Setting_INDEX                    	4	//PSCS参与定值参数
#define PSCS_TAB_Alarm_INDEX                      	5	//PSCS报警
#define PSCS_TAB_LogRecord_INDEX                  	6	//PSCS动作日志

//定义各字段在表中索引(线路)
#define PSCS_TAB_LineInfo_FIELD_ID32_INDEX                  	0	//内部ID32
#define PSCS_TAB_LineInfo_FIELD_strName_INDEX               	1	//线路名称
#define PSCS_TAB_LineInfo_FIELD_fValP_INDEX                 	2	//有功
#define PSCS_TAB_LineInfo_FIELD_iStatus_INDEX               	3	//线路开关状态
#define PSCS_TAB_LineInfo_FIELD_iIsCut_INDEX                	4	//当前是否被切除
#define PSCS_TAB_LineInfo_FIELD_iRefuseAlarm_INDEX          	5	//拒绝动作报警
#define PSCS_TAB_LineInfo_FIELD_tSnapTime_INDEX             	6	//时标
#define PSCS_TAB_LineInfo_FIELD_tCutTime_INDEX              	7	//切除时间
#define PSCS_TAB_LineInfo_FIELD_fValUab_INDEX               	8	//线电压Uab
#define PSCS_TAB_LineInfo_FIELD_fValIa_INDEX                	9	//A相电流Ia
#define PSCS_TAB_LineInfo_FIELD_fCutP_INDEX                 	10	//当前被切功率
#define PSCS_TAB_LineInfo_FIELD_iType_INDEX                 	11	//机组类型
#define PSCS_TAB_LineInfo_FIELD_iChannelStatus_INDEX        	12	//通道状态
#define PSCS_TAB_LineInfo_FIELD_iFlag_INDEX                 	13	//标识
#define PSCS_TAB_LineInfo_FIELD_iPowerAlgID_INDEX           	14	//有功对应遥测ID
#define PSCS_TAB_LineInfo_FIELD_iIaAlgID_INDEX              	15	//A相电流对应遥测ID
#define PSCS_TAB_LineInfo_FIELD_iCutPAlgID_INDEX            	16	//被切除功率遥测ID
#define PSCS_TAB_LineInfo_FIELD_fCapictiyP_INDEX            	17	//线路额定功率
#define PSCS_TAB_LineInfo_FIELD_fUabAlgID_INDEX             	18	//线电压Uab遥测ID
#define PSCS_TAB_LineInfo_FIELD_fJustOnIa_INDEX             	19	//判投运电流定值(A)
#define PSCS_TAB_LineInfo_FIELD_iLogicDigitID_INDEX         	20	//允切压板遥信ID
#define PSCS_TAB_LineInfo_FIELD_iCutDigitID_INDEX           	21	//被切机遥信ID
#define PSCS_TAB_LineInfo_FIELD_iRefuseAlarmDigitID_INDEX   	22	//拒绝动作报警ID
#define PSCS_TAB_LineInfo_FIELD_iStatusDigitID_INDEX        	23	//开关位置遥信ID
#define PSCS_TAB_LineInfo_FIELD_iChannelDigitID_INDEX       	24	//通道对应遥信ID
#define PSCS_TAB_LineInfo_FIELD_iChannelID_INDEX            	25	//通道号
#define PSCS_TAB_LineInfo_FIELD_iDevice_INDEX               	26	//设备编号
#define PSCS_TAB_LineInfo_FIELD_iYkNo_INDEX                 	27	//遥控点号
#define PSCS_TAB_LineInfo_FIELD_iOffCode_INDEX              	28	//断开开关码值(10进制)
#define PSCS_TAB_LineInfo_FIELD_iOnCode_INDEX               	29	//闭合开关码值(10进制)
#define PSCS_TAB_LineInfo_FIELD_fValQ_INDEX                 	30	//无功
#define PSCS_TAB_LineInfo_FIELD_iQAlgID_INDEX               	31	//无功对应遥测ID
#define PSCS_TAB_LineInfo_FIELD_iLogicYBVal_INDEX           	32	//允切压板值

//定义各字段在表中索引(PSCS调节参数)
#define PSCS_TAB_Parameter_FIELD_ID32_INDEX                 	0	//内部ID32
#define PSCS_TAB_Parameter_FIELD_strName_INDEX              	1	//定值名称
#define PSCS_TAB_Parameter_FIELD_strKey_INDEX               	2	//关键字
#define PSCS_TAB_Parameter_FIELD_iType_INDEX                	3	//定值类型
#define PSCS_TAB_Parameter_FIELD_iVal_INDEX                 	4	//整形定值

//定义各字段在表中索引(PSCS参与计算遥测)
#define PSCS_TAB_Analog_FIELD_ID32_INDEX                    	0	//内部ID32
#define PSCS_TAB_Analog_FIELD_strName_INDEX                 	1	//名称
#define PSCS_TAB_Analog_FIELD_strKey_INDEX                  	2	//关键字
#define PSCS_TAB_Analog_FIELD_iType_INDEX                   	3	//数据类型
#define PSCS_TAB_Analog_FIELD_fVal_INDEX                    	4	//值
#define PSCS_TAB_Analog_FIELD_iAnalogID_INDEX               	5	//遥测ID
#define PSCS_TAB_Analog_FIELD_tSnapTime_INDEX               	6	//时标量

//定义各字段在表中索引(PSCS参与计算遥信)
#define PSCS_TAB_Digit_FIELD_ID32_INDEX                     	0	//内部ID32
#define PSCS_TAB_Digit_FIELD_strName_INDEX                  	1	//定值名称
#define PSCS_TAB_Digit_FIELD_strKey_INDEX                   	2	//关键字
#define PSCS_TAB_Digit_FIELD_iType_INDEX                    	3	//数据类型
#define PSCS_TAB_Digit_FIELD_iVal_INDEX                     	4	//值
#define PSCS_TAB_Digit_FIELD_iDigitID_INDEX                 	5	//遥信ID
#define PSCS_TAB_Digit_FIELD_tSnapTime_INDEX                	6	//时标量

//定义各字段在表中索引(PSCS参与定值参数)
#define PSCS_TAB_Setting_FIELD_ID32_INDEX                   	0	//内部ID32
#define PSCS_TAB_Setting_FIELD_strName_INDEX                	1	//定值名称
#define PSCS_TAB_Setting_FIELD_strKey_INDEX                 	2	//关键字
#define PSCS_TAB_Setting_FIELD_iType_INDEX                  	3	//数据类型
#define PSCS_TAB_Setting_FIELD_fVal_INDEX                   	4	//值
#define PSCS_TAB_Setting_FIELD_iAnalogID_INDEX              	5	//遥测ID
#define PSCS_TAB_Setting_FIELD_tSnapTime_INDEX              	6	//时标量

//定义各字段在表中索引(PSCS报警)
#define PSCS_TAB_Alarm_FIELD_ID32_INDEX                     	0	//内部ID32
#define PSCS_TAB_Alarm_FIELD_strName_INDEX                  	1	//定值名称
#define PSCS_TAB_Alarm_FIELD_strKey_INDEX                   	2	//关键字
#define PSCS_TAB_Alarm_FIELD_iVal_INDEX                     	3	//值
#define PSCS_TAB_Alarm_FIELD_iDigitID_INDEX                 	4	//遥信ID
#define PSCS_TAB_Alarm_FIELD_tSnapTime_INDEX                	5	//时标量

//定义各字段在表中索引(PSCS动作日志)
#define PSCS_TAB_LogRecord_FIELD_ID32_INDEX                 	0	//内部ID32
#define PSCS_TAB_LogRecord_FIELD_tSnapTime_INDEX            	1	//时标量
#define PSCS_TAB_LogRecord_FIELD_strText_INDEX              	2	//内容



typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //线路名称
	FLOAT       		fValP;                             //有功
	UINT        		iStatus;                           //线路开关状态
	UINT        		iIsCut;                            //当前是否被切除
	UINT        		iRefuseAlarm;                      //拒绝动作报警
	SYSTEMTIME  		tSnapTime;                         //时标
	SYSTEMTIME  		tCutTime;                          //切除时间
	FLOAT       		fValUab;                           //线电压Uab
	FLOAT       		fValIa;                            //A相电流Ia
	FLOAT       		fCutP;                             //当前被切功率
	UINT        		iType;                             //机组类型
	INT         		iChannelStatus;                    //通道状态
	UINT        		iFlag;                             //标识
	INT         		iPowerAlgID;                       //有功对应遥测ID
	INT         		iIaAlgID;                          //A相电流对应遥测ID
	INT         		iCutPAlgID;                        //被切除功率遥测ID
	FLOAT       		fCapictiyP;                        //线路额定功率
	INT         		fUabAlgID;                         //线电压Uab遥测ID
	FLOAT       		fJustOnIa;                         //判投运电流定值(A)
	INT         		iLogicDigitID;                     //允切压板遥信ID
	INT         		iCutDigitID;                       //被切机遥信ID
	INT         		iRefuseAlarmDigitID;               //拒绝动作报警ID
	INT         		iStatusDigitID;                    //开关位置遥信ID
	INT         		iChannelDigitID;                   //通道对应遥信ID
	INT         		iChannelID;                        //通道号
	INT         		iDevice;                           //设备编号
	INT         		iYkNo;                             //遥控点号
	INT         		iOffCode;                          //断开开关码值(10进制)
	INT         		iOnCode;                           //闭合开关码值(10进制)
	FLOAT       		fValQ;                             //无功
	INT         		iQAlgID;                           //无功对应遥测ID
	UINT        		iLogicYBVal;                       //允切压板值
}PSCS_LineInfo;		//线路

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //定值名称
	CHAR        		strKey[48];                        //关键字
	INT         		iType;                             //定值类型
	INT         		iVal;                              //整形定值
}PSCS_Parameter;		//PSCS调节参数

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //名称
	CHAR        		strKey[48];                        //关键字
	INT         		iType;                             //数据类型
	FLOAT       		fVal;                              //值
	INT         		iAnalogID;                         //遥测ID
	SYSTEMTIME  		tSnapTime;                         //时标量
}PSCS_Analog;		//PSCS参与计算遥测

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //定值名称
	CHAR        		strKey[48];                        //关键字
	INT         		iType;                             //数据类型
	INT         		iVal;                              //值
	INT         		iDigitID;                          //遥信ID
	SYSTEMTIME  		tSnapTime;                         //时标量
}PSCS_Digit;		//PSCS参与计算遥信

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //定值名称
	CHAR        		strKey[48];                        //关键字
	INT         		iType;                             //数据类型
	FLOAT       		fVal;                              //值
	INT         		iAnalogID;                         //遥测ID
	SYSTEMTIME  		tSnapTime;                         //时标量
}PSCS_Setting;		//PSCS参与定值参数

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[96];                       //定值名称
	CHAR        		strKey[48];                        //关键字
	INT         		iVal;                              //值
	INT         		iDigitID;                          //遥信ID
	SYSTEMTIME  		tSnapTime;                         //时标量
}PSCS_Alarm;		//PSCS报警

typedef struct 
{
	INT         		ID32;                              //内部ID32
	SYSTEMTIME  		tSnapTime;                         //时标量
	CHAR        		strText[256];                      //内容
}PSCS_LogRecord;		//PSCS动作日志


#endif   //__DATABASEPSCS_h__
