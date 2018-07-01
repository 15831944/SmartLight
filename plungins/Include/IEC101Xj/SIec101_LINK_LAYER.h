#ifndef _SIEC101_LINK_LAYER_H
#define _SIEC101_LINK_LAYER_H
#include "gSystemDef.h"
//链路层数据
class SIec101_LINK_LAYER
{
public:
	bool	Sub_Inited_Flag;//是否出使完成
	BYTE	FCB_Sub;
	BYTE	FCB_Main;
	BYTE	DFC_Sub;
	BYTE	ACD_Sub;
	time_t 	LastTxTime;

	int   nAckSubCount;
private:
	int   nAckResetLinkTimes;//接到的子站上数帧数
	int   nAckLinkStatusTimes;//回复子站链路状态
	BYTE     Master_State;//主站链路状态
	BYTE	 Slave_State;//子站链路状态
public:
	void setDeaultCount();
	void  addAckLinkStatusTimes();
	void  addAckResetLinkTimes();
	int getAckResetLinkTimes();
	int  getAckLinkStatusTimes();
	//主站向子站是否建立链路完成
	bool getMasterInited();
	//子站向主站是否建立链路完成
	bool getSlaveInited();
	//设置主站的链路状态
	void setMasterState(BYTE nStatus);
	//设置子站向主站的链路状态
	void setSlaveState(BYTE nStatus);
	BYTE getMaserState();
	BYTE getSlaveStatus();

};
#endif