// RemoteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GraghDialog.h"
//#include "RemoteDlg.h"
#include "GraghDialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemoteDlg dialog


CRemoteDlg::CRemoteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemoteDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRemoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteDlg)
	DDX_Control(pDX, IDC_REMOTE_ZOOM, m_RemoteZoom);
	DDX_Control(pDX, IDC_REMOTE_CLOSE, m_RemoteClose);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemoteDlg, CDialog)
	//{{AFX_MSG_MAP(CRemoteDlg)
	ON_BN_CLICKED(IDC_REMOTE_ZOOM, OnRemoteZoom)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_REMOTE_MIN, OnRemoteMin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteDlg message handlers
/*
void CRemoteDlg::OnRemoteClose() 
{
	// TODO: Add your control notification handler code here
	INT ret=AfxMessageBox("退出会议?",MB_OKCANCEL);
	if(ret!=IDOK) return;

	((CGraghDialogDlg*)pdlg)->DestroyConference ();
	UpdateUI();
	ShowWindow(SW_HIDE);
}
*/
void CRemoteDlg::FreeSource()
{
	DeleteObject(m_hTitleRgn);
	DeleteObject(m_hVideoRgn);
	if(hdib!=NULL)
	{
		::DrawDibEnd(hdib);
		::DrawDibClose(hdib);
	}
}
void CRemoteDlg::OnRemoteZoom() 
{
	// TODO: Add your control notification handler code here
	m_bZoomed = !m_bZoomed;
	int scale=1;
	if(m_bZoomed)
	{
		scale=2;
		m_Width =m_Width*2;
		m_Height =m_Height*2-16;
	}else
	{
		m_Width=(m_Width)/2;
		m_Height=(m_Height+16)/2;
	}
	Invalidate(TRUE);
	::SetWindowPos(m_hWnd,HWND_TOP,0 ,0 ,m_Width,m_Height,SWP_NOMOVE);
	
	DeleteObject(m_hVideoRgn);
	POINT pt[4];
	pt[0].x=m_Video_x;		pt[0].y=m_Video_y;
	pt[1].x=m_Video_x+scale*IMAGE_WIDTH;	pt[1].y=m_Video_y;
	pt[2].x=m_Video_x+scale*IMAGE_WIDTH;	pt[2].y=m_Video_y+scale*IMAGE_HEIGHT;
	pt[3].x=m_Video_x;		pt[3].y=m_Video_y+scale*IMAGE_HEIGHT;
	m_hVideoRgn = CreatePolygonRgn(pt,4,ALTERNATE);
	UpdateUI();
}

BOOL CRemoteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText (_T("远程视频"));
	m_RemoteClose.SetIcon (IDI_MININ,IDI_MIN,10,10);
	m_RemoteZoom.SetIcon (IDI_ZOOMIN,IDI_ZOOM,10,10);
	m_RemoteClose.AddToolTip (_T("关闭"));
	m_RemoteZoom.AddToolTip (_T("缩放"));
	m_bHilight=FALSE;
	m_Last=FALSE;
	m_hdc = GetDC()->m_hDC ;
	RECT rc;
	GetClientRect(&rc);

	m_Width=rc.right -rc.left ;
	m_Height=rc.bottom -rc.top ;

	POINT pt[4];
	pt[0].x=0;		pt[0].y=0;
	pt[1].x=3*m_Width/4;	pt[1].y=0;
	pt[2].x=3*m_Width/4;	pt[2].y=16;
	pt[3].x=0;		pt[3].y=16;
	m_hTitleRgn	=	CreatePolygonRgn(pt,4,ALTERNATE);
	
	m_Video_x = 2;	m_Video_y=18;
	pt[0].x=m_Video_x;		pt[0].y=m_Video_y;
	pt[1].x=m_Video_x+IMAGE_WIDTH;	pt[1].y=m_Video_y;
	pt[2].x=m_Video_x+IMAGE_WIDTH;	pt[2].y=m_Video_y+IMAGE_HEIGHT;
	pt[3].x=m_Video_x;		pt[3].y=m_Video_y+IMAGE_HEIGHT;
	m_hVideoRgn = CreatePolygonRgn(pt,4,ALTERNATE);
	
	hdib=NULL;
	m_bVideoIn = FALSE;
	m_bZoomed	= FALSE;
	DrawTitleBar(m_hdc);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRemoteDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
