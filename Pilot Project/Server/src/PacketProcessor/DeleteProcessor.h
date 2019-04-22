#ifndef DELETEPROCESSOR_H
#define DELETEPROCESSOR_H

#include "PacketProcessor.h"

class DeleteProcessor :
	public PacketProcessor
{
public:
	DeleteProcessor();
	~DeleteProcessor();

	void ProcessPacket(SOCKET sock, char *msg);
};

#endif