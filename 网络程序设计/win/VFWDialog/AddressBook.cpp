// AddressBook.cpp : implementation file
//

#include "stdafx.h"
#include "GraghDialog.h"
#include "AddressBook.h"
#include "GraghDialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddressBook dialog

CAddressBook::CAddressBook(CWnd* pParent /*=NULL*/)
	: CDialog(CAddressBook::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddressBook)
	//}}AFX_DATA_INIT
}

void CAddressBook::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddressBook)
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_MODIFY, m_Modify);
	DDX_Control(pDX, IDC_DEL, m_Del);
	DDX_Control(pDX, IDC_ADD, m_Add);
	DDX_Control(pDX, IDC_ADDRLIST, m_AddrList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddressBook, CDialog)
	//{{AFX_MSG_MAP(CAddressBook)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddressBook message handlers

BOOL CAddressBook::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_AddrList.SetExtendedStyle(LVS_EX_FLATSB| LVS_EX_HEADERDRAGDROP |LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	LV_COLUMN lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.cx=60;
	lvcol.pszText="姓名";
	ListView_InsertColumn(m_AddrList,0,&lvcol);
	lvcol.cx=114;
	lvcol.pszText="地址";
	ListView_InsertColumn(m_AddrList,1,&lvcol);
	lvcol.cx=30;
	lvcol.pszText="自动接受";
	ListView_InsertColumn(m_AddrList,2,&lvcol);
	lvcol.cx=100;
	lvcol.pszText="备注";
	ListView_InsertColumn(m_AddrList,3,&lvcol);

	m_OK.AddToolTip (_T("呼叫选定联系人"));
	m_Cancel.AddToolTip (_T("取消"));
	m_Modify.AddToolTip (_T("修改选定联系人"));
	m_Del.AddToolTip (_T("删除选定联系人"));
	m_Add.AddToolTip (_T("添加新联系人"));

	m_AddrList.SetBkColor (0x00FFCC99);
	m_AddrList.SetTextBkColor (0x00FFCC99);
	m_AddrList.SetTextColor (0x00000000);
	
	m_AddRemote =NULL;

	SetWindowText(_T("地址簿"));

	m_hdc = GetDC ()->m_hDC ;

	BITMAP bm;
	m_bmpMain.LoadBitmap (IDB_ADDREMOTE);
	m_bmpMain.GetBitmap (&bm);
	m_Width		= bm.bmWidth ;
	m_Height	= bm.bmHeight ;
	RECT rc;
	m_pdlg->GetClientRect(&rc);
	ClientToScreen (&rc);
	::SetWindowPos (m_hWnd,HWND_TOP,rc.right  ,rc.top ,m_Width ,m_Height,0);

	POINT pt[4];
	pt[0].x=18;		pt[0].y=2;
	pt[1].x=118;	pt[1].y=2;
	pt[2].x=104;	pt[2].y=16;
	pt[3].x=4;		pt[3].y=16;
	m_hTitleRgn	=	CreatePolygonRgn(pt,4,ALTERNATE);
	DrawTitleBar(m_hdc);
	LoadList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddressBook::SetParent(CDialog *dlg)
{
	m_pdlg = dlg;
}

void CAddressBook::FreeSource()
{
	DeleteObject(m_hTitleRgn);
}

void CAddressBook::OnAdd() 
{
	m_AddRemote= new CAddRemote;
	m_AddRemote->SetParent (this);
	m_AddRemote->m_CallAction = 1;
	m_AddRemote->DoModal ();
	if(m_AddRemote)	delete m_AddRemote;
	m_AddRemote=NULL;
	UpdateData(TRUE);
}

void CAddressBook::OnModify() 
{
	// TODO: Add your control notification handler code here
	int iItem=ListView_GetNextItem(m_AddrList ,-1,LVNI_SELECTED);
	char sz[256];
	if(iItem<0)
	{
		AfxMessageBox("您没有选择联系人");
		return ;
	}
	ListView_GetItemText(m_AddrList,iItem,0,sz,sizeof(sz));
	m_AddRemote= new CAddRemote;
	m_AddRemote->SetParent (this);
	m_AddRemote->m_CallAction = 2;
	m_AddRemote->m_Name .Format ("%s",sz);
	ListView_GetItemText(m_AddrList,iItem,1,sz,sizeof(sz));
	m_AddRemote->m_IPAddr  .Format ("%s",sz);
	ListView_GetItemText(m_AddrList,iItem,2,sz,sizeof(sz));
	m_AddRemote->m_AutoAccept = (strstr(sz,"是")!=NULL);
	ListView_GetItemText(m_AddrList,iItem,3,sz,sizeof(sz));
	m_AddRemote->m_Comment .Format ("%s",sz);
	m_AddRemote->DoModal ();
	if(m_AddRemote)	delete m_AddRemote;
	m_AddRemote=NULL;	
	UpdateData(TRUE);
}

void CAddressBook::OnCancel() 
{
	UpdateData(TRUE);
	SaveList();
	FreeSource();
	CDialog::OnCancel ();
}

void CAddressBook::OnOK() 
{
	char hostname[200],aa[8];
	BOOL	ret=FALSE;
	ZeroMemory(hostname,sizeof(hostname));
	UpdateData(TRUE);
	SaveList();
	int iItem=ListView_GetNextItem(m_AddrList ,-1,LVNI_SELECTED);
	if(iItem<0)
	{
		AfxMessageBox("您没有选择联系人");
		return ;
	}

	int i=ListView_GetSelectionMark(m_AddrList);
	ListView_GetItemText(m_AddrList,i,1,hostname,sizeof(hostname));
	ListView_GetItemText(m_AddrList,i,2,aa,sizeof(aa));
	if(strstr(aa,"是"))
		((CGraghDialogDlg*)m_pdlg)->m_AutoAccept =TRUE;
	else
		((CGraghDialogDlg*)m_pdlg)->m_AutoAccept =FALSE;
	
	ret=((CGraghDialogDlg*)m_pdlg)->dcontrol.SendControlMessage(MESG_CONNECT,hostname);
	if(!ret)
	{
		((CGraghDialogDlg*)m_pdlg)->m_State = STATE_IDLE;
		((CGraghDialogDlg*)m_pdlg)->DisplayMesg("无法连接该联系人");
		return;
	}else
		((CGraghDialogDlg*)m_pdlg)->m_State=STATE_CALLOUT;	
	FreeSource();
	CDialog::OnOK ();
}

void CAddressBook::OnDel() 
{
	int iItem=ListView_GetNextItem(m_AddrList ,-1,LVNI_SELECTED);
	if(iItem<0)
	{
		AfxMessageBox("您没有选择联系人");
		return ;
	}
	ListView_DeleteItem(m_AddrList.m_hWnd ,iItem);
	UpdateData(TRUE);
}

void CAddressBook::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
	if(PtInRegion(m_hTitleRgn,point.x,point.y))
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y)); 
}

