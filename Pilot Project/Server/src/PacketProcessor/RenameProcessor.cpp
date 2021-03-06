#include "RenameProcessor.h"

extern void error_handle(char *msg);

RenameProcessor::RenameProcessor()
{
}


RenameProcessor::~RenameProcessor()
{
}

void RenameProcessor::ProcessPacket(SOCKET sock, char * msg)
{
	uint32_t dirLength		= 0;
	uint32_t oldFileLength	= 0;
	uint32_t newFileLength	= 0;

	int offset = 0 + kProtocolTypeSize;

	ReadLengthWithAddingLengthAndSize(&dirLength, msg, sizeof(dirLength), offset);
	ReadLengthWithAddingLengthAndSize(&oldFileLength, msg, sizeof(dirLength), offset);
	ReadLength(&newFileLength, msg, sizeof(dirLength), offset);

	if (!msg || dirLength + oldFileLength + newFileLength > 
		kRenameHeaderSize + MAX_PATH * 3 + kNullValueSize * 3) // MAX_PATH 260..
	{
		return;
	}

	// File rename
	offset = 0 + kProtocolTypeSize + sizeof(int);

	char* pDir = msg + offset;
	offset += dirLength + sizeof(int);

	char* pOldFile = msg + offset;
	offset += oldFileLength + sizeof(int);

	char* pNewFile = msg + offset;

	if (!fileManager->Rename(pDir, pOldFile, pNewFile))
	{
		error_handle("file rename fail");
		return;
	}

	// Publish
	std::list<SOCKET>* sockets = subscribeManager->GetSocketsByDir(pDir);

	if (sockets != nullptr)
	{
		publishManager->Publish(msg, *sockets, kRenameHeaderSize + dirLength + oldFileLength + newFileLength);
	}
}
