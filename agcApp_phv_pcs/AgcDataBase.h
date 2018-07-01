#ifndef __AGC_DATA_BASE_DEFINED_H
#define __AGC_DATA_BASE_DEFINED_H
#include "gSystemDef.h"
#include "MultiDataBase.h"
#include "AGC.h"
class CAgcDataBase:public CDataBase
{
public :
	CAgcDataBase();
	~CAgcDataBase();
public:
	//根据表的ID查找表的记录个数,
	//tableID eg:AGC_TAB_PhvUnit_ID
    int getRecordCountByID(UINT tableID);
	//根据表id查询记录首地址
	////tableID eg:AGC_TAB_PhvUnit_ID
	LPSTR  getDataAddress(UINT tableID);
	//初使化Agc所用的表
	void initAgcTable();
	private:
	BOOL getPhvUnitTable();
	BOOL getAgcAnalogTable();
	BOOL getAgcDigitTable();
	BOOL getAgcParameter();
	BOOL getAgcAlarm();
	BOOL getAgcLogRecord();
	BOOL getSocUnitTable();

private:
	BOOL m_bPhvUnitTableOK;//是否初使化过机组
	BOOL m_bAgcAnalogTableOK;//AGC遥测表
	BOOL m_bAgcDigitTableOK;//AGC遥信表
	BOOL m_bParameterTableOK;//参数表
	BOOL m_bLogRecordTableOK;//参数表
	BOOL m_bAlarmTableOK;//报警数表
	BOOL m_bSocUnitTableOK;//是否初使化过储能电池

	CDBTable		m_tabPhvUnit;
	CDBTable		m_tabAgcAnalog;
	CDBTable		m_tabAgcDigit;
	CDBTable        m_tabParameter;
	CDBTable        m_tabAlarm;
	CDBTable        m_tabLogRecord;
	CDBTable        m_tabSocUnit;//储能电池
	
};
#endif