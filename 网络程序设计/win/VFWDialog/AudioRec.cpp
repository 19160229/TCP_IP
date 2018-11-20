/*********************************
 *	AudioRec.cpp
 *	By	L0op8ack@
 *********************************/

#include "stdafx.h"
#include "GraghDialog.h"
#include "GraghDialogDlg.h"
#include "AudioRec.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CAudioRec, CWinThread)

BEGIN_MESSAGE_MAP(CAudioRec,CWinThread)
	ON_THREAD_MESSAGE(MM_WIM_DATA, OnSoundData)
	ON_THREAD_MESSAGE(WM_RECORDSOUND_STARTRECORDING,OnStartRecording)
	ON_THREAD_MESSAGE(WM_RECORDSOUND_STOPRECORDING,OnStopRecording)
	ON_THREAD_MESSAGE(WM_RECORDSOUND_ENDTHREAD,OnEndThread)
END_MESSAGE_MAP()

CAudioRec::CAudioRec()
{
//	TRACE("CAudioRec::CAudioRec\n");
}

CAudioRec::CAudioRec(CDialog *dialog)
{
//	TRACE("CAudioRec::CAudioRec(CDialog *dialog)\n");
	pDlg=dialog;
	
	recording=FALSE;
	isallocated=0;

	PreCreateHeader();
/*
	memset(&m_WaveFormatEx,0x00,sizeof(m_WaveFormatEx));
	m_WaveFormatEx.wfx.wFormatTag = WAVE_FORMAT_GSM610;
	m_WaveFormatEx.wfx.nChannels = 1;
	m_WaveFormatEx.wfx.wBitsPerSample = 0;
	m_WaveFormatEx.wfx.cbSize = 2;
	m_WaveFormatEx.wfx.nSamplesPerSec = SAMPLEPSEC;
	m_WaveFormatEx.wfx.nAvgBytesPerSec = 1625; //(SAMPLEPSEC/320)*65 ;
	m_WaveFormatEx.wfx.nBlockAlign = 65;
	m_WaveFormatEx.wSamplesPerBlock=320;
*/
	//Setting WAVEFORMATEX  structure for the audio input
	memset(&m_WaveFormatEx,0x00,sizeof(m_WaveFormatEx));
	m_WaveFormatEx.wFormatTag		=	WAVE_FORMAT_PCM;
	m_WaveFormatEx.nChannels		=	1;
	m_WaveFormatEx.wBitsPerSample	=	16;
	m_WaveFormatEx.cbSize			=	0;
	m_WaveFormatEx.nSamplesPerSec	=	SAMPLEPSEC;  //22.05 KHz
	m_WaveFormatEx.nBlockAlign		=	m_WaveFormatEx.wBitsPerSample/8;
	m_WaveFormatEx.nAvgBytesPerSec	=	m_WaveFormatEx.nBlockAlign * m_WaveFormatEx.nSamplesPerSec;
}


CAudioRec::~CAudioRec()
{
//	TRACE("CAudioRec::~CAudioRec\n");
	if(!isallocated)	return;

	for(int i=0;i<MAXRECBUFFER;i++)
	{
		if(rechead[i])
		{
			if(rechead[i]->lpData )
				delete [](rechead[i]->lpData);
			delete rechead[i];
		}
	}
}

BOOL CAudioRec::InitInstance()
{
//	TRACE("CAudioRec::InitInstance()\n");
	return TRUE;
}

int CAudioRec::ExitInstance()
{
//	TRACE("CAudioRec::ExitInstance()\n");
	return CWinThread::ExitInstance();
}

void CAudioRec::PreCreateHeader()
{
//	TRACE("CAudioRec::PreCreateHeader()\n");
	for(int i=0;i<MAXRECBUFFER;i++)
		rechead[i]=CreateWaveHeader();
	isallocated=1;
}

