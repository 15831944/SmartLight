#include "gSystemDef.h"
#include "Q_modbusSlaveTask.h"
#include "Q_channelBase.h"
#include "Q_protocolModbusSlave.h"


//----------------------------------------------------------------------------
//*
//*
//* Q_modbusTcpServerTask class
//*
//*
Q_modbusSlaveTask::Q_modbusSlaveTask(void)
{
}

Q_modbusSlaveTask::~Q_modbusSlaveTask(void)
{
}

bool Q_modbusSlaveTask::acceptClient()
{
	if(m_pTcpServerLink->getLinkStatus() != eCommonStatusNormal)
	{
		//TRACE("Q_modbusSlaveTask::acceptClient() status is abnormal\n");
		return false;
	}

	if(m_pTcpServerLink->acceptClient() == true)
	{
		TRACE("Q_modbusSlaveTask::acceptClient() == true\n");

		m_clientChannel = new Q_channelBase();
		Q_commonBuffer* bufferReceive = new Q_commonBuffer();
		Q_commonBuffer* bufferSend = new Q_commonBuffer();	
		m_clientChannel->setReceiveBuffer(bufferReceive);
		m_clientChannel->setSendBuffer(bufferSend);

		/*setup link info*/
		Q_tcpServerClientLink* linkTcpClient = new Q_tcpServerClientLink();
		linkTcpClient->setLogStatus(m_pTcpServerLink->getLogStatus());
		Q_tcpServerClientLinkSetting* tLinkSetting = new Q_tcpServerClientLinkSetting();
		tLinkSetting->setSocket(m_pTcpServerLink->getClientSocket());
		tLinkSetting->setAddress(m_pTcpServerLink->getClientAddress());
		Q_linkConfig* tLinkConfig = new Q_linkConfig();
		tLinkConfig->setLinkSetting(tLinkSetting);
		tLinkConfig->setReceiveBuffer(bufferReceive);
		tLinkConfig->setSendBuffer(bufferSend);
		tLinkConfig->setLinkSetting(tLinkSetting);


		m_clientChannel->setLinkConfig(tLinkConfig);
		m_clientChannel->setLink(linkTcpClient);
		m_clientChannel->setLinkSetting(tLinkSetting);
		m_clientChannel->setPrimaryFlag(false);/*一定要设置为false,否则服务器端也会不停地重连*/

		/*protocolsetting config*/
		Q_protocolModbusSlaveSetting* proSetting = new Q_protocolModbusSlaveSetting((Q_protocolModbusSlaveSetting*)m_pProtocolSetting);
		m_clientChannel->setProtocolSetting(proSetting); //fs20150716	

		/*setup CommandList*/
		Q_protocolModbusSlaveCommandManager* pCommandList = new Q_protocolModbusSlaveCommandManager();
		m_clientChannel->setCommandList(pCommandList);
		Q_protocolConfig* protocolConfig = new Q_protocolConfig();
		protocolConfig->setProtocolSetting(proSetting);
		protocolConfig->setReceiveBuffer(bufferReceive);
		protocolConfig->setSendBuffer(bufferSend);
		protocolConfig->setProtocolCommandManager(pCommandList);
		m_clientChannel->setProtocolConfig(protocolConfig);

		/*protocol config*/
		Q_protocolModbusSlave * protocol = new Q_protocolModbusSlave();
		m_clientChannel->setProtocol(protocol);	
		m_clientChannel->setChannelID(proSetting->getChannelID());//fs ppp

		TRACE("Q_modbusSlaveTask:: a new client create success channelid = %d\n",proSetting->getChannelID());
		return true;
	}

	return false;
}
