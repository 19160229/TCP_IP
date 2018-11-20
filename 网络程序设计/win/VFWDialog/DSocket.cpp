////////////////////////////////////////////////////////////////////////////
//    File description : 
//    Name    : DSocket.cpp
//    Details : Datagram Socket for communication
//		 ***  Control Packet Format ***
//		 Type				: 1 byte
//		 Length of hostname : 1 byte
//		 Hostname			: <var>
//
//		 ***  Text packet format ***
//       Type               : 1 byte
//		 Length of hostname	: 1 byte
//		 Hostname           : < var>
//       Length of message  : 2 byte
//		 Message            : <var>
//
//		 ***  Audio packet format ***
//       Type				  : 1 byte
//		 Length of hostname	  : 1 byte
//		 Hostname             : < var>
//       size of audio data   : 2 byte
//		 Audio data           : <var>
//	
//		 ***  Video packet format ***
//       Type                 : 1 byte
//		 Length of hostname	  : 1 byte
//		 Hostname             : < var>
//       size of video data   : 2 byte
//		 Video data           : <var>
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraghDialog.h"
#include "DSocket.h"
#include "GraghDialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char DSocket::remoteaddress[512]="";
char DSocket::remotename[512]="";

DSocket::DSocket()
{
	m_dlen=0;
	m_data = new char[IMAGE_WIDTH * IMAGE_HEIGHT];
	if(!m_data) AfxMessageBox("DSocket::DSocket 分配缓冲失败");
	else m_dlen=IMAGE_WIDTH * IMAGE_HEIGHT;
}

DSocket::~DSocket()
{
	if(m_data)	delete []m_data;
	m_dlen=0;
}

BOOL DSocket::CreateSocket(int port,int dtype)
{
	type=dtype;
	gethostname(localname,512);	
	switch(type)
	{
	case TYPE_CONTROL:
		rmtport=PORT_CONTROL;
		break;
	case TYPE_AUDIO:
		rmtport=PORT_AUDIO;
		break;
	case TYPE_VIDEO:
		rmtport=PORT_VIDEO;
		break;
	case TYPE_TEXT:
		rmtport=PORT_TEXT;
		break;
	default:
		rmtport=0;
	}

	return Create(port,SOCK_DGRAM);
}

void DSocket::SetParent(CDialog *dlg)
{
	pDlg=dlg;
}

