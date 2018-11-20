#include "winsock2.h"
#include "process.h"
#include "stdio.h"
#include "stdlib.h"
#include "ws2tcpip.h"


int count(char *s)
{
	int cc=0;
	for(int i=0;s[i];i++)
	{
		if (isalpha(s[i])) 
		{
			cc++;
		}
	}
	return cc;
}

u_int CALLBACK tcpserver(void * param)
{
 SOCKET s = (SOCKET)param;

//设置读超时选项：若10秒读不到数据则返回。
DWORD waittime=10000;//10秒

int res=setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,(char*)&waittime,sizeof(waittime));
if ( res ==-1) 
{

	printf("setsockopt()出错号：%d\n",WSAGetLastError());
}
 char data[254];
 do {
 	int n=recv(s,data,250,0);
	if ( n==SOCKET_ERROR ) 
	{
	 int n=WSAGetLastError();
	 if (n==10060) {
		printf("读操作超时了！");
	 }
		printf("recv()错误号：%d\n",WSAGetLastError());
		break;
	}
	data[n]='\0';
	printf("线程%d输出:客户机数据是：%s\n",GetCurrentThreadId(),data);
    char rd[254];
	sprintf(rd,"字母个数：%d",count(data));
	send(s,rd,strlen(rd),0);

 } while(strcmp(data,"quit")!=0);
 printf("线程ID:%d要结束。\n",GetCurrentThreadId());
 closesocket(s);
 
 return 1;
}//tcpserver

void _ERROR(char *s)
{
	printf("%s出错！代码是：%d\n",s,WSAGetLastError() );
}


void main(void)
{

 WSADATA wsaData;
 if ( WSAStartup(0x0202,&wsaData)!=0 )
 {
	 _ERROR("WSAStartup()");
	 return;
 }
SOCKET ss = socket(AF_INET,SOCK_STREAM,0);
if ( ss==INVALID_SOCKET )  
{
	_ERROR("socket()");
	WSACleanup();
	return;
}

struct sockaddr_in sip;
memset( &sip,0,sizeof(sip) );
sip.sin_family = AF_INET;
sip.sin_port = htons( 9999 );
sip.sin_addr.s_addr = htonl(INADDR_ANY);
if ( bind(ss,(struct sockaddr*)&sip,sizeof(sip))==SOCKET_ERROR ) 
{
	_ERROR("bind()");
	closesocket(ss);
	WSACleanup();
	return;
}

if ( listen(ss,5)==SOCKET_ERROR ) 
{
	_ERROR("listen()");
	closesocket(ss);
	WSACleanup();
	return;
}

SOCKET cs=0;
int stopIt=0;
while (!stopIt) 
{
   printf("TCP服务器正在端口号：9999上等待连接.....\n");
   struct sockaddr_in cip;
   int len = sizeof(cip);
   cs = accept( ss, (struct sockaddr* )&cip,&len );
   if ( cs == INVALID_SOCKET ) 
   {
	   _ERROR("accept()");
	   break;
   }
   printf("客户机IP：%s 端口号：%d已连接进来！\n",inet_ntoa(  cip.sin_addr ),ntohs(cip.sin_port) );
   
   u_int  id=0;
   HANDLE hThread =(HANDLE)_beginthreadex(NULL,0,tcpserver,(void*)cs,0,&id); 
   if ( hThread==0 ) 
   {
	   printf("线程创建不成功。\n");
	   break;
   }
   printf("已创建新线程:%d \n",id);
}//while

closesocket(ss);
WSACleanup();
return;


}//main