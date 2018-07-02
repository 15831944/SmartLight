//*
//*
//* Q_frameAssistant.h
//*
//*
#if !defined(__Q_DATAMANAGER_H__INCLUDED__)
#define __Q_DATAMANAGER_H__INCLUDED__

#include "Q_CommFrameDef.h"
#include "Q_frameDefine.h"
#include "ThreadMutex.h"
#include "ProcessEvent.h"
#include "MutexEx.h"
#include "FEPFuncRtdb.h"


/** 
 * @interface 
 * @brief �������൱����������һ��ң�⡢һ��ң�ţ�ÿ�����ж��е���������.bool���͵�ȫ����ң������,����ֵ����ң������
 * @author yangxi
 * @date 2012-1-15
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_dataManager
{
public:
	Q_dataManager(void);
	virtual ~Q_dataManager(void);
	int initRtdb();//��ʹ��ʵʱ��ӿ� 2015-06-23
	CFEPFuncRtdb *getRtdbInf();
	void UnRegisterDb();
public:
	/**************************************************************************************************
	@Function: initializeDataManager
	@Description: ��ʹ�����ݹ���
	@Input: nAnalogueCount ң�����,nDigitalCount ң�Ÿ���,nYKCountң�ظ���, nYTCount ң������
	@Output:
	@Return: 
	@Update:
	@Others:
	**************************************************************************************************/
	bool							initializeDataManager(int nAnalogueCount,int nDigitalCount,int nYKCount,int nYTCount);
	void							CleanMem(void);
	bool							AllocateYCMem(int nYCCount);
	bool							AllocateYXMem(int nYXCount);
	bool							AllocateYKMem(int nYKCount);
	bool							AllocateYTMem(int nYTCount);
	//ͨ��ͨ�����豸��ַ����Ϣ���ַ��
	const LPQ_ANALOGUEDATATAG findAlgTag2(int nChannelID,int nDevid,int nNo);
	const LPQ_DIGITALEDATATAG findDigTag2(int nChannelID,int nDevid,int nNo);
	//���ң������
	int getAnalogIndex(int nChannel,int nDevid,int nNo);
	//���ң������
	int getDigitalIndex(int nChannel,int nDevid,int nNo);
	//��ʹת����Ϣ
	bool							initTransData(int nAnalogueCount,int nDigitalCount);
	//��ʹ��ͨ����Ϣ��
	bool							initChnInfo(int nChnInfoCount);
	//��ʹ������
	bool                            initNodeInfo(int nNodeInfoCount);
	/**************************************************************************************************
	@Function: setDataTagInfo
	@Description: ������������ֵ����Ϣ
	@Input: dataType ��������,��Ҫ��������
	        nIndex  ��������ֵ,ң����ң���������ң����ң�ŵ�����
	        chName  ֵ����Ϣ
	@Output:
	@Return: 
	@Update:
	@Others:
	**************************************************************************************************/
	bool				setDataTagInfo(Q_DataType dataType,int nIndex,char* chName);
	/**************************************************************************************************
	@Function: setDataTagInfo
	@Description: ������������
	@Input: dataArray ��¼ֵָ�룬��������ֵ
	        nCount  ��¼��λ��
			idx_valid ��¼����index�Ƿ���Ч�����Ч����Ҫ����TUid+device+no�����ˡ�
	chName  ֵ����Ϣ
	@Output:
	@Return: 
	@Update:
	@Others:
	**************************************************************************************************/
	bool							recordDataValue(LPQ_RECORDDATATAG dataArray,int nCount,bool idx_valid=false);
	bool							evalDataValue(LPQ_RECORDDATATAG2 dataArray,int nCount,int nchanneltype=2,int nid32forZTchannel=0,int nYCYX=1);
	/**************************************************************************************************
	@Function: evalData
	@Description: ���ݼ�¼����ֵ�����������ͣ�ȡ������ֵ
	@Input: pData ��������ֵnIndex����������eDataTypeֵ����Ϊ��
	@Output:pData�ж�Ӧ��bValue,fValue;,nValue�ᱻ������Ӧ��ֵ
	@Return: 
	@Update:
	@Others:
	**************************************************************************************************/
	//bool                            evalData(LPQ_RECORDDATATAG pData);
	             //ȡң����������(ȫ���м�¼)
	bool                            getValueDataYcTotal(LPQ_ANALOGUEDATATAG pData);
	             //ȡң����������(ȫ���м�¼)
	bool                            getValueDataYxTotal(LPQ_DIGITALEDATATAG pData);
	//ֻȡ���־����䶯�����ݰ���fValue eUnitType����
	bool                            getValueDataYc(LPQ_ANALOGUEDATATAG pData);
	//ֻȡ��bValue��ֵ
	bool                            getValueDataYx(LPQ_DIGITALEDATATAG pData);

	bool                            getNameByIndex(LPQ_RECORDDATATAG2 dataArray,int nCount);
	
	/**************************************************************************************************
	@Function: evalData
	@Description: ����վ��ż�վ�ڱ�Ų�ѯң����Ϣ
	@Input: nDevidվ���,nNoվ�ڱ��
	@Output:
	@Return: 
	@Update:
	@Others:
	**************************************************************************************************/
	const LPQ_ANALOGUEDATATAG findAlgTag(int nDevid,int nNo,int nAddrbegin,int nAddrend);
	/**************************************************************************************************
	@Function: evalData
	@Description: ����վ��ż�վ�ڱ�Ų�ѯң����Ϣ
	@Input: nDevidվ���,nNoվ�ڱ��
	@Output:
	@Return: 
	@Update:
	@Others:
	**************************************************************************************************/


	const LPQ_DIGITALEDATATAG findDigTag(int nDevid,int nNo,int nAddrbegin,int nAddrend);
	

	const LPQ_YKEDATATAG findYKTag(int nDevid,int nNo,int nAddrbegin,int nAddrend);

	const LPQ_YTEDATATAG findYTTag(int nDevid,int nNo,int nAddrbegin,int nAddrend);
	//�õ�ң�����
	int getAnalogCount(){return m_nAnalogueCount;}
	//�õ�ң�Ÿ���
	int getDigitalCount(){return m_nDigitalCount;}
	int getTransAlgCount(){return m_nTransAlgCount;}
	int getTransDigCount(){return m_nTransDigCount;}
	int getChnInfoCount(){return m_nChnInfoCount;}
	//�õ�ң�ظ���
	int getYKCount(){return m_nYKCount;}
	//�õ�ң������
	int getYTCount(){return m_nYTCount;}


	//�õ�ң�����ݵ�ַ
	const LPQ_ANALOGUEDATATAG getAnalogData(LPQ_ANALOGUEDATATAG *alg,int count);
	const LPQ_ANALOGUEDATATAG getAnalogData(){return m_dataAnalogue;}
	//�õ�ң�����ݵ�ַ
	const LPQ_DIGITALEDATATAG	 getDigitalData(){return m_dataDigital;}
	const LPQ_DIGITALEDATATAG	 getDigitalData(LPQ_DIGITALEDATATAG *dig,int count);
	//�õ�ң�����ݵ�ַ
	const LPQ_YKEDATATAG	getYKData(void){return m_dataYK;}
	//�õ�ң�����ݵ�ַ
	const LPQ_YTEDATATAG	getYTData(void){return m_dataYT;}

	const LPQ_ANALOGUEDATATAG getAnalogTagInfo(int nIndex);
	const LPQ_DIGITALEDATATAG	getDigitalTagInfo(int nIndex);


	const LPQ_TRANS_ALGDATATAG getTransAlgData(void){return m_dataTransAlg;}
	const LPQ_TRANS_DIGDATATAG getTransDigData(void){return m_dataTransDig;}
	const LPQ_CHN_INFO     getChnInfo(void){return m_chnInfo;}

	//�õ�������
	int              getNodeCount(void){ return m_NodeCount;}
	//�õ������Ϣ
	const LPQ_NODE_INFO     getNodeInfo(void) {return m_NodeInfo;}
	//���뻥����
    void EnterMutex(void);
	//��������
	void LeaveMutex(void);
	bool isValidAlgIndex(int);
	bool isValidDigIndex(int);
	//�ж�double�����Ƿ���ͬ
	bool equalDouble(double num1,double num2);
	//������վң�� 2015-06-29
	int handleMasterYk(LPQ_RECORDDATATAG dataArray);
	//������վң��
	int handleMasterYt(LPQ_RECORDDATATAG dataArray);
	//���ң���Ƿ���� 0��������������
    int  checkYkExists(LPQ_RECORDDATATAG dataArray,SCADA_Relay* & pRelay);
	//���ң���Ƿ���� 0��������������
	int checkYtExists(LPQ_RECORDDATATAG dataArray,SCADA_SetPoint* & pRelay);
	//true�ɹ�,falseʧ��
	//dataArray ң���еĲ���ֵ������Ϊ�õ��ļ�����
	bool getYtValue(LPQ_RECORDDATATAG dataArray);
	float getYtcoefficient(int nChannelID,int nDevid,int nNo);

	//��rtdb�ӿڣ�����ͨ��״̬
	void UpDataChanelYX(int nchannel, int nClientOpen);
	void CleanChannelStatus();
	void CleanQualityStatus();
	void UpdatXNDataStatus();
	void UpdatSlaveQualityStatus();

