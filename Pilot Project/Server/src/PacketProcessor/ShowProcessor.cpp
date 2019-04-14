#include "ShowProcessor.h"

#include <iostream>

ShowProcessor::ShowProcessor()
{
}


ShowProcessor::~ShowProcessor()
{
}

void ShowProcessor::PacketProcess(SOCKET sock, char *msg)
{
	int length = 0;

	memcpy(&length, msg + PROTOCOL_TYPE_SIZE + sizeof(DWORD), sizeof(int));
	// @issue
	if (!msg || length > SHOW_HEADER_SIZE + MAX_PATH + NULL_VALUE_SIZE) // MAX_PATH 260..
	{
		return;
	}

	std::list<WIN32_FIND_DATA>* files = fileManager->GetFileList(msg + SUB_HEADER_SIZE + sizeof(DWORD));

	if (files == nullptr)
	{
		// send fail
		return;
	}
	
	int listSize = files->size();
	int messageSize = PROTOCOL_TYPE_SIZE;

	DWORD showNumber = -1;

	memcpy(&showNumber, msg + PROTOCOL_TYPE_SIZE, sizeof(DWORD));
	memcpy(msg + PROTOCOL_TYPE_SIZE, &listSize, sizeof(int));

	messageSize += sizeof(int);
	// kShow
	publishManager->Publish(msg, sock, messageSize);

	msg[0] = kShowAdd;
	messageSize = PROTOCOL_TYPE_SIZE;

	memcpy(msg + messageSize, &showNumber, sizeof(DWORD));
	messageSize += sizeof(DWORD);

	for (WIN32_FIND_DATA file : *files)
	{
		int fileNameSize = strlen(file.cFileName);

		int size = WIN_FIND_DATA_FRONT_SIZE
			+ fileNameSize
			+ sizeof(fileNameSize)
			+ NULL_VALUE_SIZE;
		if (size + messageSize > BUFSIZE - 2)
		{
			msg[messageSize] = '\n';
			messageSize++;

			publishManager->Publish(msg, sock, 1024);
			messageSize = 0 + PROTOCOL_TYPE_SIZE + sizeof(DWORD);
		}

		// copy file data front
		memcpy(msg + messageSize, &file, WIN_FIND_DATA_FRONT_SIZE);
		messageSize += WIN_FIND_DATA_FRONT_SIZE;

		// copy file name size
		memcpy(msg + messageSize, &fileNameSize, sizeof(int));
		messageSize += sizeof(int);

		// copy file name
		memcpy(msg + messageSize, file.cFileName, fileNameSize + NULL_VALUE_SIZE);
		messageSize += fileNameSize + NULL_VALUE_SIZE;

	}

	if (messageSize != 0)
	{
		msg[messageSize] = '\n';
		messageSize++;

		publishManager->Publish(msg, sock, messageSize);
		messageSize = 0 + PROTOCOL_TYPE_SIZE + sizeof(DWORD);
	}

	// publish to sub
	//publishManager->Publish(msg, sock);
}

