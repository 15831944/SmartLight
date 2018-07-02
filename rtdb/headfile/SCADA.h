#ifndef __DATABASESCADA_h__
#define __DATABASESCADA_h__

#include "Rtdb_Struct.h"
#define DB_ID_OF_SCADA                              21
//����ID
#define SCADA_TAB_TUnit_ID                        	2101	//�ɼ���Ԫ��
#define SCADA_TAB_Channel_ID                      	2102	//ͨ����
#define SCADA_TAB_Analog_ID                       	2103	//ģ����
#define SCADA_TAB_Digit_ID                        	2104	//������
#define SCADA_TAB_Relay_ID                        	2105	//ң�ر�
#define SCADA_TAB_SetPoint_ID                     	2106	//ң����
#define SCADA_TAB_Protocol_ID                     	2107	//��Լ��
#define SCADA_TAB_AnalogStat_ID                   	2108	//ң��ͳ�Ʊ�
#define SCADA_TAB_DigitStat_ID                    	2109	//ң��ͳ�Ʊ�
#define SCADA_TAB_Global_ID                       	2110	//SCADAȫ�ֱ�����
#define SCADA_TAB_Formula_ID                      	2111	//���㹫ʽ��
//���������ݿ��е�����
#define SCADA_TAB_TUnit_INDEX                     	0	//�ɼ���Ԫ��
#define SCADA_TAB_Channel_INDEX                   	1	//ͨ����
#define SCADA_TAB_Analog_INDEX                    	2	//ģ����
#define SCADA_TAB_Digit_INDEX                     	3	//������
#define SCADA_TAB_Relay_INDEX                     	4	//ң�ر�
#define SCADA_TAB_SetPoint_INDEX                  	5	//ң����
#define SCADA_TAB_Protocol_INDEX                  	6	//��Լ��
#define SCADA_TAB_AnalogStat_INDEX                	7	//ң��ͳ�Ʊ�
#define SCADA_TAB_DigitStat_INDEX                 	8	//ң��ͳ�Ʊ�
#define SCADA_TAB_Global_INDEX                    	9	//SCADAȫ�ֱ�����
#define SCADA_TAB_Formula_INDEX                   	10	//���㹫ʽ��

//������ֶ��ڱ�������(�ɼ���Ԫ��)
#define SCADA_TAB_TUnit_FIELD_ID32_INDEX                    	0	//�ڲ�ID32
#define SCADA_TAB_TUnit_FIELD_uStationID_INDEX              	1	//������վID
#define SCADA_TAB_TUnit_FIELD_strID_INDEX                   	2	//����
#define SCADA_TAB_TUnit_FIELD_strName_INDEX                 	3	//����
#define SCADA_TAB_TUnit_FIELD_iType_INDEX                   	4	//����
#define SCADA_TAB_TUnit_FIELD_lFlag_INDEX                   	5	//��־
#define SCADA_TAB_TUnit_FIELD_iStationAddr_INDEX            	6	//��ַ��
#define SCADA_TAB_TUnit_FIELD_dvtype_INDEX                  	7	//�豸����
#define SCADA_TAB_TUnit_FIELD_dvno_INDEX                    	8	//�豸��
#define SCADA_TAB_TUnit_FIELD_iStatus_INDEX                 	9	//����״̬
#define SCADA_TAB_TUnit_FIELD_iStatusID_INDEX               	10	//�豸״̬ң�ŵ�
#define SCADA_TAB_TUnit_FIELD_iRecordFlag_INDEX             	11	//���ļ�¼��־
#define SCADA_TAB_TUnit_FIELD_iRecordBegin_INDEX            	12	//���ļ�¼��ʼʱ��
#define SCADA_TAB_TUnit_FIELD_iRecordEnd_INDEX              	13	//���ļ�¼����ʱ��
#define SCADA_TAB_TUnit_FIELD_iRecPacketFlg_INDEX           	14	//������Ϣ���俪��
#define SCADA_TAB_TUnit_FIELD_iRecPromptFlg_INDEX           	15	//��ʾ��Ϣ���俪��

//������ֶ��ڱ�������(ͨ����)
#define SCADA_TAB_Channel_FIELD_ID32_INDEX                  	0	//�ڲ�ID32
#define SCADA_TAB_Channel_FIELD_strName_INDEX               	1	//����
#define SCADA_TAB_Channel_FIELD_iFlag_INDEX                 	2	//��־λ
#define SCADA_TAB_Channel_FIELD_iType_INDEX                 	3	//ͨ������
#define SCADA_TAB_Channel_FIELD_uProtocolID_INDEX           	4	//ͨѶ��ԼID
#define SCADA_TAB_Channel_FIELD_strProtocolPara_INDEX       	5	//��Լ�����ļ�
#define SCADA_TAB_Channel_FIELD_strLinkParam_INDEX          	6	//��·����
#define SCADA_TAB_Channel_FIELD_iStatus_INDEX               	7	//����״̬
#define SCADA_TAB_Channel_FIELD_iStatusID_INDEX             	8	//����״̬ң��ID
#define SCADA_TAB_Channel_FIELD_tStartTime_INDEX            	9	//����ʱ��
#define SCADA_TAB_Channel_FIELD_strTransFile_INDEX          	10	//ת���ļ���
#define SCADA_TAB_Channel_FIELD_iRunAttr_INDEX              	11	//ͨ����̬����

