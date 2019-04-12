#include "stdafx.h"

#include "MoveProcessor.h"

#include <iostream>

MoveProcessor::MoveProcessor()
{
}


MoveProcessor::~MoveProcessor()
{
}

void MoveProcessor::PacketProcess(SOCKET sock, char * msg)
{
	int dirLength = 0;
	int fileNameLength = 0;
	int goalLength = 0;

	int byteOffset = 0 + PROTOCOL_TYPE_SIZE;

	memcpy(&dirLength, msg + byteOffset, sizeof(int));
	byteOffset += sizeof(int) + dirLength;

	memcpy(&fileNameLength, msg + byteOffset, sizeof(int));
	byteOffset += sizeof(int) + fileNameLength;

	byteOffset = 0 + PROTOCOL_TYPE_SIZE + sizeof(int);
	char* pDir = msg + byteOffset;

	byteOffset += dirLength + sizeof(int);
	char* pFileName = msg + byteOffset;
	
	if (!strcmp(pDir, cFileAgentView->GetPCharDir()))
	{ // delete
		if (!msg || dirLength + fileNameLength >
			MOVE_HEADER_SIZE - sizeof(int) + MAX_PATH * 2 + NULL_VALUE_SIZE * 2) // MAX_PATH 260..
		{
			return;
		}

		cFileAgentView->DeleteItem(pFileName);
		cFileAgentView->SetListSize(cFileAgentView->GetListSize() - 1);
		cFileAgentView->SetItemCountEx();
	}
	else
	{ // create
		byteOffset += fileNameLength;

		memcpy(&goalLength, msg + byteOffset, sizeof(int));

		if (!msg || dirLength + fileNameLength + goalLength >
			MOVE_HEADER_SIZE + MAX_PATH * 3 + NULL_VALUE_SIZE * 3) // MAX_PATH 260..
		{
			return;
		}

		WIN32_FIND_DATA file;
		byteOffset += sizeof(int);
		// 현재 디렉터리와 일치하는지 검증
		// pDir = msg + byteOffset;

		memcpy(&(file.cFileName), CString(pFileName), (fileNameLength + NULL_VALUE_SIZE) * 2);

		byteOffset += goalLength;
		memcpy(&file, msg + byteOffset, WIN_FIND_DATA_FRONT_SIZE);

		cFileAgentView->AddItem(file);
		cFileAgentView->SetListSize(cFileAgentView->GetListSize() + 1);
		cFileAgentView->SetItemCountEx();
	}

}