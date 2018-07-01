#ifndef __CLIBLOADER_H
#define __CLIBLOADER_H

#include "gSystemDef.h"
#include "Log.h"
#include <string>
using namespace std;

class CLibLoader 
{ 
protected: 
    void *m_Handle; 
    string m_LibName;
    int  m_refcount;
    
public: 
    CLibLoader(string& libname); 
    ~CLibLoader(); 
    
    int Load(int mode); 
    int Load();
    int Unload();

    void *GetFuncPtr(const char *module); 
	inline int GetRefCount(){ return m_refcount; };
  	void setLog(Log *v_log);
protected:
    inline int IncRefCount(){ return ++m_refcount; };
    inline int DecRefCount(){ return --m_refcount; };
private:
	Log *m_log;
}; 

#endif //__CPRTCLLIBLOADER_H