//������ֶ��ڱ�������(ģ����)
#define SCADA_TAB_Analog_FIELD_ID32_INDEX                   	0	//�ڲ�ID32
#define SCADA_TAB_Analog_FIELD_uTuID_INDEX                  	1	//�ɼ���ԪID
#define SCADA_TAB_Analog_FIELD_strID_INDEX                  	2	//����
#define SCADA_TAB_Analog_FIELD_strName_INDEX                	3	//����
#define SCADA_TAB_Analog_FIELD_lDPUID_INDEX                 	4	//����DPU
#define SCADA_TAB_Analog_FIELD_iNoInDPU_INDEX               	5	//DPU�б��
#define SCADA_TAB_Analog_FIELD_fVal_INDEX                   	6	//����ֵ
#define SCADA_TAB_Analog_FIELD_iUnit_INDEX                  	7	//��λ
#define SCADA_TAB_Analog_FIELD_tSnapTime_INDEX              	8	//ʱ����
#define SCADA_TAB_Analog_FIELD_iQuality_INDEX               	9	//������
#define SCADA_TAB_Analog_FIELD_iIndex_INDEX                 	10	//��վ�����
#define SCADA_TAB_Analog_FIELD_FormulaID32_INDEX            	11	//��ʽ��ID
#define SCADA_TAB_Analog_FIELD_iZfID_INDEX                  	12	//ת��ID
#define SCADA_TAB_Analog_FIELD_iType_INDEX                  	13	//����
#define SCADA_TAB_Analog_FIELD_iDataType_INDEX              	14	//��������
#define SCADA_TAB_Analog_FIELD_fRaw_INDEX                   	15	//ԭʼֵ
#define SCADA_TAB_Analog_FIELD_fCoeff_INDEX                 	16	//ϵ��
#define SCADA_TAB_Analog_FIELD_fOffset_INDEX                	17	//ƫ����
#define SCADA_TAB_Analog_FIELD_lFlag_INDEX                  	18	//��̬��־
#define SCADA_TAB_Analog_FIELD_fLowL_INDEX                  	19	//��������
#define SCADA_TAB_Analog_FIELD_fHighL_INDEX                 	20	//��������
#define SCADA_TAB_Analog_FIELD_fLowL2_INDEX                 	21	//����������
#define SCADA_TAB_Analog_FIELD_fHighL2_INDEX                	22	//����������
#define SCADA_TAB_Analog_FIELD_fDeadSec_INDEX               	23	//����
#define SCADA_TAB_Analog_FIELD_fValidLL_INDEX               	24	//����ֵ����
#define SCADA_TAB_Analog_FIELD_fValidHL_INDEX               	25	//����ֵ����
#define SCADA_TAB_Analog_FIELD_fDayMaxV_INDEX               	26	//�����ֵ
#define SCADA_TAB_Analog_FIELD_fDayMinV_INDEX               	27	//����Сֵ
#define SCADA_TAB_Analog_FIELD_fDayAveV_INDEX               	28	//��ƽ��ֵ
#define SCADA_TAB_Analog_FIELD_tDayMaxTime_INDEX            	29	//�����ֵ����ʱ��
#define SCADA_TAB_Analog_FIELD_tDayMinTime_INDEX            	30	//����Сֵ����ʱ��
#define SCADA_TAB_Analog_FIELD_iAlmClass_INDEX              	31	//Խ�ޱ����ȼ�
#define SCADA_TAB_Analog_FIELD_iAlmClass2_INDEX             	32	//Խ���ޱ����ȼ�
#define SCADA_TAB_Analog_FIELD_dvtype_INDEX                 	33	//�豸����
#define SCADA_TAB_Analog_FIELD_dvno_INDEX                   	34	//�豸��
#define SCADA_TAB_Analog_FIELD_lDynaFlag_INDEX              	35	//��̬��־
#define SCADA_TAB_Analog_FIELD_iSavePeriod_INDEX            	36	//�洢����

//������ֶ��ڱ�������(������)
#define SCADA_TAB_Digit_FIELD_ID32_INDEX                    	0	//�ڲ�ID32
#define SCADA_TAB_Digit_FIELD_uTuID_INDEX                   	1	//�ɼ���ԪID
#define SCADA_TAB_Digit_FIELD_strID_INDEX                   	2	//����
#define SCADA_TAB_Digit_FIELD_strName_INDEX                 	3	//����
#define SCADA_TAB_Digit_FIELD_lDPUID_INDEX                  	4	//����DPU
#define SCADA_TAB_Digit_FIELD_iNoInDPU_INDEX                	5	//DPU�б��
#define SCADA_TAB_Digit_FIELD_iVal_INDEX                    	6	//����ֵ
#define SCADA_TAB_Digit_FIELD_tSnapTime_INDEX               	7	//ʱ����
#define SCADA_TAB_Digit_FIELD_iQuality_INDEX                	8	//������
#define SCADA_TAB_Digit_FIELD_iIndex_INDEX                  	9	//�ڳ�վ�����
#define SCADA_TAB_Digit_FIELD_iType_INDEX                   	10	//����
#define SCADA_TAB_Digit_FIELD_iRaw_INDEX                    	11	//ԭʼֵ
#define SCADA_TAB_Digit_FIELD_FormulaID32_INDEX             	12	//��ʽ��ID
#define SCADA_TAB_Digit_FIELD_iZfID_INDEX                   	13	//ת��ID
#define SCADA_TAB_Digit_FIELD_lFlag_INDEX                   	14	//��̬��־
#define SCADA_TAB_Digit_FIELD_iDataType_INDEX               	15	//��������
#define SCADA_TAB_Digit_FIELD_iLastTime_INDEX               	16	//ȥ��ʱ��(��)
#define SCADA_TAB_Digit_FIELD_iOpenSum_INDEX                	17	//ͳ�Ʒִ���
#define SCADA_TAB_Digit_FIELD_iCloseSum_INDEX               	18	//ͳ�ƺϴ���
#define SCADA_TAB_Digit_FIELD_iAlmClass_INDEX               	19	//�����ȼ�
#define SCADA_TAB_Digit_FIELD_lDynaFlag_INDEX               	20	//��̬��־
#define SCADA_TAB_Digit_FIELD_dvtype_INDEX                  	21	//�豸����
#define SCADA_TAB_Digit_FIELD_dvno_INDEX                    	22	//�豸��

//������ֶ��ڱ�������(ң�ر�)
#define SCADA_TAB_Relay_FIELD_ID32_INDEX                    	0	//�ڲ�ID32
#define SCADA_TAB_Relay_FIELD_uTuID_INDEX                   	1	//�ɼ���ԪID
#define SCADA_TAB_Relay_FIELD_strID_INDEX                   	2	//����
#define SCADA_TAB_Relay_FIELD_strName_INDEX                 	3	//����
#define SCADA_TAB_Relay_FIELD_lDPUID_INDEX                  	4	//����DPU
#define SCADA_TAB_Relay_FIELD_iNoInDPU_INDEX                	5	//DPU�б��
#define SCADA_TAB_Relay_FIELD_LogicalClose_INDEX            	6	//�߼��غ�
#define SCADA_TAB_Relay_FIELD_LogicalOpen_INDEX             	7	//�߼��ط�
#define SCADA_TAB_Relay_FIELD_ulYXID32_INDEX                	8	//��ң�ű��е�ID
#define SCADA_TAB_Relay_FIELD_selecttime_INDEX              	9	//ѡ��ʱ��
#define SCADA_TAB_Relay_FIELD_exectime_INDEX                	10	//ִ��ʱ��
#define SCADA_TAB_Relay_FIELD_iZF_INDEX                     	11	//�Ƿ�ת��
#define SCADA_TAB_Relay_FIELD_izfTuID_INDEX                 	12	//ת��Ŀ��ͨ�����
#define SCADA_TAB_Relay_FIELD_izfDPUID_INDEX                	13	//ת���豸��ַ
#define SCADA_TAB_Relay_FIELD_izfNoInDPU_INDEX              	14	//ת�����
#define SCADA_TAB_Relay_FIELD_izfProcessID_INDEX            	15	//ת������ID
#define SCADA_TAB_Relay_FIELD_iType_INDEX                   	16	//����
#define SCADA_TAB_Relay_FIELD_lFlag_INDEX                   	17	//��־
#define SCADA_TAB_Relay_FIELD_iDataType_INDEX               	18	//��������
#define SCADA_TAB_Relay_FIELD_iIndex_INDEX                  	19	//�ڳ�վ�е����
#define SCADA_TAB_Relay_FIELD_uAlarmClass_INDEX             	20	//�����ȼ�
#define SCADA_TAB_Relay_FIELD_iexecret_INDEX                	21	//ִ�н��
#define SCADA_TAB_Relay_FIELD_iMaxSelectTime_INDEX          	22	//ѡ�����ʱ��(s)
#define SCADA_TAB_Relay_FIELD_iMaxExecuteTime_INDEX         	23	//ִ�����ʱ��(s)
#define SCADA_TAB_Relay_FIELD_iCancelMaxTime_INDEX          	24	//�������ʱ��(s)

