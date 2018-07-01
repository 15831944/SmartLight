//#include "stdafx.h"
#include "gSystemDef.h"
#include "Q_ProtocolLight.h"
#include "tinyxmlinclude.h"
#include "Q_HttpLink.h"
#include <curl/curl.h>
#include "json/json.h" 
#include <string>
#include <math.h>
#include <fstream>
#include <stdlib.h>
Q_ProtocolLight *g_Protocol=NULL;
static int i;


char res_buf[409600 + 1];
size_t copy_data(void *ptr, size_t size, size_t nmemb, void *stream)
{ 
    memcpy(res_buf + strlen(res_buf), ptr, size * nmemb);    
    printf("write size=%u,strlen=%d\n",size * nmemb,strlen(res_buf));

    return strlen(res_buf);
}

void writeJson(string strSQL)
{
	/*char * pFilePath = getenv( "CCSOSD" );
	char szIniFile[200];
	memset(szIniFile,0,sizeof(szIniFile));

	if( pFilePath == NULL )
		pFilePath = getcwd(szIniFile, MAX_PATH);
	strcpy(szIniFile, pFilePath);*/
	ofstream out("/home/work/ldns/bin/com.txt",ios::app);
	out<<strSQL;
	out.close();

}
#ifdef UNIX_ENV
#ifdef __cplusplus
extern "C" {
#endif
#endif //OS_UNIX
	//添加命令
// 遥调、遥控
int OUTPUTAPI add_command(Q_taskBase *task,LPQ_RECORDDATATAG rec)
{
	 //int ret=0;
	return 0;
	// return ret;
}

//打印更新记录
int  OUTPUTAPI print_change_log()
{
	printf("=== CommModbusStd Protocol Dynamic lib====\n");
	printf("=== 2016-07-22====\n");
	printf("1、增加打印输入版本功能\n");
	
	return 1;
}
//读配置文件
Q_chDownReturn* OUTPUTAPI create_protocol_object(Q_chDownCommPara *v_param)
{
	printf("create_protocol_object CommLightStd\n");
	//int ret;
	Q_chDownReturn *chRet=new Q_chDownReturn();

	int ctype = v_param->cfg->type;

	int  n_sleepMills=500;//原来是 1000
	//=======
	Q_taskBase* taskTest = new Q_taskBase();
	taskTest->setBackupMode(eCommonBackupCold);
	taskTest->setRtdbInf(v_param->m_pMgr);//2015-06-23

	bool flgPri=true;
	printf("taskTest success");
	
	Q_ProtocolLightSetting *modbusSetting=new Q_ProtocolLightSetting();
	modbusSetting->setFilePath(v_param->strFilePath);

	modbusSetting->initSetting();//解析xml内容
	
	modbusSetting->setChannelType(v_param->cfg->type);//设置通道类型

	Q_commonBuffer* bufferReceive = new Q_commonBuffer();
	Q_commonBuffer* bufferSend = new Q_commonBuffer();

	Q_channelBase* channelTest = new Q_channelBase();
	channelTest->setChannelID(v_param->cfg->id);

	channelTest->setPrimaryFlag(flgPri);
	channelTest->setReceiveBuffer(bufferReceive);
	channelTest->setSendBuffer(bufferSend);

	Q_ModbusMasterCommandManager* pCommandList = new Q_ModbusMasterCommandManager();
	channelTest->setCommandList(pCommandList);

	Q_HttpLink* linkTcp = new Q_HttpLink();
	Q_HttpLinkSetting* tLinkSetting = new Q_HttpLinkSetting();

	TRACE("User Q_HttpLink \r\n");
//	char *tmp=NULL;
	//tmp=modbusSetting->getContentID(2,1);


	Q_linkConfig* tLinkConfig = new Q_linkConfig();
	tLinkConfig->setLinkSetting(tLinkSetting);
	tLinkConfig->setReceiveBuffer(bufferReceive);
	tLinkConfig->setSendBuffer(bufferSend);

	channelTest->setLinkSetting(tLinkSetting);
	channelTest->setLinkConfig(tLinkConfig);
	channelTest->setLink(linkTcp);
	channelTest->setProtocolSetting(modbusSetting);


	Q_protocolConfig* protocolConfig = new Q_protocolConfig();
	protocolConfig->setProtocolSetting(modbusSetting);
	protocolConfig->setReceiveBuffer(bufferReceive);
	protocolConfig->setSendBuffer(bufferSend);
	channelTest->setProtocolConfig(protocolConfig);

	protocolConfig->setProtocolCommandManager(pCommandList);

	Q_ProtocolLight* protocol = new Q_ProtocolLight();
	protocol->setRtdbInf(v_param->m_pMgr);
	
	g_Protocol=protocol;
	//n_sleepMills=modbusSetting->getSleepPerTimes();
	n_sleepMills=30000;
	protocol->setSendMsg(0);

	modbusSetting->setChannelID(v_param->cfg->id);//设置通道id

	channelTest->setProtocol(protocol);	

	//m_GSettting.addChannel(channelTest);

	taskTest->addChannel(channelTest);

	Q_simpleThread* threadTest = new Q_simpleThread();
	threadTest->setThreadPeriod(n_sleepMills);
	threadTest->addTask(taskTest);


	Q_taskRunningUnit* unit = new Q_taskRunningUnit();
	unit->setDriverMode(eCommonModeSingle);
	unit->addTask(taskTest);
	unit->addThread(threadTest);

	
	chRet->unit=unit;
	chRet->task=taskTest;
	sprintf_s(chRet->protocol,sizeof(v_param->cfg->protocol),"%s",v_param->cfg->protocol);
	chRet->channelID=v_param->cfg->id;
	chRet->enabled=v_param->cfg->enabled;
	printf("successsssssssssss\n");
	if(chRet)
		return chRet;	
	else
		return NULL;
}
//为了保证开辟的动态库与释放库的一致，增加此方法
void OUTPUTAPI free_protocol_object(Q_chDownReturn* pProtocol)
{
	if ((Q_chDownReturn*)pProtocol)
	{
		delete (Q_chDownReturn*)pProtocol;
		pProtocol = NULL;
	}
}
#ifdef UNIX_ENV
#ifdef __cplusplus
}
#endif
#endif //OS_UNIX
	int Q_ProtocolLight::hex_char_value(char c)   
	{   
		if(c >= '0' && c <= '9')   
			return c - '0';   
		else if(c >= 'a' && c <= 'f')   
			return (c - 'a' + 10);   
		else if(c >= 'A' && c <= 'F')   
			return (c - 'A' + 10);   
	//	assert(0);   
		return 0;   
	}   
	int Q_ProtocolLight::hex_to_decimal(const char* szHex, int len)   //实现十六进制转十进制
	{   
		int result = 0;   
		for(int i = 0; i < len; i++)   
		{   
			result += (int)pow((float)16, (int)len-i-1) * hex_char_value(szHex[i]);   
		}   
		return result;   
	}  
