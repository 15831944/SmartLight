#include "AgcDataBase.h"

//CAgcDataBase::CAgcDataBase()
//2016-10-12 tjs
CAgcDataBase::CAgcDataBase():CDataBase()
{
	//CDataBase::CDataBase(); //ubuntu 12.04版本编译不通过 gcc 4.6.3
    m_bPhvUnitTableOK=FALSE;
	m_bAgcDigitTableOK=FALSE;
	m_bAgcAnalogTableOK=FALSE;
	 m_bParameterTableOK=FALSE;
	 m_bLogRecordTableOK=FALSE;
	 m_bAlarmTableOK=FALSE;
	 m_bSocUnitTableOK=FALSE;
}
CAgcDataBase::~CAgcDataBase()
{

}
void CAgcDataBase::initAgcTable()
{
   getPhvUnitTable();
   getAgcAnalogTable();
   getAgcDigitTable();
   getAgcParameter();
   getAgcAlarm();
   getAgcLogRecord();
   getSocUnitTable();
}

BOOL CAgcDataBase::getPhvUnitTable()
{
	if(m_bPhvUnitTableOK)
		return TRUE;
	
	if(GetDBTableFromID(m_tabPhvUnit,AGC_TAB_PhvUnit_ID ) )
		m_bPhvUnitTableOK = TRUE;

	return m_bPhvUnitTableOK;
}
BOOL CAgcDataBase::getAgcAnalogTable()
{
	if(m_bAgcAnalogTableOK)
		return TRUE;

	if(GetDBTableFromID(m_tabAgcAnalog,AGC_TAB_Analog_ID ) )
		m_bAgcAnalogTableOK = TRUE;

	return m_bAgcAnalogTableOK;
}
BOOL CAgcDataBase::getAgcDigitTable()
{
	if(m_bAgcDigitTableOK)
		return TRUE;

	if(GetDBTableFromID(m_tabAgcDigit,AGC_TAB_Digit_ID ) )
		m_bAgcDigitTableOK = TRUE;

	return m_bAgcDigitTableOK;
}
BOOL CAgcDataBase::getAgcParameter()
{
	if( m_bParameterTableOK)
		return TRUE;

	if(GetDBTableFromID(m_tabParameter,AGC_TAB_Parameter_ID ) )
		 m_bParameterTableOK= TRUE;

	return  m_bParameterTableOK;
}
BOOL CAgcDataBase::getAgcAlarm()
{
	if( m_bAlarmTableOK)
		return TRUE;

	if(GetDBTableFromID(m_tabAlarm,AGC_TAB_Alarm_ID ) )
		m_bAlarmTableOK= TRUE;

	return  m_bAlarmTableOK;
}
int CAgcDataBase::getRecordCountByID(UINT tableID)
{
	int nCount=0;
	switch(tableID)
	{
	case AGC_TAB_PhvUnit_ID:
		nCount=m_tabPhvUnit.GetRecordCount();
		break;
	case AGC_TAB_Parameter_ID:
		nCount=m_tabParameter.GetRecordCount();
		break;
	case AGC_TAB_Analog_ID:
		nCount=m_tabAgcAnalog.GetRecordCount();
		break;
	case AGC_TAB_Digit_ID:
		nCount=m_tabAgcDigit.GetRecordCount();
		break;
	case AGC_TAB_Alarm_ID:
		nCount=m_tabAlarm.GetRecordCount();
		break;
	case AGC_TAB_LogRecord_ID:
		nCount=m_tabLogRecord.GetRecordCount();
		break;
	case AGC_TAB_SocUnit_ID:
		nCount=m_tabSocUnit.GetRecordCount();
		break;
	default:
         nCount=0;
	}
  return nCount;
}

LPSTR  CAgcDataBase::getDataAddress(UINT tableID)
{

	LPSTR ret=NULL;
	int nCount=0;
	switch(tableID)
	{
	case AGC_TAB_PhvUnit_ID:
		ret=m_tabPhvUnit.GetDataAddress();
		break;
	case AGC_TAB_Parameter_ID:
		ret=m_tabParameter.GetDataAddress();
		break;
	case AGC_TAB_Analog_ID:
		ret=m_tabAgcAnalog.GetDataAddress();
		break;
	case AGC_TAB_Digit_ID:
		ret=m_tabAgcDigit.GetDataAddress();
		break;
	case AGC_TAB_Alarm_ID:
		ret=m_tabAlarm.GetDataAddress();
		break;
	case AGC_TAB_LogRecord_ID:
		ret=m_tabLogRecord.GetDataAddress();
		break;
	case AGC_TAB_SocUnit_ID:
		ret=m_tabSocUnit.GetDataAddress();
	default:
		break;
	}
	return ret;
}
BOOL CAgcDataBase::getAgcLogRecord()
{
		if(m_bLogRecordTableOK)
			return TRUE;

		if(GetDBTableFromID(m_tabLogRecord,AGC_TAB_LogRecord_ID ) )
			m_bLogRecordTableOK = TRUE;

		return m_bLogRecordTableOK;
}
BOOL CAgcDataBase::getSocUnitTable()
{
		if(m_bSocUnitTableOK)
			return TRUE;

		if(GetDBTableFromID(m_tabSocUnit,AGC_TAB_SocUnit_ID ) )
			m_bSocUnitTableOK = TRUE;

		return m_bSocUnitTableOK;
}