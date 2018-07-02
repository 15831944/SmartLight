//windows�汾����
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "gSystemDef.h"
#include "tinyxmlinclude.h"
#include "Q_serialLink.h"
#include "Q_runingUnitDefine.h"
#include <time.h>
#include <string>
using namespace std;

Q_dataManager *m_pMgrModbusSlaveProtocolSetting; /*ModbusSlave��rtdbȫ�ֱ���*/
typedef enum _Q_ModXmlDataType
{
    eModXmlDataTypeNull=0,//δ֪
    eModXmlDataTypeInt=1,
    eModXmlDataTypeFloat=2,
    eModXmlDataTypeEnum=3,
    eModXmlDataTypeString=4
}Q_ModXmlDataType;
typedef struct ST_ModCfgT
{
    int group; //����
    int id;    //���
    Q_ModXmlDataType type;
    std::string name;  //������
    std::string value; //����ֵ
}ST_ModCfgT;

//���������
class Q_protcolModbusRtuSlvSetting : public Q_protocolSettingBase
{
public:
    Q_protcolModbusRtuSlvSetting(void) { memset(LastError,0,sizeof(LastError)); }
    virtual ~Q_protcolModbusRtuSlvSetting(void) {}
    void setFilePath(char *v_path) { m_sFilePath = v_path; }
    bool initSetting()
    {
        TiXmlDocument doc(m_sFilePath.c_str());
        doc.LoadFile();
        if(doc.ErrorId() > 0)  
        {
#ifdef _DEBUG
            sprintf_s(LastError,sizeof(LastError)-1,"���������ļ�������ǰ�ļ�:%s����ǰ�к�:%d\n",__FILE__,__LINE__);
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
        for ( node=todoElement->FirstChild( "Record" ); node; node=node->NextSibling( "Record" ) )
        {
            itemElement=node->ToElement();
            int groupid = atoi(itemElement->Attribute("id"));
            if(0)
            {

            }
            else
            {
                for ( nodeItem=node->FirstChild("Field"); nodeItem; nodeItem=nodeItem->NextSibling("Field") )
                {
                    itemElement=nodeItem->ToElement();
                    int id = atoi(itemElement->Attribute("id"));

                    ST_ModCfgT content;
                    content.group = groupid;
                    content.id = id;
                    content.name = itemElement->Attribute("name");
                    content.value = itemElement->Attribute("value");

                    const char *type=itemElement->Attribute("type");
                    if (strcmp(type,"STRING")==0)
                        content.type=eModXmlDataTypeString;
                    else if (strcmp(type,"ENUM")==0)
                        content.type=eModXmlDataTypeEnum;
                    else if (strcmp(type,"INT")==0)
                        content.type=eModXmlDataTypeInt;
                    else if (strcmp(type,"FLOAT")==0)
                        content.type=eModXmlDataTypeFloat;
                    m_vContent.push_back(content);
                }
            }
        }
        return true;
    }
    virtual bool reloadLog()
    {
        TiXmlDocument doc(m_sFilePath.c_str());
        doc.LoadFile();  
        if(doc.ErrorId() > 0)  
        {
#ifdef _DEBUG
            sprintf_s(LastError,sizeof(LastError)-1,"���������ļ�����,��ǰ�к�:%d\n",__LINE__);
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
        for ( node=todoElement->FirstChild( "Record" ); node; node=node->NextSibling( "Record" ) )
        {
            itemElement=node->ToElement();
            int groupid = atoi(itemElement->Attribute("id"));
            if(10 == groupid)
            {
                for ( nodeItem=node->FirstChild("Field");nodeItem; nodeItem=nodeItem->NextSibling("Field"))
                {
                    itemElement=nodeItem->ToElement();
                    int id = atoi(itemElement->Attribute("id"));
                    for (unsigned int i=0;i<m_vContent.size();i++)
                    {
                        if (m_vContent[i].group==groupid && m_vContent[i].id==id)
                        {
                            m_vContent[i].name = itemElement->Attribute("name");
                            m_vContent[i].value = itemElement->Attribute("value");
                        }
                    }
                }
            }
        }
        return true;
    }
    bool getPrintPacket()
    {
        int res=0;
        const char *tmp = getContentID(1,10);
        if(tmp!=NULL)
            res=atoi(tmp);
        return res==0?false:true;
    }
    bool getPrintTranslate()
    {
        int res=0;
        const char *tmp = getContentID(2,10);
        if(tmp!=NULL)
            res=atoi(tmp);
        return res==0?false:true;
    }
    bool getPrintPrompt()
    {
        int res=0;
        const char *tmp = getContentID(3,10);
        if(tmp!=NULL)
            res=atoi(tmp);
        return res==0?false:true;
    }
    bool getPrintError()
    {
        int res=0;
        const char *tmp = getContentID(4,10);
        if(tmp!=NULL)
            res=atoi(tmp);
        return res==0?false:true;
    }
    int getDevAddr()
    {
        int res=1;
        const char *tmp = getContentID(1,1);
        if(tmp!=NULL)
            res=atoi(tmp);
        return res;
    }
    int getDataOrd()
    {
        int res=1;
        const char *tmp = getContentID(2,1);
        if(tmp!=NULL)
            res=atoi(tmp);
        return res;
    }
    int getYcType()
    {
        int res=1;
        const char *tmp = getContentID(3,1);
        if(tmp!=NULL)
            res=atoi(tmp);
        return res;
    }
    int getYtType()
    {
        int res=1;
        const char *tmp = getContentID(4,1);
        if(tmp!=NULL)
            res=atoi(tmp);
        return res;
    }

    const char *getContentID(const int &v_idx, const int &v_grp)
    {
        for (UINT i=0;i<m_vContent.size();i++)
        {
            if (m_vContent[i].group==v_grp && m_vContent[i].id==v_idx)
                return m_vContent[i].value.c_str();
        }
        return NULL;
    }

private:
    //�ļ�·��
    std::string m_sFilePath;
    //����
    std::vector<ST_ModCfgT> m_vContent;
    //������Ϣ
    char LastError[Q_FRAME_CONST1024];
};

//���������
class Q_protocolModbusRtuSlvCommandMgr : public Q_protocolCommandManagerBase
{
public:
    Q_protocolModbusRtuSlvCommandMgr(void) {}
    virtual ~Q_protocolModbusRtuSlvCommandMgr(void) {}

};
class Q_protocolModbusRtuSlvCommand : public Q_protocolCommandBase
{
public:
	Q_protocolModbusRtuSlvCommand(void) { memset(&msg,0,sizeof(msg)); }
	virtual ~Q_protocolModbusRtuSlvCommand(void) {}

    Q_RECORDDATATAG msg;
};

static WORD CRC16(BYTE* data, WORD len)
{
    WORD crc=0xffff;
    for(WORD i=0;i<len;i++){
        crc^=*(data+i);
        for(WORD j=0;j<8;j++){
            if(crc&1){
                crc>>=1;
                crc^=0xa001;
            }else{
                crc>>=1;
            }
        }
    }
    return crc;
}

//��Լʵ����
class Q_protocolModbusRtuSlv : public Q_protocolBase
{
public:
	Q_protocolModbusRtuSlv(void)
    {
        m_PrintSetting = NULL;
        m_LastTime = 0;
        m_TimeOut = 10;
        memset(m_LastMsg,0,sizeof(m_LastMsg));
    }
	virtual ~Q_protocolModbusRtuSlv(void)
    {
    }

public:
	virtual bool initializeProtocol() { return true; }
	virtual bool explainData()
    {
		TRACE("explainData begin");
        char bufferReceive[Q_FRAME_CONST4096];	
        memset(bufferReceive,0,sizeof(bufferReceive));

        //���ͱ���
        int nLength = m_pProcotolConfig->getReceiveBuffer()->getDataLength();
     /*   if (nLength && m_PrintSetting && m_PrintSetting->getPrintPacket())
        {*/
            int nByteCount = m_pProcotolConfig->getReceiveBuffer()->readData(bufferReceive,Q_FRAME_DEFAULT_RECEIVEBUFFERSIZE,false);
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeRx,bufferReceive,nByteCount);
      //  }
        //��������
        while(nLength >= 8)
        {
            int nRes = 0;
            int nByteCount = m_pProcotolConfig->getReceiveBuffer()->readData(bufferReceive,Q_FRAME_DEFAULT_RECEIVEBUFFERSIZE,false);
            if(nByteCount >= 8)
                explainDataEx((unsigned char *)bufferReceive,nByteCount,nRes);
            //���Ȳ���
            if (nRes == 0)
                break;
            m_pProcotolConfig->getReceiveBuffer()->removeData(nRes);
            nLength -= nRes;
        }
        return true;
    }
	bool explainDataEx(unsigned char* buff, int nCount, int& nCurrentPos)
    {
		TRACE("Q_protocolModbusRtuSlv::explainDataEx begin\n");
        nCurrentPos = 1;
        if (m_PrintSetting == NULL)
		{ 
			TRACE("Q_protocolModbusRtuSlv::m_PrintSetting =NULL\n");
			return false;
		}
        if (buff[0] != m_PrintSetting->getDevAddr())
		{ 
			TRACE("Q_protocolModbusRtuSlv::�豸��ַ�����\n");
			return false;
		}
        int yt = m_PrintSetting->getYtType();
        switch(buff[1])
        {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            {
			WORD crc = CRC16(buff,6);
            WORD crcc = buff[7]<<8|buff[6];
            if (crc != crcc)
			{
				sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"crcУ��ʧ��");
				SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
				return false;
			}
            nCurrentPos = 8;
            break;
			}
        case 6:
        case 15:
        case 16:
            {int nByteLen = 9+buff[6];
            //���ֽ�д��֧�ָ��㣬��̫�淶
            if (buff[1] == 6)
            {
                if (yt == 1)
                    nByteLen = 8;
                else
                    nByteLen = 10;
            }
            //���Ȳ���
            if (nByteLen > nCount)
            {
                nCurrentPos = 0;
                return false;
            }
            WORD crc = CRC16(buff,nByteLen-2);
            WORD crcc = buff[nByteLen-1]<<8|buff[nByteLen-2];
            if (crc != crcc)
                return false;
            nCurrentPos = nByteLen;
            break;}
        default:
            return false;
        }

        int nPos = 0;
        unsigned char sendbuf[300] = {0};
        sendbuf[nPos++] = buff[0];
        sendbuf[nPos++] = buff[1];
        sendbuf[nPos++] = 0;
        if (buff[1] == 1 || buff[1] == 2)
        {
            WORD addr = buff[2]<<8|buff[3];
            WORD len = buff[4]<<8|buff[5];
            WORD data = 0;
           /* if (m_PrintSetting->getPrintTranslate())
            {*/
                sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"��ȡYX:%d ��ַ:%d ����:%d\n",(int)buff[1],(int)addr,(int)len);
                SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
           // }
				int idx=0;
            for (WORD i=0; i<len && len <= 0x7d0; i++)
            {
                Q_RECORDDATATAG2 dataArray;
                memset(&dataArray,0,sizeof(dataArray));
                dataArray.val.channelID=m_PrintSetting->getChannelID();
               // dataArray.val.device=buff[1];
				dataArray.val.device=buff[0];
              //  dataArray.val.no=i+addr+1;
				dataArray.val.no=idx+addr;//ң�Ų���Ҫ��1
                dataArray.val.eDataType=eDataTypeBit;
	
			
                if(!m_pMgr->evalDataValue(&dataArray,1))
                {
                    //�¹�Լ��ӡ
                    sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"Can't find right YX point in realDB,Channel=%d,device=%d,addr=%d",m_PrintSetting->getChannelID(),(int)buff[1],(int)(i+addr+1));
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,m_LastMsg,strlen(m_LastMsg)+1);
                }
					TRACE("get yx data =%d,ch=%d,device=%d,no=%d\n",dataArray.val.bValue==true?1:0,dataArray.val.channelID,dataArray.val.device,dataArray.val.no);
                //����
                if ((i%8 == 0) && (i != 0))
                {
                    sendbuf[nPos++] = data & 0xff;
                    data = 0;
                }
                //��ֵ
                if (dataArray.val.nValue)
                {
                    data |= 1<<(i%8);
                }
				idx++;
            }
            //�Ĵ�����������
            if (len <= 0 || len >= 0x7d0)
            {
                sendbuf[1] |= 0x80;
                sendbuf[2] = 0x01;
                nPos = 3;
            }
            else
            {
                if ((len%8) != 0)
                {
                    sendbuf[nPos++] = data & 0xff;
                    data = 0;
                }
                sendbuf[2] = (len+7)/8;
                nPos = 3+(len+7)/8;
            }
            WORD crc = CRC16(sendbuf,nPos);
            sendbuf[nPos++] = crc & 0xff;
            sendbuf[nPos++] = (crc>>8) & 0xff;
        }
        else if (buff[1] == 3 || buff[1] == 4)
        {
            WORD addr = buff[2]<<8|buff[3];
            WORD len = buff[4]<<8|buff[5];
            int type = m_PrintSetting->getYcType();
            int ord = m_PrintSetting->getDataOrd();
           /* if (m_PrintSetting->getPrintTranslate())
            {*/
                sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"��ȡYC:%d ��ַ:%d ����:%d\n",(int)buff[1],(int)addr,(int)len);
                SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
          //  }
			int idx=0;
            for (WORD i=0; i<len && len <= 0x7d; i++)
            {
                Q_RECORDDATATAG2 dataArray;
                memset(&dataArray,0,sizeof(dataArray));
                dataArray.val.channelID=m_PrintSetting->getChannelID();
               // dataArray.val.device=buff[1];
				dataArray.val.device=buff[0];
                dataArray.val.no=addr+idx;
                dataArray.val.eDataType=eDataTypeFloat;


                if(!m_pMgr->evalDataValue(&dataArray,1))
                {
                    //�¹�Լ��ӡ
                    sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"Can't find right YC point in realDB,Channel=%d,device=%d,addr=%d",m_PrintSetting->getChannelID(),(int)buff[1],(int)(i+addr+1));
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,m_LastMsg,strlen(m_LastMsg)+1);
                }

				TRACE("get yc data =%.3f,ch=%d,device=%d,no=%d\n",dataArray.val.fValue,dataArray.val.channelID,dataArray.val.device,dataArray.val.no);
                if (type == 1)
                {
                    short data = (short)dataArray.val.fValue;
                    sendbuf[nPos++] = (data>>8) & 0xff;
                    sendbuf[nPos++] = data & 0xff;

				//	sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"send yc int %d",data );
                }
                else 
                {
                    union
                    {
                        int ndata;
                        float fdata;
                        unsigned char ch[4];
                    }data;
                    if (type == 2)
                        data.ndata = (int)dataArray.val.fValue;
                    else
                        data.fdata = dataArray.val.fValue;
                    if (ord == 1)
                    {
                        sendbuf[nPos++] = data.ch[1];
                        sendbuf[nPos++] = data.ch[0];
                        sendbuf[nPos++] = data.ch[3];
                        sendbuf[nPos++] = data.ch[2];
                    }
                    else
                    {
                        sendbuf[nPos++] = data.ch[3];
                        sendbuf[nPos++] = data.ch[2];
                        sendbuf[nPos++] = data.ch[1];
                        sendbuf[nPos++] = data.ch[0];
                    }
                    i++;
                }
				idx+=2;
            }
            //�Ĵ�����������
            if (len <= 0 || len >= 0x7d)
            {
                sendbuf[1] |= 0x80;
                sendbuf[2] = 0x01;
                nPos = 3;
            }
            else
            {
                sendbuf[2] = len*2;
                nPos = 3+len*2;
            }
            WORD crc = CRC16(sendbuf,nPos);
            sendbuf[nPos++] = crc & 0xff;
            sendbuf[nPos++] = (crc>>8) & 0xff;
        }
        else if (buff[1] == 5)
        {
            WORD addr = buff[2]<<8|buff[3];
            WORD val = buff[4]<<8|buff[5];
            bool setStatus = (val == 0)?false:true;
           /* if (m_PrintSetting->getPrintTranslate())
            {*/
                sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"�յ�ң������:%d ��ַ:%d ����:%x",(int)buff[1],(int)addr,(unsigned int)val);
                SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
         //   }

            CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
            CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

            bool isExit = false;
            SCADA_Relay curRelay;
            int nRelayRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_Relay_ID);
            for (int j=0; j<nRelayRecordCount; j++)
            {
                dbmg_sdb->GetElementByIndex(SCADA_TAB_Relay_ID, j,&curRelay);
                if (m_PrintSetting->getChannelID() == (int)curRelay.uTuID && (addr+1) == curRelay.iNoInDPU)
                {
                    isExit = true;
                    break;
                }
            }

            //��Ӧ�㲻����
            if (!isExit || curRelay.ulYXID32 < 0)
            {
                sendbuf[1] |= 0x80;
                sendbuf[2] = 0x01;
                nPos = 3;
            }
            else
            {
                //д��ң�Ŷ�Ӧ��ֵ
                SCADA_Digit tempDigit;
                if (dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID,curRelay.ulYXID32,&tempDigit))
                {
                    Q_RECORDDATATAG dataArray;
                    memset(&dataArray,0,sizeof(dataArray));
                    dataArray.eDataType=eDataTypeBit;
                    dataArray.channelID=tempDigit.uTuID;
                    dataArray.nIndex=tempDigit.iIndex;
                    dataArray.bValue=setStatus;
                    m_pMgr->recordDataValue(&dataArray,1,true);
                }
                //д��ң��ʱ��
                SYSTEMTIME time;
                GetLocalTime(time);
                curRelay.selecttime = time;
                curRelay.exectime = time;
                if(setStatus)
                {
                    curRelay.LogicalClose = 1;
                    curRelay.LogicalOpen = 0;
                }
                else
                {
                    curRelay.LogicalClose = 0;
                    curRelay.LogicalOpen = 1;
                }
                dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,curRelay.ID32,&curRelay);
                //ת����Ϣ
                if(curRelay.iZF)
                {
                    RControl rc;
                    rc.iStation =curRelay.izfTuID;
                    rc.iYKPoint =curRelay.izfNoInDPU;
                    rc.iCommandType = mes_RCExecuteOk;
                    rc.iVal = setStatus;
                    rc.iDevice=curRelay.izfDPUID;
                    rc.iZf=1;
                    m_rtdbInf->getMainDb()->SendMessage(curRelay.izfProcessID, mes_RCExecute, sizeof(RControl), (char *)&rc);
                }
                sendbuf[2] = buff[2];
                sendbuf[3] = buff[3];
                sendbuf[4] = buff[4];
                sendbuf[5] = buff[5];
                nPos = 6;
            }
            WORD crc = CRC16(sendbuf,nPos);
            sendbuf[nPos++] = crc & 0xff;
            sendbuf[nPos++] = (crc>>8) & 0xff;
        }
        else if (buff[1] == 6 || buff[1] == 16)
        {
            WORD i,addr = buff[2]<<8|buff[3];
            WORD len = buff[4]<<8|buff[5];
            int dataPos = 7;
            int ord = m_PrintSetting->getDataOrd();
            //���Ĵ���д��
            if (buff[1] == 6)
            {
                dataPos = 4;
                if (yt == 1)
                    len = 1;
                else
                    len = 2;
            }

            CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
            CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();
            for (i=0; i<len && len<0x7b; i++)
            {
                float fval = 0;
                if (yt == 1)
                {
                    fval = (short)(buff[dataPos]<<8|buff[dataPos+1]);
                    dataPos += 2;
                }
                else
                {
                    union
                    {
                        int ndata;
                        float fdata;
                        unsigned char ch[4];
                    }data;
                    if (ord == 1)
                    {
                        data.ch[1] = buff[dataPos++];
                        data.ch[0] = buff[dataPos++];
                        data.ch[3] = buff[dataPos++];
                        data.ch[2] = buff[dataPos++];
                    }
                    else
                    {
                        data.ch[3] = buff[dataPos++];
                        data.ch[2] = buff[dataPos++];
                        data.ch[1] = buff[dataPos++];
                        data.ch[0] = buff[dataPos++];
                    }
                    if (yt == 2)
                        fval = (float)data.ndata;
                    else
                        fval = data.fdata;
                }
                /*if (m_PrintSetting->getPrintTranslate())
                {*/
                    sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"�յ�ң������:%d ��ַ:%d ����:%f",(int)buff[1],(int)(addr+i),fval);
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
              //  }

                bool isExit = false;
                SCADA_SetPoint curSetPoint;
                int nRelayRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_SetPoint_ID);
                for (int j=0; j<nRelayRecordCount; j++)
                {
                    dbmg_sdb->GetElementByIndex(SCADA_TAB_SetPoint_ID, j,&curSetPoint);
                    if (m_PrintSetting->getChannelID() == (int)curSetPoint.uTuID && (addr+i+1) == curSetPoint.iNoInDPU)
                    {
                        isExit = true;
                        break;
                    }
                }

                //��Ӧ�㲻����
                if (!isExit || curSetPoint.ulYCID32 < 0)
                {
                    sendbuf[1] |= 0x80;
                    sendbuf[2] = 0x01;
                    nPos = 3;
                    break;
                }
                //д��ң���Ӧ��ֵ
                SCADA_Analog tempAnalog;
                if (dbmg_sdb->GetElementByID(SCADA_TAB_Analog_ID,curSetPoint.ulYCID32,&tempAnalog))
                {
                    Q_RECORDDATATAG dataArray;
                    memset(&dataArray,0,sizeof(dataArray));
                    dataArray.eDataType=eDataTypeFloat;
                    dataArray.channelID=tempAnalog.uTuID;
                    dataArray.nIndex=tempAnalog.iIndex;
                    dataArray.fValue=fval;
                    m_pMgr->recordDataValue(&dataArray,1,true);
                }
                //д��ң��ʱ��
                SYSTEMTIME time;
                GetLocalTime(time);
                curSetPoint.selecttime = time;
                curSetPoint.exectime = time;
                curSetPoint.iexecret = 1;
                curSetPoint.fVal = fval;
                dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,curSetPoint.ID32,&curSetPoint);
                //ת����Ϣ
                if(curSetPoint.iZF)
                {
                    RSetpoint rs;
                    rs.iStation =curSetPoint.izfTuID;
                    rs.iPoint =curSetPoint.izfNoInDPU;
                    rs.iCommandType = MES_TYPE_SPOINT;
                    rs.fVal = fval;
                    rs.iDevice=curSetPoint.izfDPUID;
                    rs.iZf=1;
                    m_rtdbInf->getMainDb()->SendMessage(curSetPoint.izfProcessID, mes_SPExecute, sizeof(RSetpoint), (char *)&rs);
                }
                //������Ӧ�ļĴ���
                if (yt != 1)
                    i++;
            }
            //�Ĵ�����������
            if (len <= 0 || len >= 0x7b)
            {
                sendbuf[1] |= 0x80;
                sendbuf[2] = 0x01;
                nPos = 3;
            }
            else if(i == len)
            {
                sendbuf[2] = buff[2];
                sendbuf[3] = buff[3];
                if (buff[1] == 16 || yt == 1)
                {
                    sendbuf[4] = buff[4];
                    sendbuf[5] = buff[5];
                    nPos = 6;
                }
                else
                {
                    sendbuf[4] = buff[4];
                    sendbuf[5] = buff[5];
                    sendbuf[6] = buff[6];
                    sendbuf[7] = buff[7];
                    nPos = 8;
                }
            }
            WORD crc = CRC16(sendbuf,nPos);
            sendbuf[nPos++] = crc & 0xff;
            sendbuf[nPos++] = (crc>>8) & 0xff;
        }
        else
        {
            sendbuf[1] |= 0x80;
            sendbuf[2] = 0x02;
            nPos = 3;
        }

        //if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)sendbuf,nPos);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)sendbuf,nPos);
        return true;
    }
	virtual bool makeFrame() { return true; }
	virtual bool terminateProtocol() { m_PrintSetting=NULL;return true; }
	virtual bool startProcess()
    {
		TRACE("startProcess");
        m_eProtocolStatus = eCommonStatusNormal;
        m_PrintSetting = (Q_protcolModbusRtuSlvSetting *)m_pProcotolConfig->getProtocolSetting();
        if(m_PrintSetting!=NULL)
        {
            //�¹�Լ��ӡ
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"��ʹ����ʼ...");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,m_LastMsg,strlen(m_LastMsg)+1);
        }
        return true;
    }
	virtual bool stopProcess()
    {
        if(m_PrintSetting!=NULL)
        {
            //�¹�Լ��ӡ
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"ͨ���ر�...");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,m_LastMsg,strlen(m_LastMsg)+1);
            m_pProcotolConfig->getReceiveBuffer()->clearBuffer();
        }
        m_eProtocolStatus = eCommonStatusNull;
        return true;
    }

