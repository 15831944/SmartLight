#ifndef __DATABASEAGC_h__
#define __DATABASEAGC_h__

#include "Rtdb_Struct.h"
#define DB_ID_OF_AGC                                31
//����ID
#define AGC_TAB_Group_ID                          	3101	//AGC����
#define AGC_TAB_PhvUnit_ID                        	3102	//AGC����
#define AGC_TAB_Parameter_ID                      	3103	//AGC����
#define AGC_TAB_Analog_ID                         	3104	//AGCң��
#define AGC_TAB_Digit_ID                          	3105	//AGCң��
#define AGC_TAB_Alarm_ID                          	3106	//AGC����
#define AGC_TAB_SocUnit_ID                        	3107	//AGC���ܻ���
#define AGC_TAB_LogRecord_ID                      	3108	//AGC������־
//���������ݿ��е�����
#define AGC_TAB_Group_INDEX                       	0	//AGC����
#define AGC_TAB_PhvUnit_INDEX                     	1	//AGC����
#define AGC_TAB_Parameter_INDEX                   	2	//AGC����
#define AGC_TAB_Analog_INDEX                      	3	//AGCң��
#define AGC_TAB_Digit_INDEX                       	4	//AGCң��
#define AGC_TAB_Alarm_INDEX                       	5	//AGC����
#define AGC_TAB_SocUnit_INDEX                     	6	//AGC���ܻ���
#define AGC_TAB_LogRecord_INDEX                   	7	//AGC������־

//������ֶ��ڱ�������(AGC����)
#define AGC_TAB_Group_FIELD_ID32_INDEX                      	0	//�ڲ�ID32
#define AGC_TAB_Group_FIELD_strName_INDEX                   	1	//��������
#define AGC_TAB_Group_FIELD_iEnabled_INDEX                  	2	//���ñ�ʶ
#define AGC_TAB_Group_FIELD_iPriority_INDEX                 	3	//�������ȼ�

//������ֶ��ڱ�������(AGC����)
#define AGC_TAB_PhvUnit_FIELD_ID32_INDEX                    	0	//�ڲ�ID32
#define AGC_TAB_PhvUnit_FIELD_strName_INDEX                 	1	//��������
#define AGC_TAB_PhvUnit_FIELD_fValP_INDEX                   	2	//ʵʱ�й�kW
#define AGC_TAB_PhvUnit_FIELD_fDisPower_INDEX               	3	//�����й�kW
#define AGC_TAB_PhvUnit_FIELD_fcurCap_INDEX                 	4	//�й��kW
#define AGC_TAB_PhvUnit_FIELD_fAdMaxLimit_INDEX             	5	//�ɵ�����kW
#define AGC_TAB_PhvUnit_FIELD_fAdMinLimit_INDEX             	6	//�ɵ�����kW
#define AGC_TAB_PhvUnit_FIELD_iGroup_INDEX                  	7	//���
#define AGC_TAB_PhvUnit_FIELD_iEnabled_INDEX                	8	//���ñ�ʶ
#define AGC_TAB_PhvUnit_FIELD_iStatus_INDEX                 	9	//��������״̬
#define AGC_TAB_PhvUnit_FIELD_iChannelStatus_INDEX          	10	//ͨ��״̬
#define AGC_TAB_PhvUnit_FIELD_iUpLock_INDEX                 	11	//������
#define AGC_TAB_PhvUnit_FIELD_iDownLock_INDEX               	12	//������
#define AGC_TAB_PhvUnit_FIELD_tSnapTime_INDEX               	13	//ʱ����
#define AGC_TAB_PhvUnit_FIELD_iType_INDEX                   	14	//��������
#define AGC_TAB_PhvUnit_FIELD_iFlag_INDEX                   	15	//��ʶ
#define AGC_TAB_PhvUnit_FIELD_fValUp_INDEX                  	16	//Ŀ��ָ���ֵkW
#define AGC_TAB_PhvUnit_FIELD_fAdZone_INDEX                 	17	//��������kW
#define AGC_TAB_PhvUnit_FIELD_iModifyAdd_INDEX              	18	//���й��������kW
#define AGC_TAB_PhvUnit_FIELD_iModifySub_INDEX              	19	//���й��������kW
#define AGC_TAB_PhvUnit_FIELD_iAdTimesMax_INDEX             	20	//����������
#define AGC_TAB_PhvUnit_FIELD_iPowerAlgID_INDEX             	21	//�й�ң��ID
#define AGC_TAB_PhvUnit_FIELD_iUpValAlgID_INDEX             	22	//ָ���ֵң��ID
#define AGC_TAB_PhvUnit_FIELD_iStatusDigitID_INDEX          	23	//����״̬ң��ID
#define AGC_TAB_PhvUnit_FIELD_iChannelDigitID_INDEX         	24	//ͨ��ң��ID
#define AGC_TAB_PhvUnit_FIELD_iChannelID_INDEX              	25	//ͨ����
#define AGC_TAB_PhvUnit_FIELD_iDevice_INDEX                 	26	//�豸���
#define AGC_TAB_PhvUnit_FIELD_iYtNo_INDEX                   	27	//ң�����
#define AGC_TAB_PhvUnit_FIELD_iYkNo_INDEX                   	28	//ң�ص��
#define AGC_TAB_PhvUnit_FIELD_fFactor_INDEX                 	29	//ң��ϵ��
#define AGC_TAB_PhvUnit_FIELD_iQuality_INDEX                	30	//������
#define AGC_TAB_PhvUnit_FIELD_iDisPower_INDEX               	31	//�����й�ң��ID

