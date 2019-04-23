#ifndef RECEIVER_H
#define RECEIVER_H

#include <WinSock2.h>
#include <vector>
#include <memory>
#include "PacketProcessor.h"

class Receiver {
public:
	int id;
	void operator()(HANDLE pComPort, std::vector<std::unique_ptr<PacketProcessor>>* packetProcessors);
	
};

#endif