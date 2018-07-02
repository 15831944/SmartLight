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
	//���ݱ��ID���ұ�ļ�¼����,
	//tableID eg:AGC_TAB_PhvUnit_ID
    int getRecordCountByID(UINT tableID);
	//���ݱ�id��ѯ��¼�׵�ַ
	////tableID eg:AGC_TAB_PhvUnit_ID
	LPSTR  getDataAddress(UINT tableID);
	//��ʹ��Agc���õı�
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
	BOOL m_bPhvUnitTableOK;//�Ƿ��ʹ��������
	BOOL m_bAgcAnalogTableOK;//AGCң���
	BOOL m_bAgcDigitTableOK;//AGCң�ű�
	BOOL m_bParameterTableOK;//������
	BOOL m_bLogRecordTableOK;//������
	BOOL m_bAlarmTableOK;//��������
	BOOL m_bSocUnitTableOK;//�Ƿ��ʹ�������ܵ��

	CDBTable		m_tabPhvUnit;
	CDBTable		m_tabAgcAnalog;
	CDBTable		m_tabAgcDigit;
	CDBTable        m_tabParameter;
	CDBTable        m_tabAlarm;
	CDBTable        m_tabLogRecord;
	CDBTable        m_tabSocUnit;//���ܵ��
	
};
#endif