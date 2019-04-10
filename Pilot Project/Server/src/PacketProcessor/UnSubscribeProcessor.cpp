#include "UnSubscribeProcessor.h"

#include "../util/SubscribeManager.h"

#include <iostream>

UnSubscribeProcessor::UnSubscribeProcessor()
{
}


UnSubscribeProcessor::~UnSubscribeProcessor()
{
}


void UnSubscribeProcessor::PacketProcess(SOCKET sock, char *msg)
{
	std::cout << "Unsub Processor" << std::endl;
	SubscribeManager& subscribeManager = SubscribeManager::GetInstance();

	subscribeManager.UnSubscribe(msg + SUB_HEADER_SIZE, sock);
}
