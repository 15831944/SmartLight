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
#define MODBUS_SYNC_CODE_H  0x15  //�ķ��ıȽ����⣬һ������0x00
#define MODBUS_SYNC_CODE_STD_H 0x00 //��׼��
#define MODBUS_YX   0x01  //ң��
#define MODBUS_YC   0x03  //ң��
#define MODBUS_YK   0x05  //ң��
#define MODBUS_YT   0x06  //ң��
#define MODBUS_YT_M   0x10  //ң������ֽڵ�ң��
#define MODBUS_YT_M_03 0x03 //float������03���Ǳ�׼
#define MODBUS_MAX_PACK_COUNT 257
#define MODBUS_MAX_DATA_COUNT 255

#define MODBUS_ERROR_CODE_1 0x01 //���Ϸ�������
#define MODBUS_ERROR_CODE_2 0x02 //���Ϸ����ݵ�ַ
#define MODBUS_ERROR_CODE_3 0x03 //���Ϸ�����
#define MODBUS_ERROR_CODE_4 0x04 //�ӻ��豸����
#define MODBUS_ERROR_CODE_6 0x06 //�ӻ��豸æµ

#define MODBUS_COMMAND_ASDU 0x01 //�����ݸ�ʽ֡
#define MODBUS_DATA_ASDU    0x02 //�����ݸ�ʽ֡

typedef struct _Q_MODBUS_HEADER
{
	BYTE codeHigh;//��Ÿ��ֽ�
	BYTE codeLow;//��ŵ��ֽ�
	BYTE codeFlgHigh;//ʶ������ֽڣ��˴�Ϊ��
	BYTE codeFlgLow;//ʶ������ֽڣ��˴�Ϊ��
	BYTE lenHigh;//���ȸ��ֽ�
	BYTE lenLow;//���ȵ��ֽ�
}Q_MODBUS_HEADER;


