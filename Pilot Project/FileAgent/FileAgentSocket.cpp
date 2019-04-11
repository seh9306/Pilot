#include "stdafx.h"
#include "FileAgentView.h"
#include "FileAgentSocket.h"

#include "SubscribeProcessor.h"
#include "ShowProcessor.h"
#include "ShowAddProcessor.h"
#include "CreateProcessor.h"
#include "RenameProcessor.h"
#include "DeleteProcessor.h"
#include "MoveProcessor.h"

#include "Receiver.h"
#include <thread>

FileAgentSocket *FileAgentSocket::fileAgent = nullptr;

FileAgentSocket::FileAgentSocket() 
{
	WSAStartup(0x0202, &wsaData);
	fileAgentSocket = INVALID_SOCKET;//socket(AF_INET, SOCK_STREAM, 0);
	dataBuf.len = 0;

	// Create IO CompletionPort
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);

	packetProcessors.push_back(new SubscribeProcessor());
	packetProcessors.push_back(nullptr); // UnSubscribe
	packetProcessors.push_back(new ShowProcessor());
	packetProcessors.push_back(new ShowAddProcessor());
	packetProcessors.push_back(new CreateProcessor());
	packetProcessors.push_back(new RenameProcessor());
	packetProcessors.push_back(new DeleteProcessor());
	packetProcessors.push_back(new MoveProcessor());

}

// @issue
FileAgentSocket::~FileAgentSocket() 
{
	TRACE(TEXT("FileAgentSocket 소멸"));
	
}

// @issue
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

SOCKET& FileAgentSocket::GetSocket()
{
	return fileAgentSocket;
}

void FileAgentSocket::CloseSocket()
{
	closesocket(fileAgentSocket);
	fileAgentSocket = INVALID_SOCKET;
}

void FileAgentSocket::Connect(char *ipAddress, int port)
{
	if (cleanSocket == true)
	{
		closesocket(fileAgentSocket);
		fileAgentSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		TRACE(TEXT("새로운 소켓 개방"));
	} 
	else
	{
		cleanSocket = true;
		fileAgentSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	}
	
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr(ipAddress);
	srv_addr.sin_port = htons(port);

	Receiver receiver;
	receiver.id = 0;
	std::thread IOCPThread(receiver, hCompletionPort, packetProcessors);
	IOCPThread.detach();

	/*bool connecting = true;
	std::thread t([&](sockaddr_in* srv_addr)
	{
		connect(fileAgentSocket, (sockaddr *)&srv_addr, sizeof(srv_addr));
		connecting = false;
	}, srv_addr);

	while (connecting)
	{
		MSG msg = { 0, };
		while (connecting && PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(10);
	}*/

	if (connect(fileAgentSocket, (sockaddr *)&srv_addr, sizeof(srv_addr)) == -1) {
		AfxMessageBox(TEXT("서버와의 연결에 실패하였습니다."));
		fileAgentSocket = INVALID_SOCKET;
		return;
	}
	AfxMessageBox(TEXT("서버와의 연결에 성공하였습니다."));

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

// @issue
#include <iostream>
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
	}

}

void FileAgentSocket::UnSubscribe(char * dir)
{
	dataBuf.buf = buffer;

	dataBuf.buf[0] = kUnSubscribe;
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

void FileAgentSocket::Show(char* dir, DWORD showNumber)
{
	dataBuf.buf = buffer;

	dataBuf.buf[0] = kShow;
	dataBuf.len = 0 + PROTOCOL_TYPE_SIZE;

	memcpy(dataBuf.buf + dataBuf.len, &showNumber, sizeof(DWORD));
	dataBuf.len += sizeof(DWORD);

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

void FileAgentSocket::Delete(char* dir, char* fileName, char attribute)
{
	dataBuf.buf = buffer;

	dataBuf.buf[0] = kDelete;
	dataBuf.len = 0 + PROTOCOL_TYPE_SIZE;

	if (attribute & FILE_ATTRIBUTE_DIRECTORY)
	{
		dataBuf.buf[1] = 1;
	}
	else
	{
		dataBuf.buf[1] = 0;
	}

	dataBuf.len += 1;

	// dir
	int length = strlen(dir) + NULL_VALUE_SIZE;

	memcpy(dataBuf.buf + dataBuf.len, &length, sizeof(int));
	dataBuf.len += sizeof(int);

	memcpy(dataBuf.buf + dataBuf.len, dir, length);
	dataBuf.len += length;

	// fileName
	length = strlen(fileName) + NULL_VALUE_SIZE;

	memcpy(dataBuf.buf + dataBuf.len, &length, sizeof(int));
	dataBuf.len += sizeof(int);

	memcpy(dataBuf.buf + dataBuf.len, fileName, length);
	dataBuf.len += length;

	if (WSASend(fileAgentSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			TRACE("error");
		}
	}
}

// @issue validate data..
void FileAgentSocket::Rename(char* dir, char* oldName, char* newName)
{
	dataBuf.buf = buffer;

	dataBuf.buf[0] = kRename;
	dataBuf.len = 0 + PROTOCOL_TYPE_SIZE;

	// dir
	int length = strlen(dir) + NULL_VALUE_SIZE;

	memcpy(dataBuf.buf + dataBuf.len, &length, sizeof(int));
	dataBuf.len += sizeof(int);

	memcpy(dataBuf.buf + dataBuf.len, dir, length);
	dataBuf.len += length;

	// oldFileName
	length = strlen(oldName) + NULL_VALUE_SIZE;

	memcpy(dataBuf.buf + dataBuf.len, &length, sizeof(int));
	dataBuf.len += sizeof(int);

	memcpy(dataBuf.buf + dataBuf.len, oldName, length);
	dataBuf.len += length;

	// newFileName
	length = strlen(newName) + NULL_VALUE_SIZE;

	memcpy(dataBuf.buf + dataBuf.len, &length, sizeof(int));
	dataBuf.len += sizeof(int);

	memcpy(dataBuf.buf + dataBuf.len, newName, length);
	dataBuf.len += length;

	if (WSASend(fileAgentSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			TRACE("error");
		}
	}

	std::cout << "전송" << std::endl;
}

