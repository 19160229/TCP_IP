#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<stdio.h>
#include<string.h>

int main() {
	WSADATA wsaDate;
	int res = 0;
	res = WSAStartup(MAKEWORD(2, 2), &wsaDate);
	if (res != 0) {
		printf("WSAStart()出错");
		exit(-1);
	}
	char name[80];
	struct hostent* host;
	char **temp;
	strcpy(name, "www.sina.com");
	host = gethostbyname(name);
	if (host == NULL) {
		printf("gethostbyname()出错");
		WSACleanup();
		exit(-1);
	}
	printf("正式主机名为：%s\n", host->h_name);
	printf("\n主机别名为：\n");
	for (temp = host->h_aliases; *temp != NULL; temp++) {
		printf("%s\n", *temp);
	}
	printf("\n主机所有IP地址为：\n");
	for (temp = host->h_addr_list; *temp != NULL; temp++) {
		printf("%s\n", inet_ntoa(*(struct in_addr *)(*temp)));
	}
	WSACleanup();
	return 0;
}