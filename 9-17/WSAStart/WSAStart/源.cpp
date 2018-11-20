#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<stdio.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd) {
	WSADATA wsaDate;
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaDate) != 0) {
		char c[80];
		sprintf_s(c, "WinSock≈‰÷√ ß∞‹");
		MessageBox(NULL, c, "WinSock≈‰÷√ ß∞‹", 0);
		return -1;
	}
	if (LOBYTE(wsaDate.wVersion) != 2 || HIBYTE(wsaDate.wVersion) != 2) {
		char c[80];
		sprintf_s(c, "Winsock∞Ê±æ∫≈≤ª∆•≈‰");
		MessageBox(NULL, c, "Winsock∞Ê±æ∫≈≤ª∆•≈‰", 0);
		WSACleanup();
		return -1;
	}
	char c[80];
	sprintf_s(c, "Winsock≈‰÷√≥…π¶£°\n∞Ê±æ∫≈£∫%d.%d\n√Ë ˆ£∫%s\n◊¥Ã¨£∫%s\n",
		HIBYTE(wsaDate.wVersion), LOBYTE(wsaDate.wVersion), wsaDate.szDescription, wsaDate.szSystemStatus);
	MessageBox(NULL, c, "WSAStart ≈‰÷√≥…π¶", 0);
	WSACleanup();
	return 0;
}