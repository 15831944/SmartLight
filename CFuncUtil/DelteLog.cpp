#include "StdAfx.h"
#include <assert.h> 
#include"DeleteLog.h"

DeleteLog::DeleteLog():m_logTime(25*24*60*60)
{
	//ԭ����45�죬���ڸĳ�25�� tjs 2017-1-15
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
		const static int secsInOneDay = 60*60*24;//һ���ж�����
		time_t lastTime = now - m_logTime - secsInOneDay;
		assert(lastTime>0);
        struct tm tmLastTime = *localtime(&lastTime);
		std::string strFile(*iterFile);
		int iYear = tmLastTime.tm_year+1900;
		int iMon = tmLastTime.tm_mon+1;
		int iDate = tmLastTime.tm_mday;//ɾ����־�����ʱ���ǰһ�������
		char temp[5];
		sprintf(temp,"%04d",iYear);
		strFile.replace(YearOffSet,4,temp);
		sprintf(temp,"%02d",iMon);
		strFile.replace(MonthOffSet,2,temp);
		sprintf(temp,"%02d",iDate);
		strFile.replace(DateOffSet,2,temp);
		//printf("removeLogFile:%s\r\n",strFile.c_str());
		remove(strFile.c_str());//ɾ����־�����ʱ���ǰһ�������
	}
}

//ʹ��####�滻�ļ�������ߵ��꣬@@�滻�ļ�������ߵ��£�$$�滻�ļ�������ߵ�����
//�����ļ�����E:/epro/ldns/Bin/data/message/agc/2106-07-20-agclogfile.log  ӦдΪE:/epro/ldns/Bin/data/message/agc/####-@@-$$-agclogfile.log
void DeleteLog::addFile(const std::string& file)
{
	m_veStrFileName.push_back(file);
}
bool DeleteLog::setLogLastDay(int iDay)
{
	m_logTime = iDay*24*60*60; //����־����ʱ������ת��Ϊ��
	return true;
}

