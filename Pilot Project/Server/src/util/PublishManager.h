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
	bool Publish(char *dir, std::list<SOCKET> socks);
	void sSubscribe(char *msg, SOCKET sock);
	void fSubscribe(char *dir, SOCKET sock);
private:
	PublishManager();
};

#endif