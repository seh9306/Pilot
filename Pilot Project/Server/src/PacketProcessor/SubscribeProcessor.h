#ifndef SUBSCRIEBEPROCESSOR_H
#define SUBSCRIEBEPROCESSOR_H

#include "PacketProcessor.h"

const int kSuccessSubscribeIndex = 1;
const int kSuccessSubscribeIndexSize = 1;

class SubscribeProcessor :
	public PacketProcessor
{
public:
	SubscribeProcessor();
	virtual ~SubscribeProcessor();
	virtual void ProcessPacket(SOCKET sock, char *msg);
};

#endif
