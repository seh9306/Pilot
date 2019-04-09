#include "ShowProcessor.h"

#include "../util/PublishManager.h"

#include <iostream>

ShowProcessor::ShowProcessor()
{
}


ShowProcessor::~ShowProcessor()
{
}

void ShowProcessor::PacketProcess(SOCKET sock, char *msg)
{
	std::cout << "Show Processor" << std::endl;
	PublishManager& publishManager = PublishManager::GetInstance();

	int length = 0;

	memcpy(&length, msg + PROTOCOL_TYPE_SIZE + sizeof(DWORD), sizeof(int));
	// @issue
	if (!msg || length > SHOW_HEADER_SIZE + MAX_PATH + NULL_VALUE_SIZE) // MAX_PATH 260..
	{
		return;
	}

	// publish to sub
	publishManager.Publish(msg, sock);
}

