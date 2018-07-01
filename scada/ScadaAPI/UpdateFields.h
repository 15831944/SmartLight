// UpdateFields.h: interface for the CUpdateFields class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __AFX_Rtdb_SCADA_UPDATEFIELDS_H_2003_04_02__
#define __AFX_Rtdb_SCADA_UPDATEFIELDS_H_2003_04_02__

#include "gSystemDef.h"

class CUpdateFields  
{
public:
	CUpdateFields();
	~CUpdateFields();

public:
	int GetFieldCount() { return nFieldCount; };
	void AddFieldIndex(INT index);
	INT *pFieldIndex;

private:
	INT nFieldCount;
};

#endif // !defined(__AFX_Rtdb_SCADA_UPDATEFIELDS_H_2003_04_02__)
