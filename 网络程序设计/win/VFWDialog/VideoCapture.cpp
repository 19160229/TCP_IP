/*********************************
 *	VideoCapture.cpp
 *
 *********************************/
#include "Stdafx.h"
#include "GraghDialog.h"
#include "GraghDialogDlg.h"
#include "VideoCapture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib,"vfw32")
#pragma comment(lib,"winmm")

VideoCapture::VideoCapture()
{
	m_capwnd=NULL;
}

VideoCapture::~VideoCapture()
{
	DestroyWindow(m_capwnd);
}

void VideoCapture::SetDialog(CDialog *dialog)
{
	dlg=dialog;
}

BOOL VideoCapture::Initialize()
{
	char devname[128]={0},devversion[128]={0};
	int index=0,i=0;
	BOOL	ret=TRUE,ret1=TRUE,ret2=TRUE,ret3=TRUE,ret4=TRUE;
	TRACE("VideoCapture::Initialize\n");
//	Enumerate Video capture Drivers
//	for (i=0; i<10; i++)
//		if ( capGetDriverDescription(i, devname, sizeof(devname),devversion, sizeof(devversion)) )
//			TRACE("Driver[%d]: name=[%s],Version=[%s]\n",i,devname,devversion);

	m_capwnd = capCreateCaptureWindow("Capture",WS_POPUP,0,0,1,1,0,0);
	if(!m_capwnd)
	{
//		TRACE("\tcapCreateCaptureWindow FAIL\n");
		return FALSE;
	}
	ret = capSetUserData(m_capwnd,this) ;
//将该VideoCapture对象(this)与对话框联系。

	ret1 = capSetCallbackOnVideoStream(m_capwnd,OnCaptureVideo);
//关键点：设置回调函数。VFW每捕获到一帧，自动调用该函数。

	ret2 = capGetDriverDescription(index,devname,100,devversion,100);
//获取USB等视频设备。序号在index中。

	ret3 = capDriverConnect(m_capwnd,index);
//将找到的视频设备与捕获窗口相联系.

	if(!(ret && ret1 && ret2 && ret3))
	{
		m_capwnd=NULL;
		return FALSE;
	}
//	Set the capture parameters
	ret4 = SetCapturePara();
	if(!ret4)
	{
		capDriverDisconnect(m_capwnd);
		return FALSE;
	}
	return TRUE;
}

BOOL VideoCapture::StartCapture()
{
//	TRACE("VideoCapture::StartCapture\n");
	BOOL ret=capCaptureSequenceNoFile(m_capwnd);
	return ret;
}

BOOL VideoCapture::StopCapture()
{
	BOOL ret=TRUE,ret1=TRUE;
//	TRACE("VideoCapture::StopCapture\n");
	ret = capCaptureStop(m_capwnd) ;
	Sleep(500);	
	return (ret && ret1);
}

BOOL VideoCapture::Destroy()
{
	BOOL ret=TRUE,ret1=TRUE,ret2=TRUE;
//	TRACE("VideoCapture::Destroy\n");
	capCaptureAbort(m_capwnd) ;
	ret = capSetCallbackOnVideoStream(m_capwnd, NULL);
	ret1 = capSetUserData(NULL,this);
	Sleep(300);		// important...
	ret2 = capDriverDisconnect(m_capwnd) ;
	return (ret && ret1 && ret2);	
}

BOOL  VideoCapture::SetCapturePara()
{
	CAPTUREPARMS CapParms={0};	
//	TRACE("VideoCapture::SetCapturePara\n");
	capCaptureGetSetup(m_capwnd,&CapParms,sizeof(CapParms));
	CapParms.fAbortLeftMouse = FALSE;
	CapParms.fAbortRightMouse = FALSE;
	CapParms.fYield = TRUE;
	CapParms.fCaptureAudio = FALSE;
	CapParms.wPercentDropForError = 80;
	if(!capCaptureSetSetup(m_capwnd,&CapParms,sizeof(CapParms)))
	{
		return FALSE;
	}
//	Set Video Format 
	capGetVideoFormat(m_capwnd,&m_bmpinfo,sizeof(m_bmpinfo));
	m_bmpinfo.bmiHeader.biWidth=IMAGE_WIDTH;
	m_bmpinfo.bmiHeader.biHeight=IMAGE_HEIGHT;
	BOOL ret=capSetVideoFormat(m_capwnd,&m_bmpinfo,sizeof(m_bmpinfo));
	return ret;
}

LRESULT CALLBACK OnCaptureVideo(HWND mwnd,LPVIDEOHDR lphdr)
{
	VideoCapture *vidcap=(VideoCapture *)capGetUserData(mwnd);
	if(vidcap!=NULL )
		((CGraghDialogDlg*) (vidcap->dlg))->SendVideo(lphdr->lpData,lphdr->dwBytesUsed);
//	TRACE("VideoCapture::OnCaptureVideo OK\n");
	return TRUE;
}



