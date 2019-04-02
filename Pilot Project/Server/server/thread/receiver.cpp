#include "receiver.h"

#include <iostream>

void Receiver::operator()(HANDLE pComPort, vector<PacketProc *> *packetProcs) {
	hCompletionPort = pComPort;
	std::cout << id << std::endl;
	while (TRUE) {
		GetQueuedCompletionStatus(hCompletionPort,
			&BytesTransferred,
			(LPDWORD)&PerHandleData,
			(LPOVERLAPPED*)&PerIoData,
			INFINITE
		);

		if (BytesTransferred == 0) {
			closesocket(PerHandleData->hClntSock);
			free(PerHandleData);
			free(PerIoData);
			continue;
		}

		// decompress packet
		// ...

		// distribute packet
		if (packetProcs->size() > PerIoData->wsaBuf.buf[0]) {
			PacketProc *pp = packetProcs->at(PerIoData->wsaBuf.buf[0]);
			// packet processing 
			// according to each protocol number ( vector index )
			pp->packetProc(
				PerHandleData->hClntSock,
				PerIoData->wsaBuf.buf); 
		}
		else {
			std::cout << "NULL~" << std::endl;
		}			
		std::cout << "nonono" << std::endl;
		memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
		PerIoData->wsaBuf.len = BUFSIZE;
		PerIoData->wsaBuf.buf = PerIoData->buffer;

		flags = 0;

		WSARecv(PerHandleData->hClntSock,
			&(PerIoData->wsaBuf),
			1,
			NULL,
			&flags,
			&(PerIoData->overlapped),
			NULL
		);

	}

}