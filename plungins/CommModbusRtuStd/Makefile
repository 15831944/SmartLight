to_srcdir:=../../
include $(to_srcdir)make.rules
DEBUG=-g
CFLAGS =$(OPTIMIZE_FLAG) $(BYTE_PRECISION_DEF) $(OS_DEF) -D_TS_ERRNO -Wall -std=c++0x $(DEBUG) -I../../rtdb/include -I../../rtdb/headfile -I../../scada/include  -I../../Include/commFrameInclude -I../Include/modbusInclude  -I../../CFuncUtil \
-I../../Include/tinyxmlInclude -fPIC
LIBS = -L../../../bin  -ldl -lpthread -lcommframe -ltinyxml -lRealDB -lScadaProcess -lCFuncUtil  $(EXTRA_LIBS)

SOURCES=Q_protocolModbusRtuStd.cpp

shared:
	$(C++) $(CFLAGS) $(SOURCES)  -fPIC -o ../../../bin/libCommModbusRtuStd.so -shared $(LIBS)
	
release:shared
	$(STRIP) -s ../../../bin/libCommModbusRtuStd.so

clean:
	rm -f *.o
	rm -f ../../../bin/libCommModbusRtuStd.so
