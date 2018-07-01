//
//	CP56Time2a IEC 60870-5-104 time data type implementation
//
#include "stdafx.h"
#include "Q_104CP56Time2a.h"

//
//	Constructor: decode CP56Time2a data
//
Q_CP56Time2a::Q_CP56Time2a(unsigned char *data)
{
	CP56Time2aToTime(data, &stime);
}

Q_CP56Time2a::~Q_CP56Time2a(void)
{
}

//
//	return time string of CP56Time2a decoded
//
void Q_CP56Time2a::GetTimeString(char *buf, size_t size)
{
	sprintf_s(buf, size,"%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d.%3.3d", stime.wYear,
		stime.wMonth, stime.wDay,stime.wHour, stime.wMinute,
		stime.wSecond, stime.wMilliseconds);
	return;
}
//
//	return time string of CP56Time2a decoded
//
void Q_CP56Time2a::GetDateString(char *buf, size_t size)
{
	sprintf_s(buf, size,"%4.4d-%2.2d-%2.2d",  stime.wYear,
		stime.wMonth,stime.wDay);
	return;
}

//
//	return SYSTEMTIME structure of CP56Time2a decoded
//
SYSTEMTIME Q_CP56Time2a::_GetSystemTime(void)
{
	return stime;
}
//获得当前值微秒相加nUsecs的值
SYSTEMTIME Q_CP56Time2a::_GetSystemTime(int nUsecs)
{//这个函数效率不高，最好改一下。
	SYSTEMTIME v_time;
	memcpy(&v_time,&stime,sizeof(SYSTEMTIME));

	v_time.wMilliseconds+=nUsecs;
    if(v_time.wMilliseconds>=1000)
	{
      v_time.wSecond+=1;
	  if(v_time.wSecond>=60)
	  {
		  v_time.wMinute+=1;
		  if(v_time.wMinute>=60)
		  {
            v_time.wHour+=1;
			if(v_time.wHour>=24)
			{
				v_time.wDay+=1;
			}
		  }
	  }

	}
    return v_time;
}

//
//	return FILETIME structure of CP56Time2a decoded
//
#ifdef WINDOWS_ENV
FILETIME Q_CP56Time2a::_GetFileTime(void)
{
	FILETIME ft;
	SystemTimeToFileTime(&stime, &ft);

	return ft;
}
#endif

//
//	return CP56Time2a data of actual time.
//
void Q_CP56Time2a::ActualTimeToCP56Time(unsigned char *data)
{
	SYSTEMTIME st;
	//GetSystemTime(&st);//晚了8个小时呢
	GetLocalTime(st);
	TimeToCP56Time2a(&st, data);
}
//
//	convert FILETIME to CP56Time2a
//
#ifdef WINDOWS_ENV
void Q_CP56Time2a::TimeToCP56Time2a(FILETIME *ft, unsigned char *data)
{
	SYSTEMTIME st;
	FileTimeToSystemTime(ft, &st);
	TimeToCP56Time2a(&st, data);
}
#endif

//
//	convert SYSTEMTIME to CP56Time2a
//
void Q_CP56Time2a::TimeToCP56Time2a(SYSTEMTIME *st, unsigned char *data)
{
	unsigned int m;
	m = st->wMilliseconds + 1000 * st->wSecond;
	data[0] = m & 0xFF;
	data[1] = (m & 0xFF00)>>8;
	data[2] = st->wMinute & 0x00FF;	// add valid flag and genuine flag
	data[3] = st->wHour & 0x00FF;	// add summer flag
	data[4] = ((st->wDayOfWeek%7)&0x03)<<5 | (st->wDay&0x1F);
	data[5] = st->wMonth & 0x0F;
	data[6] = st->wYear - 2000;
}

//
//	convert CP56Time2a to FILETIME
//
#ifdef WINDOWS_ENV
void Q_CP56Time2a::CP56Time2aToTime(unsigned char *data, FILETIME *ft)
{
	SYSTEMTIME st;
	CP56Time2aToTime(data, &st);
	SystemTimeToFileTime(&st, ft);
}
#endif

//
//	convert CP56Time2a to SYSTEMTIME
//
void Q_CP56Time2a::CP56Time2aToTime(unsigned char *data, SYSTEMTIME *st)
{
	unsigned int mili = data[0] | data[1]<<8;
	st->wSecond		= mili / 1000;
	st->wMilliseconds = mili - st->wSecond*1000;

	if(data[2] & 0x40)
		genuine = true;
	else
		genuine = false;

	if(data[2] & 0x80)
		valid = true;
	else
		valid = false;

	st->wMinute		= data[2] & 0x3F;;
	st->wHour		= data[3] & 0x1F;

	if(data[3] & 0x80)
		summer = true;
	else
		summer = false;
	
	st->wDay		= data[4] & 0x1F;
	st->wDayOfWeek = (data[4] & 0xE0 ) >> 5;
	if(st->wDayOfWeek)	// if zero day of week not used.
		st->wDayOfWeek = (st->wDayOfWeek + 1)%7;
	st->wMonth		= data[5] & 0x0F;
	st->wYear		= 2000 + (data[6] & 0x7F);
}

