#ifndef RECEIVER_H
#define RECEIVER_H

#include <WinSock2.h>
#include <vector>

#include "..\..\network_data.h"
#include "..\..\packet_proc\packet_proc.h"

using std::vector;

class Receiver {
public:
	//void run(LPVOID pComPort);
	void operator()(HANDLE pComPort, vector<PacketProc *> *packetProcs);
	int id;
private:
	HANDLE hCompletionPort;
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData; // 각 클라이언트의 소켓 정보?
	LPPER_IO_DATA PerIoData; // 각 클라이언트의 버퍼 정보?
	DWORD flags; // (?)
};

#endif