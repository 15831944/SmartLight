#ifndef __DATABASESYSTEM_h__
#define __DATABASESYSTEM_h__

#include "Rtdb_Struct.h"
#define DB_ID_OF_SYSTEM                             1
//����ID
#define SYSTEM_TAB_NetNode_ID                     	101	//�ڵ��
#define SYSTEM_TAB_Process_ID                     	102	//���̱�
#define SYSTEM_TAB_User_ID                        	103	//�û���
#define SYSTEM_TAB_UserGroup_ID                   	104	//�û����
#define SYSTEM_TAB_Formula_ID                     	105	//��ʽ��
#define SYSTEM_TAB_Global_ID                      	106	//ȫ�ֱ�����
#define SYSTEM_TAB_AppConfig_ID                   	107	//Ӧ��������Ϣ
//���������ݿ��е�����
#define SYSTEM_TAB_NetNode_INDEX                  	0	//�ڵ��
#define SYSTEM_TAB_Process_INDEX                  	1	//���̱�
#define SYSTEM_TAB_User_INDEX                     	2	//�û���
#define SYSTEM_TAB_UserGroup_INDEX                	3	//�û����
#define SYSTEM_TAB_Formula_INDEX                  	4	//��ʽ��
#define SYSTEM_TAB_Global_INDEX                   	5	//ȫ�ֱ�����
#define SYSTEM_TAB_AppConfig_INDEX                	6	//Ӧ��������Ϣ

//������ֶ��ڱ�������(�ڵ��)
#define SYSTEM_TAB_NetNode_FIELD_ID32_INDEX                 	0	//�ڲ�ID32
#define SYSTEM_TAB_NetNode_FIELD_strName_INDEX              	1	//����
#define SYSTEM_TAB_NetNode_FIELD_strDescription_INDEX       	2	//����
#define SYSTEM_TAB_NetNode_FIELD_strIP1_INDEX               	3	//1��IP��ַ
#define SYSTEM_TAB_NetNode_FIELD_strIP2_INDEX               	4	//2��IP��ַ
#define SYSTEM_TAB_NetNode_FIELD_iType_INDEX                	5	//����
#define SYSTEM_TAB_NetNode_FIELD_iAllowProcess_INDEX        	6	//��ɽ���
#define SYSTEM_TAB_NetNode_FIELD_iActiveNet_INDEX           	7	//�����
#define SYSTEM_TAB_NetNode_FIELD_tStartTime_INDEX           	8	//����ʱ��
#define SYSTEM_TAB_NetNode_FIELD_tActiveTime_INDEX          	9	//�ʱ��
#define SYSTEM_TAB_NetNode_FIELD_iStatus_INDEX              	10	//����״̬
#define SYSTEM_TAB_NetNode_FIELD_uLoginUserID_INDEX         	11	//��ǰ�û�
#define SYSTEM_TAB_NetNode_FIELD_tCheckRightTime_INDEX      	12	//���һ�β���ʱ��
#define SYSTEM_TAB_NetNode_FIELD_uMaxMsgID_INDEX            	13	//������Ϣ���ID
#define SYSTEM_TAB_NetNode_FIELD_AOR_INDEX                  	14	//��������
#define SYSTEM_TAB_NetNode_FIELD_iCPU_INDEX                 	15	//CPUʹ�����
#define SYSTEM_TAB_NetNode_FIELD_iDisk_INDEX                	16	//����ʹ�����
#define SYSTEM_TAB_NetNode_FIELD_iMemory_INDEX              	17	//�ڴ�ʹ�����
#define SYSTEM_TAB_NetNode_FIELD_lWatchNode1_INDEX          	18	//�໤�ڵ�1
#define SYSTEM_TAB_NetNode_FIELD_lWatchNode2_INDEX          	19	//�໤�ڵ�2
#define SYSTEM_TAB_NetNode_FIELD_iHeart_INDEX               	20	//�ڵ�����

//������ֶ��ڱ�������(���̱�)
#define SYSTEM_TAB_Process_FIELD_ID32_INDEX                 	0	//�ڲ�ID32
#define SYSTEM_TAB_Process_FIELD_iProcessID_INDEX           	1	//����ID
#define SYSTEM_TAB_Process_FIELD_iType_INDEX                	2	//��������
#define SYSTEM_TAB_Process_FIELD_uNodeID_INDEX              	3	//�����ڵ�ID
#define SYSTEM_TAB_Process_FIELD_iMainWnd_INDEX             	4	//�����ھ��
#define SYSTEM_TAB_Process_FIELD_iFlag_INDEX                	5	//��־
#define SYSTEM_TAB_Process_FIELD_tStartTime_INDEX           	6	//����ʱ��
#define SYSTEM_TAB_Process_FIELD_tActiveTime_INDEX          	7	//����ʱ��
#define SYSTEM_TAB_Process_FIELD_iHeart_INDEX               	8	//��������

//������ֶ��ڱ�������(�û���)
#define SYSTEM_TAB_User_FIELD_ID32_INDEX                    	0	//�ڲ�ID32
#define SYSTEM_TAB_User_FIELD_strName_INDEX                 	1	//�û���
#define SYSTEM_TAB_User_FIELD_strPasswd_INDEX               	2	//����
#define SYSTEM_TAB_User_FIELD_iGroupFlag_INDEX              	3	//�û����־
#define SYSTEM_TAB_User_FIELD_strCard_INDEX                 	4	//����
#define SYSTEM_TAB_User_FIELD_strTelephone_INDEX            	5	//��ϵ�绰
#define SYSTEM_TAB_User_FIELD_nRuningUserGroup_INDEX        	6	//����ʱ�û���
#define SYSTEM_TAB_User_FIELD_nRuningUser_INDEX             	7	//����ʱ�û�

//������ֶ��ڱ�������(�û����)
#define SYSTEM_TAB_UserGroup_FIELD_ID32_INDEX               	0	//�ڲ�ID32
#define SYSTEM_TAB_UserGroup_FIELD_strName_INDEX            	1	//�û�����
#define SYSTEM_TAB_UserGroup_FIELD_iRightsFlag1_INDEX       	2	//Ȩ�ޱ�־1
#define SYSTEM_TAB_UserGroup_FIELD_iRightsFlag2_INDEX       	3	//Ȩ�ޱ�־2
#define SYSTEM_TAB_UserGroup_FIELD_iRightsFlag3_INDEX       	4	//Ȩ�ޱ�־3
#define SYSTEM_TAB_UserGroup_FIELD_iRightsFlag4_INDEX       	5	//Ȩ�ޱ�־4

//������ֶ��ڱ�������(��ʽ��)
#define SYSTEM_TAB_Formula_FIELD_ID32_INDEX                 	0	//�ڲ�ID32
#define SYSTEM_TAB_Formula_FIELD_strName_INDEX              	1	//����
#define SYSTEM_TAB_Formula_FIELD_strExpression_INDEX        	2	//��ʽ���ʽ
#define SYSTEM_TAB_Formula_FIELD_iVariableNumber_INDEX      	3	//��������
#define SYSTEM_TAB_Formula_FIELD_strVariableArray_INDEX     	4	//��������
#define SYSTEM_TAB_Formula_FIELD_nDataBaseID_INDEX          	5	//���ݿ�ID
#define SYSTEM_TAB_Formula_FIELD_lFlag_INDEX                	6	//��־

//������ֶ��ڱ�������(ȫ�ֱ�����)
#define SYSTEM_TAB_Global_FIELD_ID32_INDEX                  	0	//�ڲ�ID32
#define SYSTEM_TAB_Global_FIELD_iNetHeartPeriod_INDEX       	1	//��������(��)
#define SYSTEM_TAB_Global_FIELD_iNodeHeartPeriod_INDEX      	2	//�ڵ�����(��)
#define SYSTEM_TAB_Global_FIELD_iProcessHeartPeriod_INDEX   	3	//��������(��)
#define SYSTEM_TAB_Global_FIELD_iLogoutTime_INDEX           	4	//�޲���ע���ʱ��(m)
#define SYSTEM_TAB_Global_FIELD_iFormulaPeriod_INDEX        	5	//��ʽ����ʱ��(m)

