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
	// ����������ʼ��
	bool	InitDb(CMultiDataBase * pRtdb, const char *db);
	bool	AddSaveCfg(const char *tab, const char *field);
	bool	ClearSaveCfg();
	// ������������
	bool	SetPath(const char * path);
	bool	SetDelay(int delay);
	bool	Start();
	// �������(һ����Բ���)
	void	Exit( );

private:
	// ��Ϣ�����߳�
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
