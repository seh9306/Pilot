#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <thread>

void ErrorHandling(char *message);

#pragma comment(lib, "ws2_32.lib")

//스레드에 의해 호출되는 함수
void call_from_thread(int tid) {
	std::cout << "스레드 실행 " << tid << std::endl;
	SOCKET hSocket = WSASocketW(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	SOCKADDR_IN recvAddr;
	memset(&recvAddr, 0, sizeof(recvAddr));
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	recvAddr.sin_port = htons(9030);

	if (connect(hSocket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR) {
		int error = WSAGetLastError();
		printf("error:%d\n", error);
		ErrorHandling("connect() error!");
	}
	char buf[1024];
	_WSABUF dataBuf;
	dataBuf.buf = buf;
	dataBuf.len = 1024;

	dataBuf.buf[0] = 0;
	dataBuf.len = 0 + 1;

	int length = strlen("C:\\");

	memcpy(dataBuf.buf + dataBuf.len, &length, sizeof(int));
	dataBuf.len += sizeof(int);

	memcpy(dataBuf.buf + dataBuf.len, "C:\\", length + 1);
	dataBuf.len += length + 1;
	DWORD sendBytes = 0;
	if (WSASend(hSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
	{
		ErrorHandling("send() error!");
	}
	while (1) {
		Sleep(10000);
	}
}

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	std::thread t[1500];

	// 스레드 시작
	for (int i = 0; i < 1500; ++i) {
		t[i] = std::thread(call_from_thread, i);
		//Sleep(1);
	}

	std::cout << "메인 함수 시작" << std::endl;

	//스레드가 종료될 때 까지 대기
	for (int i = 0; i < 700; ++i) {
		t[i].join();
	}

	while (1) {
		Sleep(10000);
	}
	//closesocket(hSocket);

	//WSACleanup();

	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);

	exit(1);
}
