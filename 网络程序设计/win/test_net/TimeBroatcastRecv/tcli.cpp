//����ʱ�������ÿ3���ʱ��㲥��

#include "winsock2.h"
#include "stdio.h"
#include "stdlib.h"
#include "Ws2tcpip.h"

void main(void)
{
WSADATA wsaData;
WSAStartup(0x0202,&wsaData);
SOCKET s=socket(AF_INET,SOCK_DGRAM,0);


sockaddr_in sip;
int len = sizeof(sip);

memset(&sip,0,sizeof(sip));
sip.sin_family=AF_INET;
sip.sin_port=htons(9999);
sip.sin_addr.s_addr=htonl(INADDR_ANY);
if ( bind(s,(sockaddr*)&sip,sizeof(sip))==SOCKET_ERROR) 
{
  printf("bind() error\n");
  closesocket(s);
  WSACleanup();
}

char data[254];
int cc=10;
do{
   
   char rd[254];
   int n=recvfrom(s,rd,253,0,NULL,NULL);
   rd[n]=0;
   printf("���ܵ���ʱ��㲥�������ǣ�%s\n",rd);
   cc--;
}while(cc>0);

printf("�ټ���");
closesocket(s);
WSACleanup();
}
