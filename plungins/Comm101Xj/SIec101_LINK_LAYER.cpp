#include "SIec101_LINK_LAYER.h"
#include "Q_101StdDefine.h"
void SIec101_LINK_LAYER::setDeaultCount()
{
	nAckLinkStatusTimes = 0;
	nAckResetLinkTimes = 0;
}
void  SIec101_LINK_LAYER::addAckLinkStatusTimes()
{
	nAckLinkStatusTimes++;
}
void  SIec101_LINK_LAYER::addAckResetLinkTimes()
{
	nAckResetLinkTimes++;
}
int SIec101_LINK_LAYER::getAckResetLinkTimes()
{
	return nAckResetLinkTimes;
}
int  SIec101_LINK_LAYER::getAckLinkStatusTimes()
{
	return nAckLinkStatusTimes;
}
//主站向子站是否建立链路完成
bool SIec101_LINK_LAYER::getMasterInited()
{
	if (IEC104_LINK_PRM_STATE_LINKOK == Master_State)
	     return true;
	else 
		return false;
}
//子站向主站是否建立链路完成
bool SIec101_LINK_LAYER::getSlaveInited()
{
	if (IEC104_LINK_SUB_STATE_INITFIN == Slave_State)
		return true;
	else
		return false;

}
//设置主站的链路状态
void SIec101_LINK_LAYER::setMasterState(BYTE nStatus)
{
	Master_State = nStatus;
}
//设置子站向主站的链路状态
void SIec101_LINK_LAYER::setSlaveState(BYTE nStatus)
{
	Slave_State = nStatus;
}
BYTE SIec101_LINK_LAYER::getMaserState()
{
	return Master_State;
}
BYTE SIec101_LINK_LAYER::getSlaveStatus()
{
	return Slave_State;
}