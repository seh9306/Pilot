#ifndef SUBMANAGER_H
#define SUBMANAGER_H

#include <unordered_map>
#include <list>
#include <string>

#ifndef _WINSOCK2API_

typedef _W64 unsigned int SOCKET;

#endif

using std::unordered_map;
using std::list;
using std::string;
/*
typedef struct SUBSCRIPTION_NODE{
	_W64 unsigned int sock;
	struct SUBSCRIPTION_NODE *next;
}SUB_SOCKET_NODE, *LPSUB_SOCKET_NODE;
*/
class SubManager
{
private:
	static SubManager *sm;
	unordered_map<string, list<SOCKET> *> sockets;
	SubManager();
public:
	~SubManager();
	static SubManager *getInstance();
	void subscribe(char *dir, SOCKET sock);
	void unSubscribe(char *dir);
	list<SOCKET> *getSocketsByDir(char *dir);
};

#endif
