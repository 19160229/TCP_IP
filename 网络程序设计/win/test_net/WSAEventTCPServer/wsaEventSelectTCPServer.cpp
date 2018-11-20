
#include "winsock2.h"
#include "ws2tcpip.h"
#include "stdio.h"
#include "stdlib.h"

SOCKET SocketArray[WSA_MAXIMUM_WAIT_EVENTS];  
WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
DWORD EventTotal = 1;
void compressArray(int index)
{
  for (DWORD i=index+1;i<EventTotal;i++) {
  SocketArray[i-1]=SocketArray[i];
  EventArray[i-1]=EventArray[i];
  }
}

void main()
{
int stopIt = 0;
WSANETWORKEVENTS NetworkEvents;
sockaddr_in InetAddr;
DWORD Index;

WSADATA wsaData;
WSAStartup(0x0202,&wsaData);

//-------------------------
// 创建倾听Socket
SocketArray[0] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
InetAddr.sin_family = AF_INET;
InetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
InetAddr.sin_port = htons(9999);

//-------------------------
// 绑定 socket
bind (SocketArray[0], (SOCKADDR *) &InetAddr, sizeof(InetAddr));

//-------------------------
// 创建新的 event对象
EventArray[0] = WSACreateEvent();

//-------------------------
// 关联Event对象与网络事件　FD_ACCEPT and FD_CLOSE
// 
WSAEventSelect(SocketArray[0], EventArray[0], FD_ACCEPT|FD_CLOSE);

//-------------------------
// 开始倾听
listen(SocketArray[0], 10);


//-------------------------
// 等待网络事件，准备处理

printf("TCPServer 等待网络事件\n");
while (!stopIt) {

Index = WSAWaitForMultipleEvents(EventTotal, 
  EventArray, 
  FALSE, 
  WSA_INFINITE, 
  FALSE);
Index = Index - WSA_WAIT_EVENT_0;
WSAEnumNetworkEvents(SocketArray[Index],
      EventArray[Index],
      &NetworkEvents);

if ( NetworkEvents.lNetworkEvents & FD_ACCEPT )//FD_ACCEPT网络连接事件
{
	if (NetworkEvents.iErrorCode[FD_ACCEPT_BIT]!=0) 
	{
		printf("连接失败。错误号：%d\n",NetworkEvents.iErrorCode[FD_ACCEPT_BIT]);
		break;
	}//if－不成功
//此时连接成功
if (EventTotal<WSA_MAXIMUM_WAIT_EVENTS) 
{//没有满 ,产生新的ＳＯＣＫＥＴ

   SocketArray[EventTotal]= accept(SocketArray[Index],NULL,NULL);
   printf("SOCKET:%d　已连接成功！\n",SocketArray[EventTotal]);
   EventArray[EventTotal]= WSACreateEvent();  
   WSAEventSelect(SocketArray[EventTotal],EventArray[EventTotal],FD_READ|FD_WRITE|FD_CLOSE);
   EventTotal++;
   
}//if-连接成功
}//if-网络连接事件

if ( NetworkEvents.lNetworkEvents & FD_READ )//FD_READ网络读事件 
{
  if (NetworkEvents.iErrorCode[FD_READ_BIT]!=0) 
	{
		printf("FD_READ 失败。错误号：%d\n",NetworkEvents.iErrorCode[FD_READ_BIT]);
		break;
	}//if－不成功
   printf("Socket：%d 已读到数据\n",SocketArray[Index]);
  char data[254];
  int n = recv(SocketArray[Index],data,253,0);
  data[n]='\0';
  printf("接受的数据：%s\n",data);
  //立即写响应
  send(SocketArray[Index],"已收到",6,0);
}//if网络读事件
/*
if ( NetworkEvents.lNetworkEvents & FD_WRITE )//FD_WRITE网络读事件 
{
  if (NetworkEvents.iErrorCode[FD_WRITE_BIT]!=0) 
	{
		printf("FD_WRITE 失败。错误号：%d\n",NetworkEvents.iErrorCode[FD_WRITE_BIT]);
		break;
	}//if－不成功
  printf("Socket：%d 已可写数据\n",SocketArray[Index]);
  send(SocketArray[Index],"已收到",6,0);
  printf("已写数据：已收到\n",SocketArray[Index]);
  
}//ifFD_WRITE网络写事件
*/
if ( NetworkEvents.lNetworkEvents & FD_CLOSE )//FD_CLOSE网络读事件 
{
  if (NetworkEvents.iErrorCode[FD_CLOSE_BIT]!=0) 
	{
		printf("FD_WRITE 失败。错误号：%d\n",NetworkEvents.iErrorCode[FD_CLOSE_BIT]);
		break;
	}//if－不成功
  printf("Socket：%d 已关闭\n",SocketArray[Index]);
  closesocket(SocketArray[Index]);
  //删除该Socket
  compressArray(Index);
}//ifFD_WRITE网络写事件

}//while
WSACleanup();
}//main