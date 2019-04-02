#ifndef FILEMANAGER_H
#define FILEMANAGER_H

class FileManager
{
private:
	static FileManager *fm;
	FileManager();
public:
	~FileManager();
	static FileManager *getInstance();
	char * getFileList(char *dir);
};

#endif

