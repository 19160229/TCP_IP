// 模块名: tcpclient.cpp
//
// 用法:
//
//    tcpclient.exe <server IP address> 
//

#include "winsock2.h"
#include "stdio.h"

void main(int argc, char **argv)
{
   WSADATA              wsaData;
   SOCKET               s;
   SOCKADDR_IN          ServerAddr;
   int                  Port = 9999;
   int                  Ret;

   if (argc <= 1)
   {
      printf("用法: tcpclient <服务器IP地址>.\n");
      return;
   }

   // 初始化 Winsock version 2.2

   if ((Ret = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0)
   {
      
      printf("WSAStartup 出错。错误号： %d\n", Ret);
      return;
   }
   
   // 建立连接.
 
   if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
       == INVALID_SOCKET)
   {
      printf("socket 出错。错误号： %d\n", WSAGetLastError());
      WSACleanup();
      return;
   }
 
   // 设置 SOCKADDR_IN 结构 ，在端口号 9999上连接。

   ServerAddr.sin_family = AF_INET;
   ServerAddr.sin_port = htons(Port);    
   ServerAddr.sin_addr.s_addr = inet_addr(argv[1]);

   // 向服务器发起连接.

   printf("正在连接..... %s:%d...\n",
          inet_ntoa(ServerAddr.sin_addr), htons(ServerAddr.sin_port));

   if (connect(s, (SOCKADDR *) &ServerAddr, sizeof(ServerAddr)) 
       == SOCKET_ERROR)
   {
      printf("connect 出错。错误号： %d\n", WSAGetLastError());
      closesocket(s);
      WSACleanup();
      return;
   } 

   printf("连接成功.\n");
      
   // 通信处理的代码.

   printf("开始发送信息:你好！......\n");

Sleep(10000);

   if ((Ret = send(s, "你好！", 6, 0)) == SOCKET_ERROR)
   {
      printf("send 出错。错误号： %d\n", WSAGetLastError());
      closesocket(s);
      WSACleanup();
      return;
   }

   printf("成功发送 %d 个字节.\n", Ret);

   // 通信完成

   printf("开始关闭连接.\n");

   closesocket(s);

   // 从内存中卸载DLL

   WSACleanup();
}
