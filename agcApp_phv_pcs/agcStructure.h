#ifndef _AGC_STRUCTURE_H_
#define _AGC_STRUCTURE_H_
#include "agcFuncRtdb.h"
class  Q_AgcParameter
{
public:
	Q_AgcParameter();
	~Q_AgcParameter();
	void setRtdbPointer(agcFuncRtdb *); 
public:
	int getCalcInterval();
	int getSendCmdInterval();
	bool getReSendFlg();
	bool getLogFlg();
	bool getLockCheckFlg();
	bool getSafeCheckFlg();
	//���ڲ���
	AgcAdMethod getAdMethod();
	//�ж�����ִ�в���λʱ��Ĭ����
	int   getCmdTimeOutSecs();
    	//�õ�1���ӱ仯�����Ʊ�ʶ
	bool getOneMinLimitFlg();
private:
	agcFuncRtdb *m_pfRtdb;

};
#endif