//������ֶ��ڱ�������(AGC����)
#define AGC_TAB_Parameter_FIELD_ID32_INDEX                  	0	//�ڲ�ID32
#define AGC_TAB_Parameter_FIELD_strName_INDEX               	1	//��ֵ����
#define AGC_TAB_Parameter_FIELD_strKey_INDEX                	2	//�ؼ���
#define AGC_TAB_Parameter_FIELD_iType_INDEX                 	3	//��ֵ����
#define AGC_TAB_Parameter_FIELD_iVal_INDEX                  	4	//���ζ�ֵ

//������ֶ��ڱ�������(AGCң��)
#define AGC_TAB_Analog_FIELD_ID32_INDEX                     	0	//�ڲ�ID32
#define AGC_TAB_Analog_FIELD_strName_INDEX                  	1	//����
#define AGC_TAB_Analog_FIELD_strKey_INDEX                   	2	//�ؼ���
#define AGC_TAB_Analog_FIELD_iType_INDEX                    	3	//��������
#define AGC_TAB_Analog_FIELD_fVal_INDEX                     	4	//ֵ
#define AGC_TAB_Analog_FIELD_iAnalogID_INDEX                	5	//ң��ID
#define AGC_TAB_Analog_FIELD_tSnapTime_INDEX                	6	//ʱ����
#define AGC_TAB_Analog_FIELD_iQuality_INDEX                 	7	//������

//������ֶ��ڱ�������(AGCң��)
#define AGC_TAB_Digit_FIELD_ID32_INDEX                      	0	//�ڲ�ID32
#define AGC_TAB_Digit_FIELD_strName_INDEX                   	1	//��ֵ����
#define AGC_TAB_Digit_FIELD_strKey_INDEX                    	2	//�ؼ���
#define AGC_TAB_Digit_FIELD_iType_INDEX                     	3	//��������
#define AGC_TAB_Digit_FIELD_iVal_INDEX                      	4	//ֵ
#define AGC_TAB_Digit_FIELD_iDigitID_INDEX                  	5	//ң��ID
#define AGC_TAB_Digit_FIELD_tSnapTime_INDEX                 	6	//ʱ����
#define AGC_TAB_Digit_FIELD_iQuality_INDEX                  	7	//������

//������ֶ��ڱ�������(AGC����)
#define AGC_TAB_Alarm_FIELD_ID32_INDEX                      	0	//�ڲ�ID32
#define AGC_TAB_Alarm_FIELD_strName_INDEX                   	1	//��ֵ����
#define AGC_TAB_Alarm_FIELD_strKey_INDEX                    	2	//�ؼ���
#define AGC_TAB_Alarm_FIELD_iVal_INDEX                      	3	//ֵ
#define AGC_TAB_Alarm_FIELD_iDigitID_INDEX                  	4	//ң��ID
#define AGC_TAB_Alarm_FIELD_tSnapTime_INDEX                 	5	//ʱ����

