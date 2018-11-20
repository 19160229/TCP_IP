#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>

void _ERROR(char *s)
{
	printf("%s出错!错误代码:%d\n", s, WSAGetLastError());
}

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		_ERROR("WSAStartup()");
		exit(-1);
	}
	SOCKET udps = socket(AF_INET, SOCK_DGRAM, 0);
	if(udps == INVALID_SOCKET)
	{
		_ERROR("socket()");
		WSACleanup();
		exit(-1);
	}
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(9999);
	int len = sizeof(server);
	char sdata[254];
	char rdata[254];
	do{
		gets_s(sdata);
		sendto(udps, sdata, strlen(sdata), 0, (struct sockaddr*)&server, len);

		int rc = recvfrom(udps, rdata, 250, 0, (struct sockaddr*)&server, &len);
		rdata[rc] = '\0';
		printf("服务器响应:%s\n", rdata);
	} while (strcmp(sdata, "quit") != 0);
	printf("再见!\n");
	closesocket(udps);
	WSACleanup();
	return 0;
}