private:
    void SendMsgPrint(int nchannelid, int nmsgtype, const char* mse, int nlen)
	{
    
		TRACE("Q_protocolModbusRtuSlv::MsgType %d:%s",nmsgtype,mse);
        bool bSend = false;
        SCADA_TUnit ElementTUnit;

        CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
        m_rtdbInf->getScadaDb()->GetElementByID(SCADA_TAB_TUnit_ID,nchannelid,&ElementTUnit);
        if (ElementTUnit.iRecordFlag)
            bSend = true;
        else
            return;
        //������Ϣ��������Ϣ
        if (nmsgtype == eMsgTypeTx || nmsgtype == eMsgTypeRx)
        {
            //������Ϣ���俪��
            if (ElementTUnit.iRecPacketFlg)
                bSend = true;
             else
                return;
        }
        //��ʾ��Ϣ����Ϣֵ��������Ϣ,���ݽ��͡���Լ����
        else if (nmsgtype == eMsgTypeMsg || nmsgtype == eMsgTypeError || nmsgtype == eMsgTypeCh)
        {
            //��ʾ��Ϣ���俪��
            if (ElementTUnit.iRecPromptFlg)
                bSend = true;
            else
                return;
        }

        if (!bSend || nlen == 0)
            return;

        int sendcs = (nlen+Q_MSGTRAN_MAXMSGLEN-1)/Q_MSGTRAN_MAXMSGLEN;
		
		SYSTEM_Process *process= m_rtdbInf->getMainDb()->FindProcessByProType(pt_DBGSVR);
		if(process==NULL)
		{
			TRACE("process DBGSVR not exist in did sendMsgPrint exit\n");
			return ;
		}
		else
		{
			if((process->iFlag&ps_Run)!=ps_Run)
			{
				//���̲����У��˳�
				TRACE("process DBGSVR not run sendMsgPrint exit\n");
				return ;
			}
		}


        for (int n=0; n<sendcs; n++)
        {
            MesMsgPrint MsgPrint = {{0}};
            MsgPrint.ichannelid=nchannelid;
            //�ְ�����
            nmsgtype = nmsgtype;
            if (sendcs > 1)
                nmsgtype |= 0x0080;
            MsgPrint.msgtype=nmsgtype;
            //��������
            MsgPrint.nlen = ((n+1)==sendcs)?(nlen-n*Q_MSGTRAN_MAXMSGLEN):(Q_MSGTRAN_MAXMSGLEN);
            memcpy(MsgPrint.msg,mse+n*Q_MSGTRAN_MAXMSGLEN,MsgPrint.nlen);
           // m_rtdbInf->getMainDb()->SendMessage(pt_DBGSVR,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
			//2016-10-28 tjs ��Ϊ��Ӧ�ø�����������Ϣ
		

			m_rtdbInf->getMainDb()->SendMessageTo(process->ID32,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
        }
    }

    //ִ��ʱ��
    int m_TimeOut;
    time_t m_LastTime;
    //���ö���
    Q_protcolModbusRtuSlvSetting* m_PrintSetting;
    //��Ϣ����
    char m_LastMsg[Q_FRAME_CONST256];
};


