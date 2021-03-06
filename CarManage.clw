; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTMDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CarManage.h"
LastPage=0

ClassCount=23
Class1=CCarManageApp
Class2=CCarManageDoc
Class3=CCarManageView
Class4=CMainFrame

ResourceCount=18
Resource1=IDR_MAINFRAME
Resource2=IDD_GPSDLG
Class5=CAboutDlg
Resource3=IDD_ABOUTBOX
Class6=CMapDlg
Resource4=IDD_INOUTDLG
Class7=CRoleManageDlg
Resource5=IDD_EDITCARDATADLG
Class8=CCarDataDlg
Resource6=IDD_TMDLG
Class9=CLogDlg
Resource7=IDD_ROLEDLG
Class10=CNewRoleDlg
Resource8=IDD_NEWUSERDLG
Class11=CGpsDlg
Resource9=IDD_LOGDLG
Class12=CTraceDlg
Resource10=IDD_TRACEDLG
Class13=CPlayBack
Class14=CPlayBackDlg
Resource11=IDD_PLAYBACKDLG
Class15=CCarInfo
Resource12=IDD_DIALOG1
Class16=CNewUserDlg
Resource13=IDD_NEWROLEDLG
Class17=CEditCarDataDlg
Resource14=IDD_CARDATADLG
Class18=CManageCarDlg
Resource15=IDD_CARINFO
Class19=CInOutDlg
Class20=CXComboBox
Class21=CXCListCtrl
Resource16=IDD_MANAGECAR
Class22=CTMDlg
Resource17=IDD_CARMANAGE_FORM
Class23=CEditTargetDlg
Resource18=IDD_EDITTARGETDLG

[CLS:CCarManageApp]
Type=0
HeaderFile=CarManage.h
ImplementationFile=CarManage.cpp
Filter=N

[CLS:CCarManageDoc]
Type=0
HeaderFile=CarManageDoc.h
ImplementationFile=CarManageDoc.cpp
Filter=N

[CLS:CCarManageView]
Type=0
HeaderFile=CarManageView.h
ImplementationFile=CarManageView.cpp
Filter=D
BaseClass=CFormView
VirtualFilter=VWC
LastObject=IDC_WELCOMELAB


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_MENUITEM32772
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=CarManage.cpp
ImplementationFile=CarManage.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_MENUITEM32771
Command2=ID_MENUITEM32772
Command3=ID_MENUITEM32773
Command4=ID_MENUITEM32774
Command5=ID_APP_ABOUT
CommandCount=5

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_CARMANAGE_FORM]
Type=1
Class=CCarManageView
ControlCount=8
Control1=IDC_TAB1,SysTabControl32,1342177280
Control2=IDC_WELCOMELAB,static,1342308352
Control3=IDC_MODIPW,static,1342308608
Control4=IDC_ONE,static,1342308352
Control5=IDC_TWO,static,1342308352
Control6=IDC_RELOGIN,static,1342308608
Control7=IDC_THREE,static,1342308352
Control8=IDC_EXIT,static,1342308608

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[CLS:CMapDlg]
Type=0
HeaderFile=MapDlg.h
ImplementationFile=MapDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CMapDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG1]
Type=1
Class=CMapDlg
ControlCount=2
Control1=IDC_EXPLORER1,{8856F961-340A-11D0-A96B-00C04FD705A2},1342242816
Control2=IDC_TAB1,SysTabControl32,1342177280

[DLG:IDD_ROLEDLG]
Type=1
Class=CRoleManageDlg
ControlCount=13
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_BUTTON1,button,1342242816
Control4=IDC_BUTTON2,button,1342242816
Control5=IDC_BUTTON3,button,1342242816
Control6=IDC_LIST1,listbox,1352728835
Control7=IDC_LIST2,SysListView32,1350631435
Control8=IDC_BUTTON5,button,1342242816
Control9=IDC_BUTTON6,button,1342242816
Control10=IDC_TREE1,SysTreeView32,1350635815
Control11=IDC_BUTTON7,button,1342242816
Control12=IDC_BUTTON8,button,1342242816
Control13=IDC_BUTTON4,button,1342242816

