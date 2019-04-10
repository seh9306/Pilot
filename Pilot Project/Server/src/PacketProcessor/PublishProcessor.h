#ifndef PUBPROCESSOR_H
#define PUBPROCESSOR_H

#include "Util/FileManager.h"

class PublishProcessor
{
public:
	PublishProcessor();
	~PublishProcessor();
protected:
	FileManager *fm;
};

#endif