#include "receiver.h"

#include <iostream>

void Receiver::operator()(HANDLE pComPort, vector<PacketProc *> *packetProcs) {
	hCompletionPort = pComPort;
	
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

		// ���� ���� ó��

		// ��Ŷ �й�
		if (packetProcs->size() > PerIoData->wsaBuf.buf[0]) {
			PacketProc *pp = packetProcs->at(PerIoData->wsaBuf.buf[0]);
			pp->packetProc(PerIoData->wsaBuf.buf + 1); // ��Ŷ ó��
		}
		else {
			std::cout << "NULL~" << std::endl;
		}			

		/*PerIoData->wsaBuf.len = BytesTransferred;
		printf("%s %d\n", PerIoData->wsaBuf.buf, BytesTransferred);*/
		
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
/*
void Receiver::operator()() {
	for (int i = 0; i < 5; i++) {
		Sleep(1000);
		std::cout << "ThreadPara Num : " << id << std::endl;
	}
}*/