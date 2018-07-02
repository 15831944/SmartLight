#ifndef __DATABASEAGC_h__
#define __DATABASEAGC_h__

#include "Rtdb_Struct.h"
#define DB_ID_OF_AGC                                31
//����ID
#define AGC_TAB_PhvUnit_ID                        	3101	//AGC����
#define AGC_TAB_Parameter_ID                      	3102	//AGC���ڲ���
#define AGC_TAB_Analog_ID                         	3103	//AGC�������ң��
#define AGC_TAB_Digit_ID                          	3104	//AGC�������ң��
#define AGC_TAB_Alarm_ID                          	3105	//AGC����
#define AGC_TAB_LogRecord_ID                      	3106	//AGC������־
//���������ݿ��е�����
#define AGC_TAB_PhvUnit_INDEX                     	0	//AGC����
#define AGC_TAB_Parameter_INDEX                   	1	//AGC���ڲ���
#define AGC_TAB_Analog_INDEX                      	2	//AGC�������ң��
#define AGC_TAB_Digit_INDEX                       	3	//AGC�������ң��
#define AGC_TAB_Alarm_INDEX                       	4	//AGC����
#define AGC_TAB_LogRecord_INDEX                   	5	//AGC������־

//������ֶ��ڱ�������(AGC����)
#define AGC_TAB_PhvUnit_FIELD_ID32_INDEX                    	0	//�ڲ�ID32
#define AGC_TAB_PhvUnit_FIELD_strName_INDEX                 	1	//��������
#define AGC_TAB_PhvUnit_FIELD_fValP_INDEX                   	2	//�й�
#define AGC_TAB_PhvUnit_FIELD_fValQ_INDEX                   	3	//�޹�
#define AGC_TAB_PhvUnit_FIELD_tSnapTime_INDEX               	4	//ʱ����
#define AGC_TAB_PhvUnit_FIELD_iType_INDEX                   	5	//��������
#define AGC_TAB_PhvUnit_FIELD_iGroup_INDEX                  	6	//���
#define AGC_TAB_PhvUnit_FIELD_fcurCap_INDEX                 	7	//�й��
#define AGC_TAB_PhvUnit_FIELD_fAdMinLimit_INDEX             	8	//���ڵ���
#define AGC_TAB_PhvUnit_FIELD_iStatus_INDEX                 	9	//��������״̬
#define AGC_TAB_PhvUnit_FIELD_fSendZone_INDEX               	10	//���͵�����������
#define AGC_TAB_PhvUnit_FIELD_fAdZone_INDEX                 	11	//��������
#define AGC_TAB_PhvUnit_FIELD_iChannelStatus_INDEX          	12	//ͨ��״̬
#define AGC_TAB_PhvUnit_FIELD_iEnabled_INDEX                	13	//���ñ�ʶ
#define AGC_TAB_PhvUnit_FIELD_iFlag_INDEX                   	14	//��ʶ
#define AGC_TAB_PhvUnit_FIELD_iPriority_INDEX               	15	//�������ȼ�
#define AGC_TAB_PhvUnit_FIELD_fDisPower_INDEX               	16	//�����й�����
#define AGC_TAB_PhvUnit_FIELD_iUpLock_INDEX                 	17	//������
#define AGC_TAB_PhvUnit_FIELD_iDownLock_INDEX               	18	//������
#define AGC_TAB_PhvUnit_FIELD_iPowerAlgID_INDEX             	19	//�й���Ӧң��ID
#define AGC_TAB_PhvUnit_FIELD_iStatusDigitID_INDEX          	20	//����״̬��Ӧң��ID
#define AGC_TAB_PhvUnit_FIELD_iChannelDigitID_INDEX         	21	//�����ͨ����Ӧң��ID
#define AGC_TAB_PhvUnit_FIELD_iChannelID_INDEX              	22	//ͨ����
#define AGC_TAB_PhvUnit_FIELD_iDevice_INDEX                 	23	//�豸���
#define AGC_TAB_PhvUnit_FIELD_iYtNo_INDEX                   	24	//ң�����
#define AGC_TAB_PhvUnit_FIELD_iYkNo_INDEX                   	25	//ң�ص��
#define AGC_TAB_PhvUnit_FIELD_iAdTimesMax_INDEX             	26	//����������
#define AGC_TAB_PhvUnit_FIELD_iCanAddAlgID_INDEX            	27	//�ɵ����޶�Ӧң��ID
#define AGC_TAB_PhvUnit_FIELD_iCanSubAlgID_INDEX            	28	//�ɵ����޶�Ӧң��ID
#define AGC_TAB_PhvUnit_FIELD_iUpValAlgID_INDEX             	29	//ָ���ֵ��Ӧң��ID
#define AGC_TAB_PhvUnit_FIELD_iModifyAdd_INDEX              	30	//��������������й�
#define AGC_TAB_PhvUnit_FIELD_iModifySub_INDEX              	31	//��������������й�
#define AGC_TAB_PhvUnit_FIELD_iProcCapAlgID_INDEX           	32	//����������Ӧң��ID
#define AGC_TAB_PhvUnit_FIELD_iQAlgID_INDEX                 	33	//�޹���Ӧң��ID
#define AGC_TAB_PhvUnit_FIELD_fFactor_INDEX                 	34	//ң��ϵ��
#define AGC_TAB_PhvUnit_FIELD_nonename2_INDEX               	35	//�����ֶ�

