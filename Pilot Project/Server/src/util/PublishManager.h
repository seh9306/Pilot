#ifndef PUBMANAGER_H
#define PUBMANAGER_H

#include <WinSock2.h>
#include <list>

#include "FileManager.h"

using std::list;

class PublishManager
{
private:
	static PublishManager *pm;
	FileManager *fm;
	PublishManager();
public:
	virtual ~PublishManager();
	static PublishManager *getInstance();
	void publish(char *dir, SOCKET sock);
	void publish(char *dir, list<SOCKET> socks);
};

#endif