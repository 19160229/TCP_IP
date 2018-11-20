
#include "winsock2.h"
#include "ws2tcpip.h"
#include "stdio.h"
#include "stdlib.h"
#include "process.h"

SOCKET ms;//�鲥�˿�
SOCKADDR_IN sip,to;
struct ip_mreq mreq;//�鲥�ṹ


void _ERROR(char *s)
{
  printf("%s����������룺%d\n",GetLastError());
}
/*
unsigned int CALLBACK recvthread(void* args)
{//ֻ��������н������ݰ����߳�
	char data[254];
	int stopIt=0;
	while(!stopIt){
	if (stopThread) 
	{
		_endthreadex(1);
	}
	int len = sizeof(to);
	int n = recvfrom(ms,data,253,0,(sockaddr*)&to,&len);
    data[n]=0;
	printf("�����н��յ���%s\n",data);
	} //while
	return 1;
}
*/
void main(int argc,char ** argv)
{
 

 if ( argc < 3 ) 
 {
	 printf("�÷�: %s <�鲥��ַ>  <�˿ں�>  [<���ص�ַ>]",argv[0]);
	 exit(-1);
 }
WSADATA wsaData;
if ( WSAStartup(0x0202,&wsaData) !=0 ) 
{
	_ERROR("WSAStartup()");
	exit(-1);
}

ms = socket(AF_INET,SOCK_DGRAM,0);
if ( ms==INVALID_SOCKET ) 
{
	_ERROR("socket()");
	WSACleanup();
	exit(-1);
}

//���óɷ�������socket

u_long argp=1;
if( ioctlsocket(ms,FIONBIO,&argp) == SOCKET_ERROR )
{
	_ERROR("ioctlsocket()");
}

int len=sizeof(sip);
memset(&sip,0,(size_t)len);
sip.sin_family = AF_INET;
sip.sin_port = htons( atoi(argv[2]) );
if ( argc == 4 ) 
{
	if( ( sip.sin_addr.s_addr = inet_addr(argv[3]) ) ==INADDR_NONE )
	{
	 sip.sin_addr.s_addr = htons(INADDR_ANY);
	}
}
else
{
	sip.sin_addr.s_addr = htons(INADDR_ANY);
}

if ( bind(ms,(sockaddr*)&sip,len) == SOCKET_ERROR ) 
{
	_ERROR("bind()");
	closesocket(ms);
	WSACleanup();
	exit(-1);
}


memcpy(&mreq.imr_interface,&sip.sin_addr,sizeof(mreq.imr_interface));
if ( (mreq.imr_multiaddr.s_addr=inet_addr(argv[1])) ==INADDR_NONE ) 
{
	_ERROR("inet_addr()");
	closesocket(ms);
	WSACleanup();
	exit(-1);
}

//������
if( setsockopt(ms,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char*)&mreq,sizeof(mreq))!=0)
{
	_ERROR("setsockopt()");
	closesocket(ms);
	WSACleanup();
	exit(-1);
}


char data[254];
to.sin_family=AF_INET;
to.sin_port=sip.sin_port;
memcpy(&to.sin_addr,&mreq.imr_multiaddr,sizeof(to.sin_addr));

fd_set readfd;

do {
     
    FD_ZERO(&readfd);
	FD_SET(ms,&readfd);
	FD_SET((u_int)_fileno(stdin),&readfd);
	select(0,&readfd,NULL,NULL,NULL);
	
    if ( FD_ISSET((u_int)_fileno(stdin),&readfd) ) 
	{
    fgets(data,253,stdin);
	printf("����:%s���ͣ�%s\n",argv[1],data);
    sendto(ms,data,strlen(data),0,(sockaddr*)&to,sizeof(to));
    } 
	
	 if ( FD_ISSET(ms,&readfd) ) 
	{
    //��������н������ݰ����߳�
	char data[254];
	
	int len = sizeof(to);
	int n = recvfrom(ms,data,253,0,(sockaddr*)&to,&len);
    data[n]=0;
	printf("�����н��յ���%s\n",data);
	
    } 

} while(memcmp(data,"quit",4)!=0);

setsockopt(ms,IPPROTO_IP,IP_DROP_MEMBERSHIP,(char*)&mreq,sizeof(mreq));
closesocket(ms);
WSACleanup();
}//main

