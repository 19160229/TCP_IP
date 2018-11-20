#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>

void _ERROR(char *s)
{
	printf("%s出错!错误代码:%d\n", s, WSAGetLastError());
}

int count(char *s)
{
	int num = 0;
	for (int i = 0; s[i] != '\0'; i++)
		if (isalpha(s[i]))
			num++;
	return num;
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
	server.sin_port = htons(9999);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (bind(udps, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		_ERROR("bind()");
		closesocket(udps);
		WSACleanup();
		exit(-1);
	}
	struct sockaddr_in client;
	int addrlen = sizeof(client);
	char rdata[254];
	char sdata[254];
	do
	{
		int rc = recvfrom(udps, rdata, 250, 0, (struct sockaddr*)&client, &addrlen);
		if (rc == 0) break;
		if (rc == SOCKET_ERROR)
		{
			_ERROR("recvfrom()");
			closesocket(udps);
			WSACleanup();
			exit(-1);
		}
		rdata[rc] = '\0';
		printf("客户机数据:%s\n", rdata);
		int num = count(rdata);
		sprintf(sdata, "字母个数:%d\n", num);
		sendto(udps, sdata, strlen(sdata), 0, (struct sockaddr*)&client, addrlen);
	} while (strcmp(rdata, "quit") != 0);
	printf("再见!\n");
	closesocket(udps);
	WSACleanup();
	return 0;
}