//windows版本定义
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

Q_dataManager *m_pMgrModbusSlaveProtocolSetting; /*ModbusSlave的rtdb全局变量*/
typedef enum _Q_ModXmlDataType
{
    eModXmlDataTypeNull=0,//未知
    eModXmlDataTypeInt=1,
    eModXmlDataTypeFloat=2,
    eModXmlDataTypeEnum=3,
    eModXmlDataTypeString=4
}Q_ModXmlDataType;
typedef struct ST_ModCfgT
{
    int group; //分组
    int id;    //编号
    Q_ModXmlDataType type;
    std::string name;  //配置名
    std::string value; //配置值
}ST_ModCfgT;

//配置类对象
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
            sprintf_s(LastError,sizeof(LastError)-1,"解析配置文件出错，当前文件:%s，当前行号:%d\n",__FILE__,__LINE__);
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
            sprintf_s(LastError,sizeof(LastError)-1,"解析配置文件出错,当前行号:%d\n",__LINE__);
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
    //文件路径
    std::string m_sFilePath;
    //内容
    std::vector<ST_ModCfgT> m_vContent;
    //错误信息
    char LastError[Q_FRAME_CONST1024];
};

//命令类对象
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

//规约实现类
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

        //发送报文
        int nLength = m_pProcotolConfig->getReceiveBuffer()->getDataLength();
     /*   if (nLength && m_PrintSetting && m_PrintSetting->getPrintPacket())
        {*/
            int nByteCount = m_pProcotolConfig->getReceiveBuffer()->readData(bufferReceive,Q_FRAME_DEFAULT_RECEIVEBUFFERSIZE,false);
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeRx,bufferReceive,nByteCount);
      //  }
        //处理数据
        while(nLength >= 8)
        {
            int nRes = 0;
            int nByteCount = m_pProcotolConfig->getReceiveBuffer()->readData(bufferReceive,Q_FRAME_DEFAULT_RECEIVEBUFFERSIZE,false);
            if(nByteCount >= 8)
                explainDataEx((unsigned char *)bufferReceive,nByteCount,nRes);
            //长度不够
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
			TRACE("Q_protocolModbusRtuSlv::设备地址不相等\n");
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
				sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"crc校验失败");
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
            //单字节写入支持浮点，不太规范
            if (buff[1] == 6)
            {
                if (yt == 1)
                    nByteLen = 8;
                else
                    nByteLen = 10;
            }
            //长度不够
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
                sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"读取YX:%d 地址:%d 长度:%d\n",(int)buff[1],(int)addr,(int)len);
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
				dataArray.val.no=idx+addr;//遥信不需要加1
                dataArray.val.eDataType=eDataTypeBit;
	
			
                if(!m_pMgr->evalDataValue(&dataArray,1))
                {
                    //新规约打印
                    sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"Can't find right YX point in realDB,Channel=%d,device=%d,addr=%d",m_PrintSetting->getChannelID(),(int)buff[1],(int)(i+addr+1));
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,m_LastMsg,strlen(m_LastMsg)+1);
                }
					TRACE("get yx data =%d,ch=%d,device=%d,no=%d\n",dataArray.val.bValue==true?1:0,dataArray.val.channelID,dataArray.val.device,dataArray.val.no);
                //保存
                if ((i%8 == 0) && (i != 0))
                {
                    sendbuf[nPos++] = data & 0xff;
                    data = 0;
                }
                //赋值
                if (dataArray.val.nValue)
                {
                    data |= 1<<(i%8);
                }
				idx++;
            }
            //寄存器个数错误
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
                sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"读取YC:%d 地址:%d 长度:%d\n",(int)buff[1],(int)addr,(int)len);
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
                    //新规约打印
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
            //寄存器个数错误
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
                sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"收到遥控命令:%d 地址:%d 代码:%x",(int)buff[1],(int)addr,(unsigned int)val);
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

            //对应点不存在
            if (!isExit || curRelay.ulYXID32 < 0)
            {
                sendbuf[1] |= 0x80;
                sendbuf[2] = 0x01;
                nPos = 3;
            }
            else
            {
                //写入遥信对应的值
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
                //写入遥控时间
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
                //转发消息
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
            //单寄存器写入
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
                    sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"收到遥调命令:%d 地址:%d 数据:%f",(int)buff[1],(int)(addr+i),fval);
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

                //对应点不存在
                if (!isExit || curSetPoint.ulYCID32 < 0)
                {
                    sendbuf[1] |= 0x80;
                    sendbuf[2] = 0x01;
                    nPos = 3;
                    break;
                }
                //写入遥测对应的值
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
                //写入遥控时间
                SYSTEMTIME time;
                GetLocalTime(time);
                curSetPoint.selecttime = time;
                curSetPoint.exectime = time;
                curSetPoint.iexecret = 1;
                curSetPoint.fVal = fval;
                dbmg_sdb->SetElementByID(SCADA_TAB_SetPoint_ID,curSetPoint.ID32,&curSetPoint);
                //转发消息
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
                //跳过相应的寄存器
                if (yt != 1)
                    i++;
            }
            //寄存器个数错误
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
            //新规约打印
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"初使化开始...");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,m_LastMsg,strlen(m_LastMsg)+1);
        }
        return true;
    }
	virtual bool stopProcess()
    {
        if(m_PrintSetting!=NULL)
        {
            //新规约打印
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"通道关闭...");
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
        //发送消息、接收消息
        if (nmsgtype == eMsgTypeTx || nmsgtype == eMsgTypeRx)
        {
            //报文信息传输开启
            if (ElementTUnit.iRecPacketFlg)
                bSend = true;
             else
                return;
        }
        //提示信息、信息值、错误消息,数据解释、规约解析
        else if (nmsgtype == eMsgTypeMsg || nmsgtype == eMsgTypeError || nmsgtype == eMsgTypeCh)
        {
            //提示信息传输开启
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
				//进程不运行，退出
				TRACE("process DBGSVR not run sendMsgPrint exit\n");
				return ;
			}
		}


        for (int n=0; n<sendcs; n++)
        {
            MesMsgPrint MsgPrint = {{0}};
            MsgPrint.ichannelid=nchannelid;
            //分包发送
            nmsgtype = nmsgtype;
            if (sendcs > 1)
                nmsgtype |= 0x0080;
            MsgPrint.msgtype=nmsgtype;
            //复制内容
            MsgPrint.nlen = ((n+1)==sendcs)?(nlen-n*Q_MSGTRAN_MAXMSGLEN):(Q_MSGTRAN_MAXMSGLEN);
            memcpy(MsgPrint.msg,mse+n*Q_MSGTRAN_MAXMSGLEN,MsgPrint.nlen);
           // m_rtdbInf->getMainDb()->SendMessage(pt_DBGSVR,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
			//2016-10-28 tjs 因为不应该给备都发送消息
		

			m_rtdbInf->getMainDb()->SendMessageTo(process->ID32,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);
        }
    }

    //执行时间
    int m_TimeOut;
    time_t m_LastTime;
    //配置对象
    Q_protcolModbusRtuSlvSetting* m_PrintSetting;
    //消息内容
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
    //下置命令
	TRACE("create_protocol_object Q_protcolModbusRtuSlv");
    Q_commonBuffer* bufferReceive = new Q_commonBuffer();
    Q_commonBuffer* bufferSend = new Q_commonBuffer();
    Q_protocolCommandManagerBase* pCommandList = new Q_protocolModbusRtuSlvCommandMgr();

    //通道配置
    Q_serialLinkSetting* linkSetting = new Q_serialLinkSetting();

    Q_linkConfig* linkConfig = new Q_linkConfig();
    linkConfig->setLinkSetting(linkSetting);
    linkConfig->setReceiveBuffer(bufferReceive);
    linkConfig->setSendBuffer(bufferSend);

    //通道对象
    Q_serialLink* linkTest = new Q_serialLink();

    //规约配置
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

    //规约对象
    Q_protocolModbusRtuSlv* protocolTest = new Q_protocolModbusRtuSlv();
    protocolTest->setRtdbInf(v_param->m_pMgr);
    protocolTest->setSendMsg(0);

    //通道
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

    //任务
    Q_taskBase* taskTest = new Q_taskBase();
    taskTest->setBackupMode(eCommonBackupCold);
    taskTest->setRtdbInf(v_param->m_pMgr);
    taskTest->addChannel(channelTest);

    //通道
    Q_simpleThread* threadTest = new Q_simpleThread();
    threadTest->setThreadPeriod(500);
    threadTest->addTask(taskTest);

    //设备
    Q_taskRunningUnit* unitTest = new Q_taskRunningUnit();
    unitTest->setDriverMode(eCommonModeSingle);
    unitTest->addTask(taskTest);
    unitTest->addThread(threadTest);

    //返回值
    Q_chDownReturn *chRet=new Q_chDownReturn();
    chRet->unit=unitTest;
    sprintf_s(chRet->protocol,sizeof(v_param->cfg->protocol),"%s",v_param->cfg->protocol);
    chRet->task=taskTest;
    chRet->channelID=v_param->cfg->id;
    chRet->enabled=v_param->cfg->enabled;
    return chRet;
}

