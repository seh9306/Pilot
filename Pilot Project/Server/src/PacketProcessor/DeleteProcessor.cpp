#include "DeleteProcessor.h"

extern void error_handle(char *msg);

DeleteProcessor::DeleteProcessor()
{
}

DeleteProcessor::~DeleteProcessor()
{
}

void DeleteProcessor::PacketProcess(SOCKET sock, char* msg)
{
	int dirLength		= 0;
	int fileNameLength	= 0;

	int byteOffset = PROTOCOL_TYPE_SIZE + FILE_TYPE_SIZE;

	if (msg[FILE_TYPE_INDEX] != FILE_TYPE_NORMAL 
		&& msg[FILE_TYPE_INDEX] != FILE_TYPE_DIRECTORY)
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
	char* pDir	= msg + PROTOCOL_TYPE_SIZE + FILE_TYPE_SIZE + sizeof(int);
	char* pFile = msg + PROTOCOL_TYPE_SIZE + FILE_TYPE_SIZE + sizeof(int) + dirLength + sizeof(int);

	if (!fileManager->DeleteFileByFileName(pDir, pFile, fileType))
	{
		error_handle("file delete fail");
		return;
	}

	// Publish
	std::list<SOCKET>* sockets = subscribeManager->GetSocketsByDir(pDir);

	if (sockets != nullptr)
	{
		publishManager->Publish(msg, *sockets, PROTOCOL_TYPE_SIZE 
			+ FILE_TYPE_SIZE 
			+ sizeof(int) 
			+ dirLength 
			+ sizeof(int) 
			+ fileNameLength
		);
	}
}
