#include "CRunManager.h"
CRunManager::CRunManager(void)
{
	m_pMgr=NULL;
	getConfigPath();
	m_RunMethod=0;
	m_log=new Log();
	m_db.setLog(m_log);//临时打日志方案
	m_GSettting.setLog(m_log);
}
CRunManager::~CRunManager(void)
{
  delete m_pMgr;
  delete m_log;
}

char *CRunManager::getConfigPath()
{
	char * pFilePath = getenv( "CCSOSD" );

	memset(szIniFile,0,sizeof(szIniFile));

	if( pFilePath == NULL )
		pFilePath = getcwd(szIniFile, MAX_PATH);
	strcpy(szIniFile, pFilePath);
	//strcat(szIniFile, "/config/");


	//tjs 2016-11-19  支持当前工程名可变，方便研发调试
	//原代码
	//strcat(szIniFile, "/project/support/Channel/");
	char szSysConfigIni[MAX_PATH];
	GetSysConfigFile(szSysConfigIni);

	char szProjectFile[MAX_PATH];
	memset(szProjectFile,0,sizeof(szProjectFile));
#ifdef UNIX_ENV
	if((GetPrivateProfileString2("RTDB","当前工程","support",szProjectFile,MAX_PATH,szSysConfigIni) > 0))
	{
		if(strlen(szProjectFile)>0)
		   sprintf(szIniFile, "%s/project/%s/Channel/",szIniFile,szProjectFile);
		else
			strcat(szIniFile, "/project/support/Channel/");
		
	}
	else
	{
		strcat(szIniFile, "/project/support/Channel/");
	}
#else
	if((GetPrivateProfileString2("RTDB","当前工程","support",szProjectFile,MAX_PATH,szSysConfigIni) > 0))
	{
		if(strlen(szProjectFile)>0)
		   sprintf(szIniFile, "%s\\project\\%s\\Channel\\",szIniFile,szProjectFile);
		else
			strcat(szIniFile, "\\project\\support\\Channel\\");
		
	}
	else
	{
		strcat(szIniFile, "\\project\\support\\Channel\\");
	}
#endif
	//2016-11-19

	return szIniFile;
}

int CRunManager::init()
{
	m_eCalcThreadStatus=eThreadInitialize;
	TRACE("CRunManager::init()\n");
	int ret=0;
	if(m_pMgr==NULL)
		m_pMgr=_GlobalData.getDataManager();

	ret=m_pMgr->initRtdb();//新版实时库
	
	if(0==ret)
	{ 
	   ret=initBase();
	}
	else
	{
		printf("init rtdb failed\n");
	}
	
	int n=m_GSettting.m_vChannel.size();
	m_pMgr->initChnInfo(n);

	n=m_GSettting.m_vNodeInfo.size();
	m_pMgr->initNodeInfo(n);

	if(0==ret)
	{
	  initComm();
	}
	else
	{
		printf("init base failed\n");
	}
	
	return ret;
}
//初使化基本表
int CRunManager::initBase()
{
	int ret=0;

	m_GSettting.setDataMgr(m_pMgr);
	m_GSettting.setXmlPath(szIniFile);
	try
	{
		//删除了DATAINDEX，在initRealDatabase中统计dataindex
		if(m_GSettting.init()!=true)
		{
			ret=-1;
			printf("m_GSettting::init base failed\n");
		}
	}
	catch (...)
	{
		//初使化失
		printf("init base failed,exception occur\n");
	}



	return ret;
}

