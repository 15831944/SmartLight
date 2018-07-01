to_srcdir:=../
include $(to_srcdir)make.rules

CFLAGS =$(OPTIMIZE_FLAG) $(BYTE_PRECISION_DEF) $(OS_DEF) -D_TS_ERRNO -Wall -std=c++0x -I../rtdb/include -I../Include/commFrameInclude  -fPIC
LIBS = -L../../../bin -ldl -lpthread $(EXTRA_LIBS)


SOURCES=CBrowseDir.cpp CFuncUtil.cpp TimerTask.cpp DelteLog.cpp
shared:
	$(C++) $(CFLAGS) $(SOURCES)  -fPIC -o ../../bin/libCFuncUtil.so -shared $(LIBS)
	
release:shared
	$(STRIP) -s ../../bin/libCFuncUtil.so

clean:
	rm -f *.o
	rm -f ../../bin/libCFuncUtil.so
