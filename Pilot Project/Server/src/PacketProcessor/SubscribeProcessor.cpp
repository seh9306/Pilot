#include "SubscribeProcessor.h"
#include "../util/SubscribeManager.h"
#include "../util/PublishManager.h"

#define SP_DEBUG

#ifdef SP_DEBUG
#include <iostream>
#endif

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

	memcpy(&length, msg + PROTOCOL_TYPE_SIZE, sizeof(int));

	if (!msg || length > SUB_HEADER_SIZE + MAX_PATH + NULL_VALUE_SIZE ) // MAX_PATH 260..
	{
		return;
	}

	// subscribe directory
	if (subscribeManager.Subscribe(msg + SUB_HEADER_SIZE, sock)) 
	{
#ifdef SP_DEBUG
		std::cout << "Subscribe success :: " << msg + SUB_HEADER_SIZE <<std::endl;
#endif
		publishManager.sSubscribe(msg, sock);
	}
	else
	{
		publishManager.fSubscribe(msg + SUB_HEADER_SIZE, sock);
	}
}