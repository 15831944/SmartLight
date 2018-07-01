#ifndef _FEP_FUNCTION_RTDB_H_
#define _FEP_FUNCTION_RTDB_H_

//#include "FEPFuncApi.h"
#include "Q_frameDefine.h"
#include "funstruct.h"
#include "ScadaAPI.h"
#include "MultiDataBase.h"
#include "MsgDealer.h"
//#include "FEPFuncGlobal.h"
#include "FEPFuncUtil.h"
#include "RightDefine.h"
#include "SCADA.h"
class CMultiDataBase;
class CScadaDataBase;

#define SMAP_DIGIT			1
#define SMAP_ANALOG			2
#define SMAP_PULSE			3
#define SMAP_RELAY			4

#define mes_FepSwitch		804
#define mes_ChannelSwitch   805

const char * const FEP1NODE_CONFIG = "FEP1NODENAME";
const char * const FEP2NODE_CONFIG = "FEP2NODENAME";
const char * const FEP1NODEIP1_CONFIG = "FEP1NODEIP1";
const char * const FEP1NODEIP2_CONFIG = "FEP1NODEIP2";
const char * const FEP2NODEIP1_CONFIG = "FEP2NODEIP1";
const char * const FEP2NODEIP2_CONFIG = "FEP2NODEIP2";

const char * const FEPKEY = "FEP";
const char * const IDKEY = "ID";
const char * const NODEKEY = "NODENAME";
const char * const IP1KEY = "NODEIP1";
const char * const IP2KEY = "NODEIP2";
const char * const GROUPKEY = "GROUP";
const char * const FEPNUMKEY = "FEPNUM";

typedef struct _tagFepSwitchMsg
{
	BOOL	m_bValid;
	int		m_iMsgType;
	int		m_iPara1;
	int		m_iPara2;
} FEPSWITCHMSG;

typedef struct _tagFepNode
{
	int		m_iID;
	int		m_iGroupID;
	char	m_strName[100];
	char	m_strIP1[100];
	char	m_strIP2[100];
	bool	m_bExist;
} FEPNODE;


class  __Q_COMMFRAME_EXPORT CFEPFuncRtdb
{
public:
	CFEPFuncRtdb();
	~CFEPFuncRtdb();
	//bool rtdb_init(CFEPFuncGlobal* pGlobal);
	bool rtdb_init();
	void ProcessMsg(tabMessage *message);
	bool GetCommuState(UINT cid);

public:
	void Final();
	void Init();

	bool FepNodeInit();
	bool dbmg_init();
	int rtdb_login();
	int rtdb_logout();
	void rtdb_set_reload_flag(int flag);
	int _refress_reg();
	UINT rtdb_get_first_station(int *index);
	UINT rtdb_get_next_station(int *index);
	int rtdb_get_station_args(StationArgs *args);
	int rtdb_set_station_args(StationArgs *args);
	//int rtdb_set_commlink_args(CommLinkArgs *args);
	int rtdb_add_default_commlink();
	int rtdb_delete_commlink(UINT cid);
	//void rtdb_set_commlink_start(UINT cid);
	//void rtdb_set_commlink_state(UINT cid, int state, int rxbyte, 
 //       int rxright, int txbyte, int txright, float rts,int iFepID);
	//void rtdb_set_commlink_level(UINT cid, int level,BOOL bAuto);
	int rtdb_set_channel_args(ChannelArgs *args);
	UINT rtdb_get_first_channel(int *index);
	UINT rtdb_get_next_channel(int *index);
	int rtdb_add_default_channel();
	int rtdb_delete_channel(UINT cid);
	int rtdb_get_analog_num(UINT stid);
	int rtdb_get_digit_num(UINT stid);
	//int rtdb_get_pulse_num(UINT stid);
	int rtdb_get_relay_num(UINT stid);
	int rtdb_get_max_analog(UINT stid);
	int rtdb_get_max_digit(UINT stid);
	//int rtdb_get_max_pulse(UINT stid);
	int rtdb_get_max_relay(UINT stid);
	int rtdb_get_analog(AnalogTable *analog);
	int rtdb_get_analog_iNo(int iNo, AnalogTable *analog);
	int rtdb_get_analog_by_id(int iNo, AnalogTable *analog);
	int rtdb_set_analog(AnalogTable *analog);
	int rtdb_get_digit(DigitTable *digit);
	int	rtdb_get_digit_iNo(int iNo, DigitTable *digit);
	int rtdb_get_digit_by_id(int iNo, DigitTable *digit);
	int rtdb_set_digit(DigitTable *digit);
	//int rtdb_get_pulse(PulseTable *pulse);
	//int rtdb_get_pulse_iNo(int iNo, PulseTable *pulse);
	//int rtdb_get_pulse_by_id(UINT id, PulseTable *pulse);
	//int rtdb_set_pulse(PulseTable *pulse);
	UINT rtdb_get_relay_id(UINT stid, int point);
	int rtdb_get_relay(RelayTable *table);
	int rtdb_get_relay_iNo(int iNo, RelayTable *table);
	int rtdb_get_relay_by_id(UINT id, RelayTable *table);
	void rtdb_process_analog(AnalogPackets *packet);
	void rtdb_process_digit(DigitPackets *packet);
	//void rtdb_process_pulse(PulsePackets *packet);
	void _sync_timer();
	void _check_srs();
	void _proc_comm_statis();
	void _reload_station();
	void _set_station_state(UINT uid, int state, int alarm);
	void _proc_station_state();
	int rtdb_get_message(UINT sid, MesBody *mes);
	void _proc_relay_check(MesBody *mes);
	void _proc_soe_mes(MesBody *mes);
	void _proc_fault_mes(MesBody *mes);
	void _proc_protect_mes(MesBody *mes);
	void rtdb_process_message(MesBody *mes);
	int rtdb_proc_comm_state(UINT sid, UINT cid, int level,
							int relateid, int state);
	void _transmit_digit_event(UINT id, int value);
	void _transmit_soe_event(UINT id, int value, SYSTEMTIME *time);
	int rtdb_save_table(int type, char *buffer);

