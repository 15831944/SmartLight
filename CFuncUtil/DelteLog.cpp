#include "StdAfx.h"
#include <assert.h> 
#include"DeleteLog.h"

DeleteLog::DeleteLog():m_logTime(25*24*60*60)
{
	//原来是45天，现在改成25天 tjs 2017-1-15
}
DeleteLog::~DeleteLog()
{

}
//
void DeleteLog::task()
{
	for(std::vector<std::string>::iterator iterFile(m_veStrFileName.begin()); iterFile!=m_veStrFileName.end(); iterFile++)
	{
		int YearOffSet = iterFile->find('#');
		int MonthOffSet = iterFile->find('@');
		int DateOffSet = iterFile->find('$');
		time_t now = time(NULL);
		if(m_logTime < 0)
		{
			break ;
		}
		const static int secsInOneDay = 60*60*24;//一天有多少秒
		time_t lastTime = now - m_logTime - secsInOneDay;
		assert(lastTime>0);
        struct tm tmLastTime = *localtime(&lastTime);
		std::string strFile(*iterFile);
		int iYear = tmLastTime.tm_year+1900;
		int iMon = tmLastTime.tm_mon+1;
		int iDate = tmLastTime.tm_mday;//删除日志保存最长时间的前一天的数据
		char temp[5];
		sprintf(temp,"%04d",iYear);
		strFile.replace(YearOffSet,4,temp);
		sprintf(temp,"%02d",iMon);
		strFile.replace(MonthOffSet,2,temp);
		sprintf(temp,"%02d",iDate);
		strFile.replace(DateOffSet,2,temp);
		//printf("removeLogFile:%s\r\n",strFile.c_str());
		remove(strFile.c_str());//删除日志保存最长时间的前一天的数据
	}
}

//使用####替换文件名字里边的年，@@替换文件名字里边的月，$$替换文件名字里边的日期
//例如文件名字E:/epro/ldns/Bin/data/message/agc/2106-07-20-agclogfile.log  应写为E:/epro/ldns/Bin/data/message/agc/####-@@-$$-agclogfile.log
void DeleteLog::addFile(const std::string& file)
{
	m_veStrFileName.push_back(file);
}
bool DeleteLog::setLogLastDay(int iDay)
{
	m_logTime = iDay*24*60*60; //把日志存在时间又天转换为秒
	return true;
}

