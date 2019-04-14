#include "PublishManager.h"
#include "FileManager.h"

#include "PacketProcessor/PacketProcessor.h"

#include <iostream>

#include <WinSock2.h>

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

LPPER_IO_DATA PublishManager::CreateData(char* msg, int size)
{
	LPPER_IO_DATA perIoData = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));

	memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
	perIoData->wsaBuf.len = size;

	// copy data
	if (msg != nullptr) {
		memcpy(perIoData->buffer, msg, size);
	}
	
	perIoData->wsaBuf.buf = perIoData->buffer;

	perIoData->type = IOCP_ASYNC_SEND;

	return perIoData;
}

bool PublishManager::Publish(char * msg, std::list<SOCKET>& socks, int size)
{
	for (SOCKET sock : socks)
	{
		LPPER_IO_DATA perIoData = CreateData(msg, size);

		if (WSASend(sock, &(perIoData->wsaBuf), 1,
			nullptr, 0, &(perIoData->overlapped), nullptr) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				std::cout << "WSASend() error :: " << GetLastError() << std::endl;
		}

		if (WSASend(sock, &(perIoData->wsaBuf), 1,
			nullptr, 0, NULL, nullptr) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				std::cout << "WSASend() error :: " << GetLastError() << std::endl;
			}
		}
	}
	return true;
}

void PublishManager::Publish(char * msg, SOCKET sock, int size)
{
	LPPER_IO_DATA perIoData = CreateData(msg, size);
	
	perIoData->type = IOCP_ASYNC_SEND;

	if (WSASend(sock, &(perIoData->wsaBuf), 1,
		nullptr, 0, &(perIoData->overlapped), nullptr) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			std::cout << "WSASend() error :: " << GetLastError() << std::endl;
	}
}