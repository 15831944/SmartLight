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
#include <math.h>
using namespace std;

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

typedef struct ST_YCDATA
{
    INT lDPUID;
    INT iNoInDPU;
    FLOAT fVal;
}ST_YCDATA;
typedef struct ST_YXDATA
{
    INT lDPUID;
    INT iNoInDPU;
    INT iVal;
}ST_YXDATA;

//配置类对象
class Q_protcol101SlaveSetting : public Q_protocolSettingBase
{
public:
    Q_protcol101SlaveSetting(void) { memset(LastError,0,sizeof(LastError)); }
    virtual ~Q_protcol101SlaveSetting(void) {}
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
    int getYxType()
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
class Q_protocol101SlaveCommandMgr : public Q_protocolCommandManagerBase
{
public:
    Q_protocol101SlaveCommandMgr(void) {}
    virtual ~Q_protocol101SlaveCommandMgr(void) {}

};
class Q_protocol101SlaveCommand : public Q_protocolCommandBase
{
public:
	Q_protocol101SlaveCommand(void) { memset(&msg,0,sizeof(msg)); }
	virtual ~Q_protocol101SlaveCommand(void) {}

    Q_RECORDDATATAG msg;
};

// 主站 到 子站
#define RESET_REMOTE_LINK   0   //复位远方链路
#define CALL_LINK_STATE     9   //召唤链路状态
#define CALL_TWOCLASS_DATA  11  //召唤二级数据
#define CALL_ONECLASS_DATA  10  //召唤一级数据
#define YK_FLAG				46  //双点YK
#define YT_FLAG				47  //YT
#define SET_VALUE			48  //设定
#define GET_VALUE			52
#define CALL_ALLDATA		100 //总召唤命令
#define CALL_DD				101 //召唤DD
#define READ_DATA			102 //读数据
#define MODIFY_TIME			103 //时钟同步
#define TEST_RTU			104 //测试
#define RESET_RTU			105 //复位进程命令
#define LOAD_PARA			110 //装载参数
#define ACTIVE_PARA			113 //激活参数

//子站 到 主站
#define SPOINT_YX			1//不带Soe的单点YX   不带时标的单点信息
#define SPOINT_YX_SOE		2//带Soe的单点YX
#define DPOINT_YX			3//不带Soe的双点YX
#define DPOINT_YX_SOE		4//带Soe的单点YX
#define BIANYAQI_INFO		5//变压器分接头信息
#define BIANYAQI_INFO_TIME  6//带时标的变压器分接头信息
#define RTU_INFO			7//RTU状态信息
#define YC_INFO				9//YC
#define YC_INFO_TIME 		10//带时标的YC
#define YC_INFO_FLOAT 		13//浮点型YC
#define DD_INFO				15//电度量
#define DD_INFO_TIME		16//带时标的DD
#define SPOINT_BH_EVENT		17//带Soe的单点YX
#define SPOINT_YX_CHECK		20//具有状态变位检查的成组单点YX
#define YC_WITHOUT_QDS		21//不带品质描述的测量值
#define INIT_END			70//初始化结束
#define CALL_ALL_DATA_END   200 //总召唤结束响应

//传送原因(COT)--CONTROLLER
#define SPONT               3//突发
#define REQ                 5//请求或被请求
#define ACT                 6//激活
#define ACTCON              7//激活确认
#define DEACT               8//停止激活
#define DEACTCON            9//停止激活确认
#define ACTTERM             10//激活结束
#define INTROGEN            20//响应总召唤

#define START_FLAG_1		0x68
#define START_FLAG_2		0x10
#define END_FLAG			0x16
#define	ONE_BYTE_FLAG		0xE5

#define YKFUNC_PRESET			0x01
#define YKFUNC_CONFIRM			0x02
#define YKFUNC_CANCEL			0x03
#define YKFUNC_EXECUTE			0x04
#define YKFUNC_EXECUTEAGAIN		0x05
#define YTFUNC_PRESET			0x06
#define YTFUNC_EXECUTE			0x07

#define SETVALFUNC_PRESET			0xc8
#define SETVALFUNC_EXECUTE			0xc9

typedef unsigned char byte;
typedef unsigned short	uint16;
typedef unsigned int	uint32;
struct	IEC101_STRUCT		
{
    byte Linkaddr;//地址
    byte down_report_type;	//最近一次下发的报文的类型，在收到的报文无法确认类型时使用，组装完一帧应用层报文时设置
    byte Fun;  //链路状态或功能码	
    byte FCB;  //帧计数位	
    byte FCV;  //帧计数有效位
    byte ACD;  //要求访问位,是否有一级数据
    byte PreFCB;	
    byte PreSendBuff[1024];
    int  PreBuffLen;

    byte YKFlag;
    uint16 ykinforAddr;
    float ytdata;

    byte CallAll;
    byte SendUpAllDataType;//当前需要上送的二级数据类型
    uint16 SendUpAllDataPtr;//当前需要上送的二级数据的偏移
    uint16 SendUpYXDataPtr;
    uint16 SendUpYCDataPtr;
};

//校验和检查
byte CalculateCheck( byte *packet_ptr ,int num)
{
    byte temp = 0;
    for(int i=0; i < num ; i++)
        temp += packet_ptr[i];
    return(temp);
}

//规约实现类
class Q_protocol101Slave : public Q_protocolBase
{
public:
	Q_protocol101Slave(void)
    {
        m_PrintSetting = NULL;
        m_LastTime = 0;
        m_TimeOut = 10;
        memset(m_LastMsg,0,sizeof(m_LastMsg));
        memset(&m_IEC101Infor,0,sizeof(m_IEC101Infor));
        m_IEC101Infor.PreFCB = 0xff;
    }
	virtual ~Q_protocol101Slave(void)
    {
    }

public:
	virtual bool initializeProtocol() { return true; }
	virtual bool explainData()
    {
        char bufferReceive[Q_FRAME_CONST4096];	
        memset(bufferReceive,0,sizeof(bufferReceive));

        //发送报文
        int nLength = m_pProcotolConfig->getReceiveBuffer()->getDataLength();
        if (nLength && m_PrintSetting && m_PrintSetting->getPrintPacket())
        {
            int nByteCount = m_pProcotolConfig->getReceiveBuffer()->readData(bufferReceive,Q_FRAME_DEFAULT_RECEIVEBUFFERSIZE,false);
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeRx,bufferReceive,nByteCount);
        }
        //处理数据
        while(nLength >= 5)
        {
            int nRes = 0;
            int nByteCount = m_pProcotolConfig->getReceiveBuffer()->readData(bufferReceive,Q_FRAME_DEFAULT_RECEIVEBUFFERSIZE,false);
            if(nByteCount >= 5)
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
        nCurrentPos = 1;
        if (m_PrintSetting == NULL)
            return false;
        //同步头
        unsigned char addr = 0;
        if (buff[0] == 0x10)
        {
            unsigned char check = CalculateCheck(buff+1,2);
            if (check != buff[3])
                return false;
            addr = buff[2];
            nCurrentPos = 5;
        }
        else if(buff[0] == 0x68 && buff[3] == 0x68)
        {
            //长度不足
            unsigned char count = buff[1];
            if (count+6 > nCount)
            {
                nCurrentPos = 0;
                return false;
            }
            unsigned char check = CalculateCheck(buff+4,count);
            if (check != buff[4+count])
                return false;
            addr = buff[5];
            nCurrentPos = 6+count;
        }
        if (addr != m_IEC101Infor.Linkaddr)
            return false;

        //处理短帧
        if (buff[0] == 0x10)
            FixPacketProc(buff);
        //处理长帧
        else
            PacketProc(&buff[4]);
        //发送报文
        MasterSend();
        return true;
    }
    //固定桢长报文处理
    void FixPacketProc(byte *packet_ptr)
    {
        //链路状态或功能码
        m_IEC101Infor.Fun = (*(packet_ptr+1))&0x0f;

        //帧计数有效位
        m_IEC101Infor.FCV = (*(packet_ptr+1))&0x10;

        //帧计数位
        if(m_IEC101Infor.FCV)
            m_IEC101Infor.FCB = (*(packet_ptr+1))&0x20;

        //复位远方链路  0
        if(m_IEC101Infor.Fun == RESET_REMOTE_LINK )
        {
            m_IEC101Infor.down_report_type = RESET_REMOTE_LINK;
            return;
        }
        //召唤链路状态  9   
        if(m_IEC101Infor.Fun == CALL_LINK_STATE)
        {
            m_IEC101Infor.down_report_type = CALL_LINK_STATE;
            return;
        }
        //召唤二级数据  11
        if(m_IEC101Infor.Fun == CALL_TWOCLASS_DATA)
        {
            m_IEC101Infor.down_report_type = CALL_TWOCLASS_DATA;
            return;
        }
        //召唤一级数据  10
        if(m_IEC101Infor.Fun == CALL_ONECLASS_DATA)
        {
            m_IEC101Infor.down_report_type = CALL_ONECLASS_DATA;
            return;
        }

        m_IEC101Infor.down_report_type = 255;
    }
    //处理一帧数据,解析类型标示,而后调用不同的函数处理
    void PacketProc(byte *packet_ptr)
    {
        byte packetType = (*(packet_ptr+2));
        byte send_reason = (*(packet_ptr+4));

        //链路状态或功能码
        m_IEC101Infor.Fun = (*(packet_ptr))&0x0f;

        //帧计数有效位
        m_IEC101Infor.FCV = (*(packet_ptr))&0x10;

        //帧计数位
        if(m_IEC101Infor.FCV)
            m_IEC101Infor.FCB = (*(packet_ptr))&0x20;

        //遥控命令
        if(packetType == 45 || packetType == YK_FLAG)
        {
           // processYK(packet_ptr,packetType);
            return;
        }
        //遥调命令
        else if(packetType == SET_VALUE || packetType == 50)
        {
            processYT(packet_ptr,packetType);
            return;
        }
        //总召唤命令
        else if(packetType == CALL_ALLDATA)
        {
            m_IEC101Infor.down_report_type = CALL_ALLDATA;
            m_IEC101Infor.CallAll = 1;
            m_IEC101Infor.SendUpAllDataType = SPOINT_YX;
            m_IEC101Infor.SendUpAllDataPtr = 0;
            return;
        }
        //对时命令
        else if(packetType == MODIFY_TIME)
        {
            processTime(packet_ptr);
            return;
        }

        m_IEC101Infor.down_report_type = 255;
    }

