to_srcdir:=../../
include $(to_srcdir)make.rules
DEBUG=-g
CFLAGS =$(OPTIMIZE_FLAG) $(BYTE_PRECISION_DEF) $(OS_DEF) -D_TS_ERRNO -Wall -std=c++0x $(DEBUG) -I../../rtdb/include -I../../rtdb/headfile -I../../scada/include  -I../../Include/commFrameInclude -I../Include/IEC103Include\
-I../../Include/tinyxmlInclude -fPIC
LIBS = -L../../../bin -ldl -lpthread -lcommframe -ltinyxml -lRealDB -lScadaProcess $(EXTRA_LIBS)

SOURCES=Q_103SlaveTask.cpp \
		Q_ProtocolIEC103Slave.cpp \
		CommIEC103.cpp \
		Q_103CP56Time2a.cpp \
		Q_103StandardTags.cpp \
		Q_IEC103CommandManager.cpp \
		Q_IEC103Setting.cpp \
		Q_protocolIEC103.cpp 
		

shared:
	$(C++) $(CFLAGS) $(SOURCES)  -fPIC -o ../../../bin/libCommIEC103.so -shared $(LIBS)
	
release:shared
	$(STRIP) -s ../../../bin/libCommIEC103.so

clean:
	rm -f *.o
	rm -f ../../../bin/libCommIEC103.so
