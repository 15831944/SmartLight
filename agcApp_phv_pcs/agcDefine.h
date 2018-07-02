#ifndef __AGC_DEFINE_DEFINED_H_
#define __AGC_DEFINE_DEFINED_H_
#include "AGC.h"
#include "gSystemDef.h"
static const char *g_ParamKey[ 30 ] =
{
	"",
	"CalcInterval",   //1 ���ڼ���ʱ����(����)
	"SendCmdInterval",// �·�����ʱ����(����)
	"ReSendFlg",//5 ִ������ɹ��ظ�����
	"LogFlg", // ��־����
	"LockCheckFlg",// �ջ���鿪��
	"SafeCheckFlg", //14 ��ȫԼ������
	"DebugFlg",
	"AdMethod",         //���ڷ�ʽ���ڲ���(1����������2��ԣ��)
	"OneMinLimitFlg",				    //����1���ӱ仯������
	"CmdTimeOut",  //�ж�����ִ�в���λʱ��Ĭ����
	"AGCDataLog",
	"AGCDataLogPeriod"
};
typedef enum _enmAgcParamKey
{
	eAgcParamNull=0,
	eAgcParamCalcInterval=1,
	eAgcParamSendCmdInterval=2,
	eAgcParamReSendFlg=3,
	eAgcParamLogFlg=4,
	eAgcParamLockCheckFlg=5,
	eAgcParamSafeCheckFlg=6,
	eAgcParamDebugFlg=7,
	eAgcParamAdMethod=8,
	eAgcParamOneMinLimitFlg=9,
	eAgcParamCmdTimeOut=10,
	eAgcParamAGCDataLog=11,
	eAgcParamAGCDataLogPeriod=12

}enmAgcParamKey;

static const char *g_AgcDigitKey[ 20 ] =
{
	"",
	"AgcOptionOn",// AGCͶ���ź�
	"AgcCycleOn", // AGCͶ��ջ�����
	"TotalBlock",
	"PhvChannel"
};
typedef enum _enmAgcDigitKey
{
	eAgcDigitNull=0,
	eAgcDigitOptionOn=1,
	eAgcDigitCycleOn=2,
	eAgcDigitTotalBlock=3,
	eAgcDigitPhvChannel=4

}enmAgcDigitKey;

static const char *g_AgcAnalogKey[ 30 ] =
{
	"",
	"NetUa",   //1 �������й�
	"NetUb",// �������޹�
	"NetUc", //�������ѹ
	"NetUMax",// ������cos
	"NetUMin",//������Ƶ��
	"ULimit", // ����ǿ��
	"PhvSumPower",//������й��ܺ�
	"PhvOpenCount",// ���������̨��
	"PhvOpenCap",// �������������
	"SocSumPower",//AGCԶ���ƻ�ֵ 10
	"Capicity", //��װ������(kW)
	"SocChargeMax",
	"SocDischargeMin",
	"SocChargePMax",
	"SocDischargePMax",
	"FtPowerU",
	"SocChargekW",//�����ȶ�ʱ��繦��(kW)
	"SocDischargeKw", //�����ȶ�ʱ�ŵ繦��(kW)
	"AdVoltModify",
	"ChargeBeginHour",
	"ChargeBeginMin",
	"ChargeEndHour",
	"ChargeEndMin",
	"DischargeBeginHour",
	"DischargeBeginMin",
	"DischargeEndHour",
	"DischargeEndMin",
	"SocChPhvFactor"



};
typedef enum _enmAgcAnalogKey
{
	eAgcAnalogNull=0,
	eAgcAnalogNetUa=1,
	eAgcAnalogNetUb=2,
	eAgcAnalogNetUc=3,
	eAgcAnalogNetUMax=4,
	eAgcAnalogNetUMin=5,
	eAgcAnalogULimit=6,
	eAgcAnalogPhvSumPower=7,
	eAgcAnalogPhvOpenCount=8,
	eAgcAnalogPhvOpenCap=9,
	eAgcAnalogSocSumPower=10,
	eAgcAnalogCapicity=11,
	eAgcAnalogSocChargeMax=12,
	eAgcAnalogSocDischargeMin=13,
	eAgcAnalogSocChargePMax,
	eAgcAnalogSocDischargePMax,
	eAgcAnalogFtPowerU,
	eAgcAnalogSocChargekW,
	eAgcAnalogSocDischargeKw,
	eAgcAnalogAdVoltModify,
	eAgcAnalogChargeBeginHour,
	eAgcAnalogChargeBeginMin,
	eAgcAnalogChargeEndHour,
	eAgcAnalogChargeEndMin,
	eAgcAnalogDischargeBeginHour,
	eAgcAnalogDischargeBeginMin,
	eAgcAnalogDischargeEndHour,
	eAgcAnalogDischargeEndMin,
	eAgcAnalogSocChPhvFactor



}enmAgcAnalogKey;


