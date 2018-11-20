// GraghDialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GraghDialog.h"
#include "GraghDialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL	m_bExitNow	= FALSE;


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CButtonST	m_OK;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDOK, m_OK);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y)); 
}
/////////////////////////////////////////////////////////////////////////////
// CGraghDialogDlg dialog

CGraghDialogDlg::CGraghDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CGraghDialogDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGraghDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGraghDialogDlg)
	DDX_Control(pDX, IDC_QUIT, m_Quit);
	DDX_Control(pDX, IDC_VIEWLOCAL, m_VLocal);
	DDX_Control(pDX, IDC_ABOUT, m_About);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_OK);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGraghDialogDlg, CDialog)
	//{{AFX_MSG_MAP(CGraghDialogDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_VIEWLOCAL, OnViewlocal)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_TRAYNOTIFY,OnTrayNotify)
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraghDialogDlg message handlers

BOOL CGraghDialogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
//InitializeSkin(_T("XPCorona.ssk"));
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	HANDLE h=(HANDLE)1;
	::SetProp(m_hWnd,_T("USB音视频交谈系统"),h);

	SetMainBmp (IDB_MAIN) ;
	SetWindowText(_T("USB音视频交谈系统"));
	m_OK.SetIcon (IDI_CALLIN,IDI_CALL,32,32);
	m_OK.AddToolTip (_T("呼叫"));
	m_About.SetIcon (IDI_ABOUTIN,IDI_ABOUTOUT,10,10);
	m_About.AddToolTip(_T("关于"));
	m_Quit.SetIcon (IDI_QUITIN,IDI_QUIT,10,10);
	m_Quit.AddToolTip(_T("退出"));
	m_Cancel.SetIcon (IDI_MININ,IDI_MIN,10,10);
	m_Cancel.AddToolTip(_T("缩小到系统托盘"));
	m_hdc = GetDC()->m_hDC ;
	m_Retry = 0;
	m_VLocal.AddToolTip (_T("该窗口显示本地/远程视频"));

	m_VLocal.EnableWindow (FALSE);
	m_bVidInit	= FALSE;
	m_bStart	= FALSE;
	m_AutoAccept= FALSE;

	m_bExitNow	= FALSE;
	m_VidLocal	= TRUE;
	
	m_AFrames = 0;m_ABytesSent=0;
	m_VFrames	= 0;m_VBytesSent = 0;

	m_Video_x=7;
	m_Video_y=30;

	//make the title bar region
	POINT pt[4];
	pt[0].x=25;		pt[0].y=5;
	pt[1].x=162;	pt[1].y=5;
	pt[2].x=144;	pt[2].y=23;
	pt[3].x=7;		pt[3].y=23;
	m_hTitleRgn	=	CreatePolygonRgn(pt,4,ALTERNATE);

	pt[0].x =m_Video_x;		pt[0].y =m_Video_y;
	pt[1].x=m_Video_x+176;	pt[1].y =m_Video_y;
	pt[2].x=m_Video_x+176;	pt[2].y =m_Video_y+144;
	pt[3].x =m_Video_x;		pt[3].y =m_Video_y+144;
	m_hVideoRgn	=	CreatePolygonRgn(pt,4,ALTERNATE);

	pt[0].x =10;	pt[0].y =179;
	pt[1].x=249;	pt[1].y =179;
	pt[2].x=249;	pt[2].y =198;
	pt[3].x =10;	pt[3].y =198;
	m_hMesgRgn = 	CreatePolygonRgn(pt,4,ALTERNATE);

	// Setup the parent for differnt datagram sockets
	dcontrol.SetParent(this);
	dcontrol.CreateSocket(PORT_CONTROL,TYPE_CONTROL);

	m_VUnComped =NULL;
	m_VComped	=NULL;
	m_AUnComped =NULL;
	m_AComped	=NULL;

	m_vidcap=NULL;
	m_bmpinfo=	NULL;
	hdib	=	NULL;
