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
	eModXmlDataTypeNull=0,//δ֪
	eModXmlDataTypeInt=1,
	eModXmlDataTypeFloat=2,
	eModXmlDataTypeEnum=3,
	eModXmlDataTypeString=4
}Q_ModXmlDataType;
typedef struct ST_ModCfgT
{
	UINT group; //����
	UINT id;    //���
	char name[50]; //������
	Q_ModXmlDataType type; // Int,Float,ENUM,String
	char value[100]; //ֵ
}ST_ModCfgT;

typedef struct _ST_ModCmdCfg
{
	int id;
	int nBegin;//�Ĵ�������
	int nCount;
	int nType;//Ĭ�Ͼ���FLOAT
	int device;//�豸��
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
	BYTE LSB:1;//���λ
	BYTE v2:1;
	BYTE v3:1;
	BYTE v4:1;
	BYTE v5:1;
	BYTE v6:1;
	BYTE v7:1;
	BYTE MSB:1;//���λ
}Q_Mod_DVAL;

typedef struct MOD_PLAN_CURVE
{
	int  val; //��Ӧ��ֵ
	bool exec;//�Ƿ�ִ��trueΪִ��,falseΪselect
	int	 id;
}MOD_PLAN_CURVE,*LPMOD_PLAN_CURVE;




#endif//__Q_MODBUSTCPSERVERSTANDARDDEFINE_H_INCLUDE__

