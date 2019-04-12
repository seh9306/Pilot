#include "stdafx.h"

#include "PacketProcessor.h"
#include "MainFrm.h"

PacketProcessor::PacketProcessor()
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