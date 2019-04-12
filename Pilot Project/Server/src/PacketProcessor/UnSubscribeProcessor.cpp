#include "UnSubscribeProcessor.h"

#include <iostream>

UnSubscribeProcessor::UnSubscribeProcessor()
{
}


UnSubscribeProcessor::~UnSubscribeProcessor()
{
}


void UnSubscribeProcessor::PacketProcess(SOCKET sock, char *msg)
{
	subscribeManager->UnSubscribe(msg + SUB_HEADER_SIZE, sock);
}
