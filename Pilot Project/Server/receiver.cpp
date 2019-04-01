#include "receiver.h"

#include <iostream>

void Receiver::operator()(HANDLE pComPort) {
	hCompletionPort = pComPort;
	std::cout << "id : " << id << std::endl;
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

		PerIoData->wsaBuf.len = BytesTransferred;
		printf("%s %d\n", PerIoData->wsaBuf.buf, BytesTransferred);
		WSASend(PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1,
			NULL, 0, NULL, NULL);
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