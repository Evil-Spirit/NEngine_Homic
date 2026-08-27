# Microsoft Developer Studio Project File - Name="NEngine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NEngine - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NEngine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NEngine.mak" CFG="NEngine - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NEngine - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NEngine - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NEngine - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "NEngine_"
# PROP BASE Intermediate_Dir "NEngine_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "NEngine_"
# PROP Intermediate_Dir "NEngine_"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib comctl32.lib wsock32.lib glaux.lib winmm.lib /nologo /subsystem:windows /machine:I386 /out:"NEngine.exe"

!ELSEIF  "$(CFG)" == "NEngine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib comctl32.lib wsock32.lib glaux.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "NEngine - Win32 Release"
# Name "NEngine - Win32 Debug"
# Begin Group "RESOURCES"

# PROP Default_Filter "ico bmp rc cur"
# Begin Source File

SOURCE=.\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\Res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Evil.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Logo.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\nEn.ico
# End Source File
# Begin Source File

SOURCE=.\Res\nEngine.ico
# End Source File
# Begin Source File

SOURCE=.\res.rc

!IF  "$(CFG)" == "NEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NEngine - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\engConsole.h
# End Source File
# Begin Source File

SOURCE=.\engConsts.h
# End Source File
# Begin Source File

SOURCE=.\engImage.h
# End Source File
# Begin Source File

SOURCE=.\engInput.h
# End Source File
# Begin Source File

SOURCE=.\engLog.h
# End Source File
# Begin Source File

SOURCE=.\engMain.h
# End Source File
# Begin Source File

SOURCE=.\engMisc.h
# End Source File
# Begin Source File

SOURCE=.\engModel.h
# End Source File
# Begin Source File

SOURCE=.\engMulTex.h
# End Source File
# Begin Source File

SOURCE=.\engParticle.h
# End Source File
# Begin Source File

SOURCE=.\engResource.h
# End Source File
# Begin Source File

SOURCE=.\engText.h
# End Source File
# Begin Source File

SOURCE=.\engTypes.h
# End Source File
# Begin Source File

SOURCE=.\engUser.h
# End Source File
# Begin Source File

SOURCE=.\engVars.h
# End Source File
# Begin Source File

SOURCE=.\engVector.h
# End Source File
# Begin Source File

SOURCE=.\Execute.cpp
# End Source File
# End Target
# End Project
