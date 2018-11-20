#include "winsock2.h"
#include "stdio.h"
#include "string.h"
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
printf("描述：%s,状态：%s \n",wsaData.szDescription,
	                   wsaData.szSystemStatus);
  char name[80];
  struct hostent * host;
  char ** temp;
  //strcpy(name,"www.sina.com.cn");

  host = gethostbyname(NULL);//相当于：gethostbyname("jnscomputer1");
 //host = gethostbyname("www.sina.com.cn");
  if (host==NULL)
  {
	  printf("gethostbyname() 出错。域名:%s 对应的主机找不到。 \n",name);
	  WSACleanup();
	  exit(-1);
  }
  printf("正式主机名：%s\n地址长度：%d\n地址类型：%d\n",
	  host->h_name,host->h_length,host->h_addrtype);
  printf("主机的别名列表如下：\n");
  for (temp=host->h_aliases;*temp!=NULL;temp++)
  {
	  printf("%s\n",*temp);
  }
  printf("主机的IP列表如下：\n");
  for ( temp=host->h_addr_list;*temp!=NULL;temp++)
  {
	  printf("%s\n",inet_ntoa(*(struct in_addr *)(*temp)));
  }
  WSACleanup();
}