    //处理遥控
    int processYK(byte* pPacketBuff, int yktype)
    {
        byte setStatus = (*(pPacketBuff+8) & 0x03);
        byte reqCot = *(pPacketBuff+4);
        m_IEC101Infor.ykinforAddr = *(pPacketBuff+6) + (*(pPacketBuff+7)) * 256;
        if (reqCot != 6)
            return 0;
        //判断数据类型，确定分合操作
        if (yktype == 45)
        {
            if (setStatus != 0 && setStatus != 1)
                return 0;
            m_IEC101Infor.YKFlag = setStatus;
        }
        else
        {
            if(setStatus == 1) //分
                setStatus = 0;
            else if(setStatus == 2) //合
                setStatus = 1;
            else
                return 0;
            m_IEC101Infor.YKFlag = 10 + setStatus;
        }
        CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
        CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

        bool isExit = false;
        SCADA_Relay curRelay;
        int nRelayRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_Relay_ID);
        for (int j=0; j<nRelayRecordCount; j++)
        {
            dbmg_sdb->GetElementByIndex(SCADA_TAB_Relay_ID, j,&curRelay);
            if (m_PrintSetting->getChannelID() == (int)curRelay.uTuID && m_IEC101Infor.ykinforAddr == curRelay.iNoInDPU)
            {
                isExit = true;
                break;
            }
        }

        //对应点不存在
        if (!isExit || curRelay.ulYXID32 < 0)
            return 0;
        
        //写入遥信对应的值
        if((*(pPacketBuff+8) & 0x80) == 0)
        {
            SCADA_Digit tempDigit;
            if (dbmg_sdb->GetElementByID(SCADA_TAB_Digit_ID,curRelay.ulYXID32,&tempDigit))
            {
                Q_RECORDDATATAG dataArray;
                memset(&dataArray,0,sizeof(dataArray));
                dataArray.eDataType=eDataTypeBit;
                dataArray.channelID=tempDigit.uTuID;
                dataArray.nIndex=tempDigit.iIndex;
                dataArray.bValue=(setStatus!=0);
                m_pMgr->recordDataValue(&dataArray,1,true);
            }
            //写入遥控时间
            SYSTEMTIME time;
            GetLocalTime(time);
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
            m_IEC101Infor.down_report_type = YKFUNC_EXECUTE;
        }
        else
        {
            //写入遥控时间
            SYSTEMTIME time;
            GetLocalTime(time);
            curRelay.selecttime = time;
            dbmg_sdb->SetElementByID(SCADA_TAB_Relay_ID,curRelay.ID32,&curRelay);
            //转发消息
            if(curRelay.iZF)
            {
                RControl rc;
                rc.iStation =curRelay.izfTuID;
                rc.iYKPoint =curRelay.izfNoInDPU;
                rc.iCommandType = mes_RCSelectOk;
                rc.iVal = setStatus;
                rc.iDevice=curRelay.izfDPUID;
                rc.iZf=1;
                m_rtdbInf->getMainDb()->SendMessage(curRelay.izfProcessID, mes_RCSelect, sizeof(RControl), (char *)&rc);
            }
            m_IEC101Infor.down_report_type = YKFUNC_PRESET;
        }
        return 1;
    }
    //处理遥调
    int processYT(byte* pPacketBuff, int yttype)
    {
        byte setStatus = 0;
        float fval = 0;
        if (yttype == 50)
        {
            memcpy(&fval,pPacketBuff+8,4);
            setStatus = *(pPacketBuff+12);
        }
        else
        {
            fval = (float)(*(pPacketBuff+8) + (*(pPacketBuff+9)) * 256);
            setStatus = *(pPacketBuff+10);
        }
        byte reqCot = *(pPacketBuff+4);
        m_IEC101Infor.ytdata = fval;
        m_IEC101Infor.ykinforAddr = *(pPacketBuff+6) + (*(pPacketBuff+7)) * 256;
        if (reqCot != 6)
            return 0;
        //判断数据类型，确定分合操作
        if (yttype == 50)
        {
            if (setStatus & 0x80)
                m_IEC101Infor.YKFlag = 1;
            else
                m_IEC101Infor.YKFlag = 0;
        }
        else
        {
            if (setStatus & 0x80)
                m_IEC101Infor.YKFlag = 11;
            else
                m_IEC101Infor.YKFlag = 10;
        }
        CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
        CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

        bool isExit = false;
        SCADA_SetPoint curSetPoint;
        int nRelayRecordCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_SetPoint_ID);
        for (int j=0; j<nRelayRecordCount; j++)
        {
            dbmg_sdb->GetElementByIndex(SCADA_TAB_SetPoint_ID, j,&curSetPoint);
            if (m_PrintSetting->getChannelID() == (int)curSetPoint.uTuID && m_IEC101Infor.ykinforAddr == curSetPoint.iNoInDPU)
            {
                isExit = true;
                break;
            }
        }

        //对应点不存在
        if (!isExit || curSetPoint.ulYCID32 < 0)
            return 0;

