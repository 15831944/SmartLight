#if !defined(__Q_IEC104COMMAND_MANAGER_H__INCLUDED__)
#define __Q_IEC104COMMAND_MANAGER_H__INCLUDED__
/** 
* @interface 
* @brief IEC-60870-104 规约命令列表管理
* @author 谭井胜 TanJingSheng
* @date 2012-2-08
* @see 
* @bug No know bugs so far.
*/
#include "Q_frameInclude.h"

#include "CommIEC104Def.h"
#include "Q_104StdDefine.h"
#include "Q_IEC104Command.h"

class __Q_COMMIEC104_EXPORT Q_IEC104CommandManager:public Q_protocolCommandManagerBase
{
public:
	Q_IEC104CommandManager(void);
	virtual ~Q_IEC104CommandManager(void);
	/**************************************************************************************************
	@Function: addCommand
	@Description: 在命令列表中添加命令
	@Input: 
	@Output:v_cmd 命令
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual bool addCommand(Q_protocolCommandBase* pCommand);
	bool         getCommandEx(Q_protocolCommandBase* pCommand);
	bool		 removeCommand(Q_protocolCommandBase* pCommand);
	bool         removeCommand(const Q_IEC104CmdType &v_eType,const Q_IEC104CmdState &v_eState,const int &v_nInfoAddr);
	bool         clearCommand();
	/**************************************************************************************************
	@Function: getCommandByStatus
	@Description: 根据状态查询命令 (没错误状态的)
	@Input: 
	@Output:v_cmdList 返回命令列表
	@Return:返回元素个数
	@Update:
	@Others:查询命令错误状态为false的值
	**************************************************************************************************/
	int getCommandByStatus(list<Q_IEC104StandardCommand *> & v_cmdList);
	/**************************************************************************************************
	@Function: getCommandByStatus
	@Description: 根据状态查询命令 
	@Input: v_cmdList 返回命令列表,v_eType 命令类型
	@Output:v_cmdList 返回命令列表
	@Return: 返回结到的元素个数
	@Update:
	@Others:查询命令错误状态为false的值
	**************************************************************************************************/
	int getCommandByStatus(list<Q_IEC104StandardCommand *> & v_cmdList,const Q_IEC104CmdType &v_eType);
	/**************************************************************************************************
	@Function: getCommandByStatus
	@Description: 根据状态查询命令 
	@Input: v_cmdList 返回命令列表,v_eType 命令类型,v_eState 命令当前状态
	@Output:v_cmdList 返回命令列表
	@Return:返回结到的元素个数
	@Update:
	@Others:查询命令错误状态为false的值
	**************************************************************************************************/
	int getCommandByStatus(list<Q_IEC104StandardCommand *> & v_cmdList,const Q_IEC104CmdType &v_eType,const Q_IEC104CmdState &v_eState);
	/**************************************************************************************************
	@Function: getCommandByStatus
	@Description: 根据状态查询命令 
	@Input: v_cmdList 返回命令列表,v_eType 命令类型,v_eState 命令当前状态,v_nInfoAddr 信息元素地址
	@Output:v_cmdList 返回命令列表
	@Return:如果成功返回命令指针，如果未查询到返回NULL
	@Update:
	@Others:查询命令错误状态为false的值
	**************************************************************************************************/
	Q_cmdIEC104 *getCommandByStatus(const Q_IEC104CmdType &v_eType,const Q_IEC104CmdState &v_eState,const int &v_nInfoAddr);
	/**************************************************************************************************
	@Function: delCommandFinished
	@Description:删除执行完成的或出错的命令 
	@Input: 
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void delCommandFinished();
	
	 
};
#endif