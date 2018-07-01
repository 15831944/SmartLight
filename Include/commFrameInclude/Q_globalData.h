#if !defined(__Q_GLOBALDATA_H__INCLUDED__)
#define __Q_GLOBALDATA_H__INCLUDED__

#include "Q_frameDefine.h"
#include "Q_dataManager.h"

/** 
 * @interface 
 * @brief Q_globalData class declare
 * @author yangxi
 * @date 2012-1-15
 * @see 
 * @bug No know bugs so far.
 */
class __Q_COMMFRAME_EXPORT Q_globalData
{
public:
	Q_globalData(void);
	virtual ~Q_globalData(void);



public:
	inline Q_dataManager*	getDataManager() { return &m_dataManager;}


	bool					initializeGlobalData();


	bool					terminateGlobalData();


	inline bool				getKillThreadFlag() { return m_killAllThread;}
	inline void				setKillThreadFlag(bool bValue) { m_killAllThread = bValue;}
	inline void				setRunningPath(char* chPath) { sprintf_s(m_chRunningPath,Q_FRAME_CONST256,"%s",chPath);}
	inline char*			getRunningPath() { return m_chRunningPath;}

protected:
	Q_dataManager			m_dataManager;

	bool					m_killAllThread;
	bool                    m_isInitDb;
	char					m_chRunningPath[Q_FRAME_CONST256];

};
__Q_COMMFRAME_EXPORT extern Q_globalData _GlobalData;
//AFX_EXT_CLASS extern Q_globalData _GlobalData;
//__Q_COMMFRAME_EXPORT extern Q_globalData _GlobalData;



#endif // !defined(__Q_GLOBALDATA_H__INCLUDED__)
