/*********************************
 *	AudioPlay.cpp
 *	By	L0op8ack@
 *********************************/

#include "stdafx.h"
#include "GraghDialog.h"
#include "GraghDialogDlg.h"
#include "AudioPlay.h"


IMPLEMENT_DYNCREATE(CAudioPlay, CWinThread)

BEGIN_MESSAGE_MAP(CAudioPlay, CWinThread)
	ON_THREAD_MESSAGE(WM_PLAYSOUND_STARTPLAYING, OnStartPlaying)
	ON_THREAD_MESSAGE(WM_PLAYSOUND_STOPPLAYING, OnStopPlaying)
	ON_THREAD_MESSAGE(WM_PLAYSOUND_PLAYBLOCK, OnWriteSoundData)
	ON_THREAD_MESSAGE(MM_WOM_DONE, OnEndPlaySoundData)
	ON_THREAD_MESSAGE(WM_PLAYSOUND_ENDTHREAD,OnEndThread)
END_MESSAGE_MAP()

CAudioPlay::CAudioPlay()
{
//	TRACE("CAudioPlay::CAudioPlay\n");
}

CAudioPlay::CAudioPlay(CDialog *dialog)
{
//	TRACE("CAudioPlay::CAudioPlay(CDialog *dialog)\n");
	pDlg=dialog;
	
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
	memset(&m_WaveFormatEx,0x00,sizeof(m_WaveFormatEx));
	m_WaveFormatEx.wFormatTag		= WAVE_FORMAT_PCM;
	m_WaveFormatEx.nChannels		= 1;
	m_WaveFormatEx.wBitsPerSample	= 16;
	m_WaveFormatEx.nSamplesPerSec	= SAMPLEPSEC;
	m_WaveFormatEx.nBlockAlign		= m_WaveFormatEx.wBitsPerSample/8;
	m_WaveFormatEx.nAvgBytesPerSec	= m_WaveFormatEx.nBlockAlign*m_WaveFormatEx.nSamplesPerSec ;
	m_WaveFormatEx.cbSize			= 0;
	Playing = FALSE;
}

CAudioPlay::~CAudioPlay()
{
//	TRACE("CAudioPlay::~CAudioPlay\n");
}

BOOL CAudioPlay::InitInstance()
{
//	TRACE("CAudioPlay::InitInstance\n");
	return TRUE;
}

int CAudioPlay::ExitInstance()
{
//	TRACE("CAudioPlay::ExitInstance\n");
	return CWinThread::ExitInstance();
}

LRESULT CAudioPlay::OnStartPlaying(WPARAM wParam, LPARAM lParam)
{
//	TRACE("CAudioPlay::OnStartPlaying\n");
	MMRESULT mmReturn = 0;

	if(Playing)	return FALSE;
	mmReturn = ::waveOutOpen( &m_hPlay, WAVE_MAPPER,&m_WaveFormatEx, ::GetCurrentThreadId(), 0, CALLBACK_THREAD);
	if(!mmReturn )		
	{	
		Playing = TRUE;
		DWORD volume=0xFFFFFFFF;
		waveOutSetVolume(m_hPlay,volume);
	}
	m_Count=0;
	return TRUE;
}

LRESULT CAudioPlay::OnStopPlaying(WPARAM wParam, LPARAM lParam)
{
//	TRACE("CAudioPlay::OnStopPlaying\n");
	MMRESULT mmReturn = 0;

	if(Playing==FALSE)		return FALSE;

//	TRACE("CAudioPlay::OnStopPlaying, %d frame audio not played\n",m_Count);
	mmReturn = ::waveOutReset(m_hPlay);
		
	if(mmReturn != MMSYSERR_NOERROR)
	{
		Playing = FALSE;
		m_Count=0;
		Sleep(500);
	}
	mmReturn = ::waveOutClose(m_hPlay);

	return mmReturn;
}

LRESULT CAudioPlay::OnEndPlaySoundData(WPARAM wParam, LPARAM lParam)
{
//	TRACE("CAudioPlay::OnEndPlaySoundData\n");
	
	LPWAVEHDR lpHdr = (LPWAVEHDR) lParam;
	
	if(lpHdr)		
	{
		::waveOutUnprepareHeader(m_hPlay, lpHdr, sizeof(WAVEHDR));
		if(lpHdr->lpData )
			delete []lpHdr->lpData;
		delete lpHdr;
		m_Count--;
//		TRACE("CAudioPlay::OnEndPlaySoundData, %d audio frame to be played\n",m_Count);
	}
	return ERROR_SUCCESS;
}

LRESULT CAudioPlay::OnWriteSoundData(WPARAM wParam, LPARAM lParam)
{
//	TRACE("CAudioPlay::OnWriteSoundData\n");
	MMRESULT mmResult = FALSE;
	char *p=NULL;
	int length=(int) wParam;

	if(Playing==FALSE)	return FALSE;
	
	if(length<=0)	return FALSE;

	WAVEHDR *lpHdr=new WAVEHDR;
	if(!lpHdr)	return FALSE;
	p=new char [length];
	if(!p)	
	{
		delete lpHdr;
		return FALSE;
	}
	ZeroMemory(lpHdr,sizeof(WAVEHDR));
	ZeroMemory(p,length);
	CopyMemory(p,(char*)lParam,length);
	lpHdr->lpData=p;
	lpHdr->dwBufferLength = length;

	mmResult = ::waveOutPrepareHeader(m_hPlay, lpHdr, sizeof(WAVEHDR));

	if(mmResult)
	{
		delete lpHdr;delete p;
		return mmResult;
	}
	
	mmResult = ::waveOutWrite(m_hPlay, lpHdr, sizeof(WAVEHDR));
	if(mmResult)
	{
		delete lpHdr;delete p;
		return mmResult;
	}
	m_Count++;
	return MMSYSERR_NOERROR;
}

LRESULT CAudioPlay::OnEndThread(WPARAM wParam, LPARAM lParam)
{
//	TRACE("CAudioPlay::OnEndThread\n");

	if(Playing==TRUE)	OnStopPlaying(0,0);

	::PostQuitMessage(0);
	return TRUE;
}
