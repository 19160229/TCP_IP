/*********************************
 *	VideoCodec.cpp
 *	By	L0op8ack@
 *********************************/

#include "stdafx.h"
#include "VideoCodec.h"
#include "VideoCapture.h"

CVideoCodec::CVideoCodec()
{
	m_Codec = -1;
//	ugly MS_H261 !
	m_CodecType[0].codec  =	MS_H261;	m_CodecType[0].Fourcc = "M261";
//	MS_H263 176*144*24 20 kbps ,acceptabl for wan,but 320*240*24 failed
	m_CodecType[1].codec  =	MS_H263;	m_CodecType[1].Fourcc = "M263";
//	DIVX_MPEG4 eat tooooooooooooooo much bandwidth 176*144*24 500 kbps!
	m_CodecType[2].codec  =	DIVX_MPEG4;	m_CodecType[2].Fourcc = "DIVX";
//	eat MS_MPEG4Vx too much bandwidth 176*144*24 120 kbps,just for lan !
	m_CodecType[3].codec  =	MS_MPEG4V1;	m_CodecType[3].Fourcc = "MPG4";
	m_CodecType[4].codec  =	MS_MPEG4V2;	m_CodecType[4].Fourcc = "MP42";
	m_CodecType[5].codec  =	MS_MPEG4V3;	m_CodecType[5].Fourcc = "MP43";
//	I think XVID_MPEG4 nerver had a release version,the 1st time nice,2nd? BULL shxt!
//	176*144*24 20 kbps,nice graghic!
	m_CodecType[6].codec  =	XVID_MPEG4;	m_CodecType[6].Fourcc = "XVID";
//	following 2 ugly codecs,no one will suggest,or IDIOT !
	m_CodecType[7].codec  =	MS_VIDC	  ;	m_CodecType[7].Fourcc = "MSVC";
	m_CodecType[8].codec  =RADIUS_CVID;	m_CodecType[8].Fourcc = "CVID";
}

CVideoCodec::~CVideoCodec()
{
}

BOOL CVideoCodec::SetCodecType(int type)
{	// change range if codec added !
	if(type>=MS_H261 && type <=RADIUS_CVID)		m_Codec=type;
	return (m_Codec!=-1);
}

