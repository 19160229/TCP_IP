#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<stdio.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd) {
	WSADATA wsaDate;
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaDate) != 0) {
		char c[80];
		sprintf_s(c, "WinSock����ʧ��");
		MessageBox(NULL, c, "WinSock����ʧ��", 0);
		return -1;
	}
	if (LOBYTE(wsaDate.wVersion) != 2 || HIBYTE(wsaDate.wVersion) != 2) {
		char c[80];
		sprintf_s(c, "Winsock�汾�Ų�ƥ��");
		MessageBox(NULL, c, "Winsock�汾�Ų�ƥ��", 0);
		WSACleanup();
		return -1;
	}
	char c[80];
	sprintf_s(c, "Winsock���óɹ���\n�汾�ţ�%d.%d\n������%s\n״̬��%s\n",
		HIBYTE(wsaDate.wVersion), LOBYTE(wsaDate.wVersion), wsaDate.szDescription, wsaDate.szSystemStatus);
	MessageBox(NULL, c, "WSAStart ���óɹ�", 0);
	WSACleanup();
	return 0;
}