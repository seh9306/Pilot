#include <thread>
#include <vector>

#include "server.h"
#include "thread\receiver.h"

// packet process
#include "..\packet_proc\sub_proc.h"
#include "..\packet_proc\unsub_proc.h"
#include "..\packet_proc\show_proc.h"

extern void error_handle(char *);

Server* Server::init() {
	
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // Window Socket API initialize
		error_handle("WSAStartup() error !");
	}

	hServSock = WSASocketW(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR)) == -1) {
		int code = GetLastError();
		error_handle("bind() error !");
		exit(1);
	}

	if (listen(hServSock, 5) == -1) {
		int code = GetLastError();
		error_handle("listen() error !");
		exit(1);
	}

	// Create Processor vector
	packetProcs = new vector<PacketProc *>();

	packetProcs->push_back(new SubProc());
	packetProcs->push_back(new UnsubProc());
	packetProcs->push_back(new ShowProc());

	// Create IO CompletionPort
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&SystemInfo);

	for (int i = 0; i < SystemInfo.dwNumberOfProcessors; i++) {
		Receiver receiver;
		receiver.id = i;
		std::thread IOCPThread(receiver, hCompletionPort, std::ref(packetProcs));
		IOCPThread.detach();

	}

	return this;
	
}

void Server::start() {
	while (TRUE) {
		SOCKET hClntSock;
		SOCKADDR_IN clntAddr;
		int addrLen = sizeof(clntAddr);
		printf("waiting for client...\n");
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &addrLen);
		PerHandleData = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		PerHandleData->hClntSock = hClntSock;
		memcpy(&(PerHandleData->clntAddr), &clntAddr, addrLen);

		// 1번째 클라이언트 소켓 핸들
		// 2번째 인자에 완료 포트 핸들이 있으면 생성이 아니고 추가한다.
		// 3번째 주소와 포트 
		// 4번째 0을 입력하면 cpu 개수에 따라 자동 설정(?)
		CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (DWORD)PerHandleData, 0);

		PerIoData = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
		PerIoData->wsaBuf.len = BUFSIZE + 100;
		PerIoData->wsaBuf.buf = PerIoData->buffer;
		Flags = 0; // (?)

		WSARecv(PerHandleData->hClntSock,
			&(PerIoData->wsaBuf),
			1,
			&RecvBytes,
			&Flags,
			&(PerIoData->overlapped),
			NULL
		);
	}
}