 #include "stdafx.h"

#include "MainFrm.h"
#include "FileAgentView.h"

#include "ShowProcessor.h"

#include <iostream>

ShowProcessor::ShowProcessor()
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


ShowProcessor::~ShowProcessor()
{
}

void ShowProcessor::PacketProcess(SOCKET sock, char *msg)
{
	TRACE(TEXT("Show Processor"));
	
	int offset = 0;

	offset += PROTOCOL_TYPE_SIZE;

	int listSize = 0;
	memcpy(&listSize, msg + PROTOCOL_TYPE_SIZE, sizeof(int));

	offset += sizeof(int);

	cFileAgentView->SetListSize(listSize);
	 
}

