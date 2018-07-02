/////////////////////////////////////////////////////////////////////////////
// RtdbFile.h : ʵ��ʵʱ�����ϵ��֮��Ķ�д����
/////////////////////////////////////////////////////////////////////////////
#ifndef __AFX_RTDB_FILE_RW_H_2005_02_22__
#define __AFX_RTDB_FILE_RW_H_2005_02_22__
/////////////////////////////////////////////////////////////////////////////
#include "MultiDataBase.h"
class AFX_EXT_CLASS CRtdbFile
{
public:
	CRtdbFile(CMultiDataBase * pMultiDB);
	virtual ~CRtdbFile();

public:
	char 	m_pszPath[MAX_PATH];
	bool	m_bCName;
	char	m_cSeparator;
	char	m_szNameLink[10];
	char	m_szFileExt[6];
	//����������ݣ�wenjp
	bool	m_bDumpFile;

public:
	bool	SaveAllData(CMultiDataBase * pMDB);
	bool	SaveDbData(CDataBase& DataBase);
	bool	SaveTableData(CDataBase& db, CDBTable& tab);

	bool	InsertRecord(CDataBase& db, CDBTable& tab, UINT nID);
	bool	DeleteRecord(CDataBase& db, CDBTable& tab, UINT nID);
	bool	UpdateRecord(CDataBase& db, CDBTable& tab, UINT nID);

	bool	LoadAllData(CMultiDataBase * pMDB);
	bool	LoadDbData(CDataBase& DataBase);
	bool	LoadTableData(CDataBase& db, CDBTable& tab);
	//begin 2015-11-9 tjs 
	//���浱ǰ�����ڴ��ϣ�����������ݽϴ�ͷ���
	//pMDBʵʱ���ݿ����ṹ
	//dbName ���ݿ���
	//tbName ����
	//nBeginPos��ʼ������
	//pBuffer�ڴ�ĵ�ַ
	//nMaxSize�����ڴ��С
	//nWriteSizeд�����ݵĴ�С
	//bFinFlg ��ʾ����ȫ������
	bool	SaveDataToBuf(CMultiDataBase * pMDB,char *dbName,char *tbName,int &nBeginPos,char *pbuffer,int nMaxSize,int &nWriteSize,bool &bFinFlg);
	bool    SaveTableDataToBuf(CDataBase& db, CDBTable& tab,int &nBeginPos,char *pbuffer,int nMaxSize,int &nWriteSize,bool &bFinFlg);
	//end tjs
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_RTDB_FILE_RW_H_2005_02_22__
