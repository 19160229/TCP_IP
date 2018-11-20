/*********************************
 *	VideoCodec.h
 *
 *********************************/
#ifndef _VIDEOCODEC_H_
#define _VIDEOCODEC_H_

#include "vfw.h"

#define MS_H261			0
#define MS_H263			1
#define DIVX_MPEG4		2
#define MS_MPEG4V1		3
#define MS_MPEG4V2		4
#define MS_MPEG4V3		5
#define XVID_MPEG4		6
#define MS_VIDC			7
#define RADIUS_CVID		8


class CVideoCodec
{
public:
	CVideoCodec();
	virtual  ~CVideoCodec();

	BOOL Init();
	void Destroy();

	BOOL EncodeVideoData(char* pin,int len,char* pout,int* lenr,BOOL* pKey);
	BOOL DecodeVideoData(char *pin,int len,char* pout,int *lenr,DWORD flag);

	void SetParent(CDialog *dlg);
	BOOL SetCodecType(int type);
protected:
	HIC			m_hIC;
	BITMAPINFO  m_BmpU;
	BITMAPINFO  m_BmpC;

	COMPVARS	m_cv;
	
	int			m_Codec;
	CDialog		*pDlg;
	
	struct _CODEC_TYPE_
	{
		int		codec;
		char	*Fourcc;
	} m_CodecType[9];
};

#endif