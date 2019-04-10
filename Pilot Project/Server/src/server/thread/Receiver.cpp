#include "Server/NetworkData.h"
#include "PacketProcessor/PacketProcessor.h"
#include "Util/SubscribeManager.h"
#include "Receiver.h"

#include <iostream>
#include <string>

void Receiver::operator()(HANDLE pComPort, std::vector<PacketProcessor *> &packetProcessors)
{
	hCompletionPort = pComPort;
	std::cout << id << std::endl;
	SubscribeManager& subcribeManager = SubscribeManager::GetInstance();
	while (true) 
	{
		GetQueuedCompletionStatus(hCompletionPort,
			&bytesTransferred,
			(LPDWORD)&perHandleData,
			(LPOVERLAPPED*)&perIoData,
			INFINITE
		);

		if (bytesTransferred == 0)
		{
			std::string temp = subcribeManager.GetDirBySocket(perHandleData->hClntSock);
			char *cstr = (char *)temp.c_str();
			subcribeManager.UnSubscribe(cstr, perHandleData->hClntSock);
			std::cout << "나가셨습니다" << std::endl;
			closesocket(perHandleData->hClntSock);
			free(perHandleData);
			free(perIoData);
			continue;
		}

		// decompress packet
		// ...

		// distribute packet
		if (packetProcessors.size() > perIoData->wsaBuf.buf[0]) {
			PacketProcessor *packetProcessor = packetProcessors.at(perIoData->wsaBuf.buf[0]);
			// packet processing 
			// according to each protocol number ( vector index )
			packetProcessor->PacketProcess(perHandleData->hClntSock,
				perIoData->wsaBuf.buf); 
		}
		else {
			std::cout << "NULL~" << std::endl;
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