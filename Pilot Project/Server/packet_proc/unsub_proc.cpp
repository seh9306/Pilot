#include "unsub_proc.h"

#include <iostream>

UnsubProc::UnsubProc()
{
}


UnsubProc::~UnsubProc()
{
}


void UnsubProc::packetProc(SOCKET sock, char *msg)
{
	std::cout << "Unsub Processor" << std::endl;
}
