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
    //�趨ң��ң�ŵĵ�ַ��Χ����Ч�ĸ���
    void                setAnalogScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
    void                setDigitalScope(int nAddrBegin,int nAddrEnd,int nVaildCount);
    //ң��
    int                 getAnalogBegin();
    int                 getAnalogEnd();
    int                 getAnalogCount();
    //ң��
    int                 getPointBegin();
    int                 getPointEnd();
    int                 getPointCount();
    //--------------------------------------------------------------------------------------
    //ң���ң����ͬ����
    int                 getYCRegisterBegin();
    int                 getYTRegisterBegin();
    int                 getRegisterCount();
    int                 getMaxYCRegsOnce();
    int                 getMaxYTRegsOnce();
    int                 getYCRegsOnce();
    int                 getYTRegsOnce();

    int                 getDeviceId();          //���豸��ַ
    int                 getPerTime();           //����ң�⡢ң�ż��ʱ��
    ModbusFloatUpSeq    getFloatUpSeq();        //���������ֽڸ�����������
    ModbusYTMethod      getYTMethod();          //ң����Ҫ�ж��ǵ����й������޹�
    bool                enableShowMsg();
    bool                enableWriteLog();
	virtual bool reloadLog();
protected:
    vector<ST_ModCfgT > m_vContent; //����
    vector<ST_ModCfgT > m_vGroup;   //������
private:
    char m_sFilePath[MAX_PATH];          //�ļ�·��
    char LastError[Q_FRAME_CONST1024];            //������Ϣ
    bool init();
private:
    int m_alg_count;    //ң�����
    int m_alg_begin;    //ң����ʼ��ַ
    int m_alg_end;      //ң�������ַ

    int m_pnt_count;    //ң��Ϣ����
    int m_pnt_begin;    //ң����ʼ��ַ
    int m_pnt_end;      //ң�Ž�����ַ

};


