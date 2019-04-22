#ifndef CONNECTPROCESSOR_H
#define CONNECTPROCESSOR_H

#include "PacketProcessor.h"

class ConnectProcessor :
	public PacketProcessor
{
public:
	ConnectProcessor();
	virtual ~ConnectProcessor();
	void ProcessPacket(SOCKET sock, char *msg); //1

private:
	const int driverStringLength = 4;
};

#endif