private:
	int								m_nAnalogueCount;//ң�����
	LPQ_ANALOGUEDATATAG				m_dataAnalogue;//ң��ֵ
	int								m_nDigitalCount;//ң�Ÿ���
	LPQ_DIGITALEDATATAG				m_dataDigital;//ң��ֵ

	int								m_nYKCount;//ң�ظ���
	LPQ_YKEDATATAG					m_dataYK;//ң��ֵ
	int								m_nYTCount;//ң������
	LPQ_YTEDATATAG					m_dataYT;//ң��ֵ

	int                             m_nTransAlgCount;//ת��ң�����
	LPQ_TRANS_ALGDATATAG            m_dataTransAlg;//ת��ң��
	int                             m_nTransDigCount;//ת��ң�Ÿ���
	LPQ_TRANS_DIGDATATAG            m_dataTransDig;//ת��ң��
	int                             m_nChnInfoCount;//ͨ����Ϣ�����
	LPQ_CHN_INFO                    m_chnInfo;//ͨ����Ϣ��
	int                             m_NodeCount;//������
	LPQ_NODE_INFO                   m_NodeInfo;//�ڵ���Ϣ��

	CThreadMutex  m_csHandle;
	CFEPFuncRtdb *m_rtdbInf;//2015-06-23 tjs
};




#endif // !defined(__Q_DATAMANAGER_H__INCLUDED__)


