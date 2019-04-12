#include "RenameProcessor.h"

extern void error_handle(char *msg);

RenameProcessor::RenameProcessor()
{
}


RenameProcessor::~RenameProcessor()
{
}

void RenameProcessor::PacketProcess(SOCKET sock, char * msg)
{
	int dirLength		= 0;
	int oldFileLength	= 0;
	int newFileLength	= 0;

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

	// File rename
	byteOffset = 0 + PROTOCOL_TYPE_SIZE + sizeof(int);
	char* pDir = msg + byteOffset;

	byteOffset += dirLength + sizeof(int);
	char* pOldFile = msg + byteOffset;

	byteOffset += oldFileLength + sizeof(int);
	char* pNewFile = msg + byteOffset;

	if (!fileManager->Rename(pDir, pOldFile, pNewFile))
	{
		error_handle("file rename fail");
		return;
	}

	// Publish
	std::list<SOCKET>* sockets = subscribeManager->GetSocketsByDir(pDir);

	if (sockets != nullptr)
	{
		publishManager->Publish(msg, *sockets, RENAME_HEADER_SIZE + dirLength + oldFileLength + newFileLength);
	}
}