//������ֶ��ڱ�������(AGC���ڲ���)
#define AGC_TAB_Parameter_FIELD_ID32_INDEX                  	0	//�ڲ�ID32
#define AGC_TAB_Parameter_FIELD_strName_INDEX               	1	//��ֵ����
#define AGC_TAB_Parameter_FIELD_strKey_INDEX                	2	//�ؼ���
#define AGC_TAB_Parameter_FIELD_iType_INDEX                 	3	//��ֵ����
#define AGC_TAB_Parameter_FIELD_iVal_INDEX                  	4	//���ζ�ֵ

//������ֶ��ڱ�������(AGC�������ң��)
#define AGC_TAB_Analog_FIELD_ID32_INDEX                     	0	//�ڲ�ID32
#define AGC_TAB_Analog_FIELD_strName_INDEX                  	1	//����
#define AGC_TAB_Analog_FIELD_strKey_INDEX                   	2	//�ؼ���
#define AGC_TAB_Analog_FIELD_iType_INDEX                    	3	//��������
#define AGC_TAB_Analog_FIELD_fVal_INDEX                     	4	//ֵ
#define AGC_TAB_Analog_FIELD_iAnalogID_INDEX                	5	//ң��ID
#define AGC_TAB_Analog_FIELD_tSnapTime_INDEX                	6	//ʱ����

//������ֶ��ڱ�������(AGC�������ң��)
#define AGC_TAB_Digit_FIELD_ID32_INDEX                      	0	//�ڲ�ID32
#define AGC_TAB_Digit_FIELD_strName_INDEX                   	1	//��ֵ����
#define AGC_TAB_Digit_FIELD_strKey_INDEX                    	2	//�ؼ���
#define AGC_TAB_Digit_FIELD_iType_INDEX                     	3	//��������
#define AGC_TAB_Digit_FIELD_iVal_INDEX                      	4	//ֵ
#define AGC_TAB_Digit_FIELD_iDigitID_INDEX                  	5	//ң��ID
#define AGC_TAB_Digit_FIELD_tSnapTime_INDEX                 	6	//ʱ����

//������ֶ��ڱ�������(AGC����)
#define AGC_TAB_Alarm_FIELD_ID32_INDEX                      	0	//�ڲ�ID32
#define AGC_TAB_Alarm_FIELD_strName_INDEX                   	1	//��ֵ����
#define AGC_TAB_Alarm_FIELD_strKey_INDEX                    	2	//�ؼ���
#define AGC_TAB_Alarm_FIELD_iVal_INDEX                      	3	//ֵ
#define AGC_TAB_Alarm_FIELD_iDigitID_INDEX                  	4	//ң��ID
#define AGC_TAB_Alarm_FIELD_tSnapTime_INDEX                 	5	//ʱ����

//������ֶ��ڱ�������(AGC������־)
#define AGC_TAB_LogRecord_FIELD_tSnapTime_INDEX             	0	//ʱ����
#define AGC_TAB_LogRecord_FIELD_strText_INDEX               	1	//����



typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��������
	FLOAT       		fValP;                             //�й�
	FLOAT       		fValQ;                             //�޹�
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	UINT        		iType;                             //��������
	UINT        		iGroup;                            //���
	FLOAT       		fcurCap;                           //�й��
	FLOAT       		fAdMinLimit;                       //���ڵ���
	INT         		iStatus;                           //��������״̬
	FLOAT       		fSendZone;                         //���͵�����������
	FLOAT       		fAdZone;                           //��������
	INT         		iChannelStatus;                    //ͨ��״̬
	INT         		iEnabled;                          //���ñ�ʶ
	INT         		iFlag;                             //��ʶ
	INT         		iPriority;                         //�������ȼ�
	FLOAT       		fDisPower;                         //�����й�����
	INT         		iUpLock;                           //������
	INT         		iDownLock;                         //������
	INT         		iPowerAlgID;                       //�й���Ӧң��ID
	INT         		iStatusDigitID;                    //����״̬��Ӧң��ID
	INT         		iChannelDigitID;                   //�����ͨ����Ӧң��ID
	INT         		iChannelID;                        //ͨ����
	INT         		iDevice;                           //�豸���
	INT         		iYtNo;                             //ң�����
	INT         		iYkNo;                             //ң�ص��
	INT         		iAdTimesMax;                       //����������
	INT         		iCanAddAlgID;                      //�ɵ����޶�Ӧң��ID
	INT         		iCanSubAlgID;                      //�ɵ����޶�Ӧң��ID
	INT         		iUpValAlgID;                       //ָ���ֵ��Ӧң��ID
	INT         		iModifyAdd;                        //��������������й�
	INT         		iModifySub;                        //��������������й�
	INT         		iProcCapAlgID;                     //����������Ӧң��ID
	INT         		iQAlgID;                           //�޹���Ӧң��ID
	FLOAT       		fFactor;                           //ң��ϵ��
	INT         		nonename2;                         //�����ֶ�
}AGC_PhvUnit;		//AGC����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��ֵ����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iType;                             //��ֵ����
	INT         		iVal;                              //���ζ�ֵ
}AGC_Parameter;		//AGC���ڲ���

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iType;                             //��������
	FLOAT       		fVal;                              //ֵ
	INT         		iAnalogID;                         //ң��ID
	SYSTEMTIME  		tSnapTime;                         //ʱ����
}AGC_Analog;		//AGC�������ң��

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��ֵ����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iType;                             //��������
	INT         		iVal;                              //ֵ
	INT         		iDigitID;                          //ң��ID
	SYSTEMTIME  		tSnapTime;                         //ʱ����
}AGC_Digit;		//AGC�������ң��

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
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	CHAR        		strText[256];                      //����
}AGC_LogRecord;		//AGC������־


#endif   //__DATABASEAGC_h__
