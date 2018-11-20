#include "winsock2.h"
#include "stdio.h"

void main(int argc,char * argv[] )
{
	
 if (argc!=2) 
 {
	 printf("用法：test_inet_addr  <IP> ");
	 exit(0);
 }
struct in_addr ipv4;
ipv4.s_addr = inet_addr(argv[1]);
if (ipv4.s_addr == INADDR_NONE )
{
	printf("非法的IP ：%s\n",argv[1]);
	exit(-1);
}
printf("%x ,%u,%u,%u,%u\n",ipv4.s_addr,ipv4.S_un.S_un_b.s_b1,
	                     ipv4.S_un.S_un_b.s_b2,ipv4.S_un.S_un_b.s_b3,
						 ipv4.S_un.S_un_b.s_b4);
}
