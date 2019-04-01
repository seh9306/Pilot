#ifndef UNSUBPROC_H
#define UNSUBPROC_H

#include "packet_proc.h"

class UnsubProc :
	public PacketProc
{
public:
	UnsubProc();
	~UnsubProc();
	virtual void packetProc(char *msg);
};

#endif