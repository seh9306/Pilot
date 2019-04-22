#include "SubscribeManager.h"

#include <iostream>


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
	std::cout << "Subscribe " << dirString << std::endl;
	auto search = sockets.find(dirString);

	if (search != sockets.end()) 
	{
		search->second->push_back(sock);
		std::cout << "Number of subscriber : " << search->second->size() << std::endl;
	}
	else 
	{
		// new operator
		std::list<SOCKET> *socketsPerDir = new std::list<SOCKET>();
		socketsPerDir->push_back(sock);

		std::unique_ptr<std::list<SOCKET>> pList(std::move(socketsPerDir));
		
		sockets.insert({ dirString, std::move(pList) });
	}

	dirs.insert({ sock, dirString });

	return true;
}

bool SubscribeManager::UnSubscribe(SOCKET clientSocket)
{
	std::string dirString = GetDirBySocket(clientSocket);

	auto search = sockets.find(dirString);

	if (search != sockets.end())
	{
		if (search->second == nullptr)
		{
			return false;
		}
		
		auto it = search->second->begin();

		for (; it != search->second->end(); it++)
		{
			SOCKET s = *it;
			if (s == clientSocket)
			{
				search->second->erase(it);
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

bool SubscribeManager::UnSubscribe(char * dir, SOCKET sock)
{
	std::string dirString(dir);

	auto search = sockets.find(dirString);

	if (search != sockets.end())
	{
		if (search->second == nullptr)
		{
			return false;
		}
		auto it = search->second->begin();
		
		for (;it != search->second->end(); it++)
		{
			SOCKET s = *it;
			if (s == sock) 
			{
				search->second->erase(it);
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
		return search->second.get();
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

	return std::string("");
}