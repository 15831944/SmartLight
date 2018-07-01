/////////////////////////////////////////////////////////////////////////////
// gCmdDefine.h : 定义所有的系统命令字
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_SYSTEM_COMMAND_DEFINE_H__
#define __AFX_SYSTEM_COMMAND_DEFINE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// 请求某个表的数据		[命令报文格式: 数据库ID 表ID]
#define CMD_ASK_RTDB_DATA			1001
// 回答某个表的数据		[命令报文格式: 数据库ID 表ID 记录数 起始记录序号 逐条存放记录]
#define CMD_ANSWER_RTDB_DATA		1002
// 回答某个表的数据完毕	[命令报文格式: 数据库ID 表ID 表的总记录数]
#define CMD_ANSWER_RTDB_DATA_END	1003
// 请求实时库传输完毕
#define CMD_ASK_RTDB_FINISH			1004
// 回答实时库传输完毕
#define CMD_ANSWER_RTDB_FINISH		1005
// 某个库需要重新下载	[命令报文格式: 数据库ID]
#define CMD_RELOAD_DATABASE			1006
// 某个表需要重新下载	[命令报文格式: 数据库ID 表ID]
#define CMD_RELOAD_TABLE			1007
// 广播某个表的数据		[命令报文格式: 数据库ID 表ID 记录数 起始记录序号 逐条存放记录]
#define CMD_BROADCAST_DATA			1008
// 广播某个表的数据完毕	[命令报文格式: 数据库ID 表ID 表的总记录数]
#define CMD_BROADCAST_DATA_END		1009
/////////////////////////////////////////////////////////////////////////////
// 关闭本TCP连接		[命令报文格式: 空]
#define CMD_CLOSE_CONNECT			2001
// 网络上发送消息		[命令报文格式: 一个tabMessage结构体]
#define CMD_SEND_MESSAGE			2005
#define CMD_SEND_JAVAMESSAGE		2006
#define CMD_MESSAGE_PROCESSTYPE		2007

/////////////////////////////////////////////////////////////////////////////
// 插入多条记录			[命令报文格式: UPDATE_RECORDS_HEAD结构 逐条存放记录内容]
#define CMD_INSERT_RECORDS			3001
// 更新多条记录			[命令报文格式: UPDATE_RECORDS_HEAD结构 逐条存放记录内容]
#define CMD_UPDATE_RECORDS			3002
// 删除多条记录			[命令报文格式: DELETE_RECORDS_HEAD结构 逐条存放记录ID32]
#define CMD_DELETE_RECORDS			3003
// 删除表的所有记录		[命令报文格式: 表ID]
#define CMD_DELETE_ALL_RECORDS		3004
// 改多条记录的一列值	[命令报文格式: UPDATE_FIELD_HEAD结构 每个记录的ID|Index+字段值]
#define CMD_UPDATE_FIELD			3005
// 改连续记录的一列值	[命令报文格式: UPDATE_FIELD_AREA_HEAD结构 每个字段的值 ID32]
#define CMD_UPDATE_FIELD_AREA		3006
// 修改一条记录的多列值	[命令报文格式: UPDATE_RECORD_FIELDS_HEAD结构 每个字段的ID和值]
#define CMD_UPDATE_RECORD_FIELDS	3007
// 修改表整整一列的值	[命令报文格式: SET_COLUMN_VALUE_HEAD结构 字段的值]
#define CMD_SET_COLUMN_VALUE		3008
// 保存数据				[命令报文格式: 类型(0:全部,1:数据库,2:表) ID]
#define CMD_SAVE_DATA				3009
// 改多条记录的多个字段	[命令报文格式: UPDATE_FIELDS_HEAD 逐项[记录ID+记录index+字段index+字段值] ]
#define CMD_UPDATE_FIELDS			3010
// 循环数组添加值		[命令报文格式: PUSH_FIELDS_HEAD 字段值]
#define CMD_PUSH_FIELDS			3011
/////////////////////////////////////////////////////////////////////////////
// 检测网络是否正常		[命令报文格式: 空]
#define CMD_DETECT_NET				4001
// 网络节点对时			[命令报文格式: 一个SYSTEMTIME结构体]
#define CMD_TIME					4002
// 本地节点管理广播命令	[命令报文格式: 节点ID 节点活动网段 进程个数 进程状态信息数组]
#define CMD_LOCAL_NODE_MANAGER		4003
// 全网节点管理广播命令	[命令报文格式: 新识别死节点个数 新识别死节点ID数组]
#define CMD_TOTAL_NODE_MANAGER		4004
// TCP保持连接的心跳报文
#define CMD_TCP_HEART				4005
// 客户端告知服务器TCP连接信息 [命令报文格式: 结构体NODE_INFO]
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
// 网络复制文件		[命令报文格式: 节点ID32,是否备份,文件名称,文件数据]
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
