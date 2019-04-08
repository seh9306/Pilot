#include "stdafx.h"
#include "FileAgentView.h"
#include "FileAgentSocket.h"
#include "ShowProcessor.h"
#include "Receiver.h"
#include <thread>

FileAgentSocket *FileAgentSocket::fileAgent = nullptr;

FileAgentSocket::FileAgentSocket() 
{
	WSAStartup(0x0202, &wsaData);
	fileAgentSocket = socket(AF_INET, SOCK_STREAM, 0);
	dataBuf.len = 0;
	TRACE(TEXT("FileAgentSocket 생성"));
	// Create IO CompletionPort
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);

	packetProcessors.push_back(new ShowProcessor());
	packetProcessors.push_back(new ShowProcessor());
	packetProcessors.push_back(new ShowProcessor());
}

FileAgentSocket::~FileAgentSocket() 
{
	TRACE(TEXT("FileAgentSocket 소멸"));
	
}

FileAgentSocket* FileAgentSocket::GetInstance() 
{
	if (fileAgent == nullptr) 
	{
		fileAgent = new FileAgentSocket();
		atexit(FileAgentSocket::releaseInstance);
	}

	return fileAgent;
}

void FileAgentSocket::releaseInstance()
{
	if (fileAgent) 
	{
		// issue#
		//closesocket(fileAgentSocket);
		//fileAgentSocket = INVALID_SOCKET;
		delete fileAgent;
		fileAgent = nullptr;
	}
}

void FileAgentSocket::Connect(char *ipAddress, int port)
{
	if (cleanSocket == true)
	{
		closesocket(fileAgentSocket);
		fileAgentSocket = socket(AF_INET, SOCK_STREAM, 0);
		TRACE(TEXT("새로운 소켓 개방"));
	} 
	else
	{
		cleanSocket = true;
		TRACE(TEXT("새로운 소켓을 개방하지 않음"));
	}
	
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr(ipAddress);
	srv_addr.sin_port = htons(port);

	Receiver receiver;
	receiver.id = 0;
	std::thread IOCPThread(receiver, hCompletionPort, packetProcessors);
	IOCPThread.detach();

	connect(fileAgentSocket, (sockaddr *)&srv_addr, sizeof(srv_addr));

	perHandleData = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
	perHandleData->hClntSock = fileAgentSocket;
	memcpy(&(perHandleData->clntAddr), &fileAgentSocket, sizeof(fileAgentSocket));

	CreateIoCompletionPort((HANDLE)fileAgentSocket, hCompletionPort, (DWORD)perHandleData, 0);

	perIoData = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
	memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
	perIoData->wsaBuf.len = BUFSIZE;
	perIoData->wsaBuf.buf = perIoData->buffer;
	flags = 0; // (?)

	WSARecv(perHandleData->hClntSock,
		&(perIoData->wsaBuf),
		1,
		&recvBytes,
		&flags,
		&(perIoData->overlapped),
		nullptr
	);
}

SOCKET FileAgentSocket::GetSocket()
{
	return fileAgentSocket;
}

void FileAgentSocket::Subscribe(char * dir)
{
	dataBuf.buf = buffer;

	dataBuf.buf[0] = kSubscribe;
	dataBuf.len = 0 + PROTOCOL_TYPE_SIZE;

	int length = strlen(dir);
	memcpy(dataBuf.buf + dataBuf.len, &length, sizeof(int));
	dataBuf.len += sizeof(int);

	memcpy(dataBuf.buf + dataBuf.len, dir, length + NULL_VALUE_SIZE);
	dataBuf.len += strlen(dir) + NULL_VALUE_SIZE;

	if (WSASend(fileAgentSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			TRACE("error");
		}
		char temp[24];
		sprintf_s(temp, "%d", GetLastError());
		TRACE(temp);
		//fileAgentView.AddItem(TEXT("전송 실패"));
	}
	else
	{
		//fileAgentView.AddItem(TEXT("보내기 완료"));
	}
}

void FileAgentSocket::Show(char* dir)
{
	dataBuf.buf = buffer;

	dataBuf.buf[0] = kShow;
	dataBuf.len = 0 + PROTOCOL_TYPE_SIZE;

	int length = strlen(dir);
	memcpy(dataBuf.buf + dataBuf.len, &length, sizeof(int));
	dataBuf.len += sizeof(int);

	memcpy(dataBuf.buf + dataBuf.len, dir, length + NULL_VALUE_SIZE);
	dataBuf.len += strlen(dir) + NULL_VALUE_SIZE;

	if (WSASend(fileAgentSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			TRACE("error");
		}
	}

}

void FileAgentSocket::Update()
{
}

void FileAgentSocket::Delete()
{
}

void FileAgentSocket::Rename()
{
}

