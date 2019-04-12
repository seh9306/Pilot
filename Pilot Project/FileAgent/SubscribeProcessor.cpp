#include "stdafx.h"
#include "SubscribeProcessor.h"

#include "FileAgentSocket.h"

#include <iostream>

SubscribeProcessor::SubscribeProcessor()
{
}


SubscribeProcessor::~SubscribeProcessor()
{
}

// subcribe
void SubscribeProcessor::PacketProcess(SOCKET sock, char *msg)
{

	if (msg[SUCCESS_SUBSCRIBE_INDEX])
	{// success
		int length = 0;
		int byteOffset = PROTOCOL_TYPE_SIZE + SUCCESS_SUBSCRIBE_INDEX;

		// clear vector and generate number
		// @issue in multi-thread env
		DWORD showNumber = cFileAgentView->GenerateShowNumber();
		cFileAgentView->ClearItem();

		memcpy(&length, msg + byteOffset, sizeof(int));
		byteOffset += sizeof(int);

		//app->SetDirectory(msg + byteOffset, length);

		FileAgentSocket *fileAgentSocket = FileAgentSocket::GetInstance();

		fileAgentSocket->Show(msg + byteOffset, showNumber);
	}
	else
	{// fail
		//app->SetDirectory(NULL);
	}
}