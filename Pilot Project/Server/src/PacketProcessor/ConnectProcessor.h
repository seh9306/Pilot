#ifndef CONNECTPROCESSOR_H
#define CONNECTPROCESSOR_H

#include "PacketProcessor.h"

class ConnectProcessor :
	public PacketProcessor
{
public:
	ConnectProcessor();
	virtual ~ConnectProcessor();
	void PacketProcess(SOCKET sock, char *msg); //1
};

#endif
