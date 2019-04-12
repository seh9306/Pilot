 #include "stdafx.h"

#include "ShowProcessor.h"

#include <iostream>

ShowProcessor::ShowProcessor()
{
}


ShowProcessor::~ShowProcessor()
{
}

void ShowProcessor::PacketProcess(SOCKET sock, char *msg)
{
	int offset = 0;

	offset += PROTOCOL_TYPE_SIZE;

	int listSize = 0;
	memcpy(&listSize, msg + PROTOCOL_TYPE_SIZE, sizeof(int));

	offset += sizeof(int);

	cFileAgentView->SetListSize(listSize);
	 
}

