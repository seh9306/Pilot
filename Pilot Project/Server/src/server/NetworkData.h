#ifndef NETWORK_DATA_H
#define NETWORK_DATA_H

#define BUFSIZE 1024
#define IOCP_ASYNC_RECV 1
#define IOCP_ASYNC_SEND 2

#include <WinSock2.h>

struct SocketDataPerClient
{
	SocketDataPerClient() {}
	SocketDataPerClient(SOCKET clientSocket, SOCKADDR_IN clientAddress)
		: clientSocket(clientSocket), clientAddress(clientAddress) {}
	SOCKET clientSocket;
	SOCKADDR_IN clientAddress; 
};

struct AsyncIOBuffer
{
	AsyncIOBuffer() {}
	AsyncIOBuffer(int type) : type(type)
	{
		memset(&(overlapped), 0, sizeof(OVERLAPPED));
		wsaBuf.buf = buffer;
		wsaBuf.len = BUFSIZE;
	}
	OVERLAPPED overlapped;
	char buffer[BUFSIZE];
	WSABUF wsaBuf;
	int type;
};

#endif