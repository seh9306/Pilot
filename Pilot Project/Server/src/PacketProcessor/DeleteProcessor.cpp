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

	int offset = kProtocolTypeSize + kFileTypeSize;

	if (msg[kFileTypeIndex] != kFileTypeNormal 
		&& msg[kFileTypeIndex] != kFileTypeDirectory)
	{
		return;
	}

	char fileType = msg[kFileTypeIndex];

	ReadLengthWithAddingLengthAndSize(&dirLength, msg, sizeof(dirLength), offset);
	ReadLength(&fileNameLength, msg, sizeof(fileNameLength), offset);

	if (!msg || dirLength + fileNameLength > kDeleteHeaderSize + MAX_PATH * 2 + kNullValueSize * 2) // MAX_PATH 260..
	{
		return;
	}

	// File delete
	offset = 0 + kProtocolTypeSize + kFileTypeSize + sizeof(dirLength);

	char* pDir = msg + offset;
	offset += dirLength + sizeof(fileNameLength);

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
		publishManager->Publish(msg, *sockets, kProtocolTypeSize 
			+ kFileTypeSize 
			+ sizeof(dirLength)
			+ dirLength 
			+ sizeof(fileNameLength)
			+ fileNameLength
		);
	}
}
