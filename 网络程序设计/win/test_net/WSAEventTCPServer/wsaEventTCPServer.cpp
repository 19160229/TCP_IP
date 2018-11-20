#include <stdio.h>
#include "winsock2.h"

#define DATA_BUFSIZE 4096

void main() {
  //-----------------------------------------
  // Declare and initialize variables
  WSABUF DataBuf;
  char buffer[DATA_BUFSIZE];
  DWORD EventTotal = 0, 
    RecvBytes = 0, 
    Flags = 0, 
    BytesTransferred = 0, 
    CallBack = 0;
  WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
  WSAOVERLAPPED AcceptOverlapped;
  SOCKET ListenSocket, AcceptSocket;

  //-----------------------------------------
  // Initialize Winsock
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2,2), &wsaData);

  //-----------------------------------------
  // Create a listening socket bound to a local
  // IP address and the port specified
  ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  u_short port = 27015;
  char* ip;
  sockaddr_in service;
  service.sin_family = AF_INET;
  service.sin_port = htons(port);
  hostent* thisHost;
  thisHost = gethostbyname("");
  ip = inet_ntoa (*(struct in_addr *)*thisHost->h_addr_list);
	
  service.sin_addr.s_addr = inet_addr(ip);

  //-----------------------------------------
  // Bind the listening socket to the local IP address
  // and port number
  bind(ListenSocket, (SOCKADDR *) &service, sizeof(SOCKADDR));

  //-----------------------------------------
  // Set the socket to listen for incoming
  // connection requests
  listen(ListenSocket, 1);
  printf("Listening...\n");

  //-----------------------------------------
  // Accept and incoming connection request
  AcceptSocket = accept(ListenSocket, NULL, NULL);
  printf("Client Accepted...\n");

  //-----------------------------------------
  // Create an event handle and setup an overlapped structure.
  EventArray[EventTotal] = WSACreateEvent();
  ZeroMemory(&AcceptOverlapped, sizeof(WSAOVERLAPPED));
  AcceptOverlapped.hEvent = EventArray[EventTotal];
  DataBuf.len = DATA_BUFSIZE;
  DataBuf.buf = buffer;
  EventTotal++;

  //-----------------------------------------
  // Call WSARecv to receive data into DataBuf on 
  // the accepted socket in overlapped I/O mode
  if (WSARecv(AcceptSocket, &DataBuf, 1, &RecvBytes, &Flags, &AcceptOverlapped, NULL) == SOCKET_ERROR) {
    if (WSAGetLastError() != WSA_IO_PENDING)
      printf("Error occurred at WSARecv()\n");
  }

  //-----------------------------------------
  // Process overlapped receives on the socket
  while (1) {
    DWORD Index;

    //-----------------------------------------
    // Wait for the overlapped I/O call to complete
    Index = WSAWaitForMultipleEvents(EventTotal, EventArray, FALSE, WSA_INFINITE, FALSE);

    //-----------------------------------------
    // Reset the signaled event
    WSAResetEvent(EventArray[Index - WSA_WAIT_EVENT_0]);

    //-----------------------------------------
    // Determine the status of the overlapped event
    WSAGetOverlappedResult(AcceptSocket, &AcceptOverlapped, &BytesTransferred, FALSE, &Flags);

    //-----------------------------------------
    // If the connection has been closed, close the accepted socket
    if (BytesTransferred == 0) {
      printf("Closing Socket %d\n", AcceptSocket);
      closesocket(AcceptSocket);
      WSACloseEvent(EventArray[Index - WSA_WAIT_EVENT_0]);
      return;
    }

    //-----------------------------------------
    // If data has been received, echo the received data
    // from DataBuf back to the client
    if (WSASend(AcceptSocket, &DataBuf, 1, &RecvBytes, Flags, &AcceptOverlapped, NULL) == SOCKET_ERROR)
      printf("WSASend() is busted\n");

    //-----------------------------------------		
    // Reset the changed flags and overlapped structure
    Flags = 0;
    ZeroMemory(&AcceptOverlapped, sizeof(WSAOVERLAPPED));

    AcceptOverlapped.hEvent = EventArray[Index - WSA_WAIT_EVENT_0];

    //-----------------------------------------
    // Reset the data buffer
    DataBuf.len = DATA_BUFSIZE;
    DataBuf.buf = buffer;
  }
}
