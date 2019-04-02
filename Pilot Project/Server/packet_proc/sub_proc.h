#ifndef SUBPROC_H
#define SUBPROC_H

#define SUB_HEADER_SIZE 5

#include "packet_proc.h"

class SubProc :
	public PacketProc
{
public:
	SubProc();
	virtual ~SubProc();
	virtual void packetProc(SOCKET sock, char *msg);
};

#endif
