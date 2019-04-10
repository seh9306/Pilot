#include <iostream>
#include <thread>
#include <vector>

#include "Server.h"
#include "thread/Receiver.h"

#include "NetworkData.h"

// packet process
#include "../PacketProcessor/PacketProcessor.h"
#include "../PacketProcessor/SubscribeProcessor.h"
#include "../PacketProcessor/UnSubscribeProcessor.h"
#include "../PacketProcessor/ShowProcessor.h"
// ShowAddProcessor.h
#include "../PacketProcessor/CreateProcessor.h"
#include "../PacketProcessor/RenameProcessor.h"
#include "../PacketProcessor/DeleteProcessor.h"
#include "../PacketProcessor/MoveProcessor.h"

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

	// Create IO CompletionPort
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	
	for (int i = 0; i < systemInfo.dwNumberOfProcessors; i++) 
	{
		Receiver receiver;
		receiver.id = i;
		std::thread IOCPThread(receiver, hCompletionPort, packetProcessors);
		IOCPThread.detach();

	}

	return true;
}

bool Server::Start() 
{
	int cnt = 0;
	while (TRUE) 
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAddr;
		int addrLen = sizeof(clntAddr);
		printf("waiting for client... %d\n", cnt);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &addrLen);
		cnt++;

		perHandleData = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		perHandleData->hClntSock = hClntSock;
		memcpy(&(perHandleData->clntAddr), &clntAddr, addrLen);

		// params
		// 1. client socket handle
		// 2. 인자에 완료 포트 핸들이 있으면 생성이 아니고 추가한다.
		// 3. 주소와 포트 
		// 4. 0을 입력하면 cpu 개수에 따라 자동 설정(?)
		CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (DWORD)perHandleData, 0);

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
}