#include "sub_proc.h"
#include "..\util\sub_manager.h"
#include "..\util\pub_manager.h"

#include <iostream>

SubProc::SubProc()
{
}


SubProc::~SubProc()
{
}

// subcribe
void SubProc::packetProc(SOCKET sock, char *msg)
{
	SubManager *sm = SubManager::getInstance();
	PubManager *pm = PubManager::getInstance();

	int length;

	memcpy((void*)&length, msg + 1, 4);

	if (!msg || length > 261 + 4 + 1) // MAX_PATH 260..
		return;

	// subscribe directory
	sm->subscribe(msg + SUB_HEADER_SIZE, sock);

	// publish to sub
	pm->publish(msg + SUB_HEADER_SIZE, sock);

}