bool CRunManager::initComm(void)
{
	for(vector<ST_GeCtrlChl>::iterator it=m_GSettting.m_vChannel.begin();it!=m_GSettting.m_vChannel.end();it++)
	{
		ST_GeCtrlChl ch=(*it);
		if(ch.enabled==true)
		{
			//当配置文件为空的时候，做异常处理  tjs 2014-8-25
			if(strlen(ch.cfgfile)>0&&
				strcmp(ch.protocol,"NULL")!=0&&strcmp(ch.protocol,"null")!=0&&
				strlen(ch.protocol)>0)
			{
				printf("initComm base Protocol Channel9d = %d\n",ch.id);

				switch(ch.type)
				{
				case C_CH_TYPE_SCADA:
					createScadaProtocol(&ch);
					break;
				case C_CH_TYPE_UP:
					//2016-96-22
					if(ch.protocolSubType>0)
					{
						//规约子类型不需初使化通道
					//	return NULL;
						continue;
					}
					createScadaProtocol(&ch);
					break;
				case C_CH_TYPE_ZT:
					createZtProtocol(&ch);
					break;
				case C_CH_TYPE_OTHER:
					//if (m_pMgr != NULL)
					//{
						//printf("initComm XNChannel9d = %d\n",ch.id);
						//m_pMgr->UpdatXNDataStatus();
					//}
					break;
				}
			}
		}
	}

	m_taskManager.initializeManager();

	return true;
}
bool CRunManager::startComm(void)
{
	if (m_pMgr != NULL)
	{
		m_pMgr->CleanChannelStatus();
		m_pMgr->CleanQualityStatus();
		m_pMgr->UpdatXNDataStatus();
		m_pMgr->UpdatSlaveQualityStatus();
	}

	m_taskManager.startWork();

	m_eCalcThreadStatus = eThreadRun;
	bool bRet2 = CreateThreadEx(UpdataProcessPara,this);

	return true;
}

bool CRunManager::stopComm(void)
{
	if (m_pMgr != NULL)
	{
		CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
		if (m_rtdbInf != NULL)
		{
			m_rtdbInf->rtdb_logout();
		}
	}
	m_eCalcThreadStatus = eThreadQuit;

	m_taskManager.stopWork();
	m_taskManager.terminateManager();//樊硕添加，停止时，调用terminateLink()，停止设备

	vector<TaskCfgList>::iterator it;
	
	for(it=m_taskList.begin();it!=m_taskList.end();it++)
	{
         freeScadaProtocol(it->data);
	}
	//取消注册
	m_pMgr->UnRegisterDb();

	return true;
}

//引用动态库
CLibLoader* CRunManager::refProtocolLib(string& libname)
{
	CLibLoader* loader;
	
	Q_commonUtility::outString(m_log->getFile(),"lookup begin ,lib name=%s\n",libname.c_str());

	if(!m_protocol_lib.Lookup(libname, loader))
	{
		loader = new CLibLoader(libname);
		loader->setLog(m_log);//临时打日志方案
		m_protocol_lib.SetAt(libname, loader);
	}

	Q_commonUtility::outString(m_log->getFile(),"load begin \n");
	int ret = loader->Load();
	if(ret == 0)
	{
		//加载失败
		unrefProtocolLib(libname);
		loader = 0;
	}
	else if(ret == 1)
	{
		//加载成功
		MOD_INIT mod_init = (MOD_INIT)(loader->GetFuncPtr(HR_MOD_INIT));
		if(mod_init)
		{
			//mod_init(g_debug_level);
		}
	}

	return loader;
}
//去掉动态库引用
void CRunManager::unrefProtocolLib(string& libname)
{
	CLibLoader* loader;
	if(m_protocol_lib.Lookup(libname, loader))
	{
		if(loader->GetRefCount() == 1)
		{
			MOD_EXIT mod_exit = (MOD_EXIT)(loader->GetFuncPtr(HR_MOD_EXIT));
			if(mod_exit)
			{
				mod_exit();
			}

			m_protocol_lib.RemoveKey(libname);
			delete loader;
		}
		else
			loader->Unload();
	}
}
Q_chDownReturn* CRunManager::createZtProtocol(ST_GeCtrlChl *cfg)
{
	//设置通道的遥测遥信个数为全部
	CScadaDataBase *scada=m_pMgr->getRtdbInf()->getScadaDb();
	if(scada!=NULL)
	{
      int algCount= scada->GetElementCount(SCADA_TAB_Analog_ID);

	  int pntCount= scada->GetElementCount(SCADA_TAB_Digit_ID);

	  cfg->YXbegin=1;
	  cfg->YXend=pntCount;
	  cfg->YCbegin=1;
	  cfg->YCend=algCount;
	}
	//int m_pMgr->getRtdbInf()
	return createScadaProtocol(cfg);
}
Q_chDownReturn* CRunManager::createScadaProtocol(ST_GeCtrlChl *cfg)
{
	TRACE("strFilePath = %s%s  strProName = %s",szIniFile,cfg->cfgfile,cfg->protocol);
	
	char strFilePath[MAX_PATH];
	sprintf_s(strFilePath,MAX_PATH,"%s%s",szIniFile,cfg->cfgfile);

	Q_chDownReturn* pProtocol=NULL;

	string strProName;

#ifdef WINDOWS_ENV
	strProName =cfg->protocol;
#endif //OS_WINDOWS

#ifdef UNIX_ENV
	strProName = "lib";
	strProName += cfg->protocol;  //like "libProtocol.so"
#endif	//OS_UNIX

	strProName += HR_LIB_EXT;

	CLibLoader* libloader=NULL;	
	CREATE_PROTOCOL_OBJECT create_protocol_object;

	//引用动态库，使动态库加载到内存中
	libloader = refProtocolLib(strProName);
	
	if(libloader)
	{
		create_protocol_object = (CREATE_PROTOCOL_OBJECT)(libloader->GetFuncPtr(HR_PROTOCOL_CREATE));
		if(!create_protocol_object)
		{
			
			TRACE("create_protocol_object failed 1");
			//Q_commonUtility::outError(m_log->getFile(),"EM: can't get fucntion pointer '%s' of library '%s'\n", HR_PROTOCOL_CREATE, strProName.c_str());
		}
		else	
		{
			Q_chDownCommPara v_param;
			v_param.cfg=cfg;
			v_param.m_pMgr=m_pMgr;
			memcpy(v_param.strFilePath,strFilePath,sizeof(strFilePath));
			
			TRACE("import create_protocol_object \n");
			pProtocol= create_protocol_object(&v_param);
			
			if(pProtocol)
			{
				//把得到内容push到配置文件或runner中
				//m_GSettting.addChannel(pProtocol->unit);
				TRACE("addUnitaddUnitaddUnit/n");
				m_taskManager.addUnit(pProtocol->unit);

				TaskCfgList tskcfg;
				tskcfg.data=pProtocol;
				tskcfg.cfg=cfg;

				m_taskList.push_back(tskcfg);

			}
		}
	}

	return pProtocol;
}

