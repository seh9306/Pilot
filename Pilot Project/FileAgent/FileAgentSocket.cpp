#include "stdafx.h"
#include "FileAgentView.h"
#include "FileAgentSocket.h"

#include "SubscribeProcessor.h"
#include "ShowProcessor.h"
#include "ShowAddProcessor.h"
#include "CopyProcessor.h"
#include "RenameProcessor.h"
#include "DeleteProcessor.h"
#include "MoveProcessor.h"
#include "ConnectProcessor.h"

#include "Receiver.h"
#include "Resource.h"
#include <thread>

FileAgentSocket *FileAgentSocket::fileAgent = nullptr;

FileAgentSocket::FileAgentSocket() 
{
	WSAStartup(0x0202, &wsaData);
	fileAgentSocket = INVALID_SOCKET;//socket(AF_INET, SOCK_STREAM, 0);
	dataBuf.len = 0;

	// Create IO CompletionPort
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
	
	AddProcessor(new SubscribeProcessor());
	AddProcessor(nullptr); // UnSubscribe
	AddProcessor(new ShowProcessor());
	AddProcessor(new ShowAddProcessor());
	AddProcessor(new CopyProcessor());
	AddProcessor(new RenameProcessor());
	AddProcessor(new DeleteProcessor());
	AddProcessor(new MoveProcessor());
	AddProcessor(new ConnectProcessor());

}

void FileAgentSocket::AddProcessor(PacketProcessor* packetProcessor)
{
	packetProcessors.push_back(
		std::unique_ptr<PacketProcessor>(packetProcessor)
	);
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
		closesocket(fileAgent->fileAgentSocket);
		fileAgent->fileAgentSocket = INVALID_SOCKET;
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
	} 
	else
	{
		cleanSocket = true;
		fileAgentSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	}
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(ipAddress);
	serverAddress.sin_port = htons(port);

	Receiver receiver;
	receiver.id = 0;
	std::thread IOCPThread(receiver, hCompletionPort, &packetProcessors);
	IOCPThread.detach();
	// 메시지 펌핑
	/*bool connecting = true;
	std::thread t([&]()
	{
		connect(fileAgentSocket, (sockaddr *)&srv_addr, sizeof(srv_addr));
		connecting = false;
	});

	while (connecting)
	{
		MSG msg = { 0, };
		while (connecting && GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(10);
	}*/

	// @issue TEXT String Table
	//WSAConnect(fileAgentSocket, (sockaddr *)&srv_addr, sizeof(srv_addr), nullptr, nullptr, nullptr, nullptr);
	if (connect(fileAgentSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
		stringTableValue.LoadStringW(FILE_CLIENT_CONNECT_FAIL);
		AfxMessageBox(stringTableValue);
		fileAgentSocket = INVALID_SOCKET;
		return;
	}

	stringTableValue.LoadStringW(FILE_CLIENT_CONNECT_SUCCESS);
	AfxMessageBox(stringTableValue);

	SendConnectionMessage();

	SocketDataPerClient* socketDataPerClient
		= new SocketDataPerClient(fileAgentSocket, serverAddress);

	CreateIoCompletionPort((HANDLE)fileAgentSocket, hCompletionPort, (DWORD)socketDataPerClient, 0);

	AsyncIOBuffer* asyncIOBuffer = new AsyncIOBuffer(IOCP_ASYNC_RECV);

	flags = 0; // (?)

	WSARecv(socketDataPerClient->clientSocket,
		&(asyncIOBuffer->wsaBuf),
		1,
		nullptr,
		&flags,
		&(asyncIOBuffer->overlapped),
		nullptr
	);
}

void FileAgentSocket::SendConnectionMessage()
{
	dataBuf.buf = buffer;
	
	dataBuf.buf[0] = kConnect;
	dataBuf.len = 0 + PROTOCOL_TYPE_SIZE;

	if (WSASend(fileAgentSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			TRACE("error");
		}
	}
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
		else
		{
			printf("보내야지?\n");
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
}

void FileAgentSocket::Move(char* dir, char* fileName, char* goal, WIN32_FIND_DATA& file)
{
	dataBuf.buf = buffer;

	dataBuf.buf[0] = kMove;
	dataBuf.len = 0 + PROTOCOL_TYPE_SIZE;

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

	// goal
	strcat(goal, "\\");
	length = strlen(dir) + strlen(goal) + NULL_VALUE_SIZE;

	memcpy(dataBuf.buf + dataBuf.len, &length, sizeof(int));
	dataBuf.len += sizeof(int);

	memcpy(dataBuf.buf + dataBuf.len, dir, strlen(dir));
	dataBuf.len += strlen(dir);
	
	memcpy(dataBuf.buf + dataBuf.len, goal, strlen(goal) + NULL_VALUE_SIZE);
	dataBuf.len += strlen(goal) + NULL_VALUE_SIZE;

	memcpy(dataBuf.buf + dataBuf.len, &file, WIN_FIND_DATA_FRONT_SIZE);
	dataBuf.len += WIN_FIND_DATA_FRONT_SIZE;

	if (WSASend(fileAgentSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			TRACE("error");
		}
	}
}

