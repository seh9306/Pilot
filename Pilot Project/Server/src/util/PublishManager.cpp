#include "PublishManager.h"
#include "FileManager.h"

#include <iostream>

PublishManager::PublishManager()
{
}

PublishManager::~PublishManager()
{
}

PublishManager& PublishManager::GetInstance()
{
	static PublishManager publishManager;
	return publishManager;
}

bool PublishManager::Publish(char * dir, SOCKET sock)
{
	FileManager fileManager = FileManager::GetInstance();
	std::list<WIN32_FIND_DATA> *files = fileManager.GetFileList(dir);

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
	return true;
}

bool PublishManager::Publish(char * dir, std::list<SOCKET> socks)
{
	return false;
}
