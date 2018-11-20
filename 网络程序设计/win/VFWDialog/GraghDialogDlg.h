// GraghDialogDlg.h : header file
//

#if !defined(AFX_GRAGHDIALOGDLG_H__80CF839E_93A9_435E_849C_1B0D80A1657D__INCLUDED_)
#define AFX_GRAGHDIALOGDLG_H__80CF839E_93A9_435E_849C_1B0D80A1657D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGraghDialogDlg dialog

#include "BtnST.h"
#include "AddressBook.h"
#include "AddRemote.h"
#include "DSocket.h"
#include "RemoteDlg.h"
#include "TextChatDlg.h"
#include "VideoCapture.h"
#include "VideoCodec.h"
//#include "SkinPlusPlus.h"
#include "AudioPlay.h"
#include "AudioRec.h"
//#include "AudioCodec.h"
#include "G729aCompress.h"

///////////////////////////////////
#define WM_TRAYNOTIFY	WM_USER+100
#define STATE_IDLE			0
#define STATE_CALLOUT		1
#define STATE_CALLIN		2
#define STATE_CONNECTED		3
///////////////////////////////////

class CGraghDialogDlg : public CDialog
{
// Construction
public:
	CGraghDialogDlg(CWnd* pParent = NULL);	// standard constructor
	void PlayWavFile(LPCTSTR filename);

//	Global Control Vars
	BOOL	m_bVidInit;
	BOOL	m_bStart;
	BOOL	m_VidLocal;
	BOOL	m_AutoAccept;
	DSocket dcontrol,daudio,dvideo,dtext;

	INT		m_State;//0:idle 1:connecting 2:connected

	CRemoteDlg	m_RemoteDlg;
	CTextChatDlg	m_TextChatDlg;
//	display frame(local/remote)
	PBITMAPINFO m_bmpinfo;
	void SendVideo(BYTE *data,int size);
	void DisplayRemoteFrame(unsigned char *data,int size);
	
	BOOL StartConference();
	void DestroyConference();

	void DisplayMesg(char *str);
	
//	Video Compress/UnCompress Buffer,dynamic allocate
	char *m_VUnComped;
	char *m_VComped;
//	for frame compress and uncompress
	CVideoCodec	m_VideoCodec;

//	Audio record/play Compress/UnCompress
	CAudioPlay	*m_AudioPlay;
	CAudioRec	*m_AudioRec;

//	CAudioCodec	m_AudioCodec;
	CG729aCompress	m_G729aCompress;

//	G729a Compress/UnCompress Buffer,dynamic allocate
	char *m_AUnComped;
	char *m_AComped;

//	For Audio/Video output rate stastics
	DWORD	m_dwLastTime;
	DWORD	m_VFrames;
	DWORD	m_VBytesSent;

	DWORD	m_AFrames;
	DWORD	m_ABytesSent;

	// Dialog Data
	//{{AFX_DATA(CGraghDialogDlg)
	enum { IDD = IDD_GRAGHDIALOG_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraghDialogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON		m_hIcon;
	HINSTANCE	m_hInstance;
	int			m_Retry;

	// User Interface
	NOTIFYICONDATA	m_tnid;	
	CButtonST	m_OK;
	CButtonST	m_About;
	CButtonST	m_Quit;
	CButtonST	m_Cancel;
	CButtonST	m_VLocal;
	CAddressBook	*m_AddrBook;

	HDC 	m_hdc;
	BOOL	m_bHilight;
	BOOL	m_Last;
	CTime	m_ConnectTime;

	HRGN	m_hVideoRgn;
	HRGN	m_hMesgRgn;
	HRGN	m_hTitleRgn;
	void	DrawTitleBar(HDC hdc);

	CBitmap	m_bmpMain;
	void 	SetMainBmp(int idb);

	INT		m_Width;
	INT		m_Height;

//	Video capture
	VideoCapture	*m_vidcap;
	BOOL	InitCap();
	void	DestroyCap();
	
	HDRAWDIB		hdib;
	BOOL	InitDib();
	void	DestroyDib();

	int m_Video_x , m_Video_y;

	// Generated message map functions
	//{{AFX_MSG(CGraghDialogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOK();
	afx_msg void OnCancel() ;
	afx_msg void OnAbout();
	afx_msg void OnViewlocal();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) ;
	afx_msg LRESULT OnTrayNotify(WPARAM wParam,LPARAM lParam);
	afx_msg void OnQuit();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAGHDIALOGDLG_H__80CF839E_93A9_435E_849C_1B0D80A1657D__INCLUDED_)