[CLS:CRoleManageDlg]
Type=0
HeaderFile=RoleManageDlg.h
ImplementationFile=RoleManageDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CRoleManageDlg
VirtualFilter=dWC

[DLG:IDD_CARDATADLG]
Type=1
Class=CCarDataDlg
ControlCount=9
Control1=IDC_LIST1,SysListView32,1350631425
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT2,edit,1350631552
Control4=IDC_BUTTON1,button,1342242816
Control5=IDC_BUTTON2,button,1342242816
Control6=IDC_BUTTON3,button,1342242816
Control7=IDC_BUTTON4,button,1342242816
Control8=IDC_BUTTON5,button,1342242816
Control9=IDC_TAB1,SysTabControl32,1342177280

[CLS:CCarDataDlg]
Type=0
HeaderFile=CarDataDlg.h
ImplementationFile=CarDataDlg.cpp
BaseClass=CSizingDialog
Filter=D
LastObject=IDC_LIST1
VirtualFilter=dWC

[DLG:IDD_LOGDLG]
Type=1
Class=CLogDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUTTON3,button,1342242816
Control4=IDC_BUTTON4,button,1342242816
Control5=IDC_STATIC1,static,1342308352
Control6=IDC_STATIC2,static,1342308352
Control7=IDC_EDIT1,edit,1350631552
Control8=IDC_EDIT2,edit,1350631584
Control9=IDC_STATIC3,static,1342308352

[CLS:CLogDlg]
Type=0
HeaderFile=LogDlg.h
ImplementationFile=LogDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CLogDlg
VirtualFilter=dWC

[DLG:IDD_NEWROLEDLG]
Type=1
Class=CNewRoleDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552

[CLS:CNewRoleDlg]
Type=0
HeaderFile=NewRoleDlg.h
ImplementationFile=NewRoleDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT1

[CLS:CGpsDlg]
Type=0
HeaderFile=GpsDlg.h
ImplementationFile=GpsDlg.cpp
BaseClass=CSizingDialog
Filter=D
LastObject=IDC_TREE1
VirtualFilter=dWC

[DLG:IDD_GPSDLG]
Type=1
Class=CGpsDlg
ControlCount=4
Control1=IDC_TREE1,SysTreeView32,1350631463
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON4,button,1342242816

[DLG:IDD_TRACEDLG]
Type=1
Class=CTraceDlg
ControlCount=1
Control1=IDC_EXPLORER1,{8856F961-340A-11D0-A96B-00C04FD705A2},1342242816

[CLS:CTraceDlg]
Type=0
HeaderFile=TraceDlg.h
ImplementationFile=TraceDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CTraceDlg
VirtualFilter=dWC

[CLS:CPlayBack]
Type=0
HeaderFile=PlayBack.h
ImplementationFile=PlayBack.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CPlayBack

[DLG:IDD_PLAYBACKDLG]
Type=1
Class=CPlayBackDlg
ControlCount=24
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC4,static,1342308352
Control4=IDC_STATIC5,static,1342308352
Control5=IDC_STATICS1,static,1342308352
Control6=IDC_STATICE2,static,1342308352
Control7=IDC_COMBO4,combobox,1344340994
Control8=IDC_STATIC9,static,1342308352
Control9=IDC_STATIC10,static,1342308352
Control10=IDC_COMBO8,combobox,1344340994
Control11=IDC_COMBO12,combobox,1344340994
Control12=IDC_COMBO9,combobox,1344340994
Control13=IDC_BUTTON3,button,1342242816
Control14=IDC_DTPICKER1,{20DD1B9E-87C4-11D1-8BE3-0000F8754DA1},1342242816
Control15=IDC_DTPICKER2,{20DD1B9E-87C4-11D1-8BE3-0000F8754DA1},1342242816
Control16=IDC_EXPLORER1,{8856F961-340A-11D0-A96B-00C04FD705A2},1342242816
Control17=IDC_PROGRESS1,msctls_progress32,1350565888
Control18=IDC_BUTTON1,button,1342242816
Control19=IDC_STATIC6,static,1342308352
Control20=IDC_STATIC11,static,1342308352
Control21=IDC_COMBO13,combobox,1344340994
Control22=IDC_COMBO10,combobox,1344340994
Control23=IDC_BUTTON4,button,1342242816
Control24=IDC_STATIC13,static,1342308352

