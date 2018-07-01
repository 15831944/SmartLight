// Converse.h: 旧数据 转换成 新数据格式 的主类
//				配置文件 conversion.ini
//				调用主函数Conversion()即可
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVERSE_H__400186E7_9B7E_47C4_8192_63AD9EC33658__INCLUDED_)
#define AFX_CONVERSE_H__400186E7_9B7E_47C4_8192_63AD9EC33658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CConverse  
{
public:
	CConverse();
	virtual ~CConverse();

public:
	void Conversion();

private:
	BOOL TransTableFile( char* dbname,  const char* tablename ) ;
	void TransHead( CStdioFile* oldFile,  const char* newFile_Name,  CString& new_head) ;

	void TransTableData( char* dbname, const char* tablename , CStdioFile* oldFile, CStdioFile* newFile, CString headline );
	void TransFieldData( char* sLine, INT iIndex, char cType, CString strConverse );
		//; &  与标志 (需转换的必须写，即使标志一样，转换前初始化成0 ,  各位直接^分隔 )
		//; @ 与类型转换成 type类型  (需转换的必须写， 转换前初始化成1 )
		//; $  type类型的转换 (需转换的必须写， 转换前初始化成1 )
	
	int FindFieldIndex(  CString headline, const char* field );
		//找字段在headline中的位置

protected:
	CString	m_old_dir;
	CString	m_new_dir;
	CString	m_contrast_dir;
};

#endif // !defined(AFX_CONVERSE_H__400186E7_9B7E_47C4_8192_63AD9EC33658__INCLUDED_)
