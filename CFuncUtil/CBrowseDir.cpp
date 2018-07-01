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
	//用当前目录初始化m_szInitDir
	//getcwd(m_szInitDir,MAX_PATH);
	memset(m_szInitDir,0,sizeof(m_szInitDir));

	//如果目录的最后一个字母不是'\',则在最后加上一个'\'
	int len=strlen(m_szInitDir);
	if (m_szInitDir[len-1] != '\\')
		strcat(m_szInitDir,"\\");
}

bool CBrowseDir::SetInitDir(const char *dir)
{
	//先把dir转换为绝对路径
	/*if (_fullpath(m_szInitDir,dir,MAX_PATH) == NULL)
	return false;*/
	strcpy(m_szInitDir,dir);

	//判断目录是否存在
	//if (_chdir(m_szInitDir) != 0)
	//	return false;

	//如果目录的最后一个字母不是'\',则在最后加上一个'\'
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

	//首先查找dir中符合要求的文件
	long hFile;
	_finddata_t fileinfo;
	if ((hFile=_findfirst(filespec,&fileinfo)) != -1)
	{
		do
		{
			//检查是不是目录
			//如果不是,则进行处理
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
	//查找dir中的子目录
	//因为在处理dir中的文件时，派生类的ProcessFile有可能改变了
	//当前目录，因此还要重新设置当前目录为dir。
	//执行过_findfirst后，可能系统记录下了相关信息，因此改变目录
	//对_findnext没有影响。
	_chdir(dir);
	if ((hFile=_findfirst("*.*",&fileinfo)) != -1)
	{
		do
		{
			//检查是不是目录
			//如果是,再检查是不是 . 或 .. 
			//如果不是,进行迭代
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
//覆写虚函数ProcessFile，每调用一次，文件个数加1
bool CDelFileInDir::ProcessFile(const char *filename)
{
	m_nFileCount++;
	//判断文件名是否存在
	//如果当前文件名小于当前时间30，就删除旧的日志文件
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
	//如果大于45天的数据是可以删除的文件
	if(span>45*3600*24)
	{
		//删除文件
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
