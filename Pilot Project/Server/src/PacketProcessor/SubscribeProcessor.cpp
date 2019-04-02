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
	SubscribeManager* subscribeManager = SubscribeManager::getInstance();
	PublishManager* publishManager = PublishManager::getInstance();

	int length = 0;

	memcpy((void*)&length, msg + 1, 4);

	if (!msg || length > 261 + 4 + 1) // MAX_PATH 260..
	{
		return;
	}

	// subscribe directory
	subscribeManager->subscribe(msg + SUB_HEADER_SIZE, sock);

	// publish to sub
	publishManager->publish(msg + SUB_HEADER_SIZE, sock);

}