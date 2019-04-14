#ifndef FILEAGENTSOCKET_H
#define FILEAGENTSOCKET_H

#include <WinSock2.h>
#include <vector>
#include "NetworkData.h"
#include "PacketProcessor.h"

class CFileAgentView;

class FileAgentSocket
{
public:
	virtual ~FileAgentSocket();
	static FileAgentSocket* GetInstance();
	static void releaseInstance();
	SOCKET& GetSocket();
	void CloseSocket();
	void Connect(char* ip, int port);
	void SendConnectionMessage();
	void Subscribe(char* dir);
	void UnSubscribe(char* dir);
	void Show(char* dir, DWORD showNumber);
	void Update();
	void Delete(char* dir, char* fileName, char attribute);
	void Rename(char* dir, char* oldName, char* newName);
	void Move(char* dir, char* fileName, char* goal, WIN32_FIND_DATA & file);
private:
	FileAgentSocket();
	static FileAgentSocket* fileAgent;
	HANDLE hCompletionPort; // Handle of Completion Port
	SOCKET fileAgentSocket;
	WSADATA wsaData;
	sockaddr_in srv_addr;
	WSABUF dataBuf;
	int sendBytes = 0;
	char buffer[1024];
	bool cleanSocket;

	CString stringTableValue;

	PER_HANDLE_DATA *perHandleData;
	PER_IO_DATA *perIoData;
	DWORD flags;
	DWORD recvBytes;

	std::vector<PacketProcessor *> packetProcessors;
};

#endif