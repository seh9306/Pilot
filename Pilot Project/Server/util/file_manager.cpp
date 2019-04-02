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

	printf("%-30s\t", "파일 이름");
	printf("파일 속성\t");
	printf("생성 일자\t");
	printf("접근 일자\t");
	printf("쓰기 일자\t");
	printf("파일 큼\t");
	printf("파일 작음\n");
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


