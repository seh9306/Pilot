#ifndef PACKETPROCESSOR_H
#define PACKETPROCESSOR_H

#include <WinSock2.h>

#include "FileAgentView.h"

#define NUM_OF_PACKET_PROCESSOR 5

#define SUB_HEADER_SIZE 5
#define DELETE_HEADER_SIZE 10
#define SHOW_HEADER_SIZE 9
#define RENAME_HEADER_SIZE 13
#define MOVE_HEADER_SIZE 13

#define PROTOCOL_TYPE_SIZE 1
#define FILE_TYPE_SIZE 1
#define WIN_FIND_DATA_FRONT_SIZE 36
#define NULL_VALUE_SIZE 1

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
	CFileAgentView* cFileAgentView;
};

#endif