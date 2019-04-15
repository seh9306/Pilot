#include "Server/NetworkData.h"
#include "PacketProcessor/PacketProcessor.h"
#include "Util/SubscribeManager.h"
#include "Receiver.h"
#include "Server/Server.h"

#include <iostream>
#include <string>

void Receiver::operator()(Server* server, HANDLE pComPort, std::vector<PacketProcessor *> &packetProcessors)
{
	hCompletionPort = pComPort;
	SubscribeManager& subcribeManager = SubscribeManager::GetInstance();
	while (true) 
	{
		GetQueuedCompletionStatus(hCompletionPort,
			&bytesTransferred,
			(LPDWORD)&perHandleData,
			(LPOVERLAPPED*)&perIoData,
			INFINITE
		);

		if (perIoData->type == IOCP_ASYNC_RECV)
		{
			if (bytesTransferred == 0)
			{
				std::string temp = subcribeManager.GetDirBySocket(perHandleData->hClntSock);
				subcribeManager.UnSubscribe((char *)temp.c_str(), perHandleData->hClntSock);
				std::cout << "Out clnt";
				server->clntInOut(-1);
				closesocket(perHandleData->hClntSock);
				free(perHandleData);
				free(perIoData);
				continue;
			}

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
		else if (perIoData->type == IOCP_ASYNC_SEND)
		{
			delete perIoData;
		}

	}

}