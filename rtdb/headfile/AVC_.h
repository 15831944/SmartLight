#ifndef __DATABASEAVC_h__
#define __DATABASEAVC_h__

#include "Rtdb_Struct.h"
#define DB_ID_OF_AVC                                51
//����ID
#define AVC_TAB_Group_ID                          	5101	//����
#define AVC_TAB_PhvUnit_ID                        	5102	//AVC����
#define AVC_TAB_Parameter_ID                      	5103	//AVC���ڲ���
#define AVC_TAB_Analog_ID                         	5104	//AVC�������ң��
#define AVC_TAB_Digit_ID                          	5105	//AVC�������ң��
#define AVC_TAB_Alarm_ID                          	5106	//AVC����
#define AVC_TAB_SvgUnit_ID                        	5107	//AVC�޹�����
#define AVC_TAB_FcUnit_ID                         	5108	//AVC����������
#define AVC_TAB_Xfmr_ID                           	5109	//AVC��ѹ��
#define AVC_TAB_LogRecord_ID                      	5110	//AVC������־
//���������ݿ��е�����
#define AVC_TAB_Group_INDEX                       	0	//����
#define AVC_TAB_PhvUnit_INDEX                     	1	//AVC����
#define AVC_TAB_Parameter_INDEX                   	2	//AVC���ڲ���
#define AVC_TAB_Analog_INDEX                      	3	//AVC�������ң��
#define AVC_TAB_Digit_INDEX                       	4	//AVC�������ң��
#define AVC_TAB_Alarm_INDEX                       	5	//AVC����
#define AVC_TAB_SvgUnit_INDEX                     	6	//AVC�޹�����
#define AVC_TAB_FcUnit_INDEX                      	7	//AVC����������
#define AVC_TAB_Xfmr_INDEX                        	8	//AVC��ѹ��
#define AVC_TAB_LogRecord_INDEX                   	9	//AVC������־

//������ֶ��ڱ�������(����)
#define AVC_TAB_Group_FIELD_ID32_INDEX                      	0	//�ڲ�ID32
#define AVC_TAB_Group_FIELD_strName_INDEX                   	1	//��������
#define AVC_TAB_Group_FIELD_iEnabled_INDEX                  	2	//���ñ�ʶ

//������ֶ��ڱ�������(AVC����)
#define AVC_TAB_PhvUnit_FIELD_ID32_INDEX                    	0	//�ڲ�ID32
#define AVC_TAB_PhvUnit_FIELD_strName_INDEX                 	1	//��������
#define AVC_TAB_PhvUnit_FIELD_iStatus_INDEX                 	2	//��������״̬
#define AVC_TAB_PhvUnit_FIELD_fValQ_INDEX                   	3	//�޹�(kVar)
#define AVC_TAB_PhvUnit_FIELD_fDisQ_INDEX                   	4	//�����޹�����
#define AVC_TAB_PhvUnit_FIELD_tExecTime_INDEX               	5	//ִ������ʱ��
#define AVC_TAB_PhvUnit_FIELD_iChannelStatus_INDEX          	6	//ͨ��״̬
#define AVC_TAB_PhvUnit_FIELD_iUpLock_INDEX                 	7	//������
#define AVC_TAB_PhvUnit_FIELD_iDownLock_INDEX               	8	//������
#define AVC_TAB_PhvUnit_FIELD_iFlag_INDEX                   	9	//��ʶ
#define AVC_TAB_PhvUnit_FIELD_iEnabled_INDEX                	10	//���ñ�ʶ
#define AVC_TAB_PhvUnit_FIELD_fRatingQ_INDEX                	11	//�����(kVar)
#define AVC_TAB_PhvUnit_FIELD_fAdMaxLimit_INDEX             	12	//���ڸ���(kVar)
#define AVC_TAB_PhvUnit_FIELD_fAdMinLimit_INDEX             	13	//���ڵ���(kVar)
#define AVC_TAB_PhvUnit_FIELD_fAdZone_INDEX                 	14	//��������(kVar)
#define AVC_TAB_PhvUnit_FIELD_iChannelID_INDEX              	15	//ͨ����
#define AVC_TAB_PhvUnit_FIELD_iDevice_INDEX                 	16	//�豸���
#define AVC_TAB_PhvUnit_FIELD_iYtNo_INDEX                   	17	//ң�����
#define AVC_TAB_PhvUnit_FIELD_fFactor_INDEX                 	18	//ң��ϵ��
#define AVC_TAB_PhvUnit_FIELD_iGroup_INDEX                  	19	//���
#define AVC_TAB_PhvUnit_FIELD_iType_INDEX                   	20	//��������
#define AVC_TAB_PhvUnit_FIELD_iPriority_INDEX               	21	//�������ȼ�
#define AVC_TAB_PhvUnit_FIELD_tSnapTime_INDEX               	22	//ʱ����
#define AVC_TAB_PhvUnit_FIELD_iAdTimesMax_INDEX             	23	//����������
#define AVC_TAB_PhvUnit_FIELD_iStatusDigitID_INDEX          	24	//����״̬��Ӧң��ID
#define AVC_TAB_PhvUnit_FIELD_iChannelDigitID_INDEX         	25	//ͨ����Ӧң��ID
#define AVC_TAB_PhvUnit_FIELD_iQAlgID_INDEX                 	26	//�޹���Ӧң��ID
#define AVC_TAB_PhvUnit_FIELD_iUpValAlgID_INDEX             	27	//ָ���ֵ��Ӧң��ID
#define AVC_TAB_PhvUnit_FIELD_iDisQAlgID_INDEX              	28	//�����޹���Ӧң��ID
#define AVC_TAB_PhvUnit_FIELD_fUpVal_INDEX                  	29	//�����ָ���ֵ
#define AVC_TAB_PhvUnit_FIELD_iQuality_INDEX                	30	//������

