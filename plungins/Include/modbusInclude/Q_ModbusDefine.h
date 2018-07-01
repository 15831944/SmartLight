#pragma once
//*
//*
//* Q_modbusTcpServerStandardDefine.h
//*
//*
#ifndef __Q_MODBUS_DEFINE_H_INCLUDE__
#define __Q_MODBUS_DEFINE_H_INCLUDE__


/** @brief Add include file. */
#include "Q_frameInclude.h"
#define MODBUS_HEADER_LEN 6
const int MODBUS_FRAMELEN_MIN = 9 /*12*/;

#define MODBUS_SYNC_CODE1  0x00
#define MODBUS_SYNC_CODE2  0x00
#define MODBUS_SYNC_CODE_H  0x15  //四方的比较特殊，一方都是0x00
#define MODBUS_SYNC_CODE_STD_H 0x00 //标准的
#define MODBUS_YX   0x01  //遥信
#define MODBUS_YC   0x03  //遥测
#define MODBUS_YK   0x05  //遥控
#define MODBUS_YT   0x06  //遥调
#define MODBUS_YT_M   0x10  //遥调多个字节的遥调
#define MODBUS_YT_M_03 0x03 //float功能码03，非标准
#define MODBUS_MAX_PACK_COUNT 257
#define MODBUS_MAX_DATA_COUNT 255

#define MODBUS_ERROR_CODE_1 0x01 //不合法功能码
#define MODBUS_ERROR_CODE_2 0x02 //不合法数据地址
#define MODBUS_ERROR_CODE_3 0x03 //不合法数据
#define MODBUS_ERROR_CODE_4 0x04 //从机设备故障
#define MODBUS_ERROR_CODE_6 0x06 //从机设备忙碌

#define MODBUS_COMMAND_ASDU 0x01 //无数据格式帧
#define MODBUS_DATA_ASDU    0x02 //带数据格式帧

typedef struct _Q_MODBUS_HEADER
{
	BYTE codeHigh;//编号高字节
	BYTE codeLow;//编号低字节
	BYTE codeFlgHigh;//识别码高字节，此处为零
	BYTE codeFlgLow;//识别码低字节，此处为零
	BYTE lenHigh;//长度高字节
	BYTE lenLow;//长度低字节
}Q_MODBUS_HEADER;


typedef struct _Q_MODBUS_REC_YC
{
	BYTE startAddrHigh;//开始地址高位
	BYTE startAddrLow;//开始地址低位
	BYTE regCountHigh;//寄存器地址高位
	BYTE regCountLow;//寄存器地址低位
}Q_MODBUS_REC_YC;


typedef struct _Q_MODBUS_ASDU
{
	Q_MODBUS_HEADER header;
	BYTE addr;
	BYTE funcode;
}Q_MODBUS_ASDU;

typedef struct _Q_MODBUS_ASDU2
{
	Q_MODBUS_HEADER header;
	BYTE addr;
	BYTE funcode;
	BYTE data[MODBUS_MAX_DATA_COUNT];
}Q_MODBUS_ASDU2;
//tanjing sheng 2012-8-1
typedef struct _Q_MODBUS_ASDU_RTU
{
	BYTE addr;
	BYTE funcode;
	BYTE data[254];
}Q_MODBUS_ASDU_RTU;

//jwk 2012-8-10
typedef struct _Q_MODBUS_ASDU_YH
{
    BYTE addr;
    BYTE funcode;
    BYTE datanum;
    BYTE pad;
    BYTE data[252];
}Q_MODBUS_ASDU_YH;

typedef enum _Q_ModXmlDataType
{
	eModXmlDataTypeNull=0,//未知
	eModXmlDataTypeInt=1,
	eModXmlDataTypeFloat=2,
	eModXmlDataTypeEnum=3,
	eModXmlDataTypeString=4
}Q_ModXmlDataType;

typedef struct ST_ModCfgT
{
	UINT group; //分组
	UINT id;    //编号
	char name[50]; //配置名
	Q_ModXmlDataType type; // Int,Float,ENUM,String
	char value[100]; //值
}ST_ModCfgT;

typedef struct _ST_ModCmdCfg
{
	int id;
	int nBegin;//寄存器个数
	int nCount;
	int nType;//默认就是FLOAT
	int device;//设备号
	int code;//2017-1-10

}ST_ModCmdCfg;

typedef struct _ST_ModCmdYXCfg
{
	int id;
	int nBegin;
	int nCount;
	int nType;
	int device;
	int code;//2017-1-10
}ST_ModCmdYXCfg;
typedef struct Mod_DVAL
{
	BYTE LSB:1;//最低位
	BYTE v2:1;
	BYTE v3:1;
	BYTE v4:1;
	BYTE v5:1;
	BYTE v6:1;
	BYTE v7:1;
	BYTE MSB:1;//最高位
}Q_Mod_DVAL;

typedef struct MOD_PLAN_CURVE
{
	int  val; //对应的值
	bool exec;//是否执行true为执行,false为select
	int	 id;
}MOD_PLAN_CURVE,*LPMOD_PLAN_CURVE;


