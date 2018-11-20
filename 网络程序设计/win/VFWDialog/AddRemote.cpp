// AddRemote.cpp : implementation file
//

#include "stdafx.h"
#include "GraghDialog.h"
#include "AddRemote.h"
#include "AddressBook.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddRemote dialog

CAddRemote::CAddRemote(CWnd* pParent /*=NULL*/)
	: CDialog(CAddRemote::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddRemote)
	m_Comment = _T("连接自己");
	m_IPAddr = _T("127.0.0.1");
	m_Name = _T("Local");
	m_AutoAccept = FALSE;
	//}}AFX_DATA_INIT
}

void CAddRemote::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddRemote)
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	DDV_MaxChars(pDX, m_Comment, 64);
	DDX_Text(pDX, IDC_IPADDR, m_IPAddr);
	DDV_MaxChars(pDX, m_IPAddr, 64);
	DDX_Text(pDX, IDC_NAME, m_Name);
	DDV_MaxChars(pDX, m_Name, 64);
	DDX_Check(pDX, IDC_AUTOACCEPT, m_AutoAccept);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddRemote, CDialog)
	//{{AFX_MSG_MAP(CAddRemote)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddRemote message handlers

BOOL CAddRemote::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_hdc = GetDC ()->m_hDC ;
	
	BITMAP bm;
	m_bmpMain.LoadBitmap (IDB_ADDREMOTE);
	m_bmpMain.GetBitmap (&bm);
	m_Width		= bm.bmWidth ;
	m_Height	= bm.bmHeight ;
	RECT rc;
	GetClientRect(&rc);
	ClientToScreen (&rc);
	::SetWindowPos (m_hWnd,HWND_TOP,rc.left ,rc.top ,m_Width ,m_Height,0);
	
	POINT pt[4];
	pt[0].x=18;		pt[0].y=2;
	pt[1].x=118;	pt[1].y=2;
	pt[2].x=104;	pt[2].y=16;
	pt[3].x=4;		pt[3].y=16;
	m_hTitleRgn	=	CreatePolygonRgn(pt,4,ALTERNATE);
	DrawTitleBar(m_hdc);

	if(m_CallAction ==1)
		SetWindowText(_T("添加联系人"));
	else if(m_CallAction ==2)
		SetWindowText(_T("修改联系人"));
	else
		SetWindowText(_T("联系人"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddRemote::SetParent(CDialog *dlg)
{
	m_pdlg = dlg;
}

void CAddRemote::FreeSource()
{
	DeleteObject(m_hTitleRgn);
}

void CAddRemote::OnOK() 
{
	LVITEM li;
	int iItem;
	UpdateData(TRUE);
	if(m_CallAction == 1)	// add
	{
		li.iSubItem=0;						
		li.mask=LVIF_TEXT;
		li.iItem=1000;
		li.pszText= m_Name.GetBuffer (m_Name.GetLength ()) ;
		iItem=ListView_InsertItem(((CAddressBook*)m_pdlg)->m_AddrList.m_hWnd  ,&li);
	}else					// modify
	{
		iItem=ListView_GetNextItem(((CAddressBook*)m_pdlg)->m_AddrList ,-1,LVNI_SELECTED);
		if(iItem<0)
		{
			AfxMessageBox("严重错误: 无法指定联系人!");
			CDialog::OnOK();
		}
		ListView_SetItemText(((CAddressBook*)m_pdlg)->m_AddrList.m_hWnd,iItem,0,m_Name.GetBuffer (m_Name.GetLength ())) ; 
	}
	ListView_SetItemText(((CAddressBook*)m_pdlg)->m_AddrList.m_hWnd,iItem,1,m_IPAddr.GetBuffer (m_IPAddr.GetLength ())) ; 
	if(m_AutoAccept )	//	自动接受该用户呼叫
	{	ListView_SetItemText(((CAddressBook*)m_pdlg)->m_AddrList.m_hWnd,iItem,2,"是") ; }
	else
	{	ListView_SetItemText(((CAddressBook*)m_pdlg)->m_AddrList.m_hWnd,iItem,2,"否") ; }
	ListView_SetItemText(((CAddressBook*)m_pdlg)->m_AddrList.m_hWnd,iItem,3,m_Comment.GetBuffer (m_Comment.GetLength ())) ; 
	FreeSource();
	CDialog::OnOK();
}

void CAddRemote::OnCancel() 
{
	FreeSource();
	CDialog::OnCancel();
}

void CAddRemote::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
	if(PtInRegion(m_hTitleRgn,point.x,point.y))
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y)); 
}

void CAddRemote::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDialog::OnMouseMove(nFlags, point);
	PtInRegion(m_hTitleRgn,point.x,point.y)?m_bHilight =TRUE:m_bHilight =FALSE;
	if(m_bHilight != m_Last )
	{
		m_Last=m_bHilight;
		DrawTitleBar(m_hdc);
	}
}

BOOL CAddRemote::OnEraseBkgnd(CDC* pDC) 
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap *pOldBMP = (CBitmap*)MemDC.SelectObject(&m_bmpMain);
	pDC->BitBlt(0,0,m_Width,m_Height,&MemDC,0,0,SRCCOPY);
	if(pOldBMP)		MemDC.SelectObject(pOldBMP);
	MemDC.DeleteDC ();
	return TRUE;
}

void CAddRemote::DrawTitleBar(HDC hdc)
{
	RECT rc;
	GetClientRect(&rc);
	HBRUSH hbr;
	if(m_bHilight)	hbr=CreateSolidBrush(0x00FF3333);		
	else			hbr=CreateSolidBrush(0x00660000);		

	FillRgn(hdc,m_hTitleRgn,hbr);
	DeleteObject(hbr);
	
	char szTitle[64]="";
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(ncm);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
	HFONT hf=CreateFontIndirect(&ncm.lfCaptionFont);
	HFONT hof=(HFONT)SelectObject(hdc,hf);
	SetBkMode(hdc,TRANSPARENT);
	if(m_bHilight)	SetTextColor(hdc,0x000000ff);
	else			SetTextColor(hdc,0x00C0C0C0);
	GetWindowText(szTitle,63);
	
	DrawText(hdc,szTitle,strlen(szTitle),&CRect(25,2,144,22),DT_LEFT);
	SelectObject(hdc,hof);
	DeleteObject(hf);
	DeleteObject(hof);
}
