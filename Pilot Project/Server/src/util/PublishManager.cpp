#include "PublishManager.h"

#include <iostream>

PublishManager *PublishManager::pm = NULL;

PublishManager::PublishManager()
{
}

PublishManager::~PublishManager()
{
}

PublishManager * PublishManager::getInstance()
{
	return nullptr;
}

void PublishManager::publish(char * dir, SOCKET sock)
{
	FileManager *fm = FileManager::getInstance();
	list<WIN32_FIND_DATA> *files = fm->getFileList(dir);
	std::cout << "size :: " << sizeof(WIN32_FIND_DATA) << std::endl;
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

void PublishManager::publish(char * dir, list<SOCKET> socks)
{
}
