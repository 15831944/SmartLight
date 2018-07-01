#ifndef __DATABASESYSTEM_h__
#define __DATABASESYSTEM_h__

#include "Rtdb_Struct.h"
#define DB_ID_OF_SYSTEM                             1
//各表ID
#define SYSTEM_TAB_NetNode_ID                     	101	//节点表
#define SYSTEM_TAB_Process_ID                     	102	//进程表
#define SYSTEM_TAB_User_ID                        	103	//用户表
#define SYSTEM_TAB_UserGroup_ID                   	104	//用户组表
#define SYSTEM_TAB_Formula_ID                     	105	//公式表
#define SYSTEM_TAB_Global_ID                      	106	//全局变量表
#define SYSTEM_TAB_AppConfig_ID                   	107	//应用配置信息
//各表在数据库中的索引
#define SYSTEM_TAB_NetNode_INDEX                  	0	//节点表
#define SYSTEM_TAB_Process_INDEX                  	1	//进程表
#define SYSTEM_TAB_User_INDEX                     	2	//用户表
#define SYSTEM_TAB_UserGroup_INDEX                	3	//用户组表
#define SYSTEM_TAB_Formula_INDEX                  	4	//公式表
#define SYSTEM_TAB_Global_INDEX                   	5	//全局变量表
#define SYSTEM_TAB_AppConfig_INDEX                	6	//应用配置信息

//定义各字段在表中索引(节点表)
#define SYSTEM_TAB_NetNode_FIELD_ID32_INDEX                 	0	//内部ID32
#define SYSTEM_TAB_NetNode_FIELD_strName_INDEX              	1	//名称
#define SYSTEM_TAB_NetNode_FIELD_strDescription_INDEX       	2	//描述
#define SYSTEM_TAB_NetNode_FIELD_strIP1_INDEX               	3	//1网IP地址
#define SYSTEM_TAB_NetNode_FIELD_strIP2_INDEX               	4	//2网IP地址
#define SYSTEM_TAB_NetNode_FIELD_iType_INDEX                	5	//类型
#define SYSTEM_TAB_NetNode_FIELD_iAllowProcess_INDEX        	6	//许可进程
#define SYSTEM_TAB_NetNode_FIELD_iActiveNet_INDEX           	7	//活动网段
#define SYSTEM_TAB_NetNode_FIELD_tStartTime_INDEX           	8	//启动时间
#define SYSTEM_TAB_NetNode_FIELD_tActiveTime_INDEX          	9	//活动时间
#define SYSTEM_TAB_NetNode_FIELD_iStatus_INDEX              	10	//运行状态
#define SYSTEM_TAB_NetNode_FIELD_uLoginUserID_INDEX         	11	//当前用户
#define SYSTEM_TAB_NetNode_FIELD_tCheckRightTime_INDEX      	12	//最近一次操作时间
#define SYSTEM_TAB_NetNode_FIELD_uMaxMsgID_INDEX            	13	//本机消息最大ID
#define SYSTEM_TAB_NetNode_FIELD_AOR_INDEX                  	14	//责任区域
#define SYSTEM_TAB_NetNode_FIELD_iCPU_INDEX                 	15	//CPU使用情况
#define SYSTEM_TAB_NetNode_FIELD_iDisk_INDEX                	16	//磁盘使用情况
#define SYSTEM_TAB_NetNode_FIELD_iMemory_INDEX              	17	//内存使用情况
#define SYSTEM_TAB_NetNode_FIELD_lWatchNode1_INDEX          	18	//监护节点1
#define SYSTEM_TAB_NetNode_FIELD_lWatchNode2_INDEX          	19	//监护节点2
#define SYSTEM_TAB_NetNode_FIELD_iHeart_INDEX               	20	//节点心跳