//������ֶ��ڱ�������(Ӧ��������Ϣ)
#define SYSTEM_TAB_AppConfig_FIELD_ID32_INDEX               	0	//�ڲ�ID32
#define SYSTEM_TAB_AppConfig_FIELD_iProcessType_INDEX       	1	//��������
#define SYSTEM_TAB_AppConfig_FIELD_strKey_INDEX             	2	//������
#define SYSTEM_TAB_AppConfig_FIELD_iValue_INDEX             	3	//����ֵ
#define SYSTEM_TAB_AppConfig_FIELD_strValue_INDEX           	4	//�ַ���ֵ



typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[20];                       //����
	CHAR        		strDescription[40];                //����
	CHAR        		strIP1[20];                        //1��IP��ַ
	CHAR        		strIP2[20];                        //2��IP��ַ
	INT         		iType;                             //����
	INT         		iAllowProcess;                     //��ɽ���
	INT         		iActiveNet;                        //�����
	SYSTEMTIME  		tStartTime;                        //����ʱ��
	SYSTEMTIME  		tActiveTime;                       //�ʱ��
	INT         		iStatus;                           //����״̬
	UINT        		uLoginUserID;                      //��ǰ�û�
	INT         		tCheckRightTime;                   //���һ�β���ʱ��
	INT         		uMaxMsgID;                         //������Ϣ���ID
	INT         		AOR;                               //��������
	INT         		iCPU;                              //CPUʹ�����
	INT         		iDisk;                             //����ʹ�����
	INT         		iMemory;                           //�ڴ�ʹ�����
	INT         		lWatchNode1;                       //�໤�ڵ�1
	INT         		lWatchNode2;                       //�໤�ڵ�2
	INT         		iHeart;                            //�ڵ�����
}SYSTEM_NetNode;		//�ڵ��

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	INT         		iProcessID;                        //����ID
	INT         		iType;                             //��������
	UINT        		uNodeID;                           //�����ڵ�ID
	INT         		iMainWnd;                          //�����ھ��
	INT         		iFlag;                             //��־
	SYSTEMTIME  		tStartTime;                        //����ʱ��
	SYSTEMTIME  		tActiveTime;                       //����ʱ��
	INT         		iHeart;                            //��������
}SYSTEM_Process;		//���̱�

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[16];                       //�û���
	CHAR        		strPasswd[40];                     //����
	UINT        		iGroupFlag;                        //�û����־
	CHAR        		strCard[20];                       //����
	CHAR        		strTelephone[20];                  //��ϵ�绰
	UINT        		nRuningUserGroup;                  //����ʱ�û���
	UINT        		nRuningUser;                       //����ʱ�û�
}SYSTEM_User;		//�û���

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[16];                       //�û�����
	INT         		iRightsFlag1;                      //Ȩ�ޱ�־1
	INT         		iRightsFlag2;                      //Ȩ�ޱ�־2
	INT         		iRightsFlag3;                      //Ȩ�ޱ�־3
	INT         		iRightsFlag4;                      //Ȩ�ޱ�־4
}SYSTEM_UserGroup;		//�û����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	CHAR        		strName[40];                       //����
	CHAR        		strExpression[240];                //��ʽ���ʽ
	INT         		iVariableNumber;                   //��������
	CHAR        		strVariableArray[600];             //��������
	UINT        		nDataBaseID;                       //���ݿ�ID
	UINT        		lFlag;                             //��־
}SYSTEM_Formula;		//��ʽ��

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	INT         		iNetHeartPeriod;                   //��������(��)
	INT         		iNodeHeartPeriod;                  //�ڵ�����(��)
	INT         		iProcessHeartPeriod;               //��������(��)
	INT         		iLogoutTime;                       //�޲���ע���ʱ��(m)
	INT         		iFormulaPeriod;                    //��ʽ����ʱ��(m)
}SYSTEM_Global;		//ȫ�ֱ�����

typedef struct 
{
	UINT        		ID32;                              //�ڲ�ID32
	INT         		iProcessType;                      //��������
	CHAR        		strKey[32];                        //������
	INT         		iValue;                            //����ֵ
	CHAR        		strValue[100];                     //�ַ���ֵ
}SYSTEM_AppConfig;		//Ӧ��������Ϣ


#endif   //__DATABASESYSTEM_h__
