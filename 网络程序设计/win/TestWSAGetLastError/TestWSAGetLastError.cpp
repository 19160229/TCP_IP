#include "winsock2.h"
#include "stdio.h"

void main(void)

{
   WSADATA wsaData;
   int Ret=0; 
struct in_addr sip;
sip.s_addr=inet_addr("192.168.0.200");

   // 初始化 Winsock version 2.2

   if ((Ret = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0)
   {
      
      printf("WSAStartup 装载出错。错误号： %d\n", Ret);
      return ;
   }

   // 此处是网络通信代码 
   
   // 程序要结束运行时，要使用WSACleanup()。
   if (WSACleanup() == SOCKET_ERROR)
   {
      printf("WSACleanup 出错。错误号： %d\n", WSAGetLastError());
   }
   return ;
}
