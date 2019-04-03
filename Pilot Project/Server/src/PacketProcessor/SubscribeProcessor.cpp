#include "SubscribeProcessor.h"
#include "../util/SubscribeManager.h"
#include "../util/PublishManager.h"

#include <iostream>

SubscribeProcessor::SubscribeProcessor()
{
}


SubscribeProcessor::~SubscribeProcessor()
{
}

// subcribe
void SubscribeProcessor::PacketProcess(SOCKET sock, char *msg)
{
	SubscribeManager& subscribeManager = SubscribeManager::GetInstance();
	PublishManager& publishManager = PublishManager::GetInstance();

	int length = 0;

	memcpy((void*)&length, msg + 1, 4);

	if (!msg || length > SUB_HEADER_SIZE + MAX_PATH + 1 ) // MAX_PATH 260..
	{
		return;
	}

	// subscribe directory
	subscribeManager.Subscribe(msg + SUB_HEADER_SIZE, sock);

	// publish to sub
	publishManager.Publish(msg + SUB_HEADER_SIZE, sock);
	std::cout << "¾ßÈ£" << std::endl;
}