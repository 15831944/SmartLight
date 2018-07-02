#ifndef _SIEC101_LINK_LAYER_H
#define _SIEC101_LINK_LAYER_H
#include "gSystemDef.h"
//��·������
class SIec101_LINK_LAYER
{
public:
	bool	Sub_Inited_Flag;//�Ƿ��ʹ���
	BYTE	FCB_Sub;
	BYTE	FCB_Main;
	BYTE	DFC_Sub;
	BYTE	ACD_Sub;
	time_t 	LastTxTime;

	int   nAckSubCount;
private:
	int   nAckResetLinkTimes;//�ӵ�����վ����֡��
	int   nAckLinkStatusTimes;//�ظ���վ��·״̬
	BYTE     Master_State;//��վ��·״̬
	BYTE	 Slave_State;//��վ��·״̬
public:
	void setDeaultCount();
	void  addAckLinkStatusTimes();
	void  addAckResetLinkTimes();
	int getAckResetLinkTimes();
	int  getAckLinkStatusTimes();
	//��վ����վ�Ƿ�����·���
	bool getMasterInited();
	//��վ����վ�Ƿ�����·���
	bool getSlaveInited();
	//������վ����·״̬
	void setMasterState(BYTE nStatus);
	//������վ����վ����·״̬
	void setSlaveState(BYTE nStatus);
	BYTE getMaserState();
	BYTE getSlaveStatus();

};
#endif