void DSocket::OnReceive(int errcode)
{
	CString address,tmp;
	BOOL	ret=TRUE;
	char hname[512],str[1024],mesg[512];
	unsigned int port=0,retvalue=SOCKET_ERROR;
	int n=0,len=0;
	int ulen=0;

	if(type==TYPE_CONTROL)
	{
		TRACE("DSocket::OnReceive,TYPE_CONTROL,");
		retvalue= ReceiveFrom(m_data,m_dlen,address,port);

		if(retvalue==SOCKET_ERROR && ((CGraghDialogDlg*)pDlg)->m_State==STATE_CALLOUT)
		{
			((CGraghDialogDlg*)pDlg)->m_State = STATE_IDLE;
			((CGraghDialogDlg*)pDlg)->DestroyConference();
			((CGraghDialogDlg*)pDlg)->DisplayMesg("无法连接该联系人");
			return;
		}else if(retvalue==SOCKET_ERROR)	return;
		
		// Get host name from the data.	
		for(int i=0;i<m_data[1];i++)
			hname[i]=m_data[i+2];
		hname[i]=0;
		TRACE("hname=%s,",hname);

		switch(m_data[0])
		{
			// action   : Remote user has sent the invitation for conference
			// reaction : accept(/reject) the invitation
			case MESG_CONNECT:
				TRACE(" from [%s] MESG_CONNECT\n",address);
				((CGraghDialogDlg*)pDlg)->m_State=STATE_CALLIN;
				if(!((CGraghDialogDlg*)pDlg)->m_AutoAccept )
				{
					tmp.Format ("用户 %s 请求连接.\n是否接受该连接?",hname);
					ret=AfxMessageBox(tmp,MB_OKCANCEL);
					if(ret!=IDOK)
					{
						tmp=address;
						strcpy(remoteaddress,(LPCTSTR)address);
						((CGraghDialogDlg*)pDlg)->dcontrol.SendControlMessage(MESG_REJECT,remoteaddress);
						strcpy(remoteaddress,(LPCTSTR)tmp);
						return;
					}
				}
				// maybe wait timeout here,should check it
				if(((CGraghDialogDlg*)pDlg)->m_State==STATE_IDLE) return;
				strcpy(remotename,hname);
				strcpy(remoteaddress,(LPCTSTR)address);
				rmtport = port;
				((CGraghDialogDlg*)pDlg)->StartConference();
				((CGraghDialogDlg*)pDlg)->dcontrol.SendControlMessage(MESG_ACCEPT,remoteaddress);
				sprintf(str,"用户 %s 已连接",hname);
				((CGraghDialogDlg*)pDlg)->DisplayMesg (str);
				return;
			// action   : Remote user has disconnected
			// reaction : destroy the conference
			case MESG_DISCONNECT:
				TRACE("MESG_DISCONNECT\n");
				if(	((CGraghDialogDlg*)pDlg)->m_bStart)
				{
					((CGraghDialogDlg*)pDlg)->DestroyConference();
					sprintf(str,"用户 %s 已离开",hname);
					((CGraghDialogDlg*)pDlg)->DisplayMesg (str);
				}
			return;
			// action   : Remote user has accepted the invitation
			// reaction : start the conference
			case MESG_ACCEPT:
				TRACE("MESG_ACCEPT\n");
				if(	((CGraghDialogDlg*)pDlg)->m_State==STATE_CALLOUT || //FALSE)
					((CGraghDialogDlg*)pDlg)->m_State==STATE_CALLIN )//for local test,commect this while build release version
				{
					strcpy(remotename,hname);
					strcpy(remoteaddress,(LPCTSTR)address);
					((CGraghDialogDlg*)pDlg)->StartConference();
					sprintf(str,"用户 %s 接受您的邀请",hname);
					((CGraghDialogDlg*)pDlg)->DisplayMesg (str);
				}
				return;
			// action   : Remote user has rejected the invitation
			// reaction : what to do...?
			case MESG_REJECT:
				TRACE("MESG_REJECT\n");
				if(	((CGraghDialogDlg*)pDlg)->m_State!=STATE_CONNECTED)
				{
					((CGraghDialogDlg*)pDlg)->DestroyConference();
					sprintf(str,"用户 %s 拒绝了您的邀请",hname);
					((CGraghDialogDlg*)pDlg)->DisplayMesg (str);
				}
				return;
			default:
				TRACE("MESG_%d\n",m_data[0]);
				return;
		}
		return;
	}

	if(type==TYPE_AUDIO)
	{
//		TRACE("DSocket::OnReceive,TYPE_AUDIO\n");
		retvalue=this->ReceiveFrom(m_data,m_dlen,address,port);
		if(retvalue==SOCKET_ERROR)	return;
		rmtport = port;
		((CGraghDialogDlg*)pDlg)->m_G729aCompress .UnCompress (
				(char *)m_data,retvalue,
				((CGraghDialogDlg*)pDlg)->m_AUnComped,
				&ulen );
		if(ulen>0)
			((CGraghDialogDlg *)pDlg)->m_AudioPlay->PostThreadMessage(WM_PLAYSOUND_PLAYBLOCK,ulen,(LPARAM)((CGraghDialogDlg*)pDlg)->m_AUnComped);
		return;
	}

	if(type==TYPE_VIDEO)
	{
		retvalue=ReceiveFrom(m_data,m_dlen,address,port);
		if(retvalue==SOCKET_ERROR)			return;
//		TRACE("DSocket::OnReceive,TYPE_VIDEO, %d Bytes\n",retvalue);
		rmtport = port;
		((CGraghDialogDlg *)pDlg)->m_VideoCodec.DecodeVideoData(
			m_data,
			retvalue,
			((CGraghDialogDlg *)pDlg)->m_VUnComped,
			&ulen,
			0);
//		TRACE("DSocket::OnReceive,TYPE_VIDEO, %d Bytes,Decoded = %d Bytes\n",retvalue,ulen);
		if(ulen>0)
			((CGraghDialogDlg *)pDlg)->DisplayRemoteFrame((unsigned char*)(((CGraghDialogDlg *)pDlg)->m_VUnComped),ulen);
		return;
	}

	if(type==TYPE_TEXT)
	{
//		TRACE("DSocket::OnReceive,TYPE_TEXT\n");
		retvalue=ReceiveFrom(m_data,m_dlen,address,port);

		if(retvalue==SOCKET_ERROR)		return;
		rmtport = port;

		n=m_data[1];

		for(int i=0;i<n;i++)
			hname[i]=m_data[i+2];
		hname[i]=0;
		
		len=m_data[n+2] | ( m_data[n+3]<<8 );

		memcpy(mesg,&m_data[n+4],len);
		mesg[len]=0;

		sprintf(str,"[%s] >> %s ",hname,mesg);

		// Display message
		((CGraghDialogDlg *)pDlg)->m_TextChatDlg .DisplayRemoteMessage (hname,mesg,-1);
		return;
	}
}

