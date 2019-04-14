#include "stdafx.h"
#include "ConnectProcessor.h"

#include <iostream>

ConnectProcessor::ConnectProcessor()
{
}


ConnectProcessor::~ConnectProcessor()
{
}

void ConnectProcessor::PacketProcess(SOCKET sock, char * msg)
{
	int byteOffset = PROTOCOL_TYPE_SIZE;

	int numberOfDrives = 0;
	memcpy(&(numberOfDrives), msg + byteOffset, sizeof(int));
	byteOffset += sizeof(int);

	for (int i = 0; i < numberOfDrives; i++)
	{
		wchar_t* pLogicalDriveStrings = new wchar_t[4];

		memcpy(pLogicalDriveStrings, msg + byteOffset, sizeof(wchar_t) * 4);
		byteOffset += sizeof(wchar_t) * 4;

		unsigned int type = -1;

		memcpy(&type, msg + byteOffset, sizeof(unsigned int));
		byteOffset += sizeof(unsigned int);

		cFileAgentView->AddDrives(pLogicalDriveStrings, type);
	}
	
}