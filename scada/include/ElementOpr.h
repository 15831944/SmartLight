// ElementOpr.h: interface for the CElementOpr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTOPR_H__70CA61CE_7315_4899_8713_1A14156D3E40__INCLUDED_)
#define AFX_ELEMENTOPR_H__70CA61CE_7315_4899_8713_1A14156D3E40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PasEnv.h"
#include "MultiDataBase.h"
#include "SCADA.h"
#include "NET.h"
#include "TOPO.h"

#ifdef UNIX_ENV
#include "xstring.h"
#define CUIntArray XUIntArray
#endif

//ң�ر������Ͷ��壬����ͬʱ���ԭ�����
#define RC_ATRESIA_LOCAL			1		//�͵�
#define RC_ATRESIA_SAME				(1<<1)	//����λ��Ŀ��λ��ͬ
#define RC_ATRESIA_CARD				(1<<2)	//���ع���
#define RC_ATRESIA_TEST				(1<<3)	//�ֳ����ں�λ
#define RC_ATRESIA_RCING			(1<<4)	//ң�ؽ�����
#define RC_ATRESIA_ABNORMAL			(1<<5)	//����λ���쳣
#define RC_ATRESIA_GRANTED			(1<<6)	//��������Ȩ
#define RC_ATRESIA_UNGRANTED		(1<<7)	//����δ��Ȩ
#define RC_ATRESIA_SCABNORMAL		(1<<8)	//�ֳ�λ���쳣
#define RC_ATRESIA_COMERR			(1<<9)	//װ��ͨѶ�ж�
#define RC_ATRESIA_FAULT			(1<<10)	//��������
#define RC_ATRESIA_SPIRING			(1<<11)	//����δ����
#define RC_ATRESIA_GROUNDLINE		(1<<12)	//δ����ӵ���
#define RC_ATRESIA_Formula			(1<<13)	//��ʽ����
#define RC_ATRESIA_NORIGHT			(1<<14)	//��ң��Ȩ��
#define RC_ATRESIA_NOWATCH			(1<<15)	//�໤ϯδ������ؽ���
#define RC_ATRESIA_NOWATCHRIGHT		(1<<16)	//�໤ϯ�û��޼໤Ȩ

//���±����ǿ��Խ����
#define RC_ATRESIA_LOOP				(1<<20)	//���ܱջ�����
#define RC_ATRESIA_GGEAR			(1<<21)	//�ص����ڷ�λ
#define RC_ATRESIA_GEAROPR			(1<<22)	//���ܴ���ϵص�
#define RC_ATRESIA_SWITCHOPR		(1<<23)	//���ܴ����ɲ�����բ

//װ�ø�����ж�
#define RC_ENABLE_NONEEDRESET		(1<<25)	//���踴��

#define  SECTION_TYPE_NULL		0		//��Ч���
#define  SECTION_TYPE_LINE		1		//��·���
#define  SECTION_TYPE_TRFM		2		//��ѹ�����
#define  SECTION_TYPE_LD		3		//���ɼ��
#define  SECTION_TYPE_SO		4		//��Դ���

typedef struct 
{
	UINT         		uDigitCloseID;                  //��λ
	INT         		uDigitCloseIndex;               
	UINT         		uDigitOpenID;	                //��λ
	INT         		uDigitOpenIndex;	   
	
	UINT         		uDigitSCCloseID;				//�ֳ���λ
	INT         		uDigitSCCloseIndex;					
	UINT         		uDigitSCOpenID;					//�ֳ���λ
	INT         		uDigitSCOpenIndex;	
				
	UINT         		uDigitLocalID;	                //�͵�λ
	INT         		uDigitLocalIndex;	            
	UINT         		uDigitSpringID;	                //���ɴ���״̬
	INT         		uDigitSpringIndex;
	
	UINT				uDigitFaultID;					//�¹���
	INT					uDigitFaultIndex;	
	UINT				uDigitComErrID;					//ͨѶ�ж�
	INT					uDigitComErrIndex;	
}TabCbDigitInfo;		//���ر�����Ϣ

typedef struct 
{
	UINT         		uElementID;                      //�豸ID32
	CUIntArray			uT1EleID;						 //����1���������豸
	CUIntArray			iT1EleIndex;					 //����1���������豸�±�
	CUIntArray			uT1Terminal;					 //����1�����豸�Ķ���
	CUIntArray			uT2EleID;						 //����2���������豸
	CUIntArray			iT2EleIndex;					 //����2���������豸�±�
	CUIntArray			uT2Terminal;					 //����2�����豸�Ķ���
	BOOL				bScan;
}tabElementTopo;		//�豸���ӹ�ϵ
class AFX_EXT_CLASS CElementOpr  
{
public:
	CElementOpr(CMultiDataBase *pMultiDataBase);
	CElementOpr();
	~CElementOpr();
 	BOOL RtdbInit(CMultiDataBase *pMultiDataBase);
	
	//�����豸�Ƿ����
	//����˵����
	//uElePointID	- ����ң�ű�λ�Ŀ��صĺ�λң�ŵ��
	//bOpen			- ���ش�
	//bFault		- �¹ʵ�բ
	//bBroatCast	- �㲥������
	BOOL SetElementIfOperate(BOOL bBroatCast=FALSE, BOOL bReArray=FALSE, UINT uElePointID=0, BOOL bOpen=FALSE, BOOL bFault=FALSE);
	//���������Զ�����
	BOOL  SetScadaMapToCB();

