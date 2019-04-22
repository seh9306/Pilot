#include "PublishManager.h"

#include <iostream>
#include <WinSock2.h>

#include "FileManager.h"
#include "PacketProcessor/PacketProcessor.h"
#include "Server/NetworkData.h"

PublishManager::PublishManager()
{
}

PublishManager::~PublishManager()
{
}

PublishManager& PublishManager::GetInstance()
{
	static PublishManager publishManager;
	return publishManager;
}

AsyncIOBuffer* PublishManager::CreateData(char* msg, int size)
{
	// memory will be deleted after send in Recevier.cpp
	AsyncIOBuffer* asyncIOBuffer = new AsyncIOBuffer();

	if (asyncIOBuffer == nullptr)
	{
		return nullptr;
	}

	memset(&(asyncIOBuffer->overlapped), 0, sizeof(OVERLAPPED));
	asyncIOBuffer->wsaBuf.len = size;

	// copy data
	if (msg != nullptr) 
	{
		memcpy(asyncIOBuffer->buffer, msg, size);
	}
	else
	{
		delete asyncIOBuffer;
		return nullptr;
	}
	
	asyncIOBuffer->wsaBuf.buf = asyncIOBuffer->buffer;
	asyncIOBuffer->type = IOCP_ASYNC_SEND;

	return asyncIOBuffer;
}

bool PublishManager::Publish(char * msg, std::list<SOCKET>& socks, int size)
{
	for (SOCKET sock : socks)
	{
		Publish(msg, sock, size);
	}
	return true;
}

void PublishManager::Publish(char * msg, SOCKET sock, int size)
{
	AsyncIOBuffer* perIoData = CreateData(msg, size);
	
	if (perIoData == nullptr)
	{
		std::cout << "Memory allocate error" << std::endl;
		return;
	}

	if (WSASend(sock, &(perIoData->wsaBuf), 1,
		nullptr, 0, &(perIoData->overlapped), nullptr) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING) {
			std::cout << "WSASend() error :: " << GetLastError() << std::endl;
		}
		else
		{
		//	delete perIoData;
		}
			
	}
}