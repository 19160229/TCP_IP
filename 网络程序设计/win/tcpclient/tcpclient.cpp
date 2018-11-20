// ģ����: tcpclient.cpp
//
// �÷�:
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
      printf("�÷�: tcpclient <������IP��ַ>.\n");
      return;
   }

   // ��ʼ�� Winsock version 2.2

   if ((Ret = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0)
   {
      
      printf("WSAStartup ��������ţ� %d\n", Ret);
      return;
   }
   
   // ��������.
 
   if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
       == INVALID_SOCKET)
   {
      printf("socket ��������ţ� %d\n", WSAGetLastError());
      WSACleanup();
      return;
   }
 
   // ���� SOCKADDR_IN �ṹ ���ڶ˿ں� 9999�����ӡ�

   ServerAddr.sin_family = AF_INET;
   ServerAddr.sin_port = htons(Port);    
   ServerAddr.sin_addr.s_addr = inet_addr(argv[1]);

   // ���������������.

   printf("��������..... %s:%d...\n",
          inet_ntoa(ServerAddr.sin_addr), htons(ServerAddr.sin_port));

   if (connect(s, (SOCKADDR *) &ServerAddr, sizeof(ServerAddr)) 
       == SOCKET_ERROR)
   {
      printf("connect ��������ţ� %d\n", WSAGetLastError());
      closesocket(s);
      WSACleanup();
      return;
   } 

   printf("���ӳɹ�.\n");
      
   // ͨ�Ŵ���Ĵ���.

   printf("��ʼ������Ϣ:��ã�......\n");

Sleep(10000);

   if ((Ret = send(s, "��ã�", 6, 0)) == SOCKET_ERROR)
   {
      printf("send ��������ţ� %d\n", WSAGetLastError());
      closesocket(s);
      WSACleanup();
      return;
   }

   printf("�ɹ����� %d ���ֽ�.\n", Ret);

   // ͨ�����

   printf("��ʼ�ر�����.\n");

   closesocket(s);

   // ���ڴ���ж��DLL

   WSACleanup();
}
