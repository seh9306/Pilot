#include "unsub_proc.h"

#include <iostream>

UnsubProc::UnsubProc()
{
}


UnsubProc::~UnsubProc()
{
}


void UnsubProc::packetProc(char *msg)
{
	std::cout << "Unsub Processor" << std::endl;
}