//灯的数据返回
size_t Q_ProtocolLight::funcLight(void *strptr, size_t size, size_t nmemb, void *stream)
{
	printf("import funcLight begin1/n");
	
	copy_data(strptr,size,nmemb,stream);
	/*return 0 ;
		int nsize = size * nmemb;
		std::string *strdata = (std::string*) stream;
		if (strdata)
			{
				strdata->append( (const char*)strptr, nsize ); 
			}

				
		printf("import funcLight begin2");
		return nsize;*/
	//char * s_value=(char*)malloc(9999*sizeof(char));
	//memcpy(strptr,s_value,strlen(s_value));

	struct WriteThis *wt = (struct WriteThis *)stream;
	printf("init start..\n");
  size_t buffer_size = size*nmemb;
  printf("size===%u\n",buffer_size);
  printf("strlen====%d\n",strlen((char *)strptr));
  
	//Json::Reader reader;
	//Json::Value root;

	/*std::string strSQL;
	
	strSQL=(char *)strptr;
	cout<<"strSQL====="<<strSQL<<endl;
		writeJson(strSQL);*/
	//if(strSQL.empty()) printf("data is NULL\n");
	//std::string s_last=strSQL;
	// s_last=s_last.substr(s_last.length()-1);
	// cout<<s_last<<endl;
	////printf("thr last char is%s\n",s_last);
	//if (reader.parse(strSQL, root))//将json字符串解析到root,root包含json所有子元素
	//{
	//    printf("start parse Json\n");
	//	Q_dataManager *m_pMgr=NULL;
	//	Q_ProtocolLightSetting *m_Setting=NULL;
	//	if(g_Protocol!=NULL)
	//	{

	//		m_pMgr=g_Protocol->getRtdbInf();
	//		m_Setting=g_Protocol->getSetting();
	//		printf("get protocol ok\n");
	//	}
	//	if(m_pMgr==NULL||m_Setting==NULL)
	//	{
	//		return 0;
	//	}
	//	Q_RECORDDATATAG dataArray;//存数据
	//	dataArray.eDataType=eDataTypeFloat;
	//	//addr为dpu内部编号
	//	printf("status'size====%d\n",root["status"].size());
	//	for (unsigned int i = 0; i<root["status"].size(); i++)
	//	{
	//			printf("start parse status\n");
	//			int addr = root["status"][i]["addr"].asInt();
	//			double v_status,v_brightness;
	//			printf("addr=====%d\n",addr);
	//			//printf("status====%d\n",root["status"][i]["v"][1].asInt());
	//			/*int v[4];
	//			memcpy(v,&root["status"][i]["v"],16);*/
	//			int j=0;
	//			v_status=root["status"][i]["v"][j].asDouble();//v[0]直接编译错误,未解决
	//			v_brightness=root["status"][i]["v"][1].asDouble();
	//			if(m_Setting!=NULL)
	//			{
	//					dataArray.channelID=m_Setting->getChannelID();//父类方法
	//					printf("this  channelID is %d",dataArray.channelID);
	//			}
	//			//目前的通道id为5，dpu有两大种，状态和亮度，每种按楼层不同分为5种
	//		   printf("no===%d,  status====%d,  bright===%d\n",addr,v_status,v_brightness);
	//		   //现在dpu需要通过getsatusdpu，或getbrightdpu
	//			dataArray.device=atoi(m_Setting->getStatus_DPU(i));
	//			dataArray.no=addr;
	//			dataArray.fValue=(float)v_status;
	//			if(m_pMgr!=NULL)
	//			m_pMgr->recordDataValue(&dataArray,1);//记录状态
	//			printf(" this message has been recorded\n");
	//			dataArray.device=atoi(m_Setting->getBright_DPU(i));
	//			dataArray.no=addr;
	//			dataArray.fValue=(float)v_brightness;
	//			if(m_pMgr!=NULL)
	//			m_pMgr->recordDataValue(&dataArray,1);//记录亮度

	//	}

	//}
	//	printf("import funcLight end");
	//	
	//	

	return 0;
}
//灯的数据
bool Q_ProtocolLight::handleFrameLight()
{
	printf("start handleFrameLight......\n");
	
		for( i=1;i<=5;i++)//5次循环，5次回调
		{
		  CURL *curl;
		  CURLcode res;

		  curl = curl_easy_init();
		  if(curl) {
			/* First set the URL that is about to receive our POST. This URL can
			   just as well be a https:// URL if that is what should receive the
			   data. */
			  if(m_pCookies!=NULL)
				{
					printf("handleFrameLight cookies=%s\n",m_pCookies->data);
					printf("cookies's length====%d\n",strlen(m_pCookies->data));
				}
				else
				{
					printf("cookies return is null\n");
				}
			//对cookies进行截取
			/*从字符串的右边截取n个字符*/
			
			//char * strSession;
			char Sessionid[200];

			memset(Sessionid,0,sizeof(Sessionid));
			std::string s_session=m_pCookies->data;
			s_session=s_session.substr(s_session.length()-32);//sessionid共32位
			//strSession=(char *)s_session.c_str();
			printf("Session's-value value======%s\n",s_session.c_str());//打印值
			sprintf(Sessionid,"JSESSIONID=%s\n",s_session.c_str());
			printf("Session's value======%s",Sessionid);//打印拼接之后的结果
			char strURL[200];
			memset(strURL,0,sizeof(strURL));
			char strGid[200];
			memset(strGid,0,sizeof(strGid));
			/*char *url="http://192.168.200.44:8080/e3facility/remote/getStatus";*/
			/*curl_easy_setopt(curl,CURLOPT_URL,url);*/
			/*sprintf(strGid,"gid=%s",m_PrintSetting->getGid(i));*/
			/*curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strGid);
			curl_easy_setopt(curl, CURLOPT_POST,1);   
			 
			curl_easy_setopt(curl, CURLOPT_HEADER,1);*/
			sprintf(strGid,"http://%s/e3facility/remote/getStatus?gid=%s",m_PrintSetting->getUrl(),m_PrintSetting->getGid(i));
			printf("%s\n",strGid);
			printf("set url ok\n");  
			curl_easy_setopt(curl,CURLOPT_URL,strGid);
			//curl_easy_setopt(curl, CURLOPT_VERBOSE,1);  
			curl_easy_setopt(curl,CURLOPT_COOKIE,Sessionid);//CURLOPT_COOKIE,设置cookie值
			//curl_easy_setopt(curl,CURLOPT_BUFFERSIZE,819200);//CURLOPT_COOKIE,设置cookie值
			

			//curl_easy_setopt(curl,CURLOPT_POST,1);
			// 设置接收数据的处理函数和存放变量
			//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, funcLight);

			 memset(res_buf,0,sizeof(res_buf));
			 curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,copy_data);
             curl_easy_setopt(curl,CURLOPT_WRITEDATA,res_buf);

			/* Perform the request, res will get the return code */
			printf("import curl_easy_perform begin \n");
			GetLocalTime(m_tSendTime);
			m_bSendFlag=true;
			res = curl_easy_perform(curl);
			std::string strSQL;
			strSQL.append(res_buf);
			cout<<"in handle's value======"<<strSQL<<endl;
			printf("curl_easy_perform ok \n");
			funcLight2();
			/* always cleanup */
			curl_easy_cleanup(curl);
			printf("cleanup curl ok\n");

			
		}
		
  }
  return true;
}
int  mod_init(int dbg_level)
{
	//g_debug_level = dbg_level;

	return 0;
}	
char *Q_ProtocolLightSetting::getContentID(const UINT &v_idx,const UINT &v_grp)
{
	for (UINT i=0;i<m_vContent.size();i++)
	{
		if (m_vContent[i].group==v_grp &&
			m_vContent[i].id==v_idx)
		{
			return m_vContent[i].value;
		}
	}
	cout<<"nullll"<<endl;
	return NULL;
}
//读取URL与设备地址
char *Q_ProtocolLightSetting::getUrl()
{
	//const int c=i;
	return getContentID(1,1);
}
char *Q_ProtocolLightSetting::getGid(int i)
{
	const int c=i;
	return getContentID(2,c);
}
char *Q_ProtocolLightSetting::getStatus_DPU(int i)
{
	const int c=i;
	return getContentID(5,c);
}
char *Q_ProtocolLightSetting::getBright_DPU(int i)
{
	const int c=i;
	return getContentID(6,c);
}