        //写入遥信对应的值
        if((setStatus & 0x80) == 0)
        {
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
            m_IEC101Infor.down_report_type = YTFUNC_EXECUTE;
        }
        else
        {
            //写入遥控时间
            SYSTEMTIME time;
            GetLocalTime(time);
            curSetPoint.selecttime = time;
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
                m_rtdbInf->getMainDb()->SendMessage(curSetPoint.izfProcessID, mes_SPSelect, sizeof(RSetpoint), (char *)&rs);
            }
            m_IEC101Infor.down_report_type = YTFUNC_PRESET;
        }
        return 1;
    }
    //处理对时
    int processTime(byte* pPacketBuff)
    {
        byte reqCot = *(pPacketBuff+4);
        if (reqCot != 6)
            return 0;

        SYSTEMTIME time;
        SYSTEMTIME *pSysTime = &time;
        GetLocalTime(time);
        pPacketBuff += 8;
        int milliseconds = pPacketBuff[1]*256+pPacketBuff[0];
        time.wSecond = milliseconds/1000;
        time.wMilliseconds = milliseconds%1000;
        time.wMinute = pPacketBuff[2] & 0x3F;
        time.wHour = pPacketBuff[3] & 0x1F;
        time.wDay = pPacketBuff[4] & 0x1F;
        time.wMonth = pPacketBuff[5] & 0xF;
        time.wYear = (pPacketBuff[6] & 0x7F) + 2000;
#ifdef WINDOWS_ENV
		SetLocalTime( pSysTime );
#else
        {
			char szCmd[64];
			char szMon[5];
			char szdate[30];

			switch (pSysTime->wMonth)
			{
			case 1:
				sprintf(szMon,"JAN");
				break;
			case 2:
				sprintf(szMon,"FEB");
				break;
			case 3:
				sprintf(szMon,"MAR");
				break;
			case 4:
				sprintf(szMon,"APR");
				break;
			case 5:
				sprintf(szMon,"MAY");
				break;
			case 6:
				sprintf(szMon,"JUN");
				break;
			case 7:
				sprintf(szMon,"JUL");
				break;
			case 8:
				sprintf(szMon,"AUG");
				break;
			case 9:
				sprintf(szMon,"SEP");
				break;
			case 10:
				sprintf(szMon,"OCT");
				break;
			case 11:
				sprintf(szMon,"NOV");
				break;
			case 12:
				sprintf(szMon,"DEC");
				break;
			default:
				sprintf(szMon,"DEC");
				break;
			}

			sprintf(szCmd, "date -s ");

			sprintf(szdate, "%d %s %04d %d:%d:%d#", 
				pSysTime->wDay, szMon, pSysTime->wYear,
				pSysTime->wHour, pSysTime->wMinute, pSysTime->wSecond);

			szCmd[8] = '"';

			for (size_t aaa = 0; aaa<sizeof(szdate); aaa++)
			{
				szCmd[9+aaa] = szdate[aaa];
			}

			for (size_t bbb = 0; bbb<sizeof(szCmd); bbb++)
			{
				if (szCmd[bbb] == '#')
				{	
					szCmd[bbb] = '"';
					break;
				}
			}	

			TRACE("CanbusForlinx::explainFrameTime  command set time : %s",szCmd);
			system(szCmd);
        }
#endif
        m_IEC101Infor.down_report_type = MODIFY_TIME;
        return 1;
    }

    //发送报文
    void MasterSend()
    {
        //判断是否需要重发上一帧
        if(m_IEC101Infor.FCV)
        {
            if(m_IEC101Infor.PreFCB == m_IEC101Infor.FCB) //需要重发上一帧
            {
                if (m_PrintSetting->getPrintTranslate())
                {
                    sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"FCB:%d<->%d重发报文",m_IEC101Infor.PreFCB,m_IEC101Infor.FCB);
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
                }
                if (m_PrintSetting->getPrintPacket())
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
                m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
                return;
            }
            else
            {
                m_IEC101Infor.PreFCB = m_IEC101Infor.FCB; //将桢计数保存
            }
        }

        //检查变化遥信，有变化遥信，将变化遥信发送,置发送一级数据标志
        m_IEC101Infor.ACD = 0;	
        for (size_t i=0; i<m_YxArr.size(); i++)
        {
            ST_YXDATA& data = m_YxArr[i];
            Q_RECORDDATATAG2 dataArray;
            memset(&dataArray,0,sizeof(dataArray));
            dataArray.val.channelID=m_PrintSetting->getChannelID();
            dataArray.val.device=data.lDPUID;
            dataArray.val.no=data.iNoInDPU;
            dataArray.val.eDataType=eDataTypeBit;
            m_pMgr->evalDataValue(&dataArray,1);
            if (dataArray.val.nValue != data.iVal)
            {
                m_IEC101Infor.ACD = 1;
                break;
            }
        }

        //复位远方链路通讯单元
        if(m_IEC101Infor.down_report_type == RESET_REMOTE_LINK)
        {
            ResetRemoteLink();
            return;
        }
        //查询链路状态
        if(m_IEC101Infor.down_report_type == CALL_LINK_STATE)
        {
            AnswerLinkStatus();
            return;
        }
        //请求一级用户数据
        if(m_IEC101Infor.down_report_type == CALL_ONECLASS_DATA)
        {
            AnswerOneClassData();
            return;
        }
        //请求二级用户数据
        if(m_IEC101Infor.down_report_type == CALL_TWOCLASS_DATA)
        {
            AnswerTwoClassData();
            return;
        }
        //遥控选择
        if(m_IEC101Infor.down_report_type == YKFUNC_PRESET || m_IEC101Infor.down_report_type == YKFUNC_EXECUTE)
        {
            AnswerYk(m_IEC101Infor.down_report_type);
            return;
        }
        //遥调选择
        if(m_IEC101Infor.down_report_type == YTFUNC_PRESET || m_IEC101Infor.down_report_type == YTFUNC_EXECUTE)
        {
            AnswerYt(m_IEC101Infor.down_report_type);
            return;
        }
        //总召唤
        if(m_IEC101Infor.down_report_type == CALL_ALLDATA)
        {
            RespondCallAllData();
            return;
        }
        //对时
        if(m_IEC101Infor.down_report_type == MODIFY_TIME)
        {
            AnswerTime();
            return;
        }
    }
    //复位远方链路通讯单元
    void ResetRemoteLink()
    {
        byte buff[10];
        int ptr = 0;

        buff[ptr++] = START_FLAG_2;
        buff[ptr++] = 0x80;
        buff[ptr++] = m_IEC101Infor.Linkaddr;
        byte calc = CalculateCheck( &buff[1] , 2 );
        buff[ptr++] = calc;
        buff[ptr++] = END_FLAG;

        m_IEC101Infor.PreFCB = 0xff;
        m_IEC101Infor.FCB = 0;

        memcpy(m_IEC101Infor.PreSendBuff,buff,ptr);
        m_IEC101Infor.PreBuffLen = ptr;

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"复位链路命令");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }
    //查询链路状态
    void AnswerLinkStatus()
    {
        byte buff[10];
        int ptr = 0;

        buff[ptr++] = START_FLAG_2;
        buff[ptr++] = 0x8b;
        buff[ptr++] = m_IEC101Infor.Linkaddr;
        byte byte = CalculateCheck( &buff[1] , 2 );
        buff[ptr++] = byte;
        buff[ptr++] = END_FLAG;

        memcpy(m_IEC101Infor.PreSendBuff,buff,ptr);
        m_IEC101Infor.PreBuffLen = ptr;

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"查询链路状态命令");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }
    //召唤一级数据
    void AnswerOneClassData()
    {
        if(m_IEC101Infor.CallAll)
        {
            if(m_IEC101Infor.CallAll == 1)
            {
                AnswerAllDataStar();
                m_IEC101Infor.CallAll = 2;
            }
            else if(m_IEC101Infor.CallAll == 2)
            {
                SendUpAllData();
            }
            else if(m_IEC101Infor.CallAll == 3)
            {
                AnswerAllDataEnd();
                m_IEC101Infor.CallAll = 0;  //全数据结束
            }
            return;
        }

        //检查变化遥信，有变化遥信，将变化遥信发送,置发送一级数据标志
        for (size_t i=0; i<m_YxArr.size(); i++)
        {
            ST_YXDATA& data = m_YxArr[i];
            Q_RECORDDATATAG2 dataArray;
            memset(&dataArray,0,sizeof(dataArray));
            dataArray.val.channelID=m_PrintSetting->getChannelID();
            dataArray.val.device=data.lDPUID;
            dataArray.val.no=data.iNoInDPU;
            dataArray.val.eDataType=eDataTypeBit;
            m_pMgr->evalDataValue(&dataArray,1);
            if (dataArray.val.nValue != data.iVal)
            {
                SendUpChangeYX(m_PrintSetting->getYxType());
                return;
            }
        }
        AnswerNouData();
    }
    //召唤二级数据
    void AnswerTwoClassData()
    {
        //检查变化遥信，有变化遥信，将变化遥信发送,置发送一级数据标志
        for (size_t i=0; i<m_YcArr.size(); i++)
        {
            ST_YCDATA& data = m_YcArr[i];
            Q_RECORDDATATAG2 dataArray;
            memset(&dataArray,0,sizeof(dataArray));
            dataArray.val.channelID=m_PrintSetting->getChannelID();
            dataArray.val.device=data.lDPUID;
            dataArray.val.no=data.iNoInDPU;
            dataArray.val.eDataType=eDataTypeFloat;
            m_pMgr->evalDataValue(&dataArray,1);
            if (fabs(dataArray.val.fValue-data.fVal) > 0.0001)
            {
                int yc_type = m_PrintSetting->getYcType();
                if(yc_type == YC_WITHOUT_QDS)
                    SendUpChangeYc();
                else
                    SendUpChangeYc_Float();
                return;
            }
        }
        AnswerNouData();
    }
    //响应总召唤,上传全数据
    void SendUpAllData()
    {
        //不带时标的单点信息	
        if(m_IEC101Infor.SendUpAllDataType == SPOINT_YX) 
        {
            SendUpYx(m_PrintSetting->getYxType());
            return;
        }
        //不带品质描述的测量值
        if(m_IEC101Infor.SendUpAllDataType == YC_WITHOUT_QDS) 
        {
            int yc_type = m_PrintSetting->getYcType();
            if(yc_type == YC_WITHOUT_QDS)
                SendUpYc();
            else
                SendUpYc_Float();
            return;
        }
    }
    //总召唤遥信
    void SendUpYx(int yx_type)
    {
        byte buff[512];
        byte *pbuff = buff;
        int i,ptr =0;

        pbuff[ptr++] = START_FLAG_1;
        //长度
        pbuff[ptr++] = 6;
        //长度
        pbuff[ptr++] = 6;

        pbuff[ptr++] = START_FLAG_1;

        //控制域
        //DIR = 1;  传输方向位
        //PRM = 0;  启动报文位
        //ACD = 0;  要求访问位
        //DFC = 0;  数据流控制位
        //RESPOND_DATA	
        pbuff[ptr++] = 0xa8;

        //链路地址
        pbuff[ptr++] = m_IEC101Infor.Linkaddr;
        //类型标识
        pbuff[ptr++] = yx_type;
        //可变结构限定词
        pbuff[ptr++] = 0x00;
        //传送原因
        pbuff[ptr++] = INTROGEN;
        //公共地址
        pbuff[ptr++] = m_IEC101Infor.Linkaddr;

        for(i=m_IEC101Infor.SendUpAllDataPtr;i<(int)m_YxArr.size();i++)
        {
            ST_YXDATA& data = m_YxArr[i];
            Q_RECORDDATATAG2 dataArray;
            memset(&dataArray,0,sizeof(dataArray));
            dataArray.val.channelID=m_PrintSetting->getChannelID();
            dataArray.val.device=data.lDPUID;
            dataArray.val.no=data.iNoInDPU;
            dataArray.val.eDataType=eDataTypeBit;
            m_pMgr->evalDataValue(&dataArray,1);
            data.iVal = dataArray.val.nValue;
            //信息体地址
            uint16 yxAddr = data.iNoInDPU;
            pbuff[ptr++]=yxAddr%256;
            pbuff[ptr++]=yxAddr/256;
            if (yx_type == SPOINT_YX)
                pbuff[ptr++]=(data.iVal)?(1):(0);
            else
                pbuff[ptr++]=(data.iVal)?(2):(1);

            if(i - m_IEC101Infor.SendUpAllDataPtr + 1 == 80)  //最长发送0xfa-10 ,可变结构限定词最多80
            {
                pbuff[1] = ptr-4;
                pbuff[2] = ptr-4;
                pbuff[7] =  0x00 | 80;

                byte byte = CalculateCheck( &pbuff[4] , ptr-4 );
                pbuff[ptr++] = byte;
                pbuff[ptr++] = END_FLAG;

                m_IEC101Infor.SendUpAllDataPtr = i+1;
                if(m_IEC101Infor.SendUpAllDataPtr >= m_YxArr.size())
                {
                    m_IEC101Infor.SendUpAllDataType = YC_WITHOUT_QDS;
                    m_IEC101Infor.SendUpAllDataPtr = 0;
                }

                memcpy(m_IEC101Infor.PreSendBuff,pbuff,ptr);
                m_IEC101Infor.PreBuffLen = ptr;

                if (m_PrintSetting->getPrintTranslate())
                {
                    sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"总召唤遥信数据");
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
                }
                if (m_PrintSetting->getPrintPacket())
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
                m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
                return;
            }
        }

        pbuff[1] = ptr - 4;
        pbuff[2] = ptr - 4;
        pbuff[7] =  0x00 | (i - m_IEC101Infor.SendUpAllDataPtr );

        byte byte = CalculateCheck( &pbuff[4] , ptr-4 );
        pbuff[ptr++] = byte;
        pbuff[ptr++] = END_FLAG;

        m_IEC101Infor.SendUpAllDataType = YC_WITHOUT_QDS;
        m_IEC101Infor.SendUpAllDataPtr = 0;

        memcpy(m_IEC101Infor.PreSendBuff,pbuff,ptr);
        m_IEC101Infor.PreBuffLen = ptr;

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"总召唤遥信数据");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }
    //变化遥信数据
    void SendUpChangeYX(int yx_type)
    {
        byte buff[512];
        byte *pbuff = buff;
        int i,ptr =0;

        pbuff[ptr++] = START_FLAG_1;
        //长度
        pbuff[ptr++] = 6;
        //长度
        pbuff[ptr++] = 6;
        pbuff[ptr++] = START_FLAG_1;

        //控制域
        //DIR = 1;  传输方向位
        //PRM = 0;  启动报文位
        //ACD = 0;  要求访问位
        //DFC = 0;  数据流控制位
        //RESPOND_DATA
        pbuff[ptr++] = 0x88;

        //链路地址
        pbuff[ptr++] = m_IEC101Infor.Linkaddr;
        //类型标识
        pbuff[ptr++] = yx_type;
        //可变结构限定词
        pbuff[ptr++] = 0x7f;
        //传送原因
        pbuff[ptr++] = SPONT;
        //公共地址
        pbuff[ptr++] = m_IEC101Infor.Linkaddr;

        int findcount = 0;
        for(i=0;i<(int)m_YxArr.size();i++)
        {
            int index = (m_IEC101Infor.SendUpYXDataPtr+i)%(m_YxArr.size());
            ST_YXDATA& data = m_YxArr[index];
            Q_RECORDDATATAG2 dataArray;
            memset(&dataArray,0,sizeof(dataArray));
            dataArray.val.channelID=m_PrintSetting->getChannelID();
            dataArray.val.device=data.lDPUID;
            dataArray.val.no=data.iNoInDPU;
            dataArray.val.eDataType=eDataTypeBit;
            m_pMgr->evalDataValue(&dataArray,1);
            if (data.iVal == dataArray.val.nValue)
                continue;
            data.iVal = dataArray.val.nValue;
            //信息体地址
            uint16 yxAddr = data.iNoInDPU;
            pbuff[ptr++]=yxAddr%256;
            pbuff[ptr++]=yxAddr/256;
            if (yx_type == SPOINT_YX)
                pbuff[ptr++]=(data.iVal)?(1):(0);
            else
                pbuff[ptr++]=(data.iVal)?(2):(1);

            if(++findcount == 80)  //最长发送0xfa-10 ,可变结构限定词最多80
            {
                pbuff[1] = ptr-4;
                pbuff[2] = ptr-4;
                m_IEC101Infor.ACD = 1;
                pbuff[4] |= (m_IEC101Infor.ACD << 5);
                pbuff[7] =  0x00 | 80;

                byte byte = CalculateCheck( &pbuff[4] , ptr-4 );
                pbuff[ptr++] = byte;
                pbuff[ptr++] = END_FLAG;

                memcpy(m_IEC101Infor.PreSendBuff,pbuff,ptr);
                m_IEC101Infor.PreBuffLen = ptr;
                m_IEC101Infor.SendUpYXDataPtr = index+1;

                if (m_PrintSetting->getPrintTranslate())
                {
                    sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"变化遥信数据");
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
                }
                if (m_PrintSetting->getPrintPacket())
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
                m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
                return;
            }
        }

        pbuff[1] = ptr - 4;
        pbuff[2] = ptr - 4;
        pbuff[7] =  0x00 | findcount;

        byte byte = CalculateCheck( &pbuff[4] , ptr-4 );
        pbuff[ptr++] = byte;
        pbuff[ptr++] = END_FLAG;

        memcpy(m_IEC101Infor.PreSendBuff,pbuff,ptr);
        m_IEC101Infor.PreBuffLen = ptr;
        m_IEC101Infor.SendUpYXDataPtr = 0;

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"变化遥信数据");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }
    //总召唤遥测
    void SendUpYc()
    {
        byte buff[512];
        byte *pbuff = buff;
        int i,ptr =0;

        pbuff[ptr++] = START_FLAG_1;
        //长度
        pbuff[ptr++] = 6;
        //长度
        pbuff[ptr++] = 6;
        pbuff[ptr++] = START_FLAG_1;

        //控制域
        //DIR = 1;  传输方向位
        //PRM = 0;  启动报文位
        //ACD = 1;  要求访问位
        //DFC = 0;  数据流控制位
        //RESPOND_DATA
        pbuff[ptr++] = 0xa8;

        //链路地址
        pbuff[ptr++] = m_IEC101Infor.Linkaddr;
        //类型标识
        pbuff[ptr++] = YC_WITHOUT_QDS;
        //可变结构限定词
        pbuff[ptr++] = 0x00;
        //传送原因
        pbuff[ptr++] = INTROGEN;
        //公共地址
        pbuff[ptr++] = m_IEC101Infor.Linkaddr;

        for(i=m_IEC101Infor.SendUpAllDataPtr;i<(int)m_YcArr.size();i++)
        {
            ST_YCDATA& data = m_YcArr[i];
            Q_RECORDDATATAG2 dataArray;
            memset(&dataArray,0,sizeof(dataArray));
            dataArray.val.channelID=m_PrintSetting->getChannelID();
            dataArray.val.device=data.lDPUID;
            dataArray.val.no=data.iNoInDPU;
            dataArray.val.eDataType=eDataTypeFloat;
            m_pMgr->evalDataValue(&dataArray,1);
            data.fVal = dataArray.val.fValue;
            //信息体地址
            uint16 ycAddr = data.iNoInDPU;
            pbuff[ptr++]=ycAddr%256;
            pbuff[ptr++]=ycAddr/256;
            short temp = (short)data.fVal;
            *(pbuff+ptr++) = temp%256;    //遥测值 低字节
            *(pbuff+ptr++) = temp/256;    //遥测值 高字节

            if(i - m_IEC101Infor.SendUpAllDataPtr + 1 == 48)  //最长发送(0xfa-10)/2
            {
                pbuff[1] = ptr - 4;
                pbuff[2] = ptr - 4;
                pbuff[7] =  0x00 | 48;

                byte byte = CalculateCheck(&pbuff[4] , ptr-4);
                pbuff[ptr++] = byte;
                pbuff[ptr++] = END_FLAG;

                m_IEC101Infor.SendUpAllDataPtr = i+1;
                if(m_IEC101Infor.SendUpAllDataPtr >= m_YcArr.size())
                {
                    m_IEC101Infor.SendUpAllDataType = CALL_ALL_DATA_END;
                    m_IEC101Infor.SendUpAllDataPtr = 0;
                    m_IEC101Infor.CallAll = 3;  //全数据发送结束帧
                }

                memcpy(m_IEC101Infor.PreSendBuff,pbuff,ptr);
                m_IEC101Infor.PreBuffLen = ptr;

                if (m_PrintSetting->getPrintTranslate())
                {
                    sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"总召唤遥测数据");
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
                }
                if (m_PrintSetting->getPrintPacket())
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
                m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
                return;
            }
        }	

        buff[1] = ptr - 4;
        buff[2] = ptr - 4;
        pbuff[7] =  0x00 | (i - m_IEC101Infor.SendUpAllDataPtr);

        byte byte = CalculateCheck( &pbuff[4] , ptr-4 );
        pbuff[ptr++] = byte;
        pbuff[ptr++] = END_FLAG;

        m_IEC101Infor.SendUpAllDataType = CALL_ALL_DATA_END;
        m_IEC101Infor.SendUpAllDataPtr = 0;
        m_IEC101Infor.CallAll = 3;  //全数据发送结束帧

        memcpy(m_IEC101Infor.PreSendBuff,pbuff,ptr);
        m_IEC101Infor.PreBuffLen = ptr;

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"总召唤遥测数据");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }
    //变化遥测数据
    void SendUpChangeYc()
    {
        byte buff[512];
        byte *pbuff = buff;
        int i,ptr =0;

        pbuff[ptr++] = START_FLAG_1;
        //长度
        pbuff[ptr++] = 6;
        //长度
        pbuff[ptr++] = 6;
        pbuff[ptr++] = START_FLAG_1;

        //控制域
        //DIR = 1;  传输方向位
        //PRM = 0;  启动报文位
        //ACD = 1;  要求访问位
        //DFC = 0;  数据流控制位
        //RESPOND_DATA
        pbuff[ptr++] = 0x88 | (m_IEC101Infor.ACD<<5);

        //链路地址
        pbuff[ptr++] = m_IEC101Infor.Linkaddr;
        //类型标识
        pbuff[ptr++] = YC_WITHOUT_QDS;
        //可变结构限定词
        pbuff[ptr++] = 0x00;
        //传送原因
        pbuff[ptr++] = ACTCON;
        //公共地址
        pbuff[ptr++] = m_IEC101Infor.Linkaddr;

        int findcount = 0;
        for(i=0;i<(int)m_YcArr.size();i++)
        {
            int index = (m_IEC101Infor.SendUpYCDataPtr+i)%(m_YcArr.size());
            ST_YCDATA& data = m_YcArr[i];
            Q_RECORDDATATAG2 dataArray;
            memset(&dataArray,0,sizeof(dataArray));
            dataArray.val.channelID=m_PrintSetting->getChannelID();
            dataArray.val.device=data.lDPUID;
            dataArray.val.no=data.iNoInDPU;
            dataArray.val.eDataType=eDataTypeFloat;
            m_pMgr->evalDataValue(&dataArray,1);
            if (fabs(dataArray.val.fValue-data.fVal) < 0.0001)
                continue;
            data.fVal = dataArray.val.fValue;
            //信息体地址
            uint16 ycAddr = data.iNoInDPU;
            pbuff[ptr++]=ycAddr%256;
            pbuff[ptr++]=ycAddr/256;
            short temp = (short)data.fVal;
            *(pbuff+ptr++) = temp%256;    //遥测值 低字节
            *(pbuff+ptr++) = temp/256;    //遥测值 高字节

            if(++findcount == 48)  //最长发送(0xfa-10)/2
            {
                pbuff[1] = ptr - 4;
                pbuff[2] = ptr - 4;
                pbuff[7] =  0x00 | 48;

                byte byte = CalculateCheck(&pbuff[4] , ptr-4);
                pbuff[ptr++] = byte;
                pbuff[ptr++] = END_FLAG;

                memcpy(m_IEC101Infor.PreSendBuff,pbuff,ptr);
                m_IEC101Infor.PreBuffLen = ptr;
                m_IEC101Infor.SendUpYCDataPtr = index+1;

                if (m_PrintSetting->getPrintTranslate())
                {
                    sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"变化遥测数据");
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
                }
                if (m_PrintSetting->getPrintPacket())
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
                m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
                return;
            }
        }	

        buff[1] = ptr - 4;
        buff[2] = ptr - 4;
        pbuff[7] =  0x00 | findcount;

        byte byte = CalculateCheck( &pbuff[4] , ptr-4 );
        pbuff[ptr++] = byte;
        pbuff[ptr++] = END_FLAG;

        memcpy(m_IEC101Infor.PreSendBuff,pbuff,ptr);
        m_IEC101Infor.PreBuffLen = ptr;
        m_IEC101Infor.SendUpYCDataPtr = 0;

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"变化遥测数据");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }
    //带品质描述的浮点数
    void SendUpYc_Float()
    {
        byte buff[512];
        byte *pbuff = buff;
        int i,ptr =0;

        pbuff[ptr++] = START_FLAG_1;
        //长度
        pbuff[ptr++] = 6;
        //长度
        pbuff[ptr++] = 6;
        pbuff[ptr++] = START_FLAG_1;

        //控制域
        //DIR = 1;  传输方向位
        //PRM = 0;  启动报文位
        //ACD = 1;  要求访问位
        //DFC = 0;  数据流控制位
        //RESPOND_DATA
        pbuff[ptr++] = 0xa8;

        //链路地址
        pbuff[ptr++] = m_IEC101Infor.Linkaddr;
        //类型标识
        pbuff[ptr++] = YC_INFO_FLOAT;
        //可变结构限定词
        pbuff[ptr++] = 0x00;
        //传送原因
        pbuff[ptr++] = INTROGEN;
        //公共地址
        pbuff[ptr++] = (byte)m_IEC101Infor.Linkaddr; 

        for(i=m_IEC101Infor.SendUpAllDataPtr;i<(int)m_YcArr.size();i++)
        {
            ST_YCDATA& data = m_YcArr[i];
            Q_RECORDDATATAG2 dataArray;
            memset(&dataArray,0,sizeof(dataArray));
            dataArray.val.channelID=m_PrintSetting->getChannelID();
            dataArray.val.device=data.lDPUID;
            dataArray.val.no=data.iNoInDPU;
            dataArray.val.eDataType=eDataTypeFloat;
            m_pMgr->evalDataValue(&dataArray,1);
            data.fVal = dataArray.val.fValue;
            //信息体地址
            uint16 ycAddr = data.iNoInDPU;
            pbuff[ptr++]=ycAddr%256;
            pbuff[ptr++]=ycAddr/256;
            float ftemp = data.fVal;
            memcpy(pbuff + ptr, &ftemp , sizeof(ftemp));
            ptr += 4;
            pbuff[ptr++] = 0x80;

            if(i - m_IEC101Infor.SendUpAllDataPtr + 1 == 34)  //最长发送(0xfa-10)/2
            {
                pbuff[1] = ptr - 4;
                pbuff[2] = ptr - 4;
                pbuff[7] =  0x00 | 34;

                byte byte = CalculateCheck(&pbuff[4] , ptr-4);
                pbuff[ptr++] = byte;
                pbuff[ptr++] = END_FLAG;

                m_IEC101Infor.SendUpAllDataPtr = i+1;
                if(m_IEC101Infor.SendUpAllDataPtr >= m_YcArr.size())
                {
                    m_IEC101Infor.SendUpAllDataType = CALL_ALL_DATA_END;
                    m_IEC101Infor.SendUpAllDataPtr = 0;
                    m_IEC101Infor.CallAll = 3;  //全数据发送结束帧
                }

                memcpy(m_IEC101Infor.PreSendBuff,pbuff,ptr);
                m_IEC101Infor.PreBuffLen = ptr;

                if (m_PrintSetting->getPrintTranslate())
                {
                    sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"总召唤遥测数据");
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
                }
                if (m_PrintSetting->getPrintPacket())
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
                m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
                return;
            }
        }	

        buff[1] = ptr - 4 ;
        buff[2] = ptr - 4 ;
        pbuff[7] =  i - m_IEC101Infor.SendUpAllDataPtr;

        byte byte = CalculateCheck( &pbuff[4] , ptr-4 );
        pbuff[ptr++] = byte;
        pbuff[ptr++] = END_FLAG;

        m_IEC101Infor.SendUpAllDataType = CALL_ALL_DATA_END;
        m_IEC101Infor.SendUpAllDataPtr = 0;
        m_IEC101Infor.CallAll = 3;  //全数据发送结束帧

        memcpy(m_IEC101Infor.PreSendBuff,pbuff,ptr);
        m_IEC101Infor.PreBuffLen = ptr;

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"总召唤遥测数据");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }
    //带品质描述的浮点数
    void SendUpChangeYc_Float()
    {
        byte buff[512];
        byte *pbuff = buff;
        int i,ptr =0;

        pbuff[ptr++] = START_FLAG_1;
        //长度
        pbuff[ptr++] = 6;
        //长度
        pbuff[ptr++] = 6;
        pbuff[ptr++] = START_FLAG_1;

        //控制域
        //DIR = 1;  传输方向位
        //PRM = 0;  启动报文位
        //ACD = 1;  要求访问位
        //DFC = 0;  数据流控制位
        //RESPOND_DATA
        pbuff[ptr++] = 0x88 | (m_IEC101Infor.ACD<<5);

        //链路地址
        pbuff[ptr++] = m_IEC101Infor.Linkaddr;
        //类型标识
        pbuff[ptr++] = YC_INFO_FLOAT;
        //可变结构限定词
        pbuff[ptr++] = 0x00;
        //传送原因
        pbuff[ptr++] = ACTCON;
        //公共地址
        pbuff[ptr++] = (byte)m_IEC101Infor.Linkaddr; 

        int findcount = 0;
        for(i=m_IEC101Infor.SendUpAllDataPtr;i<(int)m_YcArr.size();i++)
        {
            int index = (m_IEC101Infor.SendUpYCDataPtr+i)%(m_YcArr.size());
            ST_YCDATA& data = m_YcArr[i];
            Q_RECORDDATATAG2 dataArray;
            memset(&dataArray,0,sizeof(dataArray));
            dataArray.val.channelID=m_PrintSetting->getChannelID();
            dataArray.val.device=data.lDPUID;
            dataArray.val.no=data.iNoInDPU;
            dataArray.val.eDataType=eDataTypeFloat;
            m_pMgr->evalDataValue(&dataArray,1);
            if (fabs(dataArray.val.fValue-data.fVal) < 0.0001)
                continue;
            data.fVal = dataArray.val.fValue;
            //信息体地址
            uint16 ycAddr = data.iNoInDPU;
            pbuff[ptr++]=ycAddr%256;
            pbuff[ptr++]=ycAddr/256;
            float ftemp = data.fVal;
            memcpy(pbuff + ptr, &ftemp , sizeof(ftemp));
            ptr += 4;
            pbuff[ptr++] = 0x80;

            if(++findcount == 34)  //最长发送(0xfa-10)/2
            {
                pbuff[1] = ptr - 4;
                pbuff[2] = ptr - 4;
                pbuff[7] =  0x00 | 34;

                byte byte = CalculateCheck(&pbuff[4] , ptr-4);
                pbuff[ptr++] = byte;
                pbuff[ptr++] = END_FLAG;

                memcpy(m_IEC101Infor.PreSendBuff,pbuff,ptr);
                m_IEC101Infor.PreBuffLen = ptr;
                m_IEC101Infor.SendUpYCDataPtr = index+1;

                if (m_PrintSetting->getPrintTranslate())
                {
                    sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"变化遥测数据");
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
                }
                if (m_PrintSetting->getPrintPacket())
                    SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
                m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
                return;
            }
        }	

        buff[1] = ptr - 4 ;
        buff[2] = ptr - 4 ;
        pbuff[7] =  i - m_IEC101Infor.SendUpAllDataPtr;

        byte byte = CalculateCheck( &pbuff[4] , ptr-4 );
        pbuff[ptr++] = byte;
        pbuff[ptr++] = END_FLAG;

        memcpy(m_IEC101Infor.PreSendBuff,pbuff,ptr);
        m_IEC101Infor.PreBuffLen = ptr;
         m_IEC101Infor.SendUpYCDataPtr = 0;

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"变化遥测数据");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }
    //无数据报文
    void AnswerNouData()
    {
        byte buff[10];
        int ptr = 0;

        buff[ptr++] = START_FLAG_2;
        buff[ptr++] = 0x89 | (m_IEC101Infor.ACD<<5);
        buff[ptr++] = m_IEC101Infor.Linkaddr;
        byte byte = CalculateCheck( &buff[1] , 2 );
        buff[ptr++] = byte;
        buff[ptr++] = END_FLAG;

        memcpy(m_IEC101Infor.PreSendBuff,buff,ptr);
        m_IEC101Infor.PreBuffLen = ptr;

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"无一类数据");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }
    //总召唤确认
    void RespondCallAllData()
    {
        byte buff[10];
        int ptr = 0;

        buff[ptr++] = START_FLAG_2;
        buff[ptr++] = 0xa0;
        buff[ptr++] = m_IEC101Infor.Linkaddr;
        byte byte = CalculateCheck( &buff[1] , 2 );
        buff[ptr++] = byte;
        buff[ptr++] = END_FLAG;

        memcpy(m_IEC101Infor.PreSendBuff,buff,ptr);
        m_IEC101Infor.PreBuffLen = ptr;

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"总召唤确认");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }
    //总召唤开始
    void AnswerAllDataStar()
    {
        byte buff[50];
        byte *pbuff = buff;	
        int ptr = 0;

        pbuff[ptr++] = START_FLAG_1;
        //长度
        pbuff[ptr++] = 9;
        //长度
        pbuff[ptr++] = 9;
        pbuff[ptr++] = START_FLAG_1;

        //控制域
        //DIR = 1;  传输方向位
        //PRM = 0;  启动报文位
        //ACD = 0;  要求访问位
        //DFC = 0;  数据流控制位	
        pbuff[ptr++] = 0xa8;

        //链路地址
        pbuff[ptr++] = m_IEC101Infor.Linkaddr;
        //类型标识
        pbuff[ptr++] = CALL_ALLDATA;
        //可变结构限定词
        pbuff[ptr++] = 1;
        //传送原因
        pbuff[ptr++] = ACTCON;
        pbuff[ptr++] = (byte)m_IEC101Infor.Linkaddr;  //公共地址

        //信息体地址
        pbuff[ptr++] = 0;
        pbuff[ptr++] = 0;
        pbuff[ptr++] = 0x14;

        byte byte = CalculateCheck( &pbuff[4] , ptr - 4 );
        pbuff[ptr++] = byte;
        pbuff[ptr++] = END_FLAG;

        memcpy(m_IEC101Infor.PreSendBuff,pbuff,ptr);
        m_IEC101Infor.PreBuffLen = ptr;
        m_IEC101Infor.SendUpAllDataPtr = 0;

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"总召唤开始");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }

    //总召唤结束
    void AnswerAllDataEnd()
    {
        byte buff[50];
        byte *pbuff = buff;	
        int ptr = 0;

        pbuff[ptr++] = START_FLAG_1;
        //长度
        pbuff[ptr++] = 9;
        //长度
        pbuff[ptr++] = 9;
        pbuff[ptr++] = START_FLAG_1;

        //控制域
        //DIR = 1;  传输方向位
        //PRM = 0;  启动报文位
        //ACD = 0;  要求访问位
        //DFC = 0;  数据流控制位	
        pbuff[ptr++] = 0xa8;

        //链路地址
        pbuff[ptr++] = m_IEC101Infor.Linkaddr;
        //类型标识
        pbuff[ptr++] = CALL_ALLDATA;
        //可变结构限定词
        pbuff[ptr++] = 1;
        //传送原因
        pbuff[ptr++] = ACTTERM;
        pbuff[ptr++] = m_IEC101Infor.Linkaddr;

        //信息体地址
        pbuff[ptr++] = 0;
        pbuff[ptr++] = 0;
        pbuff[ptr++] = 0x14;

        byte byte = CalculateCheck( &pbuff[4] , ptr-4 );
        pbuff[ptr++] = byte;
        pbuff[ptr++] = END_FLAG;

        memcpy(m_IEC101Infor.PreSendBuff,pbuff,ptr);
        m_IEC101Infor.PreBuffLen = ptr;
        m_IEC101Infor.SendUpAllDataPtr = 0;

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"总召唤结束");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }
    void AnswerYk(uint16 type)
    {
        byte buff[20];
        int ptr = 0;
        //报文填充
        buff[ptr++] = START_FLAG_1;
        buff[ptr++] = 9;
        buff[ptr++] = 9;
        buff[ptr++] = START_FLAG_1;

        buff[ptr++] = 0x88 | (m_IEC101Infor.ACD<<5);
        buff[ptr++] = m_IEC101Infor.Linkaddr;

        if (m_IEC101Infor.YKFlag < 10)
            buff[ptr++] = 45;
        else
            buff[ptr++] = YK_FLAG;           //类型标识
        buff[ptr++] = 0x01;                  //可变结构限定词
        buff[ptr++] = ACTCON;                //传送原因

        buff[ptr++] = m_IEC101Infor.Linkaddr;
        buff[ptr++] = m_IEC101Infor.ykinforAddr % 256;
        buff[ptr++] = m_IEC101Infor.ykinforAddr / 256;

        if (m_IEC101Infor.YKFlag < 10)
            buff[ptr] = m_IEC101Infor.YKFlag;
        else
            buff[ptr] = (m_IEC101Infor.YKFlag - 10) + 1;
        if (type == YKFUNC_PRESET)
            buff[ptr] |= 0x80;
        ptr++;

        byte byte = CalculateCheck( &buff[ptr-9] , 9 );
        buff[ptr++] = byte;
        buff[ptr++] = END_FLAG;	

        memcpy(m_IEC101Infor.PreSendBuff,buff,ptr);
        m_IEC101Infor.PreBuffLen = ptr;

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"遥控命令");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }
    void AnswerYt(uint16 type)
    {
        byte buff[20];
        int ptr = 0;
        //报文填充
        if (m_IEC101Infor.YKFlag < 10)
        {
            //报文填充
            buff[ptr++] = START_FLAG_1;
            buff[ptr++] = 13;
            buff[ptr++] = 13;
            buff[ptr++] = START_FLAG_1;

            buff[ptr++] = 0x88 | (m_IEC101Infor.ACD<<5);
            buff[ptr++] = m_IEC101Infor.Linkaddr;

            buff[ptr++] = 50     ;               //类型标识
            buff[ptr++] = 0x01;                  //可变结构限定词
            buff[ptr++] = ACTCON;                //传送原因

            buff[ptr++] = m_IEC101Infor.Linkaddr;
            buff[ptr++] = m_IEC101Infor.ykinforAddr % 256;
            buff[ptr++] = m_IEC101Infor.ykinforAddr / 256;

            memcpy(&buff[ptr],&m_IEC101Infor.ytdata,4);
            ptr += 4;
            if (type == YTFUNC_PRESET)
                buff[ptr++] = 0x80;
            else
                buff[ptr++] = 0;

            byte byte = CalculateCheck( &buff[ptr-13] , 13 );
            buff[ptr++] = byte;
            buff[ptr++] = END_FLAG;	
        }
        else
        {
            //报文填充
            buff[ptr++] = START_FLAG_1;
            buff[ptr++] = 11;
            buff[ptr++] = 11;
            buff[ptr++] = START_FLAG_1;

            buff[ptr++] = 0x88 | (m_IEC101Infor.ACD<<5);
            buff[ptr++] = m_IEC101Infor.Linkaddr;

            buff[ptr++] = SET_VALUE     ;        //类型标识
            buff[ptr++] = 0x01;                  //可变结构限定词
            buff[ptr++] = ACTCON;                //传送原因

            buff[ptr++] = m_IEC101Infor.Linkaddr;
            buff[ptr++] = m_IEC101Infor.ykinforAddr % 256;
            buff[ptr++] = m_IEC101Infor.ykinforAddr / 256;

            buff[ptr++] = int(m_IEC101Infor.ytdata)%256;
            buff[ptr++] = int(m_IEC101Infor.ytdata)/256;
            if (type == YTFUNC_PRESET)
                buff[ptr++] = 0x80;
            else
                buff[ptr++] = 0;

            byte byte = CalculateCheck( &buff[ptr-11] , 11 );
            buff[ptr++] = byte;
            buff[ptr++] = END_FLAG;	
        }

        memcpy(m_IEC101Infor.PreSendBuff,buff,ptr);
        m_IEC101Infor.PreBuffLen = ptr;

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"遥调命令");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }
    //回答时钟同步
    void AnswerTime()
    {
        SYSTEMTIME Localtime;
        GetLocalTime(Localtime);

        byte buff[256];
        int bufflen=0;
        //报文填充
        buff[bufflen++] = START_FLAG_1;
        buff[bufflen++] = 15;
        buff[bufflen++] = 15;
        buff[bufflen++] = START_FLAG_1;

        buff[bufflen++] = 0x88 | (m_IEC101Infor.ACD<<5);
        buff[bufflen++] = m_IEC101Infor.Linkaddr;

        buff[bufflen++] = MODIFY_TIME;          //类型标识
        buff[bufflen++] = 0x1;                  //可变结构限定词
        buff[bufflen++] = ACTCON;               //传送原因

        buff[bufflen++] = m_IEC101Infor.Linkaddr;      //公共地址
        buff[bufflen++] = 0x0;                 //信息体地址
        buff[bufflen++] = 0x0;                 //信息体地址

        //以下是 CP56Time2a 格式的时标
        int milli = Localtime.wSecond * 1000 + Localtime.wMilliseconds;
        buff[bufflen++] = (byte)(milli & 0xFF);
        buff[bufflen++] = (byte)(milli >> 8);
        buff[bufflen++] = (byte)(Localtime.wMinute & 0x3F);
        buff[bufflen++] = (byte)(Localtime.wHour & 0x1F);

        buff[bufflen++] = Localtime.wDay & 0x1F;
        buff[bufflen++] = (byte)(Localtime.wMonth & 0xF);
        buff[bufflen++] = (byte)((Localtime.wYear - 2000) & 0x7F);

        byte byte = CalculateCheck( &buff[4] , bufflen-4 );
        buff[bufflen++] = byte;
        buff[bufflen++] = END_FLAG;

        memcpy(m_IEC101Infor.PreSendBuff,buff,bufflen);
        m_IEC101Infor.PreBuffLen = bufflen;	

        if (m_PrintSetting->getPrintTranslate())
        {
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"对时命令");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeCh,m_LastMsg,strlen(m_LastMsg)+1);
        }
        if (m_PrintSetting->getPrintPacket())
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeTx,(char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
        m_pProcotolConfig->getSendBuffer()->writeData((char *)m_IEC101Infor.PreSendBuff,m_IEC101Infor.PreBuffLen);
    }
	virtual bool makeFrame() { return true; }
	virtual bool terminateProtocol() { m_PrintSetting=NULL;return true; }
	virtual bool startProcess()
    {
        m_eProtocolStatus = eCommonStatusNormal;
        m_PrintSetting = (Q_protcol101SlaveSetting *)m_pProcotolConfig->getProtocolSetting();
        if(m_PrintSetting!=NULL)
        {
            //新规约打印
            sprintf_s(m_LastMsg,sizeof(m_LastMsg)-1,"初使化开始...");
            SendMsgPrint(m_PrintSetting->getChannelID(),eMsgTypeMsg,m_LastMsg,strlen(m_LastMsg)+1);
            //初始化参数
            CFEPFuncRtdb *m_rtdbInf = m_pMgr->getRtdbInf();
            CScadaDataBase *dbmg_sdb = m_rtdbInf->getScadaDb();

            int nCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_Analog_ID);
            SCADA_Analog curYc;
            m_YcArr.clear();
            for (int j=0; j<nCount; j++)
            {
                dbmg_sdb->GetElementByIndex(SCADA_TAB_Analog_ID, j,&curYc);
                if (m_PrintSetting->getChannelID() == (int)curYc.uTuID 
                    && curYc.lDPUID >= 0 && curYc.iNoInDPU >= 0)
                {
                    Q_RECORDDATATAG2 dataArray;
                    memset(&dataArray,0,sizeof(dataArray));
                    dataArray.val.channelID=m_PrintSetting->getChannelID();
                    dataArray.val.device=curYc.lDPUID;
                    dataArray.val.no=curYc.iNoInDPU;
                    dataArray.val.eDataType=eDataTypeFloat;
                    if(m_pMgr->evalDataValue(&dataArray,1))
                    {
                        ST_YCDATA data;
                        data.iNoInDPU = curYc.iNoInDPU;
                        data.lDPUID = curYc.lDPUID;
                        data.fVal = dataArray.val.fValue;
                        m_YcArr.push_back(data);
                    }
                }
            }
            nCount = dbmg_sdb->GetTableRecordCount(SCADA_TAB_Digit_ID);
            SCADA_Digit curYx;
            m_YxArr.clear();
            for (int j=0; j<nCount; j++)
            {
                dbmg_sdb->GetElementByIndex(SCADA_TAB_Digit_ID, j,&curYx);
                if (m_PrintSetting->getChannelID() == (int)curYx.uTuID
                    && curYx.lDPUID >= 0 && curYx.iNoInDPU >= 0)
                {
                    Q_RECORDDATATAG2 dataArray;
                    memset(&dataArray,0,sizeof(dataArray));
                    dataArray.val.channelID=m_PrintSetting->getChannelID();
                    dataArray.val.device=curYc.lDPUID;
                    dataArray.val.no=curYc.iNoInDPU;
                    dataArray.val.eDataType=eDataTypeBit;
                    if(m_pMgr->evalDataValue(&dataArray,1))
                    {
                        ST_YXDATA data;
                        data.iNoInDPU = curYx.iNoInDPU;
                        data.lDPUID = curYx.lDPUID;
                        data.iVal = dataArray.val.nValue;
                        m_YxArr.push_back(data);
                    }
                }
            }
            m_IEC101Infor.Linkaddr = m_PrintSetting->getDevAddr();
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

		SYSTEM_Process * pProcess = m_rtdbInf->getMainDb()->FindProcessByProType(pt_DBGSVR, m_rtdbInf->getMainDb()->GetThisNodeID() );
		if(pProcess==NULL)
			return ;

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
			m_rtdbInf->getMainDb()->SendMessageTo(pProcess->ID32,mes_MsgPrint,sizeof(MsgPrint),(char*)&MsgPrint);

        }
    }

    //执行时间
    int m_TimeOut;
    time_t m_LastTime;
    //101协议信息
    struct IEC101_STRUCT m_IEC101Infor;
    std::vector<ST_YCDATA>m_YcArr;
    std::vector<ST_YXDATA>m_YxArr;
    //配置对象
    Q_protcol101SlaveSetting* m_PrintSetting;
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
    Q_commonBuffer* bufferReceive = new Q_commonBuffer();
    Q_commonBuffer* bufferSend = new Q_commonBuffer();
    Q_protocolCommandManagerBase* pCommandList = new Q_protocol101SlaveCommandMgr();

    //通道配置
    Q_serialLinkSetting* linkSetting = new Q_serialLinkSetting();

    Q_linkConfig* linkConfig = new Q_linkConfig();
    linkConfig->setLinkSetting(linkSetting);
    linkConfig->setReceiveBuffer(bufferReceive);
    linkConfig->setSendBuffer(bufferSend);

    //通道对象
    Q_serialLink* linkTest = new Q_serialLink();

    //规约配置
    Q_protcol101SlaveSetting *protocolSetting = new Q_protcol101SlaveSetting();
    protocolSetting->setChannelType(v_param->cfg->type);
    protocolSetting->setChannelID(v_param->cfg->id);
    protocolSetting->setFilePath(v_param->strFilePath);
    protocolSetting->initSetting();

    do 
    {
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
    } while (0);

    Q_protocolConfig* protocolConfig = new Q_protocolConfig();
    protocolConfig->setProtocolSetting(protocolSetting);
    protocolConfig->setReceiveBuffer(bufferReceive);
    protocolConfig->setSendBuffer(bufferSend);
    protocolConfig->setProtocolCommandManager(pCommandList);

    //规约对象
    Q_protocol101Slave* protocolTest = new Q_protocol101Slave();
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
	printf("=== Comm101Slave Protocol Dynamic lib====\n");
	printf("=== 2016-07-22====\n");
	printf("1、增加打印输入版本功能\n");

	return 1;
}
#ifdef UNIX_ENV
#ifdef __cplusplus
}
#endif
#endif //OS_UNIX
