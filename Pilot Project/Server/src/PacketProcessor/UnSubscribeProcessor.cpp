#include "UnSubscribeProcessor.h"

UnSubscribeProcessor::UnSubscribeProcessor()
{
}

UnSubscribeProcessor::~UnSubscribeProcessor()
{
}

void UnSubscribeProcessor::ProcessPacket(SOCKET sock, char *msg)
{
	subscribeManager->UnSubscribe(msg + kSubHeaderSize, sock);
}