LRESULT CAudioRec::OnStartRecording(WPARAM wp,LPARAM lp)
{
//	TRACE("CAudioRec::OnStartRecording()\n");
	if(recording)	return FALSE;
	
	MMRESULT mmReturn = 
		::waveInOpen( &m_hRecord, WAVE_MAPPER,
			&m_WaveFormatEx, ::GetCurrentThreadId(), 0, CALLBACK_THREAD);
	if(mmReturn!=MMSYSERR_NOERROR )		return FALSE;

	if(mmReturn==MMSYSERR_NOERROR )
	{
		for(int i=0; i < MAXRECBUFFER ; i++)
		{
			mmReturn = ::waveInPrepareHeader(m_hRecord,rechead[i], sizeof(WAVEHDR));
			mmReturn = ::waveInAddBuffer(m_hRecord, rechead[i], sizeof(WAVEHDR));
		}
		mmReturn = ::waveInStart(m_hRecord);
		if(mmReturn==MMSYSERR_NOERROR ) 	recording=TRUE;
	}
	return TRUE;
}

LRESULT CAudioRec::OnStopRecording(WPARAM wp,LPARAM lp)
{
//	TRACE("CAudioRec::OnStopRecording()\n");

	MMRESULT mmReturn = 0;
	
	if(!recording)		return FALSE;

	mmReturn = ::waveInStop(m_hRecord);
	recording = FALSE;		
//	if(!mmReturn)
	mmReturn = ::waveInReset(m_hRecord);  
	Sleep(500); 
	//if(!mmReturn)		
	mmReturn = ::waveInClose(m_hRecord);
	return mmReturn;
}

LRESULT CAudioRec::OnSoundData(WPARAM wParam, LPARAM lParam)
{
//	TRACE("CAudioRec::OnSoundData()\n");
	int clen=1;
	if(recording==FALSE)	return FALSE;

	LPWAVEHDR lpHdr = (LPWAVEHDR) lParam;
	
	if(lpHdr->dwBytesRecorded==0 || lpHdr==NULL)	return ERROR_SUCCESS;
		
	::waveInUnprepareHeader(m_hRecord, lpHdr, sizeof(WAVEHDR));

	// Send recorded audio to remote host...
	if(lpHdr->lpData!=NULL )
	{
		( (CGraghDialogDlg*) pDlg )->m_G729aCompress .Compress  (lpHdr->lpData,lpHdr->dwBytesRecorded,((CGraghDialogDlg*) pDlg )->m_AComped,&clen);
		if(clen>0)
		{
			((CGraghDialogDlg*) pDlg )->daudio.SendAudioData((unsigned char *)(((CGraghDialogDlg*) pDlg )->m_AComped),clen);
			((CGraghDialogDlg*) pDlg )->m_ABytesSent +=clen;
			((CGraghDialogDlg*) pDlg )->m_AFrames  ++;
//			((CGraghDialogDlg*) pDlg )->daudio.SendAudioData((unsigned char *)lpHdr->lpData,lpHdr->dwBytesRecorded);
		}
	}
		
	if(recording)
	{	//Reuse the old buffer
		::waveInPrepareHeader(m_hRecord,lpHdr, sizeof(WAVEHDR));
		::waveInAddBuffer(m_hRecord, lpHdr, sizeof(WAVEHDR));
	}
	return ERROR_SUCCESS;
}

LRESULT CAudioRec::OnEndThread(WPARAM wp,LPARAM lp)
{
//	TRACE("CAudioRec::OnEndThread()\n");
	if(recording)	OnStopRecording(0,0);

	::PostQuitMessage(0);
	return TRUE;
}

LPWAVEHDR  CAudioRec::CreateWaveHeader()
{
//	TRACE("CAudioRec::CreateWaveHeader()\n");
	LPWAVEHDR lpHdr = new WAVEHDR;
	if(lpHdr==NULL)		return NULL;
	
	ZeroMemory(lpHdr, sizeof(WAVEHDR));
	char* lpByte = new char[RECBUFFER];
	
	if(lpByte==NULL)	return NULL;

	lpHdr->lpData =  lpByte;
	lpHdr->dwBufferLength =RECBUFFER;
	return lpHdr;
}