//������ֶ��ڱ�������(AVC���ڲ���)
#define AVC_TAB_Parameter_FIELD_ID32_INDEX                  	0	//�ڲ�ID32
#define AVC_TAB_Parameter_FIELD_strName_INDEX               	1	//��ֵ����
#define AVC_TAB_Parameter_FIELD_strKey_INDEX                	2	//�ؼ���
#define AVC_TAB_Parameter_FIELD_iType_INDEX                 	3	//��ֵ����
#define AVC_TAB_Parameter_FIELD_iVal_INDEX                  	4	//��ֵ

//������ֶ��ڱ�������(AVC�������ң��)
#define AVC_TAB_Analog_FIELD_ID32_INDEX                     	0	//�ڲ�ID32
#define AVC_TAB_Analog_FIELD_strName_INDEX                  	1	//����
#define AVC_TAB_Analog_FIELD_strKey_INDEX                   	2	//�ؼ���
#define AVC_TAB_Analog_FIELD_iType_INDEX                    	3	//��������
#define AVC_TAB_Analog_FIELD_fVal_INDEX                     	4	//ֵ
#define AVC_TAB_Analog_FIELD_iAnalogID_INDEX                	5	//ң��ID
#define AVC_TAB_Analog_FIELD_tSnapTime_INDEX                	6	//ʱ����
#define AVC_TAB_Analog_FIELD_iGroup_INDEX                   	7	//���
#define AVC_TAB_Analog_FIELD_iQuality_INDEX                 	8	//������

//������ֶ��ڱ�������(AVC�������ң��)
#define AVC_TAB_Digit_FIELD_ID32_INDEX                      	0	//�ڲ�ID32
#define AVC_TAB_Digit_FIELD_strName_INDEX                   	1	//��ֵ����
#define AVC_TAB_Digit_FIELD_strKey_INDEX                    	2	//�ؼ���
#define AVC_TAB_Digit_FIELD_iType_INDEX                     	3	//��������
#define AVC_TAB_Digit_FIELD_iVal_INDEX                      	4	//ֵ
#define AVC_TAB_Digit_FIELD_iDigitID_INDEX                  	5	//ң��ID
#define AVC_TAB_Digit_FIELD_tSnapTime_INDEX                 	6	//ʱ����
#define AVC_TAB_Digit_FIELD_iGroup_INDEX                    	7	//���
#define AVC_TAB_Digit_FIELD_iQuality_INDEX                  	8	//������

//������ֶ��ڱ�������(AVC����)
#define AVC_TAB_Alarm_FIELD_ID32_INDEX                      	0	//�ڲ�ID32
#define AVC_TAB_Alarm_FIELD_strName_INDEX                   	1	//��ֵ����
#define AVC_TAB_Alarm_FIELD_strKey_INDEX                    	2	//�ؼ���
#define AVC_TAB_Alarm_FIELD_iVal_INDEX                      	3	//ֵ
#define AVC_TAB_Alarm_FIELD_iDigitID_INDEX                  	4	//ң��ID
#define AVC_TAB_Alarm_FIELD_tSnapTime_INDEX                 	5	//ʱ����
#define AVC_TAB_Alarm_FIELD_iGroup_INDEX                    	6	//���

//������ֶ��ڱ�������(AVC�޹�����)
#define AVC_TAB_SvgUnit_FIELD_ID32_INDEX                    	0	//�ڲ�ID32
#define AVC_TAB_SvgUnit_FIELD_strName_INDEX                 	1	//��������
#define AVC_TAB_SvgUnit_FIELD_iStatus_INDEX                 	2	//��������״̬
#define AVC_TAB_SvgUnit_FIELD_fValQ_INDEX                   	3	//�޹�(kVar)
#define AVC_TAB_SvgUnit_FIELD_fDisQ_INDEX                   	4	//�����޹�����(kVar)
#define AVC_TAB_SvgUnit_FIELD_fUpVal_INDEX                  	5	//Ŀ��ָ������
#define AVC_TAB_SvgUnit_FIELD_tExecTime_INDEX               	6	//ִ������ʱ��
#define AVC_TAB_SvgUnit_FIELD_iChannelStatus_INDEX          	7	//ͨ��״̬
#define AVC_TAB_SvgUnit_FIELD_iUpLock_INDEX                 	8	//������
#define AVC_TAB_SvgUnit_FIELD_iDownLock_INDEX               	9	//������
#define AVC_TAB_SvgUnit_FIELD_iSvgExp_INDEX                 	10	//SVG����
#define AVC_TAB_SvgUnit_FIELD_iFlag_INDEX                   	11	//��ʶ
#define AVC_TAB_SvgUnit_FIELD_tSnapTime_INDEX               	12	//ʱ����
#define AVC_TAB_SvgUnit_FIELD_iEnabled_INDEX                	13	//���ñ�ʶ
#define AVC_TAB_SvgUnit_FIELD_fCapacity_INDEX               	14	//�����(kVar)
#define AVC_TAB_SvgUnit_FIELD_fAdMaxLimit_INDEX             	15	//���ڸ���(kVar)
#define AVC_TAB_SvgUnit_FIELD_fAdMinLimit_INDEX             	16	//���ڵ���(kVar)
#define AVC_TAB_SvgUnit_FIELD_iType_INDEX                   	17	//��������
#define AVC_TAB_SvgUnit_FIELD_iGroup_INDEX                  	18	//���
#define AVC_TAB_SvgUnit_FIELD_fRateVolt_INDEX               	19	//���ѹ
#define AVC_TAB_SvgUnit_FIELD_iChannelID_INDEX              	20	//ͨ����
#define AVC_TAB_SvgUnit_FIELD_iDevice_INDEX                 	21	//�豸���
#define AVC_TAB_SvgUnit_FIELD_iYtNo_INDEX                   	22	//ң�����
#define AVC_TAB_SvgUnit_FIELD_fYtFactor_INDEX               	23	//ң��ϵ��
#define AVC_TAB_SvgUnit_FIELD_iStatusDigitID_INDEX          	24	//����״̬��Ӧң��ID
#define AVC_TAB_SvgUnit_FIELD_iChannelDigitID_INDEX         	25	//ͨ����Ӧң��ID
#define AVC_TAB_SvgUnit_FIELD_iSvgExpDigitID_INDEX          	26	//SVG���϶�Ӧң��ID
#define AVC_TAB_SvgUnit_FIELD_iUpBlockDigitID_INDEX         	27	//��������Ӧң��ID
#define AVC_TAB_SvgUnit_FIELD_iDownBlockDigitID_INDEX       	28	//��������Ӧң��ID
#define AVC_TAB_SvgUnit_FIELD_iQAlgID_INDEX                 	29	//�޹���Ӧң��ID
#define AVC_TAB_SvgUnit_FIELD_iLimitSource_INDEX            	30	//������������Դ
#define AVC_TAB_SvgUnit_FIELD_iAdMaxLimitAlgID_INDEX        	31	//�������޶�Ӧң��ID
#define AVC_TAB_SvgUnit_FIELD_iAdMinLimitAlgID_INDEX        	32	//�������޶�Ӧң��ID
#define AVC_TAB_SvgUnit_FIELD_iCmdBaseValSource_INDEX       	33	//��ǰ�����ֵ��Դ
#define AVC_TAB_SvgUnit_FIELD_iUpValAlgID_INDEX             	34	//Ŀ��ָ�����Ͷ�Ӧң��ID
#define AVC_TAB_SvgUnit_FIELD_iDisQAlgID_INDEX              	35	//�����޹���Ӧң��ID
#define AVC_TAB_SvgUnit_FIELD_iQuality_INDEX                	36	//������

