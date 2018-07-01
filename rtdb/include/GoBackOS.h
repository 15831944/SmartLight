/////////////////////////////////////////////////////////////////////////////

// GoBackOS.h : 实现热键屏蔽或启用

/////////////////////////////////////////////////////////////////////////////

#ifndef __AFX_GOBACKOS_H_2006_06_09__

#define __AFX_GOBACKOS_H_2006_06_09__

/////////////////////////////////////////////////////////////////////////////


#ifdef WINDOWS_ENV

//原始
//typedef (* IMPORT) (void);

/*end       */
//update by tjs 2015-04-16
typedef void(* IMPORT) (void);

#endif

#ifdef UNIX_ENV
#define AFX_EXT_CLASS
#endif

class AFX_EXT_CLASS CGoBackOS

{

#ifdef WINDOWS_ENV

	HMODULE    m_hGinaDll;

	IMPORT	   m_fSAS;

	IMPORT	   m_fNoSAS;

	HMODULE    m_hDll;

	IMPORT     m_fStart;

	IMPORT     m_fStop;

#endif



public:

	CGoBackOS();

	virtual ~CGoBackOS();



public:

	void SetHotKey(bool bUseHotKey);

	static bool setGinaDll();

};

/////////////////////////////////////////////////////////////////////////////

#endif // __AFX_GOBACKOS_H_2006_06_09__

