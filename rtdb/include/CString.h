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
	{///本函数仅仅支持ANSI标准字符集 '%[flags] [width] [.precision] [{h | l | I64 | L}]type'	 	
		assert(pstrFormat!=NULL);
		this->Empty();
		va_list argList;
		va_start(argList,pstrFormat); 
		int nMaxLen = 0;
		for (const char * p = pstrFormat; *p != '\0';p++ )
		{
			if (*p != '%' || *(++p) == '%')
			{// 如果不是'%'就直接累计长度，如果是'%%'也使长度加1
				nMaxLen += 1;
				continue;
			}
			int nItemLen = 0; //用来保存每个参数的长度
			int nWidth = 0; //用来保存每一项的宽度
			for (; *p != '\0'; p ++)
			{
				if (*p == '#')
					nMaxLen += 2;   // 处理 '0x'
				else if (*p == '*')
					nWidth = va_arg(argList, int);  //如：'%5f' 中的5
				else if (*p == '-' || *p == '+' || *p == '0'|| *p == ' ')
					;  //忽略该符号
				else // 不是标志字符就退出循环
					break;
			}
			if (nWidth == 0)
			{ //提取宽度
				nWidth = atoi(p);
				for (; *p != '\0' && isdigit(*p); p ++)
					;
			}
			assert(nWidth >= 0);//有效宽度
			int nPrecision = 0; //精度位数
			if (*p == '.')
			{
				p++;// 跳过 '.'字符 (宽度.精度)			
				if (*p == '*')
				{ //有参数给出
					nPrecision = va_arg(argList, int);
					p ++;// 取得精度，跳过字符
				}
				else
				{ //在格式串中有宽度
					nPrecision = atoi(p);
					for (; *p != '\0' && isdigit(*p);	p ++)
						;
				}
				assert(nPrecision >= 0);//有效宽度
			}
			switch (*p)
			{
				case 'h':     //short int 型
					p ++;
					break;
				case 'l':	 //long double 型
					p ++;
					break;
				case 'F':	 //近指针
				case 'N':	//远指针	
				case 'L':	//long double 型
					p++;
					break;
			}
			switch (*p)
			{
				case 'c':   //// 单个字符
				case 'C':
					nItemLen = 2;
					va_arg(argList, char);
					break;
				case 's':	//// 字符串
				case 'S':
					nItemLen = strlen(va_arg(argList, const char*));
					nItemLen = ((1) > (nItemLen)) ? (1) : (nItemLen);//如果是空串就使用1 即保存'\0'
					break;
			}
			if (nItemLen != 0)		
			{
				nItemLen = ((nItemLen) > (nWidth)) ? (nItemLen) : (nWidth);//使用大者
				if (nPrecision != 0)
					nItemLen = ((nItemLen) < (nPrecision)) ? (nItemLen) : (nPrecision);
			}
			else
			{
				switch (*p)
				{
					case 'd':    //整数的处理
					case 'i':
					case 'u':
					case 'x':
					case 'X':
					case 'o':
						va_arg(argList, int);
						nItemLen = 32;  //四字节
						nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//使用大者
						break;
					case 'e':	//浮点数
					case 'f':
					case 'g':
					case 'G':
						va_arg(argList, double);
						nItemLen = 32;//四字节
						nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//使用大者;
						break;
					case 'p':	//指针
						va_arg(argList, void*);
						nItemLen = 32;
						nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//使用大者;
						break;
					case 'n':
						va_arg(argList, int*); //指向整数的指针,见BorlanderC++3.1库函数P352
						break;
					default:
						assert(false);  //不能处理的格式，给出警告
					}
				}
				nMaxLen += nItemLen;//把该项的长度累计
			}
			va_end(argList);
			va_start(argList, pstrFormat);  // 重新开始提取参数
			char* ch = new char[nMaxLen+1]; //分配内存
			vsprintf(ch, pstrFormat, argList);
			//assert(vsprintf(ch, pstrFormat, argList) <= nMaxLen);
			this->append(ch); //加到string的尾部
			delete[] ch; //释放内存
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
/* 下面这个函数在低版本的gcc中能够编译通过,但高版本gcc编译出现错误
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
