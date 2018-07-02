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
	char szDBName[100];//DB����
	char szCName[100];//DB��������
	int  nDBId;//DB ID��
	int  nDBType;//DB������
	int  nTableMax;//DB�����������
}XmlDBStruct,pXmlDBStruct;  

typedef struct XmlTable
{
	int  nDBId;//DBID

	int  nDBType;//DB������
	char szName[100];//������
	char szCName[100];//����������
	int  nTableId;//�� ID��
	int  nIndex;//�� ������
	int  nMax;//����������
	int  nFieldNum;//����ֶ�����
	int  nFlag;//��ı�־
	char szPriKey[100];//���PriKey
	char szPath[100];//���Path
}XmlTable, *pXmlTable; 

typedef struct XmlField
{
	int  nDBId;//DBID
	int  nDBType;//DB������
	int  nTableId;//�� ID��
	char szName[100];//�ֶ�����
	char szCName[100];//�ֶ���������
	int  nFieldId;//�ֶ�ID��
	int  nIndex;//�ֶ�������
	char szFieldType[100];//�ֶ�����
	int  nSize;//�ֶδ�С
	int  nFlag;//�ֶεı�־
	int  nKeyGroup;//�ֶ���
	int  nShift;//�仯
	char szDefaultValue[100];//Ĭ��ֵ
}XmlField, *pXmlField; 

#else
#endif


