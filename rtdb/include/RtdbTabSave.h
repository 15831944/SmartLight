#ifndef __AFX_RTDB_TABSAVE__H
#define __AFX_RTDB_TABSAVE__H

#include "Rtdb_Struct.h"
#include "MultiDataBase.h"
#include <vector>
#include <map>
#include <string>

class AFX_EXT_CLASS CRtdbTabSave
{
public:
	CRtdbTabSave();
	virtual ~CRtdbTabSave();

public:
	// 工作参数初始化
	bool	InitDb(CMultiDataBase * pRtdb, const char *db);
	bool	AddSaveCfg(const char *tab, const char *field);
	bool	ClearSaveCfg();
	// 启动参数保存
	bool	SetPath(const char * path);
	bool	SetDelay(int delay);
	bool	Start();
	// 清除工作(一般可以不调)
	void	Exit( );

private:
	// 消息接收线程
	static  ThreadFuncRet ReceiveThread(LPVOID pData);
	bool	m_bReceiveRun;

private:
	bool	m_bExit;
	int		m_nProcessType;
	std::map<INT,std::vector<INT> >m_SaveTabs;
	std::string m_Path;
	CMultiDataBase*	m_pRtdb;
	CDataBase*	m_pDb;
	int m_nDelay;
};
#endif // __AFX_RTDB_TABSAVE__H
