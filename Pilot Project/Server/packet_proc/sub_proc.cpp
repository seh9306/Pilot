#include "sub_proc.h"

#include <iostream>

SubProc::SubProc()
{
}


SubProc::~SubProc()
{
}

void SubProc::packetProc(char *msg)
{
	std::cout << "Sub Processor" << std::endl;
}

