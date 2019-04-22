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
#include "PacketProcessor/CopyProcessor.h"
#include "PacketProcessor/RenameProcessor.h"
#include "PacketProcessor/DeleteProcessor.h"
#include "PacketProcessor/MoveProcessor.h"
#include "PacketProcessor/ConnectProcessor.h"

extern void error_handle(char *);

Server::Server() 
{
}

Server::~Server()
{
	if (serverSocket_ != INVALID_SOCKET)
	{
		closesocket(serverSocket_);
	}
	serverSocket_ = INVALID_SOCKET;
}

int Server::Init(int port) 
{
	{
		WSADATA wsaData; // Window Socket API Data

		// Window Socket API initialize
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			error_handle("WSAStartup() error !");
		}
	}

	serverSocket_ = WSASocketW(PF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);

	{
		SOCKADDR_IN servAddr; // Server Address

		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servAddr.sin_port = htons(port);

		if (bind(serverSocket_, (SOCKADDR*)&servAddr, sizeof(servAddr)) == -1)
		{
			return kBindError;
		}
	}

	if (listen(serverSocket_, 5) == -1)
	{
		return kListenError;
	}
	
	// using std::unique_ptr in AddProcessor
	AddProcessor(new SubscribeProcessor());
	AddProcessor(new UnSubscribeProcessor());
	AddProcessor(new ShowProcessor());
	AddProcessor(nullptr); // ShowAdd
	AddProcessor(new CopyProcessor());
	AddProcessor(new RenameProcessor());
	AddProcessor(new DeleteProcessor());
	AddProcessor(new MoveProcessor());
	AddProcessor(new ConnectProcessor());

	// Create IO CompletionPort
	hCompletionPort_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);

	return kSuccess;
}

int Server::Start() 
{
	std::cout << "Start Network Explorer Server..." << std::endl;

	SYSTEM_INFO systemInfo; // For number of cpu Proc
	GetSystemInfo(&systemInfo);
	
	for (unsigned int i = 0; i < systemInfo.dwNumberOfProcessors; i++)
	{
		Receiver receiver;
		receiver.id = i;
		std::thread IOCPThread(receiver, (Server *)this, hCompletionPort_, &packetProcessors_);
		std::cout << "Receiver Thread Start :: (" << i << ")" << std::endl;
		IOCPThread.detach();
	}

	SOCKET clientSocket;

	while (TRUE) 
	{
		SOCKADDR_IN clientAddress;
		int addrLen = sizeof(clientAddress);

		clientSocket = accept(serverSocket_, reinterpret_cast<SOCKADDR*>(&clientAddress), &addrLen);
		{ // exception
			int accpetErrorCode = WSAGetLastError();

			if (accpetErrorCode != 0)
			{
				continue;
			}
		}

		numberOfClient_++;

		SocketDataPerClient* socketDataPerClient
			= new SocketDataPerClient(clientSocket, clientAddress);

		CreateIoCompletionPort((HANDLE)clientSocket, hCompletionPort_, (ULONG_PTR)socketDataPerClient, 0);

		AsyncIOBuffer* asyncIOBuffer = new AsyncIOBuffer(IOCP_ASYNC_RECV);

		DWORD flags = 0;

		WSARecv(socketDataPerClient->clientSocket,
			&(asyncIOBuffer->wsaBuf),
			1,
			nullptr,
			&flags,
			&(asyncIOBuffer->overlapped),
			nullptr
		);

		{
			int recvErrorCode = WSAGetLastError();
			if (recvErrorCode != WSA_IO_PENDING && recvErrorCode != 0)
			{
				numberOfClient_--;
			}
		}
	}
}

void Server::AddProcessor(PacketProcessor* packetProcessor)
{
	packetProcessors_.push_back(
		std::unique_ptr<PacketProcessor>(packetProcessor)
	);
}
