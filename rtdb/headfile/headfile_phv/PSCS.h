#ifndef __DATABASEPSCS_h__
#define __DATABASEPSCS_h__

#include "Rtdb_Struct.h"
#define DB_ID_OF_PSCS                               41
//����ID
#define PSCS_TAB_LineInfo_ID                      	4101	//��·
#define PSCS_TAB_Parameter_ID                     	4102	//PSCS���ڲ���
#define PSCS_TAB_Analog_ID                        	4103	//PSCS�������ң��
#define PSCS_TAB_Digit_ID                         	4104	//PSCS�������ң��
#define PSCS_TAB_Setting_ID                       	4105	//PSCS���붨ֵ����
#define PSCS_TAB_Alarm_ID                         	4106	//PSCS����
#define PSCS_TAB_LogRecord_ID                     	4107	//PSCS������־
//���������ݿ��е�����
#define PSCS_TAB_LineInfo_INDEX                   	0	//��·
#define PSCS_TAB_Parameter_INDEX                  	1	//PSCS���ڲ���
#define PSCS_TAB_Analog_INDEX                     	2	//PSCS�������ң��
#define PSCS_TAB_Digit_INDEX                      	3	//PSCS�������ң��
#define PSCS_TAB_Setting_INDEX                    	4	//PSCS���붨ֵ����
#define PSCS_TAB_Alarm_INDEX                      	5	//PSCS����
#define PSCS_TAB_LogRecord_INDEX                  	6	//PSCS������־

//������ֶ��ڱ�������(��·)
#define PSCS_TAB_LineInfo_FIELD_ID32_INDEX                  	0	//�ڲ�ID32
#define PSCS_TAB_LineInfo_FIELD_strName_INDEX               	1	//��·����
#define PSCS_TAB_LineInfo_FIELD_fValP_INDEX                 	2	//�й�
#define PSCS_TAB_LineInfo_FIELD_iStatus_INDEX               	3	//��·����״̬
#define PSCS_TAB_LineInfo_FIELD_iIsCut_INDEX                	4	//��ǰ�Ƿ��г�
#define PSCS_TAB_LineInfo_FIELD_iRefuseAlarm_INDEX          	5	//�ܾ���������
#define PSCS_TAB_LineInfo_FIELD_tSnapTime_INDEX             	6	//ʱ��
#define PSCS_TAB_LineInfo_FIELD_tCutTime_INDEX              	7	//�г�ʱ��
#define PSCS_TAB_LineInfo_FIELD_fValUab_INDEX               	8	//�ߵ�ѹUab
#define PSCS_TAB_LineInfo_FIELD_fValIa_INDEX                	9	//A�����Ia
#define PSCS_TAB_LineInfo_FIELD_fCutP_INDEX                 	10	//��ǰ���й���
#define PSCS_TAB_LineInfo_FIELD_iType_INDEX                 	11	//��������
#define PSCS_TAB_LineInfo_FIELD_iChannelStatus_INDEX        	12	//ͨ��״̬
#define PSCS_TAB_LineInfo_FIELD_iFlag_INDEX                 	13	//��ʶ
#define PSCS_TAB_LineInfo_FIELD_iPowerAlgID_INDEX           	14	//�й���Ӧң��ID
#define PSCS_TAB_LineInfo_FIELD_iIaAlgID_INDEX              	15	//A�������Ӧң��ID
#define PSCS_TAB_LineInfo_FIELD_iCutPAlgID_INDEX            	16	//���г�����ң��ID
#define PSCS_TAB_LineInfo_FIELD_fCapictiyP_INDEX            	17	//��·�����
#define PSCS_TAB_LineInfo_FIELD_fUabAlgID_INDEX             	18	//�ߵ�ѹUabң��ID
#define PSCS_TAB_LineInfo_FIELD_fJustOnIa_INDEX             	19	//��Ͷ�˵�����ֵ(A)
#define PSCS_TAB_LineInfo_FIELD_iLogicDigitID_INDEX         	20	//����ѹ��ң��ID
#define PSCS_TAB_LineInfo_FIELD_iCutDigitID_INDEX           	21	//���л�ң��ID
#define PSCS_TAB_LineInfo_FIELD_iRefuseAlarmDigitID_INDEX   	22	//�ܾ���������ID
#define PSCS_TAB_LineInfo_FIELD_iStatusDigitID_INDEX        	23	//����λ��ң��ID
#define PSCS_TAB_LineInfo_FIELD_iChannelDigitID_INDEX       	24	//ͨ����Ӧң��ID
#define PSCS_TAB_LineInfo_FIELD_iChannelID_INDEX            	25	//ͨ����
#define PSCS_TAB_LineInfo_FIELD_iDevice_INDEX               	26	//�豸���
#define PSCS_TAB_LineInfo_FIELD_iYkNo_INDEX                 	27	//ң�ص��
#define PSCS_TAB_LineInfo_FIELD_iOffCode_INDEX              	28	//�Ͽ�������ֵ(10����)
#define PSCS_TAB_LineInfo_FIELD_iOnCode_INDEX               	29	//�պϿ�����ֵ(10����)
#define PSCS_TAB_LineInfo_FIELD_fValQ_INDEX                 	30	//�޹�
#define PSCS_TAB_LineInfo_FIELD_iQAlgID_INDEX               	31	//�޹���Ӧң��ID
#define PSCS_TAB_LineInfo_FIELD_iLogicYBVal_INDEX           	32	//����ѹ��ֵ