[CLS:CPlayBackDlg]
Type=0
HeaderFile=PlayBackDlg.h
ImplementationFile=PlayBackDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_msgLab
VirtualFilter=dWC

[DLG:IDD_CARINFO]
Type=1
Class=CCarInfo
ControlCount=2
Control1=IDC_BUTTON3,button,1342242816
Control2=IDC_LIST2,SysListView32,1350631437

[CLS:CCarInfo]
Type=0
HeaderFile=CarInfo.h
ImplementationFile=CarInfo.cpp
BaseClass=CSizingDialog
Filter=D
LastObject=IDC_LIST2
VirtualFilter=dWC

[DLG:IDD_NEWUSERDLG]
Type=1
Class=CNewUserDlg
ControlCount=22
Control1=IDC_EDIT2,edit,1350633600
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_COMBO1,combobox,1344340226
Control4=IDC_DTPICKER1,{20DD1B9E-87C4-11D1-8BE3-0000F8754DA1},1342242816
Control5=IDC_EDIT9,edit,1350631552
Control6=IDC_EDIT18,edit,1350631552
Control7=IDC_EDIT19,edit,1350631552
Control8=IDC_COMBO2,combobox,1344340226
Control9=IDC_DTPICKER2,{20DD1B9E-87C4-11D1-8BE3-0000F8754DA1},1342242816
Control10=IDC_EDIT12,edit,1350631552
Control11=IDOK,button,1342242817
Control12=IDCANCEL,button,1342242816
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352

[CLS:CNewUserDlg]
Type=0
HeaderFile=NewUserDlg.h
ImplementationFile=NewUserDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CNewUserDlg
VirtualFilter=dWC

[DLG:IDD_EDITCARDATADLG]
Type=1
Class=CEditCarDataDlg
ControlCount=22
Control1=IDC_CODING,edit,1350631552
Control2=IDC_NUMBER,edit,1350631552
Control3=IDC_CARBRAND,combobox,1344340226
Control4=IDC_CARTYPE,combobox,1344340226
Control5=IDC_BELONGDEPT,combobox,1344340226
Control6=IDC_CARSTATUS,combobox,1344339970
Control7=IDC_COMBO4,combobox,1344340226
Control8=IDC_BUYTIME,SysDateTimePick32,1342242848
Control9=IDC_ENTERTIME,SysDateTimePick32,1342242848
Control10=IDC_COMBO5,combobox,1344340226
Control11=IDOK,button,1342242817
Control12=IDCANCEL,button,1342242816
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352

[CLS:CEditCarDataDlg]
Type=0
HeaderFile=EditCarDataDlg.h
ImplementationFile=EditCarDataDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMBO5
VirtualFilter=dWC

[DLG:IDD_MANAGECAR]
Type=1
Class=CManageCarDlg
ControlCount=18
Control1=IDC_STARTTIME,SysDateTimePick32,1342242848
Control2=IDC_DEST,edit,1350631552
Control3=IDC_ENDTIME,SysDateTimePick32,1342242848
Control4=IDC_EDIT4,edit,1350631552
Control5=IDC_EDIT2,edit,1350631552
Control6=IDC_BUTTON4,button,1342242816
Control7=IDC_BUTTON1,button,1342242816
Control8=IDC_LIST1,SysListView32,1350631433
Control9=IDC_BUTTON2,button,1342242816
Control10=IDC_BUTTON3,button,1342242816
Control11=IDC_RADIO1,button,1342177289
Control12=IDC_RADIO2,button,1342177289
Control13=IDC_RADIO3,button,1342177289
Control14=IDC_TIME,static,1342308352
Control15=IDC_DEPT,static,1342308352
Control16=IDC_PURPOSE,static,1342308352
Control17=IDC_USER,static,1342308352
Control18=IDC_STATIC,static,1342308352

