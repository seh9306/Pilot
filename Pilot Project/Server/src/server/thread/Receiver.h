#ifndef RECEIVER_H
#define RECEIVER_H

#include <WinSock2.h>
#include <vector>
#include <memory>

class Server;

class Receiver {
public:
	int id;
	void operator()(Server* server,
		HANDLE completionPort,
		std::vector<std::unique_ptr<PacketProcessor>>* packetProcessors);

};

#endif