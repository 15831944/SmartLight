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
	 //设定遥测的地址范围及有效的遥信个数
	 void               setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	  //设定遥信的地址范围及有效的遥信个数
	 void               setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	 //设定遥控的的地址范围及有效的遥信个数
	 void               setControlScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	 //设定遥调的地址范围及有效的遥信个数
	 void               setAdjustScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
	 //遥测开始地址
	 int                 getAnalogBegin();
	 int                 getAnalogEnd();
	 //遥信开始地址
	 int                 getPointBegin();
	 int                 getPointEnd();

	 //遥控开始地址
	 int                 getCtrlBegin();
	 int                 getCtrlEnd();
	 //遥调
	 int                 getAdjustBegin();
	 int                 getAdjustEnd();

	 //遥测个数
	 int                 getAnalogCount();
	 //遥信个数
	 int                 getPointCount();

	 int                 getCtrlCount();
	 int                 getAdjustCount();


	 //Q_RECORDDATATAG 数据类型及值
	 //adr 地址
	 bool fillTagInformation(LPQ_RECORDDATATAG pData,int nAdr,int devID);
	 bool fillTagInfo_CtrlAdjust(LPQ_RECORDDATATAG pData,int nAdr,int nDevID);

	 bool fillTagYTtoYC(LPQ_RECORDDATATAG pData,int nAdr,int nDevID);
protected:
	bool                    loadData();
	int m_alg_count;//遥测个数
	int m_alg_begin;//遥测起始地址
	int m_alg_end;//遥测结束地址
	int m_pnt_count;//遥信息个数
	int m_pnt_begin;//遥信起始地址
	int m_pnt_end;//遥信结束地址
	int m_ctrl_count;//控制点个数
	int m_ctrl_begin;//控制点开始
	int m_ctrl_end;//控制点结束
	int m_adjust_count;//遥调个数
	int m_adjust_begin;//遥调点开始
	int m_adjust_end;//遥调点结束
	char m_Path[MAX_PATH];
};