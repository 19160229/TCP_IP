#include "stdafx.h"
#include "G729aCompress.h"

extern "C" void va_g729a_init_encoder();
extern "C" void va_g729a_encoder(short *speech, unsigned char *bitstream);
extern "C" void va_g729a_init_decoder();
extern "C" void va_g729a_decoder(unsigned char *bitstream, short *synth_short, int bfi);
#define  L_FRAME_COMPRESSED 10
#define  L_FRAME            80

#pragma comment(lib,"G729a")

CG729aCompress::CG729aCompress()
{
	va_g729a_init_encoder();
	va_g729a_init_decoder();
}

CG729aCompress::~CG729aCompress()
{
}

BOOL CG729aCompress::Compress(char* src,int srclen,char* dst,int* dstlen)
{
//	TRACE("CG729aCompress::Compress %d Bytes\n",srclen);
	if(!src || srclen != SIZE_AUDIO_FRAME || !dst)
		return FALSE;
	
	for( int i = 0 ; i < SIZE_AUDIO_PACKED / 10 ; i++ )
	{
		va_g729a_encoder((short*)(src + i * 160),(BYTE*)dst + i * 10);
	}

	if(dstlen)
		*dstlen=SIZE_AUDIO_PACKED;
//	TRACE("CG729aCompress::UnCompress Done\n");
	return TRUE;
}

BOOL CG729aCompress::UnCompress(char* src,int srclen,char* dst,int* dstlen)
{
//	TRACE("CG729aCompress::UnCompress %d Bytes\n",srclen);
	if( !src || srclen != SIZE_AUDIO_PACKED || !dst)
		return FALSE;

	for( int i = 0 ; i < SIZE_AUDIO_PACKED / 10 ; i++ )
	{
		va_g729a_decoder((BYTE*)src + i*10 ,(short*)(dst + i*160),0);
	}

	if(dstlen)
		*dstlen=SIZE_AUDIO_FRAME;
//	TRACE("CG729aCompress::UnCompress Done\n");
	return TRUE;
}