//为了保证开辟的动态库与释放库的一致，增加此方法
void OUTPUTAPI free_protocol_object(Q_chDownReturn* pProtocol)
{
	if (pProtocol)
	{
		delete (Q_chDownReturn*)pProtocol;
		pProtocol = NULL;
	}
}

// 遥调、遥控
int OUTPUTAPI add_command(Q_taskBase *task,LPQ_RECORDDATATAG rec)
{
    return 0;
}
//打印更新记录
int  OUTPUTAPI print_change_log()
{
	printf("=== CommModbusTcpServerStand Protocol Dynamic lib====\n");
	printf("=== 2016-07-22====\n");
	printf("1、增加打印输入版本功能\n");
	printf("=== 2016-09-10====\n");
	printf("1、增加调试信息\n");
	printf("=== 2016-09-11====\n");
	printf("1、因为发送的数据都为零\n");
	printf("=== 2016-10-28====\n");
	printf("1、更改发送消息方法，去掉全网同步报文消息的功能，只本机发送即可\n");
	printf("=== 2016-11-25====\n");
	printf("1、更改优化发送消息的内容把固定值改成strlen()+1的方式\n");
	return 1;
}

#ifdef UNIX_ENV
#ifdef __cplusplus
}
#endif
#endif //OS_UNIX
