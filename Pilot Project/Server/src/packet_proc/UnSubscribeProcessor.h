#ifndef UNSUBSCRIBEPROCESSOR_H
#define UNSUBSCRIBEPROCESSOR_H

#include "PacketProcessor.h"

class UnSubscribeProcessor :
	public PacketProcessor
{
public:
	UnSubscribeProcessor();
	virtual ~UnSubscribeProcessor();
	virtual void PacketProcess(SOCKET sock, char *msg);
};

#endif