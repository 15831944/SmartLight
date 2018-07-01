#ifndef __AFX_DELETE_LOG__H
#define __AFX_DELETE_LOG__H
#include "gSystemDef.h"
#include"TimerTask.h"
#include<string>
#include<vector>
#include "CFuncUtilDef.h"
class __Q_CFUNCUTIL_EXPORT DeleteLog: public TimerTask
{
public:
	DeleteLog();//默认45天
	~DeleteLog();
	virtual void task();
	//使用####替换文件名字里边的年，@@替换文件名字里边的月，$$替换文件名字里边的日期，例如文件名字E:/epro/ldns/Bin/data/message/agc/2106-07-20-agclogfile.log  应写为E:/epro/ldns/Bin/data/message/agc/####-@@-$$-agclogfile.log
	void addFile(const std::string& fileName);
	bool setLogLastDay(int iDay);
private:
	unsigned int m_logTime; //日志文件保存的最长时间，单位秒
	std::vector<std::string> m_veStrFileName; 

};

#endif //__AFX_DELETE_LOG__H