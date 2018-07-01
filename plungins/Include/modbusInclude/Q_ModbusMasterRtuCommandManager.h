#pragma once
#include "Q_frameInclude.h"
#include "Q_ModbusRtuMasterDef.h"
#include "Q_ModbusDefine.h"
#include "Q_protocolModbusMasterRtuCommand.h"
class __Q_MODBUSRTU_MASTER_EXPORT Q_ModbusMasterRtuCommandManager : public Q_protocolCommandManagerBase
{
public:
    Q_ModbusMasterRtuCommandManager(void);
    virtual ~Q_ModbusMasterRtuCommandManager(void);

public:
    int     getNextCmdIndex(){return m_nextCmdIndex;}
    void    setNextCmdIndex(int index){m_nextCmdIndex = index;}
    int     getLastCmdIndex(){return m_lastCmdIndex;}
    void    setLastCmdIndex(int index){m_lastCmdIndex = index;}

    bool    removeCommand(Q_Modbus_CommandType eType,Q_Modbus_CommandStatus eStatus,int nObjIndex);
    bool    removeCommand(Q_protocolCommandBase* pCommand);
    virtual bool    addCommand(Q_protocolCommandBase* pCommand);
    Q_protocolModbusMasterRtuCommand*   findCommand(Q_Modbus_CommandType eType,Q_Modbus_CommandStatus eStatus,int nObjIndex);
private:
    int m_nextCmdIndex;
    int m_lastCmdIndex;


};


