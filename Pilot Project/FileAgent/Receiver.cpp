#include "stdafx.h"

#include "Receiver.h"
#include "PacketProcessor.h"
#include "FileAgentSocket.h"
#include <iostream>

void Receiver::operator()(HANDLE pComPort, std::vector<PacketProcessor *> &packetProcessors)
{
	hCompletionPort = pComPort;
	while (true) 
	{
		GetQueuedCompletionStatus(hCompletionPort,
			&bytesTransferred,
			(PULONG_PTR)&perHandleData,
			(LPOVERLAPPED*)&perIoData,
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
			//closesocket(perHandleData->hClntSock);
			delete perHandleData;
			delete perIoData;
			continue;
		}
		// decompress packet
		// ...

		// distribute packet
		if (packetProcessors.size() > (unsigned char)perIoData->wsaBuf.buf[0]) {
			PacketProcessor *packetProcessor = packetProcessors.at(perIoData->wsaBuf.buf[0]);
			// packet processing 
			// according to each protocol number ( vector index )
			packetProcessor->PacketProcess(perHandleData->hClntSock,
				perIoData->wsaBuf.buf); 
		}
		else {
			std::cout << "no packet processor" << std::endl;
		}

		memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
		perIoData->wsaBuf.len = BUFSIZE;
		perIoData->wsaBuf.buf = perIoData->buffer;

		flags = 0;

		WSARecv(perHandleData->hClntSock,
			&(perIoData->wsaBuf),
			1,
			nullptr,
			&flags,
			&(perIoData->overlapped),
			nullptr
		);

	}

}