	//�������,����ֵ�ǿ��ص�բ���豸��,�������0xFFFFFFFF����
	BOOL GetSectionByEleID(UINT uEleID, CUIntArray& uEleArray, CUIntArray& uEleIndexArray );
	//�������������
	INT  AnalyseSectionType(CUIntArray& uEleIndexArray);
	//�����������Ϣ:�����ء���������Ƶ��豸��(��·����ѹ������Դ�����ɵ�)
	BOOL  AnalyseSectionInfo(CUIntArray& uEleIndexArray, int& iBreakerIndex, int& iDeviceIndex);
	//ȡ��ѹ���ĸ�ѹ�൵λ���
	void  GetTrfmHTapInfo(UINT uTrfmID32, NET_tapty &tapty);

	BOOL NetTopoRepair();
	
	//VQC�����˽ӿں���
	//������ѹ���Ĳ������  ���еı�ѹ������һ���м���0xFFFFFFFF����
	void  AnalyseTrfmRun(UINT uSTID32, CUIntArray& uTrfmArray);
	//���Һ�ĳ̨��ѹ�����е����б�ѹ��
	void  AnalyseTrfmRun(NET_trfm *pTrfm, CUIntArray& uTrfmArray, BOOL bFindBYQ=TRUE);
	//�����豸�ĵ�ѹ�ȼ�
	INT	  GetElementKv(INT iEleIndex); 
	//��ȡ��ָ����ѹ��������һ���ĸ��
	BOOL  GetTrfmBus( UINT uTrfmID, CUIntArray& BusHArray, CUIntArray& BusMArray, CUIntArray& BusLArray );
	//��ȡ��ָ��ĸ��������һ��ĵ�����
	BOOL  GetBusCp( UINT uBusID, CUIntArray&  CpIDArray );
	//��ȡ��ָ������������Ŀ��� 
	BOOL  GetCpCb( UINT uCpID, UINT& uCbID );
	//��ȡ�豸����״̬
	BOOL  IsOperate(UINT uDeviceType, UINT DeviceID32);
	//�жϱ�ѹ���Ƿ����(1.�����硢2.��һ��(˫Ȧ)����(��Ȧ)���ش򿪱�ѹ��Ϊ����. ��û���ж��й�������)
	BOOL  IsEmptyRun(UINT uTrfmID);

	//С�����ӵص����˽ӿں���
	//����ĳ��ѹ�ȼ���ĸ�ߵ��������  �����粻�г� 0xFFFFFFFF����
	void  AnalyseBusRun(UINT uKvID32, CUIntArray& uBusArray);
	//���Һ�ĳĸ����һ�ڵ��ϵ�����ĸ��
	void  AnalyseBusRun(NET_bus *pBus, CUIntArray& uBusArray);
	//��ȡ��ָ��ĸ��������һ��ĸ���
	BOOL  GetBusLd( UINT uBusID, CUIntArray&  LdIDArray );


	//����ͳ��(�������ڵ���)
	BOOL  CalPowerLoss();
	//������Ϣ������Ĵ���(������Ϣ��ɾ��)
	BOOL  WriteProtectRelation();

	//�Ƿ���ң�ؿ��ص��ж�
	//uYKID32		ң�ص��  
	//iTargetVal	ң��Ŀ��ֵ
	//bSingleRC		���ر�־
	//����ֵ:0��ң�أ���0����ң�� -1�������� �����μ�����
	INT  CanRControl(UINT uYXID32, UINT uYKID32, INT iTargetVal, BOOL bSingleRC=TRUE);
	//���ر�����Ϣ�Ļ�ȡ
	BOOL GetCbDigitInfo(UINT uCBID, TabCbDigitInfo & CbDigitInfo);

	//�������(������п���)
	BOOL SoftJudge(BOOL bSet);

	//�豸���ޣ�����Ϊ��λ�ܹҼ�����
	//bRepair		 - ������޻�����ɼ���
	//bSetGroundGear - �Ƿ��Զ����ýӵص�բλ��
	//������ޣ��޵ص����ڼ����һ������(��բ)���ҵ���
	//��ɼ��ޣ��Զ������нӵ��߱�־ȥ��
	BOOL DeviceRepair(UINT DeviceEleID, BOOL bRepair, BOOL bSetGroundGear=FALSE);
private:
	CMultiDataBase	*m_pMultiDataBase;
	CDataBase		m_ScadaDB;
	CDataBase		m_NetDB;
	CDataBase		m_TopoDB;
	CDBTable		m_dvlinkTable;
	CDBTable		m_elementTable;
	CDBTable		m_cbTable;
	CDBTable		m_DigtalTable;
	TOPO_element	*m_pElement;
	TOPO_dvlink	    *m_pDvlink;
	NET_global		*m_pGlobal;
	int				lv_element;
	int				lv_dvlink;

	char			m_pszSysIniFile[256];
	bool			m_bNeedInit;

	tabElementTopo	*m_tabElementTopo;
	int				m_iElementLength;
	CUIntArray		m_uSOEleID;						 //��Դ�豸
	CUIntArray		m_iSOEleIndex;					 //��Դ�豸�±�
	CUIntArray		m_uCBEleID;						 //�����豸
	CUIntArray		m_iCBEleIndex;					 //�����豸�±�
	CUIntArray		m_uCBPointID;					 //����ң��λ��
	CUIntArray		m_uSCPointID;					 //�ֳ������ֳ�ң��λ��

	BOOL FormElementTopo();
	//���ÿ��طֺ�״̬
	BOOL SetCbIfOpen();
	//ʵʱ��ӿ�
	BOOL RtdbInit();
	//��ʽ�����ж�
	BOOL  CheckRCCondition(SCADA_Relay Relay, int iTargetVal);
	
	BOOL AddElement(CUIntArray &eleArray, UINT ulID);
	BOOL FindElement(CUIntArray &eleArray, UINT ulID);
	BOOL IfNeedInit();
};

#endif // !defined(AFX_ELEMENTOPR_H__70CA61CE_7315_4899_8713_1A14156D3E40__INCLUDED_)
