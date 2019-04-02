#include "ShowProcessor.h"

#include <iostream>

ShowProcessor::ShowProcessor()
{
}


ShowProcessor::~ShowProcessor()
{
}

void ShowProcessor::PacketProcess(SOCKET sock, char *msg)
{
	std::cout << "Show Processor" << std::endl;
	
}

