#ifndef CREATEPROCESSOR_H
#define CREATEPROCESSOR_H

#include "PacketProcessor.h"
class CreateProcessor :
	public PacketProcessor
{
public:
	CreateProcessor();
	~CreateProcessor();

	void PacketProcess(SOCKET sock, char *msg);
};

#endif