#ifndef SUBSCRIBEMANAGER_H
#define SUBSCRIBEMANAGER_H

#include <WinSock2.h>
#include <unordered_map>
#include <list>
#include <string>

class SubscribeManager
{
private:
	static SubscribeManager *sm;
	std::unordered_map<std::string, std::list<SOCKET> *> sockets;
	SubscribeManager();
public:
	~SubscribeManager();
	static SubscribeManager *getInstance();
	void subscribe(char *dir, SOCKET sock);
	void unSubscribe(char *dir);
	std::list<SOCKET> *getSocketsByDir(char *dir);
};

#endif
