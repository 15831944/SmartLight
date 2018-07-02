// Converse.h: ������ ת���� �����ݸ�ʽ ������
//				�����ļ� conversion.ini
//				����������Conversion()����
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
		//; &  ���־ (��ת���ı���д����ʹ��־һ����ת��ǰ��ʼ����0 ,  ��λֱ��^�ָ� )
		//; @ ������ת���� type����  (��ת���ı���д�� ת��ǰ��ʼ����1 )
		//; $  type���͵�ת�� (��ת���ı���д�� ת��ǰ��ʼ����1 )
	
	int FindFieldIndex(  CString headline, const char* field );
		//���ֶ���headline�е�λ��

protected:
	CString	m_old_dir;
	CString	m_new_dir;
	CString	m_contrast_dir;
};

#endif // !defined(AFX_CONVERSE_H__400186E7_9B7E_47C4_8192_63AD9EC33658__INCLUDED_)
