#include "PacketProcessor.h"

PacketProcessor::PacketProcessor()
{
	subscribeManager	= &SubscribeManager::GetInstance();
	publishManager		= &PublishManager::GetInstance();
	fileManager			= &FileManager::GetInstance();
}