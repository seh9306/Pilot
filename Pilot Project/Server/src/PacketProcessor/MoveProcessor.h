#ifndef MOVEPROCESSOR_H
#define MOVEPROCESSOR_H

#include "PacketProcessor.h"

class MoveProcessor :
	public PacketProcessor
{
public:
	MoveProcessor();
	~MoveProcessor();

	void PacketProcess(SOCKET sock, char *msg);
};

#endif