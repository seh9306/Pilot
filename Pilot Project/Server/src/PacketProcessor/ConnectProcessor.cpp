#include "SubscribeProcessor.h"

#include <iostream>
#include "ConnectProcessor.h"

ConnectProcessor::ConnectProcessor()
{
}

ConnectProcessor::~ConnectProcessor()
{
}

void ConnectProcessor::PacketProcess(SOCKET sock, char * msg)
{
	msg[0] = kConnect;

	int len = PROTOCOL_TYPE_SIZE;

	int numberOfDrives = (int)fileManager->GetNumberOfDrives();
	memcpy(msg + len, &(numberOfDrives), sizeof(int));
	len += sizeof(int);

	wchar_t* pLogicalDriveStrings = fileManager->GetLogicalDriveStringsW();

	for (int i = 0; i < numberOfDrives; i++)
	{
		void* address = pLogicalDriveStrings + 4 * i;
		memcpy(msg + len, address, sizeof(wchar_t) * 4);
		len += sizeof(wchar_t) * 4;

		unsigned int type = GetDriveTypeW((LPCWSTR)address);

		memcpy(msg + len, &type, sizeof(unsigned int));
		len += sizeof(unsigned int);
	}
	
	publishManager->Publish(msg, sock, len);

}
