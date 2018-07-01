//
//	CP56Time2a IEC 60870-5-104 time data type implementation
//
#if !defined(__Q_IEC104_CP56TIME2A_H__INCLUDED__)
#define __Q_IEC104_CP56TIME2A_H__INCLUDED__

#include "Q_frameInclude.h"


#include <time.h>

class Q_CP56Time2a
{
private:
	SYSTEMTIME		stime;
	bool			valid;
	bool			genuine;
	bool			summer;

public:
	Q_CP56Time2a(unsigned char *data);
	~Q_CP56Time2a(void);

	//短日期串 2012-05-12
	void GetDateString(char *buf, size_t size);
	//长日期串 2012-05-12 00:00:00.2323
	void GetTimeString(char *buf, size_t size);
	SYSTEMTIME _GetSystemTime(void);
	//获得当前值微秒相加nUsecs的值
	SYSTEMTIME _GetSystemTime(int nUsecs);

#ifdef WINDOWS_ENV
	FILETIME _GetFileTime(void);
	void TimeToCP56Time2a(FILETIME *ft, unsigned char *data);
	void CP56Time2aToTime(unsigned char *data, FILETIME *ft);
#endif

	void ActualTimeToCP56Time(unsigned char *data);

	void TimeToCP56Time2a(SYSTEMTIME *st, unsigned char *data);

	void CP56Time2aToTime(unsigned char *data, SYSTEMTIME *st);
	
};
#endif