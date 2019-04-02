#ifndef SHOWPROCESSOR_H
#define SHOWPROCESSOR_H

#include "PacketProcessor.h"

class ShowProcessor :
	public PacketProcessor
{
public:
	ShowProcessor();
	virtual ~ShowProcessor();
	virtual void PacketProcess(SOCKET sock, char *msg);
};

#endif

