#include "PacketProcessor.h"

PacketProcessor::PacketProcessor()
{
	subscribeManager	= &SubscribeManager::GetInstance();
	publishManager		= &PublishManager::GetInstance();
	fileManager			= &FileManager::GetInstance();
}

void PacketProcessor::ReadMessage(void* dest, char* src, int size, int& offset)
{
	memcpy(dest, src + offset, size);
}

void PacketProcessor::ReadLength(uint32_t* dest, char* src, int size, int& offset)
{
	ReadMessage(dest, src, size, offset);
}

void PacketProcessor::ReadLengthWithAddingSize(uint32_t* dest, char* src, int size, int& offset)
{
	ReadMessage(dest, src, size, offset);
	offset += size;
}

void PacketProcessor::ReadLengthWithAddingLengthAndSize(uint32_t* dest, char* src, int size, int& offset)
{
	ReadMessage(dest, src, size, offset);
	offset += size + *dest;
}

void PacketProcessor::WriteMessage(char* dest, void* src, int size, int& offset)
{
	memcpy(dest + offset, src , size);
}

void PacketProcessor::WriteMessageWithAddingSize(char* dest, void* src, int size, int& offset)
{
	WriteMessage(dest, src, size, offset);
	offset += size;
}

void PacketProcessor::WriteLengthWithAddingSize(char* dest, uint32_t* src, int size, int& offset)
{
	WriteMessage(dest, src, size, offset);
	offset += size;
}