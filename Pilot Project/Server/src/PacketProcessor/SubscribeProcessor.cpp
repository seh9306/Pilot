#include "SubscribeProcessor.h"

SubscribeProcessor::SubscribeProcessor()
{
}

SubscribeProcessor::~SubscribeProcessor()
{
}

// subcribe
void SubscribeProcessor::ProcessPacket(SOCKET sock, char *msg)
{
	int length = 0;

	memcpy(&length, msg + PROTOCOL_TYPE_SIZE, sizeof(int));

	if (!msg || length > SUB_HEADER_SIZE + MAX_PATH + NULL_VALUE_SIZE ) // MAX_PATH 260..
	{
		return;
	}

	// subscribe directory
	if (subscribeManager->Subscribe(msg + SUB_HEADER_SIZE, sock))
	{
		size_t dirLength = strlen(msg + SUB_HEADER_SIZE) + NULL_VALUE_SIZE;

		// copy protocol type
		msg[kProtocolHeaderIndex] = kSubscribe;
		msg[kSuccessSubscribeIndex] = true;

		int len = 0 + kSuccessSubscribeHeaderSize;

		memcpy(msg + len + sizeof(int), msg + SUB_HEADER_SIZE, dirLength);
		len += dirLength;

		memcpy(msg + PROTOCOL_TYPE_SIZE + kSuccessSubscribeIndexSize, &dirLength, sizeof(int));
		len += sizeof(int);

		publishManager->Publish(msg, sock, len);
	}
	else
	{
		// copy protocol type
		msg[kProtocolHeaderIndex] = kSubscribe;
		msg[kSuccessSubscribeIndex] = false;

		int len = 2;

		publishManager->Publish(msg, sock, len);
	}
}