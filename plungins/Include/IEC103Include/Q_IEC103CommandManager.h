#pragma once
#if !defined(__Q_IEC103COMMAND_MANAGER_H__INCLUDED__)
#define __Q_IEC103COMMAND_MANAGER_H__INCLUDED__
/** 
* @interface 
* @brief IEC-60870-103 ��Լ�����б����
* @author ̷��ʤ TanJingSheng
* @date 2012-2-08
* @see 
* @bug No know bugs so far.
*/
#include "CommIEC103Def.h"
#include "Q_103StdDefine.h"
#include "Q_IEC103Command.h"

class   Q_IEC103CommandManager:public Q_protocolCommandManagerBase
{
public:
	Q_IEC103CommandManager(void);
	virtual ~Q_IEC103CommandManager(void);
	/**************************************************************************************************
	@Function: addCommand
	@Description: �������б����������
	@Input: 
	@Output:v_cmd ����
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	virtual bool addCommand(Q_protocolCommandBase* pCommand);
	bool         getCommandEx(Q_protocolCommandBase* pCommand);
	bool		 removeCommand(Q_protocolCommandBase* pCommand);
	bool         removeCommand(const Q_IEC103CmdType &v_eType,const Q_IEC103CmdState &v_eState,const int &v_nInfoAddr);
	bool         clearCommand();
	/**************************************************************************************************
	@Function: getCommandByStatus
	@Description: ����״̬��ѯ���� (û����״̬��)
	@Input: 
	@Output:v_cmdList ���������б�
	@Return:����Ԫ�ظ���
	@Update:
	@Others:��ѯ�������״̬Ϊfalse��ֵ
	**************************************************************************************************/
	int getCommandByStatus(list<Q_IEC103StandardCommand *> & v_cmdList);
	/**************************************************************************************************
	@Function: getCommandByStatus
	@Description: ����״̬��ѯ���� 
	@Input: v_cmdList ���������б�,v_eType ��������
	@Output:v_cmdList ���������б�
	@Return: ���ؽᵽ��Ԫ�ظ���
	@Update:
	@Others:��ѯ�������״̬Ϊfalse��ֵ
	**************************************************************************************************/
	int getCommandByStatus(list<Q_IEC103StandardCommand *> & v_cmdList,const Q_IEC103CmdType &v_eType);
	/**************************************************************************************************
	@Function: getCommandByStatus
	@Description: ����״̬��ѯ���� 
	@Input: v_cmdList ���������б�,v_eType ��������,v_eState ���ǰ״̬
	@Output:v_cmdList ���������б�
	@Return:���ؽᵽ��Ԫ�ظ���
	@Update:
	@Others:��ѯ�������״̬Ϊfalse��ֵ
	**************************************************************************************************/
	int getCommandByStatus(list<Q_IEC103StandardCommand *> & v_cmdList,const Q_IEC103CmdType &v_eType,const Q_IEC103CmdState &v_eState);
	/**************************************************************************************************
	@Function: getCommandByStatus
	@Description: ����״̬��ѯ���� 
	@Input: v_cmdList ���������б�,v_eType ��������,v_eState ���ǰ״̬,v_nInfoAddr ��ϢԪ�ص�ַ
	@Output:v_cmdList ���������б�
	@Return:����ɹ���������ָ�룬���δ��ѯ������NULL
	@Update:
	@Others:��ѯ�������״̬Ϊfalse��ֵ
	**************************************************************************************************/
	//Q_cmdIEC103 *Q_IEC103CommandManager::getCommandByStatus(const Q_IEC103CmdType &v_eType,const Q_IEC103CmdState &v_eState,const int &v_nInfoAddr);
	Q_cmdIEC103 *getCommandByStatus(const Q_IEC103CmdType &v_eType,const Q_IEC103CmdState &v_eState,const int &v_nInfoAddr);
	/**************************************************************************************************
	@Function: delCommandFinished
	@Description:ɾ��ִ����ɵĻ��������� 
	@Input: 
	@Output:
	@Return:
	@Update:
	@Others:
	**************************************************************************************************/
	void delCommandFinished();
	
	 
};
#endif