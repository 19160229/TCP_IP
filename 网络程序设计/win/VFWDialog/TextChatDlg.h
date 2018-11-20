#if !defined(AFX_TEXTCHATDLG_H__5EFBF754_BAD1_4D7D_A112_4FF9E1D38766__INCLUDED_)
#define AFX_TEXTCHATDLG_H__5EFBF754_BAD1_4D7D_A112_4FF9E1D38766__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextChatDlg.h : header file
//
#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CTextChatDlg dialog

class CTextChatDlg : public CDialog
{
// Construction
public:
	CTextChatDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTextChatDlg)
	enum { IDD = IDD_TEXTCHAT };
	CButtonST	m_OK;
	CButtonST	m_Cancel;
	CListCtrl	m_Mesg;
	CString		m_strInput;
	//}}AFX_DATA
	void SetParent(CDialog *pdialog);
	void UpdateUI();
	void FreeSource();
	void DisplayRemoteMessage(char *who,char *data,int len);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextChatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HDC 	m_hdc;
	CDialog *pDlg;

	BOOL	m_bHilight;
	BOOL	m_Last;

	HRGN	m_hTitleRgn;
	void	DrawTitleBar(HDC hdc);

	// Generated message map functions
	//{{AFX_MSG(CTextChatDlg)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTCHATDLG_H__5EFBF754_BAD1_4D7D_A112_4FF9E1D38766__INCLUDED_)
