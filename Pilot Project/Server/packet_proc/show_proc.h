#ifndef SHOWPROCESS_H
#define SHOWPROCESS_H

#include "packet_proc.h"

class ShowProc :
	public PacketProc
{
public:
	ShowProc();
	virtual ~ShowProc();
	virtual void packetProc(SOCKET sock, char *msg);
};

#endif