Q_ProtocolLight::Q_ProtocolLight()
{
	m_cpuEndian=eBigEndian;//默认是大端模式
	m_LastCmd=NULL;
	m_nMsgID=0;
	m_PrintSetting=NULL;
	//m_wnd=NULL;
	memset(m_LastMsg,0,sizeof(m_LastMsg));
	memset(m_LastMsg,0,sizeof(m_LastMsg));

	m_firstRun=true;

	printf("Q_ProtocolLight construture \n");
	GetLocalTime(m_tSendTime);
	m_bSendFlag=false;

}
Q_ProtocolLight::~Q_ProtocolLight()
{

}
bool Q_ProtocolLight::initializeProtocol()
{
		printf("Q_ProtocolLight initializeProtocol \n");

	return true;
}
bool Q_ProtocolLight::terminateProtocol()
{
	


	return true;

}
//登陆
bool Q_ProtocolLight::startProcess()
{
		printf("Q_ProtocolLight startProcess \n");
		m_eProtocolStatus = eCommonStatusNormal;
		m_PrintSetting= (Q_ProtocolLightSetting *)m_pProcotolConfig->getProtocolSetting();
		if(m_PrintSetting!=NULL)
		{
			//新规约打印
			sprintf_s(m_LastMsg,sizeof(m_LastMsg),"初使化开始...");
			SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,(BYTE*)m_LastMsg,strlen(m_LastMsg)+1);

		}

			printf("curl_easy_init begin\n");
	CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
	printf("curl_easy_init end\n");
	if(curl) 
	{
		printf("init curl\n");
		char strURL[200];
		memset(strURL,0,sizeof(strURL));
		sprintf(strURL,"http://%s/e3facility/login2?user=thadmin&password=123456",m_PrintSetting->getUrl());//用来把URL的数据写入strURL中

		 printf("exec get url= %s\n",strURL);
		curl_easy_setopt(curl, CURLOPT_URL, strURL);
		 curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ""); /* start cookie engine */
		 printf("exec curl_easy_perform\n");
			res = curl_easy_perform(curl);		
			printf("exec curl_easy_perform end\n");
	
			curl_easy_setopt(curl, CURLOPT_COOKIESESSION, 1L);//只获取session
				 printf("exec curl_easy_getinfo\n");
				 res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &m_pCookies);
				 printf("exec curl_easy_getinfo_end\n");
			  if(res != CURLE_OK) 
			  {
				printf( "Curl curl_easy_getinfo failed: %s\n",
						curl_easy_strerror(res));

			  }
  
			
 // curl_easy_setopt(curl, CURLOPT_COOKIELIST, &m_pCookies);//将获取到的cookies放到这里
	
  //curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
		//curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");
		
		if(m_pCookies!=NULL)
		{
		printf("cookies=%s\n",m_pCookies->data);
		}
		else
		{
			printf("cookies return is null\n");
		}
		curl_easy_cleanup(curl);

	}


		printf("Q_protocolModbusMaster::startProcess() eee\n");

	return true;
}
bool Q_ProtocolLight::stopProcess()//logout
{
	CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
	if(curl) 
	{
		char strURL[200];
		memset(strURL,0,sizeof(strURL));//初始化strURL数组，memset为内存赋值函数，用来给某一块内存空间进行赋值的
		sprintf(strURL,"http://%s/e3facility/logout2",m_PrintSetting->getUrl());//用来把URL的数据写入strURL中
		curl_easy_setopt(curl, CURLOPT_URL,strURL);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

	}
	return true;
}
bool Q_ProtocolLight::explainData()
{
	printf("Q_ProtocolLight::explainData 1\n");
     if(m_bSendFlag)
	 {
		
		 int n=GetTimeSpan(m_tSendTime);
		  printf("Q_ProtocolLight::explainData 2,interval=%d\n",n);
		if( n>3)
		{
			//funcLight2();
		}
	 }
	

	return true;
}
void Q_ProtocolLight::SendMsgPrint(int nchannelid,int nmsgtype,BYTE* mse,int nlen)
{
		printf("Q_ProtocolLight SendMsgPrint begin \n");
		printf("Q_ProtocolLight SendMsgPrint return  \n");
		return ;
	//TRACE("Q_protocolIEC104Master::SendMsgPrint,g_rtdbInfIEC104=%08x",m_pMgr->getRtdbInf());
			
	//g_rtdbInfIEC104 = m_pMgr->getRtdbInf();
	//CFEPFuncRtdb *m_rtdbInf = g_rtdbInfIEC104;

	CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
	//如果不判断运状态就发命令，会把消息总线给堆满了

	bool bSend = false;

	SCADA_TUnit ElementTUnit;
	m_rtdbInf->getScadaDb()->GetElementByID(SCADA_TAB_TUnit_ID,nchannelid,&ElementTUnit);

	if (ElementTUnit.iRecordFlag != 0)
	{
		bSend = true;
	}
	else
	{
		bSend = false;
		return;
	}

	if (nmsgtype == eMsgTypeTx || nmsgtype == eMsgTypeRx)//发送消息、接收消息
	{
		if (ElementTUnit.iRecPacketFlg != 0)//报文信息传输开启
		{
			bSend = true;
		}
		else
		{
			bSend = false;
			return;
		}
	}
	else if (nmsgtype == eMsgTypeMsg || nmsgtype == eMsgTypeError || nmsgtype == eMsgTypeCh)//提示信息、信息值、错误消息、数据解释、规约解析
	{
		if (ElementTUnit.iRecPromptFlg != 0)//提示信息传输开启
		{
			bSend = true;
		}
		else
		{
			bSend = false;
			return;
		}
	}

	if (nlen>=sizeof(m_LastMsg))
	{
		nlen = sizeof(m_LastMsg);
	}

	if (bSend == true)
	{
		SYSTEM_Process *pProcess= m_rtdbInf->getMainDb()->FindProcessByProType(pt_DBGSVR);
		if(pProcess==NULL)
		{
			TRACE("process DBGSVR not exist in did sendMsgPrint exit\n");
			return ;
		}
		else
		{
			if((pProcess->iFlag&ps_Run)!=ps_Run)
			{
				//进程不运行，退出
				TRACE("process DBGSVR not run sendMsgPrint exit\n");
				return ;
			}
		}

		if (nlen>Q_MSGTRAN_MAXMSGLEN)
		{
			int sendcs = 0;
			sendcs = nlen/Q_MSGTRAN_MAXMSGLEN;
			int sendys = nlen%Q_MSGTRAN_MAXMSGLEN;

			for (int n=0; n<=sendcs; n++)
			{	
				MesMsgPrint MsgPrint = {0};
				MsgPrint.ichannelid=nchannelid;
				nmsgtype = nmsgtype|0x0080;
				MsgPrint.msgtype=nmsgtype;

				if (n == sendcs)
				{
					MsgPrint.nlen = sendys;
					memcpy(MsgPrint.msg,mse+n*Q_MSGTRAN_MAXMSGLEN,sendys);
				}
				else
				{
					MsgPrint.nlen = Q_MSGTRAN_MAXMSGLEN;
					memcpy(MsgPrint.msg,mse+n*Q_MSGTRAN_MAXMSGLEN,Q_MSGTRAN_MAXMSGLEN);
				}

				//m_rtdbInf->getMainDb()->SendMessage(pt_DBGSVR,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
				

				m_rtdbInf->getMainDb()->SendMessageTo(pProcess->ID32,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
			}
		}
		else
		{		
			MesMsgPrint MsgPrint = {0};
			MsgPrint.ichannelid=nchannelid;
			MsgPrint.msgtype=nmsgtype;
			MsgPrint.nlen = nlen;
			memcpy(MsgPrint.msg,mse,nlen);
			//m_rtdbInf->getMainDb()->SendMessage(pt_DBGSVR,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
			//2016-10-28 tjs 因为不应该给备都发送消息
			

			m_rtdbInf->getMainDb()->SendMessageTo(pProcess->ID32,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
			/*char strMsg[200] = {0};
			sprintf_s(strMsg,sizeof(strMsg),"MsgPrint:stid = %d msgtype id = %d message = %s",MsgPrint.ichannelid,MsgPrint.msgtype,MsgPrint.msg);
			TRACE("strMsg");*/
		}
	}
		printf("Q_ProtocolLight SendMsgPrint end \n");
}
bool Q_ProtocolLight::explainDataEx(char* buff,int nCount,int& nCurrentPos)
{
	
	return true;
}
Q_ProtocolLightSetting* Q_ProtocolLight::getSetting()
{
	return m_PrintSetting;
}


//----------------------------------------------------------------------------
//*
//*
//* Q_protocolModbusTcpServerStandardCommand class
//*
//*
Q_ProtocolLightCommand::Q_ProtocolLightCommand(void)
{
	
}

Q_ProtocolLightCommand::~Q_ProtocolLightCommand(void)
{

}





//----------------------------------------------------------------------------
//*
//*
//* Q_protocolModbusTcpServerStandardCommandManager class
//*
//*
Q_ModbusMasterCommandManager::Q_ModbusMasterCommandManager(void)
{
}

Q_ModbusMasterCommandManager::~Q_ModbusMasterCommandManager(void)
{
}
//----------------------------------------------------------------------------
//*
//*
//* Q_protocolModbusTcpServerStandardSetting class
//*
//*
Q_ProtocolLightSetting::Q_ProtocolLightSetting(void)
{
	

}

Q_ProtocolLightSetting::~Q_ProtocolLightSetting(void)
{
	m_vGroup.clear();
	m_vContent.clear();
}

void Q_ProtocolLightSetting::setFilePath(char *v_path)
{
	strcpy(m_sFilePath,v_path);
}
bool Q_ProtocolLightSetting::initSetting()
{
	return init();
}

bool Q_ProtocolLightSetting::init(void)
{
	TiXmlDocument doc(m_sFilePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
#ifdef _DEBUG
		sprintf(LastError, "解析配置文件出错，当前文件:%s，当前行号:%d\n",__FILE__,__LINE__);
		TRACE("解析配置文件出错，当前文件:%s，当前行号:%d\n",__FILE__,__LINE__);
		printf(LastError);
#endif
		return false;
	}

	TiXmlNode* node = 0;
	TiXmlNode* root = 0;
	TiXmlNode* nodeItem = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	root = doc.FirstChild( "Setting" );
	todoElement = root->ToElement();

	assert( todoElement  );

	for ( node = todoElement->FirstChild( "Record" );
		node;
		node = node->NextSibling( "Record" ) )
	{
		itemElement=node->ToElement();

		UINT nID=atoi(itemElement->Attribute("id"));
		
		
			ST_ModCfgT group;

			group.id=nID;
			sprintf_s(group.name,sizeof(group.name),"%s",itemElement->Attribute("name"));
			group.type=eModXmlDataTypeString;

			m_vGroup.push_back(group);

			for (nodeItem=node->FirstChild("Field");nodeItem;
				nodeItem=nodeItem->NextSibling("Field"))
			{
				itemElement=nodeItem->ToElement();
				ST_ModCfgT content;
				content.group=group.id;

				content.id=atoi(itemElement->Attribute("id"));
				sprintf_s(content.name,sizeof(content.name),"%s",itemElement->Attribute("name"));
				sprintf_s(content.value,sizeof(content.value),"%s",itemElement->Attribute("value"));

				const char *type=itemElement->Attribute("type");
				if (strcmp(type,"STRING")==0)
				{
					content.type=eModXmlDataTypeString;

				}
				else if (strcmp(type,"ENUM")==0)
				{
					content.type=eModXmlDataTypeEnum;
				}
				else if (strcmp(type,"INT")==0)
				{
					content.type=eModXmlDataTypeInt;
				}
				else if (strcmp(type,"FLOAT")==0)
				{
					content.type=eModXmlDataTypeFloat;
				}
				m_vContent.push_back(content);

			}
		

	}
	// 	if (m_pnt_cmd_count==0 && m_alg_cmd_count==0)
	// 	{
	// 		CString strErr;
	// 		strErr.Format("modbusTCP规约中没有召唤遥测与遥信配置，文件名%s",m_sFilePath);
	// 		MessageBox(AfxGetMainWnd()->m_hWnd,strErr,"错误",MB_ICONERROR|MB_OK);
	// 		exit(0);
	// 	}
	return true;
}
bool Q_ProtocolLightSetting::reloadLog()
{
	TiXmlDocument doc(m_sFilePath);  
	doc.LoadFile();  
	if(doc.ErrorId() > 0)  
	{
#ifdef _DEBUG
		sprintf_s(LastError,sizeof(LastError),"解析配置文件出错,当前行号:%d\n",__LINE__);
		printf(LastError);
#endif
		return false;
	}

	TiXmlNode* node = 0;
	TiXmlNode* root = 0;
	TiXmlNode* nodeItem = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	root = doc.FirstChild( "Setting" );
	todoElement = root->ToElement();

	assert( todoElement  );

	for ( node = todoElement->FirstChild( "Record" );
		node;
		node = node->NextSibling( "Record" ) )
	{
		itemElement=node->ToElement();


		UINT groupid=atoi(itemElement->Attribute("id"));
		if(groupid==10)
		{

			for (nodeItem=node->FirstChild("Field");nodeItem;
				nodeItem=nodeItem->NextSibling("Field"))
			{
				itemElement=nodeItem->ToElement();


				UINT id=atoi(itemElement->Attribute("id"));

				for (UINT i=0;i<m_vContent.size();i++)
				{
					if (m_vContent[i].group==groupid &&
						m_vContent[i].id==id)
					{
						sprintf_s(m_vContent[i].value,sizeof(m_vContent[i].value),"%s",itemElement->Attribute("value"));
					}
				}


			}
		}
	}

	return true;

}

bool Q_ProtocolLight::makeFrame()
{
  time_t cur;
  if(!m_firstRun)
  {
	  //控制30秒读一次
	  time(&cur);
	  if(cur<m_now+30)
	  {
		  return true;
	  }
	
  }
  else
  {
	  m_firstRun=false;
  }
  handleFrameLight();
 
  

  	time(&m_now);

	return true;
}

size_t Q_ProtocolLight::funcLight2()
{
	printf("import funcLight begin1\n");
	



	
	Json::Reader reader;
	Json::Value root;

	std::string strSQL;
	strSQL.append(res_buf);
	cout<<strSQL<<endl;
	if (reader.parse(strSQL, root))//将json字符串解析到root,root包含json所有子元素
	{
		m_bSendFlag=false;
	    printf("start parse Json\n");
		Q_dataManager *m_pMgr=NULL;
		Q_ProtocolLightSetting *m_Setting=NULL;
		if(g_Protocol!=NULL)
		{

			m_pMgr=g_Protocol->getRtdbInf();
			m_Setting=g_Protocol->getSetting();
			printf("get protocol ok\n");
		}
		if(m_pMgr==NULL||m_Setting==NULL)
		{
			return 0;
		}
		Q_RECORDDATATAG dataArray;//存数据
		dataArray.eDataType=eDataTypeFloat;
		//addr为dpu内部编号
		printf("status'size====%d\n",root["status"].size());
		for (unsigned int i = 0; i<root["status"].size(); i++)
		{
				printf("start parse status\n");
				int addr = root["status"][i]["addr"].asInt();
				double v_status,v_brightness;
				printf("addr=====%d\n",addr);
				
				int j=0;
				v_status=root["status"][i]["v"][j].asDouble();//v[0]直接编译错误,未解决
				v_brightness=root["status"][i]["v"][1].asDouble();
				if(m_Setting!=NULL)
				{
						dataArray.channelID=m_Setting->getChannelID();//父类方法
						printf("this  channelID is %d",dataArray.channelID);
				}
				//目前的通道id为5，dpu有两种，状态和亮度，每种按楼层不同分为5种
			   printf("no===%d,  status====%f,  bright===%f\n",addr,v_status,v_brightness);
			   //现在dpu需要通过getsatusdpu，或getbrightdpu
			   int s_device,b_device;
			   if(i==1) 
				 {
					 s_device=101;
					 b_device=102;
			   }
			   else if(i==2)
			   {
				     s_device=103;
					 b_device=104;
			   }
			   else if(i==3)
			   {
				   s_device=105;
					 b_device=106;
			   }
			   else if(i==4)
			   {
				   s_device=107;
					 b_device=108;
			   }
			   else if(i==5)
			   {
				   s_device=109;
					 b_device=110;
			   }

			   dataArray.device=s_device;				
				dataArray.no=addr;
				dataArray.fValue=(float)v_status;
				if(dataArray.fValue)
				cout<<"dpu==="<<dataArray.device<<"addr==="<<dataArray.no<<"svalue==="<<dataArray.fValue<<endl;
				//cout<<"value==="<<dataArray.fValue<<endl;
				if(m_pMgr!=NULL)
				m_pMgr->recordDataValue(&dataArray,1);//记录状态
				printf(" this message has been recorded\n");

				dataArray.device=b_device;	
				dataArray.no=addr;
				dataArray.fValue=(float)v_brightness;
				if(dataArray.fValue)
				cout<<"dpu==="<<dataArray.device<<"addr==="<<dataArray.no<<"bvalue==="<<dataArray.fValue<<endl;
				if(m_pMgr!=NULL)
				m_pMgr->recordDataValue(&dataArray,1);//记录亮度

		}
		memset(res_buf,0,sizeof(res_buf));

	}
		printf("import funcLight end");
		
		

	return 0;
}