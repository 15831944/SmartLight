
#ifndef _COMPUTERWATCH_2006_06_16_
#define _COMPUTERWATCH_2006_06_16_

// 判断操作系统涵数及变量
typedef enum tagWin32SysType{
	Windows32s,
	WindowsNT3,
	Windows95,
	Windows98,
	WindowsME,
	WindowsNT4,
	Windows2000,
	WindowsXP
}Win32SysType;

class  AFX_EXT_CLASS CComputerWatch
{
public:
    CComputerWatch();
    ~CComputerWatch();

    void GetCpuUage( int &usage );
    void  GetMemUsage( int &usage, int& capacity );
    void  GetDiskUsage( int &usage );

protected:
    //win
    bool CpuUsage2000(int& iResult);
	bool CpuUsage9x(int& iResult);
	bool IsShellSystemType( Win32SysType& ShellType );

#ifdef TRU64_OS
//#ifdef UNIX_ENV
	int getloadavg( double* avg, int iNum );
	int getMemUsage( );
#endif
};

#endif //_COMPUTERWATCH_2006_06_16_
