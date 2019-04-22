#include "UnSubscribeProcessor.h"

UnSubscribeProcessor::UnSubscribeProcessor()
{
}

UnSubscribeProcessor::~UnSubscribeProcessor()
{
}

void UnSubscribeProcessor::ProcessPacket(SOCKET sock, char *msg)
{
	subscribeManager->UnSubscribe(msg + SUB_HEADER_SIZE, sock);
}
