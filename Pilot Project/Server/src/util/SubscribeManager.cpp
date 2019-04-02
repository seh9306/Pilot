#include "SubscribeManager.h"

#include <iostream>

SubscribeManager *SubscribeManager::sm = NULL;

SubscribeManager::SubscribeManager()
{
}

SubscribeManager::~SubscribeManager()
{
}

SubscribeManager *SubscribeManager::getInstance() {
	if (!SubscribeManager::sm) {
		SubscribeManager::sm = new SubscribeManager();
	}
	return SubscribeManager::sm;
}

void SubscribeManager::subscribe(char *dir, SOCKET sock) {
	std::string DIR(dir);
	
	auto search = sockets.find(DIR);

	if (search != sockets.end()) {
		search->second->push_back(sock);
	}
	else {
		std::cout << "NULL" << std::endl;
		std::list<SOCKET> *temp = new std::list<SOCKET>();
		temp->push_back(sock);
		sockets.insert({ DIR, temp });
	}
}

void SubscribeManager::unSubscribe(char *dir) {
	std::string DIR(dir);
}

std::list<SOCKET> *SubscribeManager::getSocketsByDir(char *dir) {
	std::string DIR(dir);

	auto search = sockets.find(DIR);

	if (search != sockets.end()) {
		return search->second;
	}

	return NULL;
}