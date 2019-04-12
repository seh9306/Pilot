#include "SubscribeProcessor.h"

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
	int length = 0;

	memcpy(&length, msg + PROTOCOL_TYPE_SIZE, sizeof(int));

	if (!msg || length > SUB_HEADER_SIZE + MAX_PATH + NULL_VALUE_SIZE ) // MAX_PATH 260..
	{
		return;
	}

	// subscribe directory
	if (subscribeManager->Subscribe(msg + SUB_HEADER_SIZE, sock))
	{
		int dirLength = strlen(msg + SUB_HEADER_SIZE) + NULL_VALUE_SIZE;

		// copy protocol type
		msg[0] = kSubscribe;
		msg[1] = true;

		int len = 2;

		memcpy(msg + len + sizeof(int), msg + SUB_HEADER_SIZE, dirLength);
		len += dirLength;

		memcpy(msg + PROTOCOL_TYPE_SIZE + 1, &dirLength, sizeof(int));
		len += sizeof(int);

		int numberOfDrives = (int)fileManager->GetNumberOfDrives();
		memcpy(msg + len, &(numberOfDrives), sizeof(int));
		len += sizeof(int);

		wchar_t* pLogicalDriveStrings = fileManager->GetLogicalDriveStringsW();
		memcpy(msg + len, pLogicalDriveStrings, (4 * numberOfDrives) * 2);
		len += (4 * numberOfDrives) * 2;

		publishManager->Publish(msg, sock, len);
	}
	else
	{
		// copy protocol type
		msg[0] = kSubscribe;
		msg[1] = false;

		int len = 2;

		publishManager->Publish(msg, sock, len);
	}
}