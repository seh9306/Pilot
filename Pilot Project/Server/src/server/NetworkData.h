#ifndef NETWORK_DATA_H
#define NETWORK_DATA_H

#define BUFSIZE 1024

#include <WinSock2.h>

typedef struct {
	SOCKET hClntSock; // Handle Client Socket
	SOCKADDR_IN clntAddr; // Client Address
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct {
	OVERLAPPED overlapped; // handle Event �̺�Ʈ ó�� �� �� �ִ� ����ü?
	char buffer[BUFSIZE];
	WSABUF wsaBuf; // buffer length, buffer
} PER_IO_DATA, *LPPER_IO_DATA;


#endif