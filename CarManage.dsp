# Microsoft Developer Studio Project File - Name="CarManage" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CarManage - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CarManage.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CarManage.mak" CFG="CarManage - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CarManage - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CarManage - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CarManage - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "CarManage - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CarManage - Win32 Release"
# Name "CarManage - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ADOConn.cpp
# End Source File
# Begin Source File

SOURCE=.\CarDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CarInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CarManage.cpp
# End Source File
# Begin Source File

SOURCE=.\CarManage.rc
# End Source File
# Begin Source File

SOURCE=.\CarManageDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CarManageView.cpp
# End Source File
# Begin Source File

SOURCE=.\CarOpt.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomControlSite.cpp
# End Source File
# Begin Source File

SOURCE=.\DeptOpt.cpp
# End Source File
# Begin Source File

SOURCE=.\DriverOpt.cpp
# End Source File
# Begin Source File

SOURCE=.\dtpicker.cpp
# End Source File
# Begin Source File

SOURCE=.\EditCarDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditTargetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\GpsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImpIDispatch.cpp
# End Source File
# Begin Source File

SOURCE=.\InOutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InOutOpt.cpp
# End Source File
# Begin Source File

SOURCE=.\LogDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ManageCarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MD5Checksum.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\NewRoleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewUserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayBackDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RoleManageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RoleOpt.cpp
# End Source File
# Begin Source File

SOURCE=.\SizingDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SizingSP.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TargetOpt.cpp
# End Source File
# Begin Source File

SOURCE=.\TeamOpt.cpp
# End Source File
# Begin Source File

SOURCE=.\TMDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TraceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserOpt.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.cpp
# End Source File
# Begin Source File

SOURCE=.\WebPage.cpp
# End Source File
# Begin Source File

SOURCE=.\XCListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\XComboBox.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ADOConn.h
# End Source File
# Begin Source File

SOURCE=.\CarDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\CarInfo.h
# End Source File
# Begin Source File

SOURCE=.\CarManage.h
# End Source File
# Begin Source File

SOURCE=.\CarManageDoc.h
# End Source File
# Begin Source File

SOURCE=.\CarManageView.h
# End Source File
# Begin Source File

SOURCE=.\CarOpt.h
# End Source File
# Begin Source File

SOURCE=.\CustomControlSite.h
# End Source File
# Begin Source File

SOURCE=.\DeptOpt.h
# End Source File
# Begin Source File

SOURCE=.\DriverOpt.h
# End Source File
# Begin Source File

SOURCE=.\dtpicker.h
# End Source File
# Begin Source File

SOURCE=.\EditCarDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditTargetDlg.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\GpsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImpIDispatch.h
# End Source File
# Begin Source File

SOURCE=.\InOutDlg.h
# End Source File
# Begin Source File

SOURCE=.\InOutOpt.h
# End Source File
# Begin Source File

SOURCE=.\LogDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ManageCarDlg.h
# End Source File
# Begin Source File

SOURCE=.\MapDlg.h
# End Source File
# Begin Source File

SOURCE=.\MD5Checksum.h
# End Source File
# Begin Source File

SOURCE=.\MD5ChecksumDefines.h
# End Source File
# Begin Source File

SOURCE=.\MyTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\NewRoleDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewUserDlg.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\PlayBackDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RoleManageDlg.h
# End Source File
# Begin Source File

SOURCE=.\RoleOpt.h
# End Source File
# Begin Source File

SOURCE=.\SizingDialog.h
# End Source File
# Begin Source File

SOURCE=.\SizingSP.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TargetOpt.h
# End Source File
# Begin Source File

SOURCE=.\TeamOpt.h
# End Source File
# Begin Source File

SOURCE=.\TMDlg.h
# End Source File
# Begin Source File

SOURCE=.\TraceDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserOpt.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.h
# End Source File
# Begin Source File

SOURCE=.\WebPage.h
# End Source File
# Begin Source File

SOURCE=.\XCListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\XComboBox.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\img\car.ico
# End Source File
# Begin Source File

SOURCE=.\res\CarManage.ico
# End Source File
# Begin Source File

SOURCE=.\res\CarManage.rc2
# End Source File
# Begin Source File

SOURCE=.\res\CarManageDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section CarManage : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section CarManage : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
# Section CarManage : {20DD1B9B-87C4-11D1-8BE3-0000F8754DA1}
# 	2:5:Class:CDTPicker
# 	2:10:HeaderFile:dtpicker.h
# 	2:8:ImplFile:dtpicker.cpp
# End Section
# Section CarManage : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section CarManage : {20DD1B9E-87C4-11D1-8BE3-0000F8754DA1}
# 	2:21:DefaultSinkHeaderFile:dtpicker.h
# 	2:16:DefaultSinkClass:CDTPicker
# End Section
# Section CarManage : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section
