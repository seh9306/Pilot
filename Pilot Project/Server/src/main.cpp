#if defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#include <crtdbg.h>
#include <cstdlib>

#endif


#include "server/Server.h"

extern void error_handle(char *);

int main(int argc, char *args) 
{
#if defined(DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// create Server
	Server server;
	int8_t errorCode = server.Init(9030);

	// initialize server
	// 0 means success
	if (errorCode != 0)
	{
		switch (errorCode)
		{
		case kBindError:
			error_handle("bind() error !");
			exit(1);
			break;
		case kListenError:
			error_handle("listen() error !");
			exit(1);
			break;
		}
	}

	// start server
	// 0 means success
	errorCode = server.Start();
	if (errorCode != 0)
	{
		error_handle("Start() error !");
		exit(1);
	}

	return 0;
}