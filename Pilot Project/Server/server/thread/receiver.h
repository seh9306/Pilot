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
	LPPER_HANDLE_DATA PerHandleData; // �� Ŭ���̾�Ʈ�� ���� ����?
	LPPER_IO_DATA PerIoData; // �� Ŭ���̾�Ʈ�� ���� ����?
	DWORD flags; // (?)
};

#endif