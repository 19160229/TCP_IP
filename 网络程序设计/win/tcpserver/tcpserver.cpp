//模块名: tcpserver.cpp
//
// 功能描述:
//
//    一个简单的TCP 服务器应用程序：在9999端口号上等待连接。
//    
//用法:
//
//    tcpserver.exe
//



#include <winsock2.h>
#include <stdio.h>

void main(void)
{
   WSADATA              wsaData;
   SOCKET               ListeningSocket;
   SOCKET               NewConnection;
   SOCKADDR_IN          ServerAddr;
   SOCKADDR_IN          ClientAddr;
   int                  ClientAddrLen;
   int                  Port = 9999;
   int                  Ret;
   char                 DataBuffer[1024];

   // 初始化 Winsock version 2.2

   if ((Ret = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0)
   {
      
      printf("WSAStartup 出错。错误号： %d\n", Ret);
      return;
   }
   
   // 创建一个倾听型Socket.
 
   if ((ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) 
       == INVALID_SOCKET)
   {
      printf("socket 出错。错误号： %d\n", WSAGetLastError());
      WSACleanup();
      return;
   } 

   // 填充SOCKADDR_IN 结构。
   
   ServerAddr.sin_family = AF_INET;
   ServerAddr.sin_port = htons(Port);    
   ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

   // 绑定Socket

   if (bind(ListeningSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr)) 
       == SOCKET_ERROR)
   {
      printf("bind 出错。错误号： %d\n", WSAGetLastError());
      closesocket(ListeningSocket);
      WSACleanup();
      return;
   }

   // 转换成倾听Socket

   if (listen(ListeningSocket, 5) == SOCKET_ERROR)
   {
      printf("listen 出错。错误号： %d\n", WSAGetLastError());
      closesocket(ListeningSocket);
      WSACleanup();
      return;
   } 

   printf("在端口号： %d 上等待连接.\n", Port);

   // 接受连接.
   ClientAddrLen=sizeof(ClientAddr);
   if ((NewConnection = accept(ListeningSocket, (SOCKADDR *) &ClientAddr,
                               &ClientAddrLen)) == INVALID_SOCKET)
   {
      printf("accept 出错。错误号： %d\n", WSAGetLastError());
      closesocket(ListeningSocket);
      WSACleanup();
      return;
   }


   printf("连接成功。连接来自  %s:%d.\n",
          inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));

   // 此时可关闭倾听Socket。因为不想再等待新的连接了。
   
   closesocket(ListeningSocket);

   // 开始接受数据
//设置读的超时选项：不超过３秒。３秒一到，不再等待读。
int time = 3000;
setsockopt(NewConnection,SOL_SOCKET,SO_RCVTIMEO,(char *)&time,sizeof(time));
int ntime=0;
int isize=sizeof(int);
getsockopt(NewConnection,SOL_SOCKET,SO_RCVTIMEO,(char*)&ntime,&isize);
printf("接受超时时间：%d\n",ntime);
   

printf("开始接受数据...\n");

   if ((Ret = recv(NewConnection, DataBuffer, sizeof(DataBuffer), 0)) 
       == SOCKET_ERROR)
   {
      printf("recv 出错。错误号： %d\n", WSAGetLastError());
      closesocket(NewConnection);
      WSACleanup();
      return;
   } 

   DataBuffer[Ret]='\0';
   printf("成功接受 %d 个字节.\n数据内容是：%s\n", Ret,DataBuffer);   

   

   printf("关闭连接.\n");

   closesocket(NewConnection);

   
   WSACleanup();
}
