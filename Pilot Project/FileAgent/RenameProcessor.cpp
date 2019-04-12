#include "stdafx.h"

#include "RenameProcessor.h"
#include "MainFrm.h"
#include "FileAgentView.h"

#include <iostream>

RenameProcessor::RenameProcessor()
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


RenameProcessor::~RenameProcessor()
{
}

void RenameProcessor::PacketProcess(SOCKET sock, char * msg)
{
	int dirLength = 0;
	int oldFileLength = 0;
	int newFileLength = 0;

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

	byteOffset = 0 + PROTOCOL_TYPE_SIZE + sizeof(int);
	char* pDir = msg + byteOffset;

	// @issue validate..
	if (cFileAgentView) {

	}

	byteOffset += dirLength + sizeof(int);
	char* pOldFile = msg + byteOffset;

	byteOffset += oldFileLength + sizeof(int);
	char* pNewFile = msg + byteOffset;

	cFileAgentView->RenameItem(pOldFile, pNewFile);
	cFileAgentView->SetItemCountEx(cFileAgentView->GetListSize());
	cFileAgentView->RedrawWindow();
}
