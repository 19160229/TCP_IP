#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<stdio.h>
#include<string.h>

int main() {
	WSADATA wsaDate;
	int res = 0;
	res = WSAStartup(MAKEWORD(2, 2), &wsaDate);
	if (res != 0) {
		printf("WSAStart()����");
		exit(-1);
	}
	char name[80];
	struct hostent* host;
	char **temp;
	strcpy(name, "www.sina.com");
	host = gethostbyname(name);
	if (host == NULL) {
		printf("gethostbyname()����");
		WSACleanup();
		exit(-1);
	}
	printf("��ʽ������Ϊ��%s\n", host->h_name);
	printf("\n��������Ϊ��\n");
	for (temp = host->h_aliases; *temp != NULL; temp++) {
		printf("%s\n", *temp);
	}
	printf("\n��������IP��ַΪ��\n");
	for (temp = host->h_addr_list; *temp != NULL; temp++) {
		printf("%s\n", inet_ntoa(*(struct in_addr *)(*temp)));
	}
	WSACleanup();
	return 0;
}