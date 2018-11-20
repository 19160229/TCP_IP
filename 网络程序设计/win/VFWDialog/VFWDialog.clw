; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTextChatDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "GraghDialog.h"

ClassCount=8
Class1=CGraghDialogApp
Class2=CGraghDialogDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_ADDREMOTE
Resource2=IDR_MAINFRAME
Resource3=IDD_GRAGHDIALOG_DIALOG
Class4=CAddressBook
Resource4=IDD_ADDRBOOK
Class5=CAddRemote
Class6=CHDialog
Resource5=IDD_ABOUTBOX
Class7=CRemoteDlg
Resource6=IDD_REMOTE
Class8=CTextChatDlg
Resource7=IDD_TEXTCHAT

[CLS:CGraghDialogApp]
Type=0
HeaderFile=GraghDialog.h
ImplementationFile=GraghDialog.cpp
Filter=N
LastObject=CGraghDialogApp

[CLS:CGraghDialogDlg]
Type=0
HeaderFile=GraghDialogDlg.h
ImplementationFile=GraghDialogDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CGraghDialogDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=GraghDialogDlg.h
ImplementationFile=GraghDialogDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342179843
Control2=IDOK,button,1342181121
Control3=IDC_STATIC,static,1342177280
Control4=IDC_STATIC,static,1342177281

[DLG:IDD_GRAGHDIALOG_DIALOG]
Type=1
Class=CGraghDialogDlg
ControlCount=5
Control1=IDC_QUIT,button,1342178560
Control2=IDOK,button,1342246656
Control3=IDC_ABOUT,button,1342181120
Control4=IDCANCEL,button,1342181120
Control5=IDC_VIEWLOCAL,button,1342246656

[DLG:IDD_ADDRBOOK]
Type=1
Class=CAddressBook
ControlCount=6
Control1=IDOK,button,1342177281
Control2=IDCANCEL,button,1342177280
Control3=IDC_ADDRLIST,SysListView32,1350598941
Control4=IDC_ADD,button,1342177280
Control5=IDC_DEL,button,1342177280
Control6=IDC_MODIFY,button,1342177280

[CLS:CAddressBook]
Type=0
HeaderFile=AddressBook.h
ImplementationFile=AddressBook.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CAddressBook

[DLG:IDD_ADDREMOTE]
Type=1
Class=CAddRemote
ControlCount=6
Control1=IDOK,button,1342177280
Control2=IDCANCEL,button,1342177280
Control3=IDC_NAME,edit,1342242817
Control4=IDC_IPADDR,edit,1342242817
Control5=IDC_COMMENT,edit,1342242817
Control6=IDC_AUTOACCEPT,button,1342242819

[CLS:CAddRemote]
Type=0
HeaderFile=AddRemote.h
ImplementationFile=AddRemote.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_AUTOACCEPT
VirtualFilter=dWC

[CLS:CHDialog]
Type=0
HeaderFile=HDialog.h
ImplementationFile=HDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CHDialog
VirtualFilter=dWC

[DLG:IDD_REMOTE]
Type=1
Class=CRemoteDlg
ControlCount=2
Control1=IDC_REMOTE_MIN,button,1342177280
Control2=IDC_REMOTE_ZOOM,button,1342177280

[CLS:CRemoteDlg]
Type=0
HeaderFile=RemoteDlg.h
ImplementationFile=RemoteDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CRemoteDlg

[DLG:IDD_TEXTCHAT]
Type=1
Class=CTextChatDlg
ControlCount=4
Control1=IDOK,button,1073807360
Control2=IDCANCEL,button,1342177280
Control3=IDC_INPUT,edit,1342246912
Control4=IDC_MESG,SysListView32,1342210049

[CLS:CTextChatDlg]
Type=0
HeaderFile=TextChatDlg.h
ImplementationFile=TextChatDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CTextChatDlg
VirtualFilter=dWC

