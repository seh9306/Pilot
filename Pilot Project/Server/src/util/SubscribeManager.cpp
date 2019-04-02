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
	
	auto search = sockets.find(dirString);

	if (search != sockets.end()) 
	{
		search->second->push_back(sock);
	}
	else 
	{
		std::cout << "NULL" << std::endl;
		std::list<SOCKET> *temp = new std::list<SOCKET>();
		temp->push_back(sock);
		sockets.insert({ dirString, temp });
	}

	return true;
}

bool SubscribeManager::UnSubscribe(char *dir) 
{
	std::string dirString(dir);
	return false;
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