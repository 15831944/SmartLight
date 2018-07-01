// ID32Base.h: interface for the CID32Base class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __ID32BASE_2006_01_05__
#define __ID32BASE_2006_01_05__

#include "DataValue.h"	
#define  BASEID32  (1<<26)
class CID32Base  
{
public:
	CID32Base();
	virtual ~CID32Base();
	static UINT GetID32Base();

protected:
	static UINT InitFromFile();
	static bool m_bInit;
	static UINT m_uID32Base;

};

#endif // __ID32BASE_2006_01_05__
