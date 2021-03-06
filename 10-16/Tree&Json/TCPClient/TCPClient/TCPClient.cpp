#pragma comment(lib,"ws2_32.lib")
#include "winsock2.h"
#include<stdio.h>
#include"Tree.h"

void _ERROR(const char* s) {
	printf("%s出错！错误代码：%d\n", s, WSAGetLastError());
}

int main() {
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		_ERROR("WSAStartup()");
		exit(-1);
	}
	SOCKET sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sd == INVALID_SOCKET) {
		_ERROR("socket()");
		WSACleanup();
		exit(-1);
	}
	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(9999);
	printf("向服务器:127.0.0.1 端口号：9999 发起连接...");
	if (connect(sd, (struct sockaddr *)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		_ERROR("connect()");
		closesocket(sd);
		WSACleanup();
		exit(-1);
	}
	printf("已连接成功!\n");
	char* data;
	char rbuf[254];
	CSNode* root = (CSNode *)malloc(sizeof(CSNode));
	InitTree(root);
	data = makeJson(root);
	if (data == NULL) {
		send(sd, "error", 10, 0);
		int rc = recv(sd, rbuf, 254, 0);
		rbuf[rc] = 0;
		printf("服务器响应：%s\n", rbuf);
	}
	else {
		printf("发送的Json数据：%s\n", data);
		send(sd, data, strlen(data), 0);
		int rc = recv(sd, rbuf, 254, 0);
		rbuf[rc] = 0;
		printf("服务器响应：%s\n", rbuf);
	}
	freeTree(root);
	getchar();
	/*do {
		gets_s(data);
		send(sd, data, strlen(data), 0);
		int rc = recv(sd, rbuf, 254, 0);
		rbuf[rc] = 0;
		printf("服务器响应：%s\n", rbuf);
	} while (strcmp(data, "quit") != 0);*/
	return 0;
}