#pragma comment(lib,"ws2_32.lib ")

#include "winsock2.h"
#include "stdio.h"

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
  WSADATA wsaData;
  
  int r = WSAStartup
	  (MAKEWORD(2,2),&wsaData);
  if( r != 0 )
  {
    char err[80];
	sprintf(err,"Winsock2 装载出错了!错误号：%d ",WSAGetLastError());
    MessageBox(NULL,err,"Winsock2测试",0);	
	return -1;
  }//if
  
  if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) 
  {
   
    MessageBox(NULL,"Winsock2 装载的版本不对！","Winsock2测试",0);
    WSACleanup();                           
    return -1;
  }//if
 
   char data[80];
   sprintf(data,"WSAStartup 装载成功!\n版本是：%d.%d \n描述：%s\n状态：%s\n ",
	   HIBYTE( wsaData.wVersion ),LOBYTE( wsaData.wVersion ),
	   wsaData.szDescription,wsaData.szSystemStatus);
   MessageBox(NULL,data,"WSAStartup",0);
   WSACleanup();  
   return 0;
}//winmain

