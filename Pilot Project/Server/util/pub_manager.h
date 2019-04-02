#ifndef PUBMANAGER_H
#define PUBMANAGER_H

#include <WinSock2.h>
#include <list>

#include "file_manager.h"

using std::list;

class PubManager
{
private:
	static PubManager *pm;
	FileManager *fm;
	PubManager();
public:
	virtual ~PubManager();
	static PubManager *getInstance();
	void publish(char *dir, SOCKET sock);
	void publish(char *dir, list<SOCKET> socks);
};

#endif