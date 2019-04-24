#include "stdafx.h"

#include "Receiver.h"
#include "PacketProcessor.h"
#include "FileAgentSocket.h"
#include <iostream>

void Receiver::operator()(HANDLE pComPort, std::vector<std::unique_ptr<PacketProcessor>>* packetProcessors)
{
	SocketDataPerClient* socketDataPerClient;
	AsyncIOBuffer* asyncIOBuffer = nullptr;
	DWORD bytesTransferred = 0;
	DWORD flags = 0; // (?) 
	while (true) 
	{
		GetQueuedCompletionStatus(pComPort,
			&bytesTransferred,
			(PULONG_PTR)&socketDataPerClient,
			(LPOVERLAPPED*)&asyncIOBuffer,
			INFINITE
		);

		if (bytesTransferred == 0)
		{
			// @issue make socket INVALID_SOCKET
			FileAgentSocket* fileAgentSocket = FileAgentSocket::GetInstance();
			if (fileAgentSocket)
			{
				fileAgentSocket->CloseSocket();
			}
			closesocket(socketDataPerClient->clientSocket);
			socketDataPerClient->clientSocket = INVALID_SOCKET;
			delete socketDataPerClient;
			delete asyncIOBuffer;
			continue;
		}
		// decompress packet
		// ...

		// distribute packet
		if (packetProcessors->size() > (unsigned char)asyncIOBuffer->wsaBuf.buf[0]) {
			PacketProcessor *packetProcessor = packetProcessors->at(asyncIOBuffer->wsaBuf.buf[0]).get();
			// packet processing 
			// according to each protocol number ( vector index )
			if (packetProcessor != nullptr)
			{
				packetProcessor->PacketProcess(socketDataPerClient->clientSocket,
					asyncIOBuffer->wsaBuf.buf);
			}
		}
		else {
			std::cout << "no packet processor" << std::endl;
		}

		memset(&(asyncIOBuffer->overlapped), 0, sizeof(OVERLAPPED));
		asyncIOBuffer->wsaBuf.len = BUFSIZE;
		asyncIOBuffer->wsaBuf.buf = asyncIOBuffer->buffer;

		flags = 0;

		WSARecv(socketDataPerClient->clientSocket,
			&(asyncIOBuffer->wsaBuf),
			1,
			nullptr,
			&flags,
			&(asyncIOBuffer->overlapped),
			nullptr
		);

	}

}