#ifndef PACKETPROC_H
#define PACKETPROC_H

#define NUM_OF_PACKET_PROCESSOR 5

#define F_SUBSCRIBE		0
#define F_UNSUBSCRIBE	1
#define F_SHOW			2
#define F_CREATE		3
#define F_RENAME		4
#define F_DELETE		5
#define F_MOVE			6

#include <WinSock2.h>

class PacketProc
{
public:
	PacketProc();
	virtual ~PacketProc();
	
	virtual void packetProc(SOCKET sock, char *msg) = 0;
};

#endif