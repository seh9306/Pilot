#ifndef PUBPROC_H
#define PUBPROC_H

#include "../util/file_manager.h"

class PubProc
{
public:
	PubProc();
	~PubProc();
protected:
	FileManager *fm;
};

#endif