BOOL CVideoCodec::Init()
{
//	TRACE("CVideoCodec::Init\n");
	Destroy();
	BOOL bRet=FALSE;

	if(m_Codec == -1)	return FALSE;
//	建议安装 K-liteCodecPack2.24FullRC7.rar
//	简易 m_cv.fccHandler 探测,m263/m261/mpg4/divx/xvid......
//	ICINFO icinfo;	CString tmp;	char *p=NULL;
//	ZeroMemory(&icinfo,sizeof(icinfo));
//	if(ICInfo(ICTYPE_VIDEO,mmioFOURCC(m_CodecType[m_Codec].Fourcc[0],m_CodecType[m_Codec].Fourcc[1], \
//		m_CodecType[m_Codec].Fourcc[2],m_CodecType[m_Codec].Fourcc[3]),&icinfo))
//	{
//		p=(char*)(&(icinfo.fccType));
//		tmp.Format ("CVideoCodec::Init ,ICInfo:\ndwSize=%u\nfccType=%C%C%C%C\nfccHandler=%C%C%C%C\ndwFlags=%08X\ndwVersion=%08X\ndwVersionICM=%08X\nszName=[%s]\nszDescription=[%S]\nszDriver=[%S]\n",\
//			icinfo.dwSize ,*p,*(p+1),*(p+2),*(p+3),*(p+4),*(p+5),*(p+6),*(p+7),icinfo.dwFlags ,\
//			icinfo.dwVersion ,icinfo.dwVersionICM ,icinfo.szName ,icinfo.szDescription ,icinfo.szDriver );
//		TRACE(tmp);
//	}

	//	图像压缩/解压缩参数 m_cv 初始化,lKey建议多一点
	ZeroMemory(&m_cv,sizeof(m_cv));

	//填充COMPVARS结构。该结构包含很重要的数据，压缩与解压缩都要用到。
	m_cv.cbSize=sizeof(m_cv);//本结构的大小。以字节为单位。
	m_cv.dwFlags=ICMF_COMPVARS_VALID ;//必须此值.
	m_cv.hic=m_hIC;//压缩程序的HANDLe。该HANDLE由ICOpen(...)获取。
	m_cv.fccType=ICTYPE_VIDEO ;	//"vidc"
	m_cv.fccHandler=
		mmioFOURCC(m_CodecType[m_Codec].Fourcc[0],m_CodecType[m_Codec].Fourcc[1], \
		m_CodecType[m_Codec].Fourcc[2],m_CodecType[m_Codec].Fourcc[3]);
	//对应m_Codec的串的四个字符。压缩器的名字。见该类的构造器。

	m_cv.lpbiOut=0;//采用默认的Image输出格式。
	m_cv.lKey=64;		// I帧（即关键帧）产生速率
	m_cv.lDataRate=20;	// KBPS
	m_cv.lQ=1000;//1-10000。图象压缩后的品质。
	
	//填充BITMAPINFO结构。主要定义视频捕获后显示在屏幕上或存储在文件中的格式。
	//将视频格式固定,一般的视频捕捉设备应该会支持(176*144/320x240)
	ZeroMemory(&m_BmpU,sizeof(m_BmpU));
	m_BmpU.bmiHeader.biSize=sizeof(BITMAPINFOHEADER); //176*144*24
	m_BmpU.bmiHeader.biWidth=IMAGE_WIDTH;//176
	m_BmpU.bmiHeader.biHeight=IMAGE_HEIGHT;//144
	m_BmpU.bmiHeader.biPlanes=1;//必须是1	
	m_BmpU.bmiHeader.biBitCount=24;	//颜色深度
	m_BmpU.bmiHeader.biSizeImage=IMAGE_WIDTH * IMAGE_HEIGHT * (m_BmpU.bmiHeader.biBitCount/8);
//	View your local 'vidc'
//	ICCompressorChoose(pDlg->m_hWnd ,ICMF_CHOOSE_ALLCOMPRESSORS,&m_BmpU,NULL,&m_cv,NULL);

	m_hIC=ICOpen(ICTYPE_VIDEO,m_cv.fccHandler,ICMODE_COMPRESS|ICMODE_DECOMPRESS);
	if(!m_hIC)		goto RET;
	
	ICCompressGetFormat(m_hIC,&m_BmpU,&m_BmpC);//告诉压缩器要压缩的图象格式数据
//	MS H.263 需要消息确认! MPEG4可不用
	ICSendMessage(m_hIC,0x60c9,0xf7329ace,0xacdeaea2);

	m_cv.hic=m_hIC;//设置已打开的压缩器HANDLE。
	m_cv.dwFlags=ICMF_COMPVARS_VALID;

	if(!ICSeqCompressFrameStart(&m_cv,&m_BmpU))		goto RET;
	//压缩器内部压缩资源初始化,准备压缩。
	
	if(ICDecompressBegin(m_hIC,&m_BmpC,&m_BmpU)!=ICERR_OK)		goto RET;
	//解压缩器初始化，准备解压缩。

	bRet=TRUE;
RET:
	if(!bRet)		Destroy();
	return bRet;	
}

void CVideoCodec::Destroy()
{
//	TRACE("CVideoCodec::Destroy\n");
	if(m_hIC)
	{
		//释放相关资源
		ICDecompressEnd(m_hIC);
		ICSeqCompressFrameEnd(&m_cv);
		ICCompressorFree(&m_cv);
		ICClose(m_hIC);
	}
	m_hIC=NULL;
}

//压缩一帧数据
BOOL CVideoCodec::EncodeVideoData(char* pin,int len,char* pout,int* lenr,BOOL* pKey)
{
//	TRACE("CVideoCodec::EncodeVideoData\n");
	BOOL bRet=FALSE;
	char*p;
	long s=2000;
	BOOL k;
	if(!pin||!pout||len!=(int)m_BmpU.bmiHeader.biSizeImage||!m_hIC)
		goto RET;
	//使用VFW压缩一帧数据
	p=(char*)ICSeqCompressFrame(&m_cv,0,pin,&k,&s);
	if(!p)		goto RET;
	if(lenr)	*lenr=s;
	if(pKey)	*pKey=k;
	CopyMemory(pout,p,s);
	bRet=TRUE;
RET:
	return bRet;	
}

//解压缩一帧数据
BOOL CVideoCodec::DecodeVideoData(char *pin,int len,char* pout,int *lenr,DWORD flag)
{
	//TRACE("CVideoCodec::DecodeVideoData\n");
	BOOL bRet=FALSE;
	if(!pin||!pout||!m_hIC)		
		goto RET;
    //使用VFW解压缩一帧数据
	if(ICDecompress(m_hIC,flag,&m_BmpC.bmiHeader,pin,&m_BmpU.bmiHeader,pout)!=ICERR_OK)
		goto RET;
	if(lenr)	*lenr=m_BmpU.bmiHeader.biSizeImage;
	
	bRet=TRUE;
RET:
	return bRet;	
}

void CVideoCodec::SetParent(CDialog *dlg)
{
	pDlg=dlg;
}