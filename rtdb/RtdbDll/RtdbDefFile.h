/****************************************************************************
 * RtdbDefFile.h: 处理实时库结构定义信息与RtdbDef.txt文件的交互操作(读存)	*
 ****************************************************************************/
#ifndef __AFX_RTDB_DEF_FILE_DEAL_H_WANGZK_2003_09_02__
#define __AFX_RTDB_DEF_FILE_DEAL_H_WANGZK_2003_09_02__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;
#include "XmlHead.h"
class CMultiDataBase;
/////////////////////////////////////////////////////////////////////////////
class CRtdbDefFile  
{
public:
	CRtdbDefFile();
	virtual ~CRtdbDefFile();

public:
	static bool	ReadRtdbDef(char * pRtdbDefFile,CMultiDataBase * pMDB);
	static bool	SaveRtdbDef(char * pRtdbDefFile,CMultiDataBase * pMDB);

protected:
	static CDataBase *	ReadDataBaseLine(char * pLine );
	static bool	ReadTableLine(char * pLine, CDBTable * pTable);
	static bool	ReadFieldLine(char * pLine, CDBField * pField);
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_RTDB_DEF_FILE_DEAL_H_WANGZK_2003_09_02__
