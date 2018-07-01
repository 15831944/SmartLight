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
	//调节策略
	AgcAdMethod getAdMethod();
	//判断命令执行不到位时间默认秒
	int   getCmdTimeOutSecs();
    	//得到1分钟变化率限制标识
	bool getOneMinLimitFlg();
private:
	agcFuncRtdb *m_pfRtdb;

};
#endif