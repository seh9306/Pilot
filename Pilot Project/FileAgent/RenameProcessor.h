#ifndef RENAMEPROCESSOR_H
#define RENAMEPROCESSOR_H

#include "PacketProcessor.h"

class RenameProcessor :
	public PacketProcessor
{
public:
	RenameProcessor();
	~RenameProcessor();

	void PacketProcess(SOCKET sock, char *msg);
};

#endif