//������ֶ��ڱ�������(AGC���ܻ���)
#define AGC_TAB_SocUnit_FIELD_ID32_INDEX                    	0	//�ڲ�ID32
#define AGC_TAB_SocUnit_FIELD_strName_INDEX                 	1	//��������
#define AGC_TAB_SocUnit_FIELD_fValP_INDEX                   	2	//ʵʱ�й�kW
#define AGC_TAB_SocUnit_FIELD_fBatteryPower_INDEX           	3	//��ص���%
#define AGC_TAB_SocUnit_FIELD_fcurCap_INDEX                 	4	//�kWh
#define AGC_TAB_SocUnit_FIELD_fValAC_INDEX                  	5	//��������
#define AGC_TAB_SocUnit_FIELD_fValUa_INDEX                  	6	//������ѹ
#define AGC_TAB_SocUnit_FIELD_fValUp_INDEX                  	7	//�й��趨ֵ����
#define AGC_TAB_SocUnit_FIELD_fAdMaxLimit_INDEX             	8	//�������%
#define AGC_TAB_SocUnit_FIELD_fAdMinLimit_INDEX             	9	//�ŵ�����%
#define AGC_TAB_SocUnit_FIELD_iGroup_INDEX                  	10	//���
#define AGC_TAB_SocUnit_FIELD_iEnabled_INDEX                	11	//���ñ�ʶ
#define AGC_TAB_SocUnit_FIELD_iStatus_INDEX                 	12	//��������״̬
#define AGC_TAB_SocUnit_FIELD_iChannelStatus_INDEX          	13	//ͨ��״̬
#define AGC_TAB_SocUnit_FIELD_iExpStatus_INDEX              	14	//�������״̬
#define AGC_TAB_SocUnit_FIELD_iUpLock_INDEX                 	15	//������
#define AGC_TAB_SocUnit_FIELD_iDownLock_INDEX               	16	//������
#define AGC_TAB_SocUnit_FIELD_tSnapTime_INDEX               	17	//ʱ����
#define AGC_TAB_SocUnit_FIELD_iFlag_INDEX                   	18	//��ʶ
#define AGC_TAB_SocUnit_FIELD_fAdZone_INDEX                 	19	//��������kW
#define AGC_TAB_SocUnit_FIELD_iPowerAlgID_INDEX             	20	//�й�ң��ID
#define AGC_TAB_SocUnit_FIELD_iStatusDigitID_INDEX          	21	//����״̬ң��ID
#define AGC_TAB_SocUnit_FIELD_iChannelDigitID_INDEX         	22	//ͨ��ң��ID
#define AGC_TAB_SocUnit_FIELD_iExpStatusDigitID_INDEX       	23	//����״̬ң��ID
#define AGC_TAB_SocUnit_FIELD_iChannelID_INDEX              	24	//ͨ����
#define AGC_TAB_SocUnit_FIELD_iDevice_INDEX                 	25	//�豸���
#define AGC_TAB_SocUnit_FIELD_iYkNo_INDEX                   	26	//ң�ص��
#define AGC_TAB_SocUnit_FIELD_iYtNo_INDEX                   	27	//�й�ң�����
#define AGC_TAB_SocUnit_FIELD_iQuality_INDEX                	28	//������
#define AGC_TAB_SocUnit_FIELD_iSocAlgID_INDEX               	29	//��ص���ң��ID
#define AGC_TAB_SocUnit_FIELD_iUaAlgID_INDEX                	30	//������ѹң��ID
#define AGC_TAB_SocUnit_FIELD_iIaAlgID_INDEX                	31	//��������ң��ID
#define AGC_TAB_SocUnit_FIELD_iBStatus_INDEX                	32	//���״̬
#define AGC_TAB_SocUnit_FIELD_iBStatusAlgID_INDEX           	33	//���״̬ң��ID
#define AGC_TAB_SocUnit_FIELD_iUpValAlgID_INDEX             	34	//ң������ң��ID
#define AGC_TAB_SocUnit_FIELD_fDisPower_INDEX               	35	//�����й�kW
#define AGC_TAB_SocUnit_FIELD_iDisPower_INDEX               	36	//�����й�ң��ID
#define AGC_TAB_SocUnit_FIELD_fFactor_INDEX                 	37	//ң��ϵ��

//������ֶ��ڱ�������(AGC������־)
#define AGC_TAB_LogRecord_FIELD_ID32_INDEX                  	0	//�ڲ�ID32
#define AGC_TAB_LogRecord_FIELD_tSnapTime_INDEX             	1	//ʱ����
#define AGC_TAB_LogRecord_FIELD_strText_INDEX               	2	//����



typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��������
	INT         		iEnabled;                          //���ñ�ʶ
	INT         		iPriority;                         //�������ȼ�
}AGC_Group;		//AGC����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��������
	FLOAT       		fValP;                             //ʵʱ�й�kW
	FLOAT       		fDisPower;                         //�����й�kW
	FLOAT       		fcurCap;                           //�й��kW
	FLOAT       		fAdMaxLimit;                       //�ɵ�����kW
	FLOAT       		fAdMinLimit;                       //�ɵ�����kW
	UINT        		iGroup;                            //���
	INT         		iEnabled;                          //���ñ�ʶ
	INT         		iStatus;                           //��������״̬
	INT         		iChannelStatus;                    //ͨ��״̬
	INT         		iUpLock;                           //������
	INT         		iDownLock;                         //������
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	INT         		iType;                             //��������
	INT         		iFlag;                             //��ʶ
	FLOAT       		fValUp;                            //Ŀ��ָ���ֵkW
	FLOAT       		fAdZone;                           //��������kW
	FLOAT       		iModifyAdd;                        //���й��������kW
	FLOAT       		iModifySub;                        //���й��������kW
	INT         		iAdTimesMax;                       //����������
	INT         		iPowerAlgID;                       //�й�ң��ID
	INT         		iUpValAlgID;                       //ָ���ֵң��ID
	INT         		iStatusDigitID;                    //����״̬ң��ID
	INT         		iChannelDigitID;                   //ͨ��ң��ID
	INT         		iChannelID;                        //ͨ����
	INT         		iDevice;                           //�豸���
	INT         		iYtNo;                             //ң�����
	INT         		iYkNo;                             //ң�ص��
	FLOAT       		fFactor;                           //ң��ϵ��
	INT         		iQuality;                          //������
	INT         		iDisPower;                         //�����й�ң��ID
}AGC_PhvUnit;		//AGC����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��ֵ����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iType;                             //��ֵ����
	INT         		iVal;                              //���ζ�ֵ
}AGC_Parameter;		//AGC����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iType;                             //��������
	FLOAT       		fVal;                              //ֵ
	INT         		iAnalogID;                         //ң��ID
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	INT         		iQuality;                          //������
}AGC_Analog;		//AGCң��

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��ֵ����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iType;                             //��������
	INT         		iVal;                              //ֵ
	INT         		iDigitID;                          //ң��ID
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	INT         		iQuality;                          //������
}AGC_Digit;		//AGCң��

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��ֵ����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iVal;                              //ֵ
	INT         		iDigitID;                          //ң��ID
	SYSTEMTIME  		tSnapTime;                         //ʱ����
}AGC_Alarm;		//AGC����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��������
	FLOAT       		fValP;                             //ʵʱ�й�kW
	FLOAT       		fBatteryPower;                     //��ص���%
	FLOAT       		fcurCap;                           //�kWh
	FLOAT       		fValAC;                            //��������
	FLOAT       		fValUa;                            //������ѹ
	FLOAT       		fValUp;                            //�й��趨ֵ����
	FLOAT       		fAdMaxLimit;                       //�������%
	FLOAT       		fAdMinLimit;                       //�ŵ�����%
	UINT        		iGroup;                            //���
	INT         		iEnabled;                          //���ñ�ʶ
	INT         		iStatus;                           //��������״̬
	INT         		iChannelStatus;                    //ͨ��״̬
	INT         		iExpStatus;                        //�������״̬
	INT         		iUpLock;                           //������
	INT         		iDownLock;                         //������
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	INT         		iFlag;                             //��ʶ
	FLOAT       		fAdZone;                           //��������kW
	INT         		iPowerAlgID;                       //�й�ң��ID
	INT         		iStatusDigitID;                    //����״̬ң��ID
	INT         		iChannelDigitID;                   //ͨ��ң��ID
	INT         		iExpStatusDigitID;                 //����״̬ң��ID
	INT         		iChannelID;                        //ͨ����
	INT         		iDevice;                           //�豸���
	INT         		iYkNo;                             //ң�ص��
	INT         		iYtNo;                             //�й�ң�����
	INT         		iQuality;                          //������
	INT         		iSocAlgID;                         //��ص���ң��ID
	INT         		iUaAlgID;                          //������ѹң��ID
	INT         		iIaAlgID;                          //��������ң��ID
	INT         		iBStatus;                          //���״̬
	INT         		iBStatusAlgID;                     //���״̬ң��ID
	INT         		iUpValAlgID;                       //ң������ң��ID
	FLOAT       		fDisPower;                         //�����й�kW
	INT         		iDisPower;                         //�����й�ң��ID
	FLOAT       		fFactor;                           //ң��ϵ��
}AGC_SocUnit;		//AGC���ܻ���

typedef struct 
{
	INT         		ID32;                              //�ڲ�ID32
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	CHAR        		strText[256];                      //����
}AGC_LogRecord;		//AGC������־


#endif   //__DATABASEAGC_h__
