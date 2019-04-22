#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>

#include "FileManager.h"

FileManager::FileManager()
{
	numberOfDrives = ::GetLogicalDriveStringsW(40, logicalDriveStrings) / 4;
}

FileManager::~FileManager()
{
}

FileManager& FileManager::GetInstance() 
{
	static FileManager fileManager;
	return fileManager;
}

bool FileManager::DeleteFileByFileName(char* dir, char* fileName, char fileType)
{
	char* temp = new char[strlen(dir) + strlen(fileName) + 1];

	temp[0] = 0;

	strcat(temp, dir);
	strcat(temp, fileName);

	if (fileType == 1) // FILE_TYPE_DIRECTORY
	{
		// @issue recursion delete
		bool result = RemoveDirectory(temp);

		delete[]temp;
		return result;
	}
	else if (fileType == 0) // FILE_TYPE_NORMAL
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

std::list<WIN32_FIND_DATA>* FileManager::GetFileList(char *dir)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	if (dir[strlen(dir)] == '\\')
	{
		hFind = FindFirstFile(TEXT(strcat(dir, "*.*")), &FindFileData);
	}
	else
	{
		hFind = FindFirstFile(TEXT(strcat(dir, "\\*.*")), &FindFileData);
	}

	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return nullptr;
	}

	// use new operator
	std::list<WIN32_FIND_DATA> *files = new std::list<WIN32_FIND_DATA>();

	do
	{
		files->push_back(FindFileData);
	} while (FindNextFile(hFind, &FindFileData));

	FindClose(hFind);

	return files;
}

wchar_t * FileManager::GetLogicalDriveStringsW()
{
	return logicalDriveStrings;
}

DWORD FileManager::GetNumberOfDrives()
{
	return numberOfDrives;
}

bool FileManager::Move(char* dir, char* fileName, char* newFileDir)
{
	char* absOldPath = new char[strlen(dir) + strlen(fileName) + 1];
	char* absNewPath = new char[strlen(newFileDir) + strlen(fileName) + 2];

	absOldPath[0] = 0;
	absNewPath[0] = 0;

	strcat(absOldPath, dir);
	strcat(absOldPath, fileName);

	strcat(absNewPath, newFileDir);
	strcat(absNewPath, fileName);

	int result = MoveFile(absOldPath, absNewPath);

	delete[] absOldPath;
	delete[] absNewPath;
	
	if (result == -1)
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool FileManager::Rename(char* dir, char* oldName, char* newName)
{
	char* absOldPath = new char[strlen(dir) + strlen(oldName) + 1];
	char* absNewPath = new char[strlen(dir) + strlen(newName) + 1];

	absOldPath[0] = 0;
	absNewPath[0] = 0;

	strcat(absOldPath, dir);
	strcat(absOldPath, oldName);

	strcat(absNewPath, dir);
	strcat(absNewPath, newName);

	int result = rename(absOldPath, absNewPath);

	delete[] absOldPath;
	delete[] absNewPath;

	if (result == -1)
	{
		return false;
	}
	else
	{
		return true;
	}

}