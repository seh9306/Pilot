#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <Windows.h>
#include <list>

class FileManager
{
private:
	static FileManager *fm;
	FileManager();
public:
	~FileManager();
	static FileManager *getInstance();
	std::list<WIN32_FIND_DATA> *getFileList(char *dir);
};

#endif

