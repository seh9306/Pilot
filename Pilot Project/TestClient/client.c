#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

void ErrorHandling(char *message);

#pragma comment(lib, "ws2_32.lib")
typedef struct _WIN32_FIND {
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
}TEST;
int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	SOCKET hSocket = WSASocketW(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	SOCKADDR_IN recvAddr;
	memset(&recvAddr, 0, sizeof(recvAddr));
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	recvAddr.sin_port = htons(9030);
	printf("%d\n\n\n", sizeof(TEST));
	if (connect(hSocket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR) {
		int error = WSAGetLastError();
		printf("%d\n", error);
		ErrorHandling("connect() error!");
	}
		

	WSAEVENT event = WSACreateEvent();

	WSAOVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));

	overlapped.hEvent = event;

	WSABUF dataBuf;
	int protocol;
	char message[1024] = { 0, };
	char message2[1024] = { 0, };
	int sendBytes = 0;
	int recvBytes = 0;
	int totalRecvBytes = 0;
	int flags = 0;
	
	char *temp = (char *)malloc(sizeof(char) * 1024);
	while (TRUE)
	{
		flags = 0;
		printf("전송할데이터(종료를원할시exit)\n");
		//scanf("%d", &protocol);
		scanf("%s", message);

		if (!strcmp(message, "exit")) break;

		dataBuf.buf = message2;
		//dataBuf.buf[0] = (char)protocol;
		dataBuf.len = strlen(message);
		memcpy(dataBuf.buf + 1, &(dataBuf.len), sizeof(int));
		dataBuf.len += 5;
		dataBuf.buf[0] = 0;
		dataBuf.buf[(dataBuf.len)++] = NULL;
		memcpy(dataBuf.buf + 5, message, strlen(message));
		printf("%s\n\n", dataBuf.buf);

		

		if (WSASend(hSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				ErrorHandling("WSASend() error");
		}

		WSAWaitForMultipleEvents(1, &event, TRUE, WSA_INFINITE, FALSE);

		WSAGetOverlappedResult(hSocket, &overlapped, (LPDWORD)&sendBytes, FALSE, NULL);

		printf("전송된바이트수: %d \n", sendBytes);

		dataBuf.len = 1024;
		dataBuf.buf = temp;

		if (WSARecv(hSocket, &dataBuf, 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, NULL, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				ErrorHandling("WSASend() error");
		}
		totalRecvBytes += recvBytes;
		int listSize;
		memcpy(&listSize, dataBuf.buf + 1, sizeof(int));
		int offset = 5;
		printf("리스트 갯수 %d\n", listSize);
		
		printf("0:%d\n", dataBuf.buf[0]);
		printf("1:%d\n", dataBuf.buf[1]);
		printf("2:%d\n", dataBuf.buf[2]);
		printf("3:%d\n", dataBuf.buf[3]);
		printf("4:%d\n", dataBuf.buf[4]);
		printf("5:%d\n", dataBuf.buf[5]);
		printf("6:%d\n", dataBuf.buf[6]);
		printf("7:%d\n", dataBuf.buf[7]);

		printf("길이 : %d \n", recvBytes);
	
		int j = 0;
 		for (int i = 0; i < listSize; i++) {
			WIN32_FIND_DATA file;
			int length = 0;

			if (dataBuf.buf[offset] == (char)'\n')
				//offset + *((int*)(dataBuf.buf + 36)) + 36 + 4 > 1024 || 
			{
				/*if (dataBuf.buf[offset] == (char)'\n')
				{
					printf("NUUUUUUUUUUUUUUULLLLLLLLLLLLLL!\n");
				}
				else 
				{
					printf("%d // %d\n", *((int*)(dataBuf.buf + 36)), offset + *((int*)(dataBuf.buf + 36)) + 36 + 4);
				}*/
				printf("%d 번째\n",i);
				
				j = 0;
				if (WSARecv(hSocket, &dataBuf, 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, NULL, NULL) == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSA_IO_PENDING)
					{
						ErrorHandling("WSASend() error");
					}
				}
				else
				{
					totalRecvBytes += recvBytes;
					offset = 0;
					//printf("%d:: %d %d\n", *((int *)(dataBuf.buf + 36)),dataBuf.len, recvBytes);
				}
			}

			memcpy(&file, dataBuf.buf + offset, 36);
			offset += 36;

			memcpy(&length, dataBuf.buf + offset, 4);
			offset += 4;

		
			//printf("file length : %d , %s\n", length, dataBuf.buf + offset);
			offset += length+1;
			j++;
			//printf("i = %d , %d\n\n", i, offset);

		}
		printf("total recv : %d\n", totalRecvBytes);

	/*	if (WSARecv(hSocket, &dataBuf, 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, NULL, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				ErrorHandling("WSASend() error");
		}*/

		//exit(0);
		WIN32_FIND_DATA *p = (WIN32_FIND_DATA *)dataBuf.buf;
		//printf("Recv[%s] %d \n", dataBuf.buf+36, recvBytes);
	}

	closesocket(hSocket);

	WSACleanup();

	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);

	exit(1);
}
