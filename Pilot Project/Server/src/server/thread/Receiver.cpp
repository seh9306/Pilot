#include "Server/NetworkData.h"
#include "PacketProcessor/PacketProcessor.h"
#include "Util/SubscribeManager.h"
#include "Receiver.h"
#include "Server/Server.h"

#include <iostream>
#include <string>

void Receiver::operator()(Server* server,
		HANDLE completionPort,
		std::vector<std::unique_ptr<PacketProcessor>>* packetProcessors)
{
	SubscribeManager& subcribeManager = SubscribeManager::GetInstance();
	
	AsyncIOBuffer* asyncIOBuffer;
	SocketDataPerClient* socketDataPerClient = 0;
	
	DWORD bytesTransferred = 0;
	DWORD flags = 0;

	while (true) 
	{
		GetQueuedCompletionStatus(completionPort,
			&bytesTransferred,
			(PULONG_PTR)&socketDataPerClient,
			(LPOVERLAPPED*)&asyncIOBuffer,
			INFINITE
		);

		if (bytesTransferred == 0)
		{
			subcribeManager.UnSubscribe(socketDataPerClient->clientSocket);
			// TODO : 클라이언트 수 감소
			closesocket(socketDataPerClient->clientSocket);
			delete socketDataPerClient;
			// 전달하고 있는 데이터를 할당해제일 가능성
			if (asyncIOBuffer != nullptr)
			{
				delete asyncIOBuffer;
			}
			continue;
		}
		
		if (asyncIOBuffer->type == IOCP_ASYNC_RECV)
		{
			// distribute packet
			int index = asyncIOBuffer->wsaBuf.buf[0];
			if (packetProcessors->size() > index) {
				PacketProcessor *packetProcessor = packetProcessors->at(index).get();
				if (packetProcessor != nullptr)
				{
					// packet processing 
					// according to each protocol number ( vector index )
					packetProcessor->ProcessPacket(socketDataPerClient->clientSocket,
						asyncIOBuffer->wsaBuf.buf);
				}
				else
				{
					printf("index %d is nullptr \n",index);
				}
			}
			else {
				std::cout << "no packet processor" << std::endl;
			}

			memset(&(asyncIOBuffer->overlapped), 0, sizeof(OVERLAPPED));

			WSARecv(socketDataPerClient->clientSocket,
				&(asyncIOBuffer->wsaBuf),
				1,
				nullptr,
				&flags,
				&(asyncIOBuffer->overlapped),
				nullptr
			);

		}
		else if (asyncIOBuffer->type == IOCP_ASYNC_SEND)
		{
			delete asyncIOBuffer;
		}
	}

}