//������ֶ��ڱ�������(AVC����������)
#define AVC_TAB_FcUnit_FIELD_ID32_INDEX                     	0	//�ڲ�ID32
#define AVC_TAB_FcUnit_FIELD_strName_INDEX                  	1	//��������
#define AVC_TAB_FcUnit_FIELD_fValQ_INDEX                    	2	//��ǰ�޹�(kVar)
#define AVC_TAB_FcUnit_FIELD_iReqOn_INDEX                   	3	//��ҪͶ��
#define AVC_TAB_FcUnit_FIELD_iReqOff_INDEX                  	4	//��Ҫ�˳�
#define AVC_TAB_FcUnit_FIELD_iStatus_INDEX                  	5	//��������״̬
#define AVC_TAB_FcUnit_FIELD_iOnOffTimes_INDEX              	6	//����������Ͷ�д���
#define AVC_TAB_FcUnit_FIELD_iFlag_INDEX                    	7	//��ʶ
#define AVC_TAB_FcUnit_FIELD_fcurCap_INDEX                  	8	//�����(kVar)
#define AVC_TAB_FcUnit_FIELD_iGroup_INDEX                   	9	//���
#define AVC_TAB_FcUnit_FIELD_fOnffMaxTimes_INDEX            	10	//�������������Ͷ�д���
#define AVC_TAB_FcUnit_FIELD_tSnapTime_INDEX                	11	//ʱ����
#define AVC_TAB_FcUnit_FIELD_iEnabled_INDEX                 	12	//���ñ�ʶ
#define AVC_TAB_FcUnit_FIELD_iStatusDigitID_INDEX           	13	//����״̬��Ӧң��ID
#define AVC_TAB_FcUnit_FIELD_iOnOffTimesAlgID_INDEX         	14	//����������Ͷ�д�����ID
#define AVC_TAB_FcUnit_FIELD_iOnFlgDigitID_INDEX            	15	//��ҪͶ���ʶң��ID
#define AVC_TAB_FcUnit_FIELD_iOffFlgDigitID_INDEX           	16	//��Ҫ�˳���ʶң��ID
#define AVC_TAB_FcUnit_FIELD_iQuality_INDEX                 	17	//������
#define AVC_TAB_FcUnit_FIELD_iValAlgID_INDEX                	18	//��ǰ�޹�ң��ID

