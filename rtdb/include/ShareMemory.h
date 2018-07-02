/////////////////////////////////////////////////////////////////////////////
// ShareMemory.h: ����һ����ƽ̨�ķ�װ��ӳ���ļ��͹����ڴ��������.
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_MAP_FILE_AND_SHARE_MEMORY_H_2004_08_18__
#define __AFX_MAP_FILE_AND_SHARE_MEMORY_H_2004_08_18__

#include "gSystemDef.h"

#define IS_MAP_FILE true
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CShareMemory
{
public:
	CShareMemory(int nkey = -1);
	~CShareMemory( );

public:
	// �����ڴ�ؼ���
	int		m_iKey;
	// �����ڴ�Ĵ�С
	int		m_iSize;
	// �Ƿ����ӳ���ļ���ʽ
	bool	m_bMapFile;
	// �����ڴ�ӳ����ʼ��ַ
	char *	m_pMapAddress;

#ifdef WINDOWS_ENV
private:
	HANDLE	m_hMap;
#endif

public:
	bool	SetKey(int nkey);
	void *	Create(int nSize,bool bMapFile = IS_MAP_FILE,bool bClear = true);

	void *	Attach();
	void	Detach();
	void	Destroy();

public:
	CShareMemory &	operator=(CShareMemory& sm);
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_MAP_FILE_AND_SHARE_MEMORY_H_2004_08_18__
