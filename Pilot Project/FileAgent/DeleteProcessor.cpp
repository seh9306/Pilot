#include "stdafx.h"

#include "DeleteProcessor.h"
#include "MainFrm.h"
#include "FileAgentView.h"

#include <iostream>

DeleteProcessor::DeleteProcessor()
{
	CMainFrame* cMainFrame = (CMainFrame*)(AfxGetApp()->GetMainWnd());
	if (cMainFrame == nullptr)
	{
		TRACE(TEXT("get Main Frame failure"));
	}
	else
	{
		TRACE(TEXT("get Main Frame success"));
	}

	cFileAgentView = (CFileAgentView*)(cMainFrame->GetActiveView());
	if (cFileAgentView == nullptr)
	{
		TRACE(TEXT("get list control failure"));
	}
}


DeleteProcessor::~DeleteProcessor()
{
}

void DeleteProcessor::PacketProcess(SOCKET sock, char* msg)
{
	int dirLength = 0;
	int fileNameLength = 0;

	int byteOffset = PROTOCOL_TYPE_SIZE + FILE_TYPE_SIZE;

	if (msg[1] != 0 && msg[1] != 1)
	{
		return;
	}
	char fileType = msg[1];

	memcpy(&dirLength, msg + byteOffset, sizeof(int));
	byteOffset += sizeof(int);

	memcpy(&fileNameLength, msg + byteOffset + dirLength, sizeof(int));

	if (!msg || dirLength + fileNameLength > DELETE_HEADER_SIZE + MAX_PATH * 2 + NULL_VALUE_SIZE * 2) // MAX_PATH 260..
	{
		return;
	}

	char* pDir = msg + PROTOCOL_TYPE_SIZE + FILE_TYPE_SIZE + sizeof(int);

	// @issue validate..
	if (cFileAgentView) {

	}

	char* pFile = msg + PROTOCOL_TYPE_SIZE + FILE_TYPE_SIZE + sizeof(int) + dirLength + sizeof(int);

	cFileAgentView->DeleteItem(pFile);
	cFileAgentView->SetListSize(cFileAgentView->GetListSize()-1);
	cFileAgentView->SetItemCountEx(cFileAgentView->GetListSize());
}