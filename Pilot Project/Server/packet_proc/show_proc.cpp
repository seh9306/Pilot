#include "show_proc.h"

#include <iostream>

ShowProc::ShowProc()
{
}


ShowProc::~ShowProc()
{
}

void ShowProc::packetProc(SOCKET sock, char *msg)
{
	std::cout << "Show Processor" << std::endl;
	
}