//定义各字段在表中索引(进程表)
#define SYSTEM_TAB_Process_FIELD_ID32_INDEX                 	0	//内部ID32
#define SYSTEM_TAB_Process_FIELD_iProcessID_INDEX           	1	//进程ID
#define SYSTEM_TAB_Process_FIELD_iType_INDEX                	2	//进程类型
#define SYSTEM_TAB_Process_FIELD_uNodeID_INDEX              	3	//所属节点ID
#define SYSTEM_TAB_Process_FIELD_iMainWnd_INDEX             	4	//主窗口句柄
#define SYSTEM_TAB_Process_FIELD_iFlag_INDEX                	5	//标志
#define SYSTEM_TAB_Process_FIELD_tStartTime_INDEX           	6	//启动时间
#define SYSTEM_TAB_Process_FIELD_tActiveTime_INDEX          	7	//最新时间
#define SYSTEM_TAB_Process_FIELD_iHeart_INDEX               	8	//进程心跳

//定义各字段在表中索引(用户表)
#define SYSTEM_TAB_User_FIELD_ID32_INDEX                    	0	//内部ID32
#define SYSTEM_TAB_User_FIELD_strName_INDEX                 	1	//用户名
#define SYSTEM_TAB_User_FIELD_strPasswd_INDEX               	2	//密码
#define SYSTEM_TAB_User_FIELD_iGroupFlag_INDEX              	3	//用户组标志
#define SYSTEM_TAB_User_FIELD_strCard_INDEX                 	4	//卡号
#define SYSTEM_TAB_User_FIELD_strTelephone_INDEX            	5	//联系电话
#define SYSTEM_TAB_User_FIELD_nRuningUserGroup_INDEX        	6	//运行时用户组
#define SYSTEM_TAB_User_FIELD_nRuningUser_INDEX             	7	//运行时用户

//定义各字段在表中索引(用户组表)
#define SYSTEM_TAB_UserGroup_FIELD_ID32_INDEX               	0	//内部ID32
#define SYSTEM_TAB_UserGroup_FIELD_strName_INDEX            	1	//用户组名
#define SYSTEM_TAB_UserGroup_FIELD_iRightsFlag1_INDEX       	2	//权限标志1
#define SYSTEM_TAB_UserGroup_FIELD_iRightsFlag2_INDEX       	3	//权限标志2
#define SYSTEM_TAB_UserGroup_FIELD_iRightsFlag3_INDEX       	4	//权限标志3
#define SYSTEM_TAB_UserGroup_FIELD_iRightsFlag4_INDEX       	5	//权限标志4

//定义各字段在表中索引(公式表)
#define SYSTEM_TAB_Formula_FIELD_ID32_INDEX                 	0	//内部ID32
#define SYSTEM_TAB_Formula_FIELD_strName_INDEX              	1	//名称
#define SYSTEM_TAB_Formula_FIELD_strExpression_INDEX        	2	//公式表达式
#define SYSTEM_TAB_Formula_FIELD_iVariableNumber_INDEX      	3	//变量个数
#define SYSTEM_TAB_Formula_FIELD_strVariableArray_INDEX     	4	//变量数组
#define SYSTEM_TAB_Formula_FIELD_nDataBaseID_INDEX          	5	//数据库ID
#define SYSTEM_TAB_Formula_FIELD_lFlag_INDEX                	6	//标志

//定义各字段在表中索引(全局变量表)
#define SYSTEM_TAB_Global_FIELD_ID32_INDEX                  	0	//内部ID32
#define SYSTEM_TAB_Global_FIELD_iNetHeartPeriod_INDEX       	1	//网络判死(秒)
#define SYSTEM_TAB_Global_FIELD_iNodeHeartPeriod_INDEX      	2	//节点判死(秒)
#define SYSTEM_TAB_Global_FIELD_iProcessHeartPeriod_INDEX   	3	//进程判死(秒)
#define SYSTEM_TAB_Global_FIELD_iLogoutTime_INDEX           	4	//无操作注销最长时间(m)
#define SYSTEM_TAB_Global_FIELD_iFormulaPeriod_INDEX        	5	//公式计算时间(m)

