#ifndef SUBSCRIBEMANAGER_H
#define SUBSCRIBEMANAGER_H

#include <WinSock2.h>
#include <unordered_map>
#include <list>
#include <string>
#include <memory>

class SubscribeManager
{
public:
	~SubscribeManager();

	static SubscribeManager& GetInstance();

	std::string GetDirBySocket(SOCKET sock);
	std::list<SOCKET>* GetSocketsByDir(char* dir);
	
	bool Subscribe(char* dir, SOCKET sock);
	bool UnSubscribe(SOCKET sock);
	bool UnSubscribe(char* dir, SOCKET sock);
	
private:
	std::unordered_map<std::string, std::unique_ptr<std::list<SOCKET>>> sockets;
	std::unordered_map<SOCKET, std::string> dirs;
	SubscribeManager();
};

#endif


