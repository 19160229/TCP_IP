/*********************************
 *	AudioRec.h
 *	By	L0op8ack@
 *********************************/

#ifndef _AUDIOREC_H_
#define _AUDIOREC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*  Used for GSM wave format....
#ifndef WAVE_FORMAT_GSM610
#define WAVE_FORMAT_GSM610  (0X0031)
#endif
*/

#include "AudioCodec.h"

#define WM_RECORDSOUND_STARTRECORDING	WM_USER+500
#define WM_RECORDSOUND_STOPRECORDING	WM_USER+501
#define WM_RECORDSOUND_ENDTHREAD		WM_USER+502

#define MAXRECBUFFER	8
#define RECBUFFER		SIZE_AUDIO_FRAME

class CAudioRec : public CWinThread  
{
	DECLARE_DYNCREATE(CAudioRec)
	
	CDialog *pDlg;
	
	HWAVEIN m_hRecord;
/* Used for GSM wave format...
	struct wave
	{
		WAVEFORMATEX wfx;
		WORD wSamplesPerBlock;
	}m_WaveFormatEx;
*/
	WAVEFORMATEX m_WaveFormatEx; 
	
	BOOL recording;
	int isallocated;
	LPWAVEHDR rechead[MAXRECBUFFER];
//	char m_AudioComped[SIZE_AUDIO_PACKED];
public:
	
	CAudioRec();
	CAudioRec(CDialog *dlg);
	virtual ~CAudioRec();

	BOOL InitInstance();
	int ExitInstance();

	LPWAVEHDR CreateWaveHeader();
	
	void PreCreateHeader();

	afx_msg LRESULT OnStartRecording(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnStopRecording(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnEndThread(WPARAM wp,LPARAM lp);
	LRESULT OnSoundData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()	
};

#endif
