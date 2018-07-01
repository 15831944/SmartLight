//*
//*
//* Q_serialLink.h
//*
//*
#if !defined(__Q_SERIALLINK_H__INCLUDED__)
#define __Q_SERIALLINK_H__INCLUDED__

#ifdef WINDOWS_ENV
#pragma once
#pragma warning(push)
#pragma warning(disable:4217)
#pragma warning(disable:4075)
#endif
#include "gSystemDef.h"
#include "Q_linkBase.h"




/** 
 * @interface 
 * @brief Q_serialLinkSetting class declare
 * @author yangxi
 * @date 2011-12-30
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_serialLinkSetting : public Q_linkSettingBase
{
public:
	Q_serialLinkSetting(void);
	virtual ~Q_serialLinkSetting(void);



public:
	inline char*		getComPortName(void) { return m_chComName;}				
	inline int			getBoundRate(void) { return m_nBoundRate;}
	inline int			getDataBit(void) { return m_nDataBit;}
	inline int			getStopBit(void) { return m_nStopBit;}
	inline int			getVerifyBit(void) { return m_nVerifyBit;}
	//inline int			getFlowMode() { return m_nFlow;}

	inline void			setComPortName(char* chName) { sprintf_s(m_chComName,16,chName);}
	inline void			setBountRate(int nBoundRate) { m_nBoundRate = nBoundRate;}
	inline void			setDataBit(int nDataBit) { m_nDataBit = nDataBit;}
	inline void			setStopBit(int nStopBit) { m_nStopBit = nStopBit;}
	//inline void			setFlowMode(int nFlowMode) { m_nFlow = nFlowMode;}

	inline void			setVerifyBit(int nVerifyBit) { m_nVerifyBit = nVerifyBit;}


protected:
	char				m_chComName[Q_FRAME_CONST16];

	int					m_nBoundRate;

	int					m_nDataBit;

	int					m_nStopBit;

	//int					m_nFlow;
	int                 m_nVerifyBit;
};


/** 
 * @interface 
 * @brief Q_serialLink class declare
 * @author yangxi
 * @date 2011-12-30
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_serialLink : public Q_linkBase
{
public:
	Q_serialLink(void);
	virtual ~Q_serialLink(void);



public:
	virtual bool			initializeLink(void);
	virtual bool			terminateLink(void);
	virtual bool			connect(void);
	virtual bool			disconnect(void);
	virtual bool			readData(void);
	virtual bool			writeData(void);


protected:
#ifdef WINDOWS_ENV
	HANDLE					m_hComPort;
#endif
#ifdef UNIX_ENV
	int m_iComPort;
#endif	

};
#ifdef WINDOWS_ENV

#pragma warning(pop)
#endif

#endif // !defined(__Q_SERIALLINK_H__INCLUDED__)
