#ifndef RECEIVER_H
#define RECEIVER_H

#include <WinSock2.h>
#include "network_data.h"

class Receiver {
public:
	//void run(LPVOID pComPort);
	void operator()(HANDLE pComPort);
	int id;
private:
	HANDLE hCompletionPort;
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData; // �� Ŭ���̾�Ʈ�� ���� ����?
	LPPER_IO_DATA PerIoData; // �� Ŭ���̾�Ʈ�� ���� ����?
	DWORD flags; // (?)
};

#endif