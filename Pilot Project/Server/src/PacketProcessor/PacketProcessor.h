#ifndef PACKETPROCESSOR_H
#define PACKETPROCESSOR_H

#include <WinSock2.h>

#include "Util\PublishManager.h"
#include "Util\FileManager.h"
#include "Util\SubscribeManager.h"

const int kProtocolHeaderIndex = 0;

// protocol header
const int kSubHeaderSize = 5;
const int kDeleteHeaderSize = 10;
const int kShowHeaderSize = 9;
const int kRenameHeaderSize = 13;
const int kMoveHeaderSize = 13;
const int kSuccessSubscribeHeaderSize = 2;

const int kProtocolTypeSize = 1;
const int kFileTypeSize = 1;
const int kWinFindDataFrontSize = 36;
const int kNullValueSize = 1;

// delete process
const int kFileTypeIndex = 1;

const int kFileTypeNormal = 0;
const int kFileTypeDirectory = 1;

enum protocolType
{
	kSubscribe = 0,
	kUnSubscribe = 1,
	kShow,
	kShowAdd,
	kCopy,
	kRename,
	kDelete,
	kMove,
	kConnect	
};

class PacketProcessor
{
public:
	PacketProcessor();
	virtual ~PacketProcessor() {};
	
	virtual void ProcessPacket(SOCKET sock, char *msg) = 0;
	void ReadMessage(void* dest, char* src, int size, int& offSet);
	void ReadLength(uint32_t * dest, char * src, int size, int & offSet);
	void ReadLengthWithAddingSize(uint32_t * dest, char * src, int size, int & offset);
	void ReadLengthWithAddingLengthAndSize(uint32_t * dest, char * src, int size, int & offSet);
	void WriteMessage(char * dest, void * src, int size, int & offset);
	void WriteMessageWithAddingSize(char * dest, void * src, int size, int & offset);
	void WriteLengthWithAddingSize(char * dest, uint32_t * src, int size, int & offset);
protected:
	SubscribeManager* subscribeManager;
	PublishManager* publishManager;
	FileManager* fileManager;
};

#endif