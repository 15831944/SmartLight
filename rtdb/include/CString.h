#ifndef STL_H_BAIKELKEY_CSTRING
#define STL_H_BAIKELKEY_CSTRING
//#pragma warning(disable:4786) 
//#pragma warning(disable:4275) 
#include "gSystemDef.h"
#include <assert.h>
#ifdef UNIX_ENV

#include <iostream>
#include <algorithm>
#include <string>
#include <cstdarg>  

#define _min(a,b) ((a)<(b)?(a):(b))
#define _max(a,b) ((a)>(b)?(a):(b))
#define bzero(ptr,n) memset(ptr,0,n)
class CString : public std::string  
{	
public:
	int Delete( int nIndex, int nCount = 1 )
	{
		this->erase(nIndex,nCount);
		return this->GetLength();
	}
	int Insert( int nIndex, const char * pstr )
	{
		this->insert(nIndex,pstr);
		return this->GetLength();	
	}
	int Insert( int nIndex, char ch )
	{	
		CString strTmp(ch);
		this->insert(nIndex,strTmp);
		strTmp.Empty();
		return this->GetLength();
	}
	int Remove( char ch )
	{
		CString::iterator iter;
		int count = 0;
		for(iter = this->begin(); iter != this->end();iter ++)
		{
			if(*iter == ch)
			{
				this->erase(iter);count++;
			}
		}
		return count;
	}
	void MakeReverse( )
	{
		CString strTmp;
		CString::iterator iter;
		iter=this->end();
		iter--;
		for(; iter != this->begin(); iter--)
		{
			strTmp += *iter;
		}
		strTmp += *iter;
		*this = strTmp;
		strTmp.Empty();
	}
	int Find( char ch ) const
	{
		return this->find(ch);
	}
	int Find( const char * lpszSub ) const
	{
		return this->find(lpszSub);
	}
	int Find( char ch, int nStart ) const
	{
		return this->find(ch,nStart);
	}
	int Find( const char * pstr, int nStart ) const
	{
		return this->find(pstr,nStart);
	}
	int ReverseFind( char ch ) const
	{
		return this->find_last_of(ch);	
	}
	int FindOneOf( const char * lpszCharSet ) const
	{
		return this->find_first_of(lpszCharSet);
	}
	int Format(const char* pstrFormat, ... )
	{///����������֧��ANSI��׼�ַ��� '%[flags] [width] [.precision] [{h | l | I64 | L}]type'	 	
		assert(pstrFormat!=NULL);
		this->Empty();
		va_list argList;
		va_start(argList,pstrFormat); 
		int nMaxLen = 0;
		for (const char * p = pstrFormat; *p != '\0';p++ )
		{
			if (*p != '%' || *(++p) == '%')
			{// �������'%'��ֱ���ۼƳ��ȣ������'%%'Ҳʹ���ȼ�1
				nMaxLen += 1;
				continue;
			}
			int nItemLen = 0; //��������ÿ�������ĳ���
			int nWidth = 0; //��������ÿһ��Ŀ��
			for (; *p != '\0'; p ++)
			{
				if (*p == '#')
					nMaxLen += 2;   // ���� '0x'
				else if (*p == '*')
					nWidth = va_arg(argList, int);  //�磺'%5f' �е�5
				else if (*p == '-' || *p == '+' || *p == '0'|| *p == ' ')
					;  //���Ը÷���
				else // ���Ǳ�־�ַ����˳�ѭ��
					break;
			}
			if (nWidth == 0)
			{ //��ȡ���
				nWidth = atoi(p);
				for (; *p != '\0' && isdigit(*p); p ++)
					;
			}
			assert(nWidth >= 0);//��Ч���
			int nPrecision = 0; //����λ��
			if (*p == '.')
			{
				p++;// ���� '.'�ַ� (���.����)			
				if (*p == '*')
				{ //�в�������
					nPrecision = va_arg(argList, int);
					p ++;// ȡ�þ��ȣ������ַ�
				}
				else
				{ //�ڸ�ʽ�����п��
					nPrecision = atoi(p);
					for (; *p != '\0' && isdigit(*p);	p ++)
						;
				}
				assert(nPrecision >= 0);//��Ч���
			}
			switch (*p)
			{
				case 'h':     //short int ��
					p ++;
					break;
				case 'l':	 //long double ��
					p ++;
					break;
				case 'F':	 //��ָ��
				case 'N':	//Զָ��	
				case 'L':	//long double ��
					p++;
					break;
			}
			switch (*p)
			{
				case 'c':   //// �����ַ�
				case 'C':
					nItemLen = 2;
					va_arg(argList, char);
					break;
				case 's':	//// �ַ���
				case 'S':
					nItemLen = strlen(va_arg(argList, const char*));
					nItemLen = ((1) > (nItemLen)) ? (1) : (nItemLen);//����ǿմ���ʹ��1 ������'\0'
					break;
			}
			if (nItemLen != 0)		
			{
				nItemLen = ((nItemLen) > (nWidth)) ? (nItemLen) : (nWidth);//ʹ�ô���
				if (nPrecision != 0)
					nItemLen = ((nItemLen) < (nPrecision)) ? (nItemLen) : (nPrecision);
			}
			else
			{
				switch (*p)
				{
					case 'd':    //�����Ĵ���
					case 'i':
					case 'u':
					case 'x':
					case 'X':
					case 'o':
						va_arg(argList, int);
						nItemLen = 32;  //���ֽ�
						nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//ʹ�ô���
						break;
					case 'e':	//������
					case 'f':
					case 'g':
					case 'G':
						va_arg(argList, double);
						nItemLen = 32;//���ֽ�
						nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//ʹ�ô���;
						break;
					case 'p':	//ָ��
						va_arg(argList, void*);
						nItemLen = 32;
						nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//ʹ�ô���;
						break;
					case 'n':
						va_arg(argList, int*); //ָ��������ָ��,��BorlanderC++3.1�⺯��P352
						break;
					default:
						assert(false);  //���ܴ���ĸ�ʽ����������
					}
				}
				nMaxLen += nItemLen;//�Ѹ���ĳ����ۼ�
			}
			va_end(argList);
			va_start(argList, pstrFormat);  // ���¿�ʼ��ȡ����
			char* ch = new char[nMaxLen+1]; //�����ڴ�
			vsprintf(ch, pstrFormat, argList);
			//assert(vsprintf(ch, pstrFormat, argList) <= nMaxLen);
			this->append(ch); //�ӵ�string��β��
			delete[] ch; //�ͷ��ڴ�
			va_end(argList);
			return nMaxLen;
	}
	int GetLength() const
	{
		return this->length();
	}
	CString Left(int nCount) const
	{
		if (nCount <=0) 
			return CString("");	
		CString strTmp;
		strTmp = this->substr(0,nCount);
		return strTmp;
	}
	CString Right(int nCount) const
	{
		if (nCount <=0) 
			return CString("");
		CString strTmp;
		if (nCount > GetLength())
			strTmp = this->substr(0);
		else
			strTmp = this->substr(GetLength()-nCount);
		return strTmp;
	}
	CString Mid(int nFirst) const
	{
		CString strTmp;
		if (nFirst >= GetLength())
			return CString("");
		if (nFirst <= 0)
			strTmp = this->substr(0);
		else
			strTmp = this->substr(nFirst);
		return strTmp;		
	}
	CString Mid( int nFirst, int nCount) const
	{
		if (nCount <= 0) 
			return CString("");
		if (nFirst >= GetLength())
			return CString("");
		CString strTmp;
		if (nFirst <= 0)
			strTmp = this->substr(0,nCount);
		else
			strTmp = this->substr(nFirst,nCount);
		return strTmp;				
	}
	CString& operator=(const std::string str)
	{
		if (this->compare(str) == 0) return *this;
		this->assign(str);
		return *this;
	}
	CString& operator=(char ch)
	{
		this->Empty();
		this->insert(this->begin(),ch);
		return *this;
	}
	CString& operator =( const char * lpsz )
	{
		this->Empty();
		this->append(lpsz);
		return *this;
	}
	void MakeUpper()
	{
		std::transform(this->begin (),
			this->end (),this->begin (),
			toupper);
	}
	void MakeLower()
	{
		std::transform(this->begin (),
			this->end (),this->begin (),
			tolower);
	}
	bool IsEmpty( ) const
	{
		return this->empty(); 
	}
	void Empty( )
	{
		this->erase(this->begin(),this->end());
	}
	char GetAt( int nIndex ) const
	{
		return this->at(nIndex);
	}
	char operator []( int nIndex ) const
	{
		return this->at(nIndex);
	}
	void SetAt( int nIndex, char ch )
	{
		this->at(nIndex) = ch;
	}
	operator const char * ( ) const
	{
		return this->c_str();
	}
	friend CString operator + (const CString& string1, const CString& string2)
	{
		CString str;
		str.append(string1);
		str.append(string2);
		return str;
	}
	friend CString operator + ( const CString& string1, char ch )
	{
		CString str;
		str.append(string1);
		str.insert(str.end(),ch);
		return str;
	}
	friend CString operator + ( const CString& string1, char* ch )
	{
		CString str;
		str.append(string1);
		str.append(ch);
		return str;
	}
	CString& operator += (const CString& string1)
	{
		this->append(string1);
		return *this;
	}
	CString& operator += (char* ch)
	{
		this->append(ch);
		return *this;
	}
	int Compare( const char * lpsz ) const
	{
		CString str;
		str.append(lpsz);
		return this->compare(str);
	}
	int Compare( const CString& string1 ) const
	{
		return this->compare(string1);
	}
	int CompareNoCase( const char * lpsz ) const
	{
		CString str,strThis;
		str.append(lpsz);
		strThis = (*this);
		str.MakeLower();
		strThis.MakeLower();
		return strThis.compare(str);
	}
	int CompareNoCase( const CString& string1 ) const
	{
		CString str,strThis;
		str = string1;
		strThis = (*this);
		str.MakeLower();
		strThis.MakeLower();
		return strThis.compare(str);
	}
	void TrimRight( )
	{
		TrimRight (' ');
	}
	void TrimLeft( )
	{   
		TrimLeft(' ');
	}
	void TrimLeft( char chTarget )
	{
		std::string::size_type pos;
		pos = this->find_first_not_of(chTarget);
		if (pos == 0) return; 
		this->erase(this->begin(),this->begin()+pos);		
	}
	void TrimRight( char chTarget )
	{
		std::string::size_type pos;
		pos = this->find_last_not_of(chTarget);
		++pos;
		if (pos == (unsigned int)this->GetLength())
			return; 
		this->erase(this->begin()+pos,this->end());		
	}
	void Replace( char chOld, char chNew )
	{
		for(int i=0;i<this->GetLength();i++)
		{
			if (this->at(i) == chOld)
				this->at(i) = chNew;
		}		
	}
	void Replace(const char* chOld,const char* chNew )
	{
		int index = this->find(chOld);
		while (index > -1)
		{
			this->erase(index,strlen(chOld));
			this->insert(index,chNew);
			index = this->find(chOld);
		}
	}		
/* ������������ڵͰ汾��gcc���ܹ�����ͨ��,���߰汾gcc������ִ���
	char * GetBuffer( int nMinBufLength )
	{
		this->resize(nMinBufLength);
		return this->begin();
	}
*/
	void ReleaseBuffer( int nNewLength = -1 )
	{
		this->TrimRight('\0');
	}
	CString(const CString& string1)
	{
		this->append(string1);
	}
	CString(const char *ch)
	{
		this->append(ch);
	}
	CString(const char ch)
	{
		*this += ch;
	}
	CString()
	{}
};
#endif
#endif
