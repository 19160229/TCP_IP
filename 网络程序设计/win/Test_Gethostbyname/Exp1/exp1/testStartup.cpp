#include "winsock2.h"
#include "stdio.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd ) 
{
 WSADATA wsaData;
 int ret;

 ret = WSAStartup(0x0202,&wsaData);
 if ( ret != 0) {
	 MessageBox(NULL,"WSAStartup���ɹ���","Startup",0);
	 return -1;
 }
 int a=1;
 int b=a=2;
 printf("b=%d");
MessageBox(NULL,"�ɹ���","startup",0);
WSACleanup();
return 0;
}