	bool rtdb_commlink_valid(int commlinkID, int & iDefaultHost);
	void rtdb_set_commlink_fepnode(UINT cid, int iFepID);
	void rtdb_set_station_commu(UINT stid,int commuid);
	bool rtdb_get_station_state(UINT stid,int & iState);

	_smap_set* _get_stations();

	void smap_load_stations(struct _smap_set *set);
	int smap_init();
	int smap_reload_stations();
	int smap_get_digit_num(UINT stid);
	int smap_get_analog_num(UINT stid);
	int smap_get_pulse_num(UINT stid);
	int smap_get_relay_num(UINT stid);
//	BOOL rt_set_commuflag(int id,int iFlagBit,BOOL bValue);

	UINT smap_get_digit_id(UINT stid, int index);
	UINT smap_get_digit_ino_id(UINT stid, int iNo, int & iIndex);
	int smap_get_max_digit(UINT stid);
	UINT smap_get_analog_id(UINT stid, int index);
	UINT smap_get_analog_ino_id(UINT stid, int iNo, int & iIndex);
	int smap_get_max_analog(UINT stid);
	UINT smap_get_pulse_id(UINT stid, int index);
	UINT smap_get_pulse_ino_id(UINT stid, int iNo, int & iIndex);
	int smap_get_max_pulse(UINT stid);
	UINT smap_get_relay_id(UINT stid, int index); 
	UINT smap_get_relay_ino_id(UINT stid, int iNo, int & iIndex); 
	int smap_get_max_relay(UINT stid);
	int smap_get_station_state(UINT sid);
	int smap_set_comm_state(UINT sid, UINT cid, int level, int relateid, int state);
	void _clear_message(MesStruct **link);
	void smap_append_message(MesBody *body);
	int smap_get_message(UINT stid, MesBody *mbody);
	void _load_station_from_file(struct _smap *tmp, char *file);
	_file_table* _get_file_table_list(char *filename);
	void _free_file_table_list(struct _file_table *sup); 
	char* _get_file_from_list(struct _file_table *sup, int id);
	int smap_get_station_point(int type, UINT id,  
			UINT *stid_array, int *point_array, int size);
	void rt_switch_fep(int iState,BOOL bAuto = TRUE);
	INT  rtdb_get_commlink_fepnode(UINT cid);
	CMultiDataBase *getMainDb();
	CScadaDataBase *getScadaDb();
	CMsgDealer *getMsgDealer();

public:		
	CDataBase systemDB;
	SYSTEM_NetNode m_Node;
	char			m_strHostName[100];

	int rtdb_get_commlink_args(CommLinkArgs *args);
	int rtdb_get_channel_args(ChannelArgs *args);
	void rtdb_check_on_time();
	int rtdb_is_reload_commlink();
	UINT rtdb_get_first_commlink(int *index);
	UINT rtdb_get_next_commlink(int *index);
	int GetSoeAlarmClass();
	int GetSystemAlarmClass();
	void GetDisplayByValue(int iValue,int iType,int iActionID,char* strDisplay);

	SYSTEM_Process * GetFepProcess();
	void GetFepSwitchMsg(FEPSWITCHMSG * pSwitchMsg);
	void WriteLog(int iMainType,char* strMsg);

	bool GetGlobalValueString(char* strField, char* strValue);
	void GetFepStatus(obj_fepnode_status & fepnode);
	//根据 key 找Index 

private:
	CMsgDealer * _msg_dealer;
	CMultiDataBase *dbmg_mdb;
	CScadaDataBase *dbmg_sdb;
	FEPSWITCHMSG m_fepSwitchMsg;
	int _register;
	volatile int _srs_state;
	volatile int _reload_flag;

	UtilTimer     _timer_srs; 

	//CFEPFuncGlobal *m_pGlobal;
	CFEPFuncUtil m_FuncUtil;
	_smap_set* _old_set;

	int m_iHostID;
	int m_iGroupID;
	int m_pGroupFep[10];
	int m_pFepGroupNum;

#ifdef WINDOWS_ENV
      HANDLE   hRWMutex;                 //读写、写写互斥锁   
      HANDLE   hReaderEve;             //读事件   
      HANDLE   hWriterEve;             //写事件   
      long   iRefReader;                 //读者引用记数   
      long   iRefWriter;                 //写者引用记数   
      bool   bWriterFirst;             //通过写锁优先开关，避免提前优化带来的负面影  
	  void pthread_rwlock_rdlock(pthread_rwlock_t* rwlock);
	  void pthread_rwlock_wrlock(pthread_rwlock_t* rwlock);
	  void pthread_rwlock_unlock(pthread_rwlock_t* rwlock);
#else   
      pthread_rwlockattr_t         attr;               //读写锁属性对象   
#endif   
      pthread_rwlock_t             _slock;         //读写锁           
    bool m_bInitLockFlag;
};

#endif //_FEP_FUNCTION_RTDB_H_
