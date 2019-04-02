#ifndef SERVER_H
#define SERVER_H

#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <vector>

#include "NetworkData.h"

enum InitErrors {
	kBindError = 0,
	kListenError = 1
};

class PacketProcessor;

class Server 
{
public:
	Server();
	bool Init();
	bool Start();
private:
	int port;
	WSADATA wsaData; // Window Socket API Data
	HANDLE hCompletionPort; // Handle of Completion Port
	SYSTEM_INFO systemInfo; // For number of cpu Proc
	SOCKADDR_IN servAddr; // Server Address
	LPPER_IO_DATA perIoData; 
	LPPER_HANDLE_DATA perHandleData; // Socket Handle data

	// Handle Server Socket
	SOCKET hServSock; 
	DWORD recvBytes;
	DWORD flags;

	// Create Processor vector
	std::vector<PacketProcessor *> packetProcessors;
};

#endif