#ifndef __LOG_DEFINED_H
#define __LOG_DEFINED_H
#include "ThreadMutex.h"
#include "Q_frameAssistant.h"
class Log
{
public:
   Log();
   virtual ~Log();
   FILE *getFile();
private:
	void initLogPath();
private:
    FILE* m_logfile;
	
};
#endif