#ifdef UNIX_ENV
#ifdef __cplusplus
extern "C" {
#endif
#endif //OS_UNIX

int  mod_init(int dbg_level)
{
	return 0;
}	

Q_chDownReturn* OUTPUTAPI create_protocol_object(Q_chDownCommPara *v_param)
{
    //��������
	TRACE("create_protocol_object Q_protcolModbusRtuSlv");
    Q_commonBuffer* bufferReceive = new Q_commonBuffer();
    Q_commonBuffer* bufferSend = new Q_commonBuffer();
    Q_protocolCommandManagerBase* pCommandList = new Q_protocolModbusRtuSlvCommandMgr();

    //ͨ������
    Q_serialLinkSetting* linkSetting = new Q_serialLinkSetting();

    Q_linkConfig* linkConfig = new Q_linkConfig();
    linkConfig->setLinkSetting(linkSetting);
    linkConfig->setReceiveBuffer(bufferReceive);
    linkConfig->setSendBuffer(bufferSend);

    //ͨ������
    Q_serialLink* linkTest = new Q_serialLink();

    //��Լ����
    Q_protcolModbusRtuSlvSetting *protocolSetting = new Q_protcolModbusRtuSlvSetting();
    protocolSetting->setChannelType(v_param->cfg->type);
    protocolSetting->setChannelID(v_param->cfg->id);
    protocolSetting->setFilePath(v_param->strFilePath);
    protocolSetting->initSetting();

    //do 
  //  {
	TRACE("create_protocol_object Q_protcolModbusRtuSlv1");
        const char *cfg = NULL;
        cfg = protocolSetting->getContentID(11,1);
        if(cfg!=NULL)
        {
            printf("Serial port name :%s\n", cfg);
            linkSetting->setComPortName((char *)cfg);
        }
        cfg = protocolSetting->getContentID(12,1);
        if(cfg!=NULL)
        {
            printf("Serial BountRate :%s\n", cfg);
            linkSetting->setBountRate(atoi(cfg));
        }	
        cfg = protocolSetting->getContentID(13,1);
        if(cfg!=NULL)
        {
            printf("Serial DataBit :%s\n", cfg);
            linkSetting->setDataBit(atoi(cfg));
        }
        cfg = protocolSetting->getContentID(14,1);
        if(cfg!=NULL)
        {
            printf("Serial StopBit :%s\n", cfg);
            linkSetting->setStopBit(atoi(cfg));
        }
        cfg = protocolSetting->getContentID(15,1);
        if(cfg!=NULL)
        {
            printf("Serial VerifyBit :%s\n", cfg);
            linkSetting->setVerifyBit(atoi(cfg));
        }
		TRACE("create_protocol_object Q_protcolModbusRtuSlv2");
    //} while (0);

    Q_protocolConfig* protocolConfig = new Q_protocolConfig();
    protocolConfig->setProtocolSetting(protocolSetting);
    protocolConfig->setReceiveBuffer(bufferReceive);
    protocolConfig->setSendBuffer(bufferSend);
    protocolConfig->setProtocolCommandManager(pCommandList);

    //��Լ����
    Q_protocolModbusRtuSlv* protocolTest = new Q_protocolModbusRtuSlv();
    protocolTest->setRtdbInf(v_param->m_pMgr);
    protocolTest->setSendMsg(0);

    //ͨ��
    Q_channelBase* channelTest = new Q_channelBase();
    channelTest->setChannelID(v_param->cfg->id);
    channelTest->setPrimaryFlag(true);
    channelTest->setReceiveBuffer(bufferReceive);
    channelTest->setSendBuffer(bufferSend);
    channelTest->setLinkSetting(linkSetting);
    channelTest->setLinkConfig(linkConfig);
    channelTest->setLink(linkTest);
    channelTest->setProtocolSetting(protocolSetting);
    channelTest->setProtocolConfig(protocolConfig);
    channelTest->setProtocol(protocolTest);	
    channelTest->setCommandList(pCommandList);

    //����
    Q_taskBase* taskTest = new Q_taskBase();
    taskTest->setBackupMode(eCommonBackupCold);
    taskTest->setRtdbInf(v_param->m_pMgr);
    taskTest->addChannel(channelTest);

    //ͨ��
    Q_simpleThread* threadTest = new Q_simpleThread();
    threadTest->setThreadPeriod(500);
    threadTest->addTask(taskTest);

    //�豸
    Q_taskRunningUnit* unitTest = new Q_taskRunningUnit();
    unitTest->setDriverMode(eCommonModeSingle);
    unitTest->addTask(taskTest);
    unitTest->addThread(threadTest);

    //����ֵ
    Q_chDownReturn *chRet=new Q_chDownReturn();
    chRet->unit=unitTest;
    sprintf_s(chRet->protocol,sizeof(v_param->cfg->protocol),"%s",v_param->cfg->protocol);
    chRet->task=taskTest;
    chRet->channelID=v_param->cfg->id;
    chRet->enabled=v_param->cfg->enabled;
    return chRet;
}

//Ϊ�˱�֤���ٵĶ�̬�����ͷſ��һ�£����Ӵ˷���
void OUTPUTAPI free_protocol_object(Q_chDownReturn* pProtocol)
{
	if (pProtocol)
	{
		delete (Q_chDownReturn*)pProtocol;
		pProtocol = NULL;
	}
}

// ң����ң��
int OUTPUTAPI add_command(Q_taskBase *task,LPQ_RECORDDATATAG rec)
{
    return 0;
}
//��ӡ���¼�¼
int  OUTPUTAPI print_change_log()
{
	printf("=== CommModbusTcpServerStand Protocol Dynamic lib====\n");
	printf("=== 2016-07-22====\n");
	printf("1�����Ӵ�ӡ����汾����\n");
	printf("=== 2016-09-10====\n");
	printf("1�����ӵ�����Ϣ\n");
	printf("=== 2016-09-11====\n");
	printf("1����Ϊ���͵����ݶ�Ϊ��\n");
	printf("=== 2016-10-28====\n");
	printf("1�����ķ�����Ϣ������ȥ��ȫ��ͬ��������Ϣ�Ĺ��ܣ�ֻ�������ͼ���\n");
	printf("=== 2016-11-25====\n");
	printf("1�������Ż�������Ϣ�����ݰѹ̶�ֵ�ĳ�strlen()+1�ķ�ʽ\n");
	return 1;
}

#ifdef UNIX_ENV
#ifdef __cplusplus
}
#endif
#endif //OS_UNIX