typedef struct _Q_MODBUS_REC_YC
{
	BYTE startAddrHigh;//��ʼ��ַ��λ
	BYTE startAddrLow;//��ʼ��ַ��λ
	BYTE regCountHigh;//�Ĵ�����ַ��λ
	BYTE regCountLow;//�Ĵ�����ַ��λ
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


typedef enum _Q_Modbus_CommandType
{
	eModbusCommandTypeNull						= 0,
	eModbusCommandRemoteControl					= 1,//ң��
	eModbusCommandRemoteAdjust					= 2,//ң��
	eModbusCommandMeasure					    = 3,//ң��
	eModbusCommandPoint                         = 4,//ң��
                                                    //jwk add 2012-8-10
    eYH_YC_MX                                   = 5,//����ң��ĸ�߲���
    eYH_YC_YW                                   = 6,//����ң������
    eYH_YC_MCR                                  = 7,//����ң��MCR����
    eYH_YC_OTHER                                = 8,//����ң����������
    eYH_YX                                      = 9,//����ң��
    eYH_YK                                      = 10,//����ң��
    eYH_YT                                      = 11//����ң��
}Q_Modbus_CommandType;
/** @brief cdt command status. */
typedef enum _Q_Modbus_CommandStatus
{
	eModbusCommandStatusNull						= 0,
	eModbusCommandSelect							= 1,
	eModbusCommandSelectBack						= 2,
	eModbusCommandAct								= 3,
	eModbusCommandCancel							= 4,//ȷ�Ϸ���
	eModbusCommandActCon							= 5,  
	eModbusCommandActError							= 6  

}Q_Modbus_CommandStatus;
typedef enum _ModbusAnalogUpType
{
	eModAnalogUpType4=1,//�ĸ��ֽڴӵ͵���
	eModAnalogUpType2=2 //���ֽڸ��ֽڴӸߵ���
}ModbusAnalogUpType;
//�������ַ�����˳��
typedef enum ModbusFloatUpSeq
{
	eFloatUpSeqLowFirst=1,//�ĸ��ֽڴӵ͵���
	eFloatUpSeqHighFirst=2,//�ĸ��ֽڴӸߵ���
	eFloatUpSeq2301=3,  //�����Ĵ���֮��,���ֽ���ǰ,���ֽ��ں󣻼Ĵ����ڲ��Ǹ���ǰ�����ں�
	eFloatUpSeq1032=4    //�����Ĵ���֮�䣬�Ǹ��ֽ���ǰ�����ֽ��ں󣬼Ĵ����ڲ�����ǰ�����ں�
}ModbusFloatUpSeq;
typedef enum ModbusIntUpSeq
{
	eIntUpSeqHighFirst=1,//����ǰ�����ں�
	eIntUpSeqLowFirst=2,//����ǰ�����ں�
	eIntUpSeq2301=3,  //�����Ĵ���֮��,���ֽ���ǰ,���ֽ��ں󣻼Ĵ����ڲ��Ǹ���ǰ�����ں�
	eIntUpSeq1032=4    //�����Ĵ���֮�䣬�Ǹ��ֽ���ǰ�����ֽ��ں󣬼Ĵ����ڲ�����ǰ�����ں�
	
}ModbusIntUpSeq;

typedef enum ModbusCheckCodSeq
{
	eCheckCodeSeqHighFirst=1,//����ǰ�����ں�
	eCheckCodeSeqLowFirst=2,//����ǰ�����ں�
	
}ModbusCheckCodSeq;

typedef enum ModbusYTMethod
{
	eYTMethodFloatFun10=1,//���ֽڸ����͹�����Ϊ0x10
	eYTMethodFloatFun06=2, //���ֽڸ����͹�����Ϊ0x06
	eYTMethodIntFun10=3, //jwk add 2012-8-1 �޹�����ң��������0x20
    eYTMethodFloatFun20=4, //jwk add 2012-8-1 �޹�����ң��������0x20
	eYTMethodIntFun06=5, //����06������
	eYTMethodFloatFun03=6,//���ֽڸ����͹�����Ϊ0x03��ɽ��������÷Ǳ�׼��
	eYTMethodIntFun06_2Bytes=7, //2�ֽ�int����
	eYTMethodDouble8=8,         //8�ֽ�double����ң��
	eYTMethodInt_4Bytes=9, //2�ֽ�int����
	eYTMethodTypeUShort=10
}ModbusYTMethod;

typedef enum NewModbusYTMethod
{
	eYtFloat=1,
	eYtInt2=2,
	eYtInt4=3
}NewModbusYTMethod;

//2015-04-15ͳһ���ġ���Q_frameDefine.h�ж���
//tjs 
//typedef enum Q_MODBUS_VarType
//{
//	eModVarTypeNull=0,//δ֪����
//	eModVarTypeYc=1,//ң��仯
//	eModVarTypeYx=2,//ң�ű仯
//	eModVarTypeEx=3 //������ֵ�仯
//}Q_MODBUS_VarType;
//
////���ڴ����ⲿִ��������
//typedef struct Q_MODBUS_ExecCmd
//{
//	int funcode;// ������
//	Q_MODBUS_VarType varType;//�仯����������
//	int m_vindex;//�仯������,Ĭ���Ǵ��㿪ʼ��
//	bool bzf;
//	int zfchannelid;//ת��ͨ��id
//	int zfdevice;//ת��վ���
//	int zfno; //ת��վ�����
//	Q_RECORDDATATAG m_tag;//��������
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
	eExplainYcTypeFloat=1,//δ֪
	eExplainYcTypeInt=2,//���ֽ����Σ��з���
	eExplainYcTypeMixed=3,//������ͣ�������ң������ָ����������
	eExplainYcTypeDouble8=4,//˫���ȸ�����
	eExplainYcTypeIntHuaWei=7,//��Ϊ���ݣ��������ֽڸ���
	eExplainYcTypeInt4=8,//���ֽ����Σ��з���
	eExplainYcTypeUnsignedInt4=9,//���ֽ����Σ��޷���
	eExplainYcTypeUShort=10 //���ֽ������޷���
}Q_eExplainYcType;

#endif//__Q_MODBUSTCPSERVERSTANDARDDEFINE_H_INCLUDE__