//remove portname<->dll map
void CRunManager::freeScadaProtocol(Q_chDownReturn* pProtocol)
{	
	if(pProtocol==NULL)
		return ;

	string strProName;

#ifdef OS_UNIX
	strProName = "lib";
#endif	//OS_UNIX

	strProName += pProtocol->protocol;
	strProName += HR_LIB_EXT;

	CLibLoader* libloader;	
	FREE_PROTOCOL_OBJECT free_protocol_object;
	libloader = refProtocolLib(strProName);

	if(libloader)
	{
		free_protocol_object = (FREE_PROTOCOL_OBJECT)(libloader->GetFuncPtr(HR_PROTOCOL_FREE));
		if(!free_protocol_object)
		{	
			//debug_printf(dfl_sensive, "EM: can't get fucntion pointer '%s' of library '%s'\n", HR_PROTOCOL_FREE, strProName.c_str());
		}
		else	
		{
			free_protocol_object(pProtocol);
		}

		unrefProtocolLib(strProName);
	}

	unrefProtocolLib(strProName);
}

bool CRunManager::chkMsg()
{
	bool ret=false;

	//检查主站消息的处理
	if(m_pMgr->getRtdbInf())
	{ 
		MesBody mbody;
		for(UINT i=0;i<m_taskList.size();i++)
		{
			printf("get msgmsgmsgmsgmsg begin\n");

	
		if(m_pMgr->getRtdbInf()->rtdb_get_message(m_taskList[i].data->channelID, &mbody))	
			{
			   printf("recv msgmsgmsgmsgmsg mbody.comm.type = %d\n",mbody.comm.type);
               if(MES_TYPE_RELAY==mbody.comm.type)//遥控
			   {
				   printf("收到遥控信息:通道号=%d,设备号=%d,点号=%d\n",m_taskList[i].data->channelID,
				   mbody.relay.device,mbody.relay.point);
				   Q_RECORDDATATAG rec; 
				   rec.channelID=m_taskList[i].data->channelID;
				   rec.device=mbody.relay.device;
				   rec.no=mbody.relay.point;
				   rec.bValue=mbody.relay.value==1?1:0;
				   rec.eDataType=eDataTypeBit;

                   ret=addMasterCmd(m_taskList[i],&rec);
			   }
			   else if(MES_TYPE_SPOINT==mbody.comm.type)//遥调
			   {
				   printf("收到遥调信息:通道号=%d,设备号=%d,点号=%d\n",m_taskList[i].data->channelID,
					   mbody.spoint.device,mbody.spoint.point);
				   Q_RECORDDATATAG rec;
				   rec.channelID=m_taskList[i].data->channelID;
				   rec.device= mbody.spoint.device;
				   rec.no=mbody.spoint.point;
				   rec.fValue=mbody.spoint.val;
				   rec.eDataType=eDataTypeFloat;

                   ret=addMasterCmd(m_taskList[i],&rec);
			   }
			   else if(MES_TYPE_MsgExpand==mbody.comm.type)//orlca消息
			   {
				   Q_RECORDDATATAG rec;
				   rec.channelID=m_taskList[i].data->channelID;
				   rec.device= mbody.spoint.device;
				   rec.no=mbody.spoint.point;
				   rec.fValue=0;
				   rec.eDataType=eDataTypeFloat;	 

				   ExpandMsg MsgExpand;
				   memcpy(&MsgExpand.comm,&mbody.mesmsgexpand.comm,sizeof(MsgExpand));	
				   printf("收到扩展消息\n");
				   ret=addMasterCmdExpandMsg(m_taskList[i],&rec,&MsgExpand);
			   }
			}
		 }

		handleSpecialMsg();
	}
	return ret;
}

