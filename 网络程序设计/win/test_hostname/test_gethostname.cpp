#include "winsock2.h"
#include "stdio.h"

void main(void)
{
 
 WSADATA wsaData;
 int ret=0;
 ret = WSAStartup(MAKEWORD(2,2),&wsaData);
 if (ret!=0)
 {
   printf("WSAStartup() 出错。代码:%d \n",WSAGetLastError());
	  exit(-1);
 }

  char name[80];
  int len = 0;
  len = sizeof(name);
  if (gethostname(name,len)==SOCKET_ERROR)
  {
	  printf("gethostname() 出错。代码:%d \n",WSAGetLastError());
	  WSACleanup();
	  
	  exit(-1);
  }
  printf("本地主机名：%s\n",name);
  WSACleanup();
}
