#include "CLibLoader.h"


#ifdef OS_UNIX
    #include <dlfcn.h> 
#endif //OS_UNIX

CLibLoader::CLibLoader(string& libname) 
{ 
    m_Handle = NULL;
    m_refcount = 0;
	m_LibName = libname;
	m_log=NULL;
}; 

CLibLoader::~CLibLoader() 
{ 
	if(m_Handle)
	{
		m_refcount = 1;
		Unload();
	}
}; 

#ifdef UNIX_ENV
int CLibLoader::Load(int mode) 
{ 
    if(!m_Handle)
    {
		if((mode != RTLD_NOW) && (mode != RTLD_LAZY))
			mode = RTLD_LAZY; 
   
	//	char filename[MAX_PATH] = "./";
	//	strcat(filename, m_LibName.c_str());
		char filename[MAX_PATH];
		strcpy(filename,m_LibName.c_str());
//update by fansuo 2015-06-23
		//	m_Handle = dlopen(filename, mode); //原代码[动态链接库是成功了但是发现把实时库rtdbInf清空了 comment by tjs 2015-07-10]
		/************************************************************************/
		/* 1、解析方式

		RTLD_LAZY：在dlopen返回前，对于动态库中的未定义的符号不执行解析（只对函数引用有效，对于变量引用总是立即解析）。

		RTLD_NOW： 需要在dlopen返回前，解析出所有未定义符号，如果解析不出来，在dlopen会返回NULL，错误为：: undefined symbol: xxxx.......

		2、作用范围，可与解析方式通过“|”组合使用。

		RTLD_GLOBAL：动态库中定义的符号可被其后打开的其它库解析。

		RTLD_LOCAL： 与RTLD_GLOBAL作用相反，动态库中定义的符号不能被其后打开的其它库重定位。如果没有指明是RTLD_GLOBAL还是RTLD_LOCAL，则缺省为RTLD_LOCAL                                                                     */
		/************************************************************************/

		TRACE(" dlopen filename = %s\n",filename);

		m_Handle = dlopen(filename, RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND); //fanshuo 代码
		if(m_Handle==NULL) 
		{ 
			//Q_commonUtility::outError(m_log->getFile(),"unable to load library %s: info: %s\n", m_LibName.c_str(), dlerror()); 
			TRACE("unable to load library %s: info: %s\n", m_LibName.c_str(), dlerror());
			return -1;
		}; 
    
		m_refcount = 1;
	}
    
    return GetRefCount();
}; 

int CLibLoader::Load() 
{ 
    return Load(RTLD_LAZY); 
}; 

void* CLibLoader::GetFuncPtr(const char* module) 
{ 
    void *ptr=NULL; 
    
    if(m_Handle==NULL) 
    { 
        Q_commonUtility::outError(m_log->getFile(),"LIB: unable to get function pointer: %s before library %s loaded\n", module, m_LibName.c_str());
        return NULL; 
    }; 
    
    dlerror();	//clear errors
    ptr = (void *)dlsym(m_Handle, module); 
 
    return ptr; 
}; 

int CLibLoader::Unload() 
{ 
    if(m_Handle==NULL) 
		return 0;
	
	int refcount = DecRefCount();
	
	if(refcount == 0)
    {
        dlclose(m_Handle); 
        m_Handle=NULL; 
    }

    return refcount; 
}; 

#endif //OS_UNIX 


#ifdef WINDOWS_ENV
int CLibLoader::Load(int) 
{ 
	return Load();
}; 

int CLibLoader::Load() 
{ 
     if(!m_Handle)
	{  
		m_Handle=LoadLibrary(m_LibName.c_str()); 
		if(m_Handle==NULL) 
		{ 
		    Q_commonUtility::outError(m_log->getFile(),"LIB: unable to load library: %s info: %s\n", m_LibName.c_str()); 
		    return -1;
		}; 
    
		m_refcount = 1;
	}
    
    return GetRefCount();
}; 

void* CLibLoader::GetFuncPtr(const char* module) 
{ 
    void *ptr=NULL; 
    
    if(m_Handle == NULL)
    { 
        Q_commonUtility::outError(m_log->getFile(),"LIB: library not loaded, unable to get function pointer %s\n", module);
        return NULL; 
    }; 
    
    ptr=(void *)GetProcAddress((HINSTANCE)m_Handle,module); 

    return ptr; 
}; 

int CLibLoader::Unload() 
{ 
	if(m_Handle == NULL) 
        return 0; 
    
	int refcount = DecRefCount();
    if(refcount == 0)
    {
        FreeLibrary((HINSTANCE)m_Handle);
        m_Handle = NULL; 
    }

    return refcount;
}

#endif //OS_WINDOWS
void CLibLoader::setLog(Log *v_log)
{
	m_log=v_log;
}

