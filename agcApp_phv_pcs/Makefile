to_srcdir:=../
include $(to_srcdir)make.rules
CFLAGS = $(OPTIMIZE_FLAG) $(BYTE_PRECISION_DEF) $(OS_DEF) -D_TS_ERRNO -Wall -std=c++0x $(DEBUG) -I../Include -I../Include/commFrameInclude -I../Include/tinyxmlInclude -I../rtdb/headfile -I../rtdb/include -I../scada/include -I../CFuncUtil -fPIC 
LIBS = -L../../bin -lRealDB -lScadaProcess -lcommframe -ltinyxml -lCFuncUtil -ldl -lpthread  $(EXTRA_LIBS) 
OBJS = 	Q_PlanXml.o \
        agcStructure.o \
	agcFuncRtdb.o \
        AgcDataBase.o \
	agcCalc.o\
	main.o

.cpp.o : Q_PlanXml.h \
        agcStructure.h \
	agcFuncRtdb.h \
	AgcDataBase.h \
	agcCalc.h \
	Inverter.h \
	agcDefine.h \
	AgcAvcDefine.h \
	gSystemDef.h
	
	$(CC) $(CFLAGS) -c -o $*.o $<  

agcApp:$(OBJS)
	$(C++) $(CFLAGS) -o ../../bin/agcApp_phv $(OBJS) $(LIBS)

release:agcApp
	$(STRIP) ../../bin/agcApp_phv

clean:
	rm -f *.o
	rm -f ../../bin/agcApp_phv
