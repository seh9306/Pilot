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

	memcpy(&length, msg + kProtocolTypeSize, sizeof(int));

	if (!msg || length > kSubHeaderSize + MAX_PATH + kNullValueSize ) // MAX_PATH 260..
	{
		return;
	}

	// subscribe directory
	if (subscribeManager->Subscribe(msg + kSubHeaderSize, sock))
	{
		size_t dirLength = strlen(msg + kSubHeaderSize) + kNullValueSize;

		// copy protocol type
		msg[kProtocolHeaderIndex] = kSubscribe;
		msg[kSuccessSubscribeIndex] = true;

		int len = 0 + kSuccessSubscribeHeaderSize;

		memcpy(msg + len + sizeof(int), msg + kSubHeaderSize, dirLength);
		len += dirLength;

		memcpy(msg + kProtocolTypeSize + kSuccessSubscribeIndexSize, &dirLength, sizeof(int));
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