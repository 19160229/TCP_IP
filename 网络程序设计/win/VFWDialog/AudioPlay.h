/*********************************
 *	AudioPlay.h
 *	By	L0op8ack@
 *********************************/

#ifndef _AUDIOPLAY_H_
#define _AUDIOPLAY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
#ifndef WAVE_FORMAT_GSM610
#define WAVE_FORMAT_GSM610 (0x0031)
#endif
*/

#include "AudioCodec.h"

#define WM_PLAYSOUND_STARTPLAYING	WM_USER+600
#define WM_PLAYSOUND_STOPPLAYING	WM_USER+601
#define WM_PLAYSOUND_PLAYBLOCK		WM_USER+602
#define WM_PLAYSOUND_ENDTHREAD		WM_USER+603

#define PLAYBUFFER   SIZE_AUDIO_PACKED

class CAudioPlay : public CWinThread  
{
	DECLARE_DYNCREATE(CAudioPlay)

public:
/*
	For GSM WAVE FORMAT uncomment this..and comment next line...
	struct wave
	{
		WAVEFORMATEX wfx;
		WORD wSamplesPerBlock;
	}m_WaveFormatEx;
*/
//	char m_AudioUnComped[SIZE_AUDIO_FRAME];
	WAVEFORMATEX m_WaveFormatEx; 

	BOOL Playing;
	HWAVEOUT m_hPlay;

	CDialog *pDlg;
	int m_Count;
	CCriticalSection m_cs;
	CAudioPlay();
	CAudioPlay(CDialog *dlg);

	virtual ~CAudioPlay();

	BOOL InitInstance();
	int ExitInstance();

	LPWAVEHDR CreateWaveHeader(CString mesg);
	void ProcessSoundData(short int *sound, DWORD dwSamples);

	afx_msg LRESULT OnStartPlaying(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStopPlaying(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndPlaySoundData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWriteSoundData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndThread(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
};

#endif