//������ֶ��ڱ�������(PSCS���ڲ���)
#define PSCS_TAB_Parameter_FIELD_ID32_INDEX                 	0	//�ڲ�ID32
#define PSCS_TAB_Parameter_FIELD_strName_INDEX              	1	//��ֵ����
#define PSCS_TAB_Parameter_FIELD_strKey_INDEX               	2	//�ؼ���
#define PSCS_TAB_Parameter_FIELD_iType_INDEX                	3	//��ֵ����
#define PSCS_TAB_Parameter_FIELD_iVal_INDEX                 	4	//���ζ�ֵ

//������ֶ��ڱ�������(PSCS�������ң��)
#define PSCS_TAB_Analog_FIELD_ID32_INDEX                    	0	//�ڲ�ID32
#define PSCS_TAB_Analog_FIELD_strName_INDEX                 	1	//����
#define PSCS_TAB_Analog_FIELD_strKey_INDEX                  	2	//�ؼ���
#define PSCS_TAB_Analog_FIELD_iType_INDEX                   	3	//��������
#define PSCS_TAB_Analog_FIELD_fVal_INDEX                    	4	//ֵ
#define PSCS_TAB_Analog_FIELD_iAnalogID_INDEX               	5	//ң��ID
#define PSCS_TAB_Analog_FIELD_tSnapTime_INDEX               	6	//ʱ����

//������ֶ��ڱ�������(PSCS�������ң��)
#define PSCS_TAB_Digit_FIELD_ID32_INDEX                     	0	//�ڲ�ID32
#define PSCS_TAB_Digit_FIELD_strName_INDEX                  	1	//��ֵ����
#define PSCS_TAB_Digit_FIELD_strKey_INDEX                   	2	//�ؼ���
#define PSCS_TAB_Digit_FIELD_iType_INDEX                    	3	//��������
#define PSCS_TAB_Digit_FIELD_iVal_INDEX                     	4	//ֵ
#define PSCS_TAB_Digit_FIELD_iDigitID_INDEX                 	5	//ң��ID
#define PSCS_TAB_Digit_FIELD_tSnapTime_INDEX                	6	//ʱ����

//������ֶ��ڱ�������(PSCS���붨ֵ����)
#define PSCS_TAB_Setting_FIELD_ID32_INDEX                   	0	//�ڲ�ID32
#define PSCS_TAB_Setting_FIELD_strName_INDEX                	1	//��ֵ����
#define PSCS_TAB_Setting_FIELD_strKey_INDEX                 	2	//�ؼ���
#define PSCS_TAB_Setting_FIELD_iType_INDEX                  	3	//��������
#define PSCS_TAB_Setting_FIELD_fVal_INDEX                   	4	//ֵ
#define PSCS_TAB_Setting_FIELD_iAnalogID_INDEX              	5	//ң��ID
#define PSCS_TAB_Setting_FIELD_tSnapTime_INDEX              	6	//ʱ����

