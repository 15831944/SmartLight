#pragma once
//*
//*
//* Q_modbusTcpServerStandardDefine.h
//*
//*
#ifndef __Q_HttpDBQH_DEFINE_H_INCLUDE__
#define __Q_HttpDBQH_DEFINE_H_INCLUDE__

#include "Q_frameInclude.h"
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




#endif//__Q_MODBUSTCPSERVERSTANDARDDEFINE_H_INCLUDE__

