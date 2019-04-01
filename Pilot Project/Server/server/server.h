#ifndef SERVER_H
#define SERVER_H

#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include <vector>

#include "..\network_data.h"

#include "..\packet_proc\packet_proc.h"

using std::vector;

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
	SYSTEM_INFO SystemInfo; // For number of cpu Proc
	SOCKADDR_IN servAddr; // Server Address
	LPPER_IO_DATA PerIoData; // 
	LPPER_HANDLE_DATA PerHandleData; // Socket Handle data

	SOCKET hServSock; // Handle Server Socket
	DWORD RecvBytes, Flags;

	vector<PacketProc *> *packetProcs;
};

#endif