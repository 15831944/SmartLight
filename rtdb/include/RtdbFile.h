/////////////////////////////////////////////////////////////////////////////
// RtdbFile.h : 实现实时库与关系库之间的读写操作
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
	//保存断面数据，wenjp
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
	//保存当前数到内存上，如果数据内容较大就返回
	//pMDB实时数据库主结构
	//dbName 数据库名
	//tbName 表名
	//nBeginPos开始的行数
	//pBuffer内存的地址
	//nMaxSize最大的内存大小
	//nWriteSize写入数据的大小
	//bFinFlg 表示数据全部读完
	bool	SaveDataToBuf(CMultiDataBase * pMDB,char *dbName,char *tbName,int &nBeginPos,char *pbuffer,int nMaxSize,int &nWriteSize,bool &bFinFlg);
	bool    SaveTableDataToBuf(CDataBase& db, CDBTable& tab,int &nBeginPos,char *pbuffer,int nMaxSize,int &nWriteSize,bool &bFinFlg);
	//end tjs
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_RTDB_FILE_RW_H_2005_02_22__
