#include "ConnectProcessor.h"
#include "SubscribeProcessor.h"

ConnectProcessor::ConnectProcessor()
{
}

ConnectProcessor::~ConnectProcessor()
{
}

void ConnectProcessor::ProcessPacket(SOCKET sock, char * msg)
{
	int offset = PROTOCOL_TYPE_SIZE;
	void* address = nullptr;

	msg[kProtocolHeaderIndex] = kConnect;

	uint32_t numberOfDrives = (uint32_t)fileManager->GetNumberOfDrives();
	WriteLengthWithAddingSize(msg, &numberOfDrives, sizeof(uint32_t), offset);

	wchar_t* pLogicalDriveStrings = fileManager->GetLogicalDriveStringsW();

	for (int i = 0; i < numberOfDrives; i++)
	{
		address = pLogicalDriveStrings + driverStringLength * i;
		WriteMessageWithAddingSize(msg, address, sizeof(wchar_t) * driverStringLength, offset);

		uint32_t type = GetDriveTypeW((LPCWSTR)address);
		WriteMessageWithAddingSize(msg, &type, sizeof(uint32_t), offset);
	}
	
	publishManager->Publish(msg, sock, offset);
}
