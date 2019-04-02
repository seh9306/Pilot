#include "UnSubscribeProcessor.h"

#include <iostream>

UnSubscribeProcessor::UnSubscribeProcessor()
{
}


UnSubscribeProcessor::~UnSubscribeProcessor()
{
}


void UnSubscribeProcessor::PacketProcess(SOCKET sock, char *msg)
{
	std::cout << "Unsub Processor" << std::endl;
}
