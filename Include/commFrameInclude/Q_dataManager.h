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
 * @brief 这里面相当于有两个表，一个遥测、一个遥信，每个表中都有单独的索引.bool类型的全存在遥信里面,其它值存在遥测里面
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
	int initRtdb();//初使化实时库接口 2015-06-23
	CFEPFuncRtdb *getRtdbInf();
	void UnRegisterDb();
public:
	/**************************************************************************************************
	@Function: initializeDataManager
	@Description: 初使化数据管理
	@Input: nAnalogueCount 遥测个数,nDigitalCount 遥信个数,nYKCount遥控个数, nYTCount 遥调个数
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
	//通过通道、设备地址、信息体地址找
	const LPQ_ANALOGUEDATATAG findAlgTag2(int nChannelID,int nDevid,int nNo);
	const LPQ_DIGITALEDATATAG findDigTag2(int nChannelID,int nDevid,int nNo);
	//获得遥测索引
	int getAnalogIndex(int nChannel,int nDevid,int nNo);
	//获得遥信索引
	int getDigitalIndex(int nChannel,int nDevid,int nNo);
	//初使转发信息
	bool							initTransData(int nAnalogueCount,int nDigitalCount);
	//初使化通道信息表
	bool							initChnInfo(int nChnInfoCount);
	//初使化结点表
	bool                            initNodeInfo(int nNodeInfoCount);
	/**************************************************************************************************
	@Function: setDataTagInfo
	@Description: 根据索引设置值的信息
	@Input: dataType 数据类型,重要，区分用
	        nIndex  数据索引值,遥测有遥测的索引，遥信有遥信的索引
	        chName  值的信息
	@Output:
	@Return: 
	@Update:
	@Others:
	**************************************************************************************************/
	bool				setDataTagInfo(Q_DataType dataType,int nIndex,char* chName);
	/**************************************************************************************************
	@Function: setDataTagInfo
	@Description: 更新数据内容
	@Input: dataArray 记录值指针，包括索引值
	        nCount  记录的位置
			idx_valid 记录中有index是否有效如果有效不需要根据TUid+device+no查找了。
	chName  值的信息
	@Output:
	@Return: 
	@Update:
	@Others:
	**************************************************************************************************/
	bool							recordDataValue(LPQ_RECORDDATATAG dataArray,int nCount,bool idx_valid=false);
	bool							evalDataValue(LPQ_RECORDDATATAG2 dataArray,int nCount,int nchanneltype=2,int nid32forZTchannel=0,int nYCYX=1);
	/**************************************************************************************************
	@Function: evalData
	@Description: 根据记录索引值，及数据类型，取得内容值
	@Input: pData 其中索引值nIndex及数据类型eDataType值不能为空
	@Output:pData中对应的bValue,fValue;,nValue会被赋予相应的值
	@Return: 
	@Update:
	@Others:
	**************************************************************************************************/
	//bool                            evalData(LPQ_RECORDDATATAG pData);
	             //取遥测数据内容(全部行记录)
	bool                            getValueDataYcTotal(LPQ_ANALOGUEDATATAG pData);
	             //取遥信数据内容(全部行记录)
	bool                            getValueDataYxTotal(LPQ_DIGITALEDATATAG pData);
	//只取部分经常变动的数据包括fValue eUnitType等域
	bool                            getValueDataYc(LPQ_ANALOGUEDATATAG pData);
	//只取得bValue的值
	bool                            getValueDataYx(LPQ_DIGITALEDATATAG pData);

	bool                            getNameByIndex(LPQ_RECORDDATATAG2 dataArray,int nCount);
	
	/**************************************************************************************************
	@Function: evalData
	@Description: 根据站编号及站内编号查询遥测信息
	@Input: nDevid站编号,nNo站内编号
	@Output:
	@Return: 
	@Update:
	@Others:
	**************************************************************************************************/
	const LPQ_ANALOGUEDATATAG findAlgTag(int nDevid,int nNo,int nAddrbegin,int nAddrend);
	/**************************************************************************************************
	@Function: evalData
	@Description: 根据站编号及站内编号查询遥信信息
	@Input: nDevid站编号,nNo站内编号
	@Output:
	@Return: 
	@Update:
	@Others:
	**************************************************************************************************/


	const LPQ_DIGITALEDATATAG findDigTag(int nDevid,int nNo,int nAddrbegin,int nAddrend);
	

	const LPQ_YKEDATATAG findYKTag(int nDevid,int nNo,int nAddrbegin,int nAddrend);

	const LPQ_YTEDATATAG findYTTag(int nDevid,int nNo,int nAddrbegin,int nAddrend);
	//得到遥测个数
	int getAnalogCount(){return m_nAnalogueCount;}
	//得到遥信个数
	int getDigitalCount(){return m_nDigitalCount;}
	int getTransAlgCount(){return m_nTransAlgCount;}
	int getTransDigCount(){return m_nTransDigCount;}
	int getChnInfoCount(){return m_nChnInfoCount;}
	//得到遥控个数
	int getYKCount(){return m_nYKCount;}
	//得到遥调个数
	int getYTCount(){return m_nYTCount;}


	//得到遥测数据地址
	const LPQ_ANALOGUEDATATAG getAnalogData(LPQ_ANALOGUEDATATAG *alg,int count);
	const LPQ_ANALOGUEDATATAG getAnalogData(){return m_dataAnalogue;}
	//得到遥信数据地址
	const LPQ_DIGITALEDATATAG	 getDigitalData(){return m_dataDigital;}
	const LPQ_DIGITALEDATATAG	 getDigitalData(LPQ_DIGITALEDATATAG *dig,int count);
	//得到遥控数据地址
	const LPQ_YKEDATATAG	getYKData(void){return m_dataYK;}
	//得到遥调数据地址
	const LPQ_YTEDATATAG	getYTData(void){return m_dataYT;}

	const LPQ_ANALOGUEDATATAG getAnalogTagInfo(int nIndex);
	const LPQ_DIGITALEDATATAG	getDigitalTagInfo(int nIndex);


	const LPQ_TRANS_ALGDATATAG getTransAlgData(void){return m_dataTransAlg;}
	const LPQ_TRANS_DIGDATATAG getTransDigData(void){return m_dataTransDig;}
	const LPQ_CHN_INFO     getChnInfo(void){return m_chnInfo;}

	//得到结点个数
	int              getNodeCount(void){ return m_NodeCount;}
	//得到结点信息
	const LPQ_NODE_INFO     getNodeInfo(void) {return m_NodeInfo;}
	//进入互斥区
    void EnterMutex(void);
	//出互斥区
	void LeaveMutex(void);
	bool isValidAlgIndex(int);
	bool isValidDigIndex(int);
	//判断double类型是否相同
	bool equalDouble(double num1,double num2);
	//处理主站遥控 2015-06-29
	int handleMasterYk(LPQ_RECORDDATATAG dataArray);
	//处理子站遥控
	int handleMasterYt(LPQ_RECORDDATATAG dataArray);
	//检查遥控是否存在 0存在其它不存在
    int  checkYkExists(LPQ_RECORDDATATAG dataArray,SCADA_Relay* & pRelay);
	//检查遥控是否存在 0存在其它不存在
	int checkYtExists(LPQ_RECORDDATATAG dataArray,SCADA_SetPoint* & pRelay);
	//true成功,false失败
	//dataArray 遥调中的参数值，返回为得到的计算结果
	bool getYtValue(LPQ_RECORDDATATAG dataArray);
	float getYtcoefficient(int nChannelID,int nDevid,int nNo);

	//新rtdb接口，更新通道状态
	void UpDataChanelYX(int nchannel, int nClientOpen);
	void CleanChannelStatus();
	void CleanQualityStatus();
	void UpdatXNDataStatus();
	void UpdatSlaveQualityStatus();

private:
	int								m_nAnalogueCount;//遥测个数
	LPQ_ANALOGUEDATATAG				m_dataAnalogue;//遥测值
	int								m_nDigitalCount;//遥信个数
	LPQ_DIGITALEDATATAG				m_dataDigital;//遥信值

	int								m_nYKCount;//遥控个数
	LPQ_YKEDATATAG					m_dataYK;//遥控值
	int								m_nYTCount;//遥调个数
	LPQ_YTEDATATAG					m_dataYT;//遥调值

	int                             m_nTransAlgCount;//转发遥测个数
	LPQ_TRANS_ALGDATATAG            m_dataTransAlg;//转发遥测
	int                             m_nTransDigCount;//转发遥信个数
	LPQ_TRANS_DIGDATATAG            m_dataTransDig;//转发遥信
	int                             m_nChnInfoCount;//通信信息表个数
	LPQ_CHN_INFO                    m_chnInfo;//通道信息表
	int                             m_NodeCount;//结点个数
	LPQ_NODE_INFO                   m_NodeInfo;//节点信息表

	CThreadMutex  m_csHandle;
	CFEPFuncRtdb *m_rtdbInf;//2015-06-23 tjs
};




#endif // !defined(__Q_DATAMANAGER_H__INCLUDED__)


