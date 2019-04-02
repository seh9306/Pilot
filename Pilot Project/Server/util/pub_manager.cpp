#include "pub_manager.h"

#include <iostream>

PubManager::PubManager()
{
}

PubManager::~PubManager()
{
}

PubManager * PubManager::getInstance()
{
	return nullptr;
}

void PubManager::publish(char * dir, SOCKET sock)
{
	FileManager *fm = FileManager::getInstance();
	std::cout << fm->getFileList(dir) << std::endl;
	/*WSABUF temp;
	temp.buf = "aaaa";
	temp.len = 5;
	// publish
	if (WSASend(sock, &temp, 1,
		NULL, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			std::cout << "WSASend() error" << std::endl;
	}
	*/
}

void PubManager::publish(char * dir, list<SOCKET> socks)
{
}
