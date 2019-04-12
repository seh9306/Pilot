#define _CRT_SECURE_NO_WARNINGS

#include "MoveProcessor.h"
#include "Util\PublishManager.h"
#include "Util\FileManager.h"
#include "Util\SubscribeManager.h"

#include <iostream>

MoveProcessor::MoveProcessor()
{
}

MoveProcessor::~MoveProcessor()
{
}

void MoveProcessor::PacketProcess(SOCKET sock, char * msg)
{
	PublishManager& publishManager = PublishManager::GetInstance();

	int dirLength = 0;
	int fileNameLength = 0;
	int goalLength = 0;

	int byteOffset = 0 + PROTOCOL_TYPE_SIZE;

	memcpy(&dirLength, msg + byteOffset, sizeof(int));
	byteOffset += sizeof(int) + dirLength;

	memcpy(&fileNameLength, msg + byteOffset, sizeof(int));
	byteOffset += sizeof(int) + fileNameLength;

	memcpy(&goalLength, msg + byteOffset, sizeof(int));

	if (!msg || dirLength + fileNameLength + goalLength>
		MOVE_HEADER_SIZE + MAX_PATH * 3 + NULL_VALUE_SIZE * 3) // MAX_PATH 260..
	{
		return;
	}

	// File move
	FileManager& fileManager = FileManager::GetInstance();

	byteOffset = 0 + PROTOCOL_TYPE_SIZE + sizeof(int);
	char* pDir = msg + byteOffset;

	byteOffset += dirLength + sizeof(int);
	char* pFileName = msg + byteOffset;

	byteOffset += fileNameLength + sizeof(int);
	char* pNewFileDir = msg + byteOffset;

	byteOffset += goalLength;

	if (!fileManager.Move(pDir, pFileName, pNewFileDir))
	{
		std::cout << "file rename fail" << std::endl;
		return;
	}

	// Publish
	SubscribeManager& subscribeManager = SubscribeManager::GetInstance();

	std::list<SOCKET>* sockets = subscribeManager.GetSocketsByDir(pDir);

	byteOffset += sizeof(WIN32_FIND_DATA);

	if (sockets != nullptr)
	{
		publishManager.Publish(msg, *sockets, RENAME_HEADER_SIZE + dirLength + fileNameLength);
	}

	sockets = subscribeManager.GetSocketsByDir(pNewFileDir);
	if(sockets != nullptr)
	{
		publishManager.Publish(msg, *sockets, byteOffset);
	}
}