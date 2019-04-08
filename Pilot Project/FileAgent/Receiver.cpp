#include "stdafx.h"

#include "Receiver.h"
#include "PacketProcessor.h"

#include <iostream>

void Receiver::operator()(HANDLE pComPort, std::vector<PacketProcessor *> &packetProcessors)
{
	hCompletionPort = pComPort;
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
			std::cout << "�����̽��ϴ�" << std::endl;
			closesocket(perHandleData->hClntSock);
			free(perHandleData);
			free(perIoData);
			continue;
		}
		TRACE(TEXT("���ż��ż���"));
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
			TRACE(TEXT("NULL\n"));
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