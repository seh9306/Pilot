#ifndef PACKETPROCESSOR_H
#define PACKETPROCESSOR_H

#include <WinSock2.h>

#include "Util\PublishManager.h"
#include "Util\FileManager.h"
#include "Util\SubscribeManager.h"

#define NUM_OF_PACKET_PROCESSOR		5

// protocol header
#define SUB_HEADER_SIZE				5
#define DELETE_HEADER_SIZE			10
#define SHOW_HEADER_SIZE			9
#define RENAME_HEADER_SIZE			13
#define MOVE_HEADER_SIZE			13

#define PROTOCOL_TYPE_SIZE			1
#define FILE_TYPE_SIZE				1
#define WIN_FIND_DATA_FRONT_SIZE	36
#define NULL_VALUE_SIZE				1

// delete process
#define FILE_TYPE_INDEX				1

#define FILE_TYPE_NORMAL			0
#define FILE_TYPE_DIRECTORY			1

enum protocolType {
	kSubscribe = 0,
	kUnSubscribe = 1,
	kShow,
	kShowAdd,
	kCreate,
	kRename,
	kDelete,
	kMove
};

class PacketProcessor
{
public:
	PacketProcessor();
	virtual ~PacketProcessor() {};
	
	virtual void PacketProcess(SOCKET sock, char *msg) = 0;
protected:
	SubscribeManager* subscribeManager;
	PublishManager* publishManager;
	FileManager* fileManager;
};

#endif