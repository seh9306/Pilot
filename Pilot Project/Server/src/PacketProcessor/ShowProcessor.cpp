#include "ShowProcessor.h"

#include <iostream>

#define BUFSIZE 1024

ShowProcessor::ShowProcessor()
{
}

ShowProcessor::~ShowProcessor()
{
}

void ShowProcessor::ProcessPacket(SOCKET sock, char *msg)
{
	uint32_t dirLength = 0;

	int readOffset = 0 + PROTOCOL_TYPE_SIZE + sizeof(DWORD);

	ReadLengthWithAddingSize(&dirLength, msg, sizeof(dirLength), readOffset);

	// @issue
	if (!msg || dirLength > SHOW_HEADER_SIZE + MAX_PATH + NULL_VALUE_SIZE) // MAX_PATH 260..
	{
		return;
	}

	std::list<WIN32_FIND_DATA>* files = fileManager->GetFileList(msg + readOffset);

	if (files == nullptr)
	{
		// send fail
		return;
	}
	
	uint32_t showNumber = -1;

	readOffset = PROTOCOL_TYPE_SIZE;
	ReadMessage(&showNumber, msg, sizeof(showNumber), readOffset);

	uint32_t listSize = files->size();
	int writeOffset = PROTOCOL_TYPE_SIZE;

	WriteLengthWithAddingSize(msg, &listSize, sizeof(listSize), writeOffset);

	// kShow
	publishManager->Publish(msg, sock, writeOffset);

	msg[0] = kShowAdd;
	writeOffset = PROTOCOL_TYPE_SIZE;

	WriteLengthWithAddingSize(msg, &showNumber, sizeof(showNumber), writeOffset);

	for (WIN32_FIND_DATA file : *files)
	{
		uint32_t fileNameSize = strlen(file.cFileName);

		uint32_t size = WIN_FIND_DATA_FRONT_SIZE
			+ fileNameSize
			+ sizeof(fileNameSize)
			+ NULL_VALUE_SIZE;
		if (size + writeOffset > BUFSIZE - 2)
		{
			msg[writeOffset] = '\n';
			writeOffset++;

			publishManager->Publish(msg, sock, 1024);
			writeOffset = 0 + PROTOCOL_TYPE_SIZE + sizeof(showNumber);
		}

		// copy file data front
		WriteMessageWithAddingSize(msg, &file, WIN_FIND_DATA_FRONT_SIZE, writeOffset);

		// copy file name size
		WriteMessageWithAddingSize(msg, &fileNameSize, sizeof(fileNameSize), writeOffset);

		// copy file name
		WriteMessageWithAddingSize(msg, &file.cFileName, fileNameSize + NULL_VALUE_SIZE, writeOffset);
	}

	if (writeOffset != 0)
	{
		msg[writeOffset] = '\n';
		writeOffset++;

		publishManager->Publish(msg, sock, writeOffset);
	}
}

