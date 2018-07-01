to_srcdir:=../../
include $(to_srcdir)make.rules
DEBUG=-g
CFLAGS =$(OPTIMIZE_FLAG) $(BYTE_PRECISION_DEF) $(OS_DEF) -D_TS_ERRNO -Wall -std=c++0x $(DEBUG) -I../../rtdb/include -I../../rtdb/headfile -I../../scada/include  -I../../Include/commFrameInclude -I../Include/IEC104Include\
-I../../Include/tinyxmlInclude -I../../CFuncUtil  -fPIC
LIBS = -L../../../bin -ldl -lpthread -lcommframe -ltinyxml -lRealDB -lScadaProcess -lCFuncUtil $(EXTRA_LIBS)

SOURCES=Q_protocolIEC104.cpp \
				Q_IEC104CommandManager.cpp \
				Q_IEC104Setting.cpp \
				Q_104StandardTags.cpp \
				Q_104CP56Time2a.cpp \
				Q_protocolIEC104Master.cpp \
                                Q_104SlaveTask.cpp \
				Q_ProtocolIEC104Slave.cpp \
				CommIEC104.cpp \
				Q_protocolIEC104SlaveShanDongPlaneCurve.cpp
   		

shared:
	$(C++) $(CFLAGS) $(SOURCES)  -fPIC -o ../../../bin/libCommIEC104.so -shared $(LIBS)
	
release:shared
	$(STRIP) -s ../../../bin/libCommIEC104.so

clean:
	rm -f *.o
	rm -f ../../../bin/libCommIEC104.so
