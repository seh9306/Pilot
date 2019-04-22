#ifndef PUBMANAGER_H
#define PUBMANAGER_H

#include <WinSock2.h>
#include <list>

struct AsyncIOBuffer;

class PublishManager
{
public:
	virtual ~PublishManager();

	static PublishManager& GetInstance();

	AsyncIOBuffer* CreateData(char* msg, int size);
	bool Publish(char* msg, std::list<SOCKET>& socks, int size);
	void Publish(char* msg, SOCKET sock, int size);

private:
	PublishManager();
};

#endif