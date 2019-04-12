#include "SubscribeManager.h"

#include <iostream>

// @issue
#define SM_DEBUG

SubscribeManager::SubscribeManager()
{
}

SubscribeManager::~SubscribeManager()
{
}

SubscribeManager& SubscribeManager::GetInstance() 
{
	static SubscribeManager subscribeManager;
	return subscribeManager;
}

bool SubscribeManager::Subscribe(char *dir, SOCKET sock) 
{
	std::string dirString(dir);
	std::cout << dirString << "을 구독하셨습니다."<<std::endl;
	auto search = sockets.find(dirString);

	if (search != sockets.end()) 
	{
		search->second->push_back(sock);
		std::cout << "구독자 수 : " << search->second->size() << std::endl;
	}
	else 
	{
		std::list<SOCKET> *temp = new std::list<SOCKET>();
		temp->push_back(sock);
		sockets.insert({ dirString, temp });
	}

	dirs.insert({ sock, dirString });

	return true;
}

bool SubscribeManager::UnSubscribe(char * dir, SOCKET sock)
{
	std::string dirString(dir);

	auto search = sockets.find(dirString);

	if (search != sockets.end())
	{
		std::list<SOCKET>* temp = search->second;
		std::list<SOCKET>::iterator it;
		
		for (it = temp->begin(); it != temp->end(); it++)
		{
			if (*it == sock) 
			{
				temp->erase(it);
				break;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

std::list<SOCKET> *SubscribeManager::GetSocketsByDir(char *dir) 
{
	std::string dirString(dir);

	auto search = sockets.find(dirString);

	if (search != sockets.end()) 
	{
		return search->second;
	}

	return nullptr;
}

std::string SubscribeManager::GetDirBySocket(SOCKET sock)
{
	auto search = dirs.find(sock);

	if (search != dirs.end())
	{
		return search->second;
	}

	return nullptr;
}