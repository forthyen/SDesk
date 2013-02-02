# Microsoft Developer Studio Project File - Name="libvlc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libvlc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libvlc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libvlc.mak" CFG="libvlc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libvlc - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libvlc - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libvlc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_MT" /D "_DLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /I "..\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_MT" /D "_DLL" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libvlc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_MT" /D "_DLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_MT" /D "_DLL" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libvlc - Win32 Release"
# Name "libvlc - Win32 Debug"

# Begin Group "Source Files"
# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "src"
# Begin Source File
SOURCE="..\.\src\libvlc.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\"
# PROP Intermediate_Dir "Release\.\src\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\"
# PROP Intermediate_Dir "Debug\.\src\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\libvlc.h"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\"
# PROP Intermediate_Dir "Release\.\src\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\"
# PROP Intermediate_Dir "Debug\.\src\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\interface\interface.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\interface\"
# PROP Intermediate_Dir "Release\.\src\interface\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\interface\"
# PROP Intermediate_Dir "Debug\.\src\interface\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\interface\intf_eject.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\interface\"
# PROP Intermediate_Dir "Release\.\src\interface\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\interface\"
# PROP Intermediate_Dir "Debug\.\src\interface\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\playlist\playlist.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\playlist\"
# PROP Intermediate_Dir "Release\.\src\playlist\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\playlist\"
# PROP Intermediate_Dir "Debug\.\src\playlist\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\playlist\sort.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\playlist\"
# PROP Intermediate_Dir "Release\.\src\playlist\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\playlist\"
# PROP Intermediate_Dir "Debug\.\src\playlist\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\playlist\loadsave.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\playlist\"
# PROP Intermediate_Dir "Release\.\src\playlist\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\playlist\"
# PROP Intermediate_Dir "Debug\.\src\playlist\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\playlist\group.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\playlist\"
# PROP Intermediate_Dir "Release\.\src\playlist\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\playlist\"
# PROP Intermediate_Dir "Debug\.\src\playlist\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\playlist\item.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\playlist\"
# PROP Intermediate_Dir "Release\.\src\playlist\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\playlist\"
# PROP Intermediate_Dir "Debug\.\src\playlist\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\playlist\item-ext.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\playlist\"
# PROP Intermediate_Dir "Release\.\src\playlist\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\playlist\"
# PROP Intermediate_Dir "Debug\.\src\playlist\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\playlist\info.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\playlist\"
# PROP Intermediate_Dir "Release\.\src\playlist\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\playlist\"
# PROP Intermediate_Dir "Debug\.\src\playlist\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\input\access.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\input\"
# PROP Intermediate_Dir "Release\.\src\input\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\input\"
# PROP Intermediate_Dir "Debug\.\src\input\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\input\clock.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\input\"
# PROP Intermediate_Dir "Release\.\src\input\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\input\"
# PROP Intermediate_Dir "Debug\.\src\input\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\input\control.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\input\"
# PROP Intermediate_Dir "Release\.\src\input\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\input\"
# PROP Intermediate_Dir "Debug\.\src\input\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\input\decoder.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\input\"
# PROP Intermediate_Dir "Release\.\src\input\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\input\"
# PROP Intermediate_Dir "Debug\.\src\input\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\input\demux.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\input\"
# PROP Intermediate_Dir "Release\.\src\input\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\input\"
# PROP Intermediate_Dir "Debug\.\src\input\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\input\es_out.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\input\"
# PROP Intermediate_Dir "Release\.\src\input\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\input\"
# PROP Intermediate_Dir "Debug\.\src\input\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\input\input.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\input\"
# PROP Intermediate_Dir "Release\.\src\input\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\input\"
# PROP Intermediate_Dir "Debug\.\src\input\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\input\input_internal.h"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\input\"
# PROP Intermediate_Dir "Release\.\src\input\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\input\"
# PROP Intermediate_Dir "Debug\.\src\input\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\input\stream.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\input\"
# PROP Intermediate_Dir "Release\.\src\input\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\input\"
# PROP Intermediate_Dir "Debug\.\src\input\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\input\subtitles.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\input\"
# PROP Intermediate_Dir "Release\.\src\input\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\input\"
# PROP Intermediate_Dir "Debug\.\src\input\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\input\var.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\input\"
# PROP Intermediate_Dir "Release\.\src\input\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\input\"
# PROP Intermediate_Dir "Debug\.\src\input\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\video_output\video_output.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\video_output\"
# PROP Intermediate_Dir "Release\.\src\video_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\video_output\"
# PROP Intermediate_Dir "Debug\.\src\video_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\video_output\vout_pictures.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\video_output\"
# PROP Intermediate_Dir "Release\.\src\video_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\video_output\"
# PROP Intermediate_Dir "Debug\.\src\video_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\video_output\vout_pictures.h"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\video_output\"
# PROP Intermediate_Dir "Release\.\src\video_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\video_output\"
# PROP Intermediate_Dir "Debug\.\src\video_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\video_output\video_text.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\video_output\"
# PROP Intermediate_Dir "Release\.\src\video_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\video_output\"
# PROP Intermediate_Dir "Debug\.\src\video_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\video_output\video_widgets.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\video_output\"
# PROP Intermediate_Dir "Release\.\src\video_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\video_output\"
# PROP Intermediate_Dir "Debug\.\src\video_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\video_output\vout_subpictures.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\video_output\"
# PROP Intermediate_Dir "Release\.\src\video_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\video_output\"
# PROP Intermediate_Dir "Debug\.\src\video_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\video_output\vout_synchro.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\video_output\"
# PROP Intermediate_Dir "Release\.\src\video_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\video_output\"
# PROP Intermediate_Dir "Debug\.\src\video_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\video_output\vout_intf.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\video_output\"
# PROP Intermediate_Dir "Release\.\src\video_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\video_output\"
# PROP Intermediate_Dir "Debug\.\src\video_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\audio_output\common.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\audio_output\"
# PROP Intermediate_Dir "Release\.\src\audio_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\audio_output\"
# PROP Intermediate_Dir "Debug\.\src\audio_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\audio_output\dec.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\audio_output\"
# PROP Intermediate_Dir "Release\.\src\audio_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\audio_output\"
# PROP Intermediate_Dir "Debug\.\src\audio_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\audio_output\filters.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\audio_output\"
# PROP Intermediate_Dir "Release\.\src\audio_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\audio_output\"
# PROP Intermediate_Dir "Debug\.\src\audio_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\audio_output\input.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\audio_output\"
# PROP Intermediate_Dir "Release\.\src\audio_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\audio_output\"
# PROP Intermediate_Dir "Debug\.\src\audio_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\audio_output\mixer.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\audio_output\"
# PROP Intermediate_Dir "Release\.\src\audio_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\audio_output\"
# PROP Intermediate_Dir "Debug\.\src\audio_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\audio_output\output.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\audio_output\"
# PROP Intermediate_Dir "Release\.\src\audio_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\audio_output\"
# PROP Intermediate_Dir "Debug\.\src\audio_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\audio_output\intf.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\audio_output\"
# PROP Intermediate_Dir "Release\.\src\audio_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\audio_output\"
# PROP Intermediate_Dir "Debug\.\src\audio_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\stream_output\stream_output.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\stream_output\"
# PROP Intermediate_Dir "Release\.\src\stream_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\stream_output\"
# PROP Intermediate_Dir "Debug\.\src\stream_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\stream_output\announce.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\stream_output\"
# PROP Intermediate_Dir "Release\.\src\stream_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\stream_output\"
# PROP Intermediate_Dir "Debug\.\src\stream_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\stream_output\sap.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\stream_output\"
# PROP Intermediate_Dir "Release\.\src\stream_output\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\stream_output\"
# PROP Intermediate_Dir "Debug\.\src\stream_output\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\charset.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\httpd.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\tls.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\mtime.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\block.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\modules.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\threads.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\cpu.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\configuration.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\iso_lang.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\iso-639_def.h"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\messages.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\objects.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\variables.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\error.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\net.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\vlm.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\extras\libc.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\extras\"
# PROP Intermediate_Dir "Release\.\src\extras\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\extras\"
# PROP Intermediate_Dir "Debug\.\src\extras\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\misc\win32_specific.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\misc\"
# PROP Intermediate_Dir "Release\.\src\misc\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\misc\"
# PROP Intermediate_Dir "Debug\.\src\misc\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\extras\dirent.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\extras\"
# PROP Intermediate_Dir "Release\.\src\extras\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\extras\"
# PROP Intermediate_Dir "Debug\.\src\extras\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\extras\dirent.h"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\extras\"
# PROP Intermediate_Dir "Release\.\src\extras\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\extras\"
# PROP Intermediate_Dir "Debug\.\src\extras\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\extras\getopt.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\extras\"
# PROP Intermediate_Dir "Release\.\src\extras\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\extras\"
# PROP Intermediate_Dir "Debug\.\src\extras\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\extras\getopt.h"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\extras\"
# PROP Intermediate_Dir "Release\.\src\extras\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\extras\"
# PROP Intermediate_Dir "Debug\.\src\extras\"
!ENDIF
# End Source File
# Begin Source File
SOURCE="..\.\src\extras\getopt1.c"
# ADD CPP /D "__VLC__" /D PLUGIN_PATH=\".\" /D DATA_PATH=\"share\"
!IF "$(CFG)" == "libvlc - Win32 Release"
# PROP Output_Dir "Release\.\src\extras\"
# PROP Intermediate_Dir "Release\.\src\extras\"
!ELSEIF "$(CFG)" == "libvlc - Win32 Debug"
# PROP Output_Dir "Debug\.\src\extras\"
# PROP Intermediate_Dir "Debug\.\src\extras\"
!ENDIF
# End Source File
# End Group
# End Group
# Begin Group "Header Files"
# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File
SOURCE="..\.\include\aout_internal.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\audio_output.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\beos_specific.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\charset.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\codecs.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\configuration.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\darwin_specific.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\intf_eject.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\iso_lang.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\main.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\mmx.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\modules.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\modules_inner.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\mtime.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\network.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\osd.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\os_specific.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\snapshot.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\stream_output.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\variables.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\video_output.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_access.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_bits.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_block.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_block_helper.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_codec.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_common.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_config.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_cpu.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_demux.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_error.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_es.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_es_out.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_filter.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_help.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_httpd.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_tls.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_input.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_interface.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_keys.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_messages.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_meta.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_objects.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_playlist.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_spu.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_stream.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_threads_funcs.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_threads.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_video.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_vlm.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_vod.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vout_synchro.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\win32_specific.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc_symbols.h"
# End Source File
# Begin Group "vlc"
# Begin Source File
SOURCE="..\.\include\vlc\vlc.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc\aout.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc\vout.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc\sout.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc\decoder.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc\input.h"
# End Source File
# Begin Source File
SOURCE="..\.\include\vlc\intf.h"
# End Source File
# End Group
# End Group
# End Target
# End Project
