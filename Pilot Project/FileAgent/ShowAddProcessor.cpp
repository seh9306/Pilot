#include "stdafx.h"

#include "ShowAddProcessor.h"

#include <iostream>

ShowAddProcessor::ShowAddProcessor()
{
}


ShowAddProcessor::~ShowAddProcessor()
{
}

void ShowAddProcessor::PacketProcess(SOCKET sock, char *msg)
{
	int offset = PROTOCOL_TYPE_SIZE;

	DWORD showNumber;

	memcpy(&showNumber, msg + offset, sizeof(DWORD));

	// @issue in multi thread
	if (showNumber != cFileAgentView->GetShowNumber())
	{
		return;
	}

	offset += sizeof(DWORD);
	
	WIN32_FIND_DATA file;// = new WIN32_FIND_DATA;
	int length = 0;

	while (msg[offset] != '\n')
	{
		memcpy(&file, msg + offset, WIN_FIND_DATA_FRONT_SIZE);
		offset += WIN_FIND_DATA_FRONT_SIZE;

		memcpy(&length, msg + offset, sizeof(int));
		offset += sizeof(int);

		memcpy(&(file.cFileName), CString(msg + offset), (length + NULL_VALUE_SIZE) * 2);

		offset += length + NULL_VALUE_SIZE;
		// @issue sync
		cFileAgentView->AddItem(file);
	}

	// @issue sync
	if (cFileAgentView->GetListSize() 
		- cFileAgentView->GetItemSize() == 0)
	{
		cFileAgentView->SetItemCountEx();
	}
	
}

