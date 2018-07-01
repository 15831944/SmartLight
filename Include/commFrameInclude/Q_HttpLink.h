//*
//*
//* Q_HttpLink.h
//*
//*
#if !defined(__Q_HTTPLINK_H__INCLUDED__)
#define __Q_HTTPLINK_H__INCLUDED__

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
 * @brief Q_HttpLinkSetting class declare
 * @author yangxi
 * @date 2011-12-30
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_HttpLinkSetting : public Q_linkSettingBase
{
public:
	Q_HttpLinkSetting(void);
	virtual ~Q_HttpLinkSetting(void);


};


class __Q_COMMFRAME_EXPORT Q_HttpLink : public Q_linkBase
{
public:
	Q_HttpLink(void);
	virtual ~Q_HttpLink(void);



public:
	virtual bool			initializeLink(void);
	virtual bool			terminateLink(void);
	virtual bool			connect(void);
	virtual bool			disconnect(void);
	virtual bool			readData(void);
	virtual bool			writeData(void);


};
#ifdef WINDOWS_ENV
#pragma warning(pop)
#endif

#endif // !defined(__Q_SERIALLINK_H__INCLUDED__)
