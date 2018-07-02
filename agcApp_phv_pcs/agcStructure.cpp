#include "agcStructure.h"
#include "agcDefine.h"
Q_AgcParameter::Q_AgcParameter()
{
	m_pfRtdb=NULL;
}
Q_AgcParameter::~Q_AgcParameter()
{

}
void Q_AgcParameter::setRtdbPointer(agcFuncRtdb *v)
{
	m_pfRtdb=v;
}
int Q_AgcParameter::getCalcInterval()
{
	int ret=3000;//Ĭ��Ϊ3��
	if(m_pfRtdb!=NULL)
	{
		ret=m_pfRtdb->getCommAdParam(eAgcParamCalcInterval);
	}
	if(ret<3000)
		ret=3000;
	return ret;
}

int Q_AgcParameter::getSendCmdInterval()
{
	int ret=500;//500����
	if(m_pfRtdb!=NULL)
	{
		ret=m_pfRtdb->getCommAdParam(eAgcParamSendCmdInterval);
	}

	return ret;
}

bool Q_AgcParameter::getReSendFlg()
{
	bool bRet=false;//Ĭ�ϲ��ظ�����
	if(m_pfRtdb!=NULL)
	{
		int ret=m_pfRtdb->getCommAdParam(eAgcParamReSendFlg);
		if(ret>0)
			bRet=true;
	}

	return bRet;
}
bool Q_AgcParameter::getLogFlg()
{
	bool bRet=false;//Ĭ�ϲ���ӡ��¼
	if(m_pfRtdb!=NULL)
	{
		int ret=m_pfRtdb->getCommAdParam(eAgcParamLogFlg);
		
		if(ret>0)
			bRet=true;
	}

	return bRet;
}



bool Q_AgcParameter::getLockCheckFlg()
{
	bool bRet=false;
	if(m_pfRtdb!=NULL)
	{
		int ret=m_pfRtdb->getCommAdParam(eAgcParamLockCheckFlg);
		
		if(ret>0)
			bRet=true;
	}

	return bRet;
}
bool Q_AgcParameter::getSafeCheckFlg()
{
	bool bRet=false;
	if(m_pfRtdb!=NULL)
	{
		int ret=m_pfRtdb->getCommAdParam(eAgcParamSafeCheckFlg);
		
		if(ret>0)
			bRet=true;
	}

	return bRet;
}

AgcAdMethod  Q_AgcParameter::getAdMethod()
{
	AgcAdMethod ret=eAdMethodByOpenCapicity;
	int n=1;
	if(m_pfRtdb!=NULL)
	{
		n=m_pfRtdb->getCommAdParam(eAgcParamAdMethod);
	}

	switch(n)
	{
	case 1:
		ret=eAdMethodByOpenCapicity;
		break;
	case 2:
		ret=eAdMethodByMargin;
		break;
	}

	return ret;
}
//�õ�1���ӱ仯�����Ʊ�ʶ
bool Q_AgcParameter::getOneMinLimitFlg()
{
	bool bRet=false;
	if(m_pfRtdb!=NULL)
	{
		int ret=m_pfRtdb->getCommAdParam(eAgcParamOneMinLimitFlg);

		if(ret>0)
			bRet=true;
	}
	
	return bRet;
}


int   Q_AgcParameter::getCmdTimeOutSecs()
{
	int ret=90;//500����
	if(m_pfRtdb!=NULL)
	{
		ret=m_pfRtdb->getCommAdParam(eAgcParamCmdTimeOut);
	}

	return ret;
}
