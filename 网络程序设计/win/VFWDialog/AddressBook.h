#if !defined(AFX_ADDRESSBOOK_H__83A2070E_8BF8_444D_BD17_65EF2FA5D567__INCLUDED_)
#define AFX_ADDRESSBOOK_H__83A2070E_8BF8_444D_BD17_65EF2FA5D567__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddressBook.h : header file
//

#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CAddressBook dialog
#include "AddRemote.h"

class CAddressBook : public CDialog
{
// Construction
public:
	CAddressBook(CWnd* pParent = NULL);   // standard constructor

	void SetParent(CDialog *dlg);
	CDialog *m_pdlg;
	void FreeSource();
	CAddRemote	*m_AddRemote;
// Dialog Data
	//{{AFX_DATA(CAddressBook)
	enum { IDD = IDD_ADDRBOOK };
	CButtonST	m_OK;
	CButtonST	m_Cancel;
	CButtonST	m_Modify;
	CButtonST	m_Del;
	CButtonST	m_Add;
	CListCtrl	m_AddrList;
	//}}AFX_DATA

	void SetParameter(char *hostname,char *hostaddress,CDialog *dlg);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddressBook)
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

	void LoadList();
	void SaveList();
	// Generated message map functions
	//{{AFX_MSG(CAddressBook)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnModify();
	afx_msg void OnDel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) ;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDRESSBOOK_H__83A2070E_8BF8_444D_BD17_65EF2FA5D567__INCLUDED_)
