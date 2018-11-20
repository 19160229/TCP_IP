// GraghDialog.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GraghDialog.h"
#include "GraghDialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraghDialogApp

BEGIN_MESSAGE_MAP(CGraghDialogApp, CWinApp)
	//{{AFX_MSG_MAP(CGraghDialogApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraghDialogApp construction

CGraghDialogApp::CGraghDialogApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGraghDialogApp object

CGraghDialogApp theApp;
WSADATA	wsaData;

CString  g_szPropName = "USB����Ƶ��̸ϵͳ";   //�Լ�����һ��������
HANDLE   g_hValue = (HANDLE)1;               //�Լ�����һ������ֵ

BOOL CALLBACK EnumWndProc(HWND hwnd,LPARAM lParam)
{
	HANDLE h = GetProp(hwnd,g_szPropName);
	if( h == g_hValue)
	{
		*(HWND*)lParam = hwnd;
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CGraghDialogApp initialization

BOOL CGraghDialogApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	HWND oldHWnd = NULL;

	EnumWindows(EnumWndProc,(LPARAM)&oldHWnd);    //ö���������еĴ���

	if(oldHWnd != NULL)
	{
        AfxMessageBox("�������Ѿ���������");
        ::ShowWindow(oldHWnd,SW_SHOWNORMAL);          //�����ҵ���ǰһ������
        ::SetForegroundWindow(oldHWnd);                //������Ϊǰ������
        return false;                                  //�˳���������
	}

	if(!AfxSocketInit(&wsaData))
		AfxMessageBox("AfxSocketInit Failed");

	CGraghDialogDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

