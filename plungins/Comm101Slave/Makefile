to_srcdir:=../../
include $(to_srcdir)make.rules
DEBUG=-g
CFLAGS =$(OPTIMIZE_FLAG) $(BYTE_PRECISION_DEF) $(OS_DEF) -D_TS_ERRNO -Wall -std=c++0x $(DEBUG) -I../../rtdb/include -I../../rtdb/headfile -I../../scada/include  -I../../Include/commFrameInclude \
-I../../Include/tinyxmlInclude -fPIC
LIBS = -L../../../bin -ldl -lpthread -lcommframe -ltinyxml -lRealDB -lScadaProcess $(EXTRA_LIBS)

SOURCES=Q_protocol101Slave.cpp

shared:
	$(C++) $(CFLAGS) $(SOURCES)  -fPIC -o ../../../bin/libComm101Slave.so -shared $(LIBS)
	
release:shared
	$(STRIP) -s ../../../bin/libComm101Slave.so

clean:
	rm -f *.o
	rm -f ../../../bin/libComm101Slave.so
