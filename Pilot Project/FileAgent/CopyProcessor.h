#ifndef COPYPROCESSOR_H
#define COPYPROCESSOR_H

#include "PacketProcessor.h"
class CopyProcessor :
	public PacketProcessor
{
public:
	CopyProcessor();
	~CopyProcessor();

	void PacketProcess(SOCKET sock, char *msg);
};

#endif