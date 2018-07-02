#ifndef __AFX_RC_CONST_H_2004_04_05__
#define __AFX_RC_CONST_H_2004_04_05__

#include "ElementOpr.h"
/*
	typedef struct 
	{
		UINT        		ID32;                              //�ڲ�ID32
		CHAR        		strName[40];                       //����
		INT         		iStationAddr;                      //վַ
		CHAR        		strSerial[420];                    //ң������
		INT         		iFlag;                             //��־
	}SCADA_RcSerial;		//ң�����б�
*/

//iFlag
#define OneStation_750_DEVICE_SUPPLY	1	//��վ750ȫ�������͵����У�ÿ��վ���ҽ���һ����
#define OneStation_750_DEVICE_STOP		2	//��վ750ȫ������ͣ������
				//��վ750ȫ������ͣ�����е�ѡ������ɳ����Զ��γ�
				//վ�������俪��ͣ������Ҳ���ø÷�ʽ�����������Ŀ�������һ����2��
#define PART_10_DEVICE_CLASSIC			3	//10kv����Ʊ
#define PART_10_DEVICE_GENERIC			4	//10kvһ��Ʊ
#define PART_750_DEVICE_SUPPLY			5	//750���ֿ����͵�����
#define PART_750_DEVICE_STOP			6	//750���ֿ���ͣ������
//#define Zone_750_DEVICE_STOP			7	//վ�������俪��ͣ������(һ��վ��������ͣ������)
											//վ�������俪��ͣ�����е�ѡ������ɳ����Զ��γ�
											//����վ��������ͣ������,����������������ͣ������
#define OneStation_DEVICE_FG			8	//��վ�豸����

//strSerial
//��PART_DEVICE_CLOSE��PART_DEVICE_OPEN
//��С��ң�ص�š���־��ѡ�У���Ŀ�ꡢ��ʱ ...

//��OneStation_750_DEVICE_CLOSE��OneStation_750_DEVICE_STOP
//��С
//����ң�ص�š���־��ѡ�С��������������ͣ�����բ1ң�ص�š���բ2ң�ص�š���ʱ 
// ... 
//����ң�ص�š���־��ѡ�С��������������ͣ�����բ1ң�ص�š���բ2ң�ص�š���ʱ 
//��ĸ����ң�ص�š���־�����룩����ʱ
#define Breaker_Select				1	//ѡ��
#define Breaker_Substitute			(1<<1)	//�����
#define Breaker_BP					(1<<2)	//��ĸ����
#define Breaker_Source				(1<<3)	//�ܿ��أ���բ5 1��
#define Breaker_Feeder				(1<<4)	//��բ���أ���բ1Ϊ��ĸ��բ����բ2Ϊ������բ��
#define NoDoSoftJudge				(1<<5)	//�������������������ж�
#define RCFail_NoContinue			(1<<6)	//ң��ʧ�ܲ��ٽ�������ң��
/*
//ң�ر������Ͷ��壬����ͬʱ���ԭ�����
#define RC_ATRESIA_LOCAL			1		//�͵�
#define RC_ATRESIA_SAME				(1<<1)	//����λ��Ŀ��λ��ͬ
#define RC_ATRESIA_MASK				(1<<2)	//��������
#define RC_ATRESIA_CARD				(1<<3)	//���ع���
#define RC_ATRESIA_TEST				(1<<4)	//�ֳ����ں�λ
#define RC_ATRESIA_GGEAR			(1<<5)	//�ص����ڷ�λ
#define RC_ATRESIA_RCING			(1<<6)	//ң�ؽ�����
#define RC_ATRESIA_ABNORMAL			(1<<7)	//����λ���쳣
#define RC_ATRESIA_GRANTED			(1<<8)	//��������Ȩ
#define RC_ATRESIA_UNGRANTED		(1<<9)	//����δ��Ȩ
#define RC_ENABLE_GOHOME			(1<<15)	//��Ҫ����
*/
struct tabRCHead
{
	int  ID;
	char strStationName[40];
	char strEditUserName[20];
	char strOperUserName[20]; 
	char strRCStartTime[40];
	char strRCEndTime[40];
};

struct tabRCContent
{
	int  ID;
	int  iNo;
	char strContent[120];
	char strResultName[8];
};

typedef XArray<tabRCContent, tabRCContent&> CtabRCContentArray;

#endif // __AFX_RC_CONST_H_2004_04_05__