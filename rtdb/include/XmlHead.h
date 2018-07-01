#pragma once

#ifndef XML________H
#define XML________H
#include <list>
//#include <array>
#include <vector>
using namespace std;

#define LINECOUNT  3000
#define DBCOUNT	   10
#define TABLECOUNT 100
#define FILECOUNT  200
typedef struct XmlDBStruct
{
	char szDBName[100];//DB名称
	char szCName[100];//DB中文名称
	int  nDBId;//DB ID号
	int  nDBType;//DB的类型
	int  nTableMax;//DB的最大表的数量
}XmlDBStruct,pXmlDBStruct;  

typedef struct XmlTable
{
	int  nDBId;//DBID

	int  nDBType;//DB的类型
	char szName[100];//表名称
	char szCName[100];//表中文名称
	int  nTableId;//表 ID号
	int  nIndex;//表 索引号
	int  nMax;//表的最大数量
	int  nFieldNum;//表的字段数量
	int  nFlag;//表的标志
	char szPriKey[100];//表的PriKey
	char szPath[100];//表的Path
}XmlTable, *pXmlTable; 

typedef struct XmlField
{
	int  nDBId;//DBID
	int  nDBType;//DB的类型
	int  nTableId;//表 ID号
	char szName[100];//字段名称
	char szCName[100];//字段中文名称
	int  nFieldId;//字段ID号
	int  nIndex;//字段索引号
	char szFieldType[100];//字段类型
	int  nSize;//字段大小
	int  nFlag;//字段的标志
	int  nKeyGroup;//字段组
	int  nShift;//变化
	char szDefaultValue[100];//默认值
}XmlField, *pXmlField; 

#else
#endif


