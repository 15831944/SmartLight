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
	DeleteLog();//Ĭ��45��
	~DeleteLog();
	virtual void task();
	//ʹ��####�滻�ļ�������ߵ��꣬@@�滻�ļ�������ߵ��£�$$�滻�ļ�������ߵ����ڣ������ļ�����E:/epro/ldns/Bin/data/message/agc/2106-07-20-agclogfile.log  ӦдΪE:/epro/ldns/Bin/data/message/agc/####-@@-$$-agclogfile.log
	void addFile(const std::string& fileName);
	bool setLogLastDay(int iDay);
private:
	unsigned int m_logTime; //��־�ļ�������ʱ�䣬��λ��
	std::vector<std::string> m_veStrFileName; 

};

#endif //__AFX_DELETE_LOG__H