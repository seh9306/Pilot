#ifndef SUBSCRIEBEPROCESSOR_H
#define SUBSCRIEBEPROCESSOR_H

#include "PacketProcessor.h"

class SubscribeProcessor :
	public PacketProcessor
{
public:
	SubscribeProcessor();
	virtual ~SubscribeProcessor();
	virtual void PacketProcess(SOCKET sock, char *msg);
};

#endif