//������ֶ��ڱ�������(AVC��ѹ��)
#define AVC_TAB_Xfmr_FIELD_ID32_INDEX                       	0	//�ڲ�ID32
#define AVC_TAB_Xfmr_FIELD_strName_INDEX                    	1	//��������
#define AVC_TAB_Xfmr_FIELD_iStatus_INDEX                    	2	//��ѹ��״̬
#define AVC_TAB_Xfmr_FIELD_iCurTap_INDEX                    	3	//��ǰ��λ
#define AVC_TAB_Xfmr_FIELD_iDisTap_INDEX                    	4	//����ִ�еĵ�λ
#define AVC_TAB_Xfmr_FIELD_iHighBrk_INDEX                   	5	//��ѹ�࿪��״̬
#define AVC_TAB_Xfmr_FIELD_iLowBrk_INDEX                    	6	//��ѹ�࿪��״̬
#define AVC_TAB_Xfmr_FIELD_iChannelStatus_INDEX             	7	//ͨ��״̬
#define AVC_TAB_Xfmr_FIELD_iAlarmDownTapTip_INDEX           	8	//��������ѹ����
#define AVC_TAB_Xfmr_FIELD_iAlarmUpTapTip_INDEX             	9	//��������ѹ����
#define AVC_TAB_Xfmr_FIELD_fHighP_INDEX                     	10	//��ѹ���й�
#define AVC_TAB_Xfmr_FIELD_fHighQ_INDEX                     	11	//��ѹ���޹�
#define AVC_TAB_Xfmr_FIELD_fHighVoltage_INDEX               	12	//��ѹ���ѹ
#define AVC_TAB_Xfmr_FIELD_fHighCurrent_INDEX               	13	//��ѹ�����
#define AVC_TAB_Xfmr_FIELD_fLowP_INDEX                      	14	//��ѹ���й�
#define AVC_TAB_Xfmr_FIELD_fLowQ_INDEX                      	15	//��ѹ���޹�
#define AVC_TAB_Xfmr_FIELD_fLowVoltage_INDEX                	16	//��ѹ���ѹ
#define AVC_TAB_Xfmr_FIELD_fLowCurrent_INDEX                	17	//��ѹ�����
#define AVC_TAB_Xfmr_FIELD_iAdTimesThisDay_INDEX            	18	//������ڴ���
#define AVC_TAB_Xfmr_FIELD_iUpVoltBlock_INDEX               	19	//��ѹ����
#define AVC_TAB_Xfmr_FIELD_iDownVoltBlock_INDEX             	20	//��ѹ����
#define AVC_TAB_Xfmr_FIELD_iExption_INDEX                   	21	//��ѹ������
#define AVC_TAB_Xfmr_FIELD_tSnapTime_INDEX                  	22	//ʱ����
#define AVC_TAB_Xfmr_FIELD_iEnabled_INDEX                   	23	//���ñ�ʶ
#define AVC_TAB_Xfmr_FIELD_fCurCap_INDEX                    	24	//�����(kVar)
#define AVC_TAB_Xfmr_FIELD_fHighFrenqucy_INDEX              	25	//�Ƶ��
#define AVC_TAB_Xfmr_FIELD_fRatedHighVolt_INDEX             	26	//��ѹ����ѹ(kV)
#define AVC_TAB_Xfmr_FIELD_fRatedLowVolt_INDEX              	27	//��ѹ����ѹ(kV)
#define AVC_TAB_Xfmr_FIELD_iRatedTapPos_INDEX               	28	//���λ
#define AVC_TAB_Xfmr_FIELD_iAutoFlg_INDEX                   	29	//�Զ����ڱ�ʶ
#define AVC_TAB_Xfmr_FIELD_iType_INDEX                      	30	//�Զ����ڱ�ʶ
#define AVC_TAB_Xfmr_FIELD_iMaxTapLimit_INDEX               	31	//��ߵ��ڵ�λ����
#define AVC_TAB_Xfmr_FIELD_iMinTapLimit_INDEX               	32	//��͵��ڵ�λ����
#define AVC_TAB_Xfmr_FIELD_iFlag_INDEX                      	33	//��ʶ
#define AVC_TAB_Xfmr_FIELD_iGroup_INDEX                     	34	//���
#define AVC_TAB_Xfmr_FIELD_iChannelID_INDEX                 	35	//ͨ����
#define AVC_TAB_Xfmr_FIELD_iDevice_INDEX                    	36	//�豸���
#define AVC_TAB_Xfmr_FIELD_iYtNo_INDEX                      	37	//ң�����
#define AVC_TAB_Xfmr_FIELD_iYtFactor_INDEX                  	38	//ң��ϵ��
#define AVC_TAB_Xfmr_FIELD_iCurTapAlgID_INDEX               	39	//��ǰ��λң����ID
#define AVC_TAB_Xfmr_FIELD_iHighPAlgID_INDEX                	40	//��ѹ��ʵʱ�й�ң��ID
#define AVC_TAB_Xfmr_FIELD_iHigthQAlgID_INDEX               	41	//��ѹ��ʵʱ�޹�ң��ID
#define AVC_TAB_Xfmr_FIELD_iHighFrenqucyAlgID_INDEX         	42	//��ѹ��ʵʱƵ��ң��ID
#define AVC_TAB_Xfmr_FIELD_iHighVoltAlgID_INDEX             	43	//��ѹ���ѹң��ID
#define AVC_TAB_Xfmr_FIELD_iHighCurrentAlgID_INDEX          	44	//��ѹ�����ң��ID
#define AVC_TAB_Xfmr_FIELD_iLowPAlgID_INDEX                 	45	//��ѹ���й�ң��ID
#define AVC_TAB_Xfmr_FIELD_iLowQAlgID_INDEX                 	46	//��ѹ���޹�ң��ID
#define AVC_TAB_Xfmr_FIELD_iLowVoltAlgID_INDEX              	47	//��ѹ���ѹң��ID
#define AVC_TAB_Xfmr_FIELD_iLowCurrentAlgID_INDEX           	48	//��ѹ�����ң��ID
#define AVC_TAB_Xfmr_FIELD_iAdTimesAlgID_INDEX              	49	//������ڴ���ң��ID
#define AVC_TAB_Xfmr_FIELD_iDisTapAlgID_INDEX               	50	//����ִ�еĵ�λң��ID
#define AVC_TAB_Xfmr_FIELD_iHighBrkDigitID_INDEX            	51	//��ѹ�࿪��״̬ң��ID
#define AVC_TAB_Xfmr_FIELD_iLowBrkDigitID_INDEX             	52	//��ѹ�࿪��״̬ң��ID
#define AVC_TAB_Xfmr_FIELD_iChannelDigitID_INDEX            	53	//ͨ��״̬ң��ID
#define AVC_TAB_Xfmr_FIELD_iUpBlockDigitID_INDEX            	54	//��ѹ����ID
#define AVC_TAB_Xfmr_FIELD_iDownBlockDigitID_INDEX          	55	//��ѹ����ID
#define AVC_TAB_Xfmr_FIELD_iExceptionDigitID_INDEX          	56	//��ѹ������ң��ID
#define AVC_TAB_Xfmr_FIELD_iDownTapTipDigitID_INDEX         	57	//��������ѹ����ң��ID
#define AVC_TAB_Xfmr_FIELD_iUpTapTipDigitID_INDEX           	58	//��������ѹ����ң��ID
#define AVC_TAB_Xfmr_FIELD_iQuality_INDEX                   	59	//������

//������ֶ��ڱ�������(AVC������־)
#define AVC_TAB_LogRecord_FIELD_ID32_INDEX                  	0	//�ڲ�ID32
#define AVC_TAB_LogRecord_FIELD_tSnapTime_INDEX             	1	//ʱ����
#define AVC_TAB_LogRecord_FIELD_strText_INDEX               	2	//����



typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��������
	INT         		iEnabled;                          //���ñ�ʶ
}AVC_Group;		//����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��������
	INT         		iStatus;                           //��������״̬
	FLOAT       		fValQ;                             //�޹�(kVar)
	FLOAT       		fDisQ;                             //�����޹�����
	SYSTEMTIME  		tExecTime;                         //ִ������ʱ��
	INT         		iChannelStatus;                    //ͨ��״̬
	INT         		iUpLock;                           //������
	INT         		iDownLock;                         //������
	INT         		iFlag;                             //��ʶ
	INT         		iEnabled;                          //���ñ�ʶ
	FLOAT       		fRatingQ;                          //�����(kVar)
	FLOAT       		fAdMaxLimit;                       //���ڸ���(kVar)
	FLOAT       		fAdMinLimit;                       //���ڵ���(kVar)
	FLOAT       		fAdZone;                           //��������(kVar)
	INT         		iChannelID;                        //ͨ����
	INT         		iDevice;                           //�豸���
	INT         		iYtNo;                             //ң�����
	FLOAT       		fFactor;                           //ң��ϵ��
	UINT        		iGroup;                            //���
	INT         		iType;                             //��������
	INT         		iPriority;                         //�������ȼ�
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	INT         		iAdTimesMax;                       //����������
	INT         		iStatusDigitID;                    //����״̬��Ӧң��ID
	INT         		iChannelDigitID;                   //ͨ����Ӧң��ID
	INT         		iQAlgID;                           //�޹���Ӧң��ID
	INT         		iUpValAlgID;                       //ָ���ֵ��Ӧң��ID
	INT         		iDisQAlgID;                        //�����޹���Ӧң��ID
	FLOAT       		fUpVal;                            //�����ָ���ֵ
	INT         		iQuality;                          //������
}AVC_PhvUnit;		//AVC����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��ֵ����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iType;                             //��ֵ����
	INT         		iVal;                              //��ֵ
}AVC_Parameter;		//AVC���ڲ���

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iType;                             //��������
	FLOAT       		fVal;                              //ֵ
	INT         		iAnalogID;                         //ң��ID
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	UINT        		iGroup;                            //���
	INT         		iQuality;                          //������
}AVC_Analog;		//AVC�������ң��

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��ֵ����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iType;                             //��������
	INT         		iVal;                              //ֵ
	INT         		iDigitID;                          //ң��ID
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	UINT        		iGroup;                            //���
	INT         		iQuality;                          //������
}AVC_Digit;		//AVC�������ң��

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��ֵ����
	CHAR        		strKey[48];                        //�ؼ���
	INT         		iVal;                              //ֵ
	INT         		iDigitID;                          //ң��ID
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	UINT        		iGroup;                            //���
}AVC_Alarm;		//AVC����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��������
	INT         		iStatus;                           //��������״̬
	FLOAT       		fValQ;                             //�޹�(kVar)
	FLOAT       		fDisQ;                             //�����޹�����(kVar)
	FLOAT       		fUpVal;                            //Ŀ��ָ������
	SYSTEMTIME  		tExecTime;                         //ִ������ʱ��
	INT         		iChannelStatus;                    //ͨ��״̬
	INT         		iUpLock;                           //������
	INT         		iDownLock;                         //������
	INT         		iSvgExp;                           //SVG����
	INT         		iFlag;                             //��ʶ
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	INT         		iEnabled;                          //���ñ�ʶ
	FLOAT       		fCapacity;                         //�����(kVar)
	FLOAT       		fAdMaxLimit;                       //���ڸ���(kVar)
	FLOAT       		fAdMinLimit;                       //���ڵ���(kVar)
	INT         		iType;                             //��������
	UINT        		iGroup;                            //���
	FLOAT       		fRateVolt;                         //���ѹ
	INT         		iChannelID;                        //ͨ����
	INT         		iDevice;                           //�豸���
	INT         		iYtNo;                             //ң�����
	FLOAT       		fYtFactor;                         //ң��ϵ��
	INT         		iStatusDigitID;                    //����״̬��Ӧң��ID
	INT         		iChannelDigitID;                   //ͨ����Ӧң��ID
	INT         		iSvgExpDigitID;                    //SVG���϶�Ӧң��ID
	INT         		iUpBlockDigitID;                   //��������Ӧң��ID
	INT         		iDownBlockDigitID;                 //��������Ӧң��ID
	INT         		iQAlgID;                           //�޹���Ӧң��ID
	INT         		iLimitSource;                      //������������Դ
	INT         		iAdMaxLimitAlgID;                  //�������޶�Ӧң��ID
	INT         		iAdMinLimitAlgID;                  //�������޶�Ӧң��ID
	INT         		iCmdBaseValSource;                 //��ǰ�����ֵ��Դ
	INT         		iUpValAlgID;                       //Ŀ��ָ�����Ͷ�Ӧң��ID
	INT         		iDisQAlgID;                        //�����޹���Ӧң��ID
	INT         		iQuality;                          //������
}AVC_SvgUnit;		//AVC�޹�����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��������
	FLOAT       		fValQ;                             //��ǰ�޹�(kVar)
	INT         		iReqOn;                            //��ҪͶ��
	INT         		iReqOff;                           //��Ҫ�˳�
	INT         		iStatus;                           //��������״̬
	INT         		iOnOffTimes;                       //����������Ͷ�д���
	INT         		iFlag;                             //��ʶ
	FLOAT       		fcurCap;                           //�����(kVar)
	UINT        		iGroup;                            //���
	INT         		fOnffMaxTimes;                     //�������������Ͷ�д���
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	INT         		iEnabled;                          //���ñ�ʶ
	INT         		iStatusDigitID;                    //����״̬��Ӧң��ID
	INT         		iOnOffTimesAlgID;                  //����������Ͷ�д�����ID
	INT         		iOnFlgDigitID;                     //��ҪͶ���ʶң��ID
	INT         		iOffFlgDigitID;                    //��Ҫ�˳���ʶң��ID
	INT         		iQuality;                          //������
	INT         		iValAlgID;                         //��ǰ�޹�ң��ID
}AVC_FcUnit;		//AVC����������

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[96];                       //��������
	INT         		iStatus;                           //��ѹ��״̬
	INT         		iCurTap;                           //��ǰ��λ
	INT         		iDisTap;                           //����ִ�еĵ�λ
	INT         		iHighBrk;                          //��ѹ�࿪��״̬
	INT         		iLowBrk;                           //��ѹ�࿪��״̬
	INT         		iChannelStatus;                    //ͨ��״̬
	INT         		iAlarmDownTapTip;                  //��������ѹ����
	INT         		iAlarmUpTapTip;                    //��������ѹ����
	FLOAT       		fHighP;                            //��ѹ���й�
	FLOAT       		fHighQ;                            //��ѹ���޹�
	FLOAT       		fHighVoltage;                      //��ѹ���ѹ
	FLOAT       		fHighCurrent;                      //��ѹ�����
	FLOAT       		fLowP;                             //��ѹ���й�
	FLOAT       		fLowQ;                             //��ѹ���޹�
	FLOAT       		fLowVoltage;                       //��ѹ���ѹ
	FLOAT       		fLowCurrent;                       //��ѹ�����
	INT         		iAdTimesThisDay;                   //������ڴ���
	INT         		iUpVoltBlock;                      //��ѹ����
	INT         		iDownVoltBlock;                    //��ѹ����
	INT         		iExption;                          //��ѹ������
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	INT         		iEnabled;                          //���ñ�ʶ
	FLOAT       		fCurCap;                           //�����(kVar)
	FLOAT       		fHighFrenqucy;                     //�Ƶ��
	FLOAT       		fRatedHighVolt;                    //��ѹ����ѹ(kV)
	FLOAT       		fRatedLowVolt;                     //��ѹ����ѹ(kV)
	INT         		iRatedTapPos;                      //���λ
	INT         		iAutoFlg;                          //�Զ����ڱ�ʶ
	INT         		iType;                             //�Զ����ڱ�ʶ
	INT         		iMaxTapLimit;                      //��ߵ��ڵ�λ����
	INT         		iMinTapLimit;                      //��͵��ڵ�λ����
	INT         		iFlag;                             //��ʶ
	UINT        		iGroup;                            //���
	INT         		iChannelID;                        //ͨ����
	INT         		iDevice;                           //�豸���
	INT         		iYtNo;                             //ң�����
	INT         		iYtFactor;                         //ң��ϵ��
	INT         		iCurTapAlgID;                      //��ǰ��λң����ID
	INT         		iHighPAlgID;                       //��ѹ��ʵʱ�й�ң��ID
	INT         		iHigthQAlgID;                      //��ѹ��ʵʱ�޹�ң��ID
	INT         		iHighFrenqucyAlgID;                //��ѹ��ʵʱƵ��ң��ID
	INT         		iHighVoltAlgID;                    //��ѹ���ѹң��ID
	INT         		iHighCurrentAlgID;                 //��ѹ�����ң��ID
	INT         		iLowPAlgID;                        //��ѹ���й�ң��ID
	INT         		iLowQAlgID;                        //��ѹ���޹�ң��ID
	INT         		iLowVoltAlgID;                     //��ѹ���ѹң��ID
	INT         		iLowCurrentAlgID;                  //��ѹ�����ң��ID
	INT         		iAdTimesAlgID;                     //������ڴ���ң��ID
	INT         		iDisTapAlgID;                      //����ִ�еĵ�λң��ID
	INT         		iHighBrkDigitID;                   //��ѹ�࿪��״̬ң��ID
	INT         		iLowBrkDigitID;                    //��ѹ�࿪��״̬ң��ID
	INT         		iChannelDigitID;                   //ͨ��״̬ң��ID
	INT         		iUpBlockDigitID;                   //��ѹ����ID
	INT         		iDownBlockDigitID;                 //��ѹ����ID
	INT         		iExceptionDigitID;                 //��ѹ������ң��ID
	INT         		iDownTapTipDigitID;                //��������ѹ����ң��ID
	INT         		iUpTapTipDigitID;                  //��������ѹ����ң��ID
	INT         		iQuality;                          //������
}AVC_Xfmr;		//AVC��ѹ��

typedef struct 
{
	INT         		ID32;                              //�ڲ�ID32
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	CHAR        		strText[256];                      //����
}AVC_LogRecord;		//AVC������־


#endif   //__DATABASEAVC_h__