//定义各字段在表中索引(应用配置信息)
#define SYSTEM_TAB_AppConfig_FIELD_ID32_INDEX               	0	//内部ID32
#define SYSTEM_TAB_AppConfig_FIELD_iProcessType_INDEX       	1	//进程类型
#define SYSTEM_TAB_AppConfig_FIELD_strKey_INDEX             	2	//配置项
#define SYSTEM_TAB_AppConfig_FIELD_iValue_INDEX             	3	//整形值
#define SYSTEM_TAB_AppConfig_FIELD_strValue_INDEX           	4	//字符串值



typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[20];                       //名称
	CHAR        		strDescription[40];                //描述
	CHAR        		strIP1[20];                        //1网IP地址
	CHAR        		strIP2[20];                        //2网IP地址
	INT         		iType;                             //类型
	INT         		iAllowProcess;                     //许可进程
	INT         		iActiveNet;                        //活动网段
	SYSTEMTIME  		tStartTime;                        //启动时间
	SYSTEMTIME  		tActiveTime;                       //活动时间
	INT         		iStatus;                           //运行状态
	UINT        		uLoginUserID;                      //当前用户
	INT         		tCheckRightTime;                   //最近一次操作时间
	INT         		uMaxMsgID;                         //本机消息最大ID
	INT         		AOR;                               //责任区域
	INT         		iCPU;                              //CPU使用情况
	INT         		iDisk;                             //磁盘使用情况
	INT         		iMemory;                           //内存使用情况
	INT         		lWatchNode1;                       //监护节点1
	INT         		lWatchNode2;                       //监护节点2
	INT         		iHeart;                            //节点心跳
}SYSTEM_NetNode;		//节点表

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	INT         		iProcessID;                        //进程ID
	INT         		iType;                             //进程类型
	UINT        		uNodeID;                           //所属节点ID
	INT         		iMainWnd;                          //主窗口句柄
	INT         		iFlag;                             //标志
	SYSTEMTIME  		tStartTime;                        //启动时间
	SYSTEMTIME  		tActiveTime;                       //最新时间
	INT         		iHeart;                            //进程心跳
}SYSTEM_Process;		//进程表

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[16];                       //用户名
	CHAR        		strPasswd[40];                     //密码
	UINT        		iGroupFlag;                        //用户组标志
	CHAR        		strCard[20];                       //卡号
	CHAR        		strTelephone[20];                  //联系电话
	UINT        		nRuningUserGroup;                  //运行时用户组
	UINT        		nRuningUser;                       //运行时用户
}SYSTEM_User;		//用户表

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[16];                       //用户组名
	INT         		iRightsFlag1;                      //权限标志1
	INT         		iRightsFlag2;                      //权限标志2
	INT         		iRightsFlag3;                      //权限标志3
	INT         		iRightsFlag4;                      //权限标志4
}SYSTEM_UserGroup;		//用户组表

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	CHAR        		strName[40];                       //名称
	CHAR        		strExpression[240];                //公式表达式
	INT         		iVariableNumber;                   //变量个数
	CHAR        		strVariableArray[600];             //变量数组
	UINT        		nDataBaseID;                       //数据库ID
	UINT        		lFlag;                             //标志
}SYSTEM_Formula;		//公式表

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	INT         		iNetHeartPeriod;                   //网络判死(秒)
	INT         		iNodeHeartPeriod;                  //节点判死(秒)
	INT         		iProcessHeartPeriod;               //进程判死(秒)
	INT         		iLogoutTime;                       //无操作注销最长时间(m)
	INT         		iFormulaPeriod;                    //公式计算时间(m)
}SYSTEM_Global;		//全局变量表

typedef struct 
{
	UINT        		ID32;                              //内部ID32
	INT         		iProcessType;                      //进程类型
	CHAR        		strKey[32];                        //配置项
	INT         		iValue;                            //整形值
	CHAR        		strValue[100];                     //字符串值
}SYSTEM_AppConfig;		//应用配置信息


#endif   //__DATABASESYSTEM_h__
