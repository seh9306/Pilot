#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <tchar.h>
#include <cstdio>
#include <cstring>

#include "FileManager.h"

FileManager *FileManager::fm = NULL;

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

FileManager *FileManager::getInstance() {
	if (!FileManager::fm) {
		FileManager::fm = new FileManager();
	}

	return FileManager::fm;
}

std::list<WIN32_FIND_DATA> *FileManager::getFileList(char *dir) {
	LARGE_INTEGER filesize;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	std::list<WIN32_FIND_DATA> *files = new std::list<WIN32_FIND_DATA>();
	
	hFind = FindFirstFile(_T(strcat(dir, "*.*")), &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return NULL;
	}

	do
	{
		files->push_back(FindFileData);
	} while (FindNextFile(hFind, &FindFileData));

	FindClose(hFind);

	return files;
}