//	setup Video Codec
	m_VideoCodec.SetCodecType (MS_MPEG4V3);
	m_VideoCodec.SetParent (this);

	m_RemoteDlg.Create (IDD_REMOTE,this);
	m_RemoteDlg.SetParent (this);
	m_RemoteDlg.ShowWindow (SW_HIDE);
	m_AddrBook = NULL;	
	m_TextChatDlg.Create (IDD_TEXTCHAT,this);
	m_TextChatDlg.SetParent (this);
	m_TextChatDlg.ShowWindow (SW_HIDE);

	m_AudioPlay=new CAudioPlay(this);
	m_AudioPlay->CreateThread();
	if(m_AudioPlay)
	{
		m_AudioPlay->PostThreadMessage(WM_PLAYSOUND_STARTPLAYING,0,0);
	}
	m_AudioRec=new CAudioRec(this);
	m_AudioRec->CreateThread();

	m_State=STATE_IDLE;

	SetTimer(1,2000,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGraghDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGraghDialogDlg::OnPaint() 
{
	CPaintDC dc(this);
	if (IsIconic())
	{
		 // device context for painting
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		DrawTitleBar(m_hdc);
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGraghDialogDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CGraghDialogDlg::InitCap()
{
	TRACE("CGraghDialogDlg::InitCap\n");
	if(m_bVidInit)	return TRUE;

	m_vidcap=new VideoCapture;
	m_vidcap->SetDialog(this);

	if(m_vidcap->Initialize()) // success
	{
		m_bmpinfo=&(m_vidcap->m_bmpinfo);
		m_bVidInit =TRUE;
	}else
	{
		if(m_bmpinfo) 		delete m_bmpinfo;
		m_bVidInit = FALSE;
		m_bmpinfo=new BITMAPINFO;
		m_bmpinfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		m_bmpinfo->bmiHeader.biWidth=IMAGE_WIDTH;
		m_bmpinfo->bmiHeader.biHeight=IMAGE_HEIGHT;
		m_bmpinfo->bmiHeader.biPlanes=1;
		m_bmpinfo->bmiHeader.biBitCount=24;
		m_bmpinfo->bmiHeader.biCompression=0;
		m_bmpinfo->bmiHeader.biSizeImage=IMAGE_WIDTH*IMAGE_HEIGHT*3;
		m_bmpinfo->bmiHeader.biXPelsPerMeter=0;
		m_bmpinfo->bmiHeader.biYPelsPerMeter=0;
		m_bmpinfo->bmiHeader.biClrUsed=0;
		m_bmpinfo->bmiHeader.biClrImportant=0;
	}
	return m_bVidInit;
}

void	CGraghDialogDlg::DestroyCap()
{
	TRACE("CGraghDialogDlg::DestroyCap\n");
	if(m_vidcap)
	{
		m_vidcap->StopCapture ();
		m_vidcap->Destroy ();
		delete m_vidcap;
		m_vidcap=NULL;
	}
	if(m_bmpinfo  )
	{
		if(!m_bVidInit)	delete m_bmpinfo;
		m_bmpinfo=NULL;
	}
	m_bVidInit=FALSE;
}

BOOL	CGraghDialogDlg::InitDib()
{
	TRACE("CGraghDialogDlg::InitDib\n");
	if(!hdib)
	{
		if((hdib=DrawDibOpen())!=NULL)
		{
			DrawDibBegin(hdib,
				m_hdc,
				-1,				// don't stretch
				-1,				// don't stretch
				&m_bmpinfo->bmiHeader,
				IMAGE_WIDTH,         // width of image
				IMAGE_HEIGHT,        // height of image
				0				
				);
			return TRUE;
		}
	}
	return TRUE;
}
void	CGraghDialogDlg::DestroyDib()
{
	TRACE("CGraghDialogDlg::DestroyDib\n");
	if(hdib!=NULL)
	{
		DrawDibEnd(hdib);
		DrawDibClose(hdib);
		hdib=NULL;
	}
}

BOOL CGraghDialogDlg::StartConference()
{
	BOOL	ret=TRUE;
	TRACE("CGraghDialogDlg::StartConference\n");
	m_OK.EnableWindow (TRUE);
//	if(!m_bStart)	return TRUE; //not started yet
	if(m_State == STATE_IDLE || m_State == STATE_CONNECTED)	
		return FALSE; //timeout and rejected or already started
	if(!m_VideoCodec.Init ()) return FALSE;
	
	daudio.SetParent(this);
	dvideo.SetParent(this);
	dtext.SetParent(this);
	ret = InitCap() ;
	ret = InitDib() && ret;
	if( !dtext.CreateSocket(PORT_TEXT,TYPE_TEXT) ||
		!daudio.CreateSocket(PORT_AUDIO,TYPE_AUDIO) ||
		!dvideo.CreateSocket(PORT_VIDEO,TYPE_VIDEO)
		)
	{
		DestroyConference();
		return FALSE;
	}

	if(m_vidcap && m_bVidInit)		
		ret =  m_vidcap->StartCapture () && ret;

	m_VUnComped = new char[m_bmpinfo->bmiHeader .biSizeImage];
	m_VComped	= new char[m_bmpinfo->bmiHeader .biWidth * m_bmpinfo->bmiHeader .biHeight ];
	m_AUnComped = new char[SIZE_AUDIO_FRAME];
	m_AComped	= new char[SIZE_AUDIO_PACKED];

	if(!m_VUnComped || !m_VComped || !m_AUnComped || !m_AComped)	
	{
		AfxMessageBox("分配视频压缩/解压缩内存出错!");
		if(m_VUnComped)	delete []m_VUnComped;
		if(m_VComped)	delete []m_VComped;
		if(m_AUnComped) delete []m_AUnComped;
		if(m_AComped)	delete []m_AComped;
		DestroyConference();
		return FALSE;
	}
	ZeroMemory(m_VUnComped,m_bmpinfo->bmiHeader .biSizeImage );
	ZeroMemory(m_VComped,m_bmpinfo->bmiHeader .biWidth * m_bmpinfo->bmiHeader .biHeight );
	ZeroMemory(m_AUnComped,SIZE_AUDIO_FRAME);
	ZeroMemory(m_AComped,SIZE_AUDIO_PACKED);	

	if(m_AudioRec)
		m_AudioRec->PostThreadMessage(WM_RECORDSOUND_STARTRECORDING,0,0);
	TRACE("CGraghDialogDlg::StartConference->InitVideoCodec,InitCap,InitDib,Socket,AV Memory Alloc,Start AudioRec OK\n");
//	if(m_AudioPlay)
//	{
//		m_AudioPlay->PostThreadMessage(WM_PLAYSOUND_STOPPLAYING,0,0);
//		m_AudioPlay->PostThreadMessage(WM_PLAYSOUND_STARTPLAYING,0,0);
//	}
	m_bExitNow = FALSE;
	m_bStart = TRUE;
	m_VLocal.EnableWindow (TRUE);
	m_OK.SetIcon (IDI_HANGIN,IDI_HANG,32,32);
	m_OK.AddToolTip (_T("断开"));
	RECT rc,rcrd;;
	GetClientRect(&rc);
	ClientToScreen (&rc);
	m_RemoteDlg.GetClientRect (&rcrd);
	if(rc.left <(rcrd.right -rcrd.left ))
		::SetWindowPos (m_RemoteDlg.m_hWnd,HWND_TOP,rc.right+1 ,rc.top ,m_bmpinfo->bmiHeader.biWidth+8 ,m_bmpinfo->bmiHeader.biHeight +24 ,0);
	else
		::SetWindowPos (m_RemoteDlg.m_hWnd,HWND_TOP,rc.left- m_bmpinfo->bmiHeader.biWidth-8,rc.top ,m_bmpinfo->bmiHeader.biWidth+8 ,m_bmpinfo->bmiHeader.biHeight +24,0);
	::SetWindowPos (m_TextChatDlg.m_hWnd,HWND_TOP,rc.left,rc.bottom+1 ,0,0,SWP_NOSIZE);
	
	SetTextColor((m_VLocal.GetDC ())->m_hDC,0x00660000);
	if(m_VidLocal)
	{
		SetWindowText (_T("本地视频"));
	//	m_RemoteDlg.SetWindowText (dcontrol.remotename );
		char sz[256];
		sprintf(sz,"远程:%s",dcontrol.remotename );
		m_RemoteDlg.SetWindowText (sz );
		m_VLocal.SetWindowText (_T("本地"));
	}else
	{
	//	m_RemoteDlg.SetWindowText (dcontrol.remotename );
        char sz[256];
		sprintf(sz,"远程:%s",dcontrol.remotename );
		m_RemoteDlg.SetWindowText (sz );
		SetWindowText (_T("本地视频"));
		m_VLocal.SetWindowText (_T("远程"));
	}
	m_State=STATE_CONNECTED;
	OnTrayNotify(IDR_MAINFRAME,WM_LBUTTONUP);
	Invalidate (TRUE);
	m_RemoteDlg.ShowWindow (SW_SHOW);
	m_TextChatDlg.ShowWindow (SW_SHOW);
	m_RemoteDlg.UpdateUI();
	m_TextChatDlg.UpdateUI ();
	m_Retry = 0;
	m_ConnectTime=CTime::GetCurrentTime();
	return ret;
}

void CGraghDialogDlg::DestroyConference()
{
	TRACE("CGraghDialogDlg::DestroyConference\n");
	m_RemoteDlg.ShowWindow (SW_HIDE);
	m_TextChatDlg.ShowWindow (SW_HIDE);
	m_State=STATE_IDLE;
	m_Retry=0;
	m_bExitNow=TRUE;
	m_OK.EnableWindow (TRUE);
	if(m_AudioRec)
		m_AudioRec->PostThreadMessage(WM_RECORDSOUND_STOPRECORDING,0,0);

	DestroyCap ();
	DestroyDib ();
	m_VideoCodec.Destroy ();

	daudio.SetParent(NULL);
	dvideo.SetParent(NULL);
	dtext.SetParent(NULL);
	dtext.CloseSocket();
	daudio.CloseSocket();
	dvideo.CloseSocket();

	if(m_VUnComped)		delete []m_VUnComped;
	m_VUnComped =NULL;
	if(m_VComped)		delete []m_VComped;
	m_VComped =NULL;
	if(m_AUnComped)		delete []m_AUnComped;
	m_AUnComped =NULL;
	if(m_AComped)		delete []m_AComped;
	m_AComped =NULL;

	m_VLocal.EnableWindow (FALSE);
	m_OK.SetIcon (IDI_CALLIN,IDI_CALL,32,32);
	m_OK.AddToolTip (_T("呼叫"));
	if(m_bStart)
		dcontrol.SendControlMessage(MESG_DISCONNECT,dcontrol.remoteaddress);
	m_bStart = FALSE;

	m_RemoteDlg.m_bVideoIn =FALSE;

	SetWindowText(_T("USB音视频交谈系统"));
	Invalidate (TRUE);
}

void CGraghDialogDlg::OnAbout() 
{
	OnSysCommand(IDM_ABOUTBOX ,0);
}

void CGraghDialogDlg::OnOK() 
{
	int ret=IDOK;
	m_bStart=!m_bStart;
	if(m_bStart)	//start now? send connect
	{
		m_AddrBook = new CAddressBook;
		m_AddrBook->SetParent (this);
		ret=m_AddrBook->DoModal();
		if(m_AddrBook) delete m_AddrBook;
		m_AddrBook=NULL;
		if(ret!=IDOK)	return;
//		MESG_CONNECT sent!
		m_dwLastTime=GetTickCount();	
		m_OK.SetIcon (IDI_HANGIN,IDI_HANG,32,32);
		m_OK.AddToolTip (_T("断开"));
	}else	//not start? send disconnect
	{
		DestroyConference();
	}
}

void	CGraghDialogDlg::SendVideo(BYTE *data,int size)
{
	int comp=0;
	if(m_bExitNow  )	return;

	if(data==NULL || size<=0)	return;
	m_VideoCodec.EncodeVideoData((char*)data,size,m_VComped,&comp,0);
//	TRACE("CGraghDialogDlg::SendVideo,Source %d Bytes,compressed %d Bytes\n",size,comp);
	if(comp>0)	
		dvideo.SendVideoData((unsigned char *)m_VComped,comp);

	m_VFrames ++;
	m_VBytesSent +=comp;
	if(m_VidLocal)
	{
		::DrawDibDraw(	
					hdib,
					m_hdc,
					m_Video_x,				// dest : left pos
					m_Video_y,				// dest : top pos
					176,							// don't zoom x
					144,							// don't zoom y
					&m_bmpinfo->bmiHeader,		// bmp header info
					data,						// bmp data
					0,							// src :left
					0,							// src :top
					IMAGE_WIDTH,				// src : width
					IMAGE_HEIGHT,				// src : height
					DDF_SAME_DRAW				// use prev params....
				  );
	}else if( !m_VidLocal && m_RemoteDlg)
		m_RemoteDlg.DisplayRemoteFrame ((unsigned char*)data,size);
}

void CGraghDialogDlg::DisplayRemoteFrame(unsigned char *data,int size)
{
//	TRACE("CGraghDialogDlg::DisplayRemoteFrame %d bytes\n",size);
	if(m_bExitNow  )	return;
	if(data == NULL || size<=0)	return;
	
	if( m_VidLocal && m_RemoteDlg)
		m_RemoteDlg.DisplayRemoteFrame (data,size);
	else if(!m_VidLocal)
		::DrawDibDraw(hdib,
				  m_hdc,
				  m_Video_x ,		// dest : left pos
				  m_Video_y,		// dest : top pos
				  176,					 // don't zoom x
				  144,					 // don't zoom y
				  &m_bmpinfo->bmiHeader,	 // bmp header info
				  data,					 // bmp data
				  0,					 // src :left
				  0,					 // src :top
				  IMAGE_WIDTH,				 // src : width
				  IMAGE_HEIGHT,				 // src : height
				  DDF_SAME_DRAW			 // use prev params....
				  );
}

void CGraghDialogDlg::PlayWavFile(LPCTSTR filename)
{
	CFile f;
	DWORD size=0;
	char *p;
	if(f.Open (filename,CFile::modeRead | CFile::typeBinary,NULL))
	{
		f.Seek (58,CFile::begin);
		size=f.GetLength ();
		p=new char [size];
		if(!p)	return;
		size=f.Read (p ,size);
		m_AudioPlay->PostThreadMessage(WM_PLAYSOUND_PLAYBLOCK,size,(LPARAM)p);
		Sleep(100);
		delete []p;
		f.Close ();
	}
}

void CGraghDialogDlg::OnViewlocal() 
{
	// TODO: Add your control notification handler code here
	m_VidLocal =!m_VidLocal;
	if(m_VidLocal)
	{
		m_VLocal.SetWindowText (_T("本地视频"));
		SetWindowText(dcontrol.remotename);
		m_RemoteDlg.SetWindowText(_T("本地视频"));
	}
	else
	{
		m_VLocal.SetWindowText (_T("远程视频"));
		
		SetWindowText(dcontrol.remotename);
		m_RemoteDlg.SetWindowText(_T("远程视频"));
	}
	Invalidate (TRUE);
	m_RemoteDlg.UpdateUI ();
}

void CGraghDialogDlg::OnQuit() 
{
	if(m_bStart)
	{
		INT ret=AfxMessageBox("退出会议?",MB_OKCANCEL);
		if(ret!=IDOK) return;
	}
	KillTimer(1);
	ShowWindow(SW_HIDE);
	m_bExitNow=TRUE;
//	dcontrol.SendControlMessage (MESG_DISCONNECT,dcontrol.remoteaddress );	
	DestroyConference();

	if(m_AddrBook)	delete m_AddrBook ;
	
	if(m_AudioPlay) 
	{	
		m_AudioPlay->PostThreadMessage(WM_PLAYSOUND_STOPPLAYING,0,0);
		m_AudioPlay->PostThreadMessage(WM_RECORDSOUND_ENDTHREAD,0,0);
		Sleep(200);
		delete m_AudioPlay;
	}
	if(m_AudioRec)
	{
		m_AudioRec->PostThreadMessage(WM_PLAYSOUND_ENDTHREAD,0,0);
		Sleep(200);
		delete m_AudioRec;
	}
	m_AddrBook = NULL;
	m_RemoteDlg.FreeSource();
	m_TextChatDlg .FreeSource();
	DeleteObject(m_hVideoRgn);
	DeleteObject(m_hMesgRgn);
	DeleteObject(m_hTitleRgn);
	CDialog::OnOK();
}

void CGraghDialogDlg::OnCancel() 
{
	m_tnid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);
	m_tnid.hWnd=m_hWnd;
	m_tnid.uID=IDR_MAINFRAME;
	m_tnid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;
	m_tnid.uCallbackMessage=WM_TRAYNOTIFY;
	m_tnid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy(m_tnid.szTip,"视频会议系统");
	Shell_NotifyIcon(NIM_ADD,&m_tnid);
	ShowWindow(SW_HIDE);
	m_RemoteDlg.ShowWindow (SW_HIDE);
	m_TextChatDlg.ShowWindow (SW_HIDE);	
}

void CGraghDialogDlg::DisplayMesg(char *str)
{
	HBRUSH hbr;

	hbr=CreateSolidBrush(0x00FF0000);		

	FillRgn(m_hdc,m_hMesgRgn,hbr);
	DeleteObject(hbr);
	
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(ncm);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
	HFONT hf=CreateFontIndirect(&ncm.lfCaptionFont);
	HFONT hof=(HFONT)SelectObject(m_hdc,hf);
	SetBkMode(m_hdc,TRANSPARENT);
	SetTextColor(m_hdc,0x00FFFFFF);
	
	DrawText(m_hdc,str,strlen(str),&CRect(10,179,249,198),DT_LEFT);
	SelectObject(m_hdc,hof);
}

void CGraghDialogDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
	if(PtInRegion(m_hTitleRgn,point.x,point.y))
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y)); 
}

void CGraghDialogDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetForegroundWindow ();
	SetActiveWindow ();
	PtInRegion(m_hTitleRgn,point.x,point.y)?m_bHilight =TRUE:m_bHilight =FALSE;
	if(m_bHilight != m_Last )
	{
		m_Last=m_bHilight;
		DrawTitleBar(m_hdc);
	}

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CGraghDialogDlg::OnEraseBkgnd(CDC* pDC) 
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap *pOldBMP = (CBitmap*)MemDC.SelectObject(&m_bmpMain);
	pDC->BitBlt(0,0,m_Width,m_Height,&MemDC,0,0,SRCCOPY);
	if(pOldBMP)		MemDC.SelectObject(pOldBMP);
	MemDC.DeleteDC ();
	return TRUE;
}

LRESULT CGraghDialogDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case WM_LBUTTONUP:
		{
			BOOL res; 
			m_tnid.cbSize = sizeof(NOTIFYICONDATA); 
			m_tnid.hWnd = m_hWnd;
			m_tnid.uID = IDR_MAINFRAME; 
			res = Shell_NotifyIcon(NIM_DELETE, &m_tnid); 
			ShowWindow(SW_SHOWNORMAL);
			SetForegroundWindow();
			SetActiveWindow();
			if( m_bStart) 
			{
				m_RemoteDlg.ShowWindow (SW_SHOW);
				m_TextChatDlg.ShowWindow (SW_SHOW);
			}
		}
		break;
	}
	return TRUE;
}