//	CDialog::OnLButtonDown(nFlags, point);
	if(PtInRegion(m_hTitleRgn,point.x,point.y))
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y)); 
}

void CRemoteDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	PtInRegion(m_hTitleRgn,point.x,point.y)?m_bHilight =TRUE:m_bHilight =FALSE;
	if(m_bHilight != m_Last )
	{
		m_Last=m_bHilight;
		DrawTitleBar(m_hdc);
	}
//	CDialog::OnMouseMove(nFlags, point);
}

void CRemoteDlg::DrawTitleBar(HDC hdc)
{
	RECT rc;
	GetClientRect(&rc);
	HBRUSH hbr;
	char szTitle[128]="";
	
	if(m_bHilight)	hbr=CreateSolidBrush(0x00FF3333);		
	else			hbr=CreateSolidBrush(0x00660000);		
	FillRgn(hdc,m_hTitleRgn,hbr);
	DeleteObject(hbr);
	
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

	if(!m_bVideoIn)// video playing
	{
		hbr=CreateSolidBrush(0x00000000);
		FillRgn(hdc,m_hVideoRgn,hbr);
		sprintf(szTitle,"没有收到对方视频");
		DrawText(hdc,szTitle,strlen(szTitle),&CRect(m_Width/4,m_Height/2,m_Width,m_Height),DT_LEFT);
		DeleteObject(hbr);
	}
	SelectObject(hdc,hof);
	DeleteObject(hf);
	DeleteObject(hof);
}

void CRemoteDlg::DisplayRemoteFrame(unsigned char *data,int size)
{
//	TRACE("CRemoteDlg::DisplayRemoteFrame %d Bytes\n",size);
	int i=0,j=0;
	RECT rc;
	if(data == NULL || size<=0)		return;
	m_bVideoIn =TRUE;
	if(!hdib)
	{// 1st time display,setup paras
		m_bmpinfo=((CGraghDialogDlg*)pdlg)->m_bmpinfo ;
		GetClientRect(&rc);
		m_Width=rc.right -rc.left ;
		m_Height=rc.bottom -rc.top ;
		hdib=::DrawDibOpen();
		if(hdib!=NULL)
		{
			::DrawDibBegin(hdib,
				m_hdc,
				-1,				// don't stretch
				-1,				// don't stretch
				&m_bmpinfo->bmiHeader,
				IMAGE_WIDTH,         // width of image
				IMAGE_HEIGHT,        // height of image
				0				
				);
		}
	}else
		::DrawDibDraw(hdib,
				  m_hdc,
				  m_Video_x,		// dest : left pos
				  m_Video_y,		// dest : top pos
				  m_bZoomed?2*IMAGE_WIDTH:-1,		// zoom x
				  m_bZoomed?2*IMAGE_HEIGHT:-1,		// zoom y
				  &m_bmpinfo->bmiHeader,	 // bmp header info
				  data,					 // bmp data
				  0,					 // src :left
				  0,					 // src :top
				  IMAGE_WIDTH,				 // src : width
				  IMAGE_HEIGHT,				 // src : height
				  DDF_SAME_DRAW			 // use prev params....
				  );
}

void CRemoteDlg::SetParent(CDialog *pdialog)
{
	pdlg=pdialog;
}
void CRemoteDlg::UpdateUI()
{
	DrawTitleBar (m_hdc);
}
void CRemoteDlg::OnOK()
{}
	
void CRemoteDlg::OnCancel()
{}

void CRemoteDlg::OnRemoteMin() 
{
	ShowWindow(SW_MINIMIZE);	
}
