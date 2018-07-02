#include <string>
#ifdef UNIX_ENV
#include <dirent.h>
#include <string.h>
#endif
#include "CBrowseDir.h"
#ifndef UNIX_ENV
#pragma warning(disable:4996) 
#endif
CBrowseDir::CBrowseDir()
{
	//�õ�ǰĿ¼��ʼ��m_szInitDir
	//getcwd(m_szInitDir,MAX_PATH);
	memset(m_szInitDir,0,sizeof(m_szInitDir));

	//���Ŀ¼�����һ����ĸ����'\',����������һ��'\'
	int len=strlen(m_szInitDir);
	if (m_szInitDir[len-1] != '\\')
		strcat(m_szInitDir,"\\");
}

bool CBrowseDir::SetInitDir(const char *dir)
{
	//�Ȱ�dirת��Ϊ����·��
	/*if (_fullpath(m_szInitDir,dir,MAX_PATH) == NULL)
	return false;*/
	strcpy(m_szInitDir,dir);

	//�ж�Ŀ¼�Ƿ����
	//if (_chdir(m_szInitDir) != 0)
	//	return false;

	//���Ŀ¼�����һ����ĸ����'\',����������һ��'\'
	int len=strlen(m_szInitDir);

	
char strPath[10] = {0};
#ifdef UNIX_ENV
	if (m_szInitDir[len-1] != '/')
		strcat(m_szInitDir,"/");
#else
if (m_szInitDir[len-1] != '\\')
	strcat(m_szInitDir,"\\");
#endif
	return true;
}

bool CBrowseDir::BeginBrowse(const char *filespec)
{
	ProcessDir(m_szInitDir,NULL);
	return BrowseDir(m_szInitDir,filespec);
}
#ifdef WINDOWS_ENV
bool CBrowseDir::BrowseDir(const char *dir,const char *filespec)
{
	_chdir(dir);

	//���Ȳ���dir�з���Ҫ����ļ�
	long hFile;
	_finddata_t fileinfo;
	if ((hFile=_findfirst(filespec,&fileinfo)) != -1)
	{
		do
		{
			//����ǲ���Ŀ¼
			//�������,����д���
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				char filename[MAX_PATH];
				strcpy(filename,dir);
				strcat(filename,fileinfo.name);
			//	cout << filename << endl;
				if (!ProcessFile(filename))
					return false;
			}
		} while (_findnext(hFile,&fileinfo) == 0);
		_findclose(hFile);
	}
	//����dir�е���Ŀ¼
	//��Ϊ�ڴ���dir�е��ļ�ʱ���������ProcessFile�п��ܸı���
	//��ǰĿ¼����˻�Ҫ�������õ�ǰĿ¼Ϊdir��
	//ִ�й�_findfirst�󣬿���ϵͳ��¼���������Ϣ����˸ı�Ŀ¼
	//��_findnextû��Ӱ�졣
	_chdir(dir);
	if ((hFile=_findfirst("*.*",&fileinfo)) != -1)
	{
		do
		{
			//����ǲ���Ŀ¼
			//�����,�ټ���ǲ��� . �� .. 
			//�������,���е���
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name,".") != 0 && strcmp
					(fileinfo.name,"..") != 0)
				{
					char subdir[MAX_PATH];
					strcpy(subdir,dir);
					strcat(subdir,fileinfo.name);
					strcat(subdir,"\\");
					ProcessDir(subdir,dir);
					if (!BrowseDir(subdir,filespec))
						return false;
				}
			}
		} while (_findnext(hFile,&fileinfo) == 0);
		_findclose(hFile);
	}
	return true;
}
#else
bool CBrowseDir::BrowseDir(const char *dir,const char *filespec)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(dir)) == NULL) {
		fprintf(stderr,"cannot open directory: %s\n", dir);
		return false;
	}
	chdir(dir);
	while((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)) 
		{

			if(strcmp(".",entry->d_name) == 0 ||
				strcmp("..",entry->d_name) == 0)
				continue;
			
			BrowseDir(entry->d_name,filespec);
		}
		else 
		{
			//printf("%*s%s\n",depth,"",entry->d_name);
			if (!ProcessFile(entry->d_name))
				return false;
		}
	}
	chdir("..");
	closedir(dp);

	return true;
}
#endif

bool CBrowseDir::ProcessFile(const char *filename)
{
	return true;
}

void CBrowseDir::ProcessDir(const char 
	*currentdir,const char *parentdir)
{
}
//��д�麯��ProcessFile��ÿ����һ�Σ��ļ�������1
bool CDelFileInDir::ProcessFile(const char *filename)
{
	m_nFileCount++;
	//�ж��ļ����Ƿ����
	//�����ǰ�ļ���С�ڵ�ǰʱ��30����ɾ���ɵ���־�ļ�
	//"2016-04-23_agclogfile.log"
char strPath[10] = {0};
#ifdef UNIX_ENV
	strcpy(strPath,"/");
#else
	strcpy(strPath,"\\");
#endif
	string str=filename;
	int begin=str.find_last_of(strPath);
	string strFile=str.substr(begin+1);
	
	vector<string> daySet;
	split(strFile, daySet,"-");

	SYSTEMTIME sysTime;
	GetLocalTime(sysTime);
	if(daySet.size()>=3)
	{
		char *end;
		sysTime.wYear=static_cast<int>(strtol(daySet[0].c_str(),&end,10));
		sysTime.wMonth=static_cast<int>(strtol(daySet[1].c_str(),&end,10));
		sysTime.wDay=static_cast<int>(strtol(daySet[2].c_str(),&end,10));
	}
	int span=GetTimeSpan(sysTime);
	//�������45��������ǿ���ɾ�����ļ�
	if(span>45*3600*24)
	{
		//ɾ���ļ�
		DeleteFile(filename);
	}
	
	
	return CBrowseDir::ProcessFile(filename);
}

int CDelFileInDir::split(const string& str, vector<string>& ret_, string sep )
{
	if (str.empty())
	{
		return 0;
	}

	string tmp;
	string::size_type pos_begin = str.find_first_not_of(sep);
	string::size_type comma_pos = 0;

	while (pos_begin != string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		if (!tmp.empty())
		{
			ret_.push_back(tmp);
			tmp.clear();
		}
	}
	return 0;
}
