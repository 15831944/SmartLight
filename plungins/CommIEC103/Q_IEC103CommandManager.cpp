#include "gSystemDef.h"
#include "Q_IEC103CommandManager.h"
Q_IEC103CommandManager::Q_IEC103CommandManager(void)
{

}
Q_IEC103CommandManager::~Q_IEC103CommandManager(void)
{
}

bool  Q_IEC103CommandManager::removeCommand(Q_protocolCommandBase* pCommand)
{
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i] == pCommand && pCommand != NULL)
		{
			//EnterCriticalSection(&m_csHandle);
			delete m_commandList[i];
			m_commandList[i] = NULL;
			//LeaveCriticalSection(&m_csHandle);
			return true;
		}
	}
	return false;
}
bool Q_IEC103CommandManager::clearCommand()
{
	//EnterCriticalSection(&m_csHandle);
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		   if(m_commandList[i]!=NULL)
		   {
			delete m_commandList[i];
			m_commandList[i] = NULL;
		   }
	}
//	LeaveCriticalSection(&m_csHandle);
	return true;
}
bool Q_IEC103CommandManager::addCommand(Q_protocolCommandBase* pCommand)
{
	if(pCommand == NULL)
	return false;

	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i] == NULL)
		{
		//	EnterCriticalSection(&m_csHandle);
			m_commandList[i] = pCommand;
		//	LeaveCriticalSection(&m_csHandle);
			return true;
		}
	}
	return false;
}
int Q_IEC103CommandManager::getCommandByStatus(list<Q_IEC103StandardCommand *> & v_cmdList)
{
	int len=0;
//	EnterCriticalSection(&m_csHandle);
	Q_IEC103StandardCommand *curCmd=NULL;
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i]!=NULL)
		{
			curCmd=(Q_IEC103StandardCommand *)m_commandList[i];
			if(false==curCmd->m_bError)
			{
				v_cmdList.push_back((Q_IEC103StandardCommand *)m_commandList[i]);
				len++;
			}

		}
		
	}
	//LeaveCriticalSection(&m_csHandle);
	return len;

}
int Q_IEC103CommandManager::getCommandByStatus(list<Q_IEC103StandardCommand *> & v_cmdList,const Q_IEC103CmdType &v_eType)
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
int Q_IEC103CommandManager::getCommandByStatus(list<Q_IEC103StandardCommand *> & v_cmdList,const Q_IEC103CmdType &v_eType,const Q_IEC103CmdState &v_eState)
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
Q_cmdIEC103 *Q_IEC103CommandManager::getCommandByStatus(const Q_IEC103CmdType &v_eType,const Q_IEC103CmdState &v_eState,const int &v_nInfoAddr)
{
	Q_cmdIEC103 *cmd=NULL;
	//EnterCriticalSection(&m_csHandle);
	Q_cmdIEC103 *curCmd=NULL;
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i]!=NULL)
		{
			curCmd=(Q_cmdIEC103 *)m_commandList[i];
			if(v_eType==curCmd->getCommand103Type() &&
				curCmd->m_eState==v_eState
				&&curCmd->m_nInfoAddr==v_nInfoAddr)
			{
				cmd=curCmd;
				break;
			}
		}

	}
	//LeaveCriticalSection(&m_csHandle);
	return cmd;

}
bool Q_IEC103CommandManager::removeCommand(const Q_IEC103CmdType &v_eType,const Q_IEC103CmdState &v_eState,const int &v_nInfoAddr)
{
	bool ret=false;

	Q_cmdIEC103 *cmd=NULL;

	//EnterCriticalSection(&m_csHandle);
	Q_IEC103StandardCommand *curCmd=NULL;
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i]!=NULL)
		{
			cmd=(Q_cmdIEC103 *)m_commandList[i];
			if(v_eType==cmd->getCommand103Type() &&
				cmd->m_eState==v_eState
				&&cmd->m_nInfoAddr==v_nInfoAddr)
			{
				delete m_commandList[i];
				m_commandList[i] = NULL;
				ret=true;
			}
		}

	}
//	LeaveCriticalSection(&m_csHandle);
	return ret;

}
void Q_IEC103CommandManager::delCommandFinished()
{
	int len=0;
	//EnterCriticalSection(&m_csHandle);
	Q_IEC103StandardCommand *curCmd=NULL;
	for(int i = 0; i < Q_COMMANDLIST_MAX; i++)
	{
		if(m_commandList[i]!=NULL)
		{
			Q_cmdIEC103 *cmd=(Q_cmdIEC103 *)m_commandList[i];
			if(true==cmd->m_bError ||
				cmd->m_eState==e103CmdStateEnd)
			{
				delete m_commandList[i];
				m_commandList[i] = NULL;
			}
		}

	}
//	LeaveCriticalSection(&m_csHandle);
}