bool CRunManager::addMasterCmd(TaskCfgList &task,LPQ_RECORDDATATAG rec)
{
	bool ret=false;
	string strProName;

#ifdef OS_UNIX
	strProName = "lib";
#endif	//OS_UNIX

	strProName +=task.data->protocol;
	strProName += HR_LIB_EXT;

	CLibLoader* libloader;	
	ADD_ROTOCOL_COMMAND add_protocol_command;
	libloader = refProtocolLib(strProName);

	if(libloader)
	{
		add_protocol_command = (ADD_ROTOCOL_COMMAND)(libloader->GetFuncPtr(HR_PROTOCOL_ADD));
		if(!add_protocol_command)
		{	
			//debug_printf(dfl_sensive, "EM: can't get fucntion pointer '%s' of library '%s'\n", HR_PROTOCOL_FREE, strProName.c_str());
		}
		else	
		{
			/*

			if(task.data->unit->CheckMasterRun() && add_protocol_command(task.data->task,rec))
			{
			ret=true;
			}

			*/
			if(task.data->bTcpServer==true)
			{
				TRACE("----------------addMasterCmd server 1\r\n ");

				Q_tcpServerUnitInfo* pSrv=task.data->taskServer;
				if(pSrv!=NULL)
				{
					TRACE("----------------addMasterCmd server 2\r\n ");
				//	Q_taskBase *taskBase= pSrv->getTaskByChannelID(task.data->channelID);
					Q_taskBase *taskBase= pSrv->getTaskByChannelID(rec->channelID);
					if(taskBase!=NULL)
					{
						TRACE("----------------addMasterCmd server 3\r\n ");
						if(taskBase->CheckMasterRun())
						{
							TRACE("----------------addMasterCmd server 4\r\n ");
							   ret=true;
							  add_protocol_command(taskBase,rec);
							  TRACE("----------------addMasterCmd server 5\r\n ");
						}
					}

				}
			}
			else
			{
				if(task.data->unit->CheckMasterRun())
				{
					ret=true;
					 add_protocol_command(task.data->task,rec);
				}
			}
			
		}

	}

	return ret;
}

