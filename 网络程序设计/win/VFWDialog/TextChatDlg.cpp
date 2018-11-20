// TextChatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GraghDialog.h"
#include "GraghDialogDlg.h"
#include "TextChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextChatDlg dialog


CTextChatDlg::CTextChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextChatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextChatDlg)
	m_strInput = _T("");
	//}}AFX_DATA_INIT
}


void CTextChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextChatDlg)
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_MESG, m_Mesg);
	DDX_Text(pDX, IDC_INPUT, m_strInput);
	DDV_MaxChars(pDX, m_strInput, 64);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextChatDlg, CDialog)
	//{{AFX_MSG_MAP(CTextChatDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextChatDlg message handlers

void CTextChatDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown(nFlags, point);
	if(PtInRegion(m_hTitleRgn,point.x,point.y))
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));	
}

void CTextChatDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PtInRegion(m_hTitleRgn,point.x,point.y)?m_bHilight =TRUE:m_bHilight =FALSE;
	if(m_bHilight != m_Last )
	{
		m_Last=m_bHilight;
		DrawTitleBar(m_hdc);
	}
	CDialog::OnMouseMove(nFlags, point);
}

BOOL CTextChatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText (_T("文本消息"));
	m_Cancel.SetIcon (IDI_MININ,IDI_MIN,10,10);
	m_Cancel.AddToolTip (_T("关闭"));
	
	m_bHilight=FALSE;
	m_Last=FALSE;
	m_Mesg.SetExtendedStyle(LVS_EX_FLATSB| LVS_EX_HEADERDRAGDROP |LVS_EX_FULLROWSELECT );
	LV_COLUMN lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_LEFT;
	lvcol.cx=60;
	lvcol.pszText="发言人";
	ListView_InsertColumn(m_Mesg,0,&lvcol);
	lvcol.cx=190;
	lvcol.pszText="内容";
	ListView_InsertColumn(m_Mesg,1,&lvcol);

	m_Mesg.SetBkColor (0x00FFCC99);
	m_Mesg.SetTextBkColor (0x00FFCC99);
	m_Mesg.SetTextColor (0x00000000);

	m_hdc = GetDC()->m_hDC ;
	RECT rc;
	GetClientRect(&rc);

	POINT pt[4];
	pt[0].x=0;		pt[0].y=0;
	pt[1].x=rc.right -rc.left -64;	pt[1].y=0;
	pt[2].x=rc.right -rc.left -64;	pt[2].y=16;
	pt[3].x=0;		pt[3].y=16;
	m_hTitleRgn	=	CreatePolygonRgn(pt,4,ALTERNATE);
	DrawTitleBar(m_hdc);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextChatDlg::OnOK()
{
	UpdateData(TRUE);
	char sz[256];
	if(m_strInput.IsEmpty ())	return;
	
	sprintf(sz,"%s",m_strInput.GetBuffer (m_strInput.GetLength ()));
	((CGraghDialogDlg*)pDlg)->dtext .SendTextData ((unsigned char *)sz,m_strInput.GetLength ());

	DisplayRemoteMessage("我",sz,m_strInput.GetLength ());

	m_strInput.Format ("");
	UpdateData (FALSE);
}

void CTextChatDlg::SetParent(CDialog *pdialog)
{
	pDlg=pdialog;
}
void CTextChatDlg::FreeSource()
{
	DeleteObject(m_hTitleRgn);
}

void CTextChatDlg::UpdateUI()
{
	DrawTitleBar (m_hdc);
}
void CTextChatDlg::DisplayRemoteMessage(char *who,char *data,int len)
{
	char sz[256];
	LVITEM li;
	int iItem;
	if(m_Mesg.GetItemCount ()>=8)
		ListView_DeleteItem(m_Mesg.m_hWnd ,0);

	li.iSubItem=0;						
	li.mask=LVIF_TEXT;
	li.iItem=1000;
	sprintf(sz,"%s",who);
	li.pszText=sz;
	iItem=ListView_InsertItem(m_Mesg.m_hWnd  ,&li);
	sprintf(sz,"%s",data);
	ListView_SetItemText(m_Mesg.m_hWnd,iItem,1,sz);
}

void CTextChatDlg::DrawTitleBar(HDC hdc)
{
	RECT rc;
	GetClientRect(&rc);
	HBRUSH hbr;
	if(m_bHilight)	hbr=CreateSolidBrush(0x00FF3333);		
	else			hbr=CreateSolidBrush(0x00660000);		
	FillRgn(hdc,m_hTitleRgn,hbr);
	DeleteObject(hbr);

	char szTitle[128]="";
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(ncm);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
	HFONT hf=CreateFontIndirect(&ncm.lfCaptionFont);
	HFONT hof=(HFONT)SelectObject(hdc,hf);
	SetBkMode(hdc,TRANSPARENT);
	if(m_bHilight)	SetTextColor(hdc,0x000000ff);
	else			SetTextColor(hdc,0x00C0C0C0);
	GetWindowText(szTitle,63);
	
	DrawText(hdc,szTitle,strlen(szTitle),&CRect(4,0,144,12),DT_LEFT);
	SelectObject(hdc,hof);
	DeleteObject(hf);
	DeleteObject(hof);
}


void CTextChatDlg::OnCancel() 
{
	ShowWindow(SW_MINIMIZE);
}