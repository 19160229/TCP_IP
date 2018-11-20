//ģ����: tcpserver.cpp
//
// ��������:
//
//    һ���򵥵�TCP ������Ӧ�ó�����9999�˿ں��ϵȴ����ӡ�
//    
//�÷�:
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

   // ��ʼ�� Winsock version 2.2

   if ((Ret = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0)
   {
      
      printf("WSAStartup ��������ţ� %d\n", Ret);
      return;
   }
   
   // ����һ��������Socket.
 
   if ((ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) 
       == INVALID_SOCKET)
   {
      printf("socket ��������ţ� %d\n", WSAGetLastError());
      WSACleanup();
      return;
   } 

   // ���SOCKADDR_IN �ṹ��
   
   ServerAddr.sin_family = AF_INET;
   ServerAddr.sin_port = htons(Port);    
   ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

   // ��Socket

   if (bind(ListeningSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr)) 
       == SOCKET_ERROR)
   {
      printf("bind ��������ţ� %d\n", WSAGetLastError());
      closesocket(ListeningSocket);
      WSACleanup();
      return;
   }

   // ת��������Socket

   if (listen(ListeningSocket, 5) == SOCKET_ERROR)
   {
      printf("listen ��������ţ� %d\n", WSAGetLastError());
      closesocket(ListeningSocket);
      WSACleanup();
      return;
   } 

   printf("�ڶ˿ںţ� %d �ϵȴ�����.\n", Port);

   // ��������.
   ClientAddrLen=sizeof(ClientAddr);
   if ((NewConnection = accept(ListeningSocket, (SOCKADDR *) &ClientAddr,
                               &ClientAddrLen)) == INVALID_SOCKET)
   {
      printf("accept ��������ţ� %d\n", WSAGetLastError());
      closesocket(ListeningSocket);
      WSACleanup();
      return;
   }


   printf("���ӳɹ�����������  %s:%d.\n",
          inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));

   // ��ʱ�ɹر�����Socket����Ϊ�����ٵȴ��µ������ˡ�
   
   closesocket(ListeningSocket);

   // ��ʼ��������
//���ö��ĳ�ʱѡ����������롣����һ�������ٵȴ�����
int time = 3000;
setsockopt(NewConnection,SOL_SOCKET,SO_RCVTIMEO,(char *)&time,sizeof(time));
int ntime=0;
int isize=sizeof(int);
getsockopt(NewConnection,SOL_SOCKET,SO_RCVTIMEO,(char*)&ntime,&isize);
printf("���ܳ�ʱʱ�䣺%d\n",ntime);
   

printf("��ʼ��������...\n");

   if ((Ret = recv(NewConnection, DataBuffer, sizeof(DataBuffer), 0)) 
       == SOCKET_ERROR)
   {
      printf("recv ��������ţ� %d\n", WSAGetLastError());
      closesocket(NewConnection);
      WSACleanup();
      return;
   } 

   DataBuffer[Ret]='\0';
   printf("�ɹ����� %d ���ֽ�.\n���������ǣ�%s\n", Ret,DataBuffer);   

   

   printf("�ر�����.\n");

   closesocket(NewConnection);

   
   WSACleanup();
}