BOOL DSocket::SendControlMessage(int type,char *address)
{
	int n;
	BOOL	ret;
	m_data[0]=type;				

	// Length of hostname
	n=strlen(localname);	
	m_data[1]=n;					
	
	// Name of the sender host

	memcpy(&m_data[2],localname,n);

	TRACE("DSocket::SendControlMessage ");
	switch(type)
	{
	case MESG_CONNECT:
		TRACE("MESG_CONNECT");
		break;
	case MESG_DISCONNECT:
		TRACE("MESG_DISCONNECT");
		break;
	case MESG_ACCEPT:
		TRACE("MESG_ACCEPT");
		break;
	case MESG_REJECT:
		TRACE("MESG_REJECT");
		break;
	default:
		TRACE("MESG_UNKNOWN");
		break;
	}

	if(address==NULL)
	{
		TRACE(" to (%s)\n",remoteaddress);
//		SendTo(data,n+2,PORT_CONTROL,remoteaddress);
		ret=SendTo(m_data,n+2,rmtport,remoteaddress);
	}else
	{
		TRACE(" to (%s)\n",address);
//		SendTo(data,n+2,PORT_CONTROL,address);
		ret=SendTo(m_data,n+2,rmtport,address);
	}
	return (ret!=SOCKET_ERROR);
}

void DSocket::SendAudioData(unsigned char *data,int length)
{
//	TRACE("DSocket::SendAudioData %d Bytes\n",length);
//	SendTo(data,length,PORT_AUDIO,remoteaddress);
	if(strlen(remoteaddress)<=1)
	{
		CopyMemory(remoteaddress,((CGraghDialogDlg *)pDlg)->dcontrol .remoteaddress,sizeof(remoteaddress));
		CopyMemory(remotename,((CGraghDialogDlg *)pDlg)->dcontrol .remotename,sizeof(remotename));
	}
	SendTo(data,length,rmtport,remoteaddress);
}

void DSocket::SendVideoData(unsigned char *data,int length)
{
//	SendTo(data,length,PORT_VIDEO,remoteaddress);
	if(strlen(remoteaddress)<=1)
	{
		CopyMemory(remoteaddress,((CGraghDialogDlg *)pDlg)->dcontrol .remoteaddress,sizeof(remoteaddress));
		CopyMemory(remotename,((CGraghDialogDlg *)pDlg)->dcontrol .remotename,sizeof(remotename));
	}
	SendTo(data,length,rmtport,remoteaddress);
//	TRACE("DSocket::SendVideoData %s %d Bytes\n",remoteaddress,length);
}

void DSocket::SendTextData(unsigned char *data,short length)
{
	int n;	

	// Text message
	m_data[0]=TYPE_TEXT;
	
	// Length of hostname
	n=strlen(localname);
	m_data[1]=n;
	
	// Hostname
	memcpy(&m_data[2],localname,n);

	// Data length
	m_data[n+2]=(unsigned char) length;
	m_data[n+3]=(unsigned char) (length>>8);

	// Data
	memcpy(&m_data[n+4],data,length);
//	TRACE("DSocket::SendTextData %d Bytes\n",length);
//	SendTo(packet,n+4+length,PORT_TEXT,remoteaddress);
	if(strlen(remoteaddress)<=1)
	{
		CopyMemory(remoteaddress,((CGraghDialogDlg *)pDlg)->dcontrol .remoteaddress,sizeof(remoteaddress));
		CopyMemory(remotename,((CGraghDialogDlg *)pDlg)->dcontrol .remotename,sizeof(remotename));
	}
	SendTo(m_data,n+4+length,rmtport,remoteaddress);
}

void DSocket::CloseSocket()
{
	DSocket::Close();
}

