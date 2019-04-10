#include "DeleteProcessor.h"
#include "Util\PublishManager.h"
#include "Util\FileManager.h"
#include "Util\SubscribeManager.h"

#include <iostream>

DeleteProcessor::DeleteProcessor()
{
}


DeleteProcessor::~DeleteProcessor()
{
}

void DeleteProcessor::PacketProcess(SOCKET sock, char* msg)
{
	PublishManager& publishManager = PublishManager::GetInstance();

	int dirLength = 0;
	int fileNameLength = 0;

	int byteOffset = PROTOCOL_TYPE_SIZE + FILE_TYPE_SIZE;

	if (msg[1] != 0 && msg[1] != 1)
	{
		return;
	}
	char fileType = msg[1];

	memcpy(&dirLength, msg + byteOffset, sizeof(int));
	byteOffset += sizeof(int);

	memcpy(&fileNameLength, msg + byteOffset + dirLength, sizeof(int));

	if (!msg || dirLength + fileNameLength > DELETE_HEADER_SIZE + MAX_PATH * 2 + NULL_VALUE_SIZE * 2) // MAX_PATH 260..
	{
		return;
	}

	// File delete
	FileManager& fileManager = FileManager::GetInstance();

	char* pDir = msg + PROTOCOL_TYPE_SIZE + FILE_TYPE_SIZE + sizeof(int);
	char* pFile = msg + PROTOCOL_TYPE_SIZE + FILE_TYPE_SIZE + sizeof(int) + dirLength + sizeof(int);

	if (!fileManager.DeleteFileByFileName(pDir, pFile, fileType))
	{
		std::cout << "file delete fail" << std::endl;
		return;
	}

	// Publish
	SubscribeManager& subscribeManager = SubscribeManager::GetInstance();

	std::list<SOCKET>* sockets = subscribeManager.GetSocketsByDir(pDir);

	std::cout << sockets->size() << "size size size " << std::endl; 

	publishManager.Publish(msg, *sockets, PROTOCOL_TYPE_SIZE + FILE_TYPE_SIZE + sizeof(int) + dirLength + sizeof(int) + fileNameLength);
}