//������ֶ��ڱ�������(PSCS����)
#define PSCS_TAB_Alarm_FIELD_ID32_INDEX                     	0	//�ڲ�ID32
#define PSCS_TAB_Alarm_FIELD_strName_INDEX                  	1	//��ֵ����
#define PSCS_TAB_Alarm_FIELD_strKey_INDEX                   	2	//�ؼ���
#define PSCS_TAB_Alarm_FIELD_iVal_INDEX                     	3	//ֵ
#define PSCS_TAB_Alarm_FIELD_iDigitID_INDEX                 	4	//ң��ID
#define PSCS_TAB_Alarm_FIELD_tSnapTime_INDEX                	5	//ʱ����

//������ֶ��ڱ�������(PSCS������־)
#define PSCS_TAB_LogRecord_FIELD_ID32_INDEX                 	0	//�ڲ�ID32
#define PSCS_TAB_LogRecord_FIELD_tSnapTime_INDEX            	1	//ʱ����
#define PSCS_TAB_LogRecord_FIELD_strText_INDEX              	2	//����



typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��·����
	FLOAT       		fValP;                             //�й�
	UINT        		iStatus;                           //��·����״̬
	UINT        		iIsCut;                            //��ǰ�Ƿ��г�
	UINT        		iRefuseAlarm;                      //�ܾ���������
	SYSTEMTIME  		tSnapTime;                         //ʱ��
	SYSTEMTIME  		tCutTime;                          //�г�ʱ��
	FLOAT       		fValUab;                           //�ߵ�ѹUab
	FLOAT       		fValIa;                            //A�����Ia
	FLOAT       		fCutP;                             //��ǰ���й���
	UINT        		iType;                             //��������
	INT         		iChannelStatus;                    //ͨ��״̬
	UINT        		iFlag;                             //��ʶ
	INT         		iPowerAlgID;                       //�й���Ӧң��ID
	INT         		iIaAlgID;                          //A�������Ӧң��ID
	INT         		iCutPAlgID;                        //���г�����ң��ID
	FLOAT       		fCapictiyP;                        //��·�����
	INT         		fUabAlgID;                         //�ߵ�ѹUabң��ID
	FLOAT       		fJustOnIa;                         //��Ͷ�˵�����ֵ(A)
	INT         		iLogicDigitID;                     //����ѹ��ң��ID
	INT         		iCutDigitID;                       //���л�ң��ID
	INT         		iRefuseAlarmDigitID;               //�ܾ���������ID
	INT         		iStatusDigitID;                    //����λ��ң��ID
	INT         		iChannelDigitID;                   //ͨ����Ӧң��ID
	INT         		iChannelID;                        //ͨ����
	INT         		iDevice;                           //�豸���
	INT         		iYkNo;                             //ң�ص��
	INT         		iOffCode;                          //�Ͽ�������ֵ(10����)
	INT         		iOnCode;                           //�պϿ�����ֵ(10����)
	FLOAT       		fValQ;                             //�޹�
	INT         		iQAlgID;                           //�޹���Ӧң��ID
	UINT        		iLogicYBVal;                       //����ѹ��ֵ
}PSCS_LineInfo;		//��·

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��ֵ����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iType;                             //��ֵ����
	INT         		iVal;                              //���ζ�ֵ
}PSCS_Parameter;		//PSCS���ڲ���

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iType;                             //��������
	FLOAT       		fVal;                              //ֵ
	INT         		iAnalogID;                         //ң��ID
	SYSTEMTIME  		tSnapTime;                         //ʱ����
}PSCS_Analog;		//PSCS�������ң��

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��ֵ����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iType;                             //��������
	INT         		iVal;                              //ֵ
	INT         		iDigitID;                          //ң��ID
	SYSTEMTIME  		tSnapTime;                         //ʱ����
}PSCS_Digit;		//PSCS�������ң��

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��ֵ����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iType;                             //��������
	FLOAT       		fVal;                              //ֵ
	INT         		iAnalogID;                         //ң��ID
	SYSTEMTIME  		tSnapTime;                         //ʱ����
}PSCS_Setting;		//PSCS���붨ֵ����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��ֵ����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iVal;                              //ֵ
	INT         		iDigitID;                          //ң��ID
	SYSTEMTIME  		tSnapTime;                         //ʱ����
}PSCS_Alarm;		//PSCS����

typedef struct 
{
	INT         		ID32;                              //�ڲ�ID32
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	CHAR        		strText[256];                      //����
}PSCS_LogRecord;		//PSCS������־


#endif   //__DATABASEPSCS_h__
