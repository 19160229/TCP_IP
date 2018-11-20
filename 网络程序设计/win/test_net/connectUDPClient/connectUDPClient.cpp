#include "winsock2.h"
#include "stdio.h"
#include "stdlib.h"

void main(void)
{
WSADATA wsaData;
WSAStartup(0x0202,&wsaData);
SOCKET s=socket(AF_INET,SOCK_DGRAM,0);
char data[254];
sockaddr_in sip;
memset(&sip,0,sizeof(sip));
sip.sin_family=AF_INET;
sip.sin_port=htons(9999);
sip.sin_addr.s_addr=inet_addr("127.0.0.1");
connect(s,(sockaddr*)&sip,sizeof(sip));

do{
   gets(data);
  // sendto(s,data,strlen(data),0,(sockaddr*)&sip,sizeof(sip));
   send(s,data,strlen(data),0);
   char rd[254];
 //  int n=recvfrom(s,rd,253,0,NULL,NULL);
   int n=recv(s,rd,253,0);
   rd[n]=0;
   printf("UDP服务器的响应是：%s\n",rd);
}while(strcmp(data,"quit")!=0);
printf("再见！");
closesocket(s);
WSACleanup();
}