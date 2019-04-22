#ifndef SHOWPROCESSOR_H
#define SHOWPROCESSOR_H

#include "PacketProcessor.h"

class ShowProcessor :
	public PacketProcessor
{
public:
	ShowProcessor();
	virtual ~ShowProcessor();
	virtual void ProcessPacket(SOCKET sock, char *msg);
};

#endif