static const char *g_AgcAlarmKey[ 20 ] =
{
	"",
	"OffNet",           //1 AGC�Զ��б�������
	"PcsFault",         // AGC�Զ���Զ������
	"PcsFaultOverLoad", // AGC����վͨ���Ͽ�
	"BusVoltHigh",      // AGC����վͨ���ָ�
	"OverLoad",         // AGC����վͨ���ָ�
	"SocLowOverLoad",   // AGC����վͨ���ָ�
	"PcsFaultStartNoExec",// AGC����վͨ���ָ�
	"PhvChBroken",      // AGC����վͨ���ָ�
	"PcsFaultChargeNoExec",   
	"PcsFaultDischargeNoExec",   
	"PcsFaultWaitNoExec"
};
typedef enum _enmAlarmKey
{
	eAgcAlarmNull=0,
	eAgcAlarmOffNet=1,
	eAgcAlarmPcsFault=2,
	eAgcAlarmPcsFaultOverLoad=3,
	eAgcAlarmBusVoltHigh=4,
	eAgcAlarmOverLoad=5,
	eAgcAlarmSocLowOverLoad=6,
	eAgcAlarmPcsFaultStartNoExec=7,
	eAgcAlarmPhvChBroken=8,
	eAgcAlarmPcsFaultChargeNoExec=9,   
	eAgcAlarmPcsFaultDischargeNoExec=10,   
	eAgcAlarmPcsFaultWaitNoExec=11
}enmAgcAlarmKey;




#define CALC_TH_KeyID 1 //�����̱߳�ʶ
#define  PLAN_TH_KeyID 2 //�ƻ��̱߳�ʶ
#define CALC_TH_AuxKeyID 3
#define AGCCALC_TIMESPAN 50 //AGC�ȴ�ʱ����С���

/*AGC���ؿ��Ʒ�ʽ*/
typedef enum _enmCMD_MODE
{
eCMD_MODE_INVALID=0,  //��Ч
eCMD_MODE_SETPOINT=2, //���ģʽ


}enmCMD_MODE;


//�����ʶ
#define _ERROR_GET_REAL_VAL_FAIL -5    //��ȡʵʱĿ��ֵʱ��������


const double AGC_CMD_DIFF_VAL_UGEXEC=2000;    //���й������ֵ������ֵ�������ǰ�������ִ���У����·�������
const int    AGC_RE_SEND_MAX_TIMES=3;//����ط�����

const char * const AGC_PATH_LOGFILE= "agclogfile.log";
const char * const AGC_LOG_PATH = "agc";

#ifdef WINDOWS_ENV
//#define COMM_PLAN_CURVE_DIR							"\\data\\" //AGCԶ������
#define COMM_PLAN_CURVE_DIR							"\\data\\remote_agc\\" //AGCԶ������
#define COMM_PLAN_LOCAL_CURVE_DIR					"\\data\\local_agc\\" //kc add AGC��������·��
const char * const agcDataLogPath="\\data\\message\\agc\\";
#else
#define COMM_PLAN_CURVE_DIR							"/data/remote_agc/" //AGCԶ������
#define COMM_PLAN_LOCAL_CURVE_DIR					"/data/local_agc/" //kc add AGC��������·��

const char * const agcDataLogPath="/data/message/agc/";
#endif

