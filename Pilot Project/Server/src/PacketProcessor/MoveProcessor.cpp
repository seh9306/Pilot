#include "MoveProcessor.h"

extern void error_handle(char *msg);

MoveProcessor::MoveProcessor()
{
}

MoveProcessor::~MoveProcessor()
{
}

void MoveProcessor::ProcessPacket(SOCKET sock, char * msg)
{
	uint32_t dirLength		= 0;
	uint32_t fileNameLength	= 0;
	uint32_t goalLength		= 0;
	
	int offset = 0 + kProtocolTypeSize;

	ReadLengthWithAddingLengthAndSize(&dirLength, msg, sizeof(dirLength), offset);
	ReadLengthWithAddingLengthAndSize(&fileNameLength, msg, sizeof(fileNameLength), offset);
	ReadLength(&goalLength, msg, sizeof(goalLength), offset);

	if (!msg || dirLength + fileNameLength + goalLength>
		kMoveHeaderSize + MAX_PATH + kNullValueSize * 3) // MAX_PATH 260..
	{
		return;
	}

	// File move
	offset = 0 + kProtocolTypeSize + sizeof(int);

	char* pDir = msg + offset;
	offset += dirLength + sizeof(int);

	char* pFileName = msg + offset;
	offset += fileNameLength + sizeof(int);

	char* pNewFileDir = msg + offset;
	offset += goalLength;

	if (!fileManager->Move(pDir, pFileName, pNewFileDir))
	{
		error_handle("file rename fail");
		return;
	}

	// Publish
	std::list<SOCKET>* sockets = subscribeManager->GetSocketsByDir(pDir);

	offset += sizeof(WIN32_FIND_DATA);

	if (sockets != nullptr)
	{
		publishManager->Publish(msg, *sockets, kRenameHeaderSize + dirLength + fileNameLength);
	}

	sockets = subscribeManager->GetSocketsByDir(pNewFileDir);
	if(sockets != nullptr)
	{
		publishManager->Publish(msg, *sockets, offset);
	}
}