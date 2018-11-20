
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
// ��������Socket
SocketArray[0] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
InetAddr.sin_family = AF_INET;
InetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
InetAddr.sin_port = htons(9999);

//-------------------------
// �� socket
bind (SocketArray[0], (SOCKADDR *) &InetAddr, sizeof(InetAddr));

//-------------------------
// �����µ� event����
EventArray[0] = WSACreateEvent();

//-------------------------
// ����Event�����������¼���FD_ACCEPT and FD_CLOSE
// 
WSAEventSelect(SocketArray[0], EventArray[0], FD_ACCEPT|FD_CLOSE);

//-------------------------
// ��ʼ����
listen(SocketArray[0], 10);


//-------------------------
// �ȴ������¼���׼������

printf("TCPServer �ȴ������¼�\n");
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

if ( NetworkEvents.lNetworkEvents & FD_ACCEPT )//FD_ACCEPT���������¼�
{
	if (NetworkEvents.iErrorCode[FD_ACCEPT_BIT]!=0) 
	{
		printf("����ʧ�ܡ�����ţ�%d\n",NetworkEvents.iErrorCode[FD_ACCEPT_BIT]);
		break;
	}//if�����ɹ�
//��ʱ���ӳɹ�
if (EventTotal<WSA_MAXIMUM_WAIT_EVENTS) 
{//û���� ,�����µģӣϣãˣţ�

   SocketArray[EventTotal]= accept(SocketArray[Index],NULL,NULL);
   printf("SOCKET:%d�������ӳɹ���\n",SocketArray[EventTotal]);
   EventArray[EventTotal]= WSACreateEvent();  
   WSAEventSelect(SocketArray[EventTotal],EventArray[EventTotal],FD_READ|FD_WRITE|FD_CLOSE);
   EventTotal++;
   
}//if-���ӳɹ�
}//if-���������¼�

if ( NetworkEvents.lNetworkEvents & FD_READ )//FD_READ������¼� 
{
  if (NetworkEvents.iErrorCode[FD_READ_BIT]!=0) 
	{
		printf("FD_READ ʧ�ܡ�����ţ�%d\n",NetworkEvents.iErrorCode[FD_READ_BIT]);
		break;
	}//if�����ɹ�
   printf("Socket��%d �Ѷ�������\n",SocketArray[Index]);
  char data[254];
  int n = recv(SocketArray[Index],data,253,0);
  data[n]='\0';
  printf("���ܵ����ݣ�%s\n",data);
  //����д��Ӧ
  send(SocketArray[Index],"���յ�",6,0);
}//if������¼�
/*
if ( NetworkEvents.lNetworkEvents & FD_WRITE )//FD_WRITE������¼� 
{
  if (NetworkEvents.iErrorCode[FD_WRITE_BIT]!=0) 
	{
		printf("FD_WRITE ʧ�ܡ�����ţ�%d\n",NetworkEvents.iErrorCode[FD_WRITE_BIT]);
		break;
	}//if�����ɹ�
  printf("Socket��%d �ѿ�д����\n",SocketArray[Index]);
  send(SocketArray[Index],"���յ�",6,0);
  printf("��д���ݣ����յ�\n",SocketArray[Index]);
  
}//ifFD_WRITE����д�¼�
*/
if ( NetworkEvents.lNetworkEvents & FD_CLOSE )//FD_CLOSE������¼� 
{
  if (NetworkEvents.iErrorCode[FD_CLOSE_BIT]!=0) 
	{
		printf("FD_WRITE ʧ�ܡ�����ţ�%d\n",NetworkEvents.iErrorCode[FD_CLOSE_BIT]);
		break;
	}//if�����ɹ�
  printf("Socket��%d �ѹر�\n",SocketArray[Index]);
  closesocket(SocketArray[Index]);
  //ɾ����Socket
  compressArray(Index);
}//ifFD_WRITE����д�¼�

}//while
WSACleanup();
}//main