#define SOC_POWER_STATUS_Charge 1  //���
#define SOC_POWER_STATUS_Discharge 2  //�ŵ�
#define SOC_POWER_STATUS_Auto  0 //����
#define Battery_Status_Null  0  // δ����
#define Battery_Status_Full  4369   // ������
#define Battery_Status_Empty  21845    //������
#define Battery_Status_Alarm  21845   //����
#define Battery_Status_Fault  43690 //����
#define Battery_Status_Normal  48059 //����
//����һ������
typedef struct _GeneratorUnit
{
 AGC_PhvUnit *p_Unit;
 time_t lastAdjustTime;//��һ�ε���ʱ��
 float lastObjectValue;//��һ�η�������ֵ
 int m_ReSendTimes;//��ǰ�ظ����͵Ĵ���
 bool adjustFlag;
 bool enabled;
 int ratio;//���ڱ���һ��Ϊ0-1000��ԭ��ֵΪ0��100
 bool canAdjust; 
 float m_CanAdd;//����
 float m_CanSub;//�ɼ�
 _GeneratorUnit()
 {
	 time(&lastAdjustTime);
	 p_Unit=NULL;
	 lastObjectValue=0;
	 m_ReSendTimes = 0;
	 adjustFlag = false;
	 enabled = true;
//	 ratio=0;
	 canAdjust=true;
	 m_CanAdd=0;
	 m_CanAdd=0;
 }

}GeneratorUnit;

//����һ������
typedef struct _GeSocUnit
{
	AGC_SocUnit *p_Unit;
	time_t lastAdjustTime;//��һ�ε���ʱ��
	float lastObjectValue;//��һ�η�������ֵ
	int m_ReSendTimes;//��ǰ�ظ����͵Ĵ���
	bool adjustFlag;
	bool enabled;
	bool canAdjust; 
	bool m_FlgSendStart;//������־
	_GeSocUnit()
	{
		time(&lastAdjustTime);
		p_Unit=NULL;
		lastObjectValue=0;
		m_ReSendTimes = 0;
		adjustFlag = false;
		enabled = true;
		canAdjust=true;

		m_FlgSendStart=false;
		m_ReSendTimes=0;
	}
	//����ǳ������Ƿŵ�
	//1���
	//2 �ŵ�
	int checkCFStatus()
	{
		if(p_Unit==NULL)
			return SOC_POWER_STATUS_Auto;
		
	
		if(p_Unit->fValUp>0)
		{
			//	kW > 0: power into the grid
			return SOC_POWER_STATUS_Discharge; 
		}
		else if(p_Unit->fValUp<0)
		{
			//kW < 0: power drawn from the grid
			return SOC_POWER_STATUS_Charge;
		}
		else
		{
			return SOC_POWER_STATUS_Auto;
		}
	}
}GeSocUnit;

#define PHVUNIT_EXEC 1
#define PHVUNIT_NOEXEC 0 

typedef enum _AgcAdMethod
{
  eAdMethodNULL=0,//��
  eAdMethodByOpenCapicity=1,//����������
  eAdMethodByMargin=2, //��ԣ�ȵ���
  eAdMethodByRequest=3, //�������
}AgcAdMethod;

#define Quality_Bad_AGC_P 2   //ʵʱ�޹�������
#define Quality_Bad_AVC_UpCmd 4 //ָ�����ͻ�����
#define Quality_Bad_AVC_Unit 8  //����״̬������
#define Quality_Bad_AVC_Channel 16 //ͨ��״̬������
#define Quality_Bad_AVC_Exception 32  // ����״̬������
#define Quality_Bad_AVC_UpLimit 64     //�������޻�����
#define Quality_Bad_AVC_DownLimit 128  //�������޻�����



#define SOC_PCS_EXP_STATUS_Normal 0  //����pss����״̬ ����
#define SOC_PCS_EXP_STATUS_Warning 1
#define SOC_PCS_EXP_STATUS_Fault 2


#define SOC_PCS_RUN_STATUS_Stopped 1  //��������״̬Ϊͣ��
#define SOC_PCS_RUN_STATUS_Run 2       //��������״̬Ϊ����
#define SOC_PCS_RUN_STATUS_Undefined 9   //��������״̬δ����
typedef enum _ServerBkType
{
	eSvrBkTypeNULL=0,//��
	eSvrBkTypeMaster=1,//����
	eSvrBkTypeSlave=2  //����
}ServerBkType;

#define AGC_SUC 0
#define AGC_FAIL -1
#endif