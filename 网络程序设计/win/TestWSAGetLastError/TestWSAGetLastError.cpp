#include "winsock2.h"
#include "stdio.h"

void main(void)

{
   WSADATA wsaData;
   int Ret=0; 
struct in_addr sip;
sip.s_addr=inet_addr("192.168.0.200");

   // ��ʼ�� Winsock version 2.2

   if ((Ret = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0)
   {
      
      printf("WSAStartup װ�س�������ţ� %d\n", Ret);
      return ;
   }

   // �˴�������ͨ�Ŵ��� 
   
   // ����Ҫ��������ʱ��Ҫʹ��WSACleanup()��
   if (WSACleanup() == SOCKET_ERROR)
   {
      printf("WSACleanup ��������ţ� %d\n", WSAGetLastError());
   }
   return ;
}
