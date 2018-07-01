to_srcdir:=../../
include $(to_srcdir)make.rules
#-Wall
DEBUG=-g
CFLAGS =$(OPTIMIZE_FLAG) $(BYTE_PRECISION_DEF) $(OS_DEF) $(DEBUG) -std=c++0x -D_TS_ERRNO -D_THREAD_SAFE -D_REENTRANT  -I../../Include/tinyxmlInclude -I../../rtdb/include -I../../rtdb/headfile \
-I. -I../../support/include -I../../support/headfile -I../include   -fPIC 
LIBS = -L../../../bin -lnsl -lpthread -ltinyxml $(EXTRA_LIBS)

SOURCES=FaultMsg.cpp \
	ScadaAPIs.cpp \
	Task.cpp \
	UpdateFields.cpp \
	ScadaProcess.cpp


shared:
	$(C++) $(CFLAGS) $(SOURCES)  -fPIC -o ../../../bin/libScadaProcess.so  -shared $(LIBS)
	
release:shared
	$(STRIP) -s ../../../bin/libScadaProcess.so 

clean:
	rm -f *.o
	rm -f ../../../bin/libScadaProcess.so