bool CRunManager::addMasterCmdExpandMsg(TaskCfgList &task,LPQ_RECORDDATATAG rec,ExpandMsg* MsgExpand)
{
	bool ret=false;
	string strProName;

#ifdef OS_UNIX
	strProName = "lib";
#endif	//OS_UNIX

	strProName +=task.data->protocol;
	strProName += HR_LIB_EXT;

	CLibLoader* libloader;	
	ADD_ROTOCOL_COMMAND_EXPAND add_protocol_command_expand;
	libloader = refProtocolLib(strProName);

	if(libloader)
	{
		add_protocol_command_expand = (ADD_ROTOCOL_COMMAND_EXPAND)(libloader->GetFuncPtr(HR_PROTOCOL_ADD_EXPAND));
		if(!add_protocol_command_expand)
		{	
			//debug_printf(dfl_sensive, "EM: can't get fucntion pointer '%s' of library '%s'\n", HR_PROTOCOL_FREE, strProName.c_str());
		}
		else	
		{
			char ctest[100] = {0};
			if(task.data->unit->CheckMasterRun() && add_protocol_command_expand(task.data->task,rec,MsgExpand))
			{
				ret=true;
			}
		}
	}
	return ret;
}
CMultiDataBase *CRunManager::getMaindb()
{
	if(m_pMgr!=NULL)
	{
       if(m_pMgr->getRtdbInf())
	   {
		   return m_pMgr->getRtdbInf()->getMainDb();
	   }
	}
	return NULL;
	
}
Q_ThreadStatusType CRunManager::getRunStatus()
{
	return m_eCalcThreadStatus;
}
ThreadFuncRet CRunManager::UpdataProcessPara(LPVOID lpParameter)
{
	CRunManager *runMgr=(CRunManager*)lpParameter;
	if(runMgr==NULL)
		return 0;

	CMultiDataBase *mdb=NULL;
	mdb=runMgr->getMaindb();

	if(mdb==NULL)
		return 0;

	mdb->updateProcessTabPara(pt_CHANNELOP,TRUE);

	while(runMgr->getRunStatus()==eThreadRun)
	{
		MilliSleep(3000);
		printf("UpdataNodeParaTimer\n"); 
		mdb->updateNodeTabPara();
		mdb->updateProcessTabPara(pt_CHANNELOP,FALSE);
	}
	return 0;
}

int CRunManager::printProtocolChangeLog(char *libName)
{
	int ret=0;

	string strProName;

#ifdef WINDOWS_ENV
	strProName =libName;
#endif //OS_WINDOWS

#ifdef UNIX_ENV
	strProName = "lib";
	strProName += libName;  //like "libProtocol.so"
#endif	//OS_UNIX

	strProName += HR_LIB_EXT;

	CLibLoader* libloader=NULL;	
	PRINT_CHANGE_LOG create_protocol_object;

	//引用动态库，使动态库加载到内存中
	libloader = refProtocolLib(strProName);

	if(libloader)
	{
		create_protocol_object = (PRINT_CHANGE_LOG)(libloader->GetFuncPtr(HR_PRO_CHANGELOG));
		if(!create_protocol_object)
		{
			//int n=GetLastError();
			//printf("error %d",n);
			//Q_commonUtility::outError(m_log->getFile(),"EM: can't get fucntion pointer '%s' of library '%s'\n", HR_PROTOCOL_CREATE, strProName.c_str());
		    printf("did not find library [%s] for print change log \n",strProName.c_str());
		}
		else	
		{
			ret=create_protocol_object();
			
		}
	}

	return ret;
}
/*
根据通道ID能得到规约的类型，如果规约的类型，规约子类型为不为0时，优先遍历一下规约库，
//如果不为0则返回通道ID，如果为0则表示为正常的ID
*/

