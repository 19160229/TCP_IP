#ifndef _CG729aCompress_H_
#define _CG729aCompress_H_

#include "AudioCodec.h"

class CG729aCompress : public CAudioCodec
{
public:
	CG729aCompress();
	virtual  ~CG729aCompress();

	virtual BOOL Compress(char* src,int srclen,char* dst,int* dstlen);
	virtual BOOL UnCompress(char* src,int srclen,char* dst,int* dstlen);
};


#endif