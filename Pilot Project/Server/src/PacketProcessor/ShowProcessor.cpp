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

	memcpy((void*)&length, msg + 1, 4);
	// @issue
	if (!msg || length > SUB_HEADER_SIZE + MAX_PATH + 1) // MAX_PATH 260..
	{
		return;
	}

	// publish to sub
	publishManager.Publish(msg, sock);
}