typedef enum _Q_Modbus_CommandType
{
	eModbusCommandTypeNull						= 0,
	eModbusCommandRemoteControl					= 1,//遥控
	eModbusCommandRemoteAdjust					= 2,//遥调
	eModbusCommandMeasure					    = 3,//遥测
	eModbusCommandPoint                         = 4,//遥信
                                                    //jwk add 2012-8-10
    eYH_YC_MX                                   = 5,//银湖遥测母线参数
    eYH_YC_YW                                   = 6,//银湖遥测油温
    eYH_YC_MCR                                  = 7,//银湖遥测MCR参数
    eYH_YC_OTHER                                = 8,//银湖遥测其他参数
    eYH_YX                                      = 9,//银湖遥信
    eYH_YK                                      = 10,//银湖遥控
    eYH_YT                                      = 11//银湖遥调
}Q_Modbus_CommandType;
/** @brief cdt command status. */
typedef enum _Q_Modbus_CommandStatus
{
	eModbusCommandStatusNull						= 0,
	eModbusCommandSelect							= 1,
	eModbusCommandSelectBack						= 2,
	eModbusCommandAct								= 3,
	eModbusCommandCancel							= 4,//确认返回
	eModbusCommandActCon							= 5,  
	eModbusCommandActError							= 6  

}Q_Modbus_CommandStatus;
typedef enum _ModbusAnalogUpType
{
	eModAnalogUpType4=1,//四个字节从低到高
	eModAnalogUpType2=2 //两字节个字节从高到低
}ModbusAnalogUpType;
//浮点型字符上送顺序
typedef enum ModbusFloatUpSeq
{
	eFloatUpSeqLowFirst=1,//四个字节从低到高
	eFloatUpSeqHighFirst=2,//四个字节从高到低
	eFloatUpSeq2301=3,  //两个寄存器之间,低字节在前,高字节在后；寄存器内部是高在前、低在后
	eFloatUpSeq1032=4    //两个寄存器之间，是高字节在前，低字节在后，寄存器内部高在前，低在后
}ModbusFloatUpSeq;
typedef enum ModbusIntUpSeq
{
	eIntUpSeqHighFirst=1,//高在前、低在后
	eIntUpSeqLowFirst=2,//低在前、高在后
	eIntUpSeq2301=3,  //两个寄存器之间,低字节在前,高字节在后；寄存器内部是高在前、低在后
	eIntUpSeq1032=4    //两个寄存器之间，是高字节在前，低字节在后，寄存器内部高在前，低在后
	
}ModbusIntUpSeq;

typedef enum ModbusCheckCodSeq
{
	eCheckCodeSeqHighFirst=1,//高在前、低在后
	eCheckCodeSeqLowFirst=2,//低在前、高在后
	
}ModbusCheckCodSeq;

typedef enum ModbusYTMethod
{
	eYTMethodFloatFun10=1,//四字节浮点型功能码为0x10
	eYTMethodFloatFun06=2, //四字节浮点型功能码为0x06
	eYTMethodIntFun10=3, //jwk add 2012-8-1 无功调节遥调功能码0x20
    eYTMethodFloatFun20=4, //jwk add 2012-8-1 无功调节遥调功能码0x20
	eYTMethodIntFun06=5, //整形06功能码
	eYTMethodFloatFun03=6,//四字节浮点型功能码为0x03（山西给金峰用非标准）
	eYTMethodIntFun06_2Bytes=7, //2字节int类型
	eYTMethodDouble8=8,         //8字节double类型遥调
	eYTMethodInt_4Bytes=9, //2字节int类型
	eYTMethodTypeUShort=10
}ModbusYTMethod;

typedef enum NewModbusYTMethod
{
	eYtFloat=1,
	eYtInt2=2,
	eYtInt4=3
}NewModbusYTMethod;

//2015-04-15统一更改　在Q_frameDefine.h中定义
//tjs 
//typedef enum Q_MODBUS_VarType
//{
//	eModVarTypeNull=0,//未知类型
//	eModVarTypeYc=1,//遥测变化
//	eModVarTypeYx=2,//遥信变化
//	eModVarTypeEx=3 //其它的值变化
//}Q_MODBUS_VarType;
//
////用于触发外部执行命令用
//typedef struct Q_MODBUS_ExecCmd
//{
//	int funcode;// 功能码
//	Q_MODBUS_VarType varType;//变化的数据类型
//	int m_vindex;//变化的索引,默认是从零开始的
//	bool bzf;
//	int zfchannelid;//转发通道id
//	int zfdevice;//转发站编号
//	int zfno; //转发站内序号
//	Q_RECORDDATATAG m_tag;//更新数据
//	Q_MODBUS_ExecCmd()
//	{
//		bzf=false;
//		zfchannelid=0;
//		zfdevice=0;
//		zfno=0;
//		funcode=0;
//		varType=eModVarTypeNull;
//		m_vindex=0;
//	}
//}Q_MODBUS_ExecCmd;
typedef Q_Protocol_VarType Q_MODBUS_VarType;
typedef Q_Protocol_ExecCmd Q_MODBUS_ExecCmd;


typedef enum _Q_eExplainYcType
{
	eExplainYcTypeNull=0,
	eExplainYcTypeFloat=1,//未知
	eExplainYcTypeInt=2,//两字节整形，有符号
	eExplainYcTypeMixed=3,//混合类型，可以在遥测表里边指定数据类型
	eExplainYcTypeDouble8=4,//双精度浮点数
	eExplainYcTypeIntHuaWei=7,//华为数据，无数据字节个数
	eExplainYcTypeInt4=8,//四字节整形，有符号
	eExplainYcTypeUnsignedInt4=9,//四字节整形，无符号
	eExplainYcTypeUShort=10 //两字节整形无符号
}Q_eExplainYcType;

#endif//__Q_MODBUSTCPSERVERSTANDARDDEFINE_H_INCLUDE__