void CAddressBook::OnMouseMove(UINT nFlags, CPoint point) 
{
	PtInRegion(m_hTitleRgn,point.x,point.y)?m_bHilight =TRUE:m_bHilight =FALSE;
	if(m_bHilight != m_Last )
	{
		m_Last=m_bHilight;
		DrawTitleBar(m_hdc);
	}
	CDialog::OnMouseMove(nFlags, point);
}

BOOL CAddressBook::OnEraseBkgnd(CDC* pDC) 
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap *pOldBMP = (CBitmap*)MemDC.SelectObject(&m_bmpMain);
	pDC->BitBlt(0,0,m_Width,m_Height,&MemDC,0,0,SRCCOPY);
	if(pOldBMP)		MemDC.SelectObject(pOldBMP);	
	MemDC.DeleteDC ();
	return TRUE;
}

void CAddressBook::DrawTitleBar(HDC hdc)
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
	
	DrawText(hdc,szTitle,strlen(szTitle),&CRect(25,3,144,22),DT_LEFT);
	SelectObject(hdc,hof);
	DeleteObject(hf);
	DeleteObject(hof);
}

void CAddressBook::LoadList()
{
//	程序所在路径
	char szPath[1024];
	GetModuleFileName(0,szPath,1024);
	int i;
	for(i=strlen(szPath);i>0;i--)
	{
		if(szPath[i]=='\\')
		{
			szPath[i+1]=0;
			break;
		}
	}

	strcpy(szPath+strlen(szPath),"联系人.ini");
	CListCtrl* h=(CListCtrl*)GetDlgItem(IDC_ADDRLIST);

//	记录数
	int count=GetPrivateProfileInt("地址","总数量",0,szPath);
	char sz[64];
	LVITEM li;
	li.iItem=10000;
	
	li.mask=LVIF_TEXT;
	li.pszText=sz;
//	读出记录并添加到列表中
	for(i=0;i<count;i++)
	{
		char s[16];
		sprintf(s,"姓名%d",i);
		li.iSubItem=0;
		GetPrivateProfileString("地址",s,"",sz,64,szPath);
		int iItem=ListView_InsertItem(h->m_hWnd ,&li);
		sprintf(s,"地址%d",i);
		GetPrivateProfileString("地址",s,"",sz,64,szPath);
		ListView_SetItemText(h->m_hWnd,iItem,1,sz);
		sprintf(s,"自动接受%d",i);
		GetPrivateProfileString("地址",s,"",sz,64,szPath);
		ListView_SetItemText(h->m_hWnd,iItem,2,sz);

		sprintf(s,"备注%d",i);
		GetPrivateProfileString("地址",s,"",sz,64,szPath);
		ListView_SetItemText(h->m_hWnd,iItem,3,sz);
	}

}

//保存通讯录
void CAddressBook::SaveList()
{
//	程序所在路径
	char szPath[1024];
	GetModuleFileName(0,szPath,1024);
	int i;
	for(i=strlen(szPath);i>0;i--)
	{
		if(szPath[i]=='\\')
		{
			szPath[i+1]=0;
			break;
		}
	}
	strcpy(szPath+strlen(szPath),"联系人.ini");
	CListCtrl* h=(CListCtrl*)GetDlgItem(IDC_ADDRLIST);
//	记录条数
	int count=ListView_GetItemCount(h->m_hWnd );
	char sz[64];
	sprintf(sz,"%d",count);
//	写入文件
	WritePrivateProfileString("地址","总数量",sz,szPath);
	for(i=0;i<count;i++)
	{
		char s[16];
		sprintf(s,"姓名%d",i);
		ListView_GetItemText(h->m_hWnd,i,0,sz,64);
		WritePrivateProfileString("地址",s,sz,szPath);
		sprintf(s,"地址%d",i);
		ListView_GetItemText(h->m_hWnd,i,1,sz,64);
		WritePrivateProfileString("地址",s,sz,szPath);
		sprintf(s,"自动接受%d",i);
		ListView_GetItemText(h->m_hWnd,i,2,sz,64);
		WritePrivateProfileString("地址",s,sz,szPath);
		sprintf(s,"备注%d",i);
		ListView_GetItemText(h->m_hWnd,i,3,sz,64);
		WritePrivateProfileString("地址",s,sz,szPath);
	}
}
