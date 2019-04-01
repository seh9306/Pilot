#ifndef SUBPROC_H
#define SUBPROC_H

#include "packet_proc.h"

class SubProc :
	public PacketProc
{
public:
	SubProc();
	~SubProc();
	virtual void packetProc(char *msg);
};

#endif
