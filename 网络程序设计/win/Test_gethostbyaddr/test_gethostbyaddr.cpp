#include "winsock2.h"
#include "stdio.h"

void main(void)
{
WSADATA wsaData;
int ret=0;
if ( (ret=WSAStartup(0x0202,&wsaData) )!=0 )
{
	printf("WSAStartup 出错！代码：%d \n",WSAGetLastError());
	exit(-1);
}

//61.172.201.194 for www.sina.com
struct in_addr ipv4;
struct hostent *phost;

if( ( ipv4.s_addr = inet_addr("127.0.0.1") ) == INADDR_NONE )
{
  printf("inet_addr出错！出错代码：%d \n",WSAGetLastError());
  goto end;
}

if((phost=gethostbyaddr((char*)&ipv4,4,AF_INET)) ==NULL )
{
 printf("gethostbyaddr出错！出错代码：%d \n",WSAGetLastError());
  goto end;
}

printf("主机正式名：%s\n地址类型：%u\n地址长度：%u\n",phost->h_name,
	   phost->h_addrtype,
	   phost->h_length	   );

printf("主机别名列表：\n");
char ** temp;
for (temp= phost->h_aliases;*temp!=NULL;temp++)
{
	printf("%s\n",*temp);
}

printf("主机IP地址列表：\n");

for (temp= phost->h_addr_list;*temp!=NULL;temp++)
{
	printf("%s\n",inet_ntoa(*(struct in_addr *)(*temp)));
}

end: WSACleanup();
}
