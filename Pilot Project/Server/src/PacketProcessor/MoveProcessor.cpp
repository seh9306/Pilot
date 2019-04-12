#include "MoveProcessor.h"

extern void error_handle(char *msg);

MoveProcessor::MoveProcessor()
{
}

MoveProcessor::~MoveProcessor()
{
}

void MoveProcessor::PacketProcess(SOCKET sock, char * msg)
{
	int dirLength		= 0;
	int fileNameLength	= 0;
	int goalLength		= 0;
	
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
	byteOffset = 0 + PROTOCOL_TYPE_SIZE + sizeof(int);
	char* pDir = msg + byteOffset;

	byteOffset += dirLength + sizeof(int);
	char* pFileName = msg + byteOffset;

	byteOffset += fileNameLength + sizeof(int);
	char* pNewFileDir = msg + byteOffset;

	byteOffset += goalLength;

	if (!fileManager->Move(pDir, pFileName, pNewFileDir))
	{
		error_handle("file rename fail");
		return;
	}

	// Publish
	std::list<SOCKET>* sockets = subscribeManager->GetSocketsByDir(pDir);

	byteOffset += sizeof(WIN32_FIND_DATA);

	if (sockets != nullptr)
	{
		publishManager->Publish(msg, *sockets, RENAME_HEADER_SIZE + dirLength + fileNameLength);
	}

	sockets = subscribeManager->GetSocketsByDir(pNewFileDir);
	if(sockets != nullptr)
	{
		publishManager->Publish(msg, *sockets, byteOffset);
	}
}