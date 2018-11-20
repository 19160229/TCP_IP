/*********************************
 *	DSocket.h
 *	By	L0op8ack@
 *********************************/
#ifndef _DSOCKET_H_
#define _DSOCKET_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Type of datagram socket
#include "AudioCodec.h"

#define TYPE_CONTROL 11
#define TYPE_AUDIO   12
#define TYPE_VIDEO   13
#define TYPE_TEXT    14

// Port used for different datagram sockets

#define PORT_CONTROL 8192
#define PORT_AUDIO   8196
#define PORT_VIDEO   8200
#define PORT_TEXT    8204

// Message Types...
#define MESG_CONNECT     101
#define MESG_DISCONNECT  102
#define MESG_ACCEPT      103
#define MESG_REJECT      104

class DSocket : public CSocket
{
public:
	CDialog *pDlg;
	char localname[256];
	static char remotename[512];
	static char remoteaddress[512];
	unsigned int rmtport;
	int type;
	
	char *m_data;
	unsigned int m_dlen;
	
// Operations
public:
	DSocket();
	virtual ~DSocket();
	BOOL CreateSocket(int port,int type);
	void SetParent(CDialog *dlg);
	void OnReceive(int errcode);
	BOOL SendControlMessage(int type,char *address);
	void SendAudioData(unsigned char *data,int length);
	void SendVideoData(unsigned char *data,int length);
	void SendTextData(unsigned char *data,short length);
	void CloseSocket();
};

#endif
