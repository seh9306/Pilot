#ifndef PUBPROCESSOR_H
#define PUBPROCESSOR_H

#include "../util/FileManager.h"

class PublishProcessor
{
public:
	PublishProcessor();
	~PublishProcessor();
protected:
	FileManager *fm;
};

#endif