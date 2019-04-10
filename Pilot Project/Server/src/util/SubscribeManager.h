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
	std::unordered_map<SOCKET, std::string> dirs;
	SubscribeManager();
public:
	~SubscribeManager();
	static SubscribeManager& GetInstance();
	bool Subscribe(char *dir, SOCKET sock);
	bool UnSubscribe(char *dir, SOCKET sock);
	std::list<SOCKET> *GetSocketsByDir(char *dir);
	std::string GetDirBySocket(SOCKET sock);
};

#endif


