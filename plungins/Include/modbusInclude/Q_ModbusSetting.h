#pragma once
#include "Q_frameInclude.h"
#include "Q_ModbusDef.h"
#include "Q_ModbusDefine.h"
#include <vector>
using namespace std;

class __Q_COMMMODBUS_EXPORT Q_ModbusSetting : public Q_protocolSettingBase
{
public:
	Q_ModbusSetting(void);
	virtual ~Q_ModbusSetting(void);
	void setFilePath(char *);
	bool initSetting();
protected:
	vector<ST_ModCfgT *> m_vContent;//����
	vector<ST_ModCfgT *> m_vGroup;//������
	char *getContentID(const int &v_idx,const int &v_grp=1);
private:
	char m_sFilePath[100];//�ļ�·��
	char		LastError[100];         //������Ϣ
	bool init();
};