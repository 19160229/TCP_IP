// VideoCapture.h

#ifndef _VIDEOCAPTURE_H_
#define _VIDEOCAPTURE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include<vfw.h>
#include<afxmt.h>
#include<afxole.h>


#define QCIF_WIDTH  320//176
#define QCIF_HEIGHT 240//144

#define IMAGE_WIDTH       QCIF_WIDTH
#define IMAGE_HEIGHT      QCIF_HEIGHT
    
class VideoCapture  
{
public:
	VideoCapture();
	virtual ~VideoCapture();
	void SetDialog(CDialog *dlg);
	CAPDRIVERCAPS caps;
	BITMAPINFOHEADER m_bmpheader;
	BITMAPINFO m_bmpinfo;

	BOOL Initialize();
	BOOL Destroy();
	BOOL StartCapture();
	BOOL StopCapture();

	//Callback functions
    friend LRESULT CALLBACK OnCaptureVideo(HWND hWnd, LPVIDEOHDR lpheader) ;
protected:
	HWND m_capwnd;
	CDialog *dlg;
	BOOL SetCapturePara();
};

#endif
