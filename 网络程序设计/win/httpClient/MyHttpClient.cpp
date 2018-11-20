#include "winsock2.h"
#include "stdio.h"

void _ERROR(char * s)
{
  printf("%s出错！错误代码：%d\n",s,WSAGetLastError());
}

void main(void)
{
 WSADATA wsaData;
 if (WSAStartup(0x0202,&wsaData)!=0)
 {
	 _ERROR("WSAStartup()");
	 exit(-1);
 }

 SOCKET sd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
 if (sd==INVALID_SOCKET)
 {
	 _ERROR("socket()");
	 WSACleanup();
	 exit(-1);
 }
 struct sockaddr_in saddr;
 memset(&saddr,0,sizeof(saddr));
 saddr.sin_family=AF_INET;
 saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
 saddr.sin_port=htons(8080);

 printf("向服务器：127.0.0.1 端口号：8080 发起连接...");
 if (connect(sd,(struct sockaddr * )&saddr,sizeof(saddr))==SOCKET_ERROR)
 {
	 _ERROR("connect()");
	 closesocket(sd);
	 WSACleanup();
	 exit(-1);
 }

 printf("已连接成功！\n");

 char data[2048]="GET http://127.0.0.1:8080/index.jsp HTTP/1.1\n\r\n\r\n";
 char rbuf[64*1024];
 do {
 	//gets(data);

	send(sd,data,strlen(data),0);
	
	int rc=recv(sd,rbuf,64*1024-1,0);
	if (rc==SOCKET_ERROR) {
		break;
	}
    rbuf[rc]=0;
	printf("服务器响应：\n%s\n",rbuf);
 } while(0);
     printf("网页获取结束！\n");
     closesocket(sd);
	 WSACleanup();
}