[CLS:CManageCarDlg]
Type=0
HeaderFile=ManageCarDlg.h
ImplementationFile=ManageCarDlg.cpp
BaseClass=CSizingDialog
Filter=D
LastObject=CManageCarDlg
VirtualFilter=dWC

[CLS:CInOutDlg]
Type=0
HeaderFile=InOutDlg.h
ImplementationFile=InOutDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT4

[DLG:IDD_INOUTDLG]
Type=1
Class=CInOutDlg
ControlCount=36
Control1=IDC_CODING,edit,1484849280
Control2=IDC_COMBO1,combobox,1344340226
Control3=IDC_COMBO2,combobox,1344340226
Control4=IDC_EDIT3,edit,1350631552
Control5=IDC_COMBO3,combobox,1344340226
Control6=IDC_EDIT4,edit,1350631552
Control7=IDC_COMBO4,combobox,1344340226
Control8=IDC_DATETIMEPICKER1,SysDateTimePick32,1342242848
Control9=IDC_DATETIMEPICKER2,SysDateTimePick32,1342242848
Control10=IDC_EDIT5,edit,1350631552
Control11=IDC_EDIT6,edit,1350631552
Control12=IDC_EDIT7,edit,1350631552
Control13=IDC_EDIT8,edit,1350631552
Control14=IDC_COMBO6,combobox,1344340226
Control15=IDC_COMBO5,combobox,1344340226
Control16=IDC_COMBO7,combobox,1344340226
Control17=IDC_COMMENTEDIT,RICHEDIT,1352732740
Control18=IDOK,button,1342242817
Control19=IDCANCEL,button,1342242816
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,static,1342308352
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,static,1342308352
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352
Control35=IDC_BACKLAB,static,1342308352
Control36=IDC_STATIC,static,1342308352

[CLS:CXComboBox]
Type=0
HeaderFile=XComboBox.h
ImplementationFile=XComboBox.cpp
BaseClass=CComboBox
Filter=W

[CLS:CXCListCtrl]
Type=0
HeaderFile=XCListCtrl.h
ImplementationFile=XCListCtrl.cpp
BaseClass=CListCtrl
Filter=W

[DLG:IDD_TMDLG]
Type=1
Class=CTMDlg
ControlCount=7
Control1=IDC_TARGETLST,SysListView32,1350631425
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_BUTTON4,button,1342242816
Control7=IDC_BUTTON5,button,1342242816

[CLS:CTMDlg]
Type=0
HeaderFile=TMDlg.h
ImplementationFile=TMDlg.cpp
BaseClass=CSizingDialog
Filter=D
LastObject=IDC_EDIT1
VirtualFilter=dWC

[DLG:IDD_EDITTARGETDLG]
Type=1
Class=CEditTargetDlg
ControlCount=22
Control1=IDC_EDIT1,edit,1350631552
Control2=IDC_EDIT2,edit,1350631552
Control3=IDC_EDIT4,edit,1350631552
Control4=IDC_EDIT5,edit,1350631552
Control5=IDC_COMBO1,combobox,1344340226
Control6=IDC_COMBO3,combobox,1344340226
Control7=IDC_COMBO4,combobox,1344340226
Control8=IDC_RICHEDIT1,RICHEDIT,1352728644
Control9=IDC_EDIT6,edit,1350631584
Control10=IDC_EDIT8,edit,1350631584
Control11=IDC_BUTTON1,button,1342242816
Control12=IDC_BUTTON3,button,1342242816
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352

[CLS:CEditTargetDlg]
Type=0
HeaderFile=EditTargetDlg.h
ImplementationFile=EditTargetDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMBO3
VirtualFilter=dWC

