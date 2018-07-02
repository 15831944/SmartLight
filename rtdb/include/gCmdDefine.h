/////////////////////////////////////////////////////////////////////////////
// gCmdDefine.h : �������е�ϵͳ������
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_SYSTEM_COMMAND_DEFINE_H__
#define __AFX_SYSTEM_COMMAND_DEFINE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// ����ĳ���������		[����ĸ�ʽ: ���ݿ�ID ��ID]
#define CMD_ASK_RTDB_DATA			1001
// �ش�ĳ���������		[����ĸ�ʽ: ���ݿ�ID ��ID ��¼�� ��ʼ��¼��� ������ż�¼]
#define CMD_ANSWER_RTDB_DATA		1002
// �ش�ĳ������������	[����ĸ�ʽ: ���ݿ�ID ��ID ����ܼ�¼��]
#define CMD_ANSWER_RTDB_DATA_END	1003
// ����ʵʱ�⴫�����
#define CMD_ASK_RTDB_FINISH			1004
// �ش�ʵʱ�⴫�����
#define CMD_ANSWER_RTDB_FINISH		1005
// ĳ������Ҫ��������	[����ĸ�ʽ: ���ݿ�ID]
#define CMD_RELOAD_DATABASE			1006
// ĳ������Ҫ��������	[����ĸ�ʽ: ���ݿ�ID ��ID]
#define CMD_RELOAD_TABLE			1007
// �㲥ĳ���������		[����ĸ�ʽ: ���ݿ�ID ��ID ��¼�� ��ʼ��¼��� ������ż�¼]
#define CMD_BROADCAST_DATA			1008
// �㲥ĳ������������	[����ĸ�ʽ: ���ݿ�ID ��ID ����ܼ�¼��]
#define CMD_BROADCAST_DATA_END		1009
/////////////////////////////////////////////////////////////////////////////
// �رձ�TCP����		[����ĸ�ʽ: ��]
#define CMD_CLOSE_CONNECT			2001
// �����Ϸ�����Ϣ		[����ĸ�ʽ: һ��tabMessage�ṹ��]
#define CMD_SEND_MESSAGE			2005
#define CMD_SEND_JAVAMESSAGE		2006
#define CMD_MESSAGE_PROCESSTYPE		2007

/////////////////////////////////////////////////////////////////////////////
// ���������¼			[����ĸ�ʽ: UPDATE_RECORDS_HEAD�ṹ ������ż�¼����]
#define CMD_INSERT_RECORDS			3001
// ���¶�����¼			[����ĸ�ʽ: UPDATE_RECORDS_HEAD�ṹ ������ż�¼����]
#define CMD_UPDATE_RECORDS			3002
// ɾ��������¼			[����ĸ�ʽ: DELETE_RECORDS_HEAD�ṹ ������ż�¼ID32]
#define CMD_DELETE_RECORDS			3003
// ɾ��������м�¼		[����ĸ�ʽ: ��ID]
#define CMD_DELETE_ALL_RECORDS		3004
// �Ķ�����¼��һ��ֵ	[����ĸ�ʽ: UPDATE_FIELD_HEAD�ṹ ÿ����¼��ID|Index+�ֶ�ֵ]
#define CMD_UPDATE_FIELD			3005
// ��������¼��һ��ֵ	[����ĸ�ʽ: UPDATE_FIELD_AREA_HEAD�ṹ ÿ���ֶε�ֵ ID32]
#define CMD_UPDATE_FIELD_AREA		3006
// �޸�һ����¼�Ķ���ֵ	[����ĸ�ʽ: UPDATE_RECORD_FIELDS_HEAD�ṹ ÿ���ֶε�ID��ֵ]
#define CMD_UPDATE_RECORD_FIELDS	3007
// �޸ı�����һ�е�ֵ	[����ĸ�ʽ: SET_COLUMN_VALUE_HEAD�ṹ �ֶε�ֵ]
#define CMD_SET_COLUMN_VALUE		3008
// ��������				[����ĸ�ʽ: ����(0:ȫ��,1:���ݿ�,2:��) ID]
#define CMD_SAVE_DATA				3009
// �Ķ�����¼�Ķ���ֶ�	[����ĸ�ʽ: UPDATE_FIELDS_HEAD ����[��¼ID+��¼index+�ֶ�index+�ֶ�ֵ] ]
#define CMD_UPDATE_FIELDS			3010
// ѭ���������ֵ		[����ĸ�ʽ: PUSH_FIELDS_HEAD �ֶ�ֵ]
#define CMD_PUSH_FIELDS			3011
/////////////////////////////////////////////////////////////////////////////
// ��������Ƿ�����		[����ĸ�ʽ: ��]
#define CMD_DETECT_NET				4001
// ����ڵ��ʱ			[����ĸ�ʽ: һ��SYSTEMTIME�ṹ��]
#define CMD_TIME					4002
// ���ؽڵ����㲥����	[����ĸ�ʽ: �ڵ�ID �ڵ����� ���̸��� ����״̬��Ϣ����]
#define CMD_LOCAL_NODE_MANAGER		4003
// ȫ���ڵ����㲥����	[����ĸ�ʽ: ��ʶ�����ڵ���� ��ʶ�����ڵ�ID����]
#define CMD_TOTAL_NODE_MANAGER		4004
// TCP�������ӵ���������
#define CMD_TCP_HEART				4005
// �ͻ��˸�֪������TCP������Ϣ [����ĸ�ʽ: �ṹ��NODE_INFO]
#define CMD_TCP_LINK_INFO			4006
/////////////////////////////////////////////////////////////////////////////
// database transfer
#define CMD_ASK_DATABASE			8001
#define CMD_ANSWER_DATABASE			8002

// table transfer
#define CMD_TRANS_ASK_TABLE			8003
#define CMD_TRANS_ANSWER_TABLE		8004

// all database id transfer
#define CMD_ASK_ALL_DATABASE_ID		8005
#define CMD_ANSWER_ALL_DATABASE_ID	8006
/////////////////////////////////////////////////////////////////////////////
// ���縴���ļ�		[����ĸ�ʽ: �ڵ�ID32,�Ƿ񱸷�,�ļ�����,�ļ�����]
#define CMD_PUT_FILE_START			9001
#define CMD_PUT_FILE_CONTINUE		9002
#define CMD_PUT_FILE_END			9003
#define CMD_PUT_FILE_FAIL			9004

#define CMD_GET_FILE				10001
#define CMD_GET_FILE_START			10002
#define CMD_GET_FILE_CONTINUE		10003
#define CMD_GET_FILE_END			10004
#define CMD_GET_FILE_FAIL			10005

#define CMD_DELETE_FILE				11001
#define CMD_RENAME_FILE				11002

#define CMD_MKDIR					12001

/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_SYSTEM_COMMAND_DEFINE_H__
