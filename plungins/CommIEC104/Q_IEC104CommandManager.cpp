#include "stdafx.h"
#include "Q_IEC104CommandManager.h"

Q_IEC104CommandManager::Q_IEC104CommandManager(void)
{

}
Q_IEC104CommandManager::~Q_IEC104CommandManager(void)
{
}


bool  Q_IEC104CommandManager::removeCommand(Q_protocolCommandBase* pCommand)
{
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i] == pCommand && pCommand != NULL)
		{
			//EnterCriticalSection(&m_csHandle);
			m_csHandle.Lock();
			delete m_commandList[i];
			m_commandList[i] = NULL;
			//LeaveCriticalSection(&m_csHandle);
			m_csHandle.UnLock();
			return true;
		}
	}
	return false;
}

bool Q_IEC104CommandManager::clearCommand()
{
	//EnterCriticalSection(&m_csHandle);
	m_csHandle.Lock();
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		   if(m_commandList[i]!=NULL)
		   {
			delete m_commandList[i];
			m_commandList[i] = NULL;
		   }
	}
	//LeaveCriticalSection(&m_csHandle);
	m_csHandle.UnLock();
	return true;
}
bool Q_IEC104CommandManager::addCommand(Q_protocolCommandBase* pCommand)
{
	if(pCommand == NULL)
	return false;

	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i] == NULL)
		{
			//EnterCriticalSection(&m_csHandle);
			m_csHandle.Lock();
			m_commandList[i] = pCommand;
			m_nCurrentCommandCount++;
			//LeaveCriticalSection(&m_csHandle);
			m_csHandle.UnLock();
			return true;
		}
	}
	return false;
}
int Q_IEC104CommandManager::getCommandByStatus(list<Q_IEC104StandardCommand *> & v_cmdList)
{
	int len=0;
	//EnterCriticalSection(&m_csHandle);
	m_csHandle.Lock();
	Q_IEC104StandardCommand *curCmd=NULL;
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i]!=NULL)
		{
			curCmd=(Q_IEC104StandardCommand *)m_commandList[i];
			if(false==curCmd->m_bError)
			{
				v_cmdList.push_back((Q_IEC104StandardCommand *)m_commandList[i]);
				len++;
			}

		}
		
	}
	//LeaveCriticalSection(&m_csHandle);
	m_csHandle.UnLock();
	return len;

}
int Q_IEC104CommandManager::getCommandByStatus(list<Q_IEC104StandardCommand *> & v_cmdList,const Q_IEC104CmdType &v_eType)
{
	int len=0;
	/*for(IT_CmdListP it=m_list.begin();it!=m_list.end();it++)
	{
	    if(false==(*it)->m_bError &&
			v_eType==(*it)->m_eType)
		{
			v_cmdList.push_back(*it);
			len++;
		}
	}*/
	return len;
}
int Q_IEC104CommandManager::getCommandByStatus(list<Q_IEC104StandardCommand *> & v_cmdList,const Q_IEC104CmdType &v_eType,const Q_IEC104CmdState &v_eState)
{
	int len=0;
	/*for(IT_CmdListP it=m_list.begin();it!=m_list.end();it++)
	{
		if(false==(*it)->m_bError &&
			v_eType==(*it)->m_eType&& 
			v_eType==(*it)->m_eState)
		{
			v_cmdList.push_back(*it);
			len++;
		}
	}*/
	return len;

}
Q_cmdIEC104 *Q_IEC104CommandManager::getCommandByStatus(const Q_IEC104CmdType &v_eType,const Q_IEC104CmdState &v_eState,const int &v_nInfoAddr)
{
	Q_cmdIEC104 *cmd=NULL;
	//EnterCriticalSection(&m_csHandle);
	m_csHandle.Lock();
	Q_cmdIEC104 *curCmd=NULL;
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i]!=NULL)
		{
			curCmd=(Q_cmdIEC104 *)m_commandList[i];
			if(v_eType==curCmd->getCommand104Type() &&
				curCmd->m_eState==v_eState
				&&curCmd->m_nInfoAddr==v_nInfoAddr)
			{
				cmd=curCmd;
				break;
			}
		}

	}
	//LeaveCriticalSection(&m_csHandle);
	m_csHandle.UnLock();
	return cmd;

}
bool Q_IEC104CommandManager::removeCommand(const Q_IEC104CmdType &v_eType,const Q_IEC104CmdState &v_eState,const int &v_nInfoAddr)
{
	bool ret=false;

	Q_cmdIEC104 *cmd=NULL;

	//EnterCriticalSection(&m_csHandle);
	m_csHandle.Lock();
	Q_IEC104StandardCommand *curCmd=NULL;
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i]!=NULL)
		{
			cmd=(Q_cmdIEC104 *)m_commandList[i];
			if(v_eType==cmd->getCommand104Type() &&
				cmd->m_eState==v_eState
				&&cmd->m_nInfoAddr==v_nInfoAddr)
			{
				delete m_commandList[i];
				m_commandList[i] = NULL;
				ret=true;
			}
		}

	}
	//LeaveCriticalSection(&m_csHandle);
	m_csHandle.UnLock();
	return ret;

}
void Q_IEC104CommandManager::delCommandFinished()
{
	int len=0;
	//EnterCriticalSection(&m_csHandle);
	m_csHandle.Lock();
	Q_IEC104StandardCommand *curCmd=NULL;
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i]!=NULL)
		{
			Q_cmdIEC104 *cmd=(Q_cmdIEC104 *)m_commandList[i];
			if(true==cmd->m_bError ||
				cmd->m_eState==eCmdStateEnd)
			{
				delete m_commandList[i];
				m_commandList[i] = NULL;
			}
		}

	}
	//LeaveCriticalSection(&m_csHandle);
	m_csHandle.UnLock();
}