//������ֶ��ڱ�������(ң����)
#define SCADA_TAB_SetPoint_FIELD_ID32_INDEX                 	0	//�ڲ�ID32
#define SCADA_TAB_SetPoint_FIELD_uTuID_INDEX                	1	//�ɼ���ԪID
#define SCADA_TAB_SetPoint_FIELD_strID_INDEX                	2	//����
#define SCADA_TAB_SetPoint_FIELD_strName_INDEX              	3	//����
#define SCADA_TAB_SetPoint_FIELD_fVal_INDEX                 	4	//����ֵ
#define SCADA_TAB_SetPoint_FIELD_lDPUID_INDEX               	5	//����DPU
#define SCADA_TAB_SetPoint_FIELD_iNoInDPU_INDEX             	6	//DPU�б��
#define SCADA_TAB_SetPoint_FIELD_selecttime_INDEX           	7	//ѡ��ʱ��
#define SCADA_TAB_SetPoint_FIELD_exectime_INDEX             	8	//ִ��ʱ��
#define SCADA_TAB_SetPoint_FIELD_fFactor_INDEX              	9	//ң��ϵ��
#define SCADA_TAB_SetPoint_FIELD_ulYCID32_INDEX             	10	//��ң����е�ID
#define SCADA_TAB_SetPoint_FIELD_iIndex_INDEX               	11	//�ڳ�վ�е����
#define SCADA_TAB_SetPoint_FIELD_iZF_INDEX                  	12	//�Ƿ�ת��
#define SCADA_TAB_SetPoint_FIELD_izfTuID_INDEX              	13	//ת��Ŀ��ͨ�����
#define SCADA_TAB_SetPoint_FIELD_izfDPUID_INDEX             	14	//ת���豸��ַ
#define SCADA_TAB_SetPoint_FIELD_izfNoInDPU_INDEX           	15	//ת�����
#define SCADA_TAB_SetPoint_FIELD_izfProcessID_INDEX         	16	//ת������ID
#define SCADA_TAB_SetPoint_FIELD_lFlag_INDEX                	17	//��־
#define SCADA_TAB_SetPoint_FIELD_iDataType_INDEX            	18	//��������
#define SCADA_TAB_SetPoint_FIELD_iexecret_INDEX             	19	//ִ�н��

//������ֶ��ڱ�������(��Լ��)
#define SCADA_TAB_Protocol_FIELD_ID32_INDEX                 	0	//�ڲ�ID32
#define SCADA_TAB_Protocol_FIELD_strName_INDEX              	1	//����
#define SCADA_TAB_Protocol_FIELD_strDLLName_INDEX           	2	//��Լ����
#define SCADA_TAB_Protocol_FIELD_strClassName_INDEX         	3	//��Լ����
#define SCADA_TAB_Protocol_FIELD_iProtocolType_INDEX        	4	//��Լ����
#define SCADA_TAB_Protocol_FIELD_iProtocolSubType_INDEX     	5	//��Լ������
#define SCADA_TAB_Protocol_FIELD_strTemplate_INDEX          	6	//��Լģ���ļ�

//������ֶ��ڱ�������(ң��ͳ�Ʊ�)
#define SCADA_TAB_AnalogStat_FIELD_ID32_INDEX               	0	//�ڲ�ID32
#define SCADA_TAB_AnalogStat_FIELD_tTime_INDEX              	1	//ʱ����
#define SCADA_TAB_AnalogStat_FIELD_ulDayNormal_INDEX        	2	//�������ۼ�ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_ulDayHigh_INDEX          	3	//��Խ�����ۼ�ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_ulDayLow_INDEX           	4	//��Խ�����ۼ�ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_ulDayStop_INDEX          	5	//��ͣ���ۼ�ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_fDayHiRatio_INDEX        	6	//��Խ������
#define SCADA_TAB_AnalogStat_FIELD_fDayLowRatio_INDEX       	7	//��Խ������
#define SCADA_TAB_AnalogStat_FIELD_fDayNorRatio_INDEX       	8	//�պϸ���
#define SCADA_TAB_AnalogStat_FIELD_ulMonthNormal_INDEX      	9	//�������ۼ�ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_ulMonthHigh_INDEX        	10	//��Խ�����ۼ�ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_ulMonthLow_INDEX         	11	//��Խ�����ۼ�ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_ulMonthStop_INDEX        	12	//��ͣ���ۼ�ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_fMonthHiRatio_INDEX      	13	//��Խ������
#define SCADA_TAB_AnalogStat_FIELD_fMonthLowRatio_INDEX     	14	//��Խ������
#define SCADA_TAB_AnalogStat_FIELD_fMonthNorRatio_INDEX     	15	//�ºϸ���
#define SCADA_TAB_AnalogStat_FIELD_ulYearNormal_INDEX       	16	//�������ۼ�ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_ulYearHigh_INDEX         	17	//��Խ�����ۼ�ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_ulYearLow_INDEX          	18	//��Խ�����ۼ�ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_ulYearStop_INDEX         	19	//��ͣ���ۼ�ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_fYearHiRatio_INDEX       	20	//��Խ������
#define SCADA_TAB_AnalogStat_FIELD_fYearLowRatio_INDEX      	21	//��Խ������
#define SCADA_TAB_AnalogStat_FIELD_fYearNorRatio_INDEX      	22	//��ϸ���
#define SCADA_TAB_AnalogStat_FIELD_iHighL_INDEX             	23	//��Խ���޴���
#define SCADA_TAB_AnalogStat_FIELD_iLowL_INDEX              	24	//��Խ���޴���
#define SCADA_TAB_AnalogStat_FIELD_iStatTime_INDEX          	25	//ͳ�Ƽ��
#define SCADA_TAB_AnalogStat_FIELD_iOverMaxNum_INDEX        	26	//���Խ�޴���
#define SCADA_TAB_AnalogStat_FIELD_iTimeSpan_INDEX          	27	//�ۼ�ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_iOverNum_INDEX           	28	//Խ�޴���
#define SCADA_TAB_AnalogStat_FIELD_iOverMaxTime_INDEX       	29	//�Խ��ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_iOverTime_INDEX          	30	//Խ��ʱ��(��)
#define SCADA_TAB_AnalogStat_FIELD_iOverNumClass_INDEX      	31	//�ƴ�Խ�ޱ����ȼ�
#define SCADA_TAB_AnalogStat_FIELD_iOverTimeClass_INDEX     	32	//��ʱԽ�ޱ����ȼ�
#define SCADA_TAB_AnalogStat_FIELD_iDelayMax_INDEX          	33	//��ʱ����ʱ��
#define SCADA_TAB_AnalogStat_FIELD_iDelayTime_INDEX         	34	//��ʱ�ۼ�
#define SCADA_TAB_AnalogStat_FIELD_fhourMaxV_INDEX          	35	//Сʱ���
#define SCADA_TAB_AnalogStat_FIELD_fhourMinV_INDEX          	36	//Сʱ��С
#define SCADA_TAB_AnalogStat_FIELD_fhourAveV_INDEX          	37	//Сʱƽ��

//������ֶ��ڱ�������(ң��ͳ�Ʊ�)
#define SCADA_TAB_DigitStat_FIELD_ID32_INDEX                	0	//�ڲ�ID32
#define SCADA_TAB_DigitStat_FIELD_tTime_INDEX               	1	//ʱ����
#define SCADA_TAB_DigitStat_FIELD_tStartTime_INDEX          	2	//ͳ�ƿ�ʼʱ��
#define SCADA_TAB_DigitStat_FIELD_ulDayNormal_INDEX         	3	//������ʱ���ۼ�(��)
#define SCADA_TAB_DigitStat_FIELD_ulDayStop_INDEX           	4	//��ͣ��ʱ���ۼ�(��)
#define SCADA_TAB_DigitStat_FIELD_fDayNorRatio_INDEX        	5	//������ʱ��ٷ���
#define SCADA_TAB_DigitStat_FIELD_ulMonthNormal_INDEX       	6	//������ʱ���ۼ�(��)
#define SCADA_TAB_DigitStat_FIELD_ulMonthStop_INDEX         	7	//��ͣ��ʱ���ۼ�(��)
#define SCADA_TAB_DigitStat_FIELD_fMonthNorRatio_INDEX      	8	//������ʱ��ٷ���
#define SCADA_TAB_DigitStat_FIELD_ulYearNormal_INDEX        	9	//������ʱ���ۼ�(��)
#define SCADA_TAB_DigitStat_FIELD_ulYearStop_INDEX          	10	//��ͣ��ʱ���ۼ�(��)
#define SCADA_TAB_DigitStat_FIELD_fYearNorRatio_INDEX       	11	//������ʱ��ٷ���
#define SCADA_TAB_DigitStat_FIELD_iOCTime_INDEX             	12	//ͳ�Ƽ��(��)
#define SCADA_TAB_DigitStat_FIELD_iOCMaxNum_INDEX           	13	//����������������
#define SCADA_TAB_DigitStat_FIELD_iTimeSpan_INDEX           	14	//�ۼ�ʱ��(��)
#define SCADA_TAB_DigitStat_FIELD_iOCNum_INDEX              	15	//�豸��������
#define SCADA_TAB_DigitStat_FIELD_iMaxRunTime_INDEX         	16	//�����ʱ��(��)
#define SCADA_TAB_DigitStat_FIELD_iRunTime_INDEX            	17	//����ʱ��(��)
#define SCADA_TAB_DigitStat_FIELD_iOverNumClass_INDEX       	18	//�ƴ�Խ�ޱ����ȼ�
#define SCADA_TAB_DigitStat_FIELD_iOverTimeClass_INDEX      	19	//��ʱԽ�ޱ����ȼ�
#define SCADA_TAB_DigitStat_FIELD_iDelayMax_INDEX           	20	//��ʱ����ʱ��
#define SCADA_TAB_DigitStat_FIELD_iDelayTime_INDEX          	21	//��ʱ�ۼ�

//������ֶ��ڱ�������(SCADAȫ�ֱ�����)
#define SCADA_TAB_Global_FIELD_ID32_INDEX                   	0	//�ڲ�ID32
#define SCADA_TAB_Global_FIELD_iNetHeartPeriod_INDEX        	1	//��������(��)
#define SCADA_TAB_Global_FIELD_iNodeHeartPeriod_INDEX       	2	//�ڵ�����(��)
#define SCADA_TAB_Global_FIELD_iProcessHeartPeriod_INDEX    	3	//��������(��)
#define SCADA_TAB_Global_FIELD_iLogoutTime_INDEX            	4	//�޲���ע���ʱ��(m)
#define SCADA_TAB_Global_FIELD_iFormulaPeriod_INDEX         	5	//�޲���ע���ʱ��(m)

//������ֶ��ڱ�������(���㹫ʽ��)
#define SCADA_TAB_Formula_FIELD_ID32_INDEX                  	0	//�ڲ�ID32
#define SCADA_TAB_Formula_FIELD_strName_INDEX               	1	//����
#define SCADA_TAB_Formula_FIELD_iValID_INDEX                	2	//���ֵID
#define SCADA_TAB_Formula_FIELD_strExpression_INDEX         	3	//��ʽ���ʽ
#define SCADA_TAB_Formula_FIELD_lFlag_INDEX                 	4	//���ֵ����



typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	UINT        		uStationID;                        //������վID
	CHAR        		strID[20];                         //����
	CHAR        		strName[40];                       //����
	INT         		iType;                             //����
	INT         		lFlag;                             //��־
	INT         		iStationAddr;                      //��ַ��
	INT         		dvtype;                            //�豸����
	INT         		dvno;                              //�豸��
	INT         		iStatus;                           //����״̬
	INT         		iStatusID;                         //�豸״̬ң�ŵ�
	INT         		iRecordFlag;                       //���ļ�¼��־
	INT         		iRecordBegin;                      //���ļ�¼��ʼʱ��
	INT         		iRecordEnd;                        //���ļ�¼����ʱ��
	INT         		iRecPacketFlg;                     //������Ϣ���俪��
	INT         		iRecPromptFlg;                     //��ʾ��Ϣ���俪��
}SCADA_TUnit;		//�ɼ���Ԫ��

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[80];                       //����
	INT         		iFlag;                             //��־λ
	INT         		iType;                             //ͨ������
	INT         		uProtocolID;                       //ͨѶ��ԼID
	CHAR        		strProtocolPara[128];              //��Լ�����ļ�
	CHAR        		strLinkParam[128];                 //��·����
	INT         		iStatus;                           //����״̬
	INT         		iStatusID;                         //����״̬ң��ID
	SYSTEMTIME  		tStartTime;                        //����ʱ��
	CHAR        		strTransFile[40];                  //ת���ļ���
	INT         		iRunAttr;                          //ͨ����̬����
}SCADA_Channel;		//ͨ����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	UINT        		uTuID;                             //�ɼ���ԪID
	CHAR        		strID[32];                         //����
	CHAR        		strName[80];                       //����
	INT         		lDPUID;                            //����DPU
	INT         		iNoInDPU;                          //DPU�б��
	FLOAT       		fVal;                              //����ֵ
	INT         		iUnit;                             //��λ
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	INT         		iQuality;                          //������
	INT         		iIndex;                            //��վ�����
	INT         		FormulaID32;                       //��ʽ��ID
	INT         		iZfID;                             //ת��ID
	INT         		iType;                             //����
	INT         		iDataType;                         //��������
	FLOAT       		fRaw;                              //ԭʼֵ
	FLOAT       		fCoeff;                            //ϵ��
	FLOAT       		fOffset;                           //ƫ����
	INT         		lFlag;                             //��̬��־
	FLOAT       		fLowL;                             //��������
	FLOAT       		fHighL;                            //��������
	FLOAT       		fLowL2;                            //����������
	FLOAT       		fHighL2;                           //����������
	FLOAT       		fDeadSec;                          //����
	FLOAT       		fValidLL;                          //����ֵ����
	FLOAT       		fValidHL;                          //����ֵ����
	FLOAT       		fDayMaxV;                          //�����ֵ
	FLOAT       		fDayMinV;                          //����Сֵ
	FLOAT       		fDayAveV;                          //��ƽ��ֵ
	SYSTEMTIME  		tDayMaxTime;                       //�����ֵ����ʱ��
	SYSTEMTIME  		tDayMinTime;                       //����Сֵ����ʱ��
	UINT        		iAlmClass;                         //Խ�ޱ����ȼ�
	UINT        		iAlmClass2;                        //Խ���ޱ����ȼ�
	UINT        		dvtype;                            //�豸����
	UINT        		dvno;                              //�豸��
	INT         		lDynaFlag;                         //��̬��־
	INT         		iSavePeriod;                       //�洢����
}SCADA_Analog;		//ģ����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	UINT        		uTuID;                             //�ɼ���ԪID
	CHAR        		strID[32];                         //����
	CHAR        		strName[80];                       //����
	INT         		lDPUID;                            //����DPU
	INT         		iNoInDPU;                          //DPU�б��
	INT         		iVal;                              //����ֵ
	SYSTEMTIME  		tSnapTime;                         //ʱ����
	INT         		iQuality;                          //������
	INT         		iIndex;                            //�ڳ�վ�����
	INT         		iType;                             //����
	INT         		iRaw;                              //ԭʼֵ
	INT         		FormulaID32;                       //��ʽ��ID
	INT         		iZfID;                             //ת��ID
	INT         		lFlag;                             //��̬��־
	INT         		iDataType;                         //��������
	INT         		iLastTime;                         //ȥ��ʱ��(��)
	INT         		iOpenSum;                          //ͳ�Ʒִ���
	INT         		iCloseSum;                         //ͳ�ƺϴ���
	UINT        		iAlmClass;                         //�����ȼ�
	INT         		lDynaFlag;                         //��̬��־
	UINT        		dvtype;                            //�豸����
	UINT        		dvno;                              //�豸��
}SCADA_Digit;		//������

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	UINT        		uTuID;                             //�ɼ���ԪID
	CHAR        		strID[32];                         //����
	CHAR        		strName[80];                       //����
	INT         		lDPUID;                            //����DPU
	INT         		iNoInDPU;                          //DPU�б��
	UINT        		LogicalClose;                      //�߼��غ�
	UINT        		LogicalOpen;                       //�߼��ط�
	INT         		ulYXID32;                          //��ң�ű��е�ID
	SYSTEMTIME  		selecttime;                        //ѡ��ʱ��
	SYSTEMTIME  		exectime;                          //ִ��ʱ��
	INT         		iZF;                               //�Ƿ�ת��
	UINT        		izfTuID;                           //ת��Ŀ��ͨ�����
	UINT        		izfDPUID;                          //ת���豸��ַ
	UINT        		izfNoInDPU;                        //ת�����
	INT         		izfProcessID;                      //ת������ID
	INT         		iType;                             //����
	INT         		lFlag;                             //��־
	INT         		iDataType;                         //��������
	INT         		iIndex;                            //�ڳ�վ�е����
	INT         		uAlarmClass;                       //�����ȼ�
	UINT        		iexecret;                          //ִ�н��
	INT         		iMaxSelectTime;                    //ѡ�����ʱ��(s)
	INT         		iMaxExecuteTime;                   //ִ�����ʱ��(s)
	INT         		iCancelMaxTime;                    //�������ʱ��(s)
}SCADA_Relay;		//ң�ر�

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	UINT        		uTuID;                             //�ɼ���ԪID
	CHAR        		strID[32];                         //����
	CHAR        		strName[80];                       //����
	FLOAT       		fVal;                              //����ֵ
	INT         		lDPUID;                            //����DPU
	INT         		iNoInDPU;                          //DPU�б��
	SYSTEMTIME  		selecttime;                        //ѡ��ʱ��
	SYSTEMTIME  		exectime;                          //ִ��ʱ��
	FLOAT       		fFactor;                           //ң��ϵ��
	INT         		ulYCID32;                          //��ң����е�ID
	INT         		iIndex;                            //�ڳ�վ�е����
	INT         		iZF;                               //�Ƿ�ת��
	UINT        		izfTuID;                           //ת��Ŀ��ͨ�����
	UINT        		izfDPUID;                          //ת���豸��ַ
	UINT        		izfNoInDPU;                        //ת�����
	INT         		izfProcessID;                      //ת������ID
	INT         		lFlag;                             //��־
	INT         		iDataType;                         //��������
	UINT        		iexecret;                          //ִ�н��
}SCADA_SetPoint;		//ң����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[24];                       //����
	CHAR        		strDLLName[48];                    //��Լ����
	CHAR        		strClassName[48];                  //��Լ����
	INT         		iProtocolType;                     //��Լ����
	INT         		iProtocolSubType;                  //��Լ������
	CHAR        		strTemplate[48];                   //��Լģ���ļ�
}SCADA_Protocol;		//��Լ��

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	SYSTEMTIME  		tTime;                             //ʱ����
	INT         		ulDayNormal;                       //�������ۼ�ʱ��(��)
	INT         		ulDayHigh;                         //��Խ�����ۼ�ʱ��(��)
	INT         		ulDayLow;                          //��Խ�����ۼ�ʱ��(��)
	INT         		ulDayStop;                         //��ͣ���ۼ�ʱ��(��)
	FLOAT       		fDayHiRatio;                       //��Խ������
	FLOAT       		fDayLowRatio;                      //��Խ������
	FLOAT       		fDayNorRatio;                      //�պϸ���
	INT         		ulMonthNormal;                     //�������ۼ�ʱ��(��)
	INT         		ulMonthHigh;                       //��Խ�����ۼ�ʱ��(��)
	INT         		ulMonthLow;                        //��Խ�����ۼ�ʱ��(��)
	INT         		ulMonthStop;                       //��ͣ���ۼ�ʱ��(��)
	FLOAT       		fMonthHiRatio;                     //��Խ������
	FLOAT       		fMonthLowRatio;                    //��Խ������
	FLOAT       		fMonthNorRatio;                    //�ºϸ���
	INT         		ulYearNormal;                      //�������ۼ�ʱ��(��)
	INT         		ulYearHigh;                        //��Խ�����ۼ�ʱ��(��)
	INT         		ulYearLow;                         //��Խ�����ۼ�ʱ��(��)
	INT         		ulYearStop;                        //��ͣ���ۼ�ʱ��(��)
	FLOAT       		fYearHiRatio;                      //��Խ������
	FLOAT       		fYearLowRatio;                     //��Խ������
	FLOAT       		fYearNorRatio;                     //��ϸ���
	INT         		iHighL;                            //��Խ���޴���
	INT         		iLowL;                             //��Խ���޴���
	INT         		iStatTime;                         //ͳ�Ƽ��
	INT         		iOverMaxNum;                       //���Խ�޴���
	INT         		iTimeSpan;                         //�ۼ�ʱ��(��)
	INT         		iOverNum;                          //Խ�޴���
	INT         		iOverMaxTime;                      //�Խ��ʱ��(��)
	INT         		iOverTime;                         //Խ��ʱ��(��)
	INT         		iOverNumClass;                     //�ƴ�Խ�ޱ����ȼ�
	INT         		iOverTimeClass;                    //��ʱԽ�ޱ����ȼ�
	INT         		iDelayMax;                         //��ʱ����ʱ��
	INT         		iDelayTime;                        //��ʱ�ۼ�
	FLOAT       		fhourMaxV;                         //Сʱ���
	FLOAT       		fhourMinV;                         //Сʱ��С
	FLOAT       		fhourAveV;                         //Сʱƽ��
}SCADA_AnalogStat;		//ң��ͳ�Ʊ�

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	SYSTEMTIME  		tTime;                             //ʱ����
	SYSTEMTIME  		tStartTime;                        //ͳ�ƿ�ʼʱ��
	INT         		ulDayNormal;                       //������ʱ���ۼ�(��)
	INT         		ulDayStop;                         //��ͣ��ʱ���ۼ�(��)
	FLOAT       		fDayNorRatio;                      //������ʱ��ٷ���
	INT         		ulMonthNormal;                     //������ʱ���ۼ�(��)
	INT         		ulMonthStop;                       //��ͣ��ʱ���ۼ�(��)
	FLOAT       		fMonthNorRatio;                    //������ʱ��ٷ���
	INT         		ulYearNormal;                      //������ʱ���ۼ�(��)
	INT         		ulYearStop;                        //��ͣ��ʱ���ۼ�(��)
	FLOAT       		fYearNorRatio;                     //������ʱ��ٷ���
	INT         		iOCTime;                           //ͳ�Ƽ��(��)
	INT         		iOCMaxNum;                         //����������������
	INT         		iTimeSpan;                         //�ۼ�ʱ��(��)
	INT         		iOCNum;                            //�豸��������
	INT         		iMaxRunTime;                       //�����ʱ��(��)
	INT         		iRunTime;                          //����ʱ��(��)
	INT         		iOverNumClass;                     //�ƴ�Խ�ޱ����ȼ�
	INT         		iOverTimeClass;                    //��ʱԽ�ޱ����ȼ�
	INT         		iDelayMax;                         //��ʱ����ʱ��
	INT         		iDelayTime;                        //��ʱ�ۼ�
}SCADA_DigitStat;		//ң��ͳ�Ʊ�

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	INT         		iNetHeartPeriod;                   //��������(��)
	INT         		iNodeHeartPeriod;                  //�ڵ�����(��)
	INT         		iProcessHeartPeriod;               //��������(��)
	INT         		iLogoutTime;                       //�޲���ע���ʱ��(m)
	INT         		iFormulaPeriod;                    //�޲���ע���ʱ��(m)
}SCADA_Global;		//SCADAȫ�ֱ�����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[40];                       //����
	UINT        		iValID;                            //���ֵID
	CHAR        		strExpression[240];                //��ʽ���ʽ
	INT         		lFlag;                             //���ֵ����
}SCADA_Formula;		//���㹫ʽ��


#endif   //__DATABASESCADA_h__
