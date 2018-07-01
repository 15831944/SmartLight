/////////////////////////////////////////////////////////////////////////////
// ShareMemory.h: 定义一个跨平台的封装了映射文件和共享内存操作的类.
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
	// 共享内存关键字
	int		m_iKey;
	// 共享内存的大小
	int		m_iSize;
	// 是否采用映射文件方式
	bool	m_bMapFile;
	// 共享内存映射起始地址
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
