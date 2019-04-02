#include <Windows.h>
#include <tchar.h>
#include <cstdio>

#include "file_manager.h"

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

char * FileManager::getFileList(char *dir) {
	LARGE_INTEGER filesize;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(_T("c:\\*.*"), &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return 0;
	}

	printf("%-30s\t", "���� �̸�");
	printf("���� �Ӽ�\t");
	printf("���� ����\t");
	printf("���� ����\t");
	printf("���� ����\t");
	printf("���� ŭ\t");
	printf("���� ����\n");
	do
	{
		printf("%-30s\t", FindFileData.cFileName);
		printf("%lu\t\t", FindFileData.dwFileAttributes);
		printf("%lu\t", FindFileData.ftCreationTime.dwHighDateTime);
		printf("%lu\t", FindFileData.ftLastAccessTime.dwLowDateTime);
		printf("%lu\t", FindFileData.ftLastWriteTime.dwHighDateTime);
		printf("%lu\t", FindFileData.nFileSizeHigh);
		printf("%lu\n", FindFileData.nFileSizeLow);

	} while (FindNextFile(hFind, &FindFileData));
	FindClose(hFind);

	return "y";
}


