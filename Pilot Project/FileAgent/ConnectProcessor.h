#pragma once
#include "PacketProcessor.h"

class ConnectProcessor :
	public PacketProcessor
{
public:
	ConnectProcessor();
	virtual ~ConnectProcessor();
	virtual void PacketProcess(SOCKET sock, char* msg);
};

