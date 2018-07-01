to_srcdir:=../
include $(to_srcdir)make.rules
DEBUG=-g
CFLAGS = $(OPTIMIZE_FLAG) $(BYTE_PRECISION_DEF) $(OS_DEF) -D_TS_ERRNO -Wall -std=c++0x $(DEBUG) -I../Include -I../Include/commFrameInclude -I../Include/tinyxmlInclude -I../rtdb/headfile -I../rtdb/include -I../scada/include -I../CFuncUtil -fPIC 
LIBS = -L../../bin -lRealDB -lScadaProcess -lcommframe -ltinyxml -lCFuncUtil -ldl -lpthread  $(EXTRA_LIBS) 
OBJS = 	Log.o \
        database.o \
	CLibLoader.o \
        Q_GSetting.o \
	CRunManager.o \
	ccsmain.o 

.cpp.o : Log.h \
        database.h \
	CLibLoader.h \
	globaldef.h \
	linearhash.h \
	Q_GSetting.h \
	sflatstorage.h \
	CRunManager.h \
	flatstorage.h \
        GlobalDefine.h 
	$(CC) $(CFLAGS) -c -o $*.o $<  

ccsapp:$(OBJS)
	$(C++) $(CFLAGS) -o ../../bin/fep $(OBJS) $(LIBS)

release:cscapp
	$(STRIP) ../../bin/fep

clean:
	rm -f *.o
	rm -f ../../bin/fep
