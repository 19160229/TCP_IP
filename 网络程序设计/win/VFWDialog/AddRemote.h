#if !defined(AFX_ADDREMOTE_H__4894E52D_64EA_4229_9433_CD4A381209AB__INCLUDED_)
#define AFX_ADDREMOTE_H__4894E52D_64EA_4229_9433_CD4A381209AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddRemote.h : header file
//

#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CAddRemote dialog

class CAddRemote : public CDialog
{
// Construction
public:
	CAddRemote(CWnd* pParent = NULL);   // standard constructor
	void FreeSource();
	void SetParent(CDialog *dlg);
	CDialog *m_pdlg;
	
	UINT	m_CallAction;
// Dialog Data
	//{{AFX_DATA(CAddRemote)
	enum { IDD = IDD_ADDREMOTE };
	CButtonST	m_OK;
	CButtonST	m_Cancel;
	CString		m_Comment;
	CString		m_IPAddr;
	CString		m_Name;
	BOOL	m_AutoAccept;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddRemote)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HDC 	m_hdc;
	BOOL	m_bHilight;
	BOOL	m_Last;
	HRGN	m_hTitleRgn;
	void	DrawTitleBar(HDC hdc);
	CBitmap	m_bmpMain;

	INT		m_Width;
	INT		m_Height;
	// Generated message map functions
	//{{AFX_MSG(CAddRemote)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) ;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDREMOTE_H__4894E52D_64EA_4229_9433_CD4A381209AB__INCLUDED_)
