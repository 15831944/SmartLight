#pragma  once
#include "Q_frameDefine.h"
#include "CommIEC103Def.h"

class Q_103StandardTags
{

public:
	 Q_103StandardTags();
	 Q_103StandardTags(Q_103StandardTags *other);
	 virtual  ~Q_103StandardTags();
	 virtual bool			initializeTags();
	 void					setTagsPath(char *vPath);
	 //�趨ң��ĵ�ַ��Χ����Ч��ң�Ÿ���
	 void               setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	  //�趨ң�ŵĵ�ַ��Χ����Ч��ң�Ÿ���
	 void               setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	 //�趨ң�صĵĵ�ַ��Χ����Ч��ң�Ÿ���
	 void               setControlScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	 //�趨ң���ĵ�ַ��Χ����Ч��ң�Ÿ���
	 void               setAdjustScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	 //ң�⿪ʼ��ַ
	 int                 getAnalogBegin();
	 int                 getAnalogEnd();
	 //ң�ſ�ʼ��ַ
	 int                 getPointBegin();
	 int                 getPointEnd();

	 //ң�ؿ�ʼ��ַ
	 int                 getCtrlBegin();
	 int                 getCtrlEnd();
	 //ң��
	 int                 getAdjustBegin();
	 int                 getAdjustEnd();

	 //ң�����
	 int                 getAnalogCount();
	 //ң�Ÿ���
	 int                 getPointCount();

	 int                 getCtrlCount();
	 int                 getAdjustCount();


	 //Q_RECORDDATATAG �������ͼ�ֵ
	 //adr ��ַ
	 bool fillTagInformation(LPQ_RECORDDATATAG pData,int nAdr,int devID);
	 bool fillTagInfo_CtrlAdjust(LPQ_RECORDDATATAG pData,int nAdr,int nDevID);

	 bool fillTagYTtoYC(LPQ_RECORDDATATAG pData,int nAdr,int nDevID);
protected:
	bool                    loadData();
	int m_alg_count;//ң�����
	int m_alg_begin;//ң����ʼ��ַ
	int m_alg_end;//ң�������ַ
	int m_pnt_count;//ң��Ϣ����
	int m_pnt_begin;//ң����ʼ��ַ
	int m_pnt_end;//ң�Ž�����ַ
	int m_ctrl_count;//���Ƶ����
	int m_ctrl_begin;//���Ƶ㿪ʼ
	int m_ctrl_end;//���Ƶ����
	int m_adjust_count;//ң������
	int m_adjust_begin;//ң���㿪ʼ
	int m_adjust_end;//ң�������
	char m_Path[MAX_PATH];
};