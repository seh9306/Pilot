#ifndef SERVER_H
#define SERVER_H

#pragma comment(lib, "ws2_32.lib")

#include <atomic>
#include <memory>
#include <vector>
#include <WinSock2.h>

#include "NetworkData.h"

enum InitErrors
{
	kSuccess = 0,
	kBindError = 1,
	kListenError = 2
};

class PacketProcessor;

class Server 
{
public:
	Server();
	~Server();
	int Init(int port = 9030);
	int Start();
	void AddProcessor(PacketProcessor* packetProcessor);

private:
	std::atomic_uint32_t numberOfClient_;

	// Handle of Completion Port
	HANDLE hCompletionPort_;

	// Handle Server Socket
	SOCKET serverSocket_; 

	// Create Processor vector
	std::vector<std::unique_ptr<PacketProcessor>> packetProcessors_;
};

#endif