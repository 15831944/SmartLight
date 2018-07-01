#pragma once
#include "Q_frameInclude.h"
#include "Q_ModbusRtuMasterDef.h"
#include "Q_ModbusDefine.h"

#include <vector>
using namespace std;

class __Q_MODBUSRTU_MASTER_EXPORT Q_protocolModbusMasterRtuSetting : public Q_protocolSettingBase
{
public:
    Q_protocolModbusMasterRtuSetting(void);
    virtual ~Q_protocolModbusMasterRtuSetting(void);
    void                setFilePath(char *);
    bool                initSetting();
    char*               getContentID(const int &v_idx,const int &v_grp=1);
    //--------------------------------------------------------------------------------------
    //设定遥测遥信的地址范围及有效的个数
    void                setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
    void                setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
    //遥测
    int                 getAnalogBegin();
    int                 getAnalogEnd();
    int                 getAnalogCount();
    //遥信
    int                 getPointBegin();
    int                 getPointEnd();
    int                 getPointCount();
    //--------------------------------------------------------------------------------------
    //遥测和遥调共同部分
    int                 getYCRegisterBegin();
    int                 getYTRegisterBegin();
    int                 getRegisterCount();
    int                 getMaxYCRegsOnce();
    int                 getMaxYTRegsOnce();
    int                 getYCRegsOnce();
    int                 getYTRegsOnce();

    int                 getDeviceId();          //读设备地址
    int                 getPerTime();           //发送遥测、遥信间隔时间
    ModbusFloatUpSeq    getFloatUpSeq();        //不存在四字节浮点数的问题
    ModbusYTMethod      getYTMethod();          //遥调需要判读是调节有功还是无功
    bool                enableShowMsg();
    bool                enableWriteLog();
	virtual bool reloadLog();
protected:
    vector<ST_ModCfgT > m_vContent; //内容
    vector<ST_ModCfgT > m_vGroup;   //分组名
private:
    char m_sFilePath[MAX_PATH];          //文件路径
    char LastError[Q_FRAME_CONST1024];            //错误信息
    bool init();
private:
    int m_alg_count;    //遥测个数
    int m_alg_begin;    //遥测起始地址
    int m_alg_end;      //遥测结束地址

    int m_pnt_count;    //遥信息个数
    int m_pnt_begin;    //遥信起始地址
    int m_pnt_end;      //遥信结束地址

};


