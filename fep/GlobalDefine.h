#ifndef  _GLOBAL_DEFING_H_
#define  _GLOBAL_DEFING_H_
#ifndef UNIX_ENV

#pragma warning(disable:4996) 
#endif
#ifdef WINDOWS_ENV
#define COMM_LOG_DETAIL								"\\log\\"
#define COMM_PLAN_CURVE_DIR							"\\data\\" //AGC远程曲线
#define COMM_PLAN_LOCAL_CURVE_DIR					"\\data\\local_agc\\" //kc add AGC本地曲线路径

#define COMM_AVC_PLAN_CURVE_DIR						"\\data\\remote_avc\\" //AVC远程曲线
#define COMM_AVC_PLAN_LOCAL_CURVE_DIR				"\\data\\local_avc\\" //AVC本地曲线路径

#define COMM_CONFIG_PATH							     "\\config\\"
#define COMM_LOG_AVC_DIR							     "\\log\\avc"
#define COMM_LOG_AGC_DIR							     "\\log\\agc"
#define COMM_LOG_ERROR_DIR							     "\\log\\error"
#define COMM_DATA_HIS_DIR							     "\\data\\his"
#define COMM_DATA_DB_DIR							     "\\data\\db"
#else
//2016-09-01 tjs
#define COMM_LOG_DETAIL								"/log/"
#define COMM_PLAN_CURVE_DIR							"/data/" //AGC远程曲线
#define COMM_PLAN_LOCAL_CURVE_DIR					"/data/local_agc/" //kc add AGC本地曲线路径

#define COMM_AVC_PLAN_CURVE_DIR						"/data/remote_avc/" //AVC远程曲线
#define COMM_AVC_PLAN_LOCAL_CURVE_DIR				"/data/local_avc/ //AVC本地曲线路径

#define COMM_CONFIG_PATH							     "/config/"
#define COMM_LOG_AVC_DIR							     "/log/avc"
#define COMM_LOG_AGC_DIR							     "/log/agc"
#define COMM_LOG_ERROR_DIR							     "/log/error"
#define COMM_DATA_HIS_DIR							     "/data/his"
#define COMM_DATA_DB_DIR							     "/data/db"
#endif

#ifndef MANGOS_WIN32_DLL_IMPORT
#define MANGOS_WIN32_DLL_IMPORT
#endif

#define C_CH_TYPE_SCADA 1 //采集通道
#define C_CH_TYPE_UP    2 //上传通道
#define C_CH_TYPE_ZT    3 //组态通道
#define C_CH_TYPE_ZB    4 //主备
#define C_CH_TYPE_OTHER  5 //虚拟

#include "Q_runingUnitDefine.h"

typedef struct TaskCfgList
{
	Q_chDownReturn* data;//数据
	ST_GeCtrlChl *cfg;//配置
}TaskCfgList;

//服务器端配置
typedef struct TaskTcpSrvCfgItem
{
	int channelID; //如果channelid
	Q_tcpServerTask* task;//任务编号
	bool enabled;//是否可用
	char protocol[50];;
	TaskTcpSrvCfgItem()
	{
		task=NULL;
		enabled=false;
		channelID=0;
		memset(protocol,0,sizeof(protocol));
	}
}TaskTcpSrvCfgItem;

#endif