int CRunManager::getTaskMainChannelID(int chid)
{

	int mainid=0;
	bool ret=false;
	if(m_pMgr==NULL)
	{
		return mainid;
	}
	if(m_pMgr->getRtdbInf()==NULL)
	{
		return mainid;
	}
	CDBTable v_pro;

	CScadaDataBase *scada=m_pMgr->getRtdbInf()->getScadaDb();
	if(scada!=NULL)
	{
		scada->GetDBTableFromID(v_pro,SCADA_TAB_Channel_ID);
	}
	else
	{
		return mainid;
	}

	if(v_pro.GetRecordCount()==0)
	{
		return mainid;
	}
	for(int i=0;i<v_pro.GetRecordCount();i++)
	{
		SCADA_Channel item;
		scada->GetElementByIndex(SCADA_TAB_Channel_ID,i,&item);

		if(item.ID32==chid)
		{ 
			SCADA_Protocol protocol;
			m_GSettting.getProtoInfoByTypeID(item.uProtocolID,protocol);
			if(protocol.iProtocolSubType!=0)
			{
				if(strcmp(protocol.strClassName,"CommIEC103Slave")==0)
				{
					//规约类型不为0,且表示当前为ret;
					ret=true;
					mainid=item.iRunAttr;
				  //特殊写的

				}
			
			}
			break;
		}
	}

	return mainid;
}
int CRunManager::handleSpecialMsg()
{
	int chid=0;

	for(UINT i=0;i<m_taskList.size();i++)
	{
		if(m_taskList[i].data->bTcpServer)
		{
			//TRACE("##############CRunManager::handleSpecialMsg: is TaskServer index=%d,",i,m_taskList[i].data->channelID);

			//如果为服务器端，即服务器端做主站时，特殊处理一下
			//查找通道表中的动态属性，如果配置为自已，就表示需要查找一下关联子类是否有消息
			//查找通道表中的所有动态属性不为1的内容
			
			bool ret=false;
			if(m_pMgr==NULL)
			{

				return 0;
			}
			if(m_pMgr->getRtdbInf()==NULL)
			{

				return 0;
			}

			CScadaDataBase *scada=m_pMgr->getRtdbInf()->getScadaDb();
	
			SCADA_Channel channel;

			if(scada->GetElementByID(SCADA_TAB_Channel_ID,m_taskList[i].data->channelID,&channel))
			{
				if(channel.iRunAttr==channel.ID32)
				{
					chid=channel.iRunAttr;
					ret=true;
				}
			}
		//	TRACE("##############CRunManager::handleSpecialMsg:find iRunAttr=%s\r\n",ret==true?"TRUE":"FALSE");

			if(ret)
			{
				//遍历所有消息
				int nCount=scada->GetTableRecordCount(SCADA_TAB_Channel_ID);
				
			//	TRACE("##############CRunManager::handleSpecialMsg:getChannelCount=%d\r\n",nCount);
				MesBody mbody;
				for(int j=0;j<nCount;j++)
				{
					if(scada->GetElementByID(SCADA_TAB_Channel_ID,j+1,&channel))
					{
						if(channel.iRunAttr==chid&&channel.iFlag>0)
						{
	                // TRACE("##############CRunManager::handleSpecialMsg:channel.iRunAttr==chid =%d &&channel.iFlag\r\n",channel.ID32);

							if(m_pMgr->getRtdbInf()->rtdb_get_message(channel.ID32, &mbody))
							{
								TRACE("##############CRunManager::handleSpecialMsg:find channel %d message success \r\n",channel.ID32);
								if(MES_TYPE_RELAY==mbody.comm.type)//遥控
								{
									printf("收到特殊遥控信息:通道号=%d,设备号=%d,点号=%d\n",channel.ID32,
										mbody.relay.device,mbody.relay.point);
									Q_RECORDDATATAG rec; 
									rec.channelID=channel.ID32;
									rec.device=mbody.relay.device;
									rec.no=mbody.relay.point;
									rec.bValue=mbody.relay.value==1?1:0;
									rec.eDataType=eDataTypeBit;

									ret=addMasterCmd(m_taskList[i],&rec);
								}
								else if(MES_TYPE_SPOINT==mbody.comm.type)//遥调
								{
									printf("收到特殊遥调信息:通道号=%d,设备号=%d,点号=%d\n",channel.ID32,
										mbody.spoint.device,mbody.spoint.point);
									Q_RECORDDATATAG rec;
									rec.channelID=channel.ID32;
									rec.device= mbody.spoint.device;
									rec.no=mbody.spoint.point;
									rec.fValue=mbody.spoint.val;
									rec.eDataType=eDataTypeFloat;

									ret=addMasterCmd(m_taskList[i],&rec);
								}
							}
							
						}
						
					}
					else
					{
						TRACE("##############CRunManager::handleSpecialMsg:scada->GetElementByID(SCADA_TAB_Channel_ID failed id=%d",j+1);
					}
				}

			}
		}
	}
	return 0;
}