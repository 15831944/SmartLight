//*
//*
//* Q_frameAssistant.h
//*
//*
#if !defined(__Q_FRAMEASSISTANT_H__INCLUDED__)
#define __Q_FRAMEASSISTANT_H__INCLUDED__
//#pragma once


#include "Q_frameDefine.h"
#include <stdarg.h>

/** 
 * @interface 
 * @brief Q_commonBuffer class declare
 * @author yangxi
 * @date 2011-12-28
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_commonBuffer
{
public:
	Q_commonBuffer(void);
	virtual ~Q_commonBuffer(void);



public:
	void			initializeBuffer(int nMaxSize);
	void			destroyBuffer(void);
	void			clearBuffer(void);
	void			removeData(int nDataLength);
	int				readData(char* buffer,int bufferSize,bool bDelete = true);
	int				readData(char* buffer,int bufferSize,int nDataLength,bool bDelete = true);
	bool			writeData(char* buffer,int nDataLength);

	
	int				getMaxSize(void);
	int				getDataLength(void);




protected:
	int				m_nMaxDataLength;
	int				m_nHeadPosition;
	int				m_nTailPosition;
	char*			m_buffer;
	bool			m_bFull;

};




/** 
 * @interface 
 * @brief Q_protocolConfig class declare
 * @author yangxi
 * @date 2011-12-28
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_commonUtility
{
public:
	Q_commonUtility(void);
	virtual ~Q_commonUtility(void);



public:
	static void				writeLog(int nType,char* chInfo);
	static void				writeLog(char* chLogFile,char* chInfo);
	static  void outString(FILE *,const char * str, ... )      ATTR_PRINTF(2,3);
	static  void outError( FILE *,const char * err, ... )       ATTR_PRINTF(2,3);

	
	static unsigned char	getCDTCRC(char* chBuffer,int nCount);
	static bool				getBitValue(char chValue,unsigned int nBitIndex);
	static char				setBitValue(char chValue,int nBitIndex,bool bZero);
	static int				getBCDValue(char chValue,bool bHigh);
	static void NU_ZERO( void *buffer,size_t count );
	/*return 1 : little-endian, return 0:big-endian*/
	static int checkCPUendian();
	static bool isBigEndian();
private:
	static void outTimestamp(FILE* file);//包括年月日 时分秒
	static  void outShortTimestamp(FILE* file);//短时间 时分秒
};





#endif // !defined(__Q_FRAMEASSISTANT_H__INCLUDED__)
