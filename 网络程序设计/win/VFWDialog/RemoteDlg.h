#if !defined(AFX_REMOTEDLG_H__5E058620_785D_47DB_9CE3_07974B4AB800__INCLUDED_)
#define AFX_REMOTEDLG_H__5E058620_785D_47DB_9CE3_07974B4AB800__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemoteDlg.h : header file
//

#include "BtnST.h"
#include <vfw.h>

/////////////////////////////////////////////////////////////////////////////
// CRemoteDlg dialog

class CRemoteDlg : public CDialog
{
// Construction
public:
	CRemoteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRemoteDlg)
	enum { IDD = IDD_REMOTE };
	//}}AFX_DATA
//	char remoteaddress[512];
	HRGN	m_hVideoRgn;
	
	HDC 	m_hdc;
	
	BOOL	m_bVideoIn;

	HDRAWDIB		hdib;
	PBITMAPINFO		m_bmpinfo;
	void	DisplayRemoteFrame(unsigned char *data,int size);
	void	FreeSource();
	CDialog *pdlg;

	void SetParent(CDialog *pdialog);
	void UpdateUI();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON	m_hIcon;

	BOOL	m_bHilight;
	BOOL	m_Last;
	HRGN	m_hTitleRgn;
	void	DrawTitleBar(HDC hdc);

	CButtonST	m_RemoteZoom;
	CButtonST	m_RemoteClose;
	BOOL		m_bZoomed;
	INT			m_Video_x,m_Video_y;
	INT			m_Width,m_Height;

	void OnOK();
	void OnCancel();
	// Generated message map functions
	//{{AFX_MSG(CRemoteDlg)
	afx_msg void OnRemoteZoom();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRemoteMin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEDLG_H__5E058620_785D_47DB_9CE3_07974B4AB800__INCLUDED_)
