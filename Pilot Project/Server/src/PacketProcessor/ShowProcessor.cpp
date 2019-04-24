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

	int readOffset = 0 + kProtocolTypeSize + sizeof(DWORD);

	ReadLengthWithAddingSize(&dirLength, msg, sizeof(dirLength), readOffset);

	// @issue
	if (!msg || dirLength > kShowHeaderSize + MAX_PATH + kNullValueSize) // MAX_PATH 260..
	{
		return;
	}

	std::list<WIN32_FIND_DATA>* files = fileManager->GetFileList(msg + readOffset);

	uint32_t showNumber = -1;

	readOffset = kProtocolTypeSize;
	ReadMessage(&showNumber, msg, sizeof(showNumber), readOffset);

	int writeOffset = kProtocolTypeSize;

	if (files == nullptr)
	{
		uint32_t listSize = 0;

		WriteLengthWithAddingSize(msg, &listSize, sizeof(listSize), writeOffset);

		// kShow
		publishManager->Publish(msg, sock, writeOffset);

		return;
	}

	uint32_t listSize = files->size();

	WriteLengthWithAddingSize(msg, &listSize, sizeof(listSize), writeOffset);

	// kShow
	publishManager->Publish(msg, sock, writeOffset);

	msg[0] = kShowAdd;
	writeOffset = kProtocolTypeSize;

	WriteLengthWithAddingSize(msg, &showNumber, sizeof(showNumber), writeOffset);

	for (WIN32_FIND_DATA file : *files)
	{
		uint32_t fileNameSize = strlen(file.cFileName);

		uint32_t size = kWinFindDataFrontSize
			+ fileNameSize
			+ sizeof(fileNameSize)
			+ kNullValueSize;
		if (size + writeOffset > BUFSIZE - 2)
		{
			msg[writeOffset] = '\n';
			writeOffset++;

			publishManager->Publish(msg, sock, 1024);
			writeOffset = 0 + kProtocolTypeSize + sizeof(showNumber);
		}

		// copy file data front
		WriteMessageWithAddingSize(msg, &file, kWinFindDataFrontSize, writeOffset);

		// copy file name size
		WriteMessageWithAddingSize(msg, &fileNameSize, sizeof(fileNameSize), writeOffset);

		// copy file name
		WriteMessageWithAddingSize(msg, &file.cFileName, fileNameSize + kNullValueSize, writeOffset);
	}

	if (writeOffset != 0)
	{
		msg[writeOffset] = '\n';
		writeOffset++;

		publishManager->Publish(msg, sock, writeOffset);
	}

	delete files;
}

