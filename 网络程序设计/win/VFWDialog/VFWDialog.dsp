# Microsoft Developer Studio Project File - Name="GraghDialog" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GraghDialog - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VFWDialog.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VFWDialog.mak" CFG="GraghDialog - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GraghDialog - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GraghDialog - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GraghDialog - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GraghDialog - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /i "." /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "GraghDialog - Win32 Release"
# Name "GraghDialog - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddRemote.cpp
# End Source File
# Begin Source File

SOURCE=.\AddressBook.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioPlay.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioRec.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\DSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\G729aCompress.cpp
# End Source File
# Begin Source File

SOURCE=.\GraghDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\GraghDialog.rc
# End Source File
# Begin Source File

SOURCE=.\GraghDialogDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextChatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoCapture.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoCodec.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddRemote.h
# End Source File
# Begin Source File

SOURCE=.\AddressBook.h
# End Source File
# Begin Source File

SOURCE=.\AudioCodec.h
# End Source File
# Begin Source File

SOURCE=.\AudioPlay.h
# End Source File
# Begin Source File

SOURCE=.\AudioRec.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\DSocket.h
# End Source File
# Begin Source File

SOURCE=.\G729aCompress.h
# End Source File
# Begin Source File

SOURCE=.\GraghDialog.h
# End Source File
# Begin Source File

SOURCE=.\GraghDialogDlg.h
# End Source File
# Begin Source File

SOURCE=.\RemoteDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextChatDlg.h
# End Source File
# Begin Source File

SOURCE=.\VideoCapture.h
# End Source File
# Begin Source File

SOURCE=.\VideoCodec.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ABOUTIN.ico
# End Source File
# Begin Source File

SOURCE=.\res\aboutout.ico
# End Source File
# Begin Source File

SOURCE=.\res\addremot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\call.ico
# End Source File
# Begin Source File

SOURCE=.\res\callin.ico
# End Source File
# Begin Source File

SOURCE=.\res\GraghDialog.ico
# End Source File
# Begin Source File

SOURCE=.\res\GraghDialog.rc2
# End Source File
# Begin Source File

SOURCE=.\res\hang.ico
# End Source File
# Begin Source File

SOURCE=.\res\hangin.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\main.bmp
# End Source File
# Begin Source File

SOURCE=.\res\min.ico
# End Source File
# Begin Source File

SOURCE=.\res\minin.ico
# End Source File
# Begin Source File

SOURCE=.\res\minout.ico
# End Source File
# Begin Source File

SOURCE=.\res\ok.bmp
# End Source File
# Begin Source File

SOURCE=.\res\okdis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\okdown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OKUP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\quit.ico
# End Source File
# Begin Source File

SOURCE=.\res\QuitDown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\quitin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\quitin.ico
# End Source File
# Begin Source File

SOURCE=.\res\quitout.bmp
# End Source File
# Begin Source File

SOURCE=.\res\quitout.ico
# End Source File
# Begin Source File

SOURCE=.\res\title.bmp
# End Source File
# Begin Source File

SOURCE=.\res\titleout.bmp
# End Source File
# Begin Source File

SOURCE=.\res\zoom.ico
# End Source File
# Begin Source File

SOURCE=.\res\zoomin.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
