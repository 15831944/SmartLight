# Microsoft Developer Studio Project File - Name="RtdbDll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RtdbDll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RtdbDll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RtdbDll.mak" CFG="RtdbDll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RtdbDll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RtdbDll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/support/RTDBDLL", KAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RtdbDll - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Release\Support\RtdbDll"
# PROP Intermediate_Dir "..\..\Release\Support\RtdbDll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\headfile" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /def:".\RtdbDll.def" /out:"..\..\bin/RtdbDll_r.dll" /implib:"..\lib/RtdbDll_r.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RtdbDll - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Debug\Support\RtdbDll"
# PROP Intermediate_Dir "..\..\..\..\Debug\Support\RtdbDll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\headfile" /I "..\..\logmanager\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "WINDOWS_ENV" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\RtdbDll.def" /out:"..\..\../../bin\RealDB.dll" /implib:"..\lib/RealDB.lib" /pdbtype:sept /libpath:"..\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "RtdbDll - Win32 Release"
# Name "RtdbDll - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BaseSql.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdToRtServer.cpp
# End Source File
# Begin Source File

SOURCE=.\ComputerWatch.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DataValue.cpp
# End Source File
# Begin Source File

SOURCE=.\DBField.cpp
# End Source File
# Begin Source File

SOURCE=.\DBTable.cpp
# End Source File
# Begin Source File

SOURCE=.\EventEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Formula.cpp
# End Source File
# Begin Source File

SOURCE=.\gFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\GoBackOS.cpp
# End Source File
# Begin Source File

SOURCE=.\ID32Base.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgDealer.cpp
# End Source File
# Begin Source File

SOURCE=.\MUdpWorker.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiDataBase.cpp
# End Source File
# Begin Source File

SOURCE=.\MutexEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\RtdbDefFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RtdbDll.cpp
# End Source File
# Begin Source File

SOURCE=.\RtdbDll.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\RtdbDll.rc
# End Source File
# Begin Source File

SOURCE=.\RtdbFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RtdbFtp.cpp
# End Source File
# Begin Source File

SOURCE=.\RtdbSql.cpp
# End Source File
# Begin Source File

SOURCE=.\ShareMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SysManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemTable.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpListener.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpReceiver.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpWorker.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadMutex.cpp
# End Source File
# Begin Source File

SOURCE=.\UdpWorker.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\BaseSql.h
# End Source File
# Begin Source File

SOURCE=..\include\CmdProcess.h
# End Source File
# Begin Source File

SOURCE=..\include\CmdToRtServer.h
# End Source File
# Begin Source File

SOURCE=..\include\ComputerWatch.h
# End Source File
# Begin Source File

SOURCE=..\include\DataBase.h
# End Source File
# Begin Source File

SOURCE=..\include\DataValue.h
# End Source File
# Begin Source File

SOURCE=..\include\DBField.h
# End Source File
# Begin Source File

SOURCE=..\include\DBTable.h
# End Source File
# Begin Source File

SOURCE=..\include\EventEx.h
# End Source File
# Begin Source File

SOURCE=..\include\Formula.h
# End Source File
# Begin Source File

SOURCE=..\include\GoBackOS.h
# End Source File
# Begin Source File

SOURCE=.\ID32Base.h
# End Source File
# Begin Source File

SOURCE=..\include\MsgDealer.h
# End Source File
# Begin Source File

SOURCE=.\MUdpWorker.h
# End Source File
# Begin Source File

SOURCE=..\include\MultiDataBase.h
# End Source File
# Begin Source File

SOURCE=..\include\MutexEx.h
# End Source File
# Begin Source File

SOURCE=..\include\ProcessEvent.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RtdbDefFile.h
# End Source File
# Begin Source File

SOURCE=..\include\RtdbFile.h
# End Source File
# Begin Source File

SOURCE=..\include\RtdbFtp.h
# End Source File
# Begin Source File

SOURCE=..\include\RtdbSql.h
# End Source File
# Begin Source File

SOURCE=..\include\ShareMemory.h
# End Source File
# Begin Source File

SOURCE=..\include\SocketEx.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TcpListener.h
# End Source File
# Begin Source File

SOURCE=.\TcpReceiver.h
# End Source File
# Begin Source File

SOURCE=.\TcpWorker.h
# End Source File
# Begin Source File

SOURCE=..\include\ThreadEvent.h
# End Source File
# Begin Source File

SOURCE=..\include\ThreadMutex.h
# End Source File
# Begin Source File

SOURCE=.\UdpWorker.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\RtdbDll.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
