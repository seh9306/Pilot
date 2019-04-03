#include "PublishManager.h"
#include "FileManager.h"
#include "../Server/NetworkData.h"
#include "PacketProcessor\PacketProcessor.h"

#include <iostream>
#include <WinSock2.h>

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

	std::list<WIN32_FIND_DATA>* files = fileManager.GetFileList(dir);

	int listSize = files->size();
	WSABUF wsaBuf;
	int bufOffset = 0;

	//char *temp = new char[sizeof(WIN32_FIND_DATA)];
	char buf[1024];
	wsaBuf.buf = buf;
	DWORD length = 0;

	// copy protocol type
	wsaBuf.buf[0] = kShow;
	bufOffset += PROTOCOL_TYPE_SIZE;

	// copy list size
	memcpy(wsaBuf.buf
		+ bufOffset, &listSize, sizeof(int));
	bufOffset += sizeof(int);
	
	for (WIN32_FIND_DATA file : *files)
	{
		int fileNameSize = strlen(file.cFileName);

		int size = WIN_FIND_DATA_FRONT_SIZE
			+ fileNameSize
			+ NULL_VALUE_SIZE;

		if (size + bufOffset > BUFSIZE - 1) {
			wsaBuf.len = bufOffset;
			// publish
			if (WSASend(sock, &wsaBuf, 1,
				&length, 0, NULL, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
					std::cout << "WSASend() error :: " << GetLastError() << std::endl;
				return false;
			}
			else 
			{
				bufOffset = 0;
			}
		}
		
		// copy file data front
		memcpy(wsaBuf.buf
			+ bufOffset, &file, WIN_FIND_DATA_FRONT_SIZE);
		bufOffset += WIN_FIND_DATA_FRONT_SIZE;
		
		// copy file name size
		memcpy(wsaBuf.buf
			+ bufOffset, &fileNameSize, sizeof(int));
		//printf("%d // \n", fileNameSize);
		bufOffset += sizeof(int);

		// copy file name
		memcpy(wsaBuf.buf
			+ bufOffset, file.cFileName, fileNameSize + NULL_VALUE_SIZE);
		bufOffset += fileNameSize + NULL_VALUE_SIZE;
		
	}

	wsaBuf.len = bufOffset;
	// publish
	if (WSASend(sock, &wsaBuf, 1,
		&length, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			std::cout << "WSASend() error :: " << GetLastError() << std::endl;
		return false;
	}

	return true;
}

bool PublishManager::Publish(char * dir, std::list<SOCKET> socks)
{
	return false;
}
