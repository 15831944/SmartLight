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
		//	m_Handle = dlopen(filename, mode); //ԭ����[��̬���ӿ��ǳɹ��˵��Ƿ��ְ�ʵʱ��rtdbInf����� comment by tjs 2015-07-10]
		/************************************************************************/
		/* 1��������ʽ

		RTLD_LAZY����dlopen����ǰ�����ڶ�̬���е�δ����ķ��Ų�ִ�н�����ֻ�Ժ���������Ч�����ڱ�����������������������

		RTLD_NOW�� ��Ҫ��dlopen����ǰ������������δ������ţ������������������dlopen�᷵��NULL������Ϊ��: undefined symbol: xxxx.......

		2�����÷�Χ�����������ʽͨ����|�����ʹ�á�

		RTLD_GLOBAL����̬���ж���ķ��ſɱ����򿪵������������

		RTLD_LOCAL�� ��RTLD_GLOBAL�����෴����̬���ж���ķ��Ų��ܱ����򿪵��������ض�λ�����û��ָ����RTLD_GLOBAL����RTLD_LOCAL����ȱʡΪRTLD_LOCAL                                                                     */
		/************************************************************************/

		TRACE(" dlopen filename = %s\n",filename);

		m_Handle = dlopen(filename, RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND); //fanshuo ����
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

