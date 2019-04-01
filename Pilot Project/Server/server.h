#ifndef SERVER_H
#define SERVER_H

#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <WinSock2.h>

#include "network_data.h"

class Server {
public:
	Server() {}
	Server(int port = 9001) : port(port) {}
	Server* init();
	void start();
private:
	int port;
	WSADATA wsaData; // Window Socket API Data
	HANDLE hCompletionPort; // Handle of Completion Port
	SYSTEM_INFO SystemInfo; // For number of cpu processor
	SOCKADDR_IN servAddr; // Server Address
	LPPER_IO_DATA PerIoData; // 
	LPPER_HANDLE_DATA PerHandleData; // Socket Handle data

	SOCKET hServSock; // Handle Server Socket
	DWORD RecvBytes, Flags;
};

#endif