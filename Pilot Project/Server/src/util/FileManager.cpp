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
// @issue
#include <iostream>
bool FileManager::DeleteFileByFileName(char* dir, char* fileName, char type)
{
	char* temp = new char[strlen(dir) + strlen(fileName) + 1];

	temp[0] = 0;

	strcat(temp, dir);
	strcat(temp, fileName);

	if (type == 1)
	{
		// @issue recursion delete
		RemoveDirectory(temp);
	}
	else
	{
		HANDLE hFile;

		hFile = CreateFile(temp, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_DELETE_ON_CLOSE, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			delete[]temp;
			return false;
		}

		CloseHandle(hFile);
	}
	
	delete[]temp;
	return true;
}