void CGraghDialogDlg::DrawTitleBar(HDC hdc)
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
	DrawText(hdc,szTitle,strlen(szTitle),&CRect(25,7,144,22),DT_LEFT);
	
	if(m_bStart && !m_bVidInit)
	{ // Started but inticap failed
		hbr=CreateSolidBrush(0x00000000);
		FillRgn(hdc,m_hVideoRgn,hbr);
		sprintf(szTitle,"打开视频捕捉设备失败");
		DrawText(hdc,szTitle,strlen(szTitle),&CRect(m_Width/8,m_Height/2,m_Width,m_Height),DT_LEFT);
		DeleteObject(hbr);	
	}

	SelectObject(hdc,hof);
	DeleteObject(hf);
	DeleteObject(hof);
}

void CGraghDialogDlg::SetMainBmp(int idb)
{
	BITMAP bm;
	m_bmpMain.LoadBitmap (idb);
	m_bmpMain.GetBitmap (&bm);
	m_Width		= bm.bmWidth ;
	m_Height	= bm.bmHeight ;

	RECT rc;
	GetClientRect(&rc);
	ClientToScreen (&rc);

	::SetWindowPos (m_hWnd,HWND_TOP,rc.left ,rc.top ,m_Width ,m_Height,0);
}

void CGraghDialogDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	char szFps[256]="";
	DWORD	NowTime;
	CTime t=CTime::GetCurrentTime();
	CTimeSpan ts = t - m_ConnectTime;

	switch(m_State)
	{
	case STATE_IDLE:
		break;
	case STATE_CONNECTED:
		NowTime=GetTickCount();
		if(NowTime - m_dwLastTime>=2000)
		{
			sprintf(szFps,"V:%2dFPS,%3dKbps/A:%dFPS,%02dKbps,%d:%02d:%02d",\
				m_VFrames/2,m_VBytesSent/250,m_AFrames/2,m_ABytesSent/250,
				ts.GetHours (),ts.GetMinutes (),ts.GetSeconds ());
			m_dwLastTime = NowTime;
			m_VFrames = 0;m_VBytesSent=0;
			m_AFrames = 0;m_ABytesSent=0;
			DisplayMesg(szFps);
		}
		break;
	case STATE_CALLIN:
	case STATE_CALLOUT:
		PlayWavFile(m_State==STATE_CALLIN?"callin.wav":"callout.wav");
		m_Retry++;
		sprintf(szFps,"等待对方应答: %d",m_Retry);
		DisplayMesg(szFps);
		if(m_Retry > 10)	//wait 10s
		{
			DestroyConference ();
			DisplayMesg("对方无人应答, 连接超时");
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

//DEL void CAboutDlg::OnOK() 
//DEL {
//DEL 	// TODO: Add extra validation here
//DEL 	
//DEL 	CDialog::OnOK();
//DEL }
