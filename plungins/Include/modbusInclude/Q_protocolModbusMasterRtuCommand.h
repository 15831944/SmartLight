#pragma once

#include "Q_frameInclude.h"
#include "Q_ModbusRtuMasterDef.h"
#include "Q_ModbusDefine.h"
#define MAX_COUNT_MODBUS_SETVAL 126
#define MAX_WAIT_TIMES 20

class __Q_MODBUSRTU_MASTER_EXPORT Q_protocolModbusMasterRtuCommand : public Q_protocolCommandBase
{
public:
    Q_protocolModbusMasterRtuCommand(void);
    virtual ~Q_protocolModbusMasterRtuCommand(void);

public:
    //命令类型
    inline void					setCommandModbusType(Q_Modbus_CommandType eType) { m_commandType = eType;}
    inline Q_Modbus_CommandType getCommandModbusType() { return m_commandType;}

    inline void					    setCommandModbusStatus(Q_Modbus_CommandStatus eStatus) { m_commandStatus = eStatus;}
    inline Q_Modbus_CommandStatus	getCommandModbusStatus() { return m_commandStatus;}

    //设备地址
    inline void					setDestinationId(unsigned char nId) { m_nDestinationId = nId;}
    inline unsigned char		getDestinationId() { return m_nDestinationId;}

    //遥调值
    inline void					setS16ValueByIndex(int index,int nValue) { m_nValue[index-1] = nValue;}
    inline int				getS16ValueByIndex(int index) { 
		return m_nValue[index-1];
	}

    inline void                 setYTFunCode(BYTE funcode){m_YTFunCode = funcode;}
    inline BYTE                 getYTFunCode(){return m_YTFunCode;}

    //遥调控制
    inline void					setYTIndex(int nIndex) { m_nYTIndex = nIndex;}
    inline int		            getYTIndex() { return m_nYTIndex;}
    inline void                 setYTOffset(int offset){m_nYTOffset = offset;}
    inline int                  getYTOffset(){ return m_nYTOffset;}
    inline void                 setYTMaxNum(int max ){m_nYTMaxNum = max;}
    inline int                  getYTMaxNum(){return m_nYTMaxNum;}

    //读遥测控制
    inline void                 setYCOffset(int offset){m_nYCOffset = offset;}
    inline int                  getYCOffset(){return m_nYCOffset;}
    inline void                 setYCIndex(int index){m_nYCIndex = index;}
    inline int                  getYCIndex(){return m_nYCIndex;}
    inline void                 setYCMaxNum(int max){m_nYCMaxNum = max;}
    inline int                  getYCMaxNum(){return m_nYCMaxNum;}

    //重试控制，删除不响应命令帧
    inline void				    setMaxTryCount(int nMaxTryCount) { m_nMaxTryCount=nMaxTryCount;}
    inline int				    getMaxTryCount() { return m_nMaxTryCount;}
    inline void				    setCurTryNum(int nCurTryNum) { m_nCurTryNum=nCurTryNum;}
    inline int				    getCurTryNum() { return m_nCurTryNum;}
    bool    beyondWaitTimes(){return m_maxNoCon>=MAX_WAIT_TIMES;}
    void    addWaitTimes(){m_maxNoCon++;}
    void    resetWaitTimes(){m_maxNoCon=0;}


protected:
    unsigned char				m_nDestinationId; //站编号
    Q_Modbus_CommandType        m_commandType;//命令类型
    Q_Modbus_CommandStatus      m_commandStatus;//状态默认设定选择
    int                         m_nMaxTryCount;//最大试验数
    int                         m_nCurTryNum;//当前实验数

    int                         m_nYCOffset;//每次读遥测个数
    int                         m_nYCIndex;//遥测地址序号
    int                         m_nYCMaxNum;//遥测总个数

    int				            m_nYTIndex;//遥调地址序号
    int                         m_nYTOffset;//一次遥调个数
    int                         m_nYTMaxNum;//遥调总个数

    int                       m_nValue[MAX_COUNT_MODBUS_SETVAL];//遥调值
    BYTE                        m_YTFunCode;
    
	int                         m_maxNoCon;

};
