#ifndef RECEIVER_H
#define RECEIVER_H

#include <WinSock2.h>
#include <vector>

class Receiver {
public:
	int id;
	void operator()(HANDLE pComPort, std::vector<PacketProcessor *> &packetProcessors);
	
private:
	HANDLE hCompletionPort;
	DWORD bytesTransferred;
	LPPER_HANDLE_DATA perHandleData; // 각 클라이언트의 소켓 정보?
	LPPER_IO_DATA perIoData; // 각 클라이언트의 버퍼 정보?
	DWORD flags; // (?)
};

#endif