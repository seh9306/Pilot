#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <tchar.h>
#include <cstdio>
#include <cstring>

#include "FileManager.h"

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

FileManager& FileManager::GetInstance() 
{
	static FileManager fileManager;
	return fileManager;
}

std::list<WIN32_FIND_DATA>* FileManager::GetFileList(char *dir) 
{
	LARGE_INTEGER filesize;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	// use new operator
	std::list<WIN32_FIND_DATA> *files = new std::list<WIN32_FIND_DATA>();
	
	hFind = FindFirstFile(_T(strcat(dir, "*.*")), &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return nullptr;
	}

	do
	{
		files->push_back(FindFileData);
	} while (FindNextFile(hFind, &FindFileData));

	FindClose(hFind);

	return files;
}


