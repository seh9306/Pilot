#ifndef PACKETPROCESSOR_H
#define PACKETPROCESSOR_H

#define NUM_OF_PACKET_PROCESSOR 5

#define SUB_HEADER_SIZE 5

#define PROTOCOL_TYPE_SIZE 1
#define WIN_FIND_DATA_FRONT_SIZE 36
#define NULL_VALUE_SIZE 1

enum protocolType {
	kSubscribe = 0,
	kUnSubscribe = 1,
	kShow,
	kCreate,
	kRename,
	kDelete,
	kMove
};

#include <WinSock2.h>

class PacketProcessor
{
public:
	PacketProcessor() {};
	virtual ~PacketProcessor() {};
	
	virtual void PacketProcess(SOCKET sock, char *msg) = 0;
};

#endif