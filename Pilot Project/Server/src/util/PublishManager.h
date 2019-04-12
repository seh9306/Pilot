#ifndef PUBMANAGER_H
#define PUBMANAGER_H

#include <WinSock2.h>
#include <list>

class PublishManager
{
public:
	virtual ~PublishManager();
	static PublishManager& GetInstance();
	bool Publish(char *dir, SOCKET sock);
	bool Publish(char * msg, std::list<SOCKET>& socks, int size);
	void Publish(char* msg, SOCKET sock, int size);
	void fSubscribe(char* dir, SOCKET sock);
private:
	PublishManager();
};

#endif