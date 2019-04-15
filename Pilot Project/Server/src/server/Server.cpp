#include <iostream>
#include <thread>
#include <vector>

#include "Server.h"
#include "thread/Receiver.h"

#include "NetworkData.h"

// packet process
#include "PacketProcessor/PacketProcessor.h"
#include "PacketProcessor/SubscribeProcessor.h"
#include "PacketProcessor/UnSubscribeProcessor.h"
#include "PacketProcessor/ShowProcessor.h"
// ShowAddProcessor.h
#include "PacketProcessor/CreateProcessor.h"
#include "PacketProcessor/RenameProcessor.h"
#include "PacketProcessor/DeleteProcessor.h"
#include "PacketProcessor/MoveProcessor.h"
#include "PacketProcessor/ConnectProcessor.h"

extern void error_handle(char *);

Server::Server() 
{
	// Window Socket API initialize
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
	{
		error_handle("WSAStartup() error !");
	}

	hServSock = WSASocketW(PF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(9030);

	GetSystemInfo(&systemInfo);

}

Server::~Server()
{
	if (hServSock != INVALID_SOCKET)
	{
		closesocket(hServSock);
	}
	hServSock = INVALID_SOCKET;

	int size = packetProcessors.size();
	for (int index = 0; index < size; index++)
	{
		PacketProcessor* packetProcessor = packetProcessors.at(index);
		if (packetProcessor)
		{
			delete packetProcessor;
		}
	}
	packetProcessors.clear();
	DeleteCriticalSection(&criticalSection);
}

bool Server::Init() 
{
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR)) == -1) 
	{
		return kBindError;
	}

	if (listen(hServSock, 5) == -1) 
	{
		return kListenError;
	}
	
	packetProcessors.push_back(new SubscribeProcessor());
	packetProcessors.push_back(new UnSubscribeProcessor());
	packetProcessors.push_back(new ShowProcessor());
	packetProcessors.push_back(nullptr); // ShowAdd
	packetProcessors.push_back(new CreateProcessor());
	packetProcessors.push_back(new RenameProcessor());
	packetProcessors.push_back(new DeleteProcessor());
	packetProcessors.push_back(new MoveProcessor());
	packetProcessors.push_back(new ConnectProcessor());

	// Create IO CompletionPort
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	
	InitializeCriticalSection(&criticalSection);

	return true;
}

bool Server::Start() 
{
	std::cout << "Start Network Explorer Server..." << std::endl;

	for (unsigned int i = 0; i < systemInfo.dwNumberOfProcessors; i++)
	{
		Receiver receiver;
		receiver.id = i;
		std::thread IOCPThread(receiver, (Server *)this, hCompletionPort, packetProcessors);
		std::cout << "Receiver Thread Start :: (" << i << ")" << std::endl;
		IOCPThread.detach();

	}

	while (TRUE) 
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAddr;
		int addrLen = sizeof(clntAddr);

		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &addrLen);
		clntInOut(1);

		perHandleData = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		perHandleData->hClntSock = hClntSock;
		memcpy(&(perHandleData->clntAddr), &clntAddr, addrLen);

		CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (DWORD)perHandleData, 0);

		perIoData = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
		perIoData->wsaBuf.len = BUFSIZE;
		perIoData->wsaBuf.buf = perIoData->buffer;
		perIoData->type = IOCP_ASYNC_RECV;
		flags = 0;

		WSARecv(perHandleData->hClntSock,
			&(perIoData->wsaBuf),
			1,
			&recvBytes,
			&flags,
			&(perIoData->overlapped),
			nullptr
		);
	}
}

void Server::clntInOut(int count)
{
	EnterCriticalSection(&criticalSection);
	numberOfClient +=  count;
	LeaveCriticalSection(&criticalSection);
	std::cout << "Number of client : " << numberOfClient << std::endl;
}
