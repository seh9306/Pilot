#ifndef SHOWADDPROCESSOR_H
#define SHOWADDPROCESSOR_H

#include "PacketProcessor.h"

class CFileAgentView;

class ShowAddProcessor :
	public PacketProcessor
{
public:
	ShowAddProcessor();
	virtual ~ShowAddProcessor();
	virtual void PacketProcess(SOCKET sock, char *msg);
private:
	WSABUF dataBuf;
};

#endif

