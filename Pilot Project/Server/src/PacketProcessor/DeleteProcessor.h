#ifndef DELETEPROCESSOR_H
#define DELETEPROCESSOR_H

#include "PacketProcessor.h"
class DeleteProcessor :
	public PacketProcessor
{
public:
	DeleteProcessor();
	~DeleteProcessor();

	void PacketProcess(SOCKET sock, char *msg);
};

#endif