#include "sub_manager.h"

#include <iostream>

SubManager *SubManager::sm = NULL;

SubManager::SubManager()
{
}

SubManager::~SubManager()
{
}

SubManager *SubManager::getInstance() {
	if (!SubManager::sm) {
		SubManager::sm = new SubManager();
	}
	return SubManager::sm;
}

void SubManager::subscribe(char *dir, SOCKET sock) {
	std::string DIR(dir);
	
	auto search = sockets.find(DIR);

	if (search != sockets.end()) {
		search->second->push_back(sock);
	}
	else {
		std::cout << "NULL" << std::endl;
		list<SOCKET> *temp = new list<SOCKET>();
		temp->push_back(sock);
		sockets.insert({ DIR, temp });
	}
}

void SubManager::unSubscribe(char *dir) {
	std::string DIR(dir);
}

list<SOCKET> *SubManager::getSocketsByDir(char *dir) {
	std::string DIR(dir);

	auto search = sockets.find(DIR);

	if (search != sockets.end()) {
		return search->second;
	}

	return NULL;
}