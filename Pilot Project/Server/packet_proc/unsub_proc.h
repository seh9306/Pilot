#ifndef UNSUBPROC_H
#define UNSUBPROC_H

#include "packet_proc.h"

class UnsubProc :
	public PacketProc
{
public:
	UnsubProc();
	virtual ~UnsubProc();
	virtual void packetProc(SOCKET sock, char *msg);
};

#endif