#include "DeleteProcessor.h"

extern void error_handle(char *msg);

DeleteProcessor::DeleteProcessor()
{
}

DeleteProcessor::~DeleteProcessor()
{
}

void DeleteProcessor::ProcessPacket(SOCKET sock, char* msg)
{
	uint32_t dirLength = 0;
	uint32_t fileNameLength	= 0;

	int offset = PROTOCOL_TYPE_SIZE + FILE_TYPE_SIZE;

	if (msg[FILE_TYPE_INDEX] != FILE_TYPE_NORMAL 
		&& msg[FILE_TYPE_INDEX] != FILE_TYPE_DIRECTORY)
	{
		return;
	}

	char fileType = msg[FILE_TYPE_INDEX];

	ReadLengthWithAddingLengthAndSize(&dirLength, msg, sizeof(dirLength), offset);
	ReadLength(&fileNameLength, msg, sizeof(fileNameLength), offset);

	if (!msg || dirLength + fileNameLength > DELETE_HEADER_SIZE + MAX_PATH * 2 + NULL_VALUE_SIZE * 2) // MAX_PATH 260..
	{
		return;
	}

	// File delete
	offset = 0 + +PROTOCOL_TYPE_SIZE + FILE_TYPE_SIZE + sizeof(dirLength);

	char* pDir = msg + offset;
	offset += +dirLength + sizeof(fileNameLength);

	char* pFile = msg + offset;

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
			+ sizeof(dirLength)
			+ dirLength 
			+ sizeof(fileNameLength)
			+ fileNameLength
		);
	}
}
