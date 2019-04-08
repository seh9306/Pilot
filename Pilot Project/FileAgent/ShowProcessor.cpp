#include "stdafx.h"

#include "MainFrm.h"
#include "FileAgentView.h"

#include "ShowProcessor.h"


#include <iostream>

ShowProcessor::ShowProcessor()
{
}


ShowProcessor::~ShowProcessor()
{
}

void ShowProcessor::PacketProcess(SOCKET sock, char *msg)
{
	TRACE(TEXT("Show Processor"));
	CMainFrame* cMainFrame = (CMainFrame*)(AfxGetApp()->GetMainWnd());
	if (cMainFrame == nullptr)
	{
		TRACE(TEXT("get Main Frame failure"));
	}
	else 
	{
		cMainFrame->GetActiveView();
		TRACE(TEXT("get Main Frame success"));
	}

	CFileAgentView* cFileAgentView = (CFileAgentView*)(cMainFrame->GetActiveView());
	if (cFileAgentView == nullptr)
	{
		TRACE(TEXT("get list control failure"));
	}
	int offset = 0;

	offset += PROTOCOL_TYPE_SIZE;

	int listSize = 0;
	memcpy(&listSize, msg + PROTOCOL_TYPE_SIZE, sizeof(int));

	offset += sizeof(int);

	std::cout << "listSize :: " << listSize << std::endl;
	int recvBytes = 0;
	int flags = 0;

	dataBuf.buf = msg;
	dataBuf.len = 1024;

	for (int i = 0; i < listSize; i++) {
		// #delete
		WIN32_FIND_DATA file;// = new WIN32_FIND_DATA;
		int length = 0;

		if (msg[offset] == '\n')
		{
			if (WSARecv(sock, &dataBuf, 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, NULL, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					//ErrorHandling("WSASend() error");
				}
			}
			else
			{
				//totalRecvBytes += recvBytes;
				offset = 1;
				//printf("%d:: %d %d\n", *((int *)(dataBuf.buf + 36)),dataBuf.len, recvBytes);
			}
		}

		memcpy(&file, msg + offset, WIN_FIND_DATA_FRONT_SIZE);
		offset += WIN_FIND_DATA_FRONT_SIZE;
		
		memcpy(&length, msg + offset, sizeof(int));
		offset += sizeof(int);
		//std::cout << msg + offset << std::endl;
		
		memcpy(&(file.cFileName), CString(msg +offset), (length+ NULL_VALUE_SIZE) * 2);
		/*CString str(msg + offset);

		cFileAgentView->AddItem(str);*/
		offset += length + NULL_VALUE_SIZE;

		//std::wcout << file.cFileName << "±â´Þ¶×" << std::endl;

		cFileAgentView->AddItem(file);

	}
	
	cFileAgentView->SetItemCountEx(listSize);

}

