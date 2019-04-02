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
	LPPER_HANDLE_DATA perHandleData; // �� Ŭ���̾�Ʈ�� ���� ����?
	LPPER_IO_DATA perIoData; // �� Ŭ���̾�Ʈ�� ���� ����?
	DWORD flags; // (?)
};

#endif