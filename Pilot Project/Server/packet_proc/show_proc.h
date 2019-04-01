#ifndef SHOWPROCESS_H
#define SHOWPROCESS_H

#include "packet_proc.h"

class ShowProc :
	public PacketProc
{
public:
	ShowProc();
	~ShowProc();
	virtual void packetProc(char *msg);
};

#endif

