#include "PublishManager.h"
#include "FileManager.h"
#include "../Server/NetworkData.h"
#include "../PacketProcessor/PacketProcessor.h"

#define PM_DEBUG

#ifdef PM_DEBUG
#include <iostream>
#endif

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

	std::list<WIN32_FIND_DATA>* files = fileManager.GetFileList(dir + SUB_HEADER_SIZE + sizeof(DWORD));

	if (files == nullptr) 
	{
		// send fail
		return false;
	}

	int listSize = files->size();
	
	DWORD showNumber;
	memcpy(&showNumber, dir + PROTOCOL_TYPE_SIZE, sizeof(DWORD));
	
	WSABUF wsaBuf;
	wsaBuf.buf = dir;
	wsaBuf.len = 0;
	DWORD length = 0;

	// copy protocol type
	wsaBuf.buf[0] = kShow;
	wsaBuf.len += PROTOCOL_TYPE_SIZE;

	// copy list size
	memcpy(wsaBuf.buf
		+ wsaBuf.len, &listSize, sizeof(int));
	wsaBuf.len += sizeof(int);

	// @send
	if (WSASend(sock, &wsaBuf, 1,
		&length, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			std::cout << "WSASend() error ::: " << GetLastError() << std::endl;
		return false;
	}

	int bufOffset = 0;
	wsaBuf.buf[0] = kShowAdd;
	bufOffset = 0 + PROTOCOL_TYPE_SIZE;
	memcpy(wsaBuf.buf + bufOffset, &showNumber, sizeof(DWORD));
	bufOffset += sizeof(DWORD);

	for (WIN32_FIND_DATA file : *files)
	{
		int fileNameSize = strlen(file.cFileName);

		int size = WIN_FIND_DATA_FRONT_SIZE
			+ fileNameSize
			+ sizeof(fileNameSize)
			+ NULL_VALUE_SIZE;
		
		if (size + bufOffset > BUFSIZE - 2 ) 
		{
			wsaBuf.len = 1024; //bufOffset;
			wsaBuf.buf[bufOffset] = '\n';

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
				bufOffset = 0 + PROTOCOL_TYPE_SIZE + sizeof(DWORD);
			}
		}

		// copy file data front
		memcpy(wsaBuf.buf
			+ bufOffset, &file, WIN_FIND_DATA_FRONT_SIZE);
		bufOffset += WIN_FIND_DATA_FRONT_SIZE;
		
		// copy file name size
		memcpy(wsaBuf.buf
			+ bufOffset, &fileNameSize, sizeof(int));
		bufOffset += sizeof(int);

		// copy file name
		memcpy(wsaBuf.buf
			+ bufOffset, file.cFileName, fileNameSize + NULL_VALUE_SIZE);
		//std::cout << wsaBuf.buf + bufOffset << ":::" << bufOffset << std::endl;
		bufOffset += fileNameSize + NULL_VALUE_SIZE;
		
	}

	if (bufOffset != NULL)
	{
		wsaBuf.len = 1024; //bufOffset;
		wsaBuf.buf[bufOffset] = '\n';

		// publish
		if (WSASend(sock, &wsaBuf, 1,
			&length, 0, NULL, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				std::cout << "WSASend() error :: " << GetLastError() << std::endl;
			return false;
		}
	}

	return true;
}

bool PublishManager::Publish(char * dir, std::list<SOCKET> socks)
{
	return false;
}

void PublishManager::sSubscribe(char * msg, SOCKET sock)
{
	WSABUF wsaBuf;

	wsaBuf.buf = msg;
	DWORD length = 0;

	int dirLength = strlen(msg + SUB_HEADER_SIZE) + NULL_VALUE_SIZE;
	// copy protocol type
	wsaBuf.buf[0] = kSubscribe;
	wsaBuf.buf[1] = true;

	wsaBuf.len = 2;
	memcpy(wsaBuf.buf + wsaBuf.len + sizeof(int), msg + SUB_HEADER_SIZE, dirLength);
	wsaBuf.len += dirLength;

	memcpy(wsaBuf.buf + PROTOCOL_TYPE_SIZE + 1, &dirLength, sizeof(int));
	wsaBuf.len += sizeof(int);

#ifdef PM_DEBUG
	std::cout << "PM_DEBUG :: " << wsaBuf.buf + SUB_HEADER_SIZE + sizeof(int) << std::endl;
#endif

	if (WSASend(sock, &wsaBuf, 1,
		&length, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			std::cout << "WSASend() error :: " << GetLastError() << std::endl;
	}
}

// @TODO
void PublishManager::fSubscribe(char * dir, SOCKET sock)
{
	WSABUF wsaBuf;

	wsaBuf.buf = dir;
	DWORD length = 0;

	// copy protocol type
	wsaBuf.buf[0] = kSubscribe;
	wsaBuf.buf[1] = false;
	wsaBuf.len = 2;

	if (WSASend(sock, &wsaBuf, 1,
		&length, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			std::cout << "WSASend() error :: " << GetLastError() << std::endl;
	}
}
