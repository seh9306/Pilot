#include "RenameProcessor.h"
#include "Util\PublishManager.h"
#include "Util\FileManager.h"
#include "Util\SubscribeManager.h"

#include <iostream>

RenameProcessor::RenameProcessor()
{
}


RenameProcessor::~RenameProcessor()
{
}

void RenameProcessor::PacketProcess(SOCKET sock, char * msg)
{
	PublishManager& publishManager = PublishManager::GetInstance();

	int dirLength = 0;
	int oldFileLength = 0;
	int newFileLength = 0;

	int byteOffset = 0 + PROTOCOL_TYPE_SIZE;

	memcpy(&dirLength, msg + byteOffset, sizeof(int));
	byteOffset += sizeof(int) + dirLength;

	memcpy(&oldFileLength, msg + byteOffset, sizeof(int));
	byteOffset += sizeof(int) + oldFileLength;

	memcpy(&newFileLength, msg + byteOffset, sizeof(int));

	if (!msg || dirLength + oldFileLength + newFileLength > 
		RENAME_HEADER_SIZE + MAX_PATH * 3 + NULL_VALUE_SIZE * 3) // MAX_PATH 260..
	{
		return;
	}

	// File delete
	FileManager& fileManager = FileManager::GetInstance();

	byteOffset = 0 + PROTOCOL_TYPE_SIZE + sizeof(int);
	char* pDir = msg + byteOffset;

	byteOffset += dirLength + sizeof(int);
	char* pOldFile = msg + byteOffset;

	byteOffset += oldFileLength + sizeof(int);
	char* pNewFile = msg + byteOffset;

	if (!fileManager.Rename(pDir, pOldFile, pNewFile))
	{
		std::cout << "file rename fail" << std::endl;
		return;
	}

	// Publish
	SubscribeManager& subscribeManager = SubscribeManager::GetInstance();

	std::list<SOCKET>* sockets = subscribeManager.GetSocketsByDir(pDir);

	publishManager.Publish(msg, *sockets, RENAME_HEADER_SIZE + dirLength + oldFileLength + newFileLength);
}
