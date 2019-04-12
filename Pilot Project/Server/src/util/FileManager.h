#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <Windows.h>
#include <list>

class FileManager
{
public:
	~FileManager();
	static FileManager& GetInstance();
	wchar_t * GetLogicalDriveStringsW();
	DWORD GetNumberOfDrives();
	std::list<WIN32_FIND_DATA>* GetFileList(char* dir);
	bool DeleteFileByFileName(char* dir, char* fileName, char type);
	bool Rename(char* dir, char* oldName, char* newName);
	bool Move(char* dir, char* oldName, char* newName);
private:
	FileManager();
	wchar_t logicalDriveStrings[40];
	DWORD numberOfDrives;
};

#endif

