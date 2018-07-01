to_srcdir:=../../
include $(to_srcdir)make.rules
#-Wall
DEBUG=-g
CFLAGS =$(OPTIMIZE_FLAG) $(BYTE_PRECISION_DEF) -std=c++0x $(OS_DEF) $(DEBUG) -D_TS_ERRNO -D_THREAD_SAFE -D_REENTRANT  -I../../Include/tinyxmlInclude -I../../CFuncUtil\
-I. -I../include -I../headfile -fPIC
LIBS = -L../../../bin  -lnsl -lpthread -ltinyxml  $(EXTRA_LIBS)


#LFLAGS = -shared -lnsl -lpthread -g
SOURCES=ShareMemory.cpp \
	MutexEx.cpp \
	EventEx.cpp \
	ThreadMutex.cpp \
	ThreadEvent.cpp \
	DataValue.cpp \
	SocketEx.cpp \
	Formula.cpp \
	DBField.cpp \
	DBTable.cpp \
	DataBase.cpp \
        ComputerWatch.cpp \
	MultiDataBase.cpp \
	SysManager.cpp \
	SystemTable.cpp \
	ProcessEvent.cpp \
	CmdProcess.cpp \
	CmdToRtServer.cpp \
	RtdbDefFile.cpp \
	MsgDealer.cpp \
	TcpListener.cpp \
	TcpReceiver.cpp \
	TcpWorker.cpp \
	UdpWorker.cpp \
	MUdpWorker.cpp \
	gFunction.cpp \
	ID32Base.cpp \
	RtdbFile.cpp \
	md5.c \
	RtdbTabSave.cpp \
        RtdbFtp.cpp \
        RtdbDefFileXml.cpp\
         RtdbFileXml.cpp \
	CLogUtil.cpp


shared:
	$(C++) $(CFLAGS) $(SOURCES)  -fPIC -o ../../../bin/libRealDB.so  -shared $(LIBS)
	
release:shared
	$(STRIP) -s ../../../bin/libRealDB.so 

clean:
	rm -f *.o
	rm -f ../../../bin/libRealDB.so
