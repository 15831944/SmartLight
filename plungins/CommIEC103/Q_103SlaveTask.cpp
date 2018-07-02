#include "gSystemDef.h"
#include "Q_103SlaveTask.h"
#include "Q_channelBase.h"
#include "Q_ProtocolIEC103Slave.h"
//----------------------------------------------------------------------------
//*
//*
//* Q_modbusTcpServerTask class
//*
//*
Q_103SlaveTask::Q_103SlaveTask(void)
{
}

Q_103SlaveTask::~Q_103SlaveTask(void)
{
}

bool Q_103SlaveTask::acceptClient()
{
	if(m_pTcpServerLink->getLinkStatus() != eCommonStatusNormal)
	{
		TRACE("Q_103SlaveTask::m_pTcpServerLink status is abnormal\n");
		return false;
	}
	/*
	ȫ�������������ģ����Եķ���������һ���鲥��Ϣ��������̨��������Ȼ������Ҫ��udp֡�����еĿͻ���
	1032�˿ڷ������ݣ���������Ϊ
	UDP����: FF 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
	 Ȼ�����Եĳ���ͻᶯ��������1048�˿��ˡ�
	*/

	//���ӷ��͵���ӦIP��UDP���ĵĹ���
	Q_IEC103Setting* p103setting = (Q_IEC103Setting*)m_pProtocolSetting;
	if (p103setting!=NULL)
	{
		char strTmp[50];
		memset(strTmp,NULL,sizeof(strTmp));
		sprintf_s(strTmp,sizeof(strTmp),"%s:%d",inet_ntoa(m_pTcpServerLink->getClientAddress().sin_addr),m_pTcpServerLink->getClientAddress().sin_port);
		
		TRACE("Q_103SlaveTask:clinet send udp message:%s begin\n",strTmp);

		Q_103BaseCfg v_basecfg;
		p103setting->getBaseSetting(&v_basecfg);

		for(int i=0;i<MAX_103_REMOTE_NUM;i++)//��Ϊ���������
		{
			if(v_basecfg.remServer[i].valid)
			{
				//	char *pIP = p103setting->getContentID(4);
				//if (pIP!=NULL && strcmp(pIP,"")!=0)
				//WSADATA wsaData;                                    //ָ��WinSocket��Ϣ�ṹ��ָ��
				bool fBroadcast = true;                             //����setsockopt(),��ʾ����
				SOCKET udpsocket;                                //�����׽���
				sockaddr_in saUdpServ;                             
				udpsocket = socket(PF_INET,SOCK_DGRAM,0);
				saUdpServ.sin_family = AF_INET;
			//	saUdpServ.sin_addr.s_addr =inet_addr(inet_ntoa(m_pTcpServerLink->getClientAddress().sin_addr));//pIP
			    saUdpServ.sin_addr.s_addr =inet_addr(v_basecfg.remServer[i].ip);//pIP
				saUdpServ.sin_port = htons(1032);
				char chbuff[41];
				memset(chbuff,0,41);
				chbuff[0] = 0xff;
				int ret = sendto(udpsocket,chbuff,41,0,(struct sockaddr *) &saUdpServ, sizeof(sockaddr_in));
				closesocket(udpsocket);

				TRACE("Q_103SlaveTask:send udp message:ip%s\n",v_basecfg.remServer[i].ip);
			}
		}
		
	}

	if(m_pTcpServerLink->acceptClient() == true)
	{
		
		// setup protocol info
		Q_IEC103Setting* ppp = (Q_IEC103Setting*)m_pProtocolSetting;

		int chid=ppp->getChannelID();

		char strTmp[50];
		memset(strTmp,NULL,sizeof(strTmp));
		sprintf_s(strTmp,sizeof(strTmp),"%s:%d",inet_ntoa(m_pTcpServerLink->getClientAddress().sin_addr),m_pTcpServerLink->getClientAddress().sin_port);
		TRACE("Q_103SlaveTask:a new client connected:%s\n",strTmp);

		

		Q_103BaseCfg v_basecfg;
		ppp->getBaseSetting(&v_basecfg);

		if(v_basecfg.remServer[0].valid)
		{
			//���ù��˹���
			char strClientIp[50];
			memset(strClientIp,NULL,sizeof(strClientIp));
			sprintf_s(strClientIp,sizeof(strClientIp),"%s",inet_ntoa(m_pTcpServerLink->getClientAddress().sin_addr));

			//
			bool findflg=false;
			for(int i=0;i<MAX_103_REMOTE_NUM;i++)//��Ϊ���������
			{
				if(v_basecfg.remServer[i].valid)
				{
					if(strcmp(v_basecfg.remServer[i].ip,strClientIp)==0)
					{
						chid=v_basecfg.remServer[i].port;
						findflg=true;
						break;
					}
				}
			}
			if(!findflg)
			{
				TRACE("Q_103SlaveTask::this ip is not in authentication list\n");
				return false;
			}
		}

		m_clientChannel = new Q_channelBase();

		Q_commonBuffer* bufferReceive = new Q_commonBuffer();
		Q_commonBuffer* bufferSend = new Q_commonBuffer();		
		m_clientChannel->setReceiveBuffer(bufferReceive);
		m_clientChannel->setSendBuffer(bufferSend);

		// setup link info
		Q_tcpServerClientLink* linkTcpClient = new Q_tcpServerClientLink();
		linkTcpClient->setLogStatus(m_pTcpServerLink->getLogStatus());
		Q_tcpServerClientLinkSetting* tLinkSetting = new Q_tcpServerClientLinkSetting();
		tLinkSetting->setSocket(m_pTcpServerLink->getClientSocket());
		tLinkSetting->setAddress(m_pTcpServerLink->getClientAddress());
		Q_linkConfig* tLinkConfig = new Q_linkConfig();
		tLinkConfig->setLinkSetting(tLinkSetting);
		tLinkConfig->setReceiveBuffer(bufferReceive);
		tLinkConfig->setSendBuffer(bufferSend);
		//tLinkConfig->setLinkSetting(tLinkSetting); 2016-12-20 ������Ĵ����ظ�
		m_clientChannel->setLinkConfig(tLinkConfig);
		m_clientChannel->setLink(linkTcpClient);
		m_clientChannel->setLinkSetting(tLinkSetting);

		m_clientChannel->setChannelID(chid);//ԭ��ΪproSetting->getChannelID()

		Q_IEC103Setting* proSetting = new Q_IEC103Setting(ppp);



		m_clientChannel->setProtocolSetting(proSetting);
		Q_IEC103CommandManager* pCommandList = new Q_IEC103CommandManager();
		m_clientChannel->setCommandList(pCommandList);
		Q_protocolConfig* protocolConfig = new Q_protocolConfig();
		protocolConfig->setProtocolSetting(proSetting);
		protocolConfig->setReceiveBuffer(bufferReceive);
		protocolConfig->setSendBuffer(bufferSend);
		protocolConfig->setProtocolCommandManager(pCommandList);
		m_clientChannel->setProtocolConfig(protocolConfig);

		Q_protocolIEC103Slave * protocol = new Q_protocolIEC103Slave();
		m_clientChannel->setProtocol(protocol);	
		m_clientChannel->setPrimaryFlag(false);/*һ��Ҫ����Ϊfalse,�����������Ҳ�᲻ͣ������*/

		proSetting->setChannelID(chid);
		return true